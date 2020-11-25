#
# This file is part of the Coriolis Software.
# Copyright (c) SU 2020-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/bigvia.py"                |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import os.path
import Cfg
from   Hurricane import Breakpoint, DbU, Box, Transformation, Point, \
                        Box, Path, Layer, Occurrence, Net,           \
                        NetExternalComponents, RoutingPad, Pad,      \
                        Horizontal, Vertical, Contact, Pin, Plug,    \
                        Instance
import CRL
from   CRL             import RoutingLayerGauge
from   helpers         import trace, dots
from   helpers.io      import ErrorMessage, WarningMessage, catch
from   helpers.overlay import UpdateSession

rg = None


# ----------------------------------------------------------------------------
# Class  :  "bigvia.BigVia".

class BigVia ( object ):
    """
    Draw a large are VIA and manage the matrix of cuts.
    """

    def __init__ ( self, net, depth, x, y, width, height ):
        self.hasLayout   = False
        self.net         = net
        self.bottomDepth = depth
        self.topDepth    = depth
        self.x           = x
        self.y           = y
        self.width       = width
        self.height      = height
        self.plates      = {}
        self.vias        = {}

    def __str__ ( self ):
        global rg
        if rg is None: rg = CRL.AllianceFramework.get().getRoutingGauge()
        return '<BigVia {} [{} {}] @({} {}) {} x {}>' \
               .format( self.net.getName()
                      , rg.getRoutingLayer(self.bottomDepth).getName()
                      , rg.getRoutingLayer(self.topDepth).getName()
                      , DbU.getValueString(self.x)
                      , DbU.getValueString(self.y)
                      , DbU.getValueString(self.width)
                      , DbU.getValueString(self.height) )

    def getNet ( self ): return self.net

    def getPlate ( self, metal ):
        if not self.hasLayout: return None
        for plate in self.plates.values():
            if plate.getLayer().contains(metal): return plate
        return None

    def mergeDepth ( self, depth ):
        if self.hasLayout:
            print( WarningMessage( 'BigVia.mergeDepth(): Cannot be called *after* BigVia.doLayout()' ))
            return
        if depth < self.bottomDepth: self.bottomDepth = depth
        if depth > self.topDepth:    self.topDepth    = depth

    def doLayout ( self ):
        global rg
        if rg is None: rg = CRL.AllianceFramework.get().getRoutingGauge()
        for depth in range(self.bottomDepth,self.topDepth+1):
            minArea   = rg.getRoutingLayer( depth ).getMinimalArea()
            minLength = DbU.fromPhysical( minArea / DbU.toPhysical( self.width, DbU.UnitPowerMicro )
                                        , DbU.UnitPowerMicro )
            #minLength = toFoundryGrid( minLength, DbU.SnapModeSuperior )
            plateArea = DbU.toPhysical( self.width , DbU.UnitPowerMicro ) \
                      * DbU.toPhysical( self.height, DbU.UnitPowerMicro )
            if plateArea < minArea:
                print( WarningMessage( 'BigVia::doLayout(): Area too small for {}'.format(self.net) ))
            self.plates[ depth ] = Contact.create( self.net
                                                 , rg.getRoutingLayer(depth)
                                                 , self.x    , self.y
                                                 , self.width, self.height
                                                 )
        if rg.isSymbolic():
            for depth in range(self.bottomDepth,self.topDepth):
                Contact.create( self.net
                              , rg.getContactLayer( depth )
                              , self.x
                              , self.y
                              , self.width  - DbU.fromLambda( 1.0 )
                              , self.height - DbU.fromLambda( 1.0 ) )
        else:
            for depth in range(self.bottomDepth,self.topDepth):
                self._doCutMatrix( depth )
        self.hasLayout = True

    def _doCutMatrix ( self, depth ):
        viaLayer     = rg.getContactLayer( depth )
        cutLayer     = viaLayer.getCut()
        cutSide      = cutLayer.getMinimalSize()
        cutSpacing   = cutLayer.getMinimalSpacing()
        if not cutSide:
            raise ErrorMessage( 1, 'BigVia._doCutMatrix(): Minimal side of cut layer "{}" is zero.' \
                                   .format( cutLayer.getName() ))
        if not cutSpacing:
            raise ErrorMessage( 1, 'BigVia._doCutMatrix(): Cut spacing on layer "{}" is zero.' \
                                   .format( cutLayer.getName() ))
        topEnclosure = max( viaLayer.getTopEnclosure( Layer.EnclosureH )
                          , viaLayer.getTopEnclosure( Layer.EnclosureV ))
        botEnclosure = max( viaLayer.getBottomEnclosure( Layer.EnclosureH )
                          , viaLayer.getBottomEnclosure( Layer.EnclosureV ))
        enclosure    = max( topEnclosure, botEnclosure )
        trace( 550, '\tBigVia: {}\n'.format(self) )
        trace( 550, '\t| topEnclosure[{}]: {}\n'.format(depth,DbU.getValueString(topEnclosure)) )
        trace( 550, '\t| botEnclosure[{}]: {}\n'.format(depth,DbU.getValueString(botEnclosure)) )
        trace( 550, '\t| enclosure   [{}]: {}\n'.format(depth,DbU.getValueString(enclosure)) )
        cutArea = self.plates[ depth ].getBoundingBox()
        cutArea.inflate( - enclosure - cutSide/2 )
        xoffset = (cutArea.getWidth () % (cutSide+cutSpacing)) / 2
        yoffset = (cutArea.getHeight() % (cutSide+cutSpacing)) / 2
        cutArea.translate( xoffset, yoffset )
       #if cutArea.isEmpty():
       #    raise ErrorMessage( 1, 'BigVia._doCutMatrix(): Cannot create at least a single cut in {}.' \
       #                           .format(self))
        self.vias[ depth ] = []
        y = cutArea.getYMin()
        while y <= cutArea.getYMax():
            x = cutArea.getXMin()
            self.vias[ depth ].append( [] )
            while x <= cutArea.getXMax():
                cut = Contact.create( self.net, cutLayer, x, y, cutSide, cutSide )
                self.vias[ depth ][ -1 ].append( cut )
                x += cutSide + cutSpacing
            y += cutSide + cutSpacing
        

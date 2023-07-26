
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/configuration.py"          |
# +-----------------------------------------------------------------+

import sys
import os.path
from   ...                   import Cfg
from   ...Hurricane          import Breakpoint, DbU, Box, Transformation, Box, \
                                    Path, Layer, Occurrence, Net, RoutingPad,  \
                                    Horizontal, Vertical, Contact, Pin, Plug,  \
                                    Instance
from   ...CRL                import AllianceFramework, RoutingLayerGauge
from   ...helpers            import trace
from   ...helpers.overlay    import UpdateSession
from   ...helpers.io         import ErrorMessage, WarningMessage, \
                                    vprint, catch
from   .                     import chip
from   ..block.configuration import BlockConf
from   .constants            import importConstants

__all__ = [ 'ChipConf' ]


importConstants( globals() )
af = AllianceFramework.get()


# -------------------------------------------------------------------
# Class  :  "Configuration.ChipConf".

class ChipConf ( BlockConf ):

    @staticmethod
    def _toSymbolic ( u, rounding ): 
        """
        Pitch the coordinates ``u`` to the symbolic grid, according
        to ``rounding`` (Superior or Inferior).
        """
        oneLambda = DbU.fromLambda( 1.0 )
        remainder = u % oneLambda
        if remainder:
            if rounding == Superior: u = u + (oneLambda - remainder)
            else:                    u = u -              remainder
        return u

    @staticmethod
    def toSymbolic ( v, rounding ): 
        """
        Pitch the coordinates of object ``v`` to the symbolic grid,
        according to ``rounding``. Were ``v`` can be:

        * A scalar, then rounding is Inferior or Superior.
        * A Box, then rounding is:
          
          * Inwards: the pitched box will be fully enclosed in the
            original box.
          * Outwards: the pitched box will fully enclose the original
            box.
        """
        if isinstance(v,int): return ChipConf._toSymbolic( v, rounding )
        if isinstance(v,Box):
            if rounding & Inwards:
                roundings = [ Superior
                            , Superior
                            , Inferior
                            , Inferior ]
            else:
                roundings = [ Inferior
                            , Inferior
                            , Superior
                            , Superior ]
            xMin = ChipConf._toSymbolic( v.getXMin(), roundings[0] )
            yMin = ChipConf._toSymbolic( v.getYMin(), roundings[1] )
            xMax = ChipConf._toSymbolic( v.getXMax(), roundings[2] )
            yMax = ChipConf._toSymbolic( v.getYMax(), roundings[3] )
            return Box( xMin, yMin, xMax, yMax )
        return v

    def __init__ ( self, cell, ioPins=[], ioPads=[] ):
        trace( 550, ',+', 'ChipConf.__init__(): "{}"'.format(cell.getName()) )
        super(ChipConf,self).__init__( cell, ioPins, ioPads )
        #trace( 550, '\tONE LAMBDA = %s\n' % DbU.getValueString(DbU.fromLambda(1.0)) )
        self.validated = True
        # Block Corona parameters (triggers loading from disk).
        self.cfg.chip.padCoreSide          = None
        self.cfg.chip.supplyRailWidth      = None
        self.cfg.chip.supplyRailPitch      = None
        self.cfg.chip.block.rails.count    = None
        self.cfg.chip.block.rails.hWidth   = None
        self.cfg.chip.block.rails.vWidth   = None
        self.cfg.chip.block.rails.hSpacing = None
        self.cfg.chip.block.rails.vSpacing = None
        self._railsCount = self.cfg.chip.block.rails.count
        # Global Net names.    
        self.blockageName     = "blockagenet"
        # Global Nets.         
        self.coronaVdd        = None
        self.coronaVss        = None
        self.coronaCks        = []
        self.blockageNet      = None
        self.padsHavePosition = False
        self.chipLogos        = []
        self.minHCorona       = 0
        self.minVCorona       = 0
        self.coreToChip       = None
        trace( 550, '-' )

    @property
    def padCoreSide ( self ):
        return self.cfg.chip.padCoreSide

    @property
    def railsCount ( self ):
        return self._railsCount

    @railsCount.setter
    def railsCount ( self, count ):
        self._railsCount = count

    @property
    def hRailWidth ( self ):
        return self.cfg.chip.block.rails.hWidth

    @property
    def vRailWidth ( self ):
        return self.cfg.chip.block.rails.vWidth

    @property
    def hRailSpace ( self ):
        return self.cfg.chip.block.rails.hSpacing

    @property
    def vRailSpace ( self ):
        return self.cfg.chip.block.rails.vSpacing

    def computeCoronaBorder ( self ):
        global af
        if self.useClockTree:
            clockNets = []
            for net in self.cellPnR.getNets():
                if net.isClock():
                    clockNets.append( net )
            self.railsCount = self.cfg.chip.block.rails.count + len(clockNets)
        trace( 550, '\tself.railsCount: {}\n'.format(self.railsCount) )
        hRailsSize = self.railsCount*(self.hRailWidth + self.hRailSpace) + self.hRailSpace
        trace( 550, '\thRailsSize={}\n'.format(DbU.getValueString( hRailsSize )))
        if hRailsSize % self.sliceHeight:
            hRailsSize += self.sliceHeight - (hRailsSize % self.sliceHeight)
        trace( 550, '\thRailsSize (sliceHeight)={}\n'.format(DbU.getValueString( hRailsSize )))
        self.minHCorona = hRailsSize #+ self.sliceHeight
        vRailsSize = self.railsCount*(self.vRailWidth + self.vRailSpace) + self.vRailSpace
        if vRailsSize % self.sliceHeight:
            vRailsSize += self.sliceHeight - (vRailsSize % self.sliceHeight)
        self.minVCorona = vRailsSize #+ self.sliceHeight

    def chipValidate ( self ):
        #self.checkPads()
        #self.checkCorona()
        #self.computeChipSize()
        #self.checkChipSize()
        self.findPowerAndClockNets()
        return

    def getInstanceAb ( self, instance ):
        ab = instance.getMasterCell().getAbutmentBox()
        instance.getTransformation().applyOn( ab )
        if instance.getCell() == self.chip: return ab
        if instance.getCell() != self.corona:
            raise ErrorMessage( 1, 'ChipConf.getInstanceAb(): Instance "{}" neither belong to chip or corona.' \
                                   .format(instance.getName()) )
            return ab
        self.icorona.getTransformation().applyOn( ab )
        return ab

    def setupICore ( self ):
        """
        Setup the abutment box of the *core* master cell and position it's unique
        instance (*icore*) in the center of the *corona* master cell.
        """
        with UpdateSession():
            trace( 550, ',+', '\tChipConf.setupICore()\n' )
            ab = self.getInstanceAb( self.icorona )
            if ab.isEmpty():
                raise ErrorMessage( 1, 'ChipConf.setupICore(): Attempt to setup core *before* corona.' )
                return
            #ab.inflate( -gapX1, -gapY1, -gapX2, -gapY2 )
            ab = self.toSymbolic( ab, Inwards )
            trace( 550, '\tself.coreAb:{}\n'.format(self.coreAb) )
            if self.core.getAbutmentBox().isEmpty():
                if not self.coreAb.isEmpty():
                    trace( 550, '\tUsing user-defined CORE size:{}\n'.format(self.coreSize) )
                    ab = self.coreAb
                else:
                    ab.inflate( -self.minHCorona, -self.minVCorona )
                    self.coreSize = (ab.getWidth(), ab.getHeight())
                trace( 550, '\tSetting CORE abutment box:{}\n'.format(ab) )
                self.core.setAbutmentBox( Box( 0, 0, ab.getWidth(), ab.getHeight() ) )
            self.coreSize = ( self.coreSize[0] - self.coreSize[0] % self.sliceStep
                            , self.coreSize[1] - self.coreSize[1] % self.sliceHeight )
            self.core.setAbutmentBox( Box( 0, 0, self.coreAb.getWidth(), self.coreAb.getHeight() ) )
            trace( 550, '\tCORE ab:{}\n'.format(self.coreAb) )
            coreX = (self.coronaAb.getWidth () - self.coreAb.getWidth ()) // 2
            trace( 550, '\tCore X, {} '.format(DbU.getValueString(coreX)) )
            coreX = coreX - (coreX % self.sliceHeight)
            trace( 550, ' adjusted on {}, {}\n'.format( DbU.getValueString(self.sliceHeight)
                                                      , DbU.getValueString(coreX)) )
            coreY = (self.coronaAb.getHeight() - self.coreAb.getHeight()) // 2
            coreY = coreY - (coreY % self.sliceHeight)
            self.icore.setTransformation( Transformation( coreX, coreY, Transformation.Orientation.ID ) )
            self.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )
            trace( 550, '-' )

    def getCoronaNet ( self, chipNet ):
        for plug in chipNet.getPlugs():
            if plug.getInstance() == self.icorona:
                return plug.getMasterNet()
        return None

    def toRoutingGauge ( self, uMin, uMax, layer ):
        trace( 550, ',+', '\ttoRoutingGauge() [{} {}] {}\n' \
                    .format(DbU.getValueString(uMin), DbU.getValueString(uMax), layer) )
        ab   = self.corona.getAbutmentBox()
        lg   = None
        mask = layer.getMask()
        for layerGauge in self.routingGauge.getLayerGauges():
            if layerGauge.getLayer().getMask() == mask:
                lg = layerGauge
                trace( 550, '\tUsing layer gauge {}\n'.format(lg) )
                break
        if uMax < uMin: uMin, uMax = uMax, uMin
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                abMin = ab.getYMin()
                abMax = ab.getYMax()
            else:
                abMin = ab.getXMin()
                abMax = ab.getXMax()
            if uMin <= abMin:
                shiftRight = abMin - uMin + lg.getPitch()
                uMin      += shiftRight
                uMax      += shiftRight
            if uMax >= abMax:
                shiftLeft = uMax - abMax + lg.getPitch()
                uMin     -= shiftLeft
                uMax     -= shiftLeft
            iTrackMin = lg.getTrackIndex( abMin, abMax, uMin, RoutingLayerGauge.Superior )
            iTrackMax = lg.getTrackIndex( abMin, abMax, uMax, RoutingLayerGauge.Inferior )
            if iTrackMax < iTrackMin: iTrackMax = iTrackMin
            uTrackMin = lg.getTrackPosition( abMin, iTrackMin )
            uTrackMax = lg.getTrackPosition( abMin, iTrackMax )
            axis  = (uTrackMax + uTrackMin) // 2
            width = (iTrackMax - iTrackMin) * lg.getPitch() + lg.getWireWidth()
            if self.routingGauge.isSymbolic():
                trace( 550, '\tRoutingGauge is symbolic, adjust on lambda.\n' )
                oneLambda = DbU.fromLambda( 1.0 )
                if axis % oneLambda:
                    axis  -= oneLambda // 2
                    width -= oneLambda
            trace( 550, '\t[{} {}] -> [{} {}]\n'.format( iTrackMin
                                                       , iTrackMax
                                                       , DbU.getValueString(uTrackMin)
                                                       , DbU.getValueString(uTrackMax) ) )
            trace( 550, '\taxis:  {:.1f}L {}\n'.format(DbU.toLambda(axis ), DbU.getValueString(axis )) )
            trace( 550, '\twidth: {:.1f}L {}\n'.format(DbU.toLambda(width), DbU.getValueString(width)) )
        else:
            axis  = (uMax + uMin) // 2
            width = (uMax - uMin)
        trace( 550, '-' )
        return axis, width

    def toCoronaPitchInChip ( self, uCore, layer ):
        trace( 550, ',+', '\tChipConf.toCoronaPitchInChip(): uCore:  {:.1f}L {}\n' \
                          .format(DbU.toLambda(uCore), DbU.getValueString(uCore)) )
        coronaAb = self.getInstanceAb( self.icorona )
        lg       = None
        mask     = layer.getMask()
        for layerGauge in self.routingGauge.getLayerGauges():
            if layerGauge.getLayer().getMask() == mask:
                lg = layerGauge
                break
        if not lg:
            trace( 550, '-' )
            return 0
        trace( 550, '\t{}\n'.format(lg) )
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                uCorona = uCore - coronaAb.getYMin()
            else:
                uCorona = uCore - coronaAb.getXMin()
        uCorona, width = self.toRoutingGauge( uCorona, uCorona, layer )
        trace( 550, '\ttoCoronaPitchInChip(): uCorona: {:.1f}L {}\n' \
                    .format(DbU.toLambda(uCorona), DbU.getValueString(uCorona)) )
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                uCore = uCorona + coronaAb.getYMin()
            else:
                uCore = uCorona + coronaAb.getXMin()
        trace( 550, '\ttoCoronaPitchInChip(): uCorona: {:.1f}L {}\n'.format(DbU.toLambda(uCorona), DbU.getValueString(uCorona)) )
        trace( 550, '\ttoCoronaPitchInChip(): uCore:   {:.1f}L {}\n'.format(DbU.toLambda(uCore  ), DbU.getValueString(uCore  )) )
        trace( 550, '-' )
        return uCore

    def coronaHorizontal ( self, chipNet, layer, chipY, width, chipXMin, chipXMax ):
        trace( 550, ',+', '\tChipConf.coronaHorizontal\n' )
        coronaAb  = self.getInstanceAb( self.icorona )
        coronaNet = self.getCoronaNet ( chipNet )
        if not coronaNet: return None
        coronaY = chipY - coronaAb.getYMin()
        dxMin   = ChipConf.toSymbolic( chipXMin - coronaAb.getXMin(), Superior )
        dxMax   = ChipConf.toSymbolic( chipXMax - coronaAb.getXMin(), Inferior )
        trace( 550, '\t| chipNet: {} {}\n'.format(chipNet, layer) )
        trace( 550, '\t| Real\n' )
        trace( 550, '\t|   axis: {}\n'.format(DbU.getValueString(coronaY)) )
        trace( 550, '\t|   width:{}\n'.format(DbU.getValueString(width)) )
        trace( 550, '\t|   dxMin:{} ({:.1f}L)\n' \
                    .format( DbU.getValueString(chipXMin - coronaAb.getXMin())
                           , DbU.toLambda(chipXMin - coronaAb.getXMin()) ) )
        trace( 550, '\t|   dxMax:{}\n'.format(DbU.getValueString(chipXMax - coronaAb.getXMin())) )
        coronaY, width = self.toRoutingGauge( coronaY - width//2, coronaY + width//2, layer )
        trace( 550, '\t| On Grid\n' )
        trace( 550, '\t|   axis: {:.1f}L or {}\n'.format(DbU.toLambda(coronaY), DbU.getValueString(coronaY)) )
        trace( 550, '\t|   width:{:.1f}L or {}\n'.format(DbU.toLambda(width)  , DbU.getValueString(width)) )
        trace( 550, '\t|   dxMin:{:.1f}L\n'.format(DbU.toLambda(dxMin)) )
        trace( 550, '\t|   dxMax:{:.1f}L\n'.format(DbU.toLambda(dxMax)) )
        h = Horizontal.create( coronaNet, layer, coronaY, width, dxMin, dxMax )
        trace( 550, '\t| {}\n'.format(h) )
        trace( 550, '-' )
        return h

    def coronaVertical ( self, chipNet, layer, chipX, width, chipYMin, chipYMax ):
        trace( 550, ',+', '\tChipConf.coronaVertical\n' )
        coronaAb  = self.getInstanceAb( self.icorona )
        coronaNet = self.getCoronaNet( chipNet )
        if not coronaNet: return None
        coronaX = chipX - coronaAb.getXMin()
        dyMin   = ChipConf.toSymbolic( chipYMin - coronaAb.getYMin(), Superior )
        dyMax   = ChipConf.toSymbolic( chipYMax - coronaAb.getYMin(), Inferior )
        trace( 550, '\t| chipNet: {} {}\n'.format(chipNet, layer) )
        trace( 550, '\t| Real\n' )
        trace( 550, '\t|   axis: {}\n'.format(DbU.getValueString(coronaX)) )
        trace( 550, '\t|   width:{}\n'.format(DbU.getValueString(width)) )
        coronaX, width = self.toRoutingGauge( coronaX - width//2, coronaX + width//2, layer )
        trace( 550, '\t| On Grid\n' )
        trace( 550, '\t|   axis: {:.1f}L or {}\n'.format(DbU.toLambda(coronaX), DbU.getValueString(coronaX)) )
        trace( 550, '\t|   width:{:.1f}L or {}\n'.format(DbU.toLambda(width)  , DbU.getValueString(width)) )
        v = Vertical.create( coronaNet, layer, coronaX, width, dyMin, dyMax )
        trace( 550, '\t| {}\n'.format(v) )
        trace( 550, '-' )
        return v

    def coronaContact ( self, chipNet, layer, chipX, chipY, width, height, flags=0 ):
        trace( 550, ',+', '\tChipConf.coronaContact\n' )
        coronaAb  = self.getInstanceAb( self.icorona )
        coronaNet = self.getCoronaNet( chipNet )
        if not coronaNet: return None
        coronaX = chipX - coronaAb.getXMin()
        coronaY = chipY - coronaAb.getYMin()
        trace( 550, '\t| chipNet: {} {}\n'.format(chipNet,layer) )
        trace( 550, '\t| Real\n' )
        trace( 550, '\t|   center: {:>12} {:>12}\n'.format(DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
        trace( 550, '\t|   WxH:    {:>12} {:>12}\n'.format(DbU.getValueString(width  ), DbU.getValueString(height )) )
        topLayer = layer.getTop()
        botLayer = layer.getBottom()
        if self.isHorizontal(topLayer):
            coronaX, width  = self.toRoutingGauge( coronaX - width //2, coronaX + width //2, botLayer )
            coronaY, height = self.toRoutingGauge( coronaY - height//2, coronaY + height//2, topLayer )
        else:
            coronaX, width  = self.toRoutingGauge( coronaX - width //2, coronaX + width //2, topLayer )
            coronaY, height = self.toRoutingGauge( coronaY - height//2, coronaY + height//2, botLayer )
        if not (flags & OnHorizontalPitch):
            trace( 550, '\tNot on horizontal routing pitch, Y on lambda only.\n' )
            coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), Superior )
        if not (flags & OnVerticalPitch  ):
            trace( 550, '\tNot on vertical routing pitch, X on lambda only.\n' )
            coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), Superior )
        trace( 550, '\t| On Grid\n' )
        trace( 550, '\t|   X axis: {:>12.1f}L or {:>12}\n'.format(DbU.toLambda(coronaX)      , DbU.getValueString(coronaX)) )
        trace( 550, '\t|   Y axis: {:>12.1f}L or {:>12}\n'.format(DbU.toLambda(coronaY)      , DbU.getValueString(coronaY)) )
        trace( 550, '\t|   center: {:>12} {:>12}\n'       .format(DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
        trace( 550, '\t|   WxH:    {:>12} {:>12}\n'       .format(DbU.getValueString(width  ), DbU.getValueString(height )) )
        c = Contact.create( coronaNet
                          , layer
                          , coronaX
                          , coronaY
                          , width
                          , height
                          )
        trace( 550, '\t| {}\n'.format(c) )
        trace( 550, '-' )
        return c

    def getViaPitch ( self, viaLayer ):
        topLayer = viaLayer.getTop()
        if topLayer.isSymbolic():
            topLayer = topLayer.getBasicLayer()
        topEnclosure = viaLayer.getEnclosure( topLayer, Layer.EnclosureH|Layer.EnclosureV )
        topPitch = 2*topEnclosure + viaLayer.getMinimalSize() + topLayer.getMinimalSpacing()  
        botLayer = viaLayer.getBottom()
        if botLayer.isSymbolic():
            botLayer = botLayer.getBasicLayer()
        botEnclosure = viaLayer.getEnclosure( botLayer, Layer.EnclosureH|Layer.EnclosureV )
        botPitch = 2*botEnclosure + viaLayer.getMinimalSize() + botLayer.getMinimalSpacing()  
        viaPitch = max( topPitch, botPitch )
        trace( 550, '\tgetViaPitch of {}: {}\n'.format(viaLayer.getName(),DbU.getValueString(viaPitch)) )
        trace( 550, '\t| minimal size of {}: {}\n'.format(viaLayer.getName(),DbU.getValueString(viaLayer.getMinimalSize())) )
        trace( 550, '\t| enclosure of {}: {}\n'.format(topLayer.getName(),DbU.getValueString(topEnclosure)) )
        trace( 550, '\t| enclosure of {}: {}\n'.format(botLayer.getName(),DbU.getValueString(botEnclosure)) )
        return viaPitch

    def coronaContactArray ( self, chipNet, layer, chipX, chipY, array, flags ):
        trace( 550, ',+', '\tChipConf.coronaContactArray\n' )
        viaPitch  = self.getViaPitch( layer )
        coronaAb  = self.getInstanceAb( self.icorona )
        coronaNet = self.getCoronaNet( chipNet )
        if not coronaNet: return None
        trace( 550, '\t| chipNet: {} {}\n'.format(chipNet, layer) )
        coronaX = chipX - coronaAb.getXMin()
        coronaY = chipY - coronaAb.getYMin()
        topLayer = layer.getTop()
        if self.isHorizontal(topLayer):
            coronaX, width  = self.toRoutingGauge( coronaX, coronaX, layer.getBottom() )
            coronaY, height = self.toRoutingGauge( coronaY, coronaY, topLayer )
        else:
            coronaX, width  = self.toRoutingGauge( coronaX, coronaX, topLayer )
            coronaY, height = self.toRoutingGauge( coronaY, coronaY, layer.getBottom() )
        if not (flags & OnHorizontalPitch):
            trace( 550, '\tNot on horizontal routing pitch, Y on lambda only.\n' )
            coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), Superior )
        if not (flags & OnVerticalPitch  ):
            trace( 550, '\tNot on vertical routing pitch, X on lambda only.\n' )
            coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), Superior )
        contacts    = []
        xContact    = coronaX - viaPitch * (array[0]-1)//2
        yContact    = coronaY - viaPitch * (array[1]-1)//2
        contactSize = layer.getMinimalSize()
        trace( 550, '\txContact:{} yContact:{}\n'.format(DbU.getValueString(xContact),DbU.getValueString(yContact)) )
        for i in range(array[0]):
            for j in range(array[1]):
                c = Contact.create( coronaNet
                                  , layer
                                  , xContact + i*viaPitch
                                  , yContact + j*viaPitch
                                  , contactSize
                                  , contactSize
                                  )
                trace( 550, '\t+ {}\n'.format(c) )
                contacts.append( c )
        trace( 550, '-' )
        return contacts

    def coronaPin ( self, chipNet, count, direction, layer, chipX, chipY, width, height ):
        trace( 550, ',+', '\tChipConf.coronaPin\n' )
        coronaAb  = self.getInstanceAb( self.icorona )
        coronaNet = self.getCoronaNet( chipNet )
        if not coronaNet: return None
        coronaX = chipX - coronaAb.getXMin()
        coronaY = chipY - coronaAb.getYMin()
        trace( 550, '\t| chipNet: {} ({}) {}\n'.format(chipNet, count, layer) )
        trace( 550, '\t| Real\n' )
        trace( 550, '\t|   center: {} {}\n'.format(DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
        trace( 550, '\t|   WxH:    {} {}\n'.format(DbU.getValueString(width  ), DbU.getValueString(height )) )
        topLayer = layer.getTop()
        if self.isHorizontal(topLayer):
            coronaX, width  = self.toRoutingGauge( coronaX - width //2, coronaX + width //2, layer.getBottom() )
            coronaY, height = self.toRoutingGauge( coronaY - height//2, coronaY + height//2, topLayer )
        else:
            coronaX, width  = self.toRoutingGauge( coronaX - width //2, coronaX + width //2, topLayer )
            coronaY, height = self.toRoutingGauge( coronaY - height//2, coronaY + height//2, layer.getBottom() )
        if direction == Pin.Direction.NORTH or direction == Pin.Direction.SOUTH: 
            trace( 550, '\tEast/West not on horizontal routing pitch, Y on lambda only.\n' )
            coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), Superior )
        if direction == Pin.Direction.EAST or direction == Pin.Direction.WEST: 
            trace( 550, '\tNorth/South not on vertical routing pitch, X on lambda only.\n' )
            coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), Superior )
        trace( 550, '\t| On Grid\n' )
        trace( 550, '\t|   X axis: {:.1f}L or {}\n'.format(DbU.toLambda(coronaY)      , DbU.getValueString(coronaY)) )
        trace( 550, '\t|   Y axis: {:.1f}L or {}\n'.format(DbU.toLambda(coronaX)      , DbU.getValueString(coronaX)) )
        trace( 550, '\t|   center: {} {}\n'       .format(DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
        trace( 550, '\t|   WxH:    {} {}\n'       .format(DbU.getValueString(width  ), DbU.getValueString(height )) )
        c = Pin.create( coronaNet
                      , '{}.{}'.format(coronaNet.getName(),count)
                      , direction
                      , Pin.PlacementStatus.FIXED
                      , layer
                      , coronaX
                      , coronaY
                      , width
                      , height
                      )
        trace( 550, '\t| {}\n'.format(c) )
        trace( 550, '-' )
        return c

    def checkPads ( self ):

        def contains ( padList, side, padInstance ):
            for i in range(len(padList)):
                if padList[i][1] == padInstance.getName():
                  if (padInstance.getMasterCell().getAbutmentBox().getHeight() != self.ioPadHeight):
                      raise ErrorMessage( 1, 'The pad [{}] {} ({}) on {} side is not an instance of a pad cell.' \
                                             .format(i,padInstance.getName(),padInstance.getMasterCell().getName(),side) )
                  padList[i][1] = padInstance
                  return True
            return False
      
        def checkNotFounds ( padList, side ):
            for i in range(len(padList)):
                if not isinstance(padList[i][1],Instance):
                    print( ErrorMessage( 1, 'The pad [{}] ({}) of list {} do not exists in netlist (skipped).'
                                            .format(i,padList[i][1],side) ))
            return
      
        global af
        cellPads = []
        for instance in self.chip.getInstances():
            if contains(self.southPads,'south',instance): continue
            if contains(self.northPads,'north',instance): continue
            if contains(self.eastPads ,'east' ,instance): continue
            if contains(self.westPads ,'west' ,instance): continue
            if (instance.getMasterCell().getAbutmentBox().getHeight() == self.ioPadHeight):
                raise ErrorMessage( 1, 'Pad "{}" is not on any side (N/S/E/W).'.format(instance.getName()) )
                self.validated = False
            else:
                self.coronas.append( instance )
        checkNotFounds( self.southPads, 'south' )    
        checkNotFounds( self.northPads, 'north' )    
        checkNotFounds( self.eastPads , 'east'  )    
        checkNotFounds( self.westPads , 'west'  )    
        if len(self.coronas) > 1:
            message = [ 'Chip "{}" have more than one corona:'.format(self.chip.getName()) ]
            for i in range(len(self.coronas)):
                message.append( '{}: {}'.format(i,self.coronas[i].getName()) )
            raise ErrorMessage( 1, message )
            self.validated = False
        if len(self.coronas) < 1:
            raise ErrorMessage( 1, 'Chip "{}" doesn\'t seems to have a corona.' \
                                   .format(self.chip.getName()) )
            self.validated = False
        else:
          for instance in self.corona.getInstances():
              self.cores.append( instance )
          if len(self.cores) > 1:
              message = [ 'Chip "{}" have more than one core:'.format(self.chip.getName()) ]
              for i in range(len(self.cores)):
                  message.append( '{}: {}'.format(i,self.cores[i].getName()) )
              raise ErrorMessage( 1, message )
              self.validated = False
              
              if len(self.cores) < 1:
                  raise ErrorMessage( 1, 'Chip "{} doesn\'t seems to have a core.' \
                                         .format(self.chip.getName()) )
                  self.validated = False
        return

    def findPowerAndClockNets ( self ):
        if self.icore:
            for plug in self.icore.getPlugs():
                masterNet = plug.getMasterNet()
                netType   = masterNet.getType()
                if     netType != Net.Type.POWER  \
                   and netType != Net.Type.GROUND \
                   and netType != Net.Type.CLOCK:
                    continue
                net = plug.getNet()
                if not net:
                    net = self.corona.getNet( masterNet.getName() )
                    if not net:
                        raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Missing global net "{}" at corona level.' \
                                               .format(masterNet.getName()) )
                        self._validated = False
                        continue
                if netType == Net.Type.GROUND:
                    if self.coronaVss and self.coronaVss != net:
                        raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Multiple ground nets "{}" and "{}" at corona level.' \
                                               .format(self.coronaVss.getName(), net.getName()) )
                        self._validated = False
                        continue
                    else:
                        self.coronaVss = net
                
                if netType == Net.Type.POWER:
                    if self.coronaVdd and self.coronaVdd != net:
                        raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Multiple power nets "{}" and "{}" at corona level.' \
                                               .format(self.coronaVdd.getName(), net.getName()) )
                        self._validated = False
                        continue
                    else:
                        self.coronaVdd = net
                
                if netType == Net.Type.CLOCK:
                    if not net in self.coronaCks:
                        self.coronaCks.append( net )
                        vprint( 2, '     - Using clock "{}".'.format(net.getName()) )
        for net in self.corona.getNets():
            if net.getType() == Net.Type.BLOCKAGE:
                self.blockageNet  = net
                self.blockageName = net.getName()
        if not self.blockageNet:
            self.blockageNet = Net.create( self.corona, self.blockageName )
            self.blockageNet.setType( Net.Type.BLOCKAGE )
        return

    def checkChipSize ( self ):
        if self.chipSize[0] % self.sliceStep:
            print( WarningMessage( 'ChipConf.checkChipSize(): Width of "{}" ({})is not on sliceStep ({}), ajusted.' \
                                   .format( self.chipConf.name
                                          , DbU.getValueString(self.chipSize[0])
                                          , DbU.getValueString(self.sliceStep))) )
            adjust = self.sliceStep - self.chipSize[0] % self.sliceStep
            self.chipSize = (self.chipSize[0] + adjust, self.chipSize[1])
        if self.chipSize[1] % self.sliceStep:
            print( WarningMessage( 'ChipConf.checkChipSize(): Height of "{}" ({})is not on sliceStep ({}), ajusted.' \
                                   .format( self.chipConf.name
                                          , DbU.getValueString(self.chipSize[1])
                                          , DbU.getValueString(self.sliceStep))) )
            adjust = self.sliceStep - self.chipSize[1] % self.sliceStep
            self.chipSize = (self.chipSize[0], self.chipSize[1] + adjust)

        #if self._coreSize.isEmpty(): return
        #
        #minWidth  = self._coreSize.getWidth () + self._minCorona + 2*self._padHeight
        #minHeight = self._coreSize.getHeight() + self._minCorona + 2*self._padHeight
        #
        #if self._chipSize.getWidth() < minWidth:
        #  raise ErrorMessage( 1, 'Core is too wide to fit into the chip. Needs: %d, but has %d' \
        #                         % ( DbU.toLambda(minWidth), DbU.toLambda(self._chipSize.getWidth()) ) )
        #  self._validated = False
        #
        #if self._chipSize.getHeight() < minHeight:
        #  raise ErrorMessage( 1, 'Core is too wide to fit into the chip. Needs: %d, but has %d' \
        #                         % ( DbU.toLambda(minHeight), DbU.toLambda(self._chipSize.getHeight()) ) )
        #  self._validated = False
        return

    def checkCorona ( self ):
        trace( 550, ',+', 'Configuration.checkCorona()\n' )
        netPads = {}
        for plug in self.icorona.getPlugs():
            padNet    = plug.getNet()
            coronaNet = plug.getMasterNet()
            if not padNet and coronaNet.isGlobal():
                padNet = self.chip.getNet( coronaNet.getName() )
            if padNet:
                if not padNet in netPads:
                    trace( 550, '\t{:>20} <-> {:<20}\n'.format(padNet.getName(),coronaNet.getName()) )
                    netPads[ padNet ] = coronaNet
                else:
                    raise ErrorMessage( 1, 'ChipConf.checkCorona(): Corona nets "{}" and "{}" connected to the same pad net "{}".' \
                                           .format(coronaNet.getName(),netPads[padNet].getName(),padNet.getName()) )
                    self._validated = False
        trace( 550, '-' )
        return

    def computeChipSize ( self ):

        def getSideLength ( pads ):
            sideLength = self.ioPadHeight * 2 
            for pad in pads: sideLength += pad.getMasterCell().getAbutmentBox().getWidth()
            return sideLength
          
        if not self.chipSize.isEmpty(): return
        southPadsLength = getSideLength( self.southPads )
        northPadsLength = getSideLength( self.northPads )
        eastPadsLength  = getSideLength( self.eastPads  )
        westPadsLength  = getSideLength( self.westPads  )
        horizontalPads  = max( len(self.southPads), len(self.northPads) )
        verticalPads    = max( len(self.eastPads ), len(self.westPads ) )
        self.chipSize = ( max( southPadsLength, northPadsLength )
                        , max(  westPadsLength,  eastPadsLength ) )

    def setupCorona ( self, gapX1, gapY1, gapX2, gapY2 ):
        ab = self.chip.getAbutmentBox()
        ab.inflate  ( -gapX1, -gapY1, -gapX2, -gapY2 )
        ab.inflate  ( - self.ioPadHeight )
        ab.translate( - self.ioPadHeight, - self.ioPadHeight)
        ab = self.toSymbolic( ab, Inwards )
        
        self. corona.setAbutmentBox( Box( 0, 0, ab.getWidth(), ab.getHeight() ) )
        self.icorona.setTransformation(
            Transformation( self.toSymbolic( self.ioPadHeight + ab.getXMin(), Superior )
                          , self.toSymbolic( self.ioPadHeight + ab.getYMin(), Superior )
                          , Transformation.Orientation.ID ) )
        self.icorona.setPlacementStatus( Instance.PlacementStatus.FIXED )
        self.setRoutingBb( self.corona.getAbutmentBox() )

    def setupCore ( self, gapX1, gapY1, gapX2, gapY2 ):
        trace( 550, ',+', '\tChipConf.setupCore()\n' )
        ab = self.getInstanceAb( self.icorona )
        if ab.isEmpty():
            raise ErrorMessage( 1, 'ChipConf.setupCore(): Attempt to setup core *before* corona.' )
            return
        ab.inflate( -gapX1, -gapY1, -gapX2, -gapY2 )
        ab = self.toSymbolic( ab, Inwards )
        tracee( 550, '\tself.coreAb:{}\n'.format(self.coreAb) )
        if not self.coreAb.isEmpty():
            trace( 550, '\tUsing user-defined CORE size:{}\n'.format(self.coreSize) )
            ab = self.coreAb
        trace( 550, '\tSetting CORE abutment box:{}\n'.format(ab) )
        self.core.setAbutmentBox( Box( 0, 0, ab.getWidth(), ab.getHeight() ) )
        self.icore.setTransformation(
            Transformation( ChipConf.toSymbolic(ab.getXMin(),Inferior) - self.icorona.getTransformation().getTx()
                          , ChipConf.toSymbolic(ab.getYMin(),Inferior) - self.icorona.getTransformation().getTy()
                          , Transformation.Orientation.ID ) )
        self.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )
        trace( 550, '-' )

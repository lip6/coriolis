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
# |  Python      :       "./plugins/block/configuration.py"         |
# +-----------------------------------------------------------------+

from   __future__ import print_function
import sys
import re
import os.path
from   operator  import itemgetter
import Cfg
from   Hurricane import DataBase, Breakpoint, DbU, Box, Transformation, \
                        Path, Layer, Occurrence, Net,                   \
                        NetExternalComponents, RoutingPad, Horizontal,  \
                        Vertical, Contact, Pin, Plug, Instance
import CRL
from   CRL                 import RoutingLayerGauge
from   helpers             import trace, l, u, n
from   helpers.utils       import classdecorator
from   helpers.io          import ErrorMessage, WarningMessage, catch
from   helpers.overlay     import CfgCache
from   plugins             import getParameter
from   plugins.rsave       import rsave
from   plugins.alpha.utils import getPlugByName


def findCellOutput ( cell, callerName, parameterId ):
    """Find the *external* output net of a cell and return it's name."""
    for net in cell.getNets():
        if not net.isExternal(): continue
        if     net.isGlobal(): continue
        elif   net.getDirection() & Net.Direction.OUT:
            return net.getName()
    raise ErrorMessage( 3, [ '{}: Cannot guess the input terminal of "{}",' \
                             .format(callerName,Cfg.getParamString(parameterId).asString())
                           , '           please check that the Nets directions are set.' ] )


def toFoundryGrid ( u, mode ):
    """Snap the DbU ``u`` to the foundry grid, according to ``mode``."""
    oneGrid = DbU.fromGrid( 1.0 )
    return DbU.getOnCustomGrid( u, oneGrid, mode )


# ----------------------------------------------------------------------------
# Class  :  "configuration.GaugeConf".

class GaugeConf ( object ):
    """
    Wrapper around the Cell and I/O pads RoutingGauge, with additional
    services.

    The Cell routing gauge is read from the configuration parameter
    ``anabatic.routingGauge``.
    """

    HAccess         = 0x0001
    OffsetRight1    = 0x0002
    OffsetTop1      = 0x0004
    OffsetBottom1   = 0x0008
    DeepDepth       = 0x0010
    UseContactWidth = 0x0020
    ExpandWidth     = 0x0040

    def __init__ ( self ):
        self._cellGauge      = None
        self._ioPadGauge     = None
        self._routingGauge   = None
        self._topLayerDepth  = 0
        self._plugToRp       = { }
        self._rpToAccess     = { }
        self._loadRoutingGauge()
        self._routingBb      = Box()
        return

    @property
    def routingGauge  ( self ): return self._routingGauge

    @property
    def sliceHeight   ( self ): return self._cellGauge.getSliceHeight()

    @property
    def sliceStep     ( self ): return self._cellGauge.getSliceStep()

    @property
    def ioPadHeight   ( self ): return self._ioPadGauge.getSliceHeight()

    @property
    def ioPadStep     ( self ): return self._ioPadGauge.getSliceStep()

    @property
    def ioPadPitch    ( self ): return self._ioPadGauge.getPitch()

    @property
    def ioPadGauge    ( self ): return self._ioPadGauge

    @property
    def hRoutingGauge ( self ): return self._routingGauge.getLayerGauge( self.horizontalDepth )

    @property
    def vRoutingGauge ( self ): return self._routingGauge.getLayerGauge( self.verticalDepth )

    @property
    def hDeepRG       ( self ): return self._routingGauge.getLayerGauge( self.horizontalDeepDepth )

    @property
    def vDeepRG       ( self ): return self._routingGauge.getLayerGauge( self.verticalDeepDepth )

    @property
    def routingBb     ( self ): return self._routingBb

    def getRoutingLayer ( self, depth ):
        return self._routingGauge.getRoutingLayer( depth )

    def getLayerDepth ( self, layer ):
        if isinstance(layer,str):
            layer = DataBase.getDB().getTechnology().getLayer( layer )
        return self._routingGauge.getLayerDepth( layer )

    def getPitch ( self, layer ): return self._routingGauge.getPitch( layer )

    def setRoutingBb ( self, bb ):
        trace( 550, '\tGaugeConf.setRoutingBb(): {}\n'.format(bb) )
        self._routingBb = bb

    def _loadRoutingGauge ( self ):
        trace( 550, ',+', '\tGaugeConf._loadRoutingGauge()\n' )
        gaugeName = Cfg.getParamString('anabatic.routingGauge').asString()
        self._cellGauge    = CRL.AllianceFramework.get().getCellGauge( gaugeName )
        self._routingGauge = CRL.AllianceFramework.get().getRoutingGauge( gaugeName )

        if not self._routingGauge:
            trace( 500, '-' )
            raise ErrorMessage( 1, [ 'RoutingGauge._loadRoutingGauge(): No routing gauge named "{}".'.format(gaugeName)
                                   , 'Please check the "anabatic.routingGauge" configuration parameter." ' ])

        topLayer = Cfg.getParamString('anabatic.topRoutingLayer').asString()

        self.topLayerDepth = 0
        for layerGauge in self._routingGauge.getLayerGauges():
            if layerGauge.getLayer().getName() == topLayer:
                self.topLayerDepth = layerGauge.getDepth()
                break
        if not self.topLayerDepth:
            print( WarningMessage( 'Gauge top layer not defined, using top of gauge ({}).' \
                                   .format(self._routingGauge.getDepth()) ))
            self._topLayerDepth = self._routingGauge.getDepth() - 1

        self.horizontalDepth     = -1
        self.verticalDepth       = -1
        self.horizontalDeepDepth = -1
        self.verticalDeepDepth   = -1
        for depth in range(0,self.topLayerDepth+1):
            trace( 550, '\tdepth:{} {}\n'.format(depth,self._routingGauge.getLayerGauge(depth) ))
            
            if self._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.PinOnly:
                continue
            if self._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.PowerSupply:
                continue
            if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
                if self.horizontalDeepDepth < 0:
                    self.horizontalDeepDepth = depth
                self.horizontalDepth = depth
            if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
                if self.verticalDeepDepth < 0:
                    self.verticalDeepDepth = depth
                self.verticalDepth = depth
        trace( 550, '\t| horizontalDepth    :{}\n'.format(self.horizontalDepth) )
        trace( 550, '\t| verticalDepth      :{}\n'.format(self.verticalDepth) )
        trace( 550, '\t| horizontalDeepDepth:{}\n'.format(self.horizontalDeepDepth) )
        trace( 550, '\t| verticalDeepDepth  :{}\n'.format(self.verticalDeepDepth) )
        trace( 500, '-' )
        return

    def _loadIoPadGauge ( self, ioPadGaugeName ):
        trace( 550, ',+', '\tGaugeConf._loadIoPadGauge(): "{}".\n'.format(ioPadGaugeName) )
        self._ioPadGauge = CRL.AllianceFramework.get().getCellGauge( ioPadGaugeName )
        if not self._ioPadGauge:
            print( WarningMessage( 'IO pad gauge "{}" not found.'.format(ioPadGaugeName) ))
        trace( 550, '-' )

    def isHorizontal ( self, layer ):
        mask = layer.getMask()
        for lg in self._routingGauge.getLayerGauges():
            if lg.getLayer().getMask() == mask:
                if lg.getDirection() == RoutingLayerGauge.Horizontal: return True
                return False
        
        print( ErrorMessage( 1, 'GaugeConf.isHorizontal(): Layer "{}" is not part of gauge "{}", cannot know preferred direction.' \
                                .format(layer.getName(), self._routingGauge.getName()) ))
        return False

    def isVertical ( self, layer ): return not self.isHorizontal( layer )

    def createContact ( self, net, x, y, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth

        if self.horizontalDepth > self.verticalDepth: depth -= 1

        trace( 550, '\t%s, horizontalDepth:%d, gaugeDepth:%d\n'
               % (self._routingGauge,self.horizontalDepth,self._routingGauge.getDepth()))
        return Contact.create( net
                             , self._routingGauge.getContactLayer(depth)
                             , x, y
                             , self._routingGauge.getLayerGauge(depth).getViaWidth()
                             , self._routingGauge.getLayerGauge(depth).getViaWidth()
                             )

    def getTrack ( self, u, depth, offset ):
        """
        Returns the y/x axis position of the H/V track nearest to ``u`` (y/x)
        with an offset of ``offset`` tracks applied.
        """
        trace( 550, '\tGaugeConf.getTrack(): u={}, depth={}, offset={}' \
                    .format( DbU.getValueString(u), depth, offset ))
        rg = self._routingGauge.getLayerGauge( depth )
        if rg.getDirection() == RoutingLayerGauge.Horizontal:
            bbMin = self.routingBb.getYMin()
            bbMax = self.routingBb.getYMax()
        else:
            bbMin = self.routingBb.getXMin()
            bbMax = self.routingBb.getXMax()
        index  = rg.getTrackIndex( bbMin, bbMax, u, RoutingLayerGauge.Nearest )
        utrack = rg.getTrackPosition( bbMin, index )
        trace( 550, ' -> utrack={}\n'.format( DbU.getValueString(utrack) ))
        return utrack + offset*rg.getPitch()

    def getNearestHorizontalTrack ( self, y, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth
        return self.getTrack( y, depth, 0 )

    def getNearestVerticalTrack ( self, x, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.verticalDeepDepth
        else:                           depth = self.verticalDepth
        return self.getTrack( x, depth, 0 )

    def createHorizontal ( self, source, target, y, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth
        
        layer = self._routingGauge.getRoutingLayer(depth)
        
        if flags & GaugeConf.UseContactWidth:
            width = source.getBoundingBox(layer.getBasicLayer()).getHeight()
        else:
            width = self._routingGauge.getLayerGauge(depth).getWireWidth()
        if flags & GaugeConf.ExpandWidth:
            width += DbU.fromLambda( 1.0 )
        
        segment = Horizontal.create( source, target, layer, y, width )
        trace( 550, segment )
        return segment
  
    def createVertical ( self, source, target, x, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.verticalDeepDepth
        else:                           depth = self.verticalDepth
        
        layer = self._routingGauge.getRoutingLayer(depth)
        
        if flags & GaugeConf.UseContactWidth:
            width = source.getBoundingBox(layer.getBasicLayer()).getWidth()
        else:
            width = self._routingGauge.getLayerGauge(depth).getWireWidth()
        if flags & GaugeConf.ExpandWidth:
            width += DbU.fromLambda( 1.0 )
        
        segment = Vertical.create( source, target, layer, x, width )
        trace( 550, segment )
        return segment
  
    def rpAccess ( self, rp, flags ):
        trace( 550, ',+', '\t_rpAccess() %s\n' % str(rp) )
  
        if self._rpToAccess.has_key(rp):
            trace( 550, '-' )
            return self._rpToAccess[rp]
  
        if flags & GaugeConf.DeepDepth:
            hdepth = self.horizontalDeepDepth
            vdepth = self.verticalDeepDepth
        else:
            hdepth = self.horizontalDepth
            vdepth = self.verticalDepth
        
        #hpitch    = self._routingGauge.getLayerGauge(hdepth).getPitch()
        #hoffset   = self._routingGauge.getLayerGauge(hdepth).getOffset()
        #contact1  = Contact.create( rp, self._routingGauge.getContactLayer(0), 0, 0 )
        #midSliceY = contact1.getY() - (contact1.getY() % self._cellGauge.getSliceHeight()) \
        #                                               + self._cellGauge.getSliceHeight() / 2
        #midTrackY = midSliceY - ((midSliceY - hoffset) % hpitch)
        #dy        = midSliceY - contact1.getY()
        #
        #if flags & GaugeConf.OffsetBottom1: dy += hpitch
        #if flags & GaugeConf.OffsetTop1:    dy -= hpitch
        #contact1.setDy( dy )

        yoffset = 0
        if flags & GaugeConf.OffsetBottom1: yoffset =  1
        if flags & GaugeConf.OffsetTop1:    yoffset = -1
        contact1  = Contact.create( rp, self._routingGauge.getContactLayer(0), 0, 0 )
        midSliceY = contact1.getY() - (contact1.getY() % self._cellGauge.getSliceHeight()) \
                                                       + self._cellGauge.getSliceHeight() / 2
        ytrack = self.getTrack( contact1.getY(), self.horizontalDeepDepth, yoffset )
        dy     = ytrack - contact1.getY()
        contact1.setDy( dy )
  
        trace( 550, contact1 )
    
        if flags & GaugeConf.HAccess: stopDepth = hdepth
        else:                         stopDepth = vdepth
        trace( 550, '\tstopDepth:%d\n' % stopDepth )
  
        for depth in range(1,stopDepth):
            rg      = self._routingGauge.getLayerGauge(depth)
            xoffset = 0
            if flags & GaugeConf.OffsetRight1 and depth == 1:
                xoffset = 1
            if rg.getDirection() == RoutingLayerGauge.Horizontal:
                xtrack = self.getTrack( contact1.getX(), depth+1, xoffset )
                ytrack = self.getTrack( contact1.getY(), depth  , 0 )
            else:
                xtrack = self.getTrack( contact1.getX(), depth  , xoffset )
                ytrack = self.getTrack( contact1.getY(), depth+1, 0 )
            contact2 = Contact.create( rp.getNet()
                                     , self._routingGauge.getContactLayer(depth)
                                     , xtrack
                                     , ytrack
                                     , self._routingGauge.getLayerGauge(depth).getViaWidth()
                                     , self._routingGauge.getLayerGauge(depth).getViaWidth()
                                     )
            trace( 550, contact2 )
            if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
                segment = Horizontal.create( contact1
                                           , contact2
                                           , rg.getLayer()
                                           , contact1.getY()
                                           , rg.getWireWidth()
                                           )
                trace( 550, segment )
            else:
                segment = Vertical.create( contact1
                                         , contact2
                                         , rg.getLayer()
                                         , contact1.getX()
                                         , rg.getWireWidth()
                                         )
                trace( 550, segment )
            contact1 = contact2
  
        self._rpToAccess[rp] = contact1
    
        trace( 550, '-' )
        return contact1

    def rpByOccurrence ( self, occurrence, net ):
        plug = occurrence.getEntity()
        if self._plugToRp.has_key(plug):
            rp = self._plugToRp[plug]
        else:
            rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return rp
  
    def rpAccessByOccurrence ( self, occurrence, net, flags ):
        plug = occurrence.getEntity()
        if self._plugToRp.has_key(plug):
            rp = self._plugToRp[plug]
        else:
            rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return self.rpAccess( self.rpByOccurrence(occurrence,net), flags )
  
    def rpByPlug ( self, plug, net ):
        if self._plugToRp.has_key(plug):
            rp = self._plugToRp[plug]
        else:
            occurrence = Occurrence( plug, Path(net.getCell(),'') )
            rp         = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return rp
  
    def rpByPlugName ( self, instance, plugName, net ):
        return self.rpByPlug( getPlugByName(instance,plugName), net )
  
    def rpAccessByPlug ( self, plug, net, flags ):
        return self.rpAccess( self.rpByPlug(plug,net), flags )
  
    def rpAccessByPlugName ( self, instance, plugName, net, flags=0 ):
        return self.rpAccess( self.rpByPlugName(instance,plugName,net), flags )

    def setStackPosition ( self, topContact, x, y ):
        topContact.setX( x )
        topContact.setY( y )
        
        count = 0
        for component in topContact.getSlaveComponents():
            segment = component
            count  += 1
        if count > 1:
            message = [ 'GaugeConf::setStackPosition(): There must be exactly one segment connected to contact, not {}.'.format(count)
                      , '+ {}'.format(topContact) ]
            for component in topContact.getSlaveComponents():
                message.append( '| {}'.format(component) )
            raise ErrorMessage( 1, message )
        
        if count == 1:
            if isinstance(segment,Horizontal):
                segment.setY( y )
                segment.getOppositeAnchor( topContact ).setY( y )
            elif isinstance(segment,Vertical):
                segment.setX( x )
                segment.getOppositeAnchor( topContact ).setX( x )
        self.expandMinArea( topContact )
        return

    def expandMinArea ( self, topContact ):
        segments = []
        contacts = [ topContact ]
        i        = 0
        while i < len(contacts):
            for component in contacts[i].getSlaveComponents():
                if not isinstance(component,Horizontal) and not isinstance(component,Vertical):
                    continue
                if component not in segments:
                    segments.append( component )
                if component.getSource() not in contacts:
                    contacts.append( component.getSource() )
                if component.getTarget() not in contacts:
                    contacts.append( component.getTarget() )
            i += 1
        for segment in segments:
            layer     = segment.getLayer()
            wireWidth = segment.getWidth()
            depth     = self._routingGauge.getLayerDepth( layer )
            minArea   = self._routingGauge.getRoutingLayer( depth ).getMinimalArea()
            extension = 0
            if minArea:
                minLength = DbU.fromPhysical( minArea / DbU.toPhysical( wireWidth, DbU.UnitPowerMicro )
                                            , DbU.UnitPowerMicro )
                minLength = toFoundryGrid( minLength, DbU.SnapModeSuperior );
                if isinstance(segment,Horizontal):
                    uMin = segment.getSource().getX()
                    uMax = segment.getTarget().getX()
                    segLength = abs( uMax - uMin )
                    if segLength < minLength:
                        extension = toFoundryGrid( (minLength - segLength)/2, DbU.SnapModeSuperior )
                        if uMin > uMax:
                            extension = - extension
                    segment.setDxSource( -extension )
                    segment.setDxTarget(  extension )
                if isinstance(segment,Vertical):
                    uMin = segment.getSource().getY()
                    uMax = segment.getTarget().getY()
                    segLength = abs( uMax - uMin )
                    if segLength < minLength:
                        extension = toFoundryGrid( (minLength - segLength)/2, DbU.SnapModeSuperior )
                        if uMin > uMax:
                            extension = - extension
                    segment.setDySource( -extension )
                    segment.setDyTarget(  extension )


# -------------------------------------------------------------------
# Class  :  "IoPadConf".

class IoPadConf ( object ):
    """
    Store all informations related to an I/O pad. It's side, position
    and connected nets.

    .. code-block:: python

       # | Side       | Pos | Instance | Pad net | To Core net   | Enable Net     | From Core Net |
         ( IoPin.SOUTH, None, 'a_0'    , 'a(0)'  , 'a(0)'        )
         ( IoPin.NORTH, None, 'p_r0'   , 'r0'    , 'r0_from_pads', 'shift_r'      , 'r0_to_pads'  )
         ( IoPin.NORTH, None, 'p_y0'   , 'y(0)'  , 'y_oe'        , 'y_to_pads(0)' )

    self._datas is a table of 8 elements, the seven first coming from
    the configuration itself. Direction are taken from the core point
    of view.
    
    Meaning of the table element's:
    
    +---------+-----------------------------------------------------------+
    |  Index  |  Type                                                     |
    +=========+===========================================================+
    |    0    |  Pad side (north, south, east, west)                      |
    +---------+-----------------------------------------------------------+
    |    1    |  Pad absolute position on the side, or None               |
    +---------+-----------------------------------------------------------+
    |    2    |  Pad instance name                                        |
    +---------+-----------------------------------------------------------+
    |    3    |  Pad connected signal name.                               |
    |         |  The name of the external signal at chip level            |
    +---------+-----------------------------------------------------------+
    |    4    |  The name of the signal going *from* the pad to the core. |
    |         |  IN direction in the core                                 |
    +---------+-----------------------------------------------------------+
    |    5    |  The name of the signal going *to* the pad from the core. |
    |         |  OUT direction in core (or None)                          |
    +---------+-----------------------------------------------------------+
    |    6    |  The enable signal, coming from the core (or None)        |
    +---------+-----------------------------------------------------------+
    |    7    |  The list of associated IoPads objects. It is set to []   |
    |         |  initially. There may be more than one in the case of     |
    |         |  meta-generated power/ground/clock pads                   |
    +---------+-----------------------------------------------------------+
    """
    CORE_POWER  = 0x0001
    CORE_GROUND = 0x0002
    IO_POWER    = 0x0004
    IO_GROUND   = 0x0008
    ALL_POWER   = 0x0010
    ALL_GROUND  = 0x0020
    CLOCK       = 0x0040
    TRISTATE    = 0x0080
    BIDIR       = 0x0100

    def __init__ ( self, datas ):
        if not isinstance(datas,tuple):
            raise ErrorMessage( 1, [ 'IoPadConf.__init__(): The "datas" parameter is not a tuple.'
                                   , str(datas) ] )
        if len(datas) < 5 and len(datas) > 8:
            raise ErrorMessage( 1, [ 'IoPadConf.__init__(): The "datas" list must have between 5 to 7 elements.'
                                   , str(datas) ] )
        self.flags  = 0
        self.index  = None
        self._datas = list( datas )
        if   len(self._datas) == 4: self._datas += [ None, None, None ]
        if   len(self._datas) == 5: self._datas += [ None, None ]
        elif len(self._datas) == 6: self._datas.insert( 5, None )
        self._datas.append( [] )
        reSpecialPads = re.compile( r'^(?P<type>.+)_(?P<index>[\d+])$' )
        m = reSpecialPads.match( self.instanceName )
        if m:
            self.index = m.group('index')
            if m.group('type') == 'allpower':  self.flags |= IoPadConf.ALL_POWER
            if m.group('type') == 'iopower':   self.flags |= IoPadConf.IO_POWER
            if m.group('type') == 'power':     self.flags |= IoPadConf.CORE_POWER
            if m.group('type') == 'allground': self.flags |= IoPadConf.ALL_GROUND
            if m.group('type') == 'ioground':  self.flags |= IoPadConf.IO_GROUND
            if m.group('type') == 'ground':    self.flags |= IoPadConf.CORE_GROUND
            if m.group('type') == 'clock' :    self.flags |= IoPadConf.CLOCK
        else:
            if   self._datas[5] is not None: self.flags |= IoPadConf.BIDIR
            elif self._datas[6] is not None: self.flags |= IoPadConf.TRISTATE
        trace( 550, '\tIoPadConf._datas: {}\n'.format(self._datas) )
  
    @property
    def side ( self ): return self._datas[0]
  
    @property
    def position ( self ): return self._datas[1]
  
    @property
    def instanceName ( self ): return self._datas[2]
  
    @property
    def padNetName ( self ): return self._datas[3]
  
    @property
    def fromCoreNetName ( self ): return self._datas[4]
  
    @property
    def toCoreNetName ( self ): return self._datas[5]
  
    @property
    def enableNetName ( self ): return self._datas[6]
  
    @property
    def padSupplyNetName ( self ): return self._datas[3]
  
    @property
    def coreSupplyNetName ( self ):
        if self._datas[4] is not None: return self._datas[4]
        return self._datas[3]
  
    @property
    def padClockNetName ( self ): return self._datas[4]
  
    @property
    def coreClockNetName ( self ): return self._datas[5]
  
    @property
    def nets ( self ): return self._datas[4:7]
  
    @property
    def pads ( self ): return self._datas[7]

    def isCorePower  ( self ): return self.flags & IoPadConf.CORE_POWER
    def isIoPower    ( self ): return self.flags & IoPadConf.IO_POWER
    def isAllPower   ( self ): return self.flags & IoPadConf.ALL_POWER
    def isCoreGround ( self ): return self.flags & IoPadConf.CORE_GROUND
    def isIoGround   ( self ): return self.flags & IoPadConf.IO_GROUND
    def isAllGround  ( self ): return self.flags & IoPadConf.ALL_GROUND
    def isClock      ( self ): return self.flags & IoPadConf.CLOCK
    def isTristate   ( self ): return self.flags & IoPadConf.TRISTATE
    def isBidir      ( self ): return self.flags & IoPadConf.BIDIR
  
    def __repr__ ( self ):
        s = '<IoPadConf {} pad:{} from:{}'.format(self.instanceName,self.padNetName,self.fromCoreNetName)
        if self.isBidir():
          s += ' to:{} en:{}'.format(self.toCoreNetName,self.enableNetName)
        if self.isTristate():
          s += ' en:{}'.format(self.enableNetName)
        s += '>'
        return s


# ----------------------------------------------------------------------------
# Class  :  "configuration.ChipConf".

class ChipConf ( object ):
    """
    Store the configuration for a complete chip, I/O pads and core/chip
    sizes mostly.
    """

    def __init__ ( self, blockConf ):
        self.blockConf    = blockConf
        self.name         = 'chip'
        self.ioPadGauge   = None
        self.padInstances = []
        self.southPads    = []
        self.northPads    = []
        self.eastPads     = []
        self.westPads     = []

    def __setattr__ ( self, attr, value ):
        object.__setattr__( self, attr, value )
        if attr == 'ioPadGauge' and value is not None:
            self.blockConf._loadIoPadGauge( value )

    def addIoPad ( self, spec, specNb ):
        """
        Add an I/O pad specification. The spec argument must be of the form:
        """
        ioPadConf = IoPadConf( spec ) 
        if   spec[0] & IoPin.SOUTH: self.southPads.append( ioPadConf )
        elif spec[0] & IoPin.NORTH: self.northPads.append( ioPadConf )
        elif spec[0] & IoPin.EAST:  self.eastPads .append( ioPadConf )
        elif spec[0] & IoPin.WEST:  self.westPads .append( ioPadConf )
        else:
            raise ErrorMessage( 1, [ 'ChipConf.addIoPad(): Unspectified side for {}'.format(ioPadConf)
                                   , '(must be NORTH, SOUTH, EAST or WEST)' ] )
        self.padInstances.append( ioPadConf )


# ----------------------------------------------------------------------------
# Class  :  "configuration.BufferConf".

class BufferConf ( object ):
    """
    Store informations on the buffer(s) to use for Net buffering operations
    and clock trees.
    """

    def __init__ ( self, framework ):
        trace( 550, ',+', '\tBufferConf.__init__()\n' )
        self.maxSinks   = Cfg.getParamInt('spares.maxSinks').asInt()
        self.masterCell = framework.getCell( Cfg.getParamString('spares.buffer').asString()
                                           , CRL.Catalog.State.Views )
        if not self.masterCell:
            trace( 550, '-' )
            raise ErrorMessage( 3, [ 'BufferConf.__init__(): Buffer cell "{}" not found in library,' \
                                     .format(Cfg.getParamString('spares.buffer').asString())
                                   , '           please check the "spares.buffer" configuration parameter in "plugins.py".' ] )
        trace( 550, '\t| masterCell   :{}\n'.format(self.masterCell) ) 
        trace( 550, '\t| maximum sinks:{}\n'.format(self.maxSinks) ) 
        self.count  = 0
        self.input  = None
        self.output = None
        for net in self.masterCell.getNets():
            if not net.isExternal(): continue
            if     net.isGlobal(): continue
            if     net.getDirection() & Net.Direction.IN:  self.input  = net.getName()
            elif   net.getDirection() & Net.Direction.OUT: self.output = net.getName()
        if self.input is None:
            raise ErrorMessage( 3, [ 'BufferConf.__init__(): Cannot guess the input terminal of "{}",' \
                                     .format(Cfg.getParamString('spares.buffer').asString())
                                   , '           please check that the Nets directions are set.' ] )
        if self.output is None:
            raise ErrorMessage( 3, [ 'BufferConf.__init__(): Cannot guess the output terminal of "{}",' \
                                     .format(Cfg.getParamString('spares.buffer').asString())
                                   , '           please check that the Nets directions are set.' ] )
        trace( 550, '\t| input        :"{}"\n'.format(self.input ) ) 
        trace( 550, '\t| output       :"{}"\n'.format(self.output) ) 
        trace( 550, '-' )
        return

    @property
    def name ( self ): return self.masterCell.getName()

    @property
    def width ( self ): return self.masterCell.getAbutmentBox().getWidth()

    @property
    def height ( self ): return self.masterCell.getAbutmentBox().getHeight()

    def createBuffer ( self, cell ):
        """
        Create a new buffer *instance* in Cell. The instance is named "spare_buffer_<Nb>",
        where ``<Nb>`` is an ever incrementing counter (self.count).
        """
        instance = Instance.create( cell, 'spare_buffer_{}'.format(self.count), self.masterCell )
        trace( 550, '\tBufferConf.createBuffer(): cell={}, instance={}\n' \
                    .format( cell, instance ))
        trace( 550, '\tplug={}\n'.format( instance.getPlug( self.masterCell.getNet('q') ) ))
        trace( 550, '\tplug.getCell()={}\n'.format( instance.getPlug( self.masterCell.getNet('q') ).getCell() ))
        self.count += 1
        return instance

    def resetBufferCount ( self ):
        """Reset the buffer instance counter (to use only in case of design reset)."""
        self.count = 0


# ----------------------------------------------------------------------------
# Class  :  "configuration.ConstantsConf".

class ConstantsConf ( object ):
    """
    Store informations on cells used to generate constant signals ("zero"
    and "one")
    """

    ZERO = 1
    ONE  = 2

    def __init__ ( self, framework, cfg ):
        trace( 550, ',+', '\tConstantsConf.__init__()\n' )
        cfg.etesian.cell.zero = None
        cfg.etesian.cell.one  = None
        self.zeroCell = framework.getCell( cfg.etesian.cell.zero, CRL.Catalog.State.Views )
        self.oneCell  = framework.getCell( cfg.etesian.cell.one , CRL.Catalog.State.Views )
        if not self.zeroCell:
            trace( 550, '-' )
            raise ErrorMessage( 3, [ 'ConstantsConf.__init__(): Zero cell "{}" not found in library,' \
                                     .format(cfg.etesian.cell.zero)
                                   , '           please check the "etesian.cell.zero" configuration parameter in "etesian.py".' ] )
        if not self.oneCell:
            trace( 550, '-' )
            raise ErrorMessage( 3, [ 'ConstantsConf.__init__(): One cell "{}" not found in library,' \
                                     .format(cfg.etesian.cell.zero)
                                   , '           please check the "etesian.cell.one" configuration parameter in "etesian.py".' ] )
        trace( 550, '\t| zeroCell:{}\n'.format(self.zeroCell) ) 
        trace( 550, '\t| oneCell :{}\n'.format(self.oneCell ) ) 
        self.zeroCount  = 0
        self.oneCount   = 0
        self.zeroOutput = findCellOutput( self.zeroCell, 'ConstantsConf.__init__()', 'etesian.cell.zero' )
        self.oneOutput  = findCellOutput( self.oneCell , 'ConstantsConf.__init__()', 'etesian.cell.one'  )
        trace( 550, '\t| zeroOutput:"{}"\n'.format(self.zeroOutput) ) 
        trace( 550, '\t| oneOutput :"{}"\n'.format(self.oneOutput ) ) 
        trace( 550, '-' )
        return

    def name ( self, cellType ):
        """Returns the master cell *name* of the selected type."""
        if   cellType == ConstantsConf.ZERO: return self.zeroCell.getName()
        elif cellType == ConstantsConf.ONE : return self.oneCell .getName()
        return None

    def output ( self, cellType ):
        """Returns the master cell *output* of the selected type."""
        if   cellType == ConstantsConf.ZERO: return self.zeroOutput
        elif cellType == ConstantsConf.ONE : return self.oneOutput
        return None

    def width ( self, cellType ):
        """Returns the master cell abutment box width of the selected type."""
        if   cellType == ConstantsConf.ZERO: return self.zeroCell.getAbutmentBox().getWidth()
        elif cellType == ConstantsConf.ONE : return self.oneCell .getAbutmentBox().getWidth()
        return None

    def height ( self, cellType ):
        """Returns the master cell abutment box height of the selected type."""
        if   cellType == ConstantsConf.ZERO: return self.zeroCell.getAbutmentBox().getHeight()
        elif cellType == ConstantsConf.ONE : return self.oneCell .getAbutmentBox().getHeight()
        return None

    def createInstance ( self, cell, cellType ):
        """
        Create a new zero/one *instance* in Cell. The instance is named "constant_<type>_<Nb>",
        where ``<type>`` is the kind of constante (zero or one) and ``<Nb>`` is an ever
        incrementing counter (self.<type>count).
        """
        instance = None
        if cellType == ConstantsConf.ZERO:
            instance = Instance.create( cell, 'constant_zero_{}'.format(self.zeroCount), self.zeroCell )
            self.zeroCount += 1
        elif cellType == ConstantsConf.ONE:
            instance = Instance.create( cell, 'constant_one_{}'.format(self.oneCount), self.oneCell )
            self.oneCount += 1
        return instance

    def resetCounts ( self ):
        """Reset the zero/one instance counters (to use only in case of design reset)."""
        self.zeroCount = 0
        self.oneCount  = 0


# ----------------------------------------------------------------------------
# Class  :  "configuration.FeedsConf".

class FeedsConf ( object ):
    """
    Store informations about feed cells and how to fill a gap.
    """

    def __init__ ( self, framework, cfg ):
        trace( 550, ',+', '\tFeedsConf.__init__()\n' )
        cfg.etesian.feedNames       = None
        cfg.etesian.latchUpDistance = None
        feeds = cfg.etesian.feedNames.split(',')
        self.count = 0
        self.feeds = []
        for feedName in feeds:
            feedCell = framework.getCell( feedName, CRL.Catalog.State.Views )
            if not feedCell:
                print( WarningMessage( 'FeedConf.__init__(): Feed cell "{}" not found in library (skipped).' \
                                       .format(feedName)) )
                continue
            feedWidth = feedCell.getAbutmentBox().getWidth()
            self.feeds.append( (feedWidth,feedCell) )
        if cfg.etesian.latchUpDistance is not None:
            self.maxFeedSpacing = cfg.etesian.latchUpDistance - self.tieWidth()
        self.feeds.sort( key=itemgetter(0) )
        self.feeds.reverse()
        for i in range(len(self.feeds)):
            trace( 550, '\t[{:>2}] {:>10} {}\n' \
                        .format(i,DbU.getValueString(self.feeds[i][0]),self.feeds[i][1]) ) 
        trace( 550, '-' )
        return

    def tieWidth ( self ):
        """Returns the master cell abutment box width of the tie."""
        if self.feeds: return self.feeds[0][0]
        return None

    def createFeed ( self, cell ):
        instance = Instance.create( cell, 'spare_feed_{}'.format(self.count), self.feeds[0][1] )
        self.count += 1
        return instance

    def fillAt ( self, cell, transf, gapWidth ):
        """
        In ``cell``, fill a *one* row gap starting at ``transf`` position and
        of length ``gapWidth``.
        """
        x = transf.getTx()
        while gapWidth > 0:
            feedAdded = False
            for i in range(len(self.feeds)):
                if self.feeds[i][0] <= gapWidth:
                    instance = Instance.create( cell, 'spare_feed_{}'.format(self.count), self.feeds[i][1] )
                    instance.setTransformation( Transformation( x
                                                              , transf.getTy()
                                                              , transf.getOrientation() ))
                    instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                    gapWidth -= self.feeds[i][0]
                    x        += self.feeds[i][0]
                    self.count += 1
                    feedAdded = True
            if not feedAdded: break
        if gapWidth > 0:
            print( WarningMessage( [ 'FeedConf.fillAt(): Unable to fill row gap in "{}".' \
                                     .format(cell.getName())
                                   , '           (@{}, lenght:{})' \
                                     .format(transf,DbU.getValueString(gapWidth)) 
                                   ] ))

    def resetFeedCount ( self ):
        self.count = 0


# ----------------------------------------------------------------------------
# Class  :  "configuration.IoPin".

class IoPin ( object ):
    """
    Create an I/O Pin on a side of a block for one net or a vector of nets.
    """

    SOUTH   = 0x0001
    NORTH   = 0x0002
    EAST    = 0x0004
    WEST    = 0x0008
    A_BEGIN = 0x0010
    A_END   = 0x0020
    A_MASK  = A_BEGIN|A_END

    @staticmethod
    def toStr ( value ):
        s = ''
        for constant, name in ( (IoPin.SOUTH  , 'SOUTH'  )
                              , (IoPin.NORTH  , 'NORTH'  )
                              , (IoPin.EAST   , 'EAST'   )
                              , (IoPin.WEST   , 'WEST'   )
                              , (IoPin.A_BEGIN, 'A_BEGIN')
                              , (IoPin.A_END  , 'A_END'  ) ):
            if value & constant:
                if len(s): s += '|'
                s += 'IoPin.'+name
        return s

    def __init__ ( self, flags, stem, upos, ustep=0, count=1 ):
        """
        Create an I/O Pin(s) on the abutment box of a block. Could be for one
        net or a vector of net.

        :param flags: On which side the pin is to be put (SOUTH, NORTH, EAST or WEST).
        :param stem:  A string giving the name of the net. In case of a vector, it
                      must be a string containing exactly one '{}' as placeholder
                      for the index. 
        :param upos:  Offset from the bottom or left corner of the abutment box side.
        :param ustep: Distance between two consecutive Pins of the vector.
        :param count: Either an integer to be passed to ```range()``` or directly
                      a list of indexes.

        Examples:

        .. code-block:: python

           IoPin( IoPin.WEST , 'debug({})', l(50.0), l(100.0), 16 )
           IoPin( IoPin.EAST , 'debug(0)' , l(45.0),       0 ,  1 )
           IoPin( IoPin.SOUTH, 'adrs({})' , l(50.0), l(100.0), [0, 2, 3, 6] )
        """
        if   flags & IoPin.SOUTH: pass
        elif flags & IoPin.NORTH: pass
        elif flags & IoPin.EAST:  pass
        elif flags & IoPin.WEST:  pass
        else:
            raise ErrorMessage( 1, [ 'IoPin.__init__(): Unsupported direction {} for "{}"'.format(flags,stem)
                                   , '(must be NORTH, SOUTH, EAST or WEST)' ] )
        self.flags = flags
        self.pins  = []
        self.stem  = stem
        self.upos  = upos
        self.ustep = ustep
        self.count = count
        if self.upos == 0 and not (self.flags & IoPin.A_MASK):
            raise ErrorMessage( 1, [ 'IoPin.__init__(): "upos" parameter cannot be zero, corners are forbidden.'
                                   , 'For net "{}"'.format(stem) ] )
        if self.count > 1 and (self.ustep == 0) and not (self.flags & IoPin.A_MASK):
            raise ErrorMessage( 1, [ 'IoPin.__init__(): "ustep" parameter cannot be zero when "count" more than 1.'
                                   , 'For net "{}"'.format(stem) ] )

    @property
    def indexes ( self ):
        if isinstance(self.count,int):
            return range(self.count)
        return self.count

#   def place ( self, block ):
#       """
#       Performs the actual creation of the Pin. Should be called prior to any
#       call to ```Etesian.place()```, so the Pin is taken into account when building
#       the RoutingPads. Returns the number of *failed* pins, so zero means that all
#       went well... Currently only check for out of bounds coordinates.
#       """
#
#       if   self.flags & IoPin.SOUTH: side = block.sides[IoPin.SOUTH]
#       elif self.flags & IoPin.NORTH: side = block.sides[IoPin.NORTH]
#       elif self.flags & IoPin.EAST:  side = block.sides[IoPin.EAST ]
#       elif self.flags & IoPin.WEST:  side = block.sides[IoPin.WEST ]
#       status = 0
#       if isinstance(self.count,int):
#           indexes = range(self.count)
#       else:
#           indexes = self.count
#       if self.flags & (IoPin.NORTH | IoPin.SOUTH):
#           gauge = block.conf.gaugeConf.vDeepRG
#           for index in indexes:
#               pinName  = self.stem.format(index)
#               net      = block.conf.cell.getNet( pinName )
#               if net is None:
#                   print( ErrorMessage( 1, [ 'IoPin.place(): No net named "{}".'.format(pinName) ] ))
#                   continue
#               pinName += '.{}'.format(block.conf.getIoPinsCounts(net))
#               pinPos   = side.getNextPinPosition( self.flags, self.upos, self.ustep )
#               if pinPos.getX() > block.conf.xMax or pinPos.getX() < block.conf.xMin:
#                   print( ErrorMessage( 1, [ 'IoPin.place(): Pin "{}" is outside north or south abutment box side.'.format(pinName)
#                                           , '(x:"{}", AB xMax:{})'.format(DbU.getValueString(pinPos.getX()),DbU.getValueString(block.conf.xMax)) ] ))
#                   status += 1
#               trace( 550, '\tIoPin.place() N/S @{} "{}" of "{}".\n'.format(pinPos,pinName,net) )
#               pin = Pin.create( net
#                               , pinName
#                               , side.pinDirection
#                               , Pin.PlacementStatus.FIXED
#                               , gauge.getLayer()
#                               , pinPos.getX()
#                               , pinPos.getY()
#                               , gauge.getWireWidth()
#                               , gauge.getWireWidth() / 2
#                               )
#               NetExternalComponents.setExternal( pin )
#               side.append( self.flags, pin )
#               block.conf.incIoPinsCounts( net )
#               if self.upos: self.upos + self.ustep
#       else:
#           gauge = block.conf.gaugeConf.hDeepRG
#           for index in indexes:
#               pinName  = self.stem.format(index)
#               net      = block.conf.cell.getNet( pinName )
#               if net is None:
#                   print( ErrorMessage( 1, [ 'IoPin.place(): No net named "{}".'.format(pinName) ] ))
#                   continue
#               pinName += '.{}'.format(block.conf.getIoPinsCounts(net))
#               pinPos   = side.getNextPinPosition( self.flags, self.upos, self.ustep )
#               if pinPos.getY() > block.conf.yMax or pinPos.getY() < block.conf.yMin:
#                   print( ErrorMessage( 1, [ 'IoPin.place(): Pin "{}" is outside east or west abutment box side.'.format(pinName)
#                                           , '(y:"{}", AB yMax:{})'.format(DbU.getValueString(pinPos.getY()),DbU.getValueString(block.conf.yMax)) ] ))
#                   status += 1
#               trace( 550, '\tIoPin.place() E/W @{} "{}" of "{}".\n'.format(pinPos,pinName,net) )
#               pin = Pin.create( net
#                               , pinName
#                               , side.pinDirection
#                               , Pin.PlacementStatus.FIXED
#                               , gauge.getLayer()
#                               , pinPos.getX()
#                               , pinPos.getY()
#                               , gauge.getWireWidth() / 2
#                               , gauge.getWireWidth()
#                               )
#               NetExternalComponents.setExternal( pin )
#               side.append( self.flags, pin )
#               block.conf.incIoPinsCounts( net )
#               if self.upos: self.upos + self.ustep
#       return status


# ----------------------------------------------------------------------------
# Class  :  "configuration.BlockConf".

class BlockConf ( GaugeConf ):
    """
    BlockConf centralize all the configurations informations related to a
    given block. It must be derived/build upon a GaugeConf class.

    It contains:

    =================  ========================================
    Attribute          Information
    =================  ========================================
    ``framework``      The Framework we are using.
    ``gaugeConf``      The routing Gauge & Cell gauge.
    ``bufferConf``     The interface of the buffer cell.
    ``chip``           The optional chip configuration.
    ``cell``           The block we are working on.
    =================  ========================================
    """
    
    def __init__ ( self, cell, ioPins=[], ioPads=[] ):
        super(BlockConf,self).__init__()
        self.validated     = True
        self.editor        = None
        self.framework     = CRL.AllianceFramework.get()
        self.cfg           = CfgCache('',Cfg.Parameter.Priority.Interactive)
        self.bufferConf    = BufferConf( self.framework )
        self.constantsConf = ConstantsConf( self.framework, self.cfg )
        self.feedsConf     = FeedsConf( self.framework, self.cfg )
        self.chipConf      = ChipConf( self )
        self.bColumns      = 2
        self.bRows         = 2
        self.cloneds       = []
        self.cell          = cell
        self.icore         = None
        self.icorona       = None
        self.chip          = None
        self.fixedWidth    = None
        self.fixedHeight   = None
        self.placeArea     = None
        self.deltaAb       = [ 0, 0, 0, 0 ]
        self.useClockTree  = False
        self.useHFNS       = False
        self.useSpares     = True
        self.isBuilt       = False
        self.ioPins        = []
        self.ioPinsCounts  = {}
        for ioPinSpec in ioPins:
            self.ioPins.append( IoPin( *ioPinSpec ) )
        for line in range(len(ioPads)):
            self.chipConf.addIoPad( ioPads[line], line )
        self.cfg.etesian.aspectRatio     = None
        self.cfg.etesian.spaceMargin     = None
        self.cfg.etesian.latchUpDistance = None
        self.cfg.block.spareSide         = None
        self.etesian = None
        self.katana  = None

    @property
    def isCoreBlock ( self ): return self.chip is not None

    @property
    def bufferWidth ( self ): return self.bufferConf.width

    @property
    def bufferHeight ( self ): return self.bufferConf.height

    @property
    def xMin ( self ): return self.cell.getAbutmentBox().getXMin()

    @property
    def yMin ( self ): return self.cell.getAbutmentBox().getYMin()

    @property
    def xMax ( self ): return self.cell.getAbutmentBox().getXMax()

    @property
    def yMax ( self ): return self.cell.getAbutmentBox().getYMax()

    @property
    def coreAb ( self ):
        if not hasattr(self,'coreSize'): return Box()
        #trace( 550, '\tcoreAb:[{} {}]\n'.format( DbU.getValueString(self.coreSize[0])
        #                                       , DbU.getValueString(self.coreSize[1]) ))
        return Box( 0, 0, self.coreSize[0], self.coreSize[1] )

    @property
    def coronaAb ( self ):
        if self.corona is None: return Box()
        return self.corona.getAbutmentBox()

    @property
    def chipAb ( self ):
        if not hasattr(self,'chipSize'): return Box()
        return Box( 0, 0, self.chipSize[0], self.chipSize[1] )

    @property
    def corona ( self ): return self.icorona.getMasterCell()

    @property
    def core ( self ): return self.cell

    @property
    def cellPnR ( self ):
        if self.icorona: return self.corona
        return self.cell

    def setEditor ( self, editor ): self.editor = editor

    def refresh ( self, cell=None ):
        if not self.editor: return
        if cell is not None:
            if cell != self.editor.getCell():
                self.editor.setCell( cell )
        self.editor.fit()

    def createBuffer ( self, cell=None ):
        if cell is None: cell = self.cellPnR
        return self.bufferConf.createBuffer( cell )

    def createFeed ( self ):
        return self.feedsConf.createFeed( self.cellPnR )

    def setDeltaAb ( self, dx1, dy1, dx2, dy2 ):
        self.deltaAb = [ dx1, dy1, dx2, dy2 ]

    def incIoPinsCounts ( self, net ):
        if not self.ioPinsCounts.has_key(net):
            self.ioPinsCounts[net] = 0
        self.ioPinsCounts[net] += 1

    def getIoPinsCounts ( self, net ):
        if not self.ioPinsCounts.has_key(net): return 0
        return self.ioPinsCounts[net]

    def resetBufferCount ( self ):
        self.bufferConf.resetBufferCount()

    def addClonedCell ( self, masterCell ):
        if not masterCell in self.cloneds:
            trace( 550, '\tNew cloned cell: "{}"\n'.format(masterCell) )
            self.cloneds.append( masterCell )
        return

    #def rsave ( self, cell, depth ):
    #    """
    #    Save the complete cell hierarchy. Saves only the physical view, except
    #    for the ones that has been cloned (their names should end up by "_cts"),
    #    for which logical and physical views are to be saved. They are completely
    #    new cells.
    #    """
    #    if depth == 0: print( '  o  Block Recursive Save-Cell.' )
    #    flags = CRL.Catalog.State.Physical
    #    if cell.getName().endswith('_cts'): flags |= CRL.Catalog.State.Logical
    #    if cell.isUniquified():             flags |= CRL.Catalog.State.Logical
    #    self.framework.saveCell( cell, flags )
    #    print( '     {}+ {}.'.format(' '*(depth*2), cell.getName() ) )
    #    for instance in  cell.getInstances():
    #        masterCell = instance.getMasterCell()
    #        if not masterCell.isTerminalNetlist():
    #            self.rsave( masterCell, depth+1 )
  
    def save ( self, flags ):
        """
        Frontend to BlockConf.rsave(). Append the "_cts" suffix to the cloned
        cells, then call rsave().
        """
        trace( 550,'\tBlockConf.save() on "{}"\n'.format(self.cell.getName()) )
        for cell in self.cloneds:
            trace( 550, '\tRenaming cloned cell: "{}"\n'.format(cell) )
            cell.setName( cell.getName()+'_cts' )
        if self.chip is None:
            self.cell.setName( self.cell.getName()+'_r' )
        rsave( self.cell, CRL.Catalog.State.Physical|flags )
        return


# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2023, All Rights Reserved
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

import sys
import re
import os.path
from   operator            import itemgetter
from   ...                import Cfg
from   ...Hurricane       import DataBase, Breakpoint, DbU, Box, Transformation, \
                                 Path, Layer, Occurrence, Net,                   \
                                 NetExternalComponents, RoutingPad, Horizontal,  \
                                 Vertical, Contact, Pin, Plug, Instance
from   ...CRL             import AllianceFramework, RoutingLayerGauge, Catalog, \
                                 Gds, Spice
from   ...helpers         import trace, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import CfgCache, CfgDefault, UpdateSession
from   ..                 import getParameter
from   ..rsave            import rsave
from   ..utils            import getPlugByName


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
    OffsetLeft1     = 0x0002
    OffsetRight1    = 0x0004
    OffsetTop1      = 0x0008
    OffsetTop2      = 0x0010
    OffsetBottom1   = 0x0020
    OffsetBottom2   = 0x0040
    OffsetFromSlice = 0x0080
    DeepDepth       = 0x0100
    UseContactWidth = 0x0200
    ExpandWidth     = 0x0400
    SourceExtend    = 0x0800

    def __init__ ( self ):
        self._cellGauge      = None
        self._ioPadGauge     = None
        self._routingGauge   = None
        self._topLayerDepth  = 0
        self._plugToRp       = { }
        self._rpToAccess     = { }
        self._loadRoutingGauge()
        self._routingBb       = Box()
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

    def isTwoMetals   ( self ): return self._routingGauge.isTwoMetals()

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
        gaugeName     = Cfg.getParamString('anabatic.routingGauge').asString()
        cellGaugeName = Cfg.getParamString('anabatic.cellGauge').asString()
        if not cellGaugeName or cellGaugeName == '<undefined>':
            cellGaugeName = gaugeName
        self._cellGauge    = AllianceFramework.get().getCellGauge( cellGaugeName )
        self._routingGauge = AllianceFramework.get().getRoutingGauge( gaugeName )

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
            self.topLayerDepth = self._routingGauge.getDepth() - 1

        self.horizontalDepth     = -1
        self.verticalDepth       = -1
        self.horizontalDeepDepth = -1
        self.verticalDeepDepth   = -1
        for depth in range(0,self.topLayerDepth+1):
            trace( 550, '\tdepth:{} {}\n'.format(depth,self._routingGauge.getLayerGauge(depth) ))
            
            if self._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.Unusable:
                continue
            if self._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.PinOnly:
                continue
            if self._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.BottomPowerSupply:
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
        self._ioPadGauge = AllianceFramework.get().getCellGauge( ioPadGaugeName )
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

    def getHorizontalPitch ( self, flags ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth
        return self._routingGauge.getLayerGauge( depth ).getPitch()

    def getNearestHorizontalTrack ( self, y, flags, offset=0 ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth
        return self.getTrack( y, depth, offset )

    def getNearestVerticalTrack ( self, x, flags, offset=0 ):
        if flags & GaugeConf.DeepDepth: depth = self.verticalDeepDepth
        else:                           depth = self.verticalDepth
        return self.getTrack( x, depth, offset )

    def createHorizontal ( self, source, target, y, flags, dxSource=0 ):
        if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
        else:                           depth = self.horizontalDepth
        layer = self._routingGauge.getRoutingLayer(depth)
        if not dxSource and flags & GaugeConf.SourceExtend:
            dxSource = self._routingGauge.getPitch(layer)
        if flags & GaugeConf.UseContactWidth:
            width = source.getBoundingBox(layer.getBasicLayer()).getHeight()
        else:
            width = self._routingGauge.getLayerGauge(depth).getWireWidth()
        if flags & GaugeConf.ExpandWidth:
            width += DbU.fromLambda( 1.0 )
        segment = Horizontal.create( source, target, layer, y, width )
        segment.setDxSource( -dxSource )
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
  
    def rpAccess ( self, rp, flags, yoffset ):
        trace( 550, ',+', '\tGaugeConf.rpAccess() {}\n'.format( rp ))
        trace( 550, '\tHAccess set : {}\n'.format( flags&GaugeConf.HAccess ))
        startDepth = self.routingGauge.getLayerDepth( rp.getOccurrence().getEntity().getLayer() )
        trace( 550, '\tlayer:{} startDepth:{}\n'.format(rp.getOccurrence().getEntity().getLayer(),startDepth) )
        if rp in self._rpToAccess:
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
        #                                               + self._cellGauge.getSliceHeight() // 2
        #midTrackY = midSliceY - ((midSliceY - hoffset) % hpitch)
        #dy        = midSliceY - contact1.getY()
        #
        #if flags & GaugeConf.OffsetBottom1: dy += hpitch
        #if flags & GaugeConf.OffsetTop1:    dy -= hpitch
        #contact1.setDy( dy )

        rg        = self.routingGauge.getLayerGauge( startDepth )
        rpContact = Contact.create( rp, rg.getLayer(), 0, 0 )

        if startDepth == 0:
            if flags & GaugeConf.OffsetFromSlice:
                sliceY = rpContact.getY() - (rpContact.getY() % self._cellGauge.getSliceHeight())
                if yoffset < 0:
                    sliceY += self._cellGauge.getSliceHeight()
                ytrack = self.getTrack( sliceY, self.horizontalDeepDepth, yoffset )
                trace( 550, '\tyoffset (from slice):{}\n'.format(yoffset) )
                trace( 550, '\tPut on Y-track:{}\n'.format(DbU.getValueString(ytrack)) )
            else:
                if yoffset is None:
                    yoffset = 0
                    if flags & GaugeConf.OffsetBottom1: yoffset = -1
                    if flags & GaugeConf.OffsetBottom2: yoffset = -2
                    if flags & GaugeConf.OffsetTop1:    yoffset =  1
                    if flags & GaugeConf.OffsetTop2:    yoffset =  2
                    trace( 550, '\tyoffset (from flags):{}\n'.format( yoffset ))
                ytrack = self.getTrack( rpContact.getY(), self.horizontalDeepDepth, yoffset )
                trace( 550, '\tyoffset (from contact):{}\n'.format(yoffset) )
                trace( 550, '\tPut on Y-track:{}\n'.format(DbU.getValueString(ytrack)) )
            contact1  = Contact.create( rp.getNet()
                                      , self._routingGauge.getContactLayer( 0 )
                                      , rpContact.getX()
                                      , ytrack
                                      , rg.getViaWidth()
                                      , rg.getViaWidth()
                                      )
            segment = Vertical.create( rpContact
                                     , contact1
                                     , rpContact.getLayer()
                                     , rpContact.getX()
                                     , rg.getWireWidth()
                                     )
            #dy       = ytrack - contact1.getY()
            #trace( 550, '\tPut on Y-tracks:{}\n'.format(DbU.getValueString(ytrack)) )
            #contact1.setDy( dy )
        else:
            ytrack    = self.getTrack( rpContact.getY(), startDepth, 0 )
            #dy        = ytrack - contact1.getY()
            contact1  = Contact.create( rp.getNet()
                                      , self._routingGauge.getContactLayer( startDepth )
                                      , rpContact.getX()
                                      , ytrack
                                      , rg.getViaWidth()
                                      , rg.getViaWidth()
                                      )
            segment = Vertical.create( rpContact
                                     , contact1
                                     , rpContact.getLayer()
                                     , rpContact.getX()
                                     , rg.getWireWidth()
                                     )
            trace( 550, '\trpContact:{}\n'.format( rpContact ))
            trace( 550, '\tcontact1: {}\n'.format( contact1 ))
            trace( 550, '\tsegment:  {}\n'.format( segment ))
        startDepth += 1
        trace( 550, '\tcontact1={}\n'.format( contact1 ))
    
        if flags & GaugeConf.HAccess: stopDepth = hdepth
        else:                         stopDepth = vdepth
        trace( 550, '\trange(startDepth={},stopDepth={})={}\n' \
                    .format( startDepth, stopDepth, list(range(startDepth,stopDepth)) ))

        if startDepth >= stopDepth:
            if not flags & GaugeConf.HAccess:
                contact1.setLayer( rpContact.getLayer() )
            self._rpToAccess[rp] = contact1
            trace( 550, '-' )
            return contact1
  
        for depth in range(startDepth,stopDepth):
            rg      = self.routingGauge.getLayerGauge(depth)
            xoffset = 0
            if flags & GaugeConf.OffsetRight1 and depth == 1:
                xoffset = 1
            if flags & GaugeConf.OffsetLeft1 and depth == 1:
                xoffset = -1
            if rg.getDirection() == RoutingLayerGauge.Horizontal:
                xtrack = self.getTrack( contact1.getX(), depth+1, xoffset )
                ytrack = self.getTrack( contact1.getY(), depth  , 0 )
                trace( 550, '\tHorizontal depth={} xtrack={} ytrack={}\n' \
                            .format(depth,DbU.getValueString(xtrack),DbU.getValueString(ytrack)) )
            else:
                xtrack = self.getTrack( contact1.getX(), depth  , xoffset )
                ytrack = self.getTrack( contact1.getY(), depth+1, 0 )
                trace( 550, '\tVertical depth={} xtrack={} ytrack={}\n' \
                            .format(depth,DbU.getValueString(xtrack),DbU.getValueString(ytrack)) )
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
        if plug in self._plugToRp:
            rp = self._plugToRp[plug]
        else:
            rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return rp
  
    def rpAccessByOccurrence ( self, occurrence, net, flags, yoffset=None ):
        plug = occurrence.getEntity()
        if plug in self._plugToRp:
            rp = self._plugToRp[plug]
        else:
            rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return self.rpAccess( self.rpByOccurrence(occurrence,net), flags, yoffset )
  
    def rpByPlug ( self, plug, net ):
        """
        Create a RoutingPad on a ``Plug`` for ``net``, in the cell owning the net.
        This will be a *top level* RoutingPad, that is one with an empty Path.

        Only creates *one* RoutingPad per Plug. Maintains a lookup table to
        return the one associated to a Plug if it is requested a second time.
        """
        if plug in self._plugToRp:
            rp = self._plugToRp[plug]
        else:
            occurrence = Occurrence( plug, Path(net.getCell(),'') )
            rp         = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
            self._plugToRp[plug] = rp
        return rp
  
    def rpByPlugName ( self, instance, plugName, net ):
        """
        Frontend for ``rpByPlug()``, extract the plug from the pair ``(instance,plugName)``. 
        """
        return self.rpByPlug( getPlugByName(instance,plugName), net )
  
    def rpAccessByPlug ( self, plug, net, flags, yoffset=None ):
        return self.rpAccess( self.rpByPlug(plug,net), flags, yoffset )
  
    def rpAccessByPlugName ( self, instance, plugName, net, flags=0, yoffset=None ):
        """
        Creates a RoutingPad from a Plug (using ``rpByPlug()``) and build a contact
        stack using a relative positionning specified by ``flags``.
        """
        return self.rpAccess( self.rpByPlugName(instance,plugName,net), flags, yoffset )

    def setStackPosition ( self, topContact, x, y ):
        trace( 550, '\tGaugeConf.setStackPosition() @({},{}) for {}\n' \
                    .format(DbU.getValueString(x),DbU.getValueString(y),topContact) )
        lg = self.routingGauge.getLayerGauge( topContact.getLayer().getTop() )
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                topContact.setY( y )
            else:
                topContact.setX( x )
        else:
            trace( 550, '\tNo LayerGauge for top layer\n' )
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
        
        #if count == 1:
        #    if isinstance(segment,Horizontal):
        #        trace( 550, '\tAdjust horizontal slave @{} {}\n' \
        #                    .format(DbU.getValueString(y),segment) )
        #        segment.setY( y )
        #        segment.getOppositeAnchor( topContact ).setY( y )
        #    elif isinstance(segment,Vertical):
        #        trace( 550, '\tAdjust vertical slave @{} {}\n' \
        #                    .format(DbU.getValueString(x),segment) )
        #        segment.setX( x )
        #        segment.getOppositeAnchor( topContact ).setX( x )
        self.expandMinArea( topContact )
        return

    def getStackY ( self, topContact, flags ):
        """
        Get the Y coordinate of a stack contact, on the top level or in
        the deep routing level.

        :param topContact:  The topmost contact of the VIA stack.

        .. note:: A stacked contact is a set of contacts with short
                  segments to connect them. Segments are alternately
                  H & V, according to the routing gauge. Segments are
                  put on the routing tracks, so the X/Y coordinates 
                  of the various contacts *may* slighlty vary.
        """
        trace( 550, '\tgetStackY() {}\n'.format( topContact ))
        y  = topContact.getY()
        lg = self.routingGauge.getLayerGauge( topContact.getLayer().getTop() )
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                y = topContact.getY()
        if not (flags & GaugeConf.DeepDepth):
            return y
        contact = topContact
        depth   = -1
        while contact:
            count   = 0
            segment = None
            for component in contact.getSlaveComponents():
                if isinstance(component,Horizontal):
                    horizontal = component
                    lg         = self.routingGauge.getLayerGauge( horizontal.getLayer() )
                    if depth == -1 or lg.getDepth() < depth:
                        segment = horizontal
                        depth   = lg.getDepth()
                if isinstance(component,Vertical):
                    vertical = component
                    lg       = self.routingGauge.getLayerGauge( vertical.getLayer() )
                    if depth == -1 or lg.getDepth() < depth:
                        segment = vertical
                        depth   = lg.getDepth()
            if not segment:
                return y
            if depth == self.horizontalDeepDepth:
                return segment.getY()
            contact = segment.getOppositeAnchor( contact )
        return y

    def getStackX ( self, topContact, flags ):
        """
        Get the X coordinate of a stack contact, on the top level or in
        the deep routing level.

        :param topContact:  The topmost contact of the VIA stack.

        .. note:: A stacked contact is a set of contacts with short
                  segments to connect them. Segments are alternately
                  H & V, according to the routing gauge. Segments are
                  put on the routing tracks, so the X/Y coordinates 
                  of the various contacts *may* slighlty vary.
        """
        trace( 550, '\tgetStackX() {}\n'.format( topContact ))
        x  = topContact.getX()
        lg = self.routingGauge.getLayerGauge( topContact.getLayer().getTop() )
        if lg:
            if lg.getDirection() == RoutingLayerGauge.Horizontal:
                x = topContact.getX()
        if not (flags & GaugeConf.DeepDepth):
            return y
        contact = topContact
        depth   = -1
        while contact:
            count   = 0
            segment = None
            for component in contact.getSlaveComponents():
                if isinstance(component,Horizontal):
                    horizontal = component
                    lg         = self.routingGauge.getLayerGauge( horizontal.getLayer() )
                    if depth == -1 or lg.getDepth() < depth:
                        segment = horizontal
                        depth   = lg.getDepth()
                if isinstance(component,Vertical):
                    vertical = component
                    lg       = self.routingGauge.getLayerGauge( vertical.getLayer() )
                    if depth == -1 or lg.getDepth() < depth:
                        segment = vertical
                        depth   = lg.getDepth()
            if not segment:
                return x
            if depth == self.verticalDeepDepth:
                return segment.getX()
            contact = segment.getOppositeAnchor( contact )
        return x

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
            trace( 550, '\tGaugeConf.expandMinArea() on {}\n'.format(segment) )
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
                    trace( 550, '\tminLength={}\n'.format(DbU.getValueString(minLength)) )
                    uMin = segment.getSource().getX()
                    uMax = segment.getTarget().getX()
                    segLength = abs( uMax - uMin )
                    if segLength < minLength:
                        trace( 550, '\texpand\n' )
                        extension = toFoundryGrid( (minLength - segLength)//2, DbU.SnapModeSuperior )
                        if uMin > uMax:
                            extension = - extension
                    segment.setDxSource( -extension )
                    segment.setDxTarget(  extension )
                if isinstance(segment,Vertical):
                    uMin = segment.getSource().getY()
                    uMax = segment.getTarget().getY()
                    segLength = abs( uMax - uMin )
                    if segLength < minLength:
                        extension = toFoundryGrid( (minLength - segLength)//2, DbU.SnapModeSuperior )
                        if uMin > uMax:
                            extension = - extension
                    segment.setDySource( -extension )
                    segment.setDyTarget(  extension )


# -------------------------------------------------------------------
# Class  :  "IoPadConf".

class IoPadConf ( object ):
    """
    Store all informations related to an I/O pad. It's side, position
    and connected nets. The kind of pad is guessed from the number of
    nets.

    .. code-block:: python

       # | Side       | Pos | Instance | Pad net | To Core net   | Enable Net     | From Core Net |
         ( IoPin.SOUTH, None, 'a_0'    , 'a(0)'  , 'a(0)'        )
         ( IoPin.NORTH, None, 'p_r0'   , 'r0'    , 'r0_from_pads', 'shift_r'      , 'r0_to_pads'  )
         ( IoPin.NORTH, None, 'p_y0'   , 'y(0)'  , 'y_oe'        , 'y_to_pads(0)' )
         ( IoPin.NORTH, None, 'nc_0'   )

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
    NON_CONNECT = 0x0200

    def __init__ ( self, datas ):
        if   isinstance(datas,list ): self._datas = datas
        elif isinstance(datas,tuple): self._datas = list( datas )
        else:
            raise ErrorMessage( 1, [ 'IoPadConf.__init__(): The "datas" parameter is neither a list nor a tuple.'
                                   , str(datas) ] )
        if len(datas) != 3 and len(datas) < 5 and len(datas) > 8:
            raise ErrorMessage( 1, [ 'IoPadConf.__init__(): The "datas" list must have between 5 to 7 elements.'
                                   , str(datas) ] )
        self.flags  = 0
        self.index  = None
        if   len(self._datas) == 3: self._datas += [ None, None, None, None ]
        if   len(self._datas) == 4: self._datas += [ None, None, None ]
        if   len(self._datas) == 5: self._datas += [ None, None ]
        elif len(self._datas) == 6: self._datas.insert( 5, None )
        self._datas.append( [] )
        m = None
        if isinstance(self.instanceName,str):
            reSpecialPads = re.compile( r'^(?P<type>.+)_(?P<index>[\d+])$' )
            m = reSpecialPads.match( self.instanceName )
        else:
            if self.instanceName is not None:
                self.flags |= IoPadConf.ALL_POWER
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
            if   self._datas[3] is     None: self.flags |= IoPadConf.NON_CONNECT
            elif self._datas[5] is not None: self.flags |= IoPadConf.BIDIR
            elif self._datas[6] is not None: self.flags |= IoPadConf.TRISTATE
        sPos = ''
        if self._datas[1]:
            sPos = DbU.getValueString(self._datas[1])
        trace( 550, '\tIoPadConf._datas: @{} {}\n'.format(sPos,self._datas) )
  
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
    def isNonConnect ( self ): return self.flags & IoPadConf.NON_CONNECT

    def isAnalog ( self ):
        if self._datas[0] is None: return False
        return self._datas[0] & IoPin.ANALOG
  
    def __repr__ ( self ):
        s = '<IoPadConf {} iopad="{}" from="{}"'.format(self.instanceName
                                                       ,self.padNetName
                                                       ,self.fromCoreNetName)
        if self.isBidir():
          s += ' to="{}" en="{}"'.format(self.toCoreNetName,self.enableNetName)
        if self.isTristate():
          s += ' en="{}"'.format(self.enableNetName)
        s += ' flags={:x}>'.format(self.flags)
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
        self.southPins    = []
        self.northPins    = []
        self.eastPins     = []
        self.westPins     = []

    def __setattr__ ( self, attr, value ):
        object.__setattr__( self, attr, value )
        if attr == 'ioPadGauge' and value is not None:
            self.blockConf._loadIoPadGauge( value )

    def ioPadsCount ( self ): return len(self.padInstances)

    def addIoPad ( self, spec, specNb ):
        """
        Add an I/O pad specification. The spec argument must be of the form:
        """
        ioPadConf = IoPadConf( spec ) 
        if spec[0] is not None:
            if   spec[0] & IoPin.SOUTH: self.southPads.append( ioPadConf )
            elif spec[0] & IoPin.NORTH: self.northPads.append( ioPadConf )
            elif spec[0] & IoPin.EAST:  self.eastPads .append( ioPadConf )
            elif spec[0] & IoPin.WEST:  self.westPads .append( ioPadConf )
            else:
                raise ErrorMessage( 1, [ 'ChipConf.addIoPad(): Unspecified side for {}'.format(ioPadConf)
                                       , '(must be NORTH, SOUTH, EAST or WEST)' ] )
        self.padInstances.append( ioPadConf )

    def addHarnessPin ( self, pin, side ):
        """
        Add an Pin to a side. This the terminal pin found in the harness.
        """
        if   side & IoPin.SOUTH: self.southPins.append( pin )
        elif side & IoPin.NORTH: self.northPins.append( pin )
        elif side & IoPin.EAST:  self.eastPins .append( pin )
        elif side & IoPin.WEST:  self.westPins .append( pin )
        else:
            raise ErrorMessage( 1, [ 'ChipConf.addHarnessPin(): Unspecified side for {}'.format(pin)
                                   , '(must be NORTH, SOUTH, EAST or WEST)' ] )


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
                                           , Catalog.State.Views )
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
        trace( 550, '\tplug={}\n'.format( instance.getPlug( self.masterCell.getNet(self.output) ) ))
        trace( 550, '\tplug.getCell()={}\n'.format( instance.getPlug( self.masterCell.getNet(self.output) ).getCell() ))
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
        self.zeroCell = framework.getCell( cfg.etesian.cell.zero, Catalog.State.Views )
        self.oneCell  = framework.getCell( cfg.etesian.cell.one , Catalog.State.Views )
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
        cfg.etesian.defaultFeed     = None
        feeds = cfg.etesian.feedNames.split(',')
        self.count       = 0
        self.feeds       = []
        self.defaultFeed = 0
        for feedName in feeds:
            feedCell = framework.getCell( feedName, Catalog.State.Views )
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
            if self.feeds[i][1].getName() == cfg.etesian.defaultFeed:
                self.defaultFeed = i
        trace( 550, '-' )
        return

    def tieWidth ( self ):
        """Returns the master cell abutment box width of the tie."""
        if self.feeds: return self.feeds[ self.defaultFeed ][0]
        return None

    def createFeed ( self, cell ):
        instance = Instance.create( cell
                                  , 'spare_feed_{}'.format(self.count)
                                  , self.feeds[self.defaultFeed][1] )
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
# Class  :  "configuration.PowersConf".

class PowersConf ( object ):
    """
    Store informations about power cells to build vertical power rails in
    technologies with low number of routing layers.
    """

    def __init__ ( self, framework, cfg ):
        trace( 550, ',+', '\tPowersConf.__init__()\n' )
        cfg.etesian.cell.power = None
        self.power = None
        powerName = cfg.etesian.cell.power
        self.count   = 0
        if cfg.etesian.cell.power:
            self.power   = framework.getCell( powerName, Catalog.State.Views )
            if not self.power:
                print( WarningMessage( 'PowersConf.__init__(): Power cell "{}" not found in library (skipped).' \
                                       .format(powerName)) )
        trace( 550, '-' )
        return

    def createPower ( self, cell ):
        instance = Instance.create( cell, 'power_{}'.format(self.count), self.power )
        self.count += 1
        return instance

    def columnAt ( self, cell, x, y, orient, sliceCount ):
        """
        In ``cell``, build a column which bottom is at ``transf`` position and
        span over ``sliceCount`` slices.
        """
        sliceHeight = self.power.getAbutmentBox().getHeight()
        if orient == Transformation.Orientation.ID:
            offset  = 1
            orients = ( Transformation.Orientation.ID, Transformation.Orientation.MX )
        elif orient == Transformation.Orientation.MX:
            offset  = 1
            orients = ( Transformation.Orientation.MX, Transformation.Orientation.ID )
        elif orient == Transformation.Orientation.R2:
            y      += sliceHeight
            offset  = 0
            orients = ( Transformation.Orientation.R2, Transformation.Orientation.MY )
        elif orient == Transformation.Orientation.MY:
            y      += sliceHeight
            offset  = 0
            orients = ( Transformation.Orientation.MY, Transformation.Orientation.R2 )
        else:
            raise ErrorMessage( [ 'PowersConf.columnAt(): Rotations are not allowed for power columns, in "{}".' \
                                  .format(cell.getName())
                                , '           (@{}, sliceCount:{})' \
                                  .format(orient,sliceCount) 
                                ] )

        for islice in range(sliceCount):
            instance = Instance.create( cell, 'power_{}'.format(self.count), self.power )
            instance.setTransformation( Transformation( x
                                                      , y
                                                      , orients[ islice%2 ] ))
            instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
            if (islice+offset) % 2:
                y += 2*sliceHeight
            self.count += 1

    def resetPowerCount ( self ):
        self.count = 0


# ----------------------------------------------------------------------------
# Class  :  "configuration.IoPin".

class IoPin ( object ):
    """
    Create an I/O Pin on a side of a block for one net or a vector of nets.
    """

    SOUTH     = 0x0001
    NORTH     = 0x0002
    EAST      = 0x0004
    WEST      = 0x0008
    A_BEGIN   = 0x0010
    A_END     = 0x0020
    ANALOG    = 0x0040
    A_MASK    = A_BEGIN|A_END
    SIDE_MASK = EAST|WEST|NORTH|SOUTH

    @staticmethod
    def toStr ( value ):
        s = ''
        for constant, name in ( (IoPin.SOUTH  , 'SOUTH'  )
                              , (IoPin.NORTH  , 'NORTH'  )
                              , (IoPin.EAST   , 'EAST'   )
                              , (IoPin.WEST   , 'WEST'   )
                              , (IoPin.A_BEGIN, 'A_BEGIN')
                              , (IoPin.A_END  , 'A_END'  )
                              , (IoPin.ANALOG , 'ANALOG' ) ):
            if value & constant:
                if len(s): s += '|'
                s += 'IoPin.'+name
        return s

    def __repr__ ( self ):
        s = '<IoPin "{}" {} @({},{},{})>'.format( self.stem
                                                , IoPin.toStr(self.flags)
                                                , DbU.getValueString(self.upos)
                                                , DbU.getValueString(self.ustep)
                                                , self.count )
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
        if isinstance(count,int):
            self.count   = count
            self.indexes = range(self.count)
        else:
            self.count   = len(count)
            self.indexes = count
        if self.upos == 0 and not (self.flags & IoPin.A_MASK):
            raise ErrorMessage( 1, [ 'IoPin.__init__(): "upos" parameter cannot be zero, corners are forbidden.'
                                   , 'For net "{}"'.format(stem) ] )
        if self.count > 1 and (self.ustep == 0) and not (self.flags & IoPin.A_MASK):
            raise ErrorMessage( 1, [ 'IoPin.__init__(): "ustep" parameter cannot be zero when "count" more than 1.'
                                   , 'For net "{}"'.format(stem) ] )

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
#                               , gauge.getWireWidth() // 2
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
#                               , gauge.getWireWidth() // 2
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
        self.framework     = AllianceFramework.get()
        self.cfg           = CfgCache('',Cfg.Parameter.Priority.Interactive)
        # H-Tree/Spares parameters (triggers loading from disk).
        self.cfg.spares.htreeOffsetDriver = None
        self.cfg.spares.htreeOffsetSink   = None
        self.bufferConf    = None
        self.constantsConf = None
        self.feedsConf     = None
        self.powersConf    = None
        self.chipConf      = None
        self.bColumns      = 2
        self.bRows         = 2
        self.sparesTies    = True
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
        self.hTreeDatas    = [ ]
        self.useHFNS       = False
        self.useSpares     = True
        self.trackAvoids   = []
        self.isBuilt       = False
        self.useHarness    = False
        self.ioPins        = []
        self.ioPinsCounts  = {}
        self.ioPinsArg     = ioPins
        self.ioPadsArg     = ioPads
        self.cfg.etesian.aspectRatio      = None
        self.cfg.etesian.spaceMargin      = None
        self.cfg.etesian.latchUpDistance  = None
        self.cfg.block.spareSide          = None
        self.cfg.block.vRailsPeriod       = None
        self.cfg.katana.dumpMeasures      = None
        self.cfg.spares.useFeedTrackAvoid = CfgDefault(False)
        self.cfg.spares.htreeRootOffset   = CfgDefault(3)
        self.cfg.spares.htreeOffset       = CfgDefault(5)
        self.chipConf = ChipConf( self )
        self.etesian  = None
        self.katana   = None

    def _postInit ( self ):
        trace( 550, ',+', '\tblock.configuration._postInit()\n' )
        self.cfg.apply()
        self.bufferConf    = BufferConf( self.framework )
        self.constantsConf = ConstantsConf( self.framework, self.cfg )
        self.feedsConf     = FeedsConf( self.framework, self.cfg )
        self.powersConf    = PowersConf( self.framework, self.cfg )
        for ioPinSpec in self.ioPinsArg:
            self.ioPins.append( IoPin( *ioPinSpec ) )
        for line in range(len(self.ioPadsArg)):
            self.chipConf.addIoPad( self.ioPadsArg[line], line )
        trace( 550, ',-' )

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
        trace( 550, '\tcoreAb:[{} {}]\n'.format( DbU.getValueString(self.coreSize[0])
                                               , DbU.getValueString(self.coreSize[1]) ))
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

    def createPower ( self ):
        return self.powersConf.createPower( self.cellPnR )

    def setDeltaAb ( self, dx1, dy1, dx2, dy2 ):
        self.deltaAb = [ dx1, dy1, dx2, dy2 ]

    def incIoPinsCounts ( self, net ):
        if not net in self.ioPinsCounts:
            self.ioPinsCounts[net] = 0
        self.ioPinsCounts[net] += 1

    def getIoPinsCounts ( self, net ):
        if not net in self.ioPinsCounts: return 0
        return self.ioPinsCounts[net]

    def resetBufferCount ( self ):
        self.bufferConf.resetBufferCount()

    def addClonedCell ( self, masterCell ):
        if not masterCell in self.cloneds:
            trace( 550, '\tNew cloned cell: "{}"\n'.format(masterCell) )
            self.cloneds.append( masterCell )
        return

    def useHTree ( self, netName, flags=0 ):
        for item in self.hTreeDatas:
            if item[0] == netName:
                print( WarningMessage( 'block.configuration.useHTree(): Redefinition of "{}" H-Tree ignored.' \
                                       .format(netName)) )
                return
        self.hTreeDatas.append( [ netName, flags ] );

    def addTrackAvoid ( self, trackAvoid ):
        if self.cfg.anabatic.netBuilderStyle == 'VH,2RL':
            self.trackAvoids.append( trackAvoid )
  
    def save ( self, flags ):
        """
        Frontend to BlockConf.rsave(). Append the "_cts" suffix to the cloned
        cells, then call rsave().
        """
        trace( 550,'\tBlockConf.save() on "{}"\n'.format(self.cell.getName()) )
        views = Catalog.State.Logical
        if self.routingGauge.isSymbolic():
            views = views | Catalog.State.Physical
        for cell in self.cloneds:
            trace( 550, '\tRenaming cloned cell: "{}"\n'.format(cell) )
            cell.setName( cell.getName()+'_cts' )
        if self.chip is None:
            topCell = self.cell
            self.cell.setName( self.cell.getName()+'_r' )
            rsave( topCell, views|flags )
        else:
            topCell = self.chip
            if not self.useHarness:
                self.corona.setName( self.corona.getName()+'_r' )
                self.chip  .setName( self.chip  .getName()+'_r' )
            rsave( self.corona, views|flags, enableSpice=True )
            rsave( self.chip  , views|flags, enableSpice=True )
        if not self.routingGauge.isSymbolic():
            print( '     + {} (GDSII).'.format( topCell.getName() ))
            Gds.save( topCell )
        Spice.clearProperties()
        return
        
    def toXPitch ( self, x, superior=False ):
        """
        Returns the coordinate of the pitch immediately inferior to X.
        Compute in the "P&R cell" coordinate system which may be core
        or corona.
        """
        area   = self.cellPnR.getAbutmentBox()
        modulo = (x - area.getXMin()) % self.sliceStep 
        if modulo and superior:
            x += self.sliceStep
        return x - modulo

    def toYSlice ( self, y, superior=False ):
        """
        Returns the coordinate of the slice immediately inferior to Y.
        Compute in the "P&R cell" coordinate system which may be core
        or corona.
        """
        area   = self.cellPnR.getAbutmentBox()
        modulo = (y - area.getYMin()) % self.sliceHeight 
        if modulo and superior:
            y += self.sliceHeight
        return y - modulo

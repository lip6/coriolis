
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
# |  Python      :       "./plugins/chip/power.py"                  |
# +-----------------------------------------------------------------+


import sys
from   ...Hurricane       import DbU, Point, Transformation, Box, Interval, \
                                 Path, Occurrence, UpdateSession, Net,      \
                                 Contact, Horizontal, Vertical, Query
from   ...helpers         import trace
from   ...helpers.io      import ErrorMessage, WarningMessage
from   ...helpers.overlay import UpdateSession
from   .constants         import importConstants

__all__ = [ 'Builder' ]

importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "power.Side"

class Side ( object ):

    def __init__ ( self, builder, side, net, metal ):
        self.builder    = builder
        self.side       = side
        self.net        = net
        self.metal      = metal
        self.deltaWidth = metal.getExtentionWidth()*2
        self.terminals  = []
  
    def addTerminal ( self, position, width ):
        toMerge = Interval( position-width//2, position+width//2 )
        length  = len(self.terminals)
        imerge  = length
        ichunk  = 0
        while ichunk < length:
            if imerge == length:
                if toMerge.getVMax() < self.terminals[ichunk][0].getVMin():
                    self.terminals.insert( ichunk, [ toMerge, None ] )
                    imerge  = ichunk
                    length += 1
                    break
                if toMerge.intersect(self.terminals[ichunk][0]):
                    imerge = ichunk
                    self.terminals[ichunk][0].merge( toMerge )
            else:
               if toMerge.getVMax() >= self.terminals[ichunk][0].getVMin():
                   self.terminals[imerge][0].merge( self.terminals[ichunk][0] )
                   del self.terminals[ichunk]
                   length -= 1
                   continue
               else:
                   break
            ichunk += 1
        if ichunk == length:
            self.terminals.append( [ toMerge, None ] )
  
    def doLayout ( self ):
        if self.side == West:
            width = 0
            x     = self.builder.icoreAb.getXMin()
        elif self.side == East:
            width = 0
            x     = self.builder.icoreAb.getXMax()
        elif self.side == South:
            height = 0
            y      = self.builder.icoreAb.getYMin()
        elif self.side == North:
            height = 0
            y      = self.builder.icoreAb.getYMax()
        minWidth = DbU.fromLambda( 6.0 )
        for terminal in self.terminals:
            if self.side == West or self.side == East:
                center = Point( x, terminal[0].getCenter() )
                height = terminal[0].getSize() - self.deltaWidth
                if height < minWidth: height = minWidth
            elif self.side == North or self.side == South:
                center = Point( terminal[0].getCenter(), y )
                width  = terminal[0].getSize() - self.deltaWidth
                if width < minWidth: width = minWidth
            self.builder.path.getTransformation().applyOn( center )
            contact = Contact.create( self.net, self.metal, center.getX(), center.getY(), width, height )
            terminal[ 1 ] = contact


# --------------------------------------------------------------------
# Class  :  "power.Plane"

class Plane ( object ):
  
    Horizontal = 1
    Vertical   = 2
  
    def __init__ ( self, builder, metal ):
        self.builder = builder
        self.metal   = metal
        self.sides   = {}
  
    def addTerminal ( self, net, direction, bb ):
        if not net in self.sides:
            self.sides[ net ] = { North : Side(self.builder,North,net,self.metal)
                                , South : Side(self.builder,South,net,self.metal)
                                , East  : Side(self.builder,East ,net,self.metal)
                                , West  : Side(self.builder,West ,net,self.metal)
                                }
        sides = self.sides[ net ]
        trace( 550, '\tPlane.addTerminal() net={} bb={} direction={}\n' \
                    .format( net.getName(), bb, direction ))
        trace( 550, '\tbuilder.icoreAb={}\n'.format( self.builder.icoreAb ))
        if direction == Plane.Horizontal:
            trace( 550, '\t| Horizontal\n' )
            if bb.getXMin() <= self.builder.icoreAb.getXMin():
                sides[West].addTerminal( bb.getCenter().getY(), bb.getHeight() )
            if bb.getXMax() >= self.builder.icoreAb.getXMax():
                sides[East].addTerminal( bb.getCenter().getY(), bb.getHeight() )
        if direction == Plane.Vertical:
            trace( 550, '\t| Vertical\n' )
            if bb.getYMin() <= self.builder.icoreAb.getYMin():
                sides[South].addTerminal( bb.getCenter().getX(), bb.getWidth() )
            if bb.getYMax() >= self.builder.icoreAb.getYMax():
                sides[North].addTerminal( bb.getCenter().getX(), bb.getWidth() )
        trace( 550, '\tTerminal added\n' )
  
    def doLayout ( self ):
        for sidesOfNet in self.sides.values():
            for side in sidesOfNet.values():
                side.doLayout()


# --------------------------------------------------------------------
# Class  :  "power.Plane"

class GoCb ( object ):
  
    def __init__ ( self, builder ):
        self.builder = builder
  
    def __call__ ( self, query, go ):
        direction = None
        if isinstance(go,Horizontal): direction = Plane.Horizontal
        if isinstance(go,Vertical):   direction = Plane.Vertical
        if not direction: return
        trace( 550, '\t| go={}\n'.format( go ))
        rootNet = None
        if go.getNet().getType() == int(Net.Type.POWER):  rootNet = self.builder.conf.coronaVdd
        if go.getNet().getType() == int(Net.Type.GROUND): rootNet = self.builder.conf.coronaVss
        if not rootNet: return
        if self.builder.activePlane:
            layer = self.builder.activePlane.metal
            if layer.isSymbolic():
                layer = layer.getBasicLayer()
            if self.builder.conf.getLayerDepth(layer) == 0:
                direction = Plane.Horizontal
            bb = go.getBoundingBox( layer )
            query.getPath().getTransformation().applyOn( bb )
            self.builder.activePlane.addTerminal( rootNet, direction, bb )
        else:
            print( WarningMessage( 'BlockPower.GoCb() callback called without an active plane.' ))
        return


# --------------------------------------------------------------------
# Class  :  "power.Builder"

class Builder ( object ):

    def __init__ ( self, conf ):
        self.conf        = conf
        self.path        = Path()
        self.corona      = self.conf.icorona.getMasterCell()
        self.icoreAb     = self.conf.icore.getAbutmentBox()
        self.planes      = {}
        self.activePlane = None
        for layerGauge in self.conf.routingGauge.getLayerGauges():
          self.planes[ layerGauge.getLayer().getName() ] = Plane( self, layerGauge.getLayer() )
  
    def connectPower ( self ):
        if not self.conf.coronaVdd or not self.conf.coronaVss:
            raise ErrorMessage( 1, 'Cannot build block power terminals as core vdd and/or vss are not known.' )
            return
        goCb  = GoCb( self )
        query = Query()
        query.setGoCallback( goCb )
        query.setCell( self.corona )
        query.setArea( self.icoreAb )
        query.setFilter( Query.DoComponents|Query.DoTerminalCells )
        for layerGauge in self.conf.routingGauge.getLayerGauges():
            self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
            layer = layerGauge.getLayer()
            if layer.isSymbolic():
                layer = layer.getBasicLayer()
            query.setBasicLayer( layer )
            query.doQuery()
        self.activePlane = None
  
    def _connectClock ( self, ck ):
        trace( 550, '\tpower.Builder._connectClock() {}\n'.format(ck) )
        blockCk = None
        for plug in self.conf.icore.getPlugs():
            if plug.getNet() == ck:
                blockCk = plug.getMasterNet()
        if not blockCk:
            raise ErrorMessage( 1, 'Block "{}" has no net connected to the clock "{}".' \
                                   .format(self.conf.icore.getName(),ck.getName()) )
            return
        htPlugs = []
        for plug in ck.getPlugs():
            if plug.getInstance().isTerminalNetlist():
                htPlugs.append( plug )
        if len(htPlugs) != 1:
            message = [ 'Clock "{}" of block "{}" is not organized as a H-Tree ({} plugs).' \
                        .format( ck.getName()
                               , self.conf.icore.getName()
                               , len(htPlugs)) ]
            for plug in htPlugs:
                message += [ '\n        - {} {}'.format(plug,plug.getInstance()) ]
            raise ErrorMessage( 1, message )
            return
        with UpdateSession():
            bufferRp = self.conf.rpAccessByOccurrence( Occurrence(htPlugs[0], Path()), ck, 0 )
            trace( 550, '\tClock buffer access RP @{}\n'.format(bufferRp) )
            self.conf.expandMinArea( bufferRp )
            layerGauge = self.conf.routingGauge.getLayerGauge(self.conf.verticalDepth)
            contact    = Contact.create( ck
                                       , self.conf.routingGauge.getRoutingLayer(self.conf.verticalDepth)
                                       , bufferRp.getX()
                                       , self.icoreAb.getYMax() 
                                       , layerGauge.getViaWidth()
                                       , layerGauge.getViaWidth()
                                       )
            segment = self.conf.createVertical( bufferRp, contact, bufferRp.getX(), 0 )
            self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
            layer = self.activePlane.metal
            if layer.isSymbolic():
                layer = layer.getBasicLayer()
            bb = segment.getBoundingBox( layer )
            self.activePlane.addTerminal( ck, Plane.Vertical, bb )
            trace( 550, '\tAdded terminal of {} to vertical plane @{}\n'.format(ck,bb) )
       #Breakpoint.stop( 99, '_connectClock() on {} done.'.format(ck) )
  
    def connectClocks ( self ):
        if not self.conf.useClockTree:
            print( WarningMessage( "Clock tree generation has been disabled ('chip.clockTree':False)." ))
            return
        if len(self.conf.coronaCks) == 0:
            raise ErrorMessage( 1, 'Cannot build clock terminal as no clock is not known.' )
            return
        for ck in self.conf.coronaCks:
            self._connectClock( ck )
  
    def doLayout ( self ):
        with UpdateSession():
            for plane in self.planes.values():
                plane.doLayout()

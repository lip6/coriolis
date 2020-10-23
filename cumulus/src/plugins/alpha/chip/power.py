#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/blockpower.py"             |
# +-----------------------------------------------------------------+


import sys
from   Hurricane  import DbU
from   Hurricane  import Point
from   Hurricane  import Transformation
from   Hurricane  import Box
from   Hurricane  import Interval
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import UpdateSession
from   Hurricane  import Net
from   Hurricane  import Contact
from   Hurricane  import Horizontal
from   Hurricane  import Vertical
from   Hurricane  import Query
import CRL
import helpers
from   helpers         import trace
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.overlay import UpdateSession
import plugins
import plugins.chip

plugins.chip.importConstants( globals() )


# --------------------------------------------------------------------
# Class  :  "power.Side"

class Side ( object ):

    def __init__ ( self, block, side, net, metal ):
        self.block      = block
        self.side       = side
        self.net        = net
        self.metal      = metal
        self.deltaWidth = metal.getExtentionWidth()*2
        self.terminals  = []
  
    def addTerminal ( self, position, width ):
        toMerge = Interval( position-width/2, position+width/2 )
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
            x     = self.block.bb.getXMin()
        elif self.side == East:
            width = 0
            x     = self.block.bb.getXMax()
        elif self.side == South:
            height = 0
            y      = self.block.bb.getYMin()
        elif self.side == North:
            height = 0
            y      = self.block.bb.getYMax()
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
            self.block.path.getTransformation().applyOn( center )
            contact = Contact.create( self.net, self.metal, center.getX(), center.getY(), width, height )
            terminal[ 1 ] = contact


# --------------------------------------------------------------------
# Class  :  "power.Plane"

class Plane ( object ):
  
    Horizontal = 0001
    Vertical   = 0002
  
    def __init__ ( self, block, metal ):
        self.block = block
        self.metal = metal
        self.sides = {}
  
    def addTerminal ( self, net, direction, bb ):
        if not self.sides.has_key(net):
            self.sides[ net ] = { North : Side(self.block,North,net,self.metal)
                                , South : Side(self.block,South,net,self.metal)
                                , East  : Side(self.block,East ,net,self.metal)
                                , West  : Side(self.block,West ,net,self.metal)
                                }
        sides = self.sides[ net ]
        if direction == Plane.Horizontal:
            if bb.getXMin() <= self.block.bb.getXMin():
                sides[West].addTerminal( bb.getCenter().getY(), bb.getHeight() )
            if bb.getXMax() >= self.block.bb.getXMax():
                sides[East].addTerminal( bb.getCenter().getY(), bb.getHeight() )
        if direction == Plane.Vertical:
            if bb.getYMin() <= self.block.bb.getYMin():
                sides[South].addTerminal( bb.getCenter().getX(), bb.getWidth() )
            if bb.getYMax() >= self.block.bb.getYMax():
                sides[North].addTerminal( bb.getCenter().getX(), bb.getWidth() )
  
    def doLayout ( self ):
        for sidesOfNet in self.sides.values():
            for side in sidesOfNet.values():
                side.doLayout()


# --------------------------------------------------------------------
# Class  :  "power.Plane"

class GoCb ( object ):
  
    def __init__ ( self, block ):
        self.block = block
  
    def __call__ ( self, query, go ):
        direction = None
        if isinstance(go,Horizontal): direction = Plane.Horizontal
        if isinstance(go,Vertical):   direction = Plane.Vertical
        if not direction: return
        rootNet = None
        if go.getNet().getType() == long(Net.Type.POWER):  rootNet = self.block.conf.coronaVdd
        if go.getNet().getType() == long(Net.Type.GROUND): rootNet = self.block.conf.coronaVss
        if not rootNet: return
        if self.block.activePlane:
            bb = go.getBoundingBox( self.block.activePlane.metal.getBasicLayer() )
            query.getPath().getTransformation().applyOn( bb )
            self.block.activePlane.addTerminal( rootNet, direction, bb )
        else:
            print WarningMessage( 'BlockPower.GoCb() callback called without an active plane.' )
        return


# --------------------------------------------------------------------
# Class  :  "power.Builder"

class Builder ( object ):

    def __init__ ( self, conf ):
        self.conf        = conf
        self.path        = Path( self.conf.icore )
        self.block       = self.conf.icore.getMasterCell()
        self.bb          = self.block.getAbutmentBox()
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
        query.setCell( self.block )
        query.setArea( self.block.getAbutmentBox() )
        query.setFilter( Query.DoComponents|Query.DoTerminalCells )
        for layerGauge in self.conf.routingGauge.getLayerGauges():
            self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
            query.setBasicLayer( layerGauge.getLayer().getBasicLayer() )
            query.doQuery()
        self.activePlane = None
  
    def connectClock ( self ):
        if not self.conf.useClockTree:
            print WarningMessage( "Clock tree generation has been disabled ('chip.clockTree':False)." )
            return
        if not self.conf.coronaCk:
            raise ErrorMessage( 1, 'Cannot build clock terminal as ck is not known.' )
            return
        blockCk = None
        for plug in self.conf.icore.getPlugs():
            if plug.getNet() == self.conf.coronaCk:
                blockCk = plug.getMasterNet()
        if not blockCk:
            raise ErrorMessage( 1, 'Block "{}" has no net connected to the clock "{}".' \
                                   .format(self.conf.icore.getName(),self.conf.coronaCk.getName()) )
            return
        htPlugs = []
        for plug in self.conf.coronaCk.getPlugs():
            print( plug )
            if plug.getInstance().isTerminalNetlist():
                htPlugs.append( plug )
        if len(htPlugs) != 1:
            message = [ 'Clock "{}" of block "{}" is not organized as a H-Tree ({} plugs).' \
                        .format( self.conf.coronaCk.getName()
                               , self.conf.icore.getName()
                               , len(htPlugs)) ]
            print( self.conf.icore )
            for plug in htPlugs:
                message += [ '\n        - {} {}'.format(plug,plug.getInstance()) ]
            raise ErrorMessage( 1, message )
            return
        with UpdateSession():
            bufferRp = self.conf.rpAccessByOccurrence( Occurrence(htPlugs[0], Path())
                                                     , self.conf.coronaCk
                                                     , 0 )
            blockAb  = self.block.getAbutmentBox()
            self.path.getTransformation().applyOn( blockAb )
            layerGauge = self.conf.routingGauge.getLayerGauge(self.conf.verticalDepth)
            contact    = Contact.create( self.conf.coronaCk
                                       , self.conf.routingGauge.getRoutingLayer(self.conf.verticalDepth)
                                       , bufferRp.getX()
                                       , blockAb.getYMax() 
                                       , layerGauge.getViaWidth()
                                       , layerGauge.getViaWidth()
                                       )
            segment = self.conf.createVertical( bufferRp, contact, bufferRp.getX(), 0 )
            self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
            bb = segment.getBoundingBox( self.activePlane.metal.getBasicLayer() )
            self.path.getTransformation().getInvert().applyOn( bb )
            self.activePlane.addTerminal( self.conf.coronaCk, Plane.Vertical, bb )
  
    def doLayout ( self ):
        with UpdateSession():
            for plane in self.planes.values():
                plane.doLayout()

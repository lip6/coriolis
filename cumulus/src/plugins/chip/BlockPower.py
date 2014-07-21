#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2014, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/BlockPower.py"             |
# +-----------------------------------------------------------------+


from   Hurricane import DbU
from   Hurricane import Point
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Interval
from   Hurricane import Path
from   Hurricane import Occurrence
from   Hurricane import UpdateSession
from   Hurricane import Net
from   Hurricane import Contact
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Query
import CRL
from   helpers   import ErrorMessage
from   helpers   import WarningMessage
import chip.Configuration


class Side ( object ):

  def __init__ ( self, block, side, net, metal ):
    self.block     = block
    self.side      = side
    self.net       = net
    self.metal     = metal
    self.terminals = [ ]
    return

  def addTerminal ( self, position, width ):
    toMerge = Interval( position-width/2, position+width/2 )

    length = len(self.terminals)
    imerge = length
    ichunk = 0

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

    return

  def doLayout ( self ):
    if self.side == chip.West:
      width = 0
      x     = self.block.bb.getXMin()
    elif self.side == chip.East:
      width = 0
      x     = self.block.bb.getXMax()
    elif self.side == chip.South:
      height = 0
      y      = self.block.bb.getYMin()
    elif self.side == chip.North:
      height = 0
      y      = self.block.bb.getYMax()

    for terminal in self.terminals:
      if self.side == chip.West or self.side == chip.East:
        center = Point( x, terminal[0].getCenter() )
        height = terminal[0].getSize()
      elif self.side == chip.North or self.side == chip.South:
        center = Point( terminal[0].getCenter(), y )
        width = terminal[0].getSize()

      self.block.path.getTransformation().applyOn( center )

      contact = Contact.create( self.net, self.metal, center.getX(), center.getY(), width, height )
      terminal[ 1 ] = contact
    return


class Plane ( object ):

  Horizontal = 0001
  Vertical   = 0002

  def __init__ ( self, block, metal ):
    self.block = block
    self.metal = metal
    self.sides = { }
    return

  def addTerminal ( self, net, direction, bb ):
    if not self.sides.has_key(net):
      self.sides[ net ] = [ Side(self.block,chip.North,net,self.metal)
                          , Side(self.block,chip.South,net,self.metal)
                          , Side(self.block,chip.East ,net,self.metal)
                          , Side(self.block,chip.West ,net,self.metal)
                          ]
    sides = self.sides[ net ]

    if direction == Plane.Horizontal:
      if bb.getXMin() <= self.block.bb.getXMin():
        sides[chip.West].addTerminal( bb.getCenter().getY(), bb.getHeight() )
      if bb.getXMax() >= self.block.bb.getXMax():
        sides[chip.East].addTerminal( bb.getCenter().getY(), bb.getHeight() )

    if direction == Plane.Vertical:
      if bb.getYMin() <= self.block.bb.getYMin():
        sides[chip.South].addTerminal( bb.getCenter().getX(), bb.getWidth() )
      if bb.getYMax() >= self.block.bb.getYMax():
        sides[chip.North].addTerminal( bb.getCenter().getX(), bb.getWidth() )
    return

  def doLayout ( self ):
    for sidesOfNet in self.sides.values():
      for side in sidesOfNet:
        side.doLayout()
    return
    


class GoCb ( object ):

  def __init__ ( self, block ):
    self.block = block
    return

  def __call__ ( self, query, go ):
    direction = None
    if isinstance(go,Horizontal): direction = Plane.Horizontal
    if isinstance(go,Vertical):   direction = Plane.Vertical
    if not direction: return

    rootNet = None
    if go.getNet().getType() == Net.Type.POWER:  rootNet = self.block.vddi
    if go.getNet().getType() == Net.Type.GROUND: rootNet = self.block.vssi
    if not rootNet: return

    if self.block.activePlane:
      bb = go.getBoundingBox( self.block.activePlane.metal.getBasicLayer() )
      query.getPath().getTransformation().applyOn( bb )
      self.block.activePlane.addTerminal( rootNet, direction, bb )
    else:
      print WarningMessage( 'BlockPower.GoCb() callback called without an active plane.' )
    return


class Block ( object ):

  def __init__ ( self, path, vddi, vssi ):
    self.path        = path
    self.cell        = self.path.getTailInstance().getMasterCell()
    self.bb          = self.cell.getAbutmentBox()
    self.vddi        = vddi
    self.vssi        = vssi
    self.planes      = { }
    self.activePlane = None

    routingGauge = CRL.AllianceFramework.get().getRoutingGauge()
    for layerGauge in routingGauge.getLayerGauges():
      self.planes[ layerGauge.getLayer().getName() ] = Plane( self, layerGauge.getLayer() )
    
    return


  def queryPower ( self ):
    if not self.vddi or not self.vssi:
      print ErrorMessage( 1, 'Cannot build block power terminals as vddi and/or vss are not known.' )
      return

    goCb = GoCb( self )
    query = Query()
    query.setGoCallback( goCb )
    query.setCell( self.cell )
    query.setArea( self.bb )
    query.setFilter( Query.DoComponents|Query.DoTerminalCells )

    routingGauge = CRL.AllianceFramework.get().getRoutingGauge()
    for layerGauge in routingGauge.getLayerGauges():
      self.activePlane = self.planes[ layerGauge.getLayer().getName() ]
      query.setBasicLayer( layerGauge.getLayer().getBasicLayer() )
      query.doQuery()
    self.activePlane = None
    return

  def doLayout ( self ):
    UpdateSession.open()
    for plane in self.planes.values():
      plane.doLayout()
    UpdateSession.close()
    return

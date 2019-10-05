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
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/PadsCorona.py"             |
# +-----------------------------------------------------------------+


import sys
import re
from   operator   import itemgetter  
import Cfg        
from   Hurricane  import DbU
from   Hurricane  import Point
from   Hurricane  import Transformation
from   Hurricane  import Interval
from   Hurricane  import Box
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import UpdateSession
from   Hurricane  import Layer
from   Hurricane  import BasicLayer
from   Hurricane  import Net
from   Hurricane  import Pin
from   Hurricane  import Contact
from   Hurricane  import Segment
from   Hurricane  import Horizontal
from   Hurricane  import Vertical
from   Hurricane  import RoutingPad
from   Hurricane  import Instance
import CRL        
from   CRL        import RoutingLayerGauge
import helpers
from   helpers    import trace
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
import chip.Configuration


class Corner ( object ):

    def __init__ ( self, corona, cornerType ):
      self.type      = cornerType
      self.corona    = corona
      self.padCorner = None
      return


    @property
    def conf ( self ): return self.corona.conf


    def _getPoints ( self, axis ):
      if self.type == chip.SouthWest:
        xCorner = self.conf.chipSize.getXMin() + axis
        yCorner = self.conf.chipSize.getYMin() + axis
        xBb     = self.conf.chipSize.getXMin() + self.conf.getIoPadHeight()
        yBb     = self.conf.chipSize.getYMin() + self.conf.getIoPadHeight()
      elif self.type == chip.SouthEast:
        xCorner = self.conf.chipSize.getXMax() - axis
        yCorner = self.conf.chipSize.getYMin() + axis
        xBb     = self.conf.chipSize.getXMax() - self.conf.getIoPadHeight()
        yBb     = self.conf.chipSize.getYMin() + self.conf.getIoPadHeight()
      elif self.type == chip.NorthEast:
        xCorner = self.conf.chipSize.getXMax() - axis
        yCorner = self.conf.chipSize.getYMax() - axis
        xBb     = self.conf.chipSize.getXMax() - self.conf.getIoPadHeight()
        yBb     = self.conf.chipSize.getYMax() - self.conf.getIoPadHeight()
      elif self.type == chip.NorthWest:
        xCorner = self.conf.chipSize.getXMin() + axis
        yCorner = self.conf.chipSize.getYMax() - axis
        xBb     = self.conf.chipSize.getXMin() + self.conf.getIoPadHeight()
        yBb     = self.conf.chipSize.getYMax() - self.conf.getIoPadHeight()
      
      return xCorner, yCorner, xBb, yBb


    def _createCorner ( self ):
      for rail in self.corona.padRails:
        net   = rail[0]
        layer = rail[1]
        axis  = rail[2]
        width = rail[3]

        xCorner, yCorner, xBb, yBb = self._getPoints( axis )
      
        Contact   .create( net, layer, xCorner, yCorner, width, width )
        Horizontal.create( net, layer, yCorner, width, xCorner, xBb )
        Vertical  .create( net, layer, xCorner, width, yCorner, yBb )
      return


    def _getTransformation ( self ):
      if self.type == chip.SouthWest:
        name = 'padcorner_sw'
        x    = self.conf.chipSize.getXMin()
        y    = self.conf.chipSize.getYMin()
        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation =  Transformation.Orientation.ID
        else:
          orientation  = Transformation.Orientation.R1
          x           += self.padCorner.getAbutmentBox().getWidth()

      elif self.type == chip.SouthEast:
        name = 'padcorner_se'
        x    = self.conf.chipSize.getXMax()
        y    = self.conf.chipSize.getYMin()
        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation  = Transformation.Orientation.R1
        else:
          orientation  = Transformation.Orientation.R2
          x           += self.padCorner.getAbutmentBox().getWidth()
          y           += self.padCorner.getAbutmentBox().getHeight()

      elif self.type == chip.NorthEast:
        name = 'padcorner_ne'
        x    = self.conf.chipSize.getXMax()
        y    = self.conf.chipSize.getYMax()
        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation  = Transformation.Orientation.R2
        else:
          orientation  = Transformation.Orientation.R3
          x           -= self.padCorner.getAbutmentBox().getwidth()
          y           -= self.padCorner.getAbutmentBox().getHeight()

      elif self.type == chip.NorthWest:
        name = 'padcorner_nw'
        x    = self.conf.chipSize.getXMin()
        y    = self.conf.chipSize.getYMax()
        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation  = Transformation.Orientation.R3
        else:
          orientation  = Transformation.Orientation.ID
          y           -= self.padCorner.getAbutmentBox().getHeight()
      
      return name, Transformation( self.corona.toGrid(x), self.corona.toGrid(y), orientation ) 


    def _instanciateCorner ( self ):
      name, transformation = self._getTransformation()

      corner = Instance.create( self.conf.cell
                              , name, self.corona.padCorner
                              , transformation
                              , Instance.PlacementStatus.FIXED
                              )
      return


    def doLayout ( self ):
      if self.corona.padCorner: self._instanciateCorner()
      else:                     self._createCorner()
      return


class Side ( object ):

    def __init__ ( self, corona, sideType ):
      self.type          = sideType
      self.corona        = corona
      self.pins          = []
      self.u             = self.conf.getIoPadHeight()
      self.spacerCount   = 0
      self.gap           = 0
      self.coreWires     = []

      if   self.type == chip.North:
        self.pads       = self.conf.northPads
        self.sideName   = 'north'
        self.sideLength = self.conf.chipSize.getWidth()

      elif self.type == chip.South:
        self.pads       = self.conf.southPads
        self.sideName   = 'south'
        self.sideLength = self.conf.chipSize.getWidth()

      elif self.type == chip.East:
        self.pads       = self.conf.eastPads
        self.sideName   = 'east'
        self.sideLength = self.conf.chipSize.getHeight()

      elif self.type == chip.West:
        self.pads       = self.conf.westPads
        self.sideName   = 'west'
        self.sideLength = self.conf.chipSize.getHeight()

      else:
        raise ErrorMessage( 1, 'PadsCorona.Side.__init__(): Invalid value for sideType (%d)' % sideType )

      self.spacerNames = 'padspacer_' + self.sideName + '_%d'
      return


    @property
    def conf ( self ): return self.corona.conf


    def toGrid ( self, u ): return self.corona.toGrid( u )


    def getAxis ( self, i ):
      if   self.type == chip.North: return self.conf.chipSize.getYMax() - self.conf.getIoPadHeight() + self.corona.powerRails[i][2]
      elif self.type == chip.South: return self.conf.chipSize.getYMin() + self.conf.getIoPadHeight() - self.corona.powerRails[i][2]
      elif self.type == chip.East:  return self.conf.chipSize.getXMax() - self.conf.getIoPadHeight() + self.corona.powerRails[i][2]
      elif self.type == chip.West:  return self.conf.chipSize.getXMin() + self.conf.getIoPadHeight() - self.corona.powerRails[i][2]
      else:
        raise ErrorMessage( 1, 'PadsCorona.Side.__init__(): Invalid value for sideType (%d)' % sideType )
      return 0


    def hasPad ( self, padInstance ):
      for pad in self.pads:
        if pad[1] == padInstance:
          return True
      return False


    def addCoreWire ( self, coreWire ):
      self.coreWires.append( coreWire )
      return


    def updateGap ( self, gapWidth ):
      self.gap = max( self.gap, gapWidth )
      return


    def _check ( self, checkSize, checkName ):
      sideName = 'unknown'
      chipSize = 0
      if self.type == chip.North or self.type == chip.South:
        chipSize = self.conf.chipSize.getWidth()
        sideName = 'wide'
      elif self.type == chip.East or self.type == chip.West:
        chipSize = self.conf.chipSize.getHeight()
        sideName = 'tall'
  
      if checkSize > chipSize:
        sliceHeight = self.conf.getSliceHeight()
        if checkSize % sliceHeight != 0:
          checkSize += sliceHeight - (checkSize % sliceHeight)

        raise ErrorMessage( 1, [ 'Chip is not %s enought to accomodate the %s,' % (sideName,checkName)
                               , 'needs %s, but only has %s.'
                                 % ( DbU.getValueString(checkSize), DbU.getValueString(chipSize) )
                               ] )
        return False
      return True


    def check ( self ):
      self.validated = True
      if self.type == chip.North:
        self.validated = self._check(     self.conf.coreSize.getWidth()
                                      + 2*self.conf.minCorona
                                      + 2*self.conf.getIoPadHeight()
                                    , 'core' )
        checkName = 'north pads'
      elif self.type == chip.East:
        self.validated = self._check(     self.conf.coreSize.getHeight()
                                      + 2*self.conf.minCorona
                                      + 2*self.conf.getIoPadHeight()
                                    , 'core' )
        checkName = 'east pads'
      elif self.type == chip.South: checkName = 'south pads'
      elif self.type == chip.West:  checkName = 'west pads'

      self.validated = self._check( len(self.pads) *   self.conf.gaugeConf.getIoPadStep  ()
                                                   + 2*self.conf.gaugeConf.getIoPadHeight()
                                  , checkName ) and self.validated
      return self.validated


#   def _createPowerContacts ( self, pad, net ):
#     if self._type == chip.North or self._type == chip.South:
#       hvDepth = self._corona.padVDepth
#     elif self._type == chip.East or self._type == chip.West:
#       hvDepth = self._corona.padHDepth
#
#     trace( 550, ',+', '\t_createPowerContacts() for %s\n' % net.getName() )
#
#     components = None
#     masterCell = pad.getMasterCell()
#     trace( 550, '\tLooking for global net %s\n' % net.getName() )
#     for plug in net.getPlugs():
#       if plug.getInstance() == pad:
#         trace( 550, '\tFound Plug on %s\n' % pad )
#         components = plug.getMasterNet().getExternalComponents()
#     if not components:
#       masterNet = masterCell.getNet( net.getName() )
#       if masterNet:
#         components = masterCell.getNet(net.getName()).getExternalComponents()
#     if not components:
#       raise ErrorMessage( 1, [ 'PadsCorona.Side._createPowerContact():'
#                              , 'Pad model <%s> of instance <%s> neither have global net <%s>' % (pad.getName(),masterCell.getName(),net.getName())
#                              , 'for implicit connection nor is it explicitly connected.'
#                              , 'The power/clock nets *names* in the chip must match those of the pads models.'
#                              ] )
#         
#
#     connecteds = False
#     trace( 550, '\t %s\n' % str(masterCell.getAbutmentBox()) )
#     for component in components:
#       if component.getBoundingBox().getYMin() > masterCell.getAbutmentBox().getYMin(): continue
#       if self._corona.routingGauge.getLayerDepth(component.getLayer()) != hvDepth: continue
#       if not isinstance(component,Vertical): continue
#
#       if self._type == chip.North or self._type == chip.South:
#         width  = component.getWidth()
#         height = 0
#       else:
#         width  = 0
#         height = component.getWidth()
#
#       position = Point( component.getX(), masterCell.getAbutmentBox().getYMin() )
#       pad.getTransformation().applyOn( position )
#
#       connecteds = True
#       self._powerContacts.append( Contact.create( net
#                                                 , component.getLayer()
#                                                 , position.getX()
#                                                 , position.getY()
#                                                 , width
#                                                 , height
#                                                 ) )
#     if not connecteds:
#       print WarningMessage( 'Cannot find a suitable connector for <%s> on pad <%s>'
#                           % (net.getName(),pad.getName()) )
#
#     trace( 550, '-' )
#     return


#   def _createAllPowerContacts ( self ):
#     for pad in self.pads:
#       masterCell = pad.getMasterCell()
#       if     masterCell.getName() != self._corona.pvddickName \
#          and masterCell.getName() != self._corona.pvssickName \
#          and masterCell.getName() != self._corona.pvddeckName \
#          and masterCell.getName() != self._corona.pvsseckName:
#         continue
#      #print 'Power pad:', pad
#       self._createPowerContacts( pad, self._corona.vddi )
#       self._createPowerContacts( pad, self._corona.vssi )
#       if self._corona.useClockTree:
#           self._createPowerContacts( pad, self._corona.cko  )
#     return


    def _fillPadSpacing ( self, gapWidth ):
      if not self.corona.padSpacers:
        self.u += gapWidth
        return
    
      iPadSpacer = 0

      def _getWidth   ( spacer ): return spacer.getAbutmentBox().getWidth()
      def _nextSpacer ( iPadSpacer ):
        while     iPadSpacer < len(self.corona.padSpacers) \
              and gapWidth < _getWidth(self.corona.padSpacers[iPadSpacer]):
          iPadSpacer += 1
        return iPadSpacer

      iPadSpacer = _nextSpacer( iPadSpacer )

      while iPadSpacer < len(self.corona.padSpacers) and gapWidth > 0:
        gapWidth -= _getWidth( self.corona.padSpacers[iPadSpacer] )
        spacer    = Instance.create( self.conf.cell
                                   , self.spacerNames % self.spacerCount
                                   , self.corona.padSpacers[iPadSpacer])
        self.spacerCount += 1
        self._placePad( spacer )

        if gapWidth < _getWidth(self.corona.padSpacers[iPadSpacer]):
          iPadSpacer = _nextSpacer( iPadSpacer )

      if gapWidth != 0:
        raise ErrorMessage( 1, 'PadsCorona.Side._placePads(): Pad fillers cannot close the gap between pads on %s side, %s remains.' \
                               % (self.sideName,DbU.getValueString(gapWidth)) )
         
      self.u += gapWidth
      return


    def _placePad ( self, padInstance ):
      if self.type == chip.North:
        x = self.conf.chipSize.getXMin() + self.u
        y = self.conf.chipSize.getYMax()

        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation = Transformation.Orientation.MY
        else:
          orientation = Transformation.Orientation.ID
          y          -= self.conf.getIoPadHeight()

      elif self.type == chip.South:
        x = self.conf.chipSize.getXMin() + self.u
        y = self.conf.chipSize.getYMin()

        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation = Transformation.Orientation.ID
        else:
          orientation = Transformation.Orientation.MY
          y          += self.conf.getIoPadHeight()

      elif self.type == chip.West:
        x = self.conf.chipSize.getXMin()
        y = self.conf.chipSize.getYMin() + self.u

        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation = Transformation.Orientation.R3
          y          += padInstance.getMasterCell().getAbutmentBox().getWidth()
        else:
          orientation = Transformation.Orientation.R1
          x          += padInstance.getMasterCell().getAbutmentBox().getHeight()

      elif self.type == chip.East:
        x = self.conf.chipSize.getXMax()
        y = self.conf.chipSize.getYMin() + self.u

        if self.corona.padOrient == Transformation.Orientation.ID:
          orientation = Transformation.Orientation.R1
        else:
          orientation = Transformation.Orientation.R3
          x          -= padInstance.getMasterCell().getAbutmentBox().getHeight()
          y          += padInstance.getMasterCell().getAbutmentBox().getWidth()

      padInstance.setTransformation ( Transformation( self.toGrid(x), self.toGrid(y), orientation ) )
      padInstance.setPlacementStatus( Instance.PlacementStatus.FIXED )

      self.u += padInstance.getMasterCell().getAbutmentBox().getWidth()

      p = None
      if self.conf.getIoPadGauge().getName() == 'pxlib':
        p = re.compile( r'p(?P<power>v[sd]{2}[ei])ck_px' )

      if self.conf.getIoPadGauge().getName().startswith('phlib'):
        p = re.compile( r'p(?P<power>v[sd]{2})ck2_sp' )

      if p:
        m = p.match( padInstance.getMasterCell().getName() )

        padName = 'pad'
        if m: padName = m.group( 'power' )
          
        padNet = padInstance.getMasterCell().getNet( padName )
        print 'padName:', padName, 'padNet:', padNet
        if padNet:
          plug    = padInstance.getPlug( padNet )
          chipNet = plug.getNet()

          if not chipNet and padNet.isGlobal():
            chipNet = padInstance.getCell().getNet( padNet.getName() )

          if chipNet:
            rp = RoutingPad.create( chipNet, Occurrence(plug), RoutingPad.BiggestArea )
      return


    def _placePads ( self ):
      padLength = 0
      for pad in self.pads: padLength += pad[1].getMasterCell().getAbutmentBox().getWidth() 
      padSpacing = (self.sideLength - 2*self.conf.getIoPadHeight() - padLength) / (len(self.pads) + 1)

      if self.conf.padsHavePosition:
        for i in range(len(self.pads)):
          self.pads[i][0] = self.toGrid( self.pads[i][0] )
      else:
        position = self.u
        for i in range(len(self.pads)):
          position += padSpacing
          self.pads[i][0] = self.toGrid( position )
          position += self.pads[i][1].getMasterCell().getAbutmentBox().getWidth()

      for pad in self.pads:
        self._fillPadSpacing( pad[0] - self.u )
        self._placePad( pad[1] )

      self._fillPadSpacing( self.sideLength - self.conf.getIoPadHeight() - self.u )

      return


    def _getUMin ( self, box ):
       if self.type == chip.North or self.type == chip.South:
         return box.getXMin()
       return box.getYMin()


    def _getUMax ( self, box ):
       if self.type == chip.North or self.type == chip.South:
         return box.getXMax()
       return box.getYMax()


    def _createSegment ( self, rail, uMin, uMax ):
      net, layer, axis, width = rail
      if self.conf.getIoPadGauge().getName() == 'pxlib':
        if net.isClock():
          uMin -= DbU.fromLambda(5.0)
          uMax += DbU.fromLambda(5.0)
        else:
          uMin -= width/2
          uMax += width/2
      
      if self.type == chip.North or self.type == chip.South:
        if self.type == chip.North:
          axis = self.conf.chipSize.getYMax() - axis
        Horizontal.create( net, layer, axis, width, uMin, uMax )
      else:
        if self.type == chip.East:
          axis = self.conf.chipSize.getXMax() - axis
        Vertical.create( net, layer, axis, width, uMin, uMax )
      return


    def _routePads ( self ):
      if self.type == chip.South or self.type == chip.North:
        startCorner = self.conf.chipSize.getXMin()
        stopCorner  = self.conf.chipSize.getXMax()
      elif self.type == chip.West or self.type == chip.East:
        startCorner = self.conf.chipSize.getYMin()
        stopCorner  = self.conf.chipSize.getYMax()
      else:
        return

      startCorner += self.conf.getIoPadHeight()
      stopCorner  -= self.conf.getIoPadHeight()

      if len(self.pads) == 0:
        return

      padAb = self.conf.getInstanceAb( self.pads[0][1] )
      for irail in range(len(self.corona.padRails)):
        self._createSegment( self.corona.padRails[ irail ]
                           , startCorner
                           , self._getUMin(padAb) )

      padAb = self.conf.getInstanceAb( self.pads[-1][1] )
      for irail in range(len(self.corona.padRails)):
        self._createSegment( self.corona.padRails[ irail ]
                           , self._getUMax(padAb)
                           , stopCorner )

      for i in range(len(self.pads)-1):
        padAb1 = self.conf.getInstanceAb( self.pads[i  ][1] )
        padAb2 = self.conf.getInstanceAb( self.pads[i+1][1] )

        for rail in self.corona.padRails:
          self._createSegment( rail, self._getUMax(padAb1), self._getUMin(padAb2) )

      return


    def doLayout ( self ):
      self._placePads()
      if not self.corona.padSpacers:
        self._routePads()
     #self._createAllPowerContacts()
      return


    def drawCoreWires ( self ):
      trace( 550, ',+', '\tSide.drawCoreWire()\n' )

      if self.type == chip.West or self.type == chip.East:
        trace( 550, 'Sort East/West' )
        self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getY() )
      if self.type == chip.North or self.type == chip.South:
        trace( 550, 'Sort North/South' )
        self.coreWires.sort( key=lambda wire: wire.bbSegment.getCenter().getX() )
          
      for wire in self.coreWires:
        wire.updateInCorona()

      size   = len(self.coreWires)
      offset = 0
      for i in range(size):
        if self.coreWires[i].inCoronaRange: break
        offset += 1
      for i in range(offset):
        self.coreWires[i].setOffset( i+1, CoreWire.AtBegin )

      offset = 0
      for i in range(1,size+1):
        if self.coreWires[-i].inCoronaRange: break
        offset += 1
      for i in range(offset):
        self.coreWires[ -(offset+1) ].setOffset( i+1, CoreWire.AtEnd )

      for wire in self.coreWires:
        if self.type == chip.West or self.type == chip.East:
          trace( 550, '\t| %s %s %d %s inRange:%s\n' % ( wire.chipNet.getName()
                                                       , DbU.getValueString(wire.bbSegment.getCenter().getY())
                                                       , wire.count
                                                       , wire.padSegment.getLayer()
                                                       , wire.inCoronaRange ) )
        
        if self.type == chip.North or self.type == chip.South:
          trace( 550, '\t| %s %s %d %s inRange:%s\n' % ( wire.chipNet.getName()
                                                       , DbU.getValueString(wire.bbSegment.getCenter().getX())
                                                       , wire.count
                                                       , wire.padSegment.getLayer()
                                                       , wire.inCoronaRange ) )
        wire.drawWire()
      trace( 550, '-' )
      return


class CoreWire ( object ):

  # Should be read from the symbolic technology rules.
  viaPitch = DbU.fromLambda( 4.0 )

  NoOffset = 0x0000
  AtBegin  = 0x0001
  AtEnd    = 0x0002

  def __init__ ( self, corona, chipNet, padSegment, bbSegment, side, preferredDir, count ):
    self.corona        = corona
    self.chipNet       = chipNet
    self.padSegment    = padSegment
    self.bbSegment     = bbSegment
    self.offset        = 0
    self.offsetType    = CoreWire.NoOffset
    self.side          = side
    self.preferredDir  = preferredDir
    self.inCoronaRange = True
    self.arraySize     = None
    self.count         = count

    return


  @property
  def conf ( self ): return self.corona.conf


  def updateInCorona ( self ):
    coronaAb = self.conf.getInstanceAb( self.conf.icorona )

    if self.side == chip.South or self.side == chip.North:
      xCoronaPin = self.bbSegment.getCenter().getX()
      if   xCoronaPin <= coronaAb.getXMin(): self.inCoronaRange = False
      elif xCoronaPin >= coronaAb.getXMax(): self.inCoronaRange = False

    if self.side == chip.East or self.side == chip.West:
      yCoronaPin = self.bbSegment.getCenter().getY()
      if   yCoronaPin <= coronaAb.getYMin(): self.inCoronaRange = False
      elif yCoronaPin >= coronaAb.getYMax(): self.inCoronaRange = False

    return


  def setOffset ( self, offset, offsetType ):
    self.offset     = offset
    self.offsetType = offsetType


  def _computeCoreLayers ( self ):
    rg   = self.conf.gaugeConf.routingGauge
    mask = self.padSegment.getLayer().getMask()

    trace( 550, ',+', '\tCoreWire._computeCoreLayers()\n' )
    trace( 550, '\tbbSegment: %s\n' % (self.bbSegment) )

    self.symSegmentLayer = None
    for layerGauge in rg.getLayerGauges():
      if layerGauge.getDepth() > self.conf.gaugeConf.topLayerDepth: break

      if layerGauge.getLayer().getMask() == mask:
        self.symSegmentLayer = layerGauge.getLayer()

        if self.preferredDir:
          self.symContactLayer = self.symSegmentLayer
          if self.side & (chip.West|chip.East):
            self.symContactSize = ( layerGauge.getWireWidth(), self.bbSegment.getHeight() )
          else:
            self.symContactSize = ( self.bbSegment.getWidth(), layerGauge.getWireWidth() )
        else:
          depth = layerGauge.getDepth()
          if layerGauge.getDepth() + 1 > self.conf.gaugeConf.topLayerDepth:
            self.symSegmentLayer = rg.getLayerGauge( depth-1 ).getLayer()
            depth -= 1
          else:
            self.symSegmentLayer = rg.getLayerGauge( depth+1 ).getLayer()

          self.symContactLayer = rg.getContactLayer( depth )
          if self.side & (chip.West|chip.East):
            self.symContactSize  = ( self.bbSegment.getHeight(), self.bbSegment.getHeight() )
          else:
            self.symContactSize  = ( self.bbSegment.getWidth(), self.bbSegment.getWidth() )

          contactMinSize = 2*self.symContactLayer.getEnclosure( self.symSegmentLayer.getBasicLayer()
                                                              , Layer.EnclosureH|Layer.EnclosureV ) \
                         +   self.symContactLayer.getMinimalSize()

          arrayWidth = self.symContactSize[0]
          arrayCount = (arrayWidth - contactMinSize) / CoreWire.viaPitch
          trace( 550, '\tCoreWire.viaPitch: %sl\n' % (DbU.toLambda(CoreWire.viaPitch)) )
          trace( 550, '\tcontactMinSize: %sl, arrayWidth: %sl, arrayCount: %d\n'
                      % (DbU.toLambda(contactMinSize),DbU.toLambda(arrayWidth),arrayCount) )
          if arrayCount < 0: arrayCount = 0
          if arrayCount < 3:
            if self.side & (chip.North|chip.South):
              self.arraySize = ( arrayCount+1, 2 )
            else:
              self.arraySize = ( 2, arrayCount+1 )

            trace( 550, '\tarraySize = (%d,%d)\n' % (self.arraySize[0], self.arraySize[1]) )
        trace( 550, ',-' )
        return

    raise ErrorMessage( 1, 'CoreWire._computeCoreLayers(): Layer of IO pad segment "%s" is not in routing gauge.' \
                           %  self.chipNet.getName() )
      
    trace( 550, ',-' )
    return


  def drawWire ( self ):
    trace( 550, ',+', '\tCoreWire.drawWire(): chip:"%s"\n' %(self.chipNet.getName()) )

    coronaAb = self.conf.getInstanceAb( self.conf.icorona )

    self._computeCoreLayers()

    padLayer = self.padSegment.getLayer()
    if not isinstance(padLayer,BasicLayer):
      padLayer = padLayer.getBasicLayer()
    
    if self.side == chip.West or self.side == chip.East:
      flags = chip.OnHorizontalPitch
      hPitch = self.conf.gaugeConf.getPitch( self.symSegmentLayer )
      vPitch = self.conf.gaugeConf.getPitch( self.padSegment.getLayer() )

      yCore = self.conf.toCoronaPitchInChip( self.bbSegment.getCenter().getY(), self.symSegmentLayer )
      if self.offset:
        if self.offsetType == CoreWire.AtBegin:
          yCore += 2*hPitch*self.offset
        else:
          yCore -= 2*hPitch*self.offset

      if self.side == chip.West:
        accessDirection = Pin.Direction.WEST
        xPadMin         = self.bbSegment.getXMin()
        xContact        = self.corona.coreSymBb.getXMin() - self.offset * 2*vPitch
        xPadMax         = xContact
        xCore           = coronaAb.getXMin()
        if not self.preferredDir:
          xPadMax += self.bbSegment.getHeight()/2
      else:
        accessDirection = Pin.Direction.EAST
        xPadMax         = self.bbSegment.getXMax()
        xContact        = self.corona.coreSymBb.getXMax() + self.offset * 2*vPitch
        xPadMin         = xContact
        xCore           = coronaAb.getXMax()
        if not self.preferredDir:
          xPadMin -= self.bbSegment.getHeight()/2

      hReal = Horizontal.create( self.chipNet
                               , self.padSegment.getLayer()
                               , self.bbSegment.getCenter().getY()
                               , self.bbSegment.getHeight()
                               , xPadMin
                               , xPadMax
                               )
      trace( 550, '\tself.arraySize: %s\n' % str(self.arraySize) )
      if self.arraySize:
        contacts = self.conf.coronaContactArray( self.chipNet
                                               , self.symContactLayer
                                               , xContact
                                               , yCore
                                               , self.arraySize
                                               , flags
                                               )
        vStrapBb = Box()
        for contact in contacts:
          vStrapBb.merge( contact.getBoundingBox() )
      else:
        contact = self.conf.coronaContact( self.chipNet
                                         , self.symContactLayer
                                         , xContact
                                         , yCore
                                         , self.symContactSize[0]
                                         , self.symContactSize[1]
                                         , flags
                                         )
        vStrapBb  = contact.getBoundingBox( padLayer )

      hRealBb = hReal.getBoundingBox( padLayer )
      self.conf.icorona.getTransformation().applyOn( vStrapBb )
      vStrapBb.merge( vStrapBb.getXMin(), hRealBb.getYMin() )
      vStrapBb.merge( vStrapBb.getXMin(), hRealBb.getYMax() )
      if self.padSegment.getLayer().isSymbolic():
        vStrapBb.inflate( 0, -self.padSegment.getLayer().getExtentionCap()
                        , 0, -self.padSegment.getLayer().getExtentionCap() )
      
      Vertical.create( self.chipNet
                     , self.padSegment.getLayer()
                     , vStrapBb.getCenter().getX()
                     , vStrapBb.getWidth()
                     , vStrapBb.getYMin()
                     , vStrapBb.getYMax()
                     )

      if self.arraySize:
        if self.side == chip.West: xContact = min( xContact, vStrapBb.getXMin() )
        else:                      xContact = max( xContact, vStrapBb.getXMax() )

      self.conf.coronaHorizontal( self.chipNet
                                , self.symSegmentLayer
                                , yCore
                                , self.bbSegment.getHeight()
                                , xContact
                                , xCore
                                )
      trace( 550, '\tCORONA PIN: %s %d\n' % (self.chipNet, self.count) )
      pin = self.conf.coronaPin( self.chipNet
                               , self.count
                               , accessDirection
                               , self.symSegmentLayer
                               , xCore
                               , yCore
                               , DbU.fromLambda( 1.0 )
                               , self.bbSegment.getHeight()
                               )
    else:
      flags  = chip.OnVerticalPitch
      hPitch = self.conf.gaugeConf.getPitch( self.padSegment.getLayer() )
      vPitch = self.conf.gaugeConf.getPitch( self.symSegmentLayer )

      trace( 550, '\t%s translated of %s\n' % (self.symSegmentLayer, DbU.getValueString( 2*vPitch*self.offset )) )

      xCore = self.conf.toCoronaPitchInChip( self.bbSegment.getCenter().getX(), self.symSegmentLayer )
      if self.offset:
        if self.offsetType == CoreWire.AtBegin:
          xCore += 2*vPitch*self.offset
        else:
          xCore -= 2*vPitch*self.offset

      if self.side == chip.South:
        accessDirection = Pin.Direction.SOUTH
        yPadMin         = self.bbSegment.getYMin()
        yPadMax         = self.corona.coreSymBb.getYMin() - self.offset * 2*hPitch
        yContact        = yPadMax
        yCore           = coronaAb.getYMin()
        if not self.preferredDir:
          yPadMax += self.bbSegment.getWidth()/2
      else:
        accessDirection = Pin.Direction.NORTH
        yPadMax         = self.bbSegment.getYMax()
        yPadMin         = self.corona.coreSymBb.getYMax() + self.offset * 2*hPitch
        yContact        = yPadMin
        yCore           = coronaAb.getYMax()
        if not self.preferredDir:
          yPadMin -= self.bbSegment.getWidth()/2

      vReal = Vertical.create( self.chipNet
                             , self.padSegment.getLayer()
                             , self.bbSegment.getCenter().getX()
                             , self.bbSegment.getWidth()
                             , yPadMin
                             , yPadMax
                             )
      trace( 550, '\tself.arraySize: %s\n' % str(self.arraySize) )
      if self.arraySize:
        contacts = self.conf.coronaContactArray( self.chipNet
                                               , self.symContactLayer
                                               , xCore
                                               , yContact
                                               , self.arraySize
                                               , flags
                                               )
        hStrapBb = Box()
        for contact in contacts:
          hStrapBb.merge( contact.getBoundingBox() )
      else:
        contact   = self.conf.coronaContact( self.chipNet
                                           , self.symContactLayer
                                           , self.bbSegment.getCenter().getX()
                                           , yContact
                                           , self.symContactSize[0]
                                           , self.symContactSize[1]
                                           , flags
                                           )
        hStrapBb  = contact.getBoundingBox( padLayer )

      vRealBb = vReal.getBoundingBox()
      self.conf.icorona.getTransformation().applyOn( hStrapBb )
      hStrapBb.merge( vRealBb.getXMin(), hStrapBb.getYMin() )
      hStrapBb.merge( vRealBb.getXMax(), hStrapBb.getYMin() )
      if self.padSegment.getLayer().isSymbolic():
        hStrapBb.inflate( -self.padSegment.getLayer().getExtentionCap(), 0
                        , -self.padSegment.getLayer().getExtentionCap(), 0 )
      
      Horizontal.create( self.chipNet
                       , self.padSegment.getLayer()
                       , hStrapBb.getCenter().getY()
                       , hStrapBb.getHeight()
                       , hStrapBb.getXMin()
                       , hStrapBb.getXMax()
                       )
      
      if self.arraySize:
        if self.side == chip.South: yContact = min( yContact, hStrapBb.getYMin() )
        else:                       yContact = max( yContact, hStrapBb.getYMax() )
      
      trace( 550, '\txCore:  %sl %s\n' % (DbU.toLambda(xCore), DbU.getValueString(xCore)) )
      self.conf.coronaVertical( self.chipNet
                              , self.symSegmentLayer
                              , xCore
                              , self.bbSegment.getWidth()
                              , yContact
                              , yCore
                              )
      pin = self.conf.coronaPin( self.chipNet
                               , self.count
                               , accessDirection
                               , self.symSegmentLayer
                               , xCore
                               , yCore
                               , self.bbSegment.getWidth()
                               , DbU.fromLambda( 1.0 )
                               )

    if self.side & chip.North: self.corona.northSide.pins.append( pin )
    if self.side & chip.South: self.corona.southSide.pins.append( pin )
    if self.side & chip.East : self.corona.eastSide .pins.append( pin )
    if self.side & chip.West : self.corona.westSide .pins.append( pin )

    trace( 550, '-' )
    return


class Corona ( object ):

  def __init__ ( self, conf ):
    def _cmpPad ( pad1, pad2):
      width1 = pad1.getAbutmentBox().getWidth()
      width2 = pad2.getAbutmentBox().getWidth()
      if width1 == width2: return 0
      if width1 >  width2: return  -1
      return 1

    self.conf       = conf  
    self.validated  = False
    self.northSide  = Side( self, chip.North )
    self.southSide  = Side( self, chip.South )
    self.eastSide   = Side( self, chip.East  )
    self.westSide   = Side( self, chip.West  )
    self.corners    = { chip.SouthWest : Corner( self, chip.SouthWest )
                      , chip.SouthEast : Corner( self, chip.SouthEast )
                      , chip.NorthWest : Corner( self, chip.NorthWest )
                      , chip.NorthEast : Corner( self, chip.NorthEast )
                      }
    self.padLib     = None
    self.padOrient  = Transformation.Orientation.ID
    self.padSpacers = []
    self.padCorner  = []
    self.padRails   = []  # [ , [net, layer, axis, width] ]

    if Cfg.hasParameter('chip.padCoreSide'):
      if Cfg.getParamString('chip.padCoreSide').asString().lower() == 'south':
        self.padOrient = Transformation.Orientation.MY

    self._allPadsAnalysis()
    if Cfg.hasParameter('chip.padSpacers'):
      for spacerName in Cfg.getParamString('chip.padSpacers').asString().split(','):
        spacerCell = self.padLib.getCell( spacerName )
        if spacerCell: self.padSpacers.append( spacerCell )
        else:
          raise ErrorMessage( 1, 'Corona.__init__(): Missing spacer cell "%s"' % spacerName )
        self.padSpacers.sort( _cmpPad )

    if Cfg.hasParameter('chip.padCorner'):
      self.padCorner = self.padLib.getCell( Cfg.getParamString('chip.padCorner').asString() )
   
    return


  def toGrid ( self, u ): return u - (u % self.conf.getIoPadPitch())


  def validate ( self ):
    self._allPadsAnalysis()
    self.validated = self.northSide.check()
    self.validated = self.southSide.check() and self.validated
    self.validated = self.eastSide.check()  and self.validated
    self.validated = self.westSide.check()  and self.validated
    return self.validated


  def hasNorthPad ( self, padInstance ): return self.northSide.hasPad(padInstance)
  def hasSouthPad ( self, padInstance ): return self.southSide.hasPad(padInstance)
  def hasEastPad  ( self, padInstance ): return self.eastSide.hasPad(padInstance)
  def hasWestPad  ( self, padInstance ): return self.westSide.hasPad(padInstance)


  def hasPad ( self, padInstance ):
    if self.hasNorthPad(padInstance): return True
    if self.hasSouthPad(padInstance): return True
    if self.hasEastPad (padInstance): return True
    if self.hasWestPad (padInstance): return True
    return False


  def _allPadsAnalysis ( self ):
    for pad in self.conf.southPads:
      self._padAnalysis( pad[1] )
      if self.padRails: return
      
    for pad in self.conf.northPads:
      self._padAnalysis( pad[1] )
      if self.padRails: return
      
    for pad in self.conf.eastPads:
      self._padAnalysis( pad[1] )
      if self.padRails: return
      
    for pad in self.conf.westPads:
      self._padAnalysis( pad[1] )
      if self.padRails: return
      
    return


  def _padAnalysis ( self, padInstance ):
    if self.padRails: return
    
    padCell = padInstance.getMasterCell()
    ab      = padCell.getAbutmentBox()

    if not self.padLib: self.padLib = padCell.getLibrary()

    for plug in padInstance.getPlugs():
      for component in plug.getMasterNet().getComponents():
        if isinstance(component,Horizontal):
          hspan = Interval( component.getBoundingBox().getXMin()
                          , component.getBoundingBox().getXMax() )

         # Specific hack for Alliance pxlib pad library.
          if self.conf.getIoPadGauge().getName() == 'pxlib':
            if plug.getMasterNet().isClock():
              hspan.inflate( DbU.fromLambda(5.0) )
            else:
              hspan.inflate( component.getWidth() / 2 )

          if hspan.contains( ab.getXMin() ) or hspan.contains( ab.getXMax() ):
            duplicate = False

            if self.padOrient == Transformation.Orientation.ID:
              axis = component.getY()
            else:
              axis = self.conf.getIoPadHeight() - component.getY()
            
            for rail in self.padRails:
              if     rail[0] == plug.getNet()        \
                 and rail[1] == component.getLayer() \
                 and rail[2] == axis:
                duplicate = True
                break

            if not duplicate:
              net = plug.getNet()
              if not net:
                if plug.getMasterNet().isGlobal():
                  net = self.conf.cell.getNet( plug.getMasterNet().getName() )
                  if not net:
                    raise ErrorMessage( 1, 'PadsCorona._padAnalysis(): Ring net "%s" is not connected and there is no global net (in pad \"%s").' \
                                           %  plug.getMasterNet().getName(), padCell.getName() )
                else:
                  raise ErrorMessage( 1, 'PadsCorona._padAnalysis(): Ring net "%s" is neither connected nor global (in pad \"%s").' \
                                         %  plug.getMasterNet().getName(), padCell.getName() )
                
              if net:
                self.padRails.append( ( net
                                      , component.getLayer()
                                      , axis
                                      , component.getWidth() ) )
    return


  def _createCoreWire ( self, chipIntNet, padNet, padInstance, count ):
    side = None
    if   self.hasSouthPad(padInstance): side = self.southSide
    elif self.hasNorthPad(padInstance): side = self.northSide
    elif self.hasEastPad (padInstance): side = self.eastSide
    elif self.hasWestPad (padInstance): side = self.westSide
    if not side: return count

    innerBb   = self.conf.cell.getAbutmentBox().inflate( -self.conf.getIoPadHeight() )
    rg        = self.conf.gaugeConf.routingGauge
    hsegments = { }
    vsegments = { }

    for component in padNet.getExternalComponents():
      if isinstance(component,Segment) or isinstance(component,Contact):
        bb = component.getBoundingBox()
        padInstance.getTransformation().applyOn( bb )
        if bb.intersect(innerBb):
          trace( 550, '\tTerm comp:%s bb:%s\n' % (component,bb) )

          lg    = rg.getLayerGauge( component.getLayer() )
          depth = lg.getDepth()
          if depth > self.conf.gaugeConf.topLayerDepth: continue

          if lg.getDirection() == RoutingLayerGauge.Vertical:
            if not vsegments.has_key(depth): vsegments[ depth ] = []
            vsegments[ depth ].append( (component,bb) )
          else:
            if not hsegments.has_key(depth): hsegments[ depth ] = []
            hsegments[ depth ].append( (component,bb) )

   #if len(vsegments) + len(hsegments) == 0:
   #  raise ErrorMessage( 1, [ 'Corona._createCorewire(): No connector of "%s" in I/O pad "%s" is in routing gauge range (top layer "%s")' \
   #                           % ( padNet.getName()
   #                             , padInstance.getMasterCell().getName()
   #                             , rg.getLayerGauge(self.conf.gaugeConf.topLayerDepth).getLayer().getName() )
   #                         , 'while connecting corona net "%s"' % chipIntNet.getName()
   #                         ] )

    gapWidth       = 0
    segments       = None
    inPreferredDir = False
    if side.type == chip.North or side.type == chip.South:
      if len(vsegments):
        inPreferredDir = True
        segments       = vsegments[ min(vsegments.keys()) ]
      elif len(hsegments):
        segments       = hsegments[ min(hsegments.keys()) ]
    else:
      if len(hsegments):
        inPreferredDir = True
        segments       = hsegments[ min(hsegments.keys()) ]
      elif len(vsegments):
        segments       = vsegments[ min(vsegments.keys()) ]

    coreWires = []
    if segments:
      for segment, bb in segments:
        if not inPreferredDir:
          if side.type == chip.North or side.type == chip.South:
            trace( 550, '\tNorth/South "%s" but RDir H, gapWidth: %s\n'
                   % (chipIntNet.getName(),DbU.getValueString(bb.getWidth()) ) )
            side.updateGap( bb.getWidth() )
          else:
            trace( 550, '\tEast/West "%s" but RDir V, gapWidth: %s\n'
                   % (chipIntNet.getName(),DbU.getValueString(bb.getHeight())) )
            side.updateGap( bb.getHeight() )

        side.addCoreWire( CoreWire( self, chipIntNet, segment, bb, side.type, inPreferredDir, count ) )
        count += 1
    else:
      if not chipIntNet.isGlobal():
        raise ErrorMessage( 1, [ 'PadsCorona._createCoreWire(): In I/O pad "%s" (%s),'
                                 % ( padInstance.getMasterCell().getName()
                                   , padInstance.getName() )
                               , 'connector "%s" has no suitable segment for net "%s".' 
                                 % ( padNet.getName()
                                   , chipIntNet.getName() )
                               ] )
          
    return count


  def _placeInnerCorona ( self ):
    rg = self.conf.gaugeConf.routingGauge

    coronaSouthGap = 0
    for layerGauge in rg.getLayerGauges():
      self.southSide.gap = max( self.southSide.gap, layerGauge.getPitch() * 6 )
    self.northSide.gap = self.southSide.gap
    self.eastSide.gap  = self.southSide.gap
    self.westSide.gap  = self.southSide.gap

    for coronaPlug in self.conf.icorona.getPlugs():
      chipIntNet = coronaPlug.getNet()
      if not chipIntNet:
        raise ErrorMessage( 1, 'PadsCorona._placeInnerCorona(): Corona net "%s" is not connected to a pad.' \
                               %  coronaPlug.getMasterNet().getName() )
        continue

      padConnected  = 0
      doneInstances = []
      for chipPlug in chipIntNet.getPlugs():
        doneInstances.append( chipPlug.getInstance() )
        padNet       = chipPlug.getMasterNet()
        padConnected = self._createCoreWire( chipIntNet, padNet, doneInstances[-1], padConnected )

      if chipIntNet.isGlobal():
        for instance in self.conf.cell.getInstances():
          if instance in doneInstances: continue
          doneInstances.append( instance )
          
          padNet = instance.getMasterCell().getNet( chipIntNet.getName() )
          if not padNet: continue

          padConnected = self._createCoreWire( chipIntNet, padNet, doneInstances[-1], padConnected )

      if padConnected == 0:
        raise ErrorMessage( 1, 'PadsCorona._placeInnerCorona(): Chip net "%s" is not connected to a pad.' \
                               %  chipIntNet.getName() )

    self.conf.setupCorona( self.westSide.gap, self.southSide.gap, self.eastSide.gap, self.northSide.gap )

    self.coreSymBb = self.conf.getInstanceAb( self.conf.icorona )
    self.coreSymBb.inflate( self.conf.toSymbolic( self.westSide.gap /2, chip.Superior )
                          , self.conf.toSymbolic( self.southSide.gap/2, chip.Superior )
                          , self.conf.toSymbolic( self.eastSide.gap /2, chip.Inferior )
                          , self.conf.toSymbolic( self.northSide.gap/2, chip.Inferior ) )

    self.southSide.drawCoreWires()
    self.northSide.drawCoreWires()
    self.eastSide .drawCoreWires()
    self.westSide .drawCoreWires()

    if     len(self.southSide.coreWires) \
       and len(self.westSide .coreWires) \
       and not self.southSide.coreWires[0].inCoronaRange \
       and not self.westSide .coreWires[0].inCoronaRange:
        print ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on south-east corner are outside corona range.'
                               , 'This will generate a short-circuit between S:"%s" and W:"%s".' % \
                                 ( self.southSide.coreWires[0].chipNet.getName()
                                 , self.westSide .coreWires[0].chipNet.getName()) ] )

    if     len(self.southSide.coreWires) \
       and len(self.eastSide .coreWires) \
       and not self.southSide.coreWires[-1].inCoronaRange \
       and not self.eastSide .coreWires[ 0].inCoronaRange:
        print ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on south-west corner are outside corona range.'
                               , 'This will generate a short-circuit between S:"%s" and E:"%s.' % \
                                 ( self.southSide.coreWires[-1].chipNet.getName()
                                 , self.eastSide .coreWires[ 0].chipNet.getName()) ] )

    if     len(self.northSide.coreWires) \
       and len(self.westSide .coreWires) \
       and not self.northSide.coreWires[ 0].inCoronaRange \
       and not self.westSide .coreWires[-1].inCoronaRange:
        print ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on north-east corner are outside corona range.'
                               , 'This will generate a short-circuit between N:"%s" and W:"%s".' % \
                                 ( self.northSide.coreWires[ 0].chipNet.getName()
                                 , self.westSide .coreWires[-1].chipNet.getName()) ] )

    if     len(self.northSide.coreWires) \
       and len(self.eastSide .coreWires) \
       and not self.northSide.coreWires[-1].inCoronaRange \
       and not self.eastSide .coreWires[-1].inCoronaRange:
        print ErrorMessage( 1, [ 'Corona._placeInnerCorona(): Both pads on north-west corner are outside corona range.'
                               , 'This will generate a short-circuit between N:"%s" and E:"%s.' % \
                                 ( self.northSide.coreWires[-1].chipNet.getName()
                                 , self.eastSide .coreWires[-1].chipNet.getName()) ] )

    return


# def _locatePadRails ( self ):
#   if not self.clockPad:
#     print ErrorMessage( 1, 'There must be at least one pad of model "%s" to guess the pad rails.' \
#                            % self.pckName )
#     return False
#
#   for plug in self.clockPad.getPlugs():
#     masterNet = plug.getMasterNet()
#     netType   = masterNet.getType()
#     net       = plug.getNet()
#
#     if     netType != Net.Type.POWER \
#        and netType != Net.Type.GROUND \
#        and netType != Net.Type.CLOCK:
#       continue
#
#     if not net:
#       net = self.cell.getNet( masterNet.getName() )
#       if not net:
#         print ErrorMessage( 1, 'Missing global net <%s> at chip level.' % masterNet.getName() )
#         continue
#
#     for component in masterNet.getExternalComponents():
#       if not isinstance(component,Horizontal): continue
#       xs = component.getSourcePosition().getX()
#       xt = component.getTargetPosition().getX()
#       if xs < xt: length = xt - xs
#       else:       length = xs - xt
#       if length < self.padWidth*.6: continue
#
#       self._powerRails.append( [ net, component.getLayer(), component.getY(), component.getWidth() ] )
#
#   self._powerRails.sort( key=itemgetter(2) )
#
#  #for rail in self._powerRails:
#  #  print 'Pad rail %s @%d width:%d layer:%s' % ( str(rail[0].getName())
#  #                                              , DbU.toLambda(rail[2])
#  #                                              , DbU.toLambda(rail[3])
#  #                                              , str(rail[1].getName())
#  #                                              )
#   return
#
# def _guessPadHvLayers ( self ):
#   if not self.powerPad:
#     print ErrorMessage( 1, 'There must be at least one pad of model "%s" to guess the pad power terminals.' \
#                            % self.pvddick )
#     return False
#
#   availableDepths = set()
#   masterCell      = self.powerPad.getMasterCell()
#   for component in masterCell.getNet('vddi').getExternalComponents():
#     if component.getBoundingBox().getYMin() <= masterCell.getAbutmentBox().getYMin():
#       availableDepths.add( self.routingGauge.getLayerDepth(component.getLayer()) )
#
#  #print 'available depth:', availableDepths
#
#   self._horizontalPadDepth = 0
#   self._verticalPadDepth   = 0
#   for depth in range(0,self.topLayerDepth+1):
#     if not depth in availableDepths: continue
#
#     if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
#       self._horizontalPadDepth = depth
#     if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
#       self._verticalPadDepth = depth
#
#  #print 'h:', self._horizontalPadDepth
#  #print 'v:', self._verticalPadDepth
#   return


  def doLayout ( self ):
    if not self.validated: return
    UpdateSession.open()
    self.conf.cell.setAbutmentBox( self.conf.chipSize )

    for corner in self.corners.values():
      corner.doLayout()

    self.northSide.doLayout()
    self.southSide.doLayout()
    self.eastSide.doLayout()
    self.westSide.doLayout()
    self._placeInnerCorona()
    self.conf.chip.setRouted( True )

    UpdateSession.close()
    return

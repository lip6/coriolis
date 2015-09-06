#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2015, All Rights Reserved
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
from   operator  import itemgetter  
import Cfg
from   Hurricane import DbU
from   Hurricane import Point
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Occurrence
from   Hurricane import UpdateSession
from   Hurricane import Net
from   Hurricane import Contact
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Instance
import CRL
from   CRL import RoutingLayerGauge
from   helpers   import trace
from   helpers   import ErrorMessage
from   helpers   import WarningMessage
import chip.Configuration


class Side ( object ):

    def __init__ ( self, corona, sideType ):
      self._type          = sideType
      self._corona        = corona
      self._powerContacts = []

      if   self._type == chip.North: self._pads = self._corona.northPads
      elif self._type == chip.South: self._pads = self._corona.southPads
      elif self._type == chip.East:  self._pads = self._corona.eastPads
      elif self._type == chip.West:  self._pads = self._corona.westPads
      else:
        raise ErrorMessage( 1, 'PadsCorona.Side.__init__(): Invalid value for sideType (%d)' % sideType )
      return


    def _toGrid ( self, v ): return v - (v % self._corona.getSliceStep())


    def getAxis ( self, i ):
      if   self._type == chip.North: return self._corona.chipSize.getYMax() - self._corona.padHeight + self._corona._powerRails[i][2]
      elif self._type == chip.South: return self._corona.chipSize.getYMin() + self._corona.padHeight - self._corona._powerRails[i][2]
      elif self._type == chip.East:  return self._corona.chipSize.getXMax() - self._corona.padHeight + self._corona._powerRails[i][2]
      elif self._type == chip.West:  return self._corona.chipSize.getXMin() + self._corona.padHeight - self._corona._powerRails[i][2]
      else:
        raise ErrorMessage( 1, 'PadsCorona.Side.__init__(): Invalid value for sideType (%d)' % sideType )
      return 0


    def _check ( self, checkSize, checkName ):
      sideName = 'unknown'
      chipSize = 0
      if self._type == chip.North or self._type == chip.South:
        chipSize = self._corona.chipSize.getWidth()
        sideName = 'wide'
      elif self._type == chip.East or self._type == chip.West:
        chipSize = self._corona.chipSize.getHeight()
        sideName = 'tall'
  
      if checkSize > chipSize:
        sliceHeight = self._corona.getSliceHeight()
        if checkSize % sliceHeight != 0:
          checkSize += sliceHeight - (checkSize % sliceHeight)

        print ErrorMessage( 1, [ 'Chip is not %s enought to accomodate the %s,' % (sideName,checkName)
                               , 'needs %dl, but only has %dl.'
                                 % ( DbU.toLambda(checkSize), DbU.toLambda(chipSize) )
                               ] )
        return False
      return True


    def check ( self ):
      self.validated = True
      if self._type == chip.North:
        self.validated = self._check(     self._corona.coreSize.getWidth()
                                      + 2*self._corona.minCorona
                                      + 2*self._corona.padHeight
                                    , 'core' )
        checkName = 'north pads'
      elif self._type == chip.East:
        self.validated = self._check(     self._corona.coreSize.getHeight()
                                      + 2*self._corona.minCorona
                                      + 2*self._corona.padHeight
                                    , 'core' )
        checkName = 'east pads'
      elif self._type == chip.South: checkName = 'south pads'
      elif self._type == chip.West:  checkName = 'west pads'

      self.validated = self._check( len(self._pads) *   self._corona.padWidth
                                                    + 2*self._corona.padHeight
                                  , checkName ) and self.validated
      return self.validated


    def _createPowerContacts ( self, pad, net ):
      if self._type == chip.North or self._type == chip.South:
        hvDepth = self._corona.padVDepth
      elif self._type == chip.East or self._type == chip.West:
        hvDepth = self._corona.padHDepth

      trace( 550, ',+', '\t_createPowerContacts() for %s\n' % net.getName() )

      components = None
      masterCell = pad.getMasterCell()
      trace( 550, '\tLooking for global net %s\n' % net.getName() )
      for plug in net.getPlugs():
        if plug.getInstance() == pad:
          trace( 550, '\tFound Plug on %s\n' % pad )
          components = plug.getMasterNet().getExternalComponents()
      if not components:
        masterNet = masterCell.getNet( net.getName() )
        if masterNet:
          components = masterCell.getNet(net.getName()).getExternalComponents()
      if not components:
        raise ErrorMessage( 1, [ 'PadsCorona.Side._createPowerContact():'
                               , 'Pad model <%s> of instance <%s> neither have global net <%s>' % (pad.getName(),masterCell.getName(),net.getName())
                               , 'for implicit connection nor is it explicitly connected.'
                               , 'The power/clock nets *names* in the chip must match those of the pads models.'
                               ] )
          

      connecteds = False
      trace( 550, '\t %s\n' % str(masterCell.getAbutmentBox()) )
      for component in components:
        if component.getBoundingBox().getYMin() > masterCell.getAbutmentBox().getYMin(): continue
        if self._corona.routingGauge.getLayerDepth(component.getLayer()) != hvDepth: continue
        if not isinstance(component,Vertical): continue

        if self._type == chip.North or self._type == chip.South:
          width  = component.getWidth()
          height = 0
        else:
          width  = 0
          height = component.getWidth()

        position = Point( component.getX(), masterCell.getAbutmentBox().getYMin() )
        pad.getTransformation().applyOn( position )

        connecteds = True
        self._powerContacts.append( Contact.create( net
                                                  , component.getLayer()
                                                  , position.getX()
                                                  , position.getY()
                                                  , width
                                                  , height
                                                  ) )
      if not connecteds:
        print WarningMessage( 'Cannot find a suitable connector for <%s> on pad <%s>'
                            % (net.getName(),pad.getName()) )

      trace( 550, '-' )
      return


    def _createAllPowerContacts ( self ):
      for pad in self._pads:
        masterCell = pad.getMasterCell()
        if     masterCell.getName() != self._corona.pvddickName \
           and masterCell.getName() != self._corona.pvssickName \
           and masterCell.getName() != self._corona.pvddeckName \
           and masterCell.getName() != self._corona.pvsseckName:
          continue
       #print 'Power pad:', pad
        self._createPowerContacts( pad, self._corona.vddi )
        self._createPowerContacts( pad, self._corona.vssi )
        if self._corona.useClockTree:
            self._createPowerContacts( pad, self._corona.cko  )
      return


    def _placePads ( self ):
      if self._type == chip.North:
        hAdvance    = True
        orientation = Transformation.Orientation.ID
        sideLength  = self._corona.chipSize.getWidth()
        x           = self._corona.padHeight
        y           = self._corona.chipSize.getHeight() - self._corona.padHeight
      elif self._type == chip.South:
        hAdvance    = True
        orientation = Transformation.Orientation.MY
        sideLength  = self._corona.chipSize.getWidth()
        x           = self._corona.padHeight
        y           = self._corona.padHeight
      elif self._type == chip.East:
        hAdvance    = False
        orientation = Transformation.Orientation.R3
        sideLength  = self._corona.chipSize.getHeight()
        x           = self._corona.chipSize.getWidth() - self._corona.padHeight
        y           = self._corona.padHeight + self._corona.padWidth
      elif self._type == chip.West:
        hAdvance    = False
        orientation = Transformation.Orientation.R1
        sideLength  = self._corona.chipSize.getHeight()
        x           = self._corona.padHeight
        y           = self._corona.padHeight

      step = (sideLength - 2*self._corona.padHeight) / len(self._pads)
      for pad in self._pads:
        pad.setTransformation ( Transformation( self._toGrid(x), self._toGrid(y), orientation ) )
        pad.setPlacementStatus( Instance.PlacementStatus.FIXED )
        if hAdvance: x += step
        else:        y += step
      return


    def _routePads ( self ):
      for i in range(len(self._corona._powerRails)):
        if self._type == chip.South:
          Horizontal.create( self._corona._corners[chip.SouthWest][i]
                           , self._corona._corners[chip.SouthEast][i]
                           , self._corona._powerRails[i][1]
                           , self.getAxis( i )
                           , self._corona._powerRails[i][3]
                           )
        elif self._type == chip.North:
          Horizontal.create( self._corona._corners[chip.NorthWest][i]
                           , self._corona._corners[chip.NorthEast][i]
                           , self._corona._powerRails[i][1]
                           , self.getAxis( i )
                           , self._corona._powerRails[i][3]
                           )
        elif self._type == chip.East:
          Vertical.create( self._corona._corners[chip.SouthEast][i]
                         , self._corona._corners[chip.NorthEast][i]
                         , self._corona._powerRails[i][1]
                         , self.getAxis( i )
                         , self._corona._powerRails[i][3]
                         )
        elif self._type == chip.West:
          Vertical.create( self._corona._corners[chip.SouthWest][i]
                         , self._corona._corners[chip.NorthWest][i]
                         , self._corona._powerRails[i][1]
                         , self.getAxis( i )
                         , self._corona._powerRails[i][3]
                         )
      return

    def doLayout ( self ):
      self._placePads()
      self._routePads()
      self._createAllPowerContacts()
      return


class Corona ( chip.Configuration.ChipConfWrapper ):

  def __init__ ( self, conf ):
    chip.Configuration.ChipConfWrapper.__init__( self, conf.gaugeConf, conf.chipConf )
    self.validated   = False
    self._northSide  = Side( self, chip.North )
    self._southSide  = Side( self, chip.South )
    self._eastSide   = Side( self, chip.East  )
    self._westSide   = Side( self, chip.West  )
    self._corners    = { chip.SouthWest : []
                       , chip.SouthEast : []
                       , chip.NorthWest : []
                       , chip.NorthEast : []
                       }
    self._powerRails = []  # [ , [net, layer, axis, width] ]

    self._locatePadRails()
    self._guessPadHvLayers()
    return

  @property
  def southSide ( self ): return self._southSide
  @property
  def northSide ( self ): return self._northSide
  @property
  def eastSide  ( self ): return self._eastSide
  @property
  def westSide  ( self ): return self._westSide
  @property
  def padHDepth ( self ): return self._horizontalPadDepth
  @property
  def padVDepth ( self ): return self._verticalPadDepth

  def getSwCorner  ( self, i ): return self._corners[chip.SouthWest][i]
  def getSeCorner  ( self, i ): return self._corners[chip.SouthEast][i]
  def getNwCorner  ( self, i ): return self._corners[chip.NorthWest][i]
  def getNeCorner  ( self, i ): return self._corners[chip.NorthEast][i]
  def getRailNet   ( self, i ): return self._powerRails[i][0]
  def getRailLayer ( self, i ): return self._powerRails[i][1]
  def getRailAxis  ( self, i ): return self._powerRails[i][2]
  def getRailWidth ( self, i ): return self._powerRails[i][3]


  def validate ( self ):
    self.validated = self._northSide.check()
    self.validated = self._southSide.check() and self.validated
    self.validated = self._eastSide.check()  and self.validated
    self.validated = self._westSide.check()  and self.validated
    return self.validated


  def _locatePadRails ( self ):
    if not self.clockPad:
      print ErrorMessage( 1, 'There must be at least one pad of model "%s" to guess the pad rails.' \
                             % self.pckName )
      return False

    for plug in self.clockPad.getPlugs():
      masterNet = plug.getMasterNet()
      netType   = masterNet.getType()
      net       = plug.getNet()

      if     netType != Net.Type.POWER \
         and netType != Net.Type.GROUND \
         and netType != Net.Type.CLOCK:
        continue

      if not net:
        net = self.cell.getNet( masterNet.getName() )
        if not net:
          print ErrorMessage( 1, 'Missing global net <%s> at chip level.' % masterNet.getName() )
          continue

      for component in masterNet.getExternalComponents():
        if not isinstance(component,Horizontal): continue
        xs = component.getSourcePosition().getX()
        xt = component.getTargetPosition().getX()
        if xs < xt: length = xt - xs
        else:       length = xs - xt
        if length < self.padWidth*.6: continue

        self._powerRails.append( [ net, component.getLayer(), component.getY(), component.getWidth() ] )

    self._powerRails.sort( key=itemgetter(2) )

   #for rail in self._powerRails:
   #  print 'Pad rail %s @%d width:%d layer:%s' % ( str(rail[0].getName())
   #                                              , DbU.toLambda(rail[2])
   #                                              , DbU.toLambda(rail[3])
   #                                              , str(rail[1].getName())
   #                                              )
    return

  def _guessPadHvLayers ( self ):
    if not self.powerPad:
      print ErrorMessage( 1, 'There must be at least one pad of model "%s" to guess the pad power terminals.' \
                             % self.pvddick )
      return False

    availableDepths = set()
    masterCell      = self.powerPad.getMasterCell()
    for component in masterCell.getNet('vddi').getExternalComponents():
      if component.getBoundingBox().getYMin() <= masterCell.getAbutmentBox().getYMin():
        availableDepths.add( self.routingGauge.getLayerDepth(component.getLayer()) )

   #print 'available depth:', availableDepths

    self._horizontalPadDepth = 0
    self._verticalPadDepth   = 0
    for depth in range(0,self.topLayerDepth+1):
      if not depth in availableDepths: continue

      if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
        self._horizontalPadDepth = depth
      if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
        self._verticalPadDepth = depth

   #print 'h:', self._horizontalPadDepth
   #print 'v:', self._verticalPadDepth
    return


  def doLayout ( self ):
    if not self.validated: return
    UpdateSession.open()
    self.cell.setAbutmentBox( self.chipSize )

    for i in range(len(self._powerRails)):
      xBL = self._westSide .getAxis(i)
      yBL = self._southSide.getAxis(i)
      xTR = self._eastSide .getAxis(i)
      yTR = self._northSide.getAxis(i)

      self._corners[chip.SouthWest].append( 
          Contact.create( self.getRailNet(i)
                        , self.getRailLayer(i)
                        , xBL, yBL
                        , self.getRailWidth(i)
                        , self.getRailWidth(i)
                        ) )
      self._corners[chip.NorthWest].append( 
          Contact.create( self.getRailNet(i)
                        , self.getRailLayer(i)
                        , xBL, yTR
                        , self.getRailWidth(i)
                        , self.getRailWidth(i)
                        ) )
      self._corners[chip.SouthEast].append( 
          Contact.create( self.getRailNet(i)
                        , self.getRailLayer(i)
                        , xTR, yBL
                        , self.getRailWidth(i)
                        , self.getRailWidth(i)
                        ) )
      self._corners[chip.NorthEast].append( 
          Contact.create( self.getRailNet(i)
                        , self.getRailLayer(i)
                        , xTR, yTR
                        , self.getRailWidth(i)
                        , self.getRailWidth(i)
                        ) )

    self._northSide.doLayout()
    self._southSide.doLayout()
    self._eastSide.doLayout()
    self._westSide.doLayout()

    UpdateSession.close()
    return

#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/Core2Chip.py"         |
# +-----------------------------------------------------------------+

import re
from   Hurricane  import UpdateSession
from   Hurricane  import Net
from   Hurricane  import Instance
from   CRL        import Catalog
from   CRL        import AllianceFramework
from   helpers    import netDirectionToStr
from   helpers.io import ErrorMessage


# -------------------------------------------------------------------
# Class : "IoNet".


class IoNet ( object ):

  reVHDLVector = re.compile( r'(?P<name>[^(]*)\((?P<index>[\d]+)\)$' )

  def __init__ ( self, coreToChip, coreNet ):
      self.coreToChip  = coreToChip
      self.coreNet    = coreNet
      self.coronaNet  = None  # Corona net going from core to corona.
      self.chipIntNet = None  # Chip net going from corona to I/O pad.
      self.chipExtNet = None  # Chip net going from I/O pad to the outside world.
      self.pads       = [ ]
    
      m = IoNet.reVHDLVector.match( self.coreNet.getName() )
      if m:
        self._isElem = True
        self._name   = m.group('name')
        self._index  = m.group('index')
      else:
        self._isElem = False
        self._name   = self.coreNet.getName()
        self._index  = 0

      self._type = self.coreToChip.getNetType( self._name )
      return

  def isElem ( self ): return self._isElem

  def isGlobal ( self ): return self.isGlobal( self._name )

  @property
  def name ( self ): return self._name

  @property
  def index ( self ): return self._index

  @property
  def coreNetName ( self ): return self.coreNet.getName()

  @property
  def coronaNetName ( self ):
    s = self._name + '_core'
    if self._isElem: s  += '(' + self._index + ')'
    return s

  @property
  def padNetName ( self ):
    s = self._name
    if self._isElem: s  += '(' + self._index + ')'
    return s

  @property
  def padInstanceName ( self ):
    s = self._name
    if self._isElem: s  += '_' + self._index
    return s

  def buildNets ( self ):
    self.chipExtNet = Net.create( self.coreToChip.chip, self.padNetName )
    self.chipExtNet.setExternal ( True )
    self.chipExtNet.setDirection( self.coreNet.getDirection() )

    self.chipIntNet = Net.create( self.coreToChip.chip, self.coronaNetName )

    self.coronaNet  = Net.create( self.coreToChip.corona, self.coreNetName )
    self.coronaNet.setExternal ( True )
    self.coronaNet.setDirection( self.coreNet.getDirection() )

    self.coreToChip.icorona.getPlug( self.coronaNet ).setNet( self.chipIntNet )
    self.coreToChip.icore  .getPlug( self.coreNet   ).setNet( self.coronaNet  )

    netType = Net.Type.LOGICAL
    if self.coreNet.isPower (): netType = Net.Type.POWER
    if self.coreNet.isGround(): netType = Net.Type.GROUND
    if self.coreNet.isClock (): netType = Net.Type.CLOCK

    if netType != Net.Type.LOGICAL:
      self.chipIntNet.setType ( netType )
      self.coronaNet.setType  ( netType )
      self.coronaNet.setGlobal( True )
    return


# -------------------------------------------------------------------
# Class : "CoreToChip".


class CoreToChip ( object ):

    class IoPadInfo ( object ):

        def __init__ ( self, padName, padNet, coreNets ):
            self.name     = padName
            self.padNet   = padNet
            self.coreNets = coreNets
            return


    def getNetType ( self, netName ):
        raise ErrorMessage( 1, 'coreToChip.getNetType(): This method must be overloaded.' )

    def isGlobal ( self, netName ):
        raise ErrorMessage( 1, 'coreToChip.isGlobal(): This method must be overloaded.' )

    def getCell ( self, masterCellName ):
        raise ErrorMessage( 1, 'coreToChip.getCell(): This method must be overloaded.' )

    @staticmethod
    def _connect ( instance, netO, masterNetO=None ):
      if isinstance(netO,Net): chipNet = netO
      else:                    chipNet = instance.getCell().getNet( netO )

      if   not masterNetO:             masterNet = instance.getMasterCell().getNet( chipNet.getName() )
      elif isinstance(masterNetO,Net): masterNet = masterNetO
      else:                            masterNet = instance.getMasterCell().getNet( masterNetO )
  
      instance.getPlug( masterNet ).setNet( chipNet )
      return

    def __init__ ( self, core ):
        self.ringNetNames   = [ ]
        self.ioPads         = { }
        self.chipPads       = [ ]
        self.padLib         = None
        self.core           = core
        self.chip           = None
        self.corona         = None
        self.icorona        = None
        self.icore          = None
        self.powerPadCount  = 0
        self.groundPadCount = 0
        self.clockPadCount  = 0
        return

    def _connectRing ( self, padInstance ):
        for ringNetName in self.ringNetNames:
          if isinstance(ringNetName,tuple):
            CoreToChip._connect( padInstance, ringNetName[0], ringNetName[1] )
          else:
            CoreToChip._connect( padInstance, ringNetName )
        return

    def _buildRingNets ( self ):
        for ringNetSpec in self.ringNetNames:
          if isinstance(ringNetSpec,tuple): ringNetName = ringNetSpec[0]
          else:                             ringNetName = ringNetSpec
          ringNet  = Net.create( self.chip, ringNetName )
          ringNet.setType  ( self.getNetType(ringNetName) )
          ringNet.setGlobal( self.isGlobal  (ringNetName) )
        return

    def _buildStandardPad ( self, ioNet ):
      if not self.ioPads.has_key(ioNet.coreNet.getDirection()):
        raise ErrorMessage( 1, 'CoreToChip._buildStandardPad(): Unsupported direction %d for core net "%s".' \
                               % (netDirectionToStr(ioNet.coreNet.getDirection())
                                 ,ioNet.coreNet.getName()) )

      padInfo = self.ioPads[ ioNet.coreNet.getDirection() ]

      ioNet.pads.append( Instance.create( self.chip
                                        , ioNet.padInstanceName
                                        , self.getCell(padInfo.name) ) )
      CoreToChip._connect( ioNet.pads[0], ioNet.chipExtNet, padInfo.padNet )
      CoreToChip._connect( ioNet.pads[0], ioNet.chipIntNet, padInfo.coreNets[0] )
      self._connectRing( ioNet.pads[0] )
      self.chipPads += ioNet.pads
      return

    def _buildGroundPads ( self, ioNet ):
        raise ErrorMessage( 1, 'coreToChip._buildGroundPads(): This method must be overloaded.' )

    def _buildPowerPads ( self, ioNet ):
        raise ErrorMessage( 1, 'coreToChip._buildPowerPads(): This method must be overloaded.' )

    def _buildClockPads ( self, ioNet ):
        raise ErrorMessage( 1, 'coreToChip._buildClockPads(): This method must be overloaded.' )

    def buildChip ( self, chipName ):
        af = AllianceFramework.get()

        UpdateSession.open()
  
        self.chip    = af.createCell( chipName )
        self.corona  = af.createCell( 'corona' )
        self.icore   = Instance.create( self.corona, 'core'  , self.core   )
        self.icorona = Instance.create( self.chip  , 'corona', self.corona )

        self._buildRingNets()

        clockIoNets = [ ]
        for coreNet in self.core.getNets():
          if not coreNet.isExternal(): continue

          ioNet = IoNet( self, coreNet )
          ioNet.buildNets()

          if   coreNet.isPower():  self._buildPowerPads  ( ioNet )
          elif coreNet.isGround(): self._buildGroundPads ( ioNet )
          elif coreNet.isClock():  clockIoNets.append( ioNet )
          else:                    self._buildStandardPad( ioNet )

        for clockIoNet in clockIoNets:
          self._buildClockPads( clockIoNet )
          
        UpdateSession.close()

        af.saveCell( self.chip  , Catalog.State.Logical )
        af.saveCell( self.corona, Catalog.State.Logical )
        return

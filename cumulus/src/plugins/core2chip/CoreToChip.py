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
import chip.Chip


# -------------------------------------------------------------------
# Class : "IoNet".


class IoNet ( object ):

  IsElem       = 0x0001
  IsEnable     = 0x0002
  DoExtNet     = 0x0008
  reVHDLVector = re.compile( r'(?P<name>[^(]*)\((?P<index>[\d]+)\)$' )

  def __init__ ( self, coreToChip, coreNet ):
      self.coreToChip      = coreToChip
      self._flags          = 0
      self._chipExtNetName = None    # In the case of bidir pad, force external net name.
      self.coreNet         = coreNet
      self.coronaNet       = None    # Corona net going from core to corona.
      self.chipIntNet      = None    # Chip net going from corona to I/O pad.
      self.chipExtNet      = None    # Chip net going from I/O pad to the outside world.
    
      m = IoNet.reVHDLVector.match( self.coreNet.getName() )
      if m:
        self._flags |= IoNet.IsElem
        self._name   = m.group('name')
        self._index  = m.group('index')
      else:
        self._name   = self.coreNet.getName()
        self._index  = 0

      self._type = self.coreToChip.getNetType( self._name )
      return

  def __repr__ ( self ):
      s = '<IoNet "%s" ext:' % self.coreNet.getName()
      if self.chipExtNet: s += self.chipExtNet.getName()
      else:               s += 'None'
      s += ' int:'
      if self.chipIntNet: s += self.chipIntNet.getName()
      else:               s += 'None'
      s += '>'
      return s

  def isElem   ( self ): return self._flags & IoNet.IsElem
  def isEnable ( self ): return self._flags & IoNet.IsEnable

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
    if self._flags & IoNet.IsElem: s  += '(' + self._index + ')'
    return s

  @property
  def padNetName ( self ):
    if self._chipExtNetName is not None: return self._chipExtNetName

    s = self._name
    if self._flags & IoNet.IsElem: s  += '(' + self._index + ')'
    return s

  @padNetName.setter
  def padNetName ( self, name ): self._chipExtNetName = name

  @property
  def padInstanceName ( self ):
    s = self._name
    if self._flags & IoNet.IsElem: s  += '_' + self._index
    return s

  def setChipExtNetName ( self, name ): self.chipExtNetName = name

  def setEnable ( self, state ):
    if state == True: self._flags |=  IoNet.IsEnable
    else:             self._flags &= ~IoNet.IsEnable
    return

  def buildNets ( self, context=DoExtNet ):
    print 'context:', context

    netType = Net.Type.LOGICAL
    if self.coreNet.isPower (): netType = Net.Type.POWER
    if self.coreNet.isGround(): netType = Net.Type.GROUND
    if self.coreNet.isClock (): netType = Net.Type.CLOCK

   # Corona net, connect to Core instance net.
    if not self.coronaNet:
      self.coronaNet  = Net.create( self.coreToChip.corona, self.coreNetName )
      self.coronaNet.setExternal ( True )
      self.coronaNet.setDirection( self.coreNet.getDirection() )
      if netType != Net.Type.LOGICAL:
        self.coronaNet.setType  ( netType )
        self.coronaNet.setGlobal( True )

      self.coreToChip.icore.getPlug( self.coreNet ).setNet( self.coronaNet  )

   # Chip "internal" net, connect Corona instance net to I/O inside the chip.
    if not self.chipIntNet:
      self.chipIntNet = Net.create( self.coreToChip.chip, self.coronaNetName )
      if netType != Net.Type.LOGICAL:
        self.chipIntNet.setType( netType )

      self.coreToChip.icorona.getPlug( self.coronaNet ).setNet( self.chipIntNet )

   # Chip "external" net, connected to the pad I/O to the outside world.
    if not self.chipExtNet and (context & IoNet.DoExtNet):
      self.chipExtNet = Net.create( self.coreToChip.chip, self.padNetName )
      self.chipExtNet.setExternal ( True )
      self.chipExtNet.setDirection( self.coreNet.getDirection() )
      print 'PAD ', self.chipExtNet

    return


# -------------------------------------------------------------------
# Class : "IoPad".


class IoPad ( object ):

  IN          = 0x0001
  OUT         = 0x0002
  BIDIR       = 0x0004
  TRI_OUT     = 0x0008
  UNSUPPORTED = 0x0010


  @staticmethod
  def directionToStr ( direction ):
      if direction == IoPad.IN:          return "IN"
      if direction == IoPad.OUT:         return "OUT"
      if direction == IoPad.BIDIR:       return "BIDIR"
      if direction == IoPad.TRI_OUT:     return "TRI_OUT"
      if direction == IoPad.UNSUPPORTED: return "UNSUPPORTED"
      return "Invalid value"


  def __init__ ( self, coreToChip, padInstanceName ):
      self.coreToChip      = coreToChip
      self.padInstanceName = padInstanceName
      self.direction       = 0
      self.nets            = [ ]
      self.pads            = [ ]
      return


  def __str__ ( self ):
      s = '<IoPad "%s" ' % self.padInstanceName
      for ioNet in self.nets:
        s += ' %s' % ioNet.coreNetName
      s += '>'
      return s


  def addNet ( self, ioNet ):
      self.nets.append( ioNet )
      if len(self.nets) == 1:
        if   self.nets[0].coreNet.getDirection() == Net.Direction.IN:  self.direction = IoPad.IN
        elif self.nets[0].coreNet.getDirection() == Net.Direction.OUT: self.direction = IoPad.OUT
        else:
          raise ErrorMessage( 1, 'IoPad.addNet(): Unsupported direction %d (%s) for core net "%s" in I/O pad \"%s\".' \
                                 % ( self.nets[0].coreNet.getDirection()
                                   , netDirectionToStr(self.nets[0].coreNet.getDirection())
                                   , self.nets[0].coreNet.getName()
                                   , self.padInstanceName ) )
      elif len(self.nets) == 2:
        self.direction = IoPad.TRI_OUT
      elif len(self.nets) == 3:
        self.direction = IoPad.BIDIR
      else:
        message = [ 'IoPad.addNet(): More than three core nets on I/O pad "%s".'  % self.padInstanceName ]
        for ioNet in self.nets:
          message += [ "%s" % ioNet.coreNet ]
        raise ErrorMessage( 1, message )
                               
      return


  def createPad ( self ):
      fromCoreNet = None
      toCoreNet   = None
      enableNet   = None

      for ioNet in self.nets:
        print 'BEFORE ', ioNet, ioNet.coreNet
        context = 0

        if self.direction == IoPad.TRI_OUT:
          if ioNet.isEnable(): enableNet   = ioNet
          else:
            fromCoreNet = ioNet
            context    |= IoNet.DoExtNet
        elif self.direction == IoPad.BIDIR:
          if ioNet.coreNet.getDirection() == Net.Direction.IN: toCoreNet = ioNet
          if ioNet.coreNet.getDirection() == Net.Direction.OUT:
            if ioNet.isEnable(): enableNet   = ioNet
            else:
              fromCoreNet = ioNet
              context    |= IoNet.DoExtNet
        else:
          context    |= IoNet.DoExtNet
          fromCoreNet = ioNet

        ioNet.buildNets( context )
        print ioNet, context

      if not self.coreToChip.ioPadInfos.has_key(self.direction):
        raise ErrorMessage( 1, 'IoPad.createPad(): Unsupported direction %d (%s) for pad "%s".' \
                               % (self.direction
                                 ,IoPad.directionToStr(self.direction)
                                  ,self.padInstanceName) )
      padInfo = self.coreToChip.ioPadInfos[ self.direction ]
    
      self.pads.append( Instance.create( self.coreToChip.chip
                                       , self.padInstanceName
                                       , self.coreToChip.getCell(padInfo.name) ) )

      CoreToChip._connect( self.pads[0], fromCoreNet.chipExtNet, padInfo.padNet )
      CoreToChip._connect( self.pads[0], fromCoreNet.chipIntNet, padInfo.coreNets[0] )

      if toCoreNet: CoreToChip._connect( self.pads[0], toCoreNet.chipIntNet, padInfo.coreNets[-2] )
      if enableNet: CoreToChip._connect( self.pads[0], enableNet.chipIntNet, padInfo.coreNets[-1] )

      self.coreToChip._connectRing( self.pads[0] )
      self.coreToChip.chipPads += self.pads
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
        self.conf           = chip.Configuration.loadConfiguration( core )
        self.ringNetNames   = [ ]
        self.ioPadInfos     = { }
        self.chipPads       = [ ]
        self.padLib         = None
        self.core           = core
        self.chip           = None
        self.corona         = None
        self.icorona        = None
        self.icore          = None
        self._ioNets        = { }
        self.powerPadCount  = 0
        self.groundPadCount = 0
        self.clockPadCount  = 0
        return


    def hasIoNet ( self, netName ):
        if self._ioNets.has_key(netName): return True
        return False


    def getIoNet ( self, coreNet ):
        if isinstance(coreNet,str):
          if self._ioNets.has_key(coreNet): return self._ioNet[ coreNet ]
          raise ErrorMessage( 1, 'CoreToChip.getIoNet(): Cannot lookup net "%s" by name.' % coreNet )

        if not self._ioNets.has_key(coreNet.getName()):
          self._ioNets[ coreNet.getName() ] = IoNet( self, coreNet )

        return self._ioNets[ coreNet.getName() ]


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
      if not self.ioPadInfos.has_key(ioNet.coreNet.getDirection()):
        raise ErrorMessage( 1, 'CoreToChip._buildStandardPad(): Unsupported direction %d (%s) for core net "%s".' \
                               % (ioNet.coreNet.getDirection()
                                 ,netDirectionToStr(ioNet.coreNet.getDirection())
                                 ,ioNet.coreNet.getName()) )

      padInfo = self.ioPadInfos[ ioNet.coreNet.getDirection() ]

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

    def buildChip ( self ):
        af = AllianceFramework.get()

        UpdateSession.open()
  
        self.chip    = af.createCell( self.conf.chipName )
        self.corona  = af.createCell( 'corona' )
        self.icore   = Instance.create( self.corona, 'core'  , self.core   )
        self.icorona = Instance.create( self.chip  , 'corona', self.corona )

        self._buildRingNets()

        ioPads      = [ ]
        clockIoNets = [ ]

        for padConf in self.conf.padInstances:
          padConf.udata = IoPad( self, padConf.padInstanceName )

          for netName in padConf.nets:
            coreNet = self.core.getNet(netName)
            if not coreNet:
              raise ErrorMessage( 1, 'CoreToChip.buildChip(): "%s" doesn\'t have a "%s" net.'
                                     % (self.core.getName(),netName) )
            ioNet            = self.getIoNet( coreNet )
            ioNet.padNetName = padConf.padNetName

            if padConf.isBidir() or padConf.isTristate():
              if coreNet.getName() == padConf.enableNet:
                ioNet.setEnable( True )

            padConf.udata.addNet( ioNet )
          ioPads.append( padConf )
        
        for coreNet in self.core.getNets():
          if not coreNet.isExternal(): continue

         # Case of special nets (power, ground & clocks).
          if coreNet.isPower():
            self._buildPowerPads( IoNet( self, coreNet ) )
            continue
          elif coreNet.isGround():
            self._buildGroundPads( IoNet( self, coreNet ) )
            continue
          elif coreNet.isClock():
            clockIoNets.append( IoNet( self, coreNet ) )
            continue

          if self.hasIoNet(coreNet.getName()):
            continue

         # Remaining non configured Standard I/O pads.
          ioNet     = IoNet( self, coreNet )
          directPad = chip.Configuration.IoPadConf( [ ioNet.padInstanceName
                                                    , ioNet.padNetName
                                                    , ioNet.coreNetName
                                                    ] )
          directPad.udata = IoPad( self, directPad.padInstanceName )
          directPad.udata.addNet( ioNet )
          ioPads.append( directPad )

        for ioPad in ioPads:
          ioPad.udata.createPad()

        for clockIoNet in clockIoNets:
          self._buildClockPads( clockIoNet )
          
        UpdateSession.close()

        af.saveCell( self.chip  , Catalog.State.Logical )
        af.saveCell( self.corona, Catalog.State.Logical )
        return

# -*- explicit-buffer-name: "Configuration.py<cumulus/src/plugins/chip>" -*-
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
# |  Python      :       "./plugins/chip/Configuration.py"          |
# +-----------------------------------------------------------------+


import sys
import os.path
import Cfg
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Net
import CRL
from   helpers   import ErrorMessage
from   helpers   import WarningMessage
from   plugins   import getParameter


class Configuration ( object ):

    @staticmethod
    def _readChipSize( chipConfig ):
      if not chipConfig.has_key('chip.size'): return Box()
      chipSize = chipConfig['chip.size']
      if not isinstance(chipSize,tuple):
        print ErrorMessage( 1, 'The Chip size parameter is *not* a tuple.' )
        return Box()
      if len(chipSize) != 2:
        print ErrorMessage( 1, 'The Chip size parameter is *not* a tuple of exactly two items.' )
        return Box()
      return Box( 0, 0, DbU.fromLambda(chipSize[0]), DbU.fromLambda(chipSize[1]) )


    @staticmethod
    def _readCoreSize( chipConfig ):
      if not chipConfig.has_key('core.size'):
        print ErrorMessage( 1, 'The Core size parameter is missing.' )
        return Box()
      coreSize = chipConfig['core.size']
      if not isinstance(coreSize,tuple):
        print ErrorMessage( 1, 'The Core size parameter is *not* a tuple.' )
        return Box()
      if len(coreSize) != 2:
        print ErrorMessage( 1, 'The Core size parameter is *not* a tuple of exactly two items.' )
        return Box()
      return Box( 0, 0, DbU.fromLambda(coreSize[0]), DbU.fromLambda(coreSize[1]) )


    def _readPads ( self, chipConfig, keyword ):
      if not chipConfig.has_key(keyword): return []
      padNameList = chipConfig[keyword]
      if not isinstance(padNameList,list):
          print ErrorMessage( 1, 'The "%s" entry is not a list.' )
          return []

      af      = CRL.AllianceFramework.get()
      padList = []
      for i in range(len(padNameList)):
        if not isinstance(padNameList[i],str):
          print ErrorMessage( 1, 'The element [%d] of list %s is *not* a string (skipped).'
                                  % (i,keyword) )
          continue

        instance = self._cell.getInstance( padNameList[i] )
        if not instance:
          print ErrorMessage( 1, 'The pad [%d] (%s) of list %s do not exists in netlist (skipped).'
                                  % (i,padNameList[i],keyword) )
          continue

        if (not af.isPad(instance.getMasterCell().getName())):
          print ErrorMessage( 1, 'The pad [%d] (%s) of list %s is not an instance of a pad cell (skipped).'
                                  % (i,padNameList[i],keyword) )
          continue

        padList.append( instance )

        if not self._clockPad and instance.getMasterCell().getName() == 'pck_px':
          self._clockPad = instance

        if not self._powerPad and instance.getMasterCell().getName() == 'pvddick_px':
          self._powerPad = instance
      
      return padList


    def _guessGlobalNet ( self, name, net ):
      if name == self._vddeName: self._vdde = net
      if name == self._vddiName: self._vddi = net
      if name == self._vsseName: self._vsse = net
      if name == self._vssiName: self._vssi = net
      if name == self._ckiName:  self._cki  = net
      if name == self._ckoName:  self._cko  = net
      if name == self._ckName:   self._ck   = net
      return


    def __init__ ( self, chipConfig, cell ):
      if not isinstance(chipConfig,dict):
          raise ErrorMessage( 1, 'The "chip" variable is not a dictionnary.' )

      self._validated     = True
      self._routingGauge  = None
      self._topLayerDepth = 0
      self._cell          = cell
     # Block Corona parameters.
      self._railsNb         = DbU.fromLambda( getParameter('chip','chip.block.rails.count'   ).asInt() )
      self._hRailWidth      = DbU.fromLambda( getParameter('chip','chip.block.rails.hWidth'  ).asInt() )
      self._vRailWidth      = DbU.fromLambda( getParameter('chip','chip.block.rails.vWidth'  ).asInt() )
      self._hRailSpace      = DbU.fromLambda( getParameter('chip','chip.block.rails.hSpacing').asInt() )
      self._vRailSpace      = DbU.fromLambda( getParameter('chip','chip.block.rails.vSpacing').asInt() )
     # Global net names.
      self._vddeName      = "vdde"
      self._vddiName      = "vddi"
      self._vsseName      = "vsse"
      self._vssiName      = "vssi"
      self._ckiName       = "ck"
      self._ckoName       = "cko"
      self._ckName        = "pad"
     # Global net names.  
      self._vdde          = None
      self._vddi          = None
      self._vsse          = None
      self._vssi          = None
      self._cki           = None
      self._cko           = None
      self._ck            = None
                          
      self._clockPad      = None
      self._powerPad      = None
      self._cores         = []
      self._southPads     = self._readPads( chipConfig, 'pads.south' )
      self._northPads     = self._readPads( chipConfig, 'pads.north' )
      self._eastPads      = self._readPads( chipConfig, 'pads.east'  )
      self._westPads      = self._readPads( chipConfig, 'pads.west'  )
      self._coreSize      = Configuration._readCoreSize( chipConfig )
      self._chipSize      = Configuration._readChipSize( chipConfig )
      self._minCorona     = DbU.fromLambda( 100 )
      self._padWidth      = 0
      self._padHeight     = 0

      self.loadRoutingGauge()
      self.checkPads()
      self.computeChipSize()
      self.findPowerAndClockNets()
      return


    def loadRoutingGauge ( self ):
      self._routingGauge = CRL.AllianceFramework.get().getRoutingGauge()

      topLayer = Cfg.getParamString('katabatic.topRoutingLayer').asString()

      self._topLayerDepth = 0
      for layerGauge in self._routingGauge.getLayerGauges():
        if layerGauge.getLayer().getName() == topLayer:
          self._topLayerDepth = layerGauge.getDepth()
          break
      if not self._topLayerDepth:
        print WarningMessage( 'Gauge top layer not defined, using top of gauge (%d).' \
                              % self._routingGauge.getDepth() )
        self._topLayerDepth = self._routingGauge.getDepth()
      return


    def checkPads ( self ):
      af       = CRL.AllianceFramework.get()
      cellPads = []
      for instance in self._cell.getInstances():
        if (af.isPad(instance.getMasterCell().getName())):
          cellPads.append( instance )
        else:
          self._cores.append( instance )
            
      for pad in cellPads:
        if pad in self._southPads: continue
        if pad in self._northPads: continue
        if pad in self._eastPads:  continue
        if pad in self._westPads:  continue
        print ErrorMessage( 1, 'Pad "%s" is not on any side (N/S/E/W).' % pad.getName() )
        self._validated = False

      if len(self._cores) < 1:
        print ErrorMessage( 1, 'Chip "%s" doesn\'t seems to have a core.' % self._cell.getName() )
        self._validated = False

      if len(self._cores) > 1:
        message = [ 'Chip "%s" have more than one core:' % self._cell.getName() ]
        for i in range(len(self._cores)):
          message.append( '%4d: %s' % (i,self._cores[i].getName()) )
        print ErrorMessage( 1, message )
        self._validated = False

      return


    def findPowerAndClockNets ( self ):
      if self._powerPad:
        for plug in self._powerPad.getPlugs():
          masterNet = plug.getMasterNet()
          netType   = masterNet.getType()
          if     netType != Net.Type.POWER \
             and netType != Net.Type.GROUND \
             and netType != Net.Type.CLOCK:
            continue

          net = plug.getNet()
          if not net:
            net = self._cell.getNet( masterNet.getName() )
            if not net:
              print ErrorMessage( 1, 'Missing global net <%s> at chip level.' % masterNet.getName() )
              self._validated = False
              continue
          self._guessGlobalNet( masterNet.getName(), net )

      if self._clockPad:
        for plug in self._powerPad.getPlugs():
          masterNet = plug.getMasterNet()
          netType   = masterNet.getType()
          net = plug.getNet()

          if not net:
            net = self._cell.getNet( masterNet.getName() )
            if not net:
              print ErrorMessage( 1, 'Missing global net <%s> at chip level.' % masterNet.getName() )
              self._validated = False
              continue

          if masterNet.getName() == self._ckName:
            self._guessGlobalNet( masterNet.getName(), net )
      return


    def computeChipSize ( self ):
      if not self._clockPad:
        print ErrorMessage( 1, 'There must be at least one pad of model "pck_px" to be used as reference.' )
        self._validated = False
        return False

      self._padHeight = self._clockPad.getMasterCell().getAbutmentBox().getHeight()
      self._padWidth  = self._clockPad.getMasterCell().getAbutmentBox().getWidth()

      if not self._chipSize.isEmpty(): return
      
      horizontalPads = max( len(self._southPads), len(self._northPads) )
      verticalPads   = max( len(self._eastPads ), len(self._westPads ) )
      self._chipSize  = Box( 0
                           , 0
                           , self._padWidth * horizontalPads + 2*self._padHeight
                           , self._padWidth * verticalPads   + 2*self._padHeight
                           )
      return

    def getSpecialNetRoot ( self, net ):
      if net.getName() == self._vddeName:  return self._vdde
      if net.getName() == self._vsseName:  return self._vsse
      if net.getType() == Net.Type.POWER:  return self._vddi
      if net.getType() == Net.Type.GROUND: return self._vssi
      return None


class Wrapper ( object ):

    def __init__ ( self, conf ):
      if not isinstance(conf,Configuration):
        raise ErrorMessage('Attempt to create a Wrapper() from non-Configuration object.')
      self._conf = conf
      return

    def isValid ( self ): return self._conf._validated

    @property
    def conf          ( self ): return self._conf

    @property
    def routingGauge  ( self ): return self._conf._routingGauge
    @property
    def topLayerDepth ( self ): return self._conf._topLayerDepth

    @property
    def cell          ( self ): return self._conf._cell

    # Global Net names.
    @property
    def vddeName ( self ): return self._conf._vddeName
    @property
    def vsseName ( self ): return self._conf._vsseName
    @property
    def vddiName ( self ): return self._conf._vddiName
    @property
    def vssiName ( self ): return self._conf._vssiName
    @property
    def ckiName  ( self ): return self._conf._ckiName
    @property
    def ckoName  ( self ): return self._conf._ckoName
    @property
    def ckName   ( self ): return self._conf._ckName

    # Global Nets.
    @property
    def vdde ( self ): return self._conf._vdde
    @property
    def vsse ( self ): return self._conf._vsse
    @property
    def vddi ( self ): return self._conf._vddi
    @property
    def vssi ( self ): return self._conf._vssi
    @property
    def cki  ( self ): return self._conf._cki
    @property
    def cko  ( self ): return self._conf._cko
    @property
    def ck   ( self ): return self._conf._ck

    # Various.
    @property
    def clockPad  ( self ): return self._conf._clockPad
    @property
    def powerPad  ( self ): return self._conf._powerPad
    @property
    def cores     ( self ): return self._conf._cores
    @property
    def southPads ( self ): return self._conf._southPads
    @property
    def northPads ( self ): return self._conf._northPads
    @property
    def eastPads  ( self ): return self._conf._eastPads
    @property
    def westPads  ( self ): return self._conf._westPads
    @property
    def coreSize  ( self ): return self._conf._coreSize
    @coreSize.setter
    def coreSize  ( self, ab ): self._conf._coreSize = ab
    @property
    def chipSize  ( self ): return self._conf._chipSize
    @property
    def minCorona ( self ): return self._conf._minCorona
    @property
    def padWidth  ( self ): return self._conf._padWidth
    @property
    def padHeight ( self ): return self._conf._padHeight


def loadConfiguration ( cell ):
    sys.path.append( os.getcwd() )

    confFile   = cell.getName()+'_chip'
    confModule = __import__( confFile, globals(), locals(), confFile )

    if not confModule.__dict__.has_key('chip'):
      raise WarningMessage( 'Module <%s> do not provides the chip variable, skipped.' \
                            % confFile )

    return Wrapper( Configuration( confModule.__dict__['chip'], cell ) )

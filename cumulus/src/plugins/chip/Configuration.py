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
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Occurrence
from   Hurricane import Net
from   Hurricane import RoutingPad
from   Hurricane import Contact
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Plug
from   Hurricane import Instance
import CRL
from   CRL import RoutingLayerGauge
from   helpers   import trace
from   helpers   import ErrorMessage
from   helpers   import WarningMessage
from   plugins   import getParameter


def breakpoint ( editor, level, message ):
  if editor:
    editor.fit()
    editor.refresh()
    Breakpoint.stop( level, message )
  return


def getPlugByName ( instance, netName ):
  masterCell = instance.getMasterCell()
  masterNet  = masterCell.getNet( netName )
  if masterNet:
    return instance.getPlug( masterNet )
  return None


def getPlugByNet ( instance, net ):
  for plug in net.getPlugs():
    if plug.getInstance() == instance:
      return plug
  return None


def getRpBb ( instance, netName ):
  bb = Box()
  for net in instance.getMasterCell().getNets():
    if net.isExternal() and net.getName() == netName:
      for component in net.getExternalComponents():
        if isinstance(component,Vertical):
          bb = component.getBoundingBox()
  instance.getTransformation().applyOn( bb )
  return bb


def showNet ( cell, netName ):
  net = cell.getNet(netName)
  if not net:
    print ErrorMessage( 3, 'Cell %s doesn\'t have net %s' % (cell.getName(),netName) ) 
    return
  
  print 'Components of', netName
  for component in net.getComponents():
    print '| ', component, component.getBoundingBox() 
  return


def destroyNetComponents ( net ):
 # 1. We cannot iterate over a Hurricane Collection if we are deleting
 #    some of it's elements at the same time (could be improved as it
 #    is an intrusive map.
 # 2. Lazy programming: as we don't know the destruction order, some
 #    components can be deleted by (previous) others so we can endup
 #    on dangling Python proxy which send an exception that we catch.
 # 3. Plugs are not destroyed (they cannot as they are part of the
 #    Instance). They stay connected to the net.
  toDestroy = []
  for component in net.getComponents():
    if not isinstance(component,Plug):
      toDestroy.append( component )
  for component in toDestroy:
    try:
      component.destroy()
    except:
     pass
  return


# -------------------------------------------------------------------
# Class  :  "Configuration.GaugeConf".

class GaugeConf ( object ):

    HAccess       = 0x0001
    OffsetRight1  = 0x0002
    OffsetTop1    = 0x0004
    OffsetBottom1 = 0x0008

    def __init__ ( self ):
      self._cellGauge     = None
      self._routingGauge  = None
      self._topLayerDepth = 0
      self._plugToRp      = { }

      self._loadRoutingGauge()
      return

    def getSliceHeight ( self ): return self._cellGauge.getSliceHeight()
    def getSliceStep   ( self ): return self._cellGauge.getSliceStep()

    def _loadRoutingGauge ( self ):
      self._cellGauge    = CRL.AllianceFramework.get().getCellGauge()
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

      self._horizontalDepth = 0
      self._verticalDepth   = 0
      for depth in range(0,self._topLayerDepth+1):
        if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
          self._horizontalDepth = depth
        if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
          self._verticalDepth = depth
      return

    def _createContact ( self, net, x, y ):
      return Contact.create( net
                           , self._routingGauge.getContactLayer(self._horizontalDepth)
                           , x, y
                           , self._routingGauge.getLayerGauge(self._horizontalDepth).getViaWidth()
                           , self._routingGauge.getLayerGauge(self._horizontalDepth).getViaWidth()
                           )

    def _createHorizontal ( self, source, target, y ):
      segment = Horizontal.create( source
                                 , target
                                 , self._routingGauge.getRoutingLayer(self._horizontalDepth)
                                 , y
                                 , self._routingGauge.getLayerGauge(self._horizontalDepth).getWireWidth()
                                 )
      trace( 550, segment )
      return segment
  
    def _createVertical ( self, source, target, x ):
      segment = Vertical.create( source
                               , target
                               , self._routingGauge.getRoutingLayer(self._verticalDepth)
                               , x
                               , self._routingGauge.getLayerGauge(self._verticalDepth).getWireWidth()
                               )
      trace( 550, segment )
      return segment
  
    def _rpAccess ( self, rp, flags ):
      trace( 550, ',+', '\t_rpAccess() %s\n' % str(rp) )

      hpitch    = self._routingGauge.getLayerGauge(self._horizontalDepth).getPitch()
      hoffset   = self._routingGauge.getLayerGauge(self._horizontalDepth).getOffset()
      contact1  = Contact.create( rp, self._routingGauge.getContactLayer(0), 0, 0 )
      midSliceY = contact1.getY() - (contact1.getY() % self._cellGauge.getSliceHeight()) \
                                                     + self._cellGauge.getSliceHeight() / 2
      midTrackY = midSliceY - ((midSliceY - hoffset) % hpitch)
      dy        = midSliceY - contact1.getY()
  
      if flags & GaugeConf.OffsetBottom1: dy += hpitch
      if flags & GaugeConf.OffsetTop1:    dy -= hpitch
      contact1.setDy( dy )

      trace( 550, contact1 )
  
      if flags & GaugeConf.HAccess: stopDepth = self._horizontalDepth
      else:                         stopDepth = self._verticalDepth
  
      for depth in range(1,stopDepth):
        xoffset = 0
        if flags & GaugeConf.OffsetRight1 and depth == 1:
          xoffset = self._routingGauge.getLayerGauge(depth+1).getPitch()
        contact2 = Contact.create( rp.getNet()
                                 , self._routingGauge.getContactLayer(depth)
                                 , contact1.getX() + xoffset
                                 , contact1.getY()
                                 , self._routingGauge.getLayerGauge(depth).getViaWidth()
                                 , self._routingGauge.getLayerGauge(depth).getViaWidth()
                                 )
        trace( 550, contact2 )
        if self._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
          segment = Horizontal.create( contact1
                                     , contact2
                                     , self._routingGauge.getRoutingLayer(depth)
                                     , contact1.getY()
                                     , self._routingGauge.getLayerGauge(depth).getWireWidth()
                                     )
          trace( 550, segment )
        else:
          segment = Vertical.create( contact1
                                   , contact2
                                   , self._routingGauge.getRoutingLayer(depth)
                                   , contact1.getX()
                                   , self._routingGauge.getLayerGauge(depth).getWireWidth()
                                   )
          trace( 550, segment )
        contact1 = contact2
  
      trace( 550, '-' )
      return contact1
  
    def _rpByOccurrence ( self, occurrence, net ):
      plug = occurrence.getEntity()
      if self._plugToRp.has_key(plug):
        rp = self._plugToRp[plug]
      else:
        rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
        self._plugToRp[plug] = rp
  
      return rp
  
    def _rpAccessByOccurrence ( self, occurrence, net, flags ):
      plug = occurrence.getEntity()
      if self._plugToRp.has_key(plug):
        rp = self._plugToRp[plug]
      else:
        rp = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
        self._plugToRp[plug] = rp
  
      return self._rpAccess( self._rpByOccurrence(occurrence,net), flags )
  
    def _rpByPlug ( self, plug, net ):
      if self._plugToRp.has_key(plug):
        rp = self._plugToRp[plug]
      else:
        occurrence = Occurrence( plug, Path(net.getCell(),'') )
        rp         = RoutingPad.create( net, occurrence, RoutingPad.BiggestArea )
        self._plugToRp[plug] = rp
  
      return rp
  
    def _rpByPlugName ( self, instance, plugName, net ):
      return self._rpByPlug( getPlugByName(instance,plugName), net )
  
    def _rpAccessByPlug ( self, plug, net, flags ):
      return self._rpAccess( self._rpByPlug(plug,net), flags )
  
    def _rpAccessByPlugName ( self, instance, plugName, net, flags=0 ):
      return self._rpAccess( self._rpByPlugName(instance,plugName,net), flags )


# -------------------------------------------------------------------
# Class  :  "Configuration.GaugeConfWrapper".

class GaugeConfWrapper ( object ):

    def __init__ ( self, conf ):
     #print id(conf), type(conf)
     #if not isinstance(conf,GaugeConf):
     #  raise ErrorMessage( 1, 'Attempt to create a GaugeConfWrapper() from non-GaugeConf object.' )
      self._gaugeConf = conf
      return

    @property
    def gaugeConf ( self ): return self._gaugeConf

    @property
    def routingGauge    ( self ): return self._gaugeConf._routingGauge
    @property
    def topLayerDepth   ( self ): return self._gaugeConf._topLayerDepth
    @property
    def horizontalDepth ( self ): return self._gaugeConf._horizontalDepth
    @property
    def verticalDepth   ( self ): return self._gaugeConf._verticalDepth

    def loadRoutingGauge ( self ): self._gaugeConf._loadRoutingGauge()

    def rpByOccurrence ( self, occurrence, net ):
      return self._gaugeConf._rpByOccurrence ( occurrence, net )

    def rpByPlugName ( self, instance, plugName, net ):
      return self._gaugeConf._rpByPlugName ( instance, plugName, net )

    def rpAccess ( self, rp, flags=0 ):
      return self._gaugeConf._rpAccess( rp, flags )

    def rpAccessByOccurrence ( self, occurrence, net, flags=0 ):
      return self.gaugeConf._rpAccessByOccurrence ( occurrence, net, flags )

    def rpAccessByPlugName ( self, instance, plugName, net, flags=0 ):
      return self._gaugeConf._rpAccessByPlugName( instance, plugName, net, flags )

    def createContact ( self, net, x, y ):
      return self._gaugeConf._createContact( net, x, y )

    def createHorizontal ( self, source, target, y ):
      return self._gaugeConf._createHorizontal( source, target, y )

    def createVertical ( self, source, target, x ):
      return self._gaugeConf._createVertical( source, target, x )


# -------------------------------------------------------------------
# Class  :  "Configuration.ChipConf".

class ChipConf ( object ):

    @staticmethod
    def _readChipSize( chipConfigDict ):
      if not chipConfigDict.has_key('chip.size'): return Box()
      chipSize = chipConfigDict['chip.size']
      if not isinstance(chipSize,tuple):
        print ErrorMessage( 1, 'The Chip size parameter is *not* a tuple.' )
        return Box()
      if len(chipSize) != 2:
        print ErrorMessage( 1, 'The Chip size parameter is *not* a tuple of exactly two items.' )
        return Box()
      return Box( 0, 0, DbU.fromLambda(chipSize[0]), DbU.fromLambda(chipSize[1]) )


    @staticmethod
    def _readCoreSize( chipConfigDict ):
      if not chipConfigDict.has_key('core.size'):
        print ErrorMessage( 1, 'The Core size parameter is missing.' )
        return Box()
      coreSize = chipConfigDict['core.size']
      if not isinstance(coreSize,tuple):
        print ErrorMessage( 1, 'The Core size parameter is *not* a tuple.' )
        return Box()
      if len(coreSize) != 2:
        print ErrorMessage( 1, 'The Core size parameter is *not* a tuple of exactly two items.' )
        return Box()
      return Box( 0, 0, DbU.fromLambda(coreSize[0]), DbU.fromLambda(coreSize[1]) )


    @staticmethod
    def _readClockTree( chipConfigDict ):
      useClockTree = False
      if chipConfigDict.has_key('chip.clockTree'):
        if chipConfigDict['chip.clockTree']:
          useClockTree = True
      return useClockTree


    def _readPads ( self, chipConfigDict, keyword ):
      if not chipConfigDict.has_key(keyword): return []
      padNameList = chipConfigDict[keyword]
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

        if not self._clockPad and instance.getMasterCell().getName() == self._pckName:
          self._clockPad = instance

        if not self._powerPad and instance.getMasterCell().getName() == self._pvddickName:
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


    def __init__ ( self, chipConfigDict, cell ):
      if not isinstance(chipConfigDict,dict):
          raise ErrorMessage( 1, 'The "chip" variable is not a dictionnary.' )

      self._validated     = True
      self._cell          = cell
     # Block Corona parameters.
      self._railsNb       = getParameter('chip','chip.block.rails.count').asInt()
      self._hRailWidth    = DbU.fromLambda( getParameter('chip','chip.block.rails.hWidth'  ).asInt() )
      self._vRailWidth    = DbU.fromLambda( getParameter('chip','chip.block.rails.vWidth'  ).asInt() )
      self._hRailSpace    = DbU.fromLambda( getParameter('chip','chip.block.rails.hSpacing').asInt() )
      self._vRailSpace    = DbU.fromLambda( getParameter('chip','chip.block.rails.vSpacing').asInt() )
     # Global Pad names.
      self._pckName       = getParameter('chip', 'chip.pad.pck'    ).asString()
      self._pvddickName   = getParameter('chip', 'chip.pad.pvddick').asString()
      self._pvssickName   = getParameter('chip', 'chip.pad.pvssick').asString()
      self._pvddeckName   = getParameter('chip', 'chip.pad.pvddeck').asString()
      self._pvsseckName   = getParameter('chip', 'chip.pad.pvsseck').asString()
     # Global Net names.
      self._vddeName      = "vdde"
      self._vddiName      = "vddi"
      self._vsseName      = "vsse"
      self._vssiName      = "vssi"
      self._ckiName       = "ck"
      self._ckoName       = "cko"
      self._ckName        = "pad"
     # Global Nets.  
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
      self._southPads     = self._readPads( chipConfigDict, 'pads.south' )
      self._northPads     = self._readPads( chipConfigDict, 'pads.north' )
      self._eastPads      = self._readPads( chipConfigDict, 'pads.east'  )
      self._westPads      = self._readPads( chipConfigDict, 'pads.west'  )
      self._coreSize      = ChipConf._readCoreSize( chipConfigDict )
      self._chipSize      = ChipConf._readChipSize( chipConfigDict )
      self._padWidth      = 0
      self._padHeight     = 0
      self._useClockTree  = ChipConf._readClockTree( chipConfigDict )

      minHCorona          = self._railsNb*(self._hRailWidth + self._hRailSpace) + self._hRailSpace
      minVCorona          = self._railsNb*(self._vRailWidth + self._vRailSpace) + self._vRailSpace
      if minHCorona > minVCorona: self._minCorona = minHCorona*2
      else:                       self._minCorona = minVCorona*2

      self.checkPads()
      self.computeChipSize()
      self.findPowerAndClockNets()
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
        print ErrorMessage( 1, 'There must be at least one pad of model "%s" to be used as reference.' \
                               % self._pckName )
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


# -------------------------------------------------------------------
# Class  :  "Configuration.ChipConfWrapper".

class ChipConfWrapper ( GaugeConfWrapper ):

    def __init__ ( self, gaugeConf, chipConf ):
      GaugeConfWrapper.__init__( self, gaugeConf )

     #if not isinstance(chipConf,ChipConf):
     #  raise ErrorMessage( 1, 'Attempt to create a ChipConfWrapper() from non-ChipConf object.' )
      self._chipConf = chipConf
      return

    def isValid ( self ): return self._chipConf._validated

    @property
    def chipConf ( self ): return self._chipConf

    def getSliceHeight ( self ): return self._gaugeConf.getSliceHeight()
    def getSliceStep   ( self ): return self._gaugeConf.getSliceStep()

    @property
    def cell ( self ): return self._chipConf._cell

    # Global Pad names.
    @property
    def pvddeckName ( self ): return self._chipConf._pvddeckName
    @property
    def pvsseckName ( self ): return self._chipConf._pvsseckName
    @property
    def pvddickName ( self ): return self._chipConf._pvddickName
    @property
    def pvssickName ( self ): return self._chipConf._pvssickName
    @property
    def pckName     ( self ): return self._chipConf._pckName

    # Global Net names.
    @property
    def vddeName ( self ): return self._chipConf._vddeName
    @property
    def vsseName ( self ): return self._chipConf._vsseName
    @property
    def vddiName ( self ): return self._chipConf._vddiName
    @property
    def vssiName ( self ): return self._chipConf._vssiName
    @property
    def ckiName  ( self ): return self._chipConf._ckiName
    @property
    def ckoName  ( self ): return self._chipConf._ckoName
    @property
    def ckName   ( self ): return self._chipConf._ckName

    # Global Nets.
    @property
    def vdde ( self ): return self._chipConf._vdde
    @property
    def vsse ( self ): return self._chipConf._vsse
    @property
    def vddi ( self ): return self._chipConf._vddi
    @property
    def vssi ( self ): return self._chipConf._vssi
    @property
    def cki  ( self ): return self._chipConf._cki
    @property
    def cko  ( self ): return self._chipConf._cko
    @property
    def ck   ( self ): return self._chipConf._ck

    # Various.
    @property
    def clockPad     ( self ): return self._chipConf._clockPad
    @property        
    def powerPad     ( self ): return self._chipConf._powerPad
    @property        
    def cores        ( self ): return self._chipConf._cores
    @property        
    def southPads    ( self ): return self._chipConf._southPads
    @property        
    def northPads    ( self ): return self._chipConf._northPads
    @property        
    def eastPads     ( self ): return self._chipConf._eastPads
    @property        
    def westPads     ( self ): return self._chipConf._westPads
    @property        
    def coreSize     ( self ): return self._chipConf._coreSize
    @coreSize.setter
    def coreSize     ( self, ab ): self._chipConf._coreSize = ab
    @property        
    def chipSize     ( self ): return self._chipConf._chipSize
    @property        
    def minCorona    ( self ): return self._chipConf._minCorona
    @property        
    def padWidth     ( self ): return self._chipConf._padWidth
    @property        
    def padHeight    ( self ): return self._chipConf._padHeight
    @property
    def useClockTree ( self ): return self._chipConf._useClockTree


def loadConfiguration ( cell ):
    sys.path.append( os.getcwd() )

    confFile   = cell.getName()+'_chip'
    confModule = __import__( confFile, globals(), locals(), confFile )

    if not confModule.__dict__.has_key('chip'):
      raise WarningMessage( 'Module <%s> do not provides the chip variable, skipped.' \
                            % confFile )

    return ChipConfWrapper( GaugeConf()
                          , ChipConf ( confModule.__dict__['chip'], cell ) )

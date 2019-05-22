# -*- explicit-buffer-name: "Configuration.py<cumulus/src/plugins/chip>" -*-
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
# |  Python      :       "./plugins/chip/Configuration.py"          |
# +-----------------------------------------------------------------+


import sys
import os.path
import Cfg
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Occurrence
from   Hurricane import Net
from   Hurricane import RoutingPad
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Contact
from   Hurricane import Pin
from   Hurricane import Plug
from   Hurricane import Instance
import CRL
from   CRL import RoutingLayerGauge
from   helpers    import trace
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
from   plugins    import getParameter
import chip


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

    HAccess         = 0x0001
    OffsetRight1    = 0x0002
    OffsetTop1      = 0x0004
    OffsetBottom1   = 0x0008
    DeepDepth       = 0x0010
    UseContactWidth = 0x0020
    ExpandWidth     = 0x0040

    def __init__ ( self ):
      self.cellGauge      = None
      self.ioPadGauge     = None
      self.routingGauge   = None
      self.topLayerDepth  = 0
      self._plugToRp      = { }
      self._rpToAccess    = { }

      self._loadRoutingGauge()
      return

    def getSliceHeight   ( self ): return self.cellGauge.getSliceHeight()
    def getSliceStep     ( self ): return self.cellGauge.getSliceStep()
    def getIoPadHeight   ( self ): return self.ioPadGauge.getSliceHeight()
    def getIoPadStep     ( self ): return self.ioPadGauge.getSliceStep()
    def getIoPadPitch    ( self ): return self.ioPadGauge.getPitch()
    def getIoPadGauge    ( self ): return self.ioPadGauge
    def getHRoutingGauge ( self ): return self.routingGauge.getLayerGauge( self.horizontalDepth )
    def getVRoutingGauge ( self ): return self.routingGauge.getLayerGauge( self.verticalDepth )

    def _loadRoutingGauge ( self ):
      gaugeName = Cfg.getParamString('anabatic.routingGauge').asString()
      self.cellGauge    = CRL.AllianceFramework.get().getCellGauge( gaugeName )
      self.routingGauge = CRL.AllianceFramework.get().getRoutingGauge( gaugeName )

      topLayer = Cfg.getParamString('anabatic.topRoutingLayer').asString()

      self.topLayerDepth = 0
      for layerGauge in self.routingGauge.getLayerGauges():
        if layerGauge.getLayer().getName() == topLayer:
          self.topLayerDepth = layerGauge.getDepth()
          break
      if not self.topLayerDepth:
        print WarningMessage( 'Gauge top layer not defined, using top of gauge (%d).' \
                              % self.routingGauge.getDepth() )
        self.topLayerDepth = self.routingGauge.getDepth() - 1

      self.horizontalDepth     = -1
      self.verticalDepth       = -1
      self.horizontalDeepDepth = -1
      self.verticalDeepDepth   = -1
      for depth in range(0,self.topLayerDepth+1):
        trace( 550, '\tdepth:%d\n' % depth )
        trace( 550, '\t%s\n'       % self.routingGauge.getLayerGauge(depth) )

        if self.routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.PinOnly:
          continue
        if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
          if self.horizontalDeepDepth < 0:
            self.horizontalDeepDepth = depth
          self.horizontalDepth = depth
        if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
          if self.verticalDeepDepth < 0:
            self.verticalDeepDepth = depth
          self.verticalDepth = depth
      return

    def _loadIoPadGauge ( self, ioPadGaugeName ):
      self.ioPadGauge = CRL.AllianceFramework.get().getCellGauge( ioPadGaugeName )
      if not self.ioPadGauge:
        print WarningMessage( 'IO pad gauge "%s" not found.' % ioPadGaugeName )
      return

    def isHorizontal ( self, layer ):
      mask = layer.getMask()
      for lg in self.routingGauge.getLayerGauges():
        if lg.getLayer().getMask() == mask:
          if lg.getDirection() == RoutingLayerGauge.Horizontal: return True
          return False

      print ErrorMessage( 1, 'GaugeConf.isHorizontal(): Layer "%s" is not part of gauge "%s", cannot know preferred direction.' \
                              % (layer.getName(), self.routingGauge.getName()) )
      return False

    def isVertical ( self, layer ): return not self.isHorizontal( layer )

    def _createContact ( self, net, x, y, flags ):
      if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
      else:                           depth = self.horizontalDepth

      if self.horizontalDepth > self.verticalDepth: depth -= 1

      trace( 550, '\t%s, horizontalDepth:%d, gaugeDepth:%d\n'
             % (self.routingGauge,self.horizontalDepth,self.routingGauge.getDepth()))
      return Contact.create( net
                           , self.routingGauge.getContactLayer(depth)
                           , x, y
                           , self.routingGauge.getLayerGauge(depth).getViaWidth()
                           , self.routingGauge.getLayerGauge(depth).getViaWidth()
                           )

    def _getNearestHorizontalTrack ( self, bb, y, flags ):
      if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
      else:                           depth = self.horizontalDepth

      index = self.routingGauge.getLayerGauge(depth).getTrackIndex( bb.getYMin(), bb.getYMax(), y, RoutingLayerGauge.Nearest )
      return self.routingGauge.getLayerGauge(depth).getTrackPosition( bb.getYMin(), index )

    def _getNearestVerticalTrack ( self, bb, x, flags ):
      if flags & GaugeConf.DeepDepth: depth = self.verticalDeepDepth
      else:                           depth = self.verticalDepth

      index = self.routingGauge.getLayerGauge(depth).getTrackIndex( bb.getXMin(), bb.getXMax(), x, RoutingLayerGauge.Nearest )
      return self.routingGauge.getLayerGauge(depth).getTrackPosition( bb.getXMin(), index )

    def _createHorizontal ( self, source, target, y, flags ):
      if flags & GaugeConf.DeepDepth: depth = self.horizontalDeepDepth
      else:                           depth = self.horizontalDepth

      layer = self.routingGauge.getRoutingLayer(depth)

      if flags & GaugeConf.UseContactWidth: width  = source.getBoundingBox(layer.getBasicLayer()).getHeight()
      else:                                 width  = self.routingGauge.getLayerGauge(depth).getWireWidth()
      if flags & GaugeConf.ExpandWidth:     width += DbU.fromLambda( 1.0 )

      segment = Horizontal.create( source, target, layer, y, width )
      trace( 550, segment )
      return segment
  
    def _createVertical ( self, source, target, x, flags ):
      if flags & GaugeConf.DeepDepth: depth = self.verticalDeepDepth
      else:                           depth = self.verticalDepth

      layer = self.routingGauge.getRoutingLayer(depth)

      if flags & GaugeConf.UseContactWidth: width  = source.getBoundingBox(layer.getBasicLayer()).getWidth()
      else:                                 width  = self.routingGauge.getLayerGauge(depth).getWireWidth()
      if flags & GaugeConf.ExpandWidth:     width += DbU.fromLambda( 1.0 )

      segment = Vertical.create( source, target, layer, x, width )
      trace( 550, segment )
      return segment
  
    def _rpAccess ( self, rp, flags ):
      trace( 550, ',+', '\t_rpAccess() %s\n' % str(rp) )

      if self._rpToAccess.has_key(rp):
        trace( 550, '-' )
        return self._rpToAccess[rp]

      if flags & GaugeConf.DeepDepth:
        hdepth = self.horizontalDeepDepth
        vdepth = self.verticalDeepDepth
      else:
        hdepth = self.horizontalDepth
        vdepth = self.verticalDepth

      hpitch    = self.routingGauge.getLayerGauge(hdepth).getPitch()
      hoffset   = self.routingGauge.getLayerGauge(hdepth).getOffset()
      contact1  = Contact.create( rp, self.routingGauge.getContactLayer(0), 0, 0 )
      midSliceY = contact1.getY() - (contact1.getY() % self.cellGauge.getSliceHeight()) \
                                                     + self.cellGauge.getSliceHeight() / 2
      midTrackY = midSliceY - ((midSliceY - hoffset) % hpitch)
      dy        = midSliceY - contact1.getY()
  
      if flags & GaugeConf.OffsetBottom1: dy += hpitch
      if flags & GaugeConf.OffsetTop1:    dy -= hpitch
      contact1.setDy( dy )

      trace( 550, contact1 )
  
      if flags & GaugeConf.HAccess: stopDepth = hdepth
      else:                         stopDepth = vdepth
      trace( 550, '\tstopDepth:%d\n' % stopDepth )

      for depth in range(1,stopDepth):
        xoffset = 0
        if flags & GaugeConf.OffsetRight1 and depth == 1:
          xoffset = self.routingGauge.getLayerGauge(depth+1).getPitch()
        contact2 = Contact.create( rp.getNet()
                                 , self.routingGauge.getContactLayer(depth)
                                 , contact1.getX() + xoffset
                                 , contact1.getY()
                                 , self.routingGauge.getLayerGauge(depth).getViaWidth()
                                 , self.routingGauge.getLayerGauge(depth).getViaWidth()
                                 )
        trace( 550, contact2 )
        if self.routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
          segment = Horizontal.create( contact1
                                     , contact2
                                     , self.routingGauge.getRoutingLayer(depth)
                                     , contact1.getY()
                                     , self.routingGauge.getLayerGauge(depth).getWireWidth()
                                     )
          trace( 550, segment )
        else:
          segment = Vertical.create( contact1
                                   , contact2
                                   , self.routingGauge.getRoutingLayer(depth)
                                   , contact1.getX()
                                   , self.routingGauge.getLayerGauge(depth).getWireWidth()
                                   )
          trace( 550, segment )
        contact1 = contact2

      self._rpToAccess[rp] = contact1
  
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

    def _setStackPosition ( self, topContact, x, y ):
      topContact.setX( x )
      topContact.setY( y )

      count = 0
      for component in topContact.getSlaveComponents():
        segment = component
        count  += 1
      if count != 1:
        raise ErrorMessage( 1, 'GaugeConf::_setStackPosition(): There must be exactly one segment connected to %s, not %d.' % (topContact,count) )

      if isinstance(segment,Horizontal):
        segment.setY( y )
        segment.getOppositeAnchor( topContact ).setY( y )
      elif isinstance(segment,Vertical):
        segment.setX( x )
        segment.getOppositeAnchor( topContact ).setX( x )
      return


# -------------------------------------------------------------------
# Class  :  "Configuration.ChipConf".

class ChipConf ( object ):

    @staticmethod
    def _toSymbolic ( u, rounding ): 
      oneLambda = DbU.fromLambda( 1.0 )
      remainder = u % oneLambda
      if remainder:
        if rounding == chip.Superior: u = u + (oneLambda - remainder)
        else:                          u = u -              remainder
      return u


    @staticmethod
    def toSymbolic ( v, rounding ): 
      if isinstance(v,long): return ChipConf._toSymbolic( v, rounding )
      if isinstance(v,Box):
        if rounding & chip.Inwards:
          roundings = [ chip.Superior, chip.Superior, chip.Inferior, chip.Inferior ]
        else:
          roundings = [ chip.Inferior, chip.Inferior, chip.Superior, chip.Superior ]
        xMin = ChipConf._toSymbolic( v.getXMin(), roundings[0] )
        yMin = ChipConf._toSymbolic( v.getYMin(), roundings[1] )
        xMax = ChipConf._toSymbolic( v.getXMax(), roundings[2] )
        yMax = ChipConf._toSymbolic( v.getYMax(), roundings[3] )
        return Box( xMin, yMin, xMax, yMax )
      return v


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
      return Box( 0, 0, chipSize[0], chipSize[1] )


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
      return Box( 0, 0, coreSize[0], coreSize[1] )


    @staticmethod
    def _readClockTree( chipConfigDict ):
      useClockTree = False
      if chipConfigDict.has_key('chip.clockTree'):
        if chipConfigDict['chip.clockTree']:
          useClockTree = True
      return useClockTree


    def _loadIoPadGauge ( self, chipConfigDict ):
      if not chipConfigDict.has_key('pads.ioPadGauge'):
        raise ErrorMessage( 1, 'The IO pad gauge configuration paramater "pads.ioPadGauge" is missing.' )
        return
      self.gaugeConf._loadIoPadGauge( chipConfigDict['pads.ioPadGauge'] )
      return


    def _readPads ( self, chipConfigDict, keyword ):
      if not chipConfigDict.has_key(keyword): return []
      padConfList = chipConfigDict[keyword]
      if not isinstance(padConfList,list):
          raise ErrorMessage( 1, 'The "%s" entry is not a list.' )
          return []

      af      = CRL.AllianceFramework.get()
      padList = []
      for i in range(len(padConfList)):
        position     = None
        instanceName = None
        if isinstance(padConfList[i],str):
          instanceName = padConfList[i]
        elif isinstance(padConfList[i],list):
          self.padsHavePosition = True
          if isinstance(padConfList[i][0],long) and isinstance(padConfList[i][1],str):
            position     = padConfList[i][0]
            instanceName = padConfList[i][1]

        if not instanceName:
          raise ErrorMessage( 1, 'The element [%d] of list %s is neither a string nor a list "[pos,name]" (skipped).'
                                 % (i,keyword) )
          continue

        instance = self.cell.getInstance( instanceName )
        if not instance:
          raise ErrorMessage( 1, 'The pad [%d] (%s) of list %s do not exists in netlist (skipped).'
                                 % (i,instanceName,keyword) )
          continue

        if (instance.getMasterCell().getAbutmentBox().getHeight() != self.gaugeConf.getIoPadHeight()):
          raise ErrorMessage( 1, 'The pad [%d] (%s) of list %s is not an instance of a pad cell (skipped).'
                                 % (i,instanceName,keyword) )
          continue

        padList.append( [ position, instance ] )
      
      return padList


    def __init__ ( self, chipConfigDict, cell, viewer=None ):
      trace( 550, '\tONE LAMBDA = %s\n' % DbU.getValueString(DbU.fromLambda(1.0)) )

      if not isinstance(chipConfigDict,dict):
        raise ErrorMessage( 1, 'The "chip" variable is not a dictionnary.' )
      
      self.chipMode = True
      if len(chipConfigDict.keys()) == 0: self.chipMode = False

      self.validated        = True
      self.gaugeConf        = GaugeConf()
      self.cell             = cell
      self.viewer           = viewer
     # Block Corona parameters.
      self.railsNb          = getParameter('chip','chip.block.rails.count'   ).asInt()
      self.hRailWidth       = getParameter('chip','chip.block.rails.hWidth'  ).asInt()
      self.vRailWidth       = getParameter('chip','chip.block.rails.vWidth'  ).asInt()
      self.hRailSpace       = getParameter('chip','chip.block.rails.hSpacing').asInt()
      self.vRailSpace       = getParameter('chip','chip.block.rails.vSpacing').asInt()
     # Global Net names.    
      self.blockageName     = "blockagenet"
     # Global Nets.         
      self.coronaVdd        = None
      self.coronaVss        = None
      self.coronaCk         = None
      self.blockageNet      = None
                          
      if self.chipMode:
        self._loadIoPadGauge( chipConfigDict )

      self.coronas          = []
      self.cores            = []
      if self.chipMode:
        self.padsHavePosition = False
        self.southPads        = self._readPads( chipConfigDict, 'pads.south' )
        self.northPads        = self._readPads( chipConfigDict, 'pads.north' )
        self.eastPads         = self._readPads( chipConfigDict, 'pads.east'  )
        self.westPads         = self._readPads( chipConfigDict, 'pads.west'  )
        self.coreSize         = ChipConf._readCoreSize( chipConfigDict )
        self.chipSize         = ChipConf._readChipSize( chipConfigDict )
        self.useClockTree     = ChipConf._readClockTree( chipConfigDict )

        minHCorona = self.railsNb*(self.hRailWidth + self.hRailSpace) + self.hRailSpace
        minVCorona = self.railsNb*(self.vRailWidth + self.vRailSpace) + self.vRailSpace
        if minHCorona > minVCorona: self.minCorona = minHCorona*2
        else:                       self.minCorona = minVCorona*2
        
        self.checkPads()
        self.checkCorona()
        self.computeChipSize()
       #self.checkChipSize()
        self.findPowerAndClockNets()
      return


    @property
    def icorona ( self ): return self.coronas[0]

    @property
    def corona  ( self ): return self.coronas[0].getMasterCell()

    @property
    def icore ( self ): return self.cores[0]

    @property
    def core  ( self ): return self.cores[0].getMasterCell()


    def getInstanceAb ( self, instance ):
      ab = instance.getMasterCell().getAbutmentBox()
      instance.getTransformation().applyOn( ab )

      if instance.getCell() == self.cell: return ab

      if instance.getCell() != self.corona:
        raise ErrorMessage( 1, 'ChipConf.getInstanceAb(): Instance "%s" neither belong to chip or corona.' % instance.getName() )
        return ab

      self.icorona.getTransformation().applyOn( ab )
      return ab


    def getCoronaNet ( self, chipNet ):
      for plug in chipNet.getPlugs():
        if plug.getInstance() == self.icorona:
          return plug.getMasterNet()
      return None


    def toRoutingGauge ( self, uMin, uMax, layer ):
      trace( 550, ',+', '\ttoRoutingGauge() [%s %s] %s\n' \
             % (DbU.getValueString(uMin), DbU.getValueString(uMax), layer) )

      ab   = self.corona.getAbutmentBox()
      lg   = None
      mask = layer.getMask()
      for layerGauge in self.gaugeConf.routingGauge.getLayerGauges():
        if layerGauge.getLayer().getMask() == mask:
          lg = layerGauge
          trace( 550, '\tUsing layer gauge %s\n' % str(lg) )
          break

      if uMax < uMin: uMin, uMax = uMax, uMin
      if lg:
        if lg.getDirection() == RoutingLayerGauge.Horizontal:
          abMin = ab.getYMin()
          abMax = ab.getYMax()
        else:
          abMin = ab.getXMin()
          abMax = ab.getXMax()

        iTrackMin = lg.getTrackIndex( abMin, abMax, uMin, RoutingLayerGauge.Superior )
        iTrackMax = lg.getTrackIndex( abMin, abMax, uMax, RoutingLayerGauge.Inferior )
        if iTrackMax < iTrackMin: iTrackMax = iTrackMin
        
        uTrackMin = lg.getTrackPosition( abMin, iTrackMin )
        uTrackMax = lg.getTrackPosition( abMin, iTrackMax )

        axis  = (uTrackMax + uTrackMin) / 2
        width = (iTrackMax - iTrackMin) * lg.getPitch() + lg.getWireWidth()

        if self.gaugeConf.routingGauge.isSymbolic():
          oneLambda = DbU.fromLambda( 1.0 )
          if axis % oneLambda:
            axis  -= oneLambda / 2
            width -= oneLambda

        trace( 550, '\t[%i %i]\n' % (iTrackMin, iTrackMax) )
        trace( 550, '\taxis:  %sl %s\n' %  (DbU.toLambda(axis ), DbU.getValueString(axis )) )
        trace( 550, '\twidth: %sl %s\n' %  (DbU.toLambda(width), DbU.getValueString(width)) )
      else:
        axis  = (uMax + uMin) / 2
        width = (uMax - uMin)

      trace( 550, '-' )
      return axis, width


    def coronaHorizontal ( self, chipNet, layer, chipY, width, chipXMin, chipXMax ):
      trace( 550, ',+', '\tChipConf.coronaHorizontal\n' )

      coronaAb  = self.getInstanceAb( self.icorona )
      coronaNet = self.getCoronaNet ( chipNet )
      if not coronaNet: return None

      coronaY = chipY - coronaAb.getYMin()
      dxMin   = ChipConf.toSymbolic( chipXMin - coronaAb.getXMin(), chip.Superior )
      dxMax   = ChipConf.toSymbolic( chipXMax - coronaAb.getXMin(), chip.Inferior )

      trace( 550, '\t| chipNet: %s %s\n' % (chipNet, layer) )
      trace( 550, '\t| Real\n' )
      trace( 550, '\t|   axis: %10s\n' % DbU.getValueString(coronaY) )
      trace( 550, '\t|   width:%10s\n' % DbU.getValueString(width) )
      trace( 550, '\t|   dxMin:%10s (%sl)\n' \
             % (DbU.getValueString(chipXMin - coronaAb.getXMin()), DbU.toLambda(chipXMin - coronaAb.getXMin()) ) )
      trace( 550, '\t|   dxMax:%10s\n' % DbU.getValueString(chipXMax - coronaAb.getXMin()) )

      coronaY, width = self.toRoutingGauge( coronaY - width/2, coronaY + width/2, layer )

      trace( 550, '\t| On Grid\n' )
      trace( 550, '\t|   axis: %10sl or %10s\n' % (DbU.toLambda(coronaY), DbU.getValueString(coronaY)) )
      trace( 550, '\t|   width:%10sl or %10s\n' % (DbU.toLambda(width)  , DbU.getValueString(width)) )
      trace( 550, '\t|   dxMin:%10sl\n' % DbU.toLambda(dxMin) )
      trace( 550, '\t|   dxMax:%10sl\n' % DbU.toLambda(dxMax) )

      h = Horizontal.create( coronaNet, layer, coronaY, width, dxMin, dxMax )

      trace( 550, '\t| %s\n' % str(h) )
      trace( 550, '-' )
      return h


    def coronaVertical ( self, chipNet, layer, chipX, width, chipYMin, chipYMax ):
      trace( 550, ',+', '\tChipConf.coronaVertical\n' )

      coronaAb  = self.getInstanceAb( self.icorona )
      coronaNet = self.getCoronaNet( chipNet )
      if not coronaNet: return None

      coronaX = chipX - coronaAb.getXMin()
      dyMin   = ChipConf.toSymbolic( chipYMin - coronaAb.getYMin(), chip.Superior )
      dyMax   = ChipConf.toSymbolic( chipYMax - coronaAb.getYMin(), chip.Inferior )

      trace( 550, '\t| chipNet: %s %s\n' % (chipNet, layer) )
      trace( 550, '\t| Real\n' )
      trace( 550, '\t|   axis: %s\n' % DbU.getValueString(coronaX) )
      trace( 550, '\t|   width:%s\n' % DbU.getValueString(width) )

      coronaX, width = self.toRoutingGauge( coronaX - width/2, coronaX + width/2, layer )

      trace( 550, '\t| On Grid\n' )
      trace( 550, '\t|   axis: %s or %s\n' % (DbU.toLambda(coronaX), DbU.getValueString(coronaX)) )
      trace( 550, '\t|   width:%s or %s\n' % (DbU.toLambda(width)  , DbU.getValueString(width)) )

      v = Vertical.create( coronaNet, layer, coronaX, width, dyMin, dyMax )

      trace( 550, '\t| %s\n' % str(v) )
      trace( 550, '-' )
      return v


    def coronaContact ( self, chipNet, layer, chipX, chipY, width, height, flags=0 ):
      trace( 550, ',+', '\tChipConf.coronaContact\n' )

      coronaAb  = self.getInstanceAb( self.icorona )
      coronaNet = self.getCoronaNet( chipNet )
      if not coronaNet: return None

      coronaX = chipX - coronaAb.getXMin()
      coronaY = chipY - coronaAb.getYMin()

      trace( 550, '\t| chipNet: %s %s\n' % (chipNet, layer) )
      trace( 550, '\t| Real\n' )
      trace( 550, '\t|   center: %12s %12s\n' % (DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
      trace( 550, '\t|   WxH:    %12s %12s\n' % (DbU.getValueString(width  ), DbU.getValueString(height )) )

      topLayer = layer.getTop()
      if self.gaugeConf.isHorizontal(topLayer):
        coronaX, width  = self.toRoutingGauge( coronaX - width /2, coronaX + width /2, layer.getBottom() )
        coronaY, height = self.toRoutingGauge( coronaY - height/2, coronaY + height/2, topLayer )
      else:
        coronaX, width  = self.toRoutingGauge( coronaX - width /2, coronaX + width /2, topLayer )
        coronaY, height = self.toRoutingGauge( coronaY - height/2, coronaY + height/2, layer.getBottom() )

      if not (flags & chip.OnHorizontalPitch):
        trace( 550, '\tNot on horizontal routing pitch, Y on lambda only.\n' )
        coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), chip.Superior )
      if not (flags & chip.OnVerticalPitch  ):
        trace( 550, '\tNot on vertical routing pitch, X on lambda only.\n' )
        coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), chip.Superior )

      trace( 550, '\t| On Grid\n' )
      trace( 550, '\t|   X axis: %12s or %12s\n' % (DbU.toLambda(coronaX)      , DbU.getValueString(coronaX)) )
      trace( 550, '\t|   Y axis: %12s or %12s\n' % (DbU.toLambda(coronaY)      , DbU.getValueString(coronaY)) )
      trace( 550, '\t|   center: %12s %12s\n'    % (DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
      trace( 550, '\t|   WxH:    %12s %12s\n'    % (DbU.getValueString(width  ), DbU.getValueString(height )) )

      c = Contact.create( coronaNet
                        , layer
                        , coronaX
                        , coronaY
                        , width
                        , height
                        )

      trace( 550, '\t| %s\n' % str(c) )
      trace( 550, '-' )
      return c


    def coronaContactArray ( self, chipNet, layer, chipX, chipY, array, flags ):
      trace( 550, ',+', '\tChipConf.coronaContactArray\n' )

      # Should be read from the symbolic technology rules.
      viaPitch = DbU.fromLambda( 4.0 )

      coronaAb  = self.getInstanceAb( self.icorona )
      coronaNet = self.getCoronaNet( chipNet )
      if not coronaNet: return None

      trace( 550, '\t| chipNet: %s %s\n' % (chipNet, layer) )

      coronaX = chipX - coronaAb.getXMin()
      coronaY = chipY - coronaAb.getYMin()

      topLayer = layer.getTop()
      if self.gaugeConf.isHorizontal(topLayer):
        coronaX, width  = self.toRoutingGauge( coronaX, coronaX, layer.getBottom() )
        coronaY, height = self.toRoutingGauge( coronaY, coronaY, topLayer )
      else:
        coronaX, width  = self.toRoutingGauge( coronaX, coronaX, topLayer )
        coronaY, height = self.toRoutingGauge( coronaY, coronaY, layer.getBottom() )

      if not (flags & chip.OnHorizontalPitch):
        trace( 550, '\tNot on horizontal routing pitch, Y on lambda only.\n' )
        coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), chip.Superior )
      if not (flags & chip.OnVerticalPitch  ):
        trace( 550, '\tNot on vertical routing pitch, X on lambda only.\n' )
        coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), chip.Superior )

      contacts    = []
      xContact    = coronaX - viaPitch * (array[0]-1)/2
      yContact    = coronaY - viaPitch * (array[1]-1)/2
      contactSize = layer.getMinimalSize()

      trace( 550, '\txContact:%sl yContact:%sl\n' % (DbU.toLambda(xContact),DbU.toLambda(yContact)) )

      for i in range(array[0]):
        for j in range(array[1]):
          c = Contact.create( coronaNet
                            , layer
                            , xContact + i*viaPitch
                            , yContact + j*viaPitch
                            , contactSize
                            , contactSize
                            )
          trace( 550, '\t+ %s\n' % str(c) )
          contacts.append( c )
    
      trace( 550, '-' )
      return contacts


    def coronaPin ( self, chipNet, count, direction, layer, chipX, chipY, width, height ):
      trace( 550, ',+', '\tChipConf.coronaPin\n' )

      coronaAb  = self.getInstanceAb( self.icorona )
      coronaNet = self.getCoronaNet( chipNet )
      if not coronaNet: return None

      coronaX = chipX - coronaAb.getXMin()
      coronaY = chipY - coronaAb.getYMin()

      trace( 550, '\t| chipNet: %s (%d) %s\n' % (chipNet, count, layer) )
      trace( 550, '\t| Real\n' )
      trace( 550, '\t|   center: %s %s\n' % (DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
      trace( 550, '\t|   WxH:    %s %s\n' % (DbU.getValueString(width  ), DbU.getValueString(height )) )

      topLayer = layer.getTop()
      if self.gaugeConf.isHorizontal(topLayer):
        coronaX, width  = self.toRoutingGauge( coronaX - width /2, coronaX + width /2, layer.getBottom() )
        coronaY, height = self.toRoutingGauge( coronaY - height/2, coronaY + height/2, topLayer )
      else:
        coronaX, width  = self.toRoutingGauge( coronaX - width /2, coronaX + width /2, topLayer )
        coronaY, height = self.toRoutingGauge( coronaY - height/2, coronaY + height/2, layer.getBottom() )

      if direction == Pin.Direction.NORTH or direction == Pin.Direction.SOUTH: 
        trace( 550, '\tEast/West not on horizontal routing pitch, Y on lambda only.\n' )
        coronaY = self.toSymbolic( chipY - coronaAb.getYMin(), chip.Superior )
      if direction == Pin.Direction.EAST or direction == Pin.Direction.WEST: 
        trace( 550, '\tNorth/South not on vertical routing pitch, X on lambda only.\n' )
        coronaX = self.toSymbolic( chipX - coronaAb.getXMin(), chip.Superior )

      trace( 550, '\t| On Grid\n' )
      trace( 550, '\t|   X axis: %s or %s\n' % (DbU.toLambda(coronaY)      , DbU.getValueString(coronaY)) )
      trace( 550, '\t|   Y axis: %s or %s\n' % (DbU.toLambda(coronaX)      , DbU.getValueString(coronaX)) )
      trace( 550, '\t|   center: %s %s\n'    % (DbU.getValueString(coronaX), DbU.getValueString(coronaY)) )
      trace( 550, '\t|   WxH:    %s %s\n'    % (DbU.getValueString(width  ), DbU.getValueString(height )) )

      c = Pin.create( coronaNet
                    , '%s.%d' % (coronaNet.getName(),count)
                    , direction
                    , Pin.PlacementStatus.FIXED
                    , layer
                    , coronaX
                    , coronaY
                    , width
                    , height
                    )

      trace( 550, '\t| %s\n' % str(c) )
      trace( 550, '-' )
      return c


    def checkPads ( self ):

      def contains ( padList, pad ):
        for padPair in padList:
          if padPair[1] == pad: return True
        return False
        
      af       = CRL.AllianceFramework.get()
      cellPads = []
      for instance in self.cell.getInstances():
        if contains(self.southPads,instance): continue
        if contains(self.northPads,instance): continue
        if contains(self.eastPads ,instance): continue
        if contains(self.westPads ,instance): continue
        if (instance.getMasterCell().getAbutmentBox().getHeight() == self.gaugeConf.getIoPadHeight()):
          raise ErrorMessage( 1, 'Pad "%s" is not on any side (N/S/E/W).' % instance.getName() )
          self.validated = False
        else:
          self.coronas.append( instance )

      if len(self.coronas) > 1:
        message = [ 'Chip "%s" have more than one corona:' % self.cell.getName() ]
        for i in range(len(self.coronas)):
          message.append( '%4d: %s' % (i,self.coronas[i].getName()) )
        raise ErrorMessage( 1, message )
        self.validated = False

      if len(self.coronas) < 1:
        raise ErrorMessage( 1, 'Chip "%s" doesn\'t seems to have a corona.' % self.cell.getName() )
        self.validated = False
      else:
        for instance in self.corona.getInstances(): self.cores.append( instance )

        if len(self.cores) > 1:
          message = [ 'Chip "%s" have more than one core:' % self.cell.getName() ]
          for i in range(len(self.cores)):
            message.append( '%4d: %s' % (i,self.cores[i].getName()) )
          raise ErrorMessage( 1, message )
          self.validated = False

          if len(self.cores) < 1:
            raise ErrorMessage( 1, 'Chip "%s" doesn\'t seems to have a core.' % self.cell.getName() )
            self.validated = False

      return


    def findPowerAndClockNets ( self ):
      if self.icore:
        for plug in self.icore.getPlugs():
          masterNet = plug.getMasterNet()
          netType   = masterNet.getType()
          if     netType != Net.Type.POWER  \
             and netType != Net.Type.GROUND \
             and netType != Net.Type.CLOCK:
            continue

          net = plug.getNet()
          if not net:
            net = self.corona.getNet( masterNet.getName() )
            if not net:
              raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Missing global net "%s" at corona level.'
                                     % masterNet.getName() )
              self._validated = False
              continue

          if netType == Net.Type.GROUND:
            if self.coronaVss and self.coronaVss != net:
              raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Multiple ground nets "%s" and "%s" at corona level.'
                                     % (self.coronaVss.getName(), net.getName()) )
              self._validated = False
              continue
            else:
              self.coronaVss = net

          if netType == Net.Type.POWER:
            if self.coronaVdd and self.coronaVdd != net:
              raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Multiple power nets "%s" and "%s" at corona level.'
                                     % (self.coronaVdd.getName(), net.getName()) )
              self._validated = False
              continue
            else:
              self.coronaVdd = net

          if netType == Net.Type.CLOCK:
            if self.coronaCk and self.coronaCk != net:
              raise ErrorMessage( 1, 'ChipConf.findPowerAndClockNets(): Multiple clock nets "%s" and "%s" at corona level.'
                                     % (self.coronaCk.getName(), net.getName()) )
              self._validated = False
              continue
            else:
              self.coronaCk = net

      for net in self.corona.getNets():
        if net.getType() == Net.Type.BLOCKAGE:
          self.blockageNet  = net
          self.blockageName = net.getName()

      if not self.blockageNet:
        self.blockageNet = Net.create( self.corona, self.blockageName )
        self.blockageNet.setType( Net.Type.BLOCKAGE )

      return

    def checkChipSize ( self ):
     #if self._coreSize.isEmpty(): return
     #
     #minWidth  = self._coreSize.getWidth () + self._minCorona + 2*self._padHeight
     #minHeight = self._coreSize.getHeight() + self._minCorona + 2*self._padHeight
     #
     #if self._chipSize.getWidth() < minWidth:
     #  raise ErrorMessage( 1, 'Core is too wide to fit into the chip. Needs: %d, but has %d' \
     #                         % ( DbU.toLambda(minWidth), DbU.toLambda(self._chipSize.getWidth()) ) )
     #  self._validated = False
     #
     #if self._chipSize.getHeight() < minHeight:
     #  raise ErrorMessage( 1, 'Core is too wide to fit into the chip. Needs: %d, but has %d' \
     #                         % ( DbU.toLambda(minHeight), DbU.toLambda(self._chipSize.getHeight()) ) )
     #  self._validated = False
      return

    def checkCorona ( self ):
      trace( 550, ',+', 'Configuration.checkCorona()\n' )
      netPads = {}
      for plug in self.icorona.getPlugs():
        padNet    = plug.getNet()
        coronaNet = plug.getMasterNet()
        if not padNet and coronaNet.isGlobal():
          padNet = self.cell.getNet( coronaNet.getName() )

        if padNet:
          if not netPads.has_key(padNet):
            trace( 550, '\t%20s <-> %-20s\n' % (padNet.getName(),coronaNet.getName()) )
            netPads[ padNet ] = coronaNet
          else:
            raise ErrorMessage( 1, 'ChipConf.checkCorona(): Corona nets "%s" and "%s" connected to the same pad net "%s".' \
                                    % (coronaNet.getName(),netPads[padNet].getName(),padNet.getName()) )
            self._validated = False
          
      trace( 550, '-' )
      return


    def computeChipSize ( self ):

      def getSideLength ( pads ):
        sideLength = self.gaugeConf.getIoPadHeight() * 2 
        for pad in pads: sideLength += pad.getMasterCell().getAbutmentBox().getWidth()
        return sideLength

        
      if not self.chipSize.isEmpty(): return
      
      southPadsLength = getSideLength( self.southPads )
      northPadsLength = getSideLength( self.northPads )
      eastPadsLength  = getSideLength( self.eastPads  )
      westPadsLength  = getSideLength( self.westPads  )
      
      horizontalPads = max( len(self.southPads), len(self.northPads) )
      verticalPads   = max( len(self.eastPads ), len(self.westPads ) )
      self.chipSize = Box( 0
                         , 0
                         , max( southPadsLength, northPadsLength )
                         , max(  westPadsLength,  eastPadsLength )
                         )
      return


    def setupCorona ( self, gapX1, gapY1, gapX2, gapY2 ):
      ab = self.cell.getAbutmentBox()
      ab.inflate  ( -gapX1, -gapY1, -gapX2, -gapY2 )
      ab.inflate  ( - self.getIoPadHeight() )
      ab.translate( - self.getIoPadHeight(), - self.getIoPadHeight())
      ab = self.toSymbolic( ab, chip.Inwards )

      self. corona.setAbutmentBox( Box( 0, 0, ab.getWidth(), ab.getHeight() ) )
      self.icorona.setTransformation(
        Transformation( self.toSymbolic( self.getIoPadHeight() + ab.getXMin(), chip.Superior )
                      , self.toSymbolic( self.getIoPadHeight() + ab.getYMin(), chip.Superior )
                      , Transformation.Orientation.ID ) )
      self.icorona.setPlacementStatus( Instance.PlacementStatus.FIXED )
      return


    def setupCore ( self, gapX1, gapY1, gapX2, gapY2 ):
      ab = self.getInstanceAb( self.icorona )
      if ab.isEmpty():
        raise ErrorMessage( 1, 'ChipConf.setupCore(): Attempt to setup core *before* corona.' )
        return

      ab.inflate( -gapX1, -gapY1, -gapX2, -gapY2 )
      ab = self.toSymbolic( ab, chip.Inwards )

      trace( 550, '\tChipConf.setupCore(): Abutment box:%s\n' % str(ab) )
      
      self.core.setAbutmentBox( Box( 0, 0, ab.getWidth(), ab.getHeight() ) )
      self.icore.setTransformation(
        Transformation( ChipConf.toSymbolic(ab.getXMin(),chip.Inferior) - self.icorona.getTransformation().getTx()
                      , ChipConf.toSymbolic(ab.getYMin(),chip.Inferior) - self.icorona.getTransformation().getTy()
                      , Transformation.Orientation.ID ) )
      self.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )
      return


    @property
    def cellGauge        ( self ): return self.gaugeConf.cellGauge

    @property
    def routingGauge     ( self ): return self.gaugeConf.routingGauge

    @property
    def verticalDepth    ( self ): return self.gaugeConf.verticalDepth

    @property
    def horizontalDepth  ( self ): return self.gaugeConf.horizontalDepth

    def getSliceHeight   ( self ): return self.gaugeConf.getSliceHeight()
    def getSliceStep     ( self ): return self.gaugeConf.getSliceStep()
    def getIoPadHeight   ( self ): return self.gaugeConf.getIoPadHeight()
    def getIoPadStep     ( self ): return self.gaugeConf.getIoPadStep()
    def getIoPadPitch    ( self ): return self.gaugeConf.getIoPadPitch()
    def getIoPadGauge    ( self ): return self.gaugeConf.getIoPadGauge()
    def getHRoutingGauge ( self ): return self.gaugeConf.getHRoutingGauge()
    def getVRoutingGauge ( self ): return self.gaugeConf.getVRoutingGauge()

    def rpByOccurrence ( self, occurrence, net ):
      return self.gaugeConf._rpByOccurrence ( occurrence, net )

    def rpByPlugName ( self, instance, plugName, net ):
      return self.gaugeConf._rpByPlugName ( instance, plugName, net )

    def rpAccess ( self, rp, flags=0 ):
      return self.gaugeConf._rpAccess( rp, flags )

    def rpAccessByOccurrence ( self, occurrence, net, flags=0 ):
      return self.gaugeConf._rpAccessByOccurrence ( occurrence, net, flags )

    def rpAccessByPlugName ( self, instance, plugName, net, flags=0 ):
      return self.gaugeConf._rpAccessByPlugName( instance, plugName, net, flags )

    def createContact ( self, net, x, y, flags=0 ):
      return self.gaugeConf._createContact( net, x, y, flags )

    def createHorizontal ( self, source, target, y, flags=0 ):
      return self.gaugeConf._createHorizontal( source, target, y, flags )

    def createVertical ( self, source, target, x, flags=0 ):
      return self.gaugeConf._createVertical( source, target, x, flags )

    def getNearestHorizontalTrack ( self, bb, y, flags ):
      return self.gaugeConf._getNearestHorizontalTrack ( bb, y, flags )

    def getNearestVerticalTrack ( self, bb, x, flags ):
      return self.gaugeConf._getNearestVerticalTrack( bb, x, flags )

    def setStackPosition ( self, topContact, x, y ):
      self.gaugeConf._setStackPosition( topContact, x, y )


def loadConfiguration ( cell, viewer=None ):
    sys.path.append( os.getcwd() )

    confFile = cell.getName()+'_ioring'
    if not os.path.isfile(confFile+'.py'):
      raise ErrorMessage( 1, 'ChipPlugin configuration file <%s.py> is missing.' % confFile )
    
    confModule = __import__( confFile, globals(), locals(), confFile )

    if not confModule.__dict__.has_key('chip'):
      ErrorMessage( 1, 'Module <%s> do not provides the chip variable, skipped.' \
                       % confFile )

    return ChipConf( confModule.__dict__['chip'], cell, viewer )


# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/sky130.py"            |
# +-----------------------------------------------------------------+

"""
Core2Chip configuration for the Sky130 harness.
"""

import sys
import re
from   Hurricane  import DbU, DataBase, UpdateSession, Breakpoint, \
                         Transformation , Box, Instance , Net,     \
                         Contact
import Viewer     
from   CRL             import Catalog
from   CRL             import AllianceFramework, DefImport
from   helpers         import trace, l, u, n
from   helpers.io      import ErrorMessage, WarningMessage
from   helpers.overlay import CfgCache, UpdateSession
import plugins.alpha.chip
from   plugins.alpha.block.configuration import IoPin, GaugeConf
from   plugins.alpha.core2chip.core2chip import CoreToChip as BaseCoreToChip, \
                                                IoNet, IoPad


# --------------------------------------------------------------------
# Class  :  "sky130.CoreToChip"

class CoreToChip ( BaseCoreToChip ):
    """
    Provide harness-specific part for SkyWater 130 (works in real mode).
    Emulate the behavior of I/O pads.
    """
    rePadType = re.compile(r'(?P<type>.+)_(?P<index>[\d]+)$')

    def __init__ ( self, core ):
        self.ioPadNames = { 'bidir' :'IOHarnessInOut'
                          , 'analog':'IOHarnessAnalog'
                          , 'vdd'   :'IOHarnessVdd'
                          , 'vss'   :'IOHarnessVss'
                          , 'iovdd' :'IOHarnessIOVdd'
                          , 'iovss' :'IOHarnessIOVss'
                          }
        BaseCoreToChip.__init__ ( self, core )
        self.conf.useHarness = True
        self.ringNetNames = { 'vssa2' : None
                            , 'vdda2' : None
                            , 'vssa1' : None
                            , 'vdda1' : None
                            , 'vssd2' : None
                            , 'vccd2' : None
                            , 'vssd1' : None
                            , 'vccd1' : None
                            }
        self.ioPadInfos   = [ BaseCoreToChip.IoPadInfo( IoPad.BIDIR
                                                      , self.ioPadNames['bidir']
                                                      , 'pad', ['io_in', 'io_out', 'io_oeb'] )
                            , BaseCoreToChip.IoPadInfo( IoPad.ANALOG
                                                      , self.ioPadNames['analog']
                                                      , 'pad', ['pad', 'padres'] )
                            ]
        self._getPadLib()
        return

    def _getPadLib ( self ):
        return None

    def getNetType ( self, netName ):
        if netName.startswith('vss') or netName.startswith('vee'): return Net.Type.GROUND
        if netName.startswith('vdd') or netName.startswith('vcc'): return Net.Type.POWER
        return Net.Type.LOGICAL

    def isGlobal ( self, netName ):
        if netName in self.ringNetNames: return True
        return False

    def getCell ( self, masterCellName ):
        raise NotImplementedError( 'coreToChip.getCell(): Harness does not provides I/O pad cells.' )

    def _buildCoreGroundPads ( self, ioPadConf ):
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName )
        if not coronaNet:
            coronaNet = Net.create( self.corona, ioPadConf.coreSupplyNetName )
            coronaNet.setExternal( True )
            coronaNet.setGlobal  ( True )
            coronaNet.setType    ( Net.Type.GROUND )
            self.icore.getPlug( coreNet ).setNet( coronaNet  )
        if not chipNet:
            chipNet = Net.create( self.chip, ioPadConf.coreSupplyNetName )
            chipNet.setExternal( True )
            chipNet.setType    ( Net.Type.GROUND )
        coronaPlug = self.icorona.getPlug( coronaNet )
        if not coronaPlug.getNet():
            coronaPlug.setNet( chipNet  )
        self.ringNetNames['vss'] = chipNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vss_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['vss']) ) )
        self._connect( ioPadConf.pads[0], chipNet, 'vss' )
        self.groundPadCount += 1
        self.chipPads       += ioPadConf.pads

    def _buildIoGroundPads ( self, ioPadConf ):
        padNet = self.chip.getNet( ioPadConf.padSupplyNetName  )
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.GROUND )
        self.ringNetNames['iovss'] = padNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_iovss_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['iovss']) ) )
        self._connect( ioPadConf.pads[0], padNet , 'iovss' )
        self.groundPadCount += 1
        self.chipPads       += ioPadConf.pads

    def _buildCorePowerPads ( self, ioPadConf ):
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName )
        if not coronaNet:
            coronaNet = Net.create( self.corona, ioPadConf.coreSupplyNetName )
            coronaNet.setExternal( True )
            coronaNet.setGlobal  ( True )
            coronaNet.setType    ( Net.Type.POWER )
            self.icore.getPlug( coreNet ).setNet( coronaNet  )
        if not chipNet:
            chipNet = Net.create( self.chip, ioPadConf.coreSupplyNetName )
            chipNet.setExternal( True )
            chipNet.setType    ( Net.Type.POWER )
            self.icorona.getPlug( coronaNet ).setNet( chipNet  )
            self.ringNetNames['vdd'] = chipNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vdd_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['vdd']) ) )
        self._connect( ioPadConf.pads[0], chipNet, 'vdd' )
        self.powerPadCount += 1
        self.chipPads      += ioPadConf.pads

    def _buildIoPowerPads ( self, ioPadConf ):
        padNet = self.chip  .getNet( ioPadConf.padSupplyNetName  )
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.POWER )
            self.ringNetNames['iovdd'] = padNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_iovdd_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['iovdd']) ) )
        self._connect( ioPadConf.pads[0], padNet , 'iovdd' )
        self.powerPadCount += 1
        self.chipPads      += ioPadConf.pads

    def _buildClockPads ( self, ioPadConf ):
        """For "Sky130" there is no specialized clock I/O pad. So do nothing."""
        pass

    def _connectClocks ( self ):
        """For "Sky130" there is no pad internal clock ring. So do nothing."""
        pass

    def _loadHarness ( self ):
        """
        Load the DEF file containing the reference harness layout.
        Remove the supplied internal power grid and slightly shrink
        the central P&R area so that I/O pins are fully outside of it.
        """
        self.harness = DefImport.load( self.conf.cfg.harness.path )
        innerAb  = self.harness.getAbutmentBox()
        wholeBb  = self.harness.getBoundingBox()
        filterBb = Box( wholeBb.getXMin(), innerAb.getYMin()
                      , wholeBb.getXMax(), innerAb.getYMax() )
        components = []
        for component in self.harness.getComponentsUnder( filterBb ):
            if component.getNet().isSupply() and filterBb.contains( component.getBoundingBox() ):
                components.append( component )
        for instance in self.harness.getInstancesUnder( filterBb ):
            components.append( instance )
        with UpdateSession():
            for component in components:
                component.destroy()
        components = []
        filterBb = Box( innerAb.getXMin(), wholeBb.getYMin()
                      , innerAb.getXMax(), wholeBb.getYMax() )
        for component in self.harness.getComponentsUnder( filterBb ):
            if component.getNet().isSupply() and filterBb.contains( component.getBoundingBox() ):
                components.append( component )
        for instance in self.harness.getInstancesUnder( filterBb ):
            components.append( instance )
        with UpdateSession():
            for component in components:
                component.destroy()
            areaXMin = innerAb.getXMin()
            areaYMin = innerAb.getYMin()
            areaXMax = innerAb.getXMax()
            areaYMax = innerAb.getYMax()
            for net in self.harness.getNets():
                if net.isSupply(): continue
                for component in net.getComponents():
                    trace( 550, '\t| {}\n'.format(component) )
                    bb   = component.getBoundingBox()
                    side = None
                    if bb.getXMin() < innerAb.getXMin():
                        areaXMin = max( areaXMin, bb.getXMax() )
                        side     = IoPin.WEST
                    if bb.getXMax() > innerAb.getXMax():
                        areaXMax = min( areaXMax, bb.getXMin() )
                        side     = IoPin.EAST
                    if bb.getYMin() < innerAb.getYMin():
                        areaYMin = max( areaYMin, bb.getYMax() )
                        side     = IoPin.SOUTH
                    if bb.getYMax() > innerAb.getYMax():
                        areaYMax = min( areaYMax, bb.getYMin() )
                        side     = IoPin.NORTH
                    trace( 550, '\tside: {} {}\n'.format(side,type(component)) )
                    if side and isinstance(component,Contact):
                        trace( 550, '\tAdded on {} side: {}\n'.format(side,component) )
                        self.conf.chipConf.addHarnessPin( component, side )
            area = Box( areaXMin, areaYMin, areaXMax, areaYMax )
            area.inflate( - self.conf.hRoutingGauge.getPitch()*2
                        , - self.conf.vRoutingGauge.getPitch()*2 )
            xmodulo = area.getWidth () % self.conf.sliceStep
            ymodulo = area.getHeight() % self.conf.sliceHeight
            area.inflate( 0, 0, -xmodulo, -ymodulo )
            self.harness.setAbutmentBox( area )
        return self.harness

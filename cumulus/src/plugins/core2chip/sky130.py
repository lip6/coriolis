
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
from   ...Hurricane          import DbU, DataBase, UpdateSession, Breakpoint, \
                                    Transformation , Box, Instance , Net,     \
                                    Contact
from   ...CRL                import Catalog, AllianceFramework, DefImport
from   ...helpers            import trace, l, u, n
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import CfgCache, UpdateSession
from   ..block.configuration import IoPin, GaugeConf
from   .core2chip            import CoreToChip as BaseCoreToChip, IoNet, IoPad


# -------------------------------------------------------------------
# Class : "MatchHarnessIos".

class MatchHarnessIos ( object ):
    """
    In case the connections of the core to the I/O pins are not specified
    (empty ioPadsSpecs list), try to guess the connection by names.

    .. note:: Currently, only matching with the (io_in,io_out,io_oeb),
              power and clock are supported.
    """

    def __init__ ( self, coreToChip ):
        self.coreToChip = coreToChip
        self.digitalIos = []
        for i in range(38):
            self.digitalIos.append( [None, None, None] )
        self.clock      = None
        self.digitalVdd = None
        self.digitalVss = None

    @property
    def conf ( self ): return self.coreToChip.conf

    def matchCore ( self ):
        print( '  o  Using implicit Caravel harness connections.' )
        reIoIn = re.compile( r'^(?P<name>\w+)\((?P<index>\d+)\)$' )
        for net in self.conf.core.getNets():
            if not net.isExternal(): continue
            if net.isClock():
                self.clock = net
                continue
            if net.isPower():
                self.digitalVdd = net
                continue
            if net.isGround():
                self.digitalVss = net
                continue
            m = reIoIn.match( net.getName() )
            if m:
                index = int( m.group('index') )
                if index >= 38:
                    continue
                if m.group('name') == 'io_in':
                    self.digitalIos[ index ][0] = net
                elif m.group('name') == 'io_out':
                    self.digitalIos[ index ][1] = net
                elif m.group('name') == 'io_oeb':
                    self.digitalIos[ index ][2] = net
                else:
                    print( WarningMessage('Unable to match "{}" into harness net.'.format( net.getName() )))
            else:
                print( WarningMessage('Unable to match "{}" into harness net.'.format( net.getName() )))
        if self.digitalVdd:
            self.conf.chipConf.addIoPad( (None, None, 'power_0', 'vccd1', self.digitalVdd.getName()), 0 )
        else:
            print( WarningMessage('Missing digital power net in "{}".'.format(self.conf.core.getName())) )
        if self.digitalVss:
            self.conf.chipConf.addIoPad( (None, None, 'ground_0', 'vssd1', self.digitalVss.getName()), 0 )
        else:
            print( WarningMessage('Missing digital ground net in "{}".'.format(self.conf.core.getName())) )
        if self.clock:
            self.conf.chipConf.addIoPad( (None, None, None, 'user_clock2', self.clock.getName()), 0 )
        else:
            print( WarningMessage('Missing digital clock net in "{}".'.format(self.conf.core.getName())) )
        for i in range(38):
            ioNets = self.digitalIos[ i ]
            if ioNets[0] and ioNets[1] and ioNets[2]:
                self.conf.chipConf.addIoPad( ( None, None, None, ioNets[0].getName()
                                                               , ioNets[0].getName()
                                                               , ioNets[1].getName()
                                                               , ioNets[2].getName() ), 0 )
            elif ioNets[0] and not ioNets[1] and not ioNets[2]:
                self.conf.chipConf.addIoPad( ( None, None, None, ioNets[0].getName()
                                                               , ioNets[0].getName() ), 0 )
            elif not ioNets[0] and not ioNets[1] and ioNets[2]:
                self.conf.chipConf.addIoPad( ( None, None, None, ioNets[2].getName()
                                                               , ioNets[2].getName() ), 0 )
            elif not ioNets[0] and not ioNets[1] and not ioNets[2]:
                continue
            else:
                print( WarningMessage( [ 'Incomplete digital connexion to I/O pad "{}".'.format(index)
                                         , '* In :{}'.format( ioNets[0] )
                                         , '* Out:{}'.format( ioNets[1] )
                                         , '* OEb:{}'.format( ioNets[2] ) ]))


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
        if self.conf.chipConf.ioPadsCount() == 0:
            MatchHarnessIos( self ).matchCore()
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
        trace( 550, '\tsky130.CoreToChip._buildGroundPowerPads()\n' )
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.padSupplyNetName )
        if not coronaNet:
            coronaNet = Net.create( self.corona, ioPadConf.coreSupplyNetName )
            coronaNet.setExternal ( True )
            coronaNet.setGlobal   ( True )
            coronaNet.setDirection( Net.Direction.IN )
            coronaNet.setType     ( Net.Type.GROUND )
            self.icore.getPlug( coreNet ).setNet( coronaNet  )
            trace( 550, '\tCreated corona ground net: {}\n'.format(coronaNet) )
        if not chipNet:
            raise ErrorMessage( 1, 'Harness do not provide ground net ""{}' \
                                   .format( ioPadConf.padSupplyNetName ))
        coronaPlug = self.icorona.getPlug( coronaNet )
        if not coronaPlug.getNet():
            coronaPlug.setNet( chipNet  )
        self.groundPadCount += 1

    def _buildCorePowerPads ( self, ioPadConf ):
        trace( 550, '\tsky130.CoreToChip._buildCorePowerPads()\n' )
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.padSupplyNetName )
        if not coronaNet:
            coronaNet = Net.create( self.corona, ioPadConf.coreSupplyNetName )
            coronaNet.setExternal ( True )
            coronaNet.setGlobal   ( True )
            coronaNet.setDirection( Net.Direction.IN )
            coronaNet.setType     ( Net.Type.POWER )
            self.icore.getPlug( coreNet ).setNet( coronaNet  )
            trace( 550, '\tCreated corona power net: {}\n'.format(coronaNet) )
        if not chipNet:
            raise ErrorMessage( 1, 'Harness do not provide power net ""{}' \
                                   .format( ioPadConf.padSupplyNetName ))
            self.icorona.getPlug( coronaNet ).setNet( chipNet  )
        coronaPlug = self.icorona.getPlug( coronaNet )
        if not coronaPlug.getNet():
            coronaPlug.setNet( chipNet  )
        self.powerPadCount += 1

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

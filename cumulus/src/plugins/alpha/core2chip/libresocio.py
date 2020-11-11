# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) SU 2020-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/libresocio.py"        |
# +-----------------------------------------------------------------+

"""
Core2Chip configuration for the FlexLib I/O pad library.

It provides the same features as the symbolic "niolib", but is
using the real cells provided by LibreSOC.py. For now, apart from
cell library change, the only difference is that the cell names are
in upppercase. We can use them because, in real mode, we are less
dependent from the Alliance design flow.
"""

from   __future__ import print_function
import sys
import re
from   Hurricane  import DbU, DataBase, UpdateSession, Breakpoint, \
                         Transformation , Instance , Net
import Viewer     
from   CRL        import Catalog
from   CRL        import AllianceFramework
from   helpers.io import ErrorMessage, WarningMessage
from   plugins.alpha.core2chip.core2chip import CoreToChip as BaseCoreToChip, \
                                                IoNet, IoPad


class CoreToChip ( BaseCoreToChip ):
    """
    Provide pad-specific part for LibreSOCIO I/O pads (works in real mode).
    """
    rePadType = re.compile(r'(?P<type>.+)_(?P<index>[\d]+)$')

    def __init__ ( self, core ):
        BaseCoreToChip.__init__ ( self, core )
        self.ringNetNames = { 'iovdd' : None
                            , 'iovss' : None
                            , 'vdd'   : None
                            , 'vss'   : None
                            }
        self.ioPadInfos   = [ BaseCoreToChip.IoPadInfo( IoPad.BIDIR, 'GPIO', 'pad', ['s', 'd', 'de'] )
                            ]
        self._getPadLib()
        return

    def _getPadLib ( self ):
        self.padLib = AllianceFramework.get().getLibrary( "LibreSOCIO" )
        if not self.padLib:
            message = [ 'CoreToChip.libresocio._getPadLib(): Unable to find Alliance "LibreSOCIO" library' ]
            raise ErrorMessage( 1, message )

    def getNetType ( self, netName ):
        if netName.startswith('vss') or netName.startswith('iovss'): return Net.Type.GROUND
        if netName.startswith('vdd') or netName.startswith('iovdd'): return Net.Type.POWER
        return Net.Type.LOGICAL

    def isGlobal ( self, netName ):
        if netName in self.ringNetNames: return True
        return False

    def getCell ( self, masterCellName ):
       #cell = self.padLib.getCell( masterCellName )
        cell = AllianceFramework.get().getCell( masterCellName, Catalog.State.Views )
        if not cell:
            raise ErrorMessage( 1, 'libresocio.getCell(): I/O pad library "%s" does not contain cell named "%s"' \
                                   % (self.padLib.getName(),masterCellName) )
        return cell

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
            self.icorona.getPlug( coronaNet ).setNet( chipNet  )
            self.ringNetNames['vss'] = chipNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vss_{}'.format(ioPadConf.index)
                                              , self.getCell('VSS') ) )
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
                                              , self.getCell('IOVSS') ) )
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
                                              , self.getCell('VDD') ) )
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
                                              , self.getCell('IOVDD') ) )
        self._connect( ioPadConf.pads[0], padNet , 'iovdd' )
        self.powerPadCount += 1
        self.chipPads      += ioPadConf.pads

    def _buildClockPads ( self, ioPadConf ):
        """For "LibreSOCIO" there is no specialized clock I/O pad. So do nothing."""
        pass

    def _connectClocks ( self ):
        """For "LibreSOCIO" there is no pad internal clock ring. So do nothing."""
        pass

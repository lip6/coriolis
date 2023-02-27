
# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/cmos.py"              |
# +-----------------------------------------------------------------+

"""
Core2Chip configuration for the SxLib/PxLib Alliance historical
standard cell library.
"""

import sys
import re
from   ...Hurricane  import DbU, DataBase, UpdateSession, Breakpoint, \
                            Transformation, Instance, Net
from   ...CRL        import Catalog, AllianceFramework
from   ...helpers    import trace
from   ...helpers.io import ErrorMessage
from   .core2chip    import CoreToChip as BaseCoreToChip
from   .core2chip    import IoNet, IoPad


class CoreToChip ( BaseCoreToChip ):

    def __init__ ( self, core ):
        BaseCoreToChip.__init__ ( self, core )
        self.ringNetNames = { 'vsse' : None
                            , 'vssi' : None
                            , 'vdde' : None
                            , 'vddi' : None
                            , 'ck'   : None  # Go through the pads from pck_px.
                            }
        self.ioPadInfos   = [ BaseCoreToChip.IoPadInfo( IoPad.IN     , 'pi_px'  , 'pad', ['t',] )
                            , BaseCoreToChip.IoPadInfo( IoPad.OUT    , 'po_px'  , 'pad', ['i',] )
                            , BaseCoreToChip.IoPadInfo( IoPad.TRI_OUT, 'pot_px' , 'pad', ['i', 'b'] )
                            , BaseCoreToChip.IoPadInfo( IoPad.BIDIR  , 'piot_px', 'pad', ['i', 't', 'b'] )
                            ]
        self._getPadLib()
        return

    def _getPadLib ( self ):
        self.padLib = AllianceFramework.get().getLibrary( "pxlib" )
        if not self.padLib:
            message = [ 'CoreToChip.cmos._getPadLib(): Unable to find Alliance "pxlib" library' ]
            raise ErrorMessage( 1, message )

    def getNetType ( self, netName ):
        if netName.startswith('vss'): return Net.Type.GROUND
        if netName.startswith('vdd'): return Net.Type.POWER
        if netName in ('cki', 'ck'):  return Net.Type.CLOCK
        return Net.Type.LOGICAL

    def isGlobal ( self, netName ):
        if netName in self.ringNetNames: return True
        return False

    def getCell ( self, masterCellName ):
       #cell = self.padLib.getCell( masterCellName )
        cell = AllianceFramework.get().getCell( masterCellName, Catalog.State.Views )
        if not cell:
            raise ErrorMessage( 1, 'cmos.getCell(): I/O pad library "%s" does not contain cell named "%s"' \
                                   % (self.padLib.getName(),masterCellName) )
        return cell

    def _buildAllGroundPads ( self, ioPadConf ):
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName )
        padNet    = self.chip  .getNet( ioPadConf.padSupplyNetName  )
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
            self.ringNetNames['vssi'] = chipNet
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.GROUND )
            self.ringNetNames['vsse'] = padNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vssick_{}'.format(ioPadConf.index)
                                              , self.getCell('pvssick_px') ) )
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vsseck_{}'.format(ioPadConf.index)
                                              , self.getCell('pvsseck_px') ) )
        self._connect( ioPadConf.pads[0], chipNet, 'vssi' )
        self._connect( ioPadConf.pads[1], padNet , 'vsse' )
        self.groundPadCount += 1
        self.chipPads       += ioPadConf.pads

    def _buildAllPowerPads ( self, ioPadConf ):
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName )
        padNet    = self.chip  .getNet( ioPadConf.padSupplyNetName  )
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
            self.ringNetNames['vddi'] = chipNet
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.POWER )
            self.ringNetNames['vdde'] = padNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vddick_{}'.format(ioPadConf.index)
                                              , self.getCell('pvddick_px') ) )
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_vddeck_{}'.format(ioPadConf.index)
                                              , self.getCell('pvddeck_px') ) )
        self._connect( ioPadConf.pads[0], chipNet, 'vddi' )
        self._connect( ioPadConf.pads[1], padNet , 'vdde' )
        self.powerPadCount += 1
        self.chipPads      += ioPadConf.pads

    def _buildClockPads ( self, ioPadConf ):
        coreNet   = self.core  .getNet( ioPadConf.coreSupplyNetName )
        coronaNet = self.corona.getNet( ioPadConf.coreSupplyNetName )
        chipNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName+'_core' )
        ringNet   = self.chip  .getNet( ioPadConf.coreSupplyNetName+'_ring' )
        padNet    = self.chip  .getNet( ioPadConf.padSupplyNetName  )
        if not coronaNet:
            coronaNet = Net.create( self.corona, ioPadConf.coreSupplyNetName )
            coronaNet.setExternal( True )
            coronaNet.setType( Net.Type.CLOCK )
            self.icore.getPlug( coreNet ).setNet( coronaNet  )
        if not chipNet:
            chipNet = Net.create( self.chip, ioPadConf.coreSupplyNetName+'_core' )
            chipNet.setType    ( Net.Type.CLOCK )
            self.icorona.getPlug( coronaNet ).setNet( chipNet  )
            self.coreClock = chipNet
        if not ringNet:
            ringNet = Net.create( self.chip, ioPadConf.coreSupplyNetName+'_ring' )
            ringNet.setType( Net.Type.CLOCK )
            self.ringNetNames['ck'] = ringNet
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.CLOCK )
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_ck_{}'.format(ioPadConf.index)
                                              , self.getCell('pck_px') ) )
        self._connect( ioPadConf.pads[0], padNet, 'pad' )
        self.clockPadCount += 1
        self.chipPads      += ioPadConf.pads

    def _connectClocks ( self ):
        p = re.compile( r'pv[ds]{2}[ei]ck_px' )
        for pad in self.chipPads: 
            if p.match( pad.getMasterCell().getName() ): 
                self._connect( pad, self.coreClock, 'cko' )

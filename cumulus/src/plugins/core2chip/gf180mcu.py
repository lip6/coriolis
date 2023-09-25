
# -*- coding: utf-8 -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2023, All Rights Reserved
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
Core2Chip configuration for the Global Foudries 180nm I/O pad library (GF180MCU).
"""

import sys
import re
from   ...Hurricane       import DbU, DataBase, UpdateSession, Breakpoint, \
                                 Transformation , Instance , Net
from   ...CRL             import Catalog, AllianceFramework
from   ...helpers         import trace
from   ...helpers.io      import ErrorMessage, WarningMessage
from   ...helpers.overlay import CfgCache
from   .core2chip         import CoreToChip as BaseCoreToChip, IoNet, IoPad


class CoreToChip ( BaseCoreToChip ):
    """
    Provide pad-specific part for GF180MCU I/O pads (works in real mode).
    """
    rePadType = re.compile(r'(?P<type>.+)_(?P<index>[\d]+)$')

    def __init__ ( self, core ):
        with CfgCache() as cfg:
            cfg.chip.useAbstractPads = False
            self.ioPadNames = { 'in'       :'gf180mcu_fd_io__in_s'
                              , 'bidir'    :'gf180mcu_fd_io__bi_t'
                              , 'analog'   :'gf180mcu_fd_io__asig_5p0'
                              , 'vdd'      :'gf180mcu_fd_io__dvdd'
                              , 'vss'      :'gf180mcu_fd_io__dvss'
                              , 'corner'   :'gf180mcu_fd_io__cor'
                              , 'spacer1'  :'gf180mcu_fd_io__fill1'
                              , 'spacer5'  :'gf180mcu_fd_io__fill5'
                              , 'spacer10' :'gf180mcu_fd_io__fill10'
                              }
        BaseCoreToChip.__init__ ( self, core )
        self.ringNetNames = { 'DVDD' : None
                            , 'DVSS' : None
                           #, 'VDD'  : None
                           #, 'VSS'  : None
                            }
        self.ioPadInfos   = [ BaseCoreToChip.IoPadInfo( IoPad.IN
                                                      , self.ioPadNames['in']
                                                      , 'PAD', ['Y'], [ ( 'PU'   , False )
                                                                      , ( 'PD'   , False )
                                                                      ] )
                            , BaseCoreToChip.IoPadInfo( IoPad.BIDIR
                                                      , self.ioPadNames['bidir']
                                                      , 'PAD', ['A', 'Y', 'OE'], [ ( 'SL'   , True  ) 
                                                                                 , ( 'CS'   , True  )
                                                                                 , ( 'PU'   , False )
                                                                                 , ( 'PD'   , False )
                                                                                 , ( 'PDRV0', False )
                                                                                 , ( 'PDRV1', False )
                                                                                 , ( 'IE'   , True  )
                                                                                 ] )
                            , BaseCoreToChip.IoPadInfo( IoPad.ANALOG
                                                      , self.ioPadNames['analog']
                                                      , 'ASIG5V', ['asig5v'] )
                            , BaseCoreToChip.IoPadInfo( IoPad.CORNER
                                                      , self.ioPadNames['corner']
                                                      , None, [] )
                            , BaseCoreToChip.IoPadInfo( IoPad.FILLER
                                                      , self.ioPadNames['spacer1']
                                                      , None, [] )
                            , BaseCoreToChip.IoPadInfo( IoPad.FILLER
                                                      , self.ioPadNames['spacer5']
                                                      , None, [] )
                            , BaseCoreToChip.IoPadInfo( IoPad.FILLER
                                                      , self.ioPadNames['spacer10']
                                                      , None, [] )
                            ]
        self.cornerCount = 0
        self.spacerCount = 0
        self.padSpacers  = []
        self._getPadLib()
        return

    def _getPadLib ( self ):
        """
        Check that the I/O pad library is present and pre-load the spacer cells.
        """
        def _cmpPad ( pad ):
            """Used to sort I/O pads by decreasing width."""
            return pad.getAbutmentBox().getWidth()

        self.padLib = AllianceFramework.get().getLibrary( "iolib" )
        if not self.padLib:
            message = [ 'CoreToChip.libresocio._getPadLib(): Unable to find Alliance "iolib" library' ]
            raise ErrorMessage( 1, message )
        for ioPadInfo in self.ioPadInfos:
            if ioPadInfo.flags & IoPad.FILLER: 
                spacerCell = self.padLib.getCell( ioPadInfo.name )
                if spacerCell: self.padSpacers.append( spacerCell )
                else:
                    raise ErrorMessage( 1, 'CoreToChip.gf180mcu._getPadLib(): Missing spacer cell "{}"'.format(spacerName) )
        self.padSpacers = sorted( self.padSpacers, key=_cmpPad, reverse=True )

    def getNetType ( self, netName ):
        if netName.lower().startswith('vss') or netName.lower().startswith('dvss'): return Net.Type.GROUND
        if netName.lower().startswith('vdd') or netName.lower().startswith('dvdd'): return Net.Type.POWER
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
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.GROUND )
        coronaPlug = self.icorona.getPlug( coronaNet )
        if not coronaPlug.getNet():
            coronaPlug.setNet( chipNet  )
        self.ringNetNames['DVSS' ] = chipNet
       #self.ringNetNames['VSS'  ] = padNet
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_iovss_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['vss']) ) )
       #self._connect( ioPadConf.pads[0], chipNet, 'VSS' )
        self._connect( ioPadConf.pads[0], padNet , 'DVSS'  )
        self.groundPadCount += 1
        self.chipPads       += ioPadConf.pads

    def _buildAllPowerPads ( self, ioPadConf ):
        trace( 550, ',+', '\tgf180mcu.CoreToChip()\n' )
        trace( 550, '\tcoreSupplyNetName="{}"\n'.format( ioPadConf.coreSupplyNetName ))
        trace( 550, '\tpadSupplyNetName ="{}"\n'.format( ioPadConf.padSupplyNetName ))
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
        trace( 550, '\tchipNet ="{}"\n'.format( chipNet ))
        if not padNet:
            padNet = Net.create( self.chip, ioPadConf.padSupplyNetName )
            padNet.setExternal( True )
            padNet.setType    ( Net.Type.POWER )
        self.ringNetNames['DVDD'] = chipNet
       #self.ringNetNames['VDD'] = padNet
        trace( 550, '\tpadNet ="{}"\n'.format( padNet ))
        ioPadConf.pads.append( Instance.create( self.chip
                                              , 'p_iovdd_{}'.format(ioPadConf.index)
                                              , self.getCell(self.ioPadNames['vdd']) ) )
       #self._connect( ioPadConf.pads[0], chipNet, 'VDD' )
        self._connect( ioPadConf.pads[0], padNet , 'DVDD'  )
        self.powerPadCount += 1
        self.chipPads      += ioPadConf.pads
        trace( 550, '-,' )

    def _buildClockPads ( self, ioPadConf ):
        """For "GF180MCU" there is no specialized clock I/O pad. So do nothing."""
        pass

    def _connectClocks ( self ):
        """For "GF180MCU" there is no pad internal clock ring. So do nothing."""
        pass

    def hasCornerCell ( self ):
        """Overload of CoreToChip, YES we have dedicated corner cells."""
        return True

    def hasFillerCells ( self ):
        """Overload of CoreToChip, YES we have dedicated filler cells."""
        return True

    def getCornerCell ( self ):
        """Return the model of corner cell."""
        return self.getCell( self.ioPadNames['corner'] )

    def createSpacer ( self, gapWidth ):
        """Return a new instance of spacer cell."""
        spacerCell = None
        for candidate in self.padSpacers:
            if gapWidth >= candidate.getAbutmentBox().getWidth():
                spacerCell = candidate
                break
        if not spacerCell:
            return None
        spacer = Instance.create( self.chip
                                , 'pad_spacer_{}'.format( self.spacerCount )
                                , spacerCell )
        self.spacerCount += 1
        #self._connect( spacer, self.ringNetNames['vddring'], 'vddring' )
        self._connect( spacer, self.ringNetNames['DVDD'], 'DVDD' )
        #self._connect( spacer, self.ringNetNames['gndring'], 'gndring' )
        self._connect( spacer, self.ringNetNames['DVSS'], 'DVSS' )
        return spacer

    def createCorner ( self, instanceName=None ):
        """Return a new instance of corner cell."""
        if instanceName is None:
            instanceName = 'pad_corner_{}'.format( self.cornerCount )
        corner = Instance.create( self.chip, instanceName, self.getCornerCell() )
        self.cornerCount += 1
        self._connect( corner, self.ringNetNames['DVDD'], 'DVDD' )
        #self._connect( corner, self.ringNetNames['vddcore'], 'vddcore' )
        self._connect( corner, self.ringNetNames['DVSS'], 'DVSS' )
        #self._connect( corner, self.ringNetNames['gndcore'], 'gndcore' )
        return corner

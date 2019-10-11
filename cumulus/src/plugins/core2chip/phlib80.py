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
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/core2chip/phlib80.py"           |
# +-----------------------------------------------------------------+


import re
from   Hurricane                    import DbU
from   Hurricane                    import DataBase
from   Hurricane                    import UpdateSession
from   Hurricane                    import Breakpoint
from   Hurricane                    import Transformation
from   Hurricane                    import Instance
from   Hurricane                    import Net
import Viewer                       
from   CRL                          import Catalog
from   CRL                          import AllianceFramework
from   helpers.io                   import ErrorMessage
from   plugins.core2chip.CoreToChip import IoPad
from   plugins.core2chip.CoreToChip import CoreToChip


class phlib80 ( CoreToChip ):

    def __init__ ( self, core ):
        CoreToChip.__init__ ( self, core )
        self.ringNetNames = [ 'vss', 'vss', ('cki', 'ck') ]
        self.ioPadInfos   = { IoPad.IN      : CoreToChip.IoPadInfo( 'pi_sp'  , 'pad', ['t',] )
                            , IoPad.OUT     : CoreToChip.IoPadInfo( 'po_sp'  , 'pad', ['i',] )
                           #, IoPad.TRI_OUT : CoreToChip.IoPadInfo( 'pot_sp' , 'pad', ['i', 'b' ] )
                           #, IoPad.BIDIR   : CoreToChip.IoPadInfo( 'piot_sp', 'pad', ['i', 't', 'b' ] )
                            }
        self._getPadLib()
        return

    def _getPadLib ( self ):
        self.padLib = AllianceFramework.get().getLibrary( 'phlib80' )

        if not self.padLib:
          message = [ 'CoreToChip.phlib80._getPadLib(): Unable to find Alliance "phlib80" library' ]
          raise ErrorMessage( 1, message )

        return 

    def getNetType ( self, netName ):
        if netName.startswith('vss'): return Net.Type.GROUND
        if netName.startswith('vdd'): return Net.Type.POWER
        if netName in ('cko', 'ck'):  return Net.Type.CLOCK
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

    def _buildGroundPads ( self, ioNet ):
        ioNet.buildNets()
        vss = self.chip.getNet( 'vss' )
        vss.setExternal( True )
        vss.setGlobal  ( True )
        vss.setType    ( Net.Type.GROUND )
        vss.merge( ioNet.chipIntNet )
        ioNet.chipIntNet = vss

        pads = []
        pads.append( Instance.create( self.chip
                                    , 'p_' + ioNet.padInstanceName + 'ck_%d' % self.groundPadCount
                                    , self.getCell('pvssck2_sp') ) )

        CoreToChip._connect( pads[0], ioNet.chipIntNet, 'vss' )

        for pad in pads: self._connectRing( pad )
        self.groundPadCount += 1
        self.chipPads       += pads
        return

    def _buildPowerPads ( self, ioNet ):
        ioNet.buildNets()
        vdd = self.chip.getNet( 'vdd' )
        vdd.setExternal( True )
        vdd.setGlobal  ( True )
        vdd.setType    ( Net.Type.POWER )
        vdd.merge( ioNet.chipIntNet )
        ioNet.chipIntNet = vdd

        pads = [ ]
        pads.append( Instance.create( self.chip
                                    , 'p_' + ioNet.padInstanceName + 'ck_%d' % self.powerPadCount
                                    , self.getCell('pvddck2_sp') ) )

        CoreToChip._connect( pads[0], ioNet.chipIntNet, 'vdd' )

        for pad in pads: self._connectRing( pad )
        self.powerPadCount += 1
        self.chipPads      += pads
        return

    def _buildClockPads ( self, ioNet ):
        ioNet.buildNets()
        pads = [ ]
        pads.append( Instance.create( self.chip
                                    , 'p_' + ioNet.padInstanceName + '_%d' % self.clockPadCount
                                    , self.getCell('pck_sp') ) )

        CoreToChip._connect( pads[0], ioNet.chipExtNet, 'pad' )

        for pad in pads: self._connectRing( pad )
        self.clockPadCount += 1
        self.chipPads      += pads

        p = re.compile( r'pv[ds]{2}ck2_sp' )
        for pad in self.chipPads: 
          if p.match( pad.getMasterCell().getName() ): 
            CoreToChip._connect( pad, ioNet.chipIntNet, 'cko' )
        return

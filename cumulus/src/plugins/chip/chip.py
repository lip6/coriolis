
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/chip.py"                   |
# +-----------------------------------------------------------------+


import sys
import traceback
import os.path
import optparse
import math
import cProfile
import pstats
from   ...                import Cfg
from   ...Hurricane       import DataBase, DbU ,Point, Transformation, Box,      \
                                 Path, Occurrence, UpdateSession, Breakpoint,    \
                                 Net, RoutingPad, Contact, Horizontal, Vertical, \
                                 Instance, HyperNet, Query
from   ...CRL             import Catalog, RoutingLayerGauge
from   ...helpers         import trace
from   ...helpers.io      import ErrorMessage, WarningMessage
from   ...helpers.overlay import UpdateSession
from   ...                import Etesian, Anabatic, Katana, Unicorn
from   ..block.block      import Block
from   .                  import pads        as chipPads
from   .                  import power       as chipPower
from   .                  import powerplane  as chipPowerplane
from   .                  import corona      as chipCorona
from   ..harness          import pads        as harnessPads


# --------------------------------------------------------------------
# Class  :  "chip.Chip"

class Chip ( Block ):

    def __init__ ( self, conf ):
        super(Chip,self).__init__( conf )
  
    def validate ( self ):
        self.conf.validated = True
        coreAb = self.conf.core.getAbutmentBox()
        if (not coreAb.isEmpty()):
            if     coreAb.getWidth () <= self.conf.coreAb.getWidth() \
               and coreAb.getHeight() <= self.conf.coreAb.getHeight():
                self.conf.coreSize = (coreAb.getWidth(), coreAb.getHeight())
            else:
                raise ErrorMessage( 1, [ 'Core "{}" already have an abutment box, bigger than the requested one:' \
                                         .format(self.conf.core.getName())
                                      , "       Cell abutment box: {}".format(coreAb)
                                      , "    Maximum abutment box: {}".format(self.conf.coreAb) ] )
                self.conf.validated = False
        return self.conf.validated
  
    def doChipNetlist ( self ):
        """
        Build the netlist at chip-level around the ``core`` block. Needs the ``conf.coreToChipClass``
        configuration parameter to be set up. Otherwise assume the netlist is already a chip.
        """
        if not hasattr(self.conf,'coreToChipClass'):
            print( WarningMessage( 'Chip.doChipNetlist(): No "conf.coreToChipClass" defined, assume we already have a chip-level netlist.' ) )
            return
        self.conf.coreToChip = self.conf.coreToChipClass( self.conf )
        self.conf.coreToChip.buildChip()
  
    def doChipFloorplan ( self ):
        self.padsCorona = None
        minHCorona = self.conf.minHCorona
        minVCorona = self.conf.minVCorona
        self.conf.chipValidate()
        if not self.conf.useHarness:
            print( '     - Chip has {} north pads.'.format(len(self.conf.chipConf.northPads)) )
            print( '     - Chip has {} south pads.'.format(len(self.conf.chipConf.southPads)) )
            print( '     - Chip has {} east pads.' .format(len(self.conf.chipConf.eastPads )) )
            print( '     - Chip has {} west pads.' .format(len(self.conf.chipConf.westPads )) )
            self.conf.computeCoronaBorder()
            if not self.conf.validated:
                raise ErrorMessage( 1, 'chip.doChipFloorplan(): Chip is not valid, aborting.' )
            self.conf.chip.setAbutmentBox( self.conf.chipAb )
            trace( 550, '\tSet chip ab:{}\n'.format(self.conf.chip.getAbutmentBox()) )
            trace( 550, '\tUsing core ab:{}\n'.format(self.conf.core.getAbutmentBox()) )
            self.padsCorona = chipPads.Corona( self )
            self.conf.validated =  self.padsCorona.validate()
            if not self.conf.validated:
                return False
            self.padsCorona.doLayout()
            self.validate()
            minHCorona = self.conf.minHCorona
            minVCorona = self.conf.minVCorona
            trace( 550, '\tminHCorona={}\n'.format(DbU.getValueString( minHCorona )))
            trace( 550, '\tminVCorona={}\n'.format(DbU.getValueString( minVCorona )))
        else:
            self.padsCorona = harnessPads.Corona( self )
            self.padsCorona.doLayout()
        innerBb = Box( self.conf.coreAb )
        innerBb.inflate( minHCorona, minVCorona )
        coronaAb = self.conf.corona.getAbutmentBox()
        if innerBb.getWidth() > coronaAb.getWidth():
            raise ErrorMessage( 1, 'Core is too wide to fit into the corona, needs {} but only has {}.' \
                                   .format( DbU.getValueString(innerBb .getWidth())
                                          , DbU.getValueString(coronaAb.getWidth()) ) )
        if innerBb.getHeight() > coronaAb.getHeight():
            raise ErrorMessage( 1, 'Core is too tall to fit into the corona, needs {} but only has {}.' \
                                   .format( DbU.getValueString(innerBb .getHeight())
                                          , DbU.getValueString(coronaAb.getHeight()) ) )
        with UpdateSession():
            self.conf.core.setAbutmentBox( self.conf.coreAb )
            x = (coronaAb.getWidth () - self.conf.coreAb.getWidth ()) // 2
            y = (coronaAb.getHeight() - self.conf.coreAb.getHeight()) // 2
            trace( 550, '\tCore X, {} '.format(DbU.getValueString(x)) )
            x = x - (x % self.conf.sliceHeight)
            trace( 550, ' adjusted on {}, {}\n'.format( DbU.getValueString(self.conf.sliceHeight)
                                                      , DbU.getValueString(x)) )
            y = y - (y % self.conf.sliceHeight)
            self.conf.icore.setTransformation ( Transformation(x,y,Transformation.Orientation.ID) )
            self.conf.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )
        self.conf.refresh()

    def doConnectCore ( self ):
        if self.padsCorona:
            self.padsCorona.doPowerLayout()
        if self.conf.routingGauge.hasPowerSupply():
            power = chipPowerplane.Builder( self.conf )
            power.connectPower()
           #power.connectHTrees( self.hTrees )
            power.doLayout()
        else:
            if self.conf.useHarness:
                return
            power = chipPower.Builder( self.conf )
            power.connectPower()
            power.connectClocks()
            power.doLayout()
            self.conf.refresh()
            corona = chipCorona.Builder( power )
            corona.connectPads( self.padsCorona )
            corona.connectCore()
            corona.doLayout()
        self.conf.refresh()
  
    def doPnR ( self ):
        status = super(Chip,self).doPnR()
        self.conf.refresh( self.conf.chip )
        return self.conf.validated and status
  
    def save ( self, flags=0 ):
        if not self.conf.validated:
            raise ErrorMessage( 1, 'chip.save(): Chip is not valid, aborting.' )
        views = Catalog.State.Logical
        if self.conf.routingGauge.isSymbolic():
            views = views | Catalog.State.Physical
        super(Chip,self).save( flags )


# This file is part of the Coriolis Software.
# Copyright (c) SU 2014-2020, All Rights Reserved
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


from   __future__ import print_function
from   __future__ import absolute_import
import sys
import traceback
import os.path
import optparse
import math
import cProfile
import pstats
import Cfg
import Hurricane
from   Hurricane  import DataBase
from   Hurricane  import DbU
from   Hurricane  import Point
from   Hurricane  import Transformation
from   Hurricane  import Box
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import UpdateSession
from   Hurricane  import Breakpoint
from   Hurricane  import Net
from   Hurricane  import RoutingPad
from   Hurricane  import Contact
from   Hurricane  import Horizontal
from   Hurricane  import Vertical
from   Hurricane  import Instance
from   Hurricane  import HyperNet
from   Hurricane  import Query
import Viewer
import CRL
from   CRL        import RoutingLayerGauge
import helpers
from   helpers         import trace
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.overlay import UpdateSession
import Etesian
import Anabatic
import Katana
import Unicorn
import plugins
import plugins.rsave
from   plugins.alpha.block.block import Block
import plugins.alpha.chip.pads
import plugins.alpha.chip.power
import plugins.alpha.chip.corona


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
  
    def doCoronaFloorplan ( self ):
        if not self.conf.validated:
            raise ErrorMessage( 1, 'chip.doCoronaFloorplan(): Chip is not valid, aborting.' )
            return
        minHCorona = self.conf.minHCorona
        minVCorona = self.conf.minVCorona
        innerBb    = Box( self.conf.coreAb )
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
            x = (coronaAb.getWidth () - self.conf.coreAb.getWidth ()) / 2
            y = (coronaAb.getHeight() - self.conf.coreAb.getHeight()) / 2
            trace( 550, '\tCore X, {} '.format(DbU.getValueString(x)) )
            x = x - (x % self.conf.sliceHeight)
            trace( 550, ' adjusted on {}, {}\n'.format( DbU.getValueString(self.conf.sliceHeight)
                                                      , DbU.getValueString(x)) )
            y = y - (y % self.conf.sliceHeight)
            self.conf.icore.setTransformation ( Transformation(x,y,Transformation.Orientation.ID) )
            self.conf.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )

    def doConnectCore ( self ):
        power = plugins.alpha.chip.power.Builder( self.conf )
        power.connectPower()
        power.connectClocks()
        power.doLayout()
        self.conf.refresh()
        corona = plugins.alpha.chip.corona.Builder( power )
        corona.connectPads( self.padsCorona )
        corona.connectCore()
        corona.doLayout()
        self.conf.refresh()
  
    def doPnR ( self ):
        self.conf.computeCoronaBorder()
        self.conf.chipValidate()
        if not self.conf.validated:
            raise ErrorMessage( 1, 'chip.doChipPnR(): Chip is not valid, aborting.' )
        self.conf.chip.setAbutmentBox( self.conf.chipAb )
        trace( 550, '\tSet chip ab:{}\n'.format(self.conf.chip.getAbutmentBox()) )
        trace( 550, '\tUsing core ab:{}\n'.format(self.conf.core.getAbutmentBox()) )
        self.padsCorona = plugins.alpha.chip.pads.Corona( self.conf )
        self.conf.validated =  self.padsCorona.validate()
        if not self.conf.validated:
            return False
        self.padsCorona.doLayout()
        self.validate()
        self.doCoronaFloorplan()
        self.conf.refresh()
        super(Chip,self).doPnR()
        self.conf.refresh( self.conf.chip )
        return self.conf.validated
  
    def save ( self, flags=0 ):
        if not self.conf.validated:
            raise ErrorMessage( 1, 'chip.save(): Chip is not valid, aborting.' )
        super(Chip,self).save( flags )
        self.conf.corona.setName( self.conf.corona.getName()+'_r' )
        self.conf.chip  .setName( self.conf.chip  .getName()+'_r' )
        af = CRL.AllianceFramework.get()
        af.saveCell( self.conf.corona, CRL.Catalog.State.Views|flags )
        af.saveCell( self.conf.chip  , CRL.Catalog.State.Views|flags )

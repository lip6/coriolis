#!/usr/bin/env python
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
# |  Python      :       "./plugins/ChipPlugin.py"                  |
# +-----------------------------------------------------------------+

try:
  import sys
  import traceback
  import os.path
  import optparse
  import math
  import Cfg
  import Hurricane
  from   Hurricane import DataBase
  from   Hurricane import DbU
  from   Hurricane import Point
  from   Hurricane import Transformation
  from   Hurricane import Box
  from   Hurricane import Path
  from   Hurricane import Occurrence
  from   Hurricane import UpdateSession
  from   Hurricane import Breakpoint
  from   Hurricane import Net
  from   Hurricane import RoutingPad
  from   Hurricane import Contact
  from   Hurricane import Horizontal
  from   Hurricane import Vertical
  from   Hurricane import Instance
  from   Hurricane import HyperNet
  from   Hurricane import Query
  import Viewer
  import CRL
  from   CRL import RoutingLayerGauge
  from   helpers   import ErrorMessage
  from   helpers   import WarningMessage
  import Nimbus
  import Metis
  import Mauka
  import Katabatic
  import Kite
  import Unicorn
  import plugins
  import chip.Configuration
  import chip.BlockPower
  import chip.BlockCorona
  import chip.PadsCorona
except ImportError, e:
  module = str(e).split()[-1]

  print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
  print '        Please check the integrity of the <coriolis> package.'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


# --------------------------------------------------------------------
# Plugin working part.


class PlaceCore ( chip.Configuration.Wrapper ):

  def __init__ ( self, confWrapper ):
    chip.Configuration.Wrapper.__init__( self, confWrapper.conf )
    self.validated = False
    return


  def validate ( self ):
    self.validated = True
    if len(self.cores) < 1: self.validated = False

    coreAb = self.cores[0].getMasterCell().getAbutmentBox()
    if (not coreAb.isEmpty()):
      if     coreAb.getWidth () < self.coreSize.getWidth() \
         and coreAb.getHeight() < self.coreSize.getHeight():
        self.coreSize = coreAb
      else:
        print ErrorMessage( 1, [ 'Core %s already have an abutment box, bigger than the requested one:'
                                 % self.cores[0].getName()
                               , "       Cell abutment box: %s" % str(coreAb)
                               , "    Maximum abutment box: %s" % str(self.coreSize) ] )
        self.validated = False

    return self.validated


  def doFloorplan ( self ):
    if not self.validated: return
    UpdateSession.open()
    self.cores[0].getMasterCell().setAbutmentBox( self.coreSize )
    x = (self.chipSize.getWidth () - self.coreSize.getWidth ()) / 2
    y = (self.chipSize.getHeight() - self.coreSize.getHeight()) / 2
    self.cores[0].setTransformation ( Transformation(x,y,Transformation.Orientation.ID) )
    self.cores[0].setPlacementStatus( Instance.PlacementStatus.FIXED )
    UpdateSession.close()
    return


  def doPlacement ( self ):
    if not self.validated: return

    checkUnplaced = plugins.CheckUnplaced( self.cores[0].getMasterCell(), plugins.NoFlags )
    if not checkUnplaced.check(): return
    
    mauka  = Mauka.MaukaEngine.create( self.cores[0].getMasterCell() )
    mauka.run()
    mauka.destroy()
    return




# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'plugins.chip'
                         , 'Chip Placement'
                         , 'Place a Complete Chip (pads && core)'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  try:
    cell, editor = plugins.kwParseMain( **kw )

    conf = chip.Configuration.loadConfiguration( cell )
    if not conf.isValid(): return

    padsCorona = chip.PadsCorona.Corona( conf )
    if not padsCorona.validate(): return

    padsCorona.doLayout()
    placeCore = PlaceCore( conf )
    placeCore.validate()
    placeCore.doFloorplan()
    placeCore.doPlacement()
    corePower = chip.BlockPower.Block( Path(conf.cores[0]), conf.vddi, conf.vssi )
    corePower.queryPower()
    corePower.doLayout()
    coreCorona = chip.BlockCorona.Corona( corePower )
    coreCorona.connectPads( padsCorona )
    coreCorona.connectBlock()
    coreCorona.doLayout()
    editor.fit()

  except ErrorMessage, e:
    print e; errorCode = e.code
    editor.fit()
  except Exception, e:
    print '\n\n', e; errorCode = 1
    traceback.print_tb(sys.exc_info()[2])
      
  return 0

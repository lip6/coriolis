#!/usr/bin/env python
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
# |  Python      :       "./plugins/ChipRoute.py"                   |
# +-----------------------------------------------------------------+



try:
  import sys
  import traceback
  import Viewer
  import helpers
  from   helpers.io import ErrorMessage
  from   helpers.io import WarningMessage
  import plugins
  import chip.Chip
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'placeAndRoute.stepByStep'
   #kw['beforeAction'] = 'placeAndRoute.placeChip'

    plugins.kwAddMenu    ( 'placeAndRoute', 'P&&R', **kw )
    plugins.kwUnicornHook( 'placeAndRoute.placeRouteChip'
                         , 'PLace && Route Chip'
                         , 'Place & route a Complete Chip (pads && core)'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  rvalue = True
  try:
    helpers.staticInitialization( quiet=True )
   #helpers.setTraceLevel( 550 )

    cell, editor = plugins.kwParseMain( **kw )
    conf = chip.Configuration.loadConfiguration( cell, editor )
    if not conf.validated: return False

    prChip = chip.Chip.PlaceRoute( conf )
    prChip.validate()
    prChip.doChipPlacement()
    prChip.doChipRouting()
    prChip.save()
    return prChip.validated

  except Exception, e:
    helpers.io.catch( e )
    rvalue = False

  sys.stdout.flush()
  sys.stderr.flush()
      
  return rvalue

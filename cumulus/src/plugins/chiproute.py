
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chiproute.py"                   |
# +-----------------------------------------------------------------+


import sys
import traceback
import Viewer
import helpers
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
import plugins
import plugins.chip.chip
from   Hurricane  import Breakpoint


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


def scriptMain ( **kw ):
    rvalue = True
    try:
        Breakpoint.setStopLevel( 99 )
        helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        conf = plugins.chip.configuration.loadConfiguration( cell, editor )
        conf.chipValidate()
        if not conf.validated: return False
        prChip = plugins.chip.chip.PlaceRoute( conf )
        prChip.validate()
        prChip.doChipPlacement()
        prChip.doChipRouting()
        prChip.save()
        return prChip.validated
    except Exception as e:
        helpers.io.catch( e )
        rvalue = False
    sys.stdout.flush()
    sys.stderr.flush()
    return rvalue

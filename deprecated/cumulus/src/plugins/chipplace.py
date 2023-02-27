
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
# |  Python      :       "./plugins/chipplace.py"                   |
# +-----------------------------------------------------------------+


import sys
import traceback
import helpers
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
import plugins
import plugins.chip.chip


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'placeAndRoute.stepByStep'
   #kw['beforeAction'] = 'placeAndRoute.clockTree'
    plugins.kwAddMenu    ( 'placeAndRoute', 'P&&R', **kw )
    plugins.kwUnicornHook( 'placeAndRoute.placeChip'
                         , 'PLace Chip'
                         , 'Place a Complete Chip (pads && core)'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    rvalue = True
    try:
       #helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        conf = plugins.chip.configuration.loadConfiguration( cell, editor )
        conf.chipValidate()
        if not conf.validated: return False
        placeChip = plugins.chip.chip.PlaceRoute( conf )
        placeChip.doChipPlacement()
        return placeChip.validated
    except Exception as e:
        helpers.io.catch( e )
        rvalue = False
    sys.stdout.flush()
    sys.stderr.flush()
    return rvalue

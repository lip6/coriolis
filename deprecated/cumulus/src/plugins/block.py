
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block.py"                       |
# +-----------------------------------------------------------------+

"""
This script hook the Block plugin inside GCT/Unicorn.
"""

import sys
import traceback
import helpers
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.overlay import UpdateSession
from   helpers         import trace
import plugins
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Net
from   Hurricane import Cell
from   Hurricane import Instance
from   Hurricane import Transformation
from   plugins.alpha.block.block import Block


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    kw['beforeAction'] = 'misc.alpha'
    plugins.kwUnicornHook( 'misc.alpha.block'
                         , 'Block P&&R'
                         , 'Perform block-level placement'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    """The mandatory function that Coriolis CGT/Unicorn will look for."""
    rvalue = True
    try:
        helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        block = Block.create( cell )
        if editor: block.setEditor( editor )
        rvalue = block.build()
    except Exception as e:
        helpers.io.catch( e )
        rvalue = False
    sys.stdout.flush()
    sys.stderr.flush()
    return rvalue

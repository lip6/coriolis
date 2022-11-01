
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
# |  Python      :       "./plugins/aboutwindow.py"                 |
# +-----------------------------------------------------------------+


import sys
import traceback
import Viewer
import helpers
from   helpers.io   import ErrorMessage
from   helpers.io   import WarningMessage
import plugins


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'file.About'
                         , 'About Coriolis'
                         , 'Informations about the Coriolis Software'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    try:
        Viewer.AboutWindow.show()
    except Exception as e:
        helpers.io.catch( e )
    sys.stdout.flush()
    sys.stderr.flush()
    return True


# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/node45/freepdk45/__init__.py"       |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg
from   coriolis.helpers    import truncPath, tagConfModules
from   coriolis.helpers.io import vprint
vprint( 1, '  o  Loading "node45.freepdk45" technology.' )
vprint( 2, '     - "%s".' % truncPath(__file__) )

from coriolis.Hurricane import DataBase
from coriolis.CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

DataBase.create()
System.get()

from .freepdk45 import misc
from .freepdk45 import technology
from .freepdk45 import display
from .freepdk45 import analog
from .freepdk45 import alliance
from .freepdk45 import etesian
from .freepdk45 import kite
from .freepdk45 import plugins
from .freepdk45 import stratus1
from .freepdk45 import devices

Cfg.Configuration.popDefaultPriority()

tagConfModules()

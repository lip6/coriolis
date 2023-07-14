
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
# |  Python      :       "./etc/node600/phenitec/__init__.py"       |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg
from   coriolis.helpers    import truncPath, tagConfModules
from   coriolis.helpers.io import vprint
vprint( 1, '  o  Loading "node600/phenitec" technology.' )
vprint( 2, '     - "%s".' % truncPath(__file__) )

from   coriolis.Hurricane import DataBase
from   coriolis.CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

if not DataBase.getDB(): DataBase.create()
System.get()

from .phenitec import misc
from .phenitec import technology
from .phenitec import display
from .phenitec import analog
from .phenitec import alliance
from .phenitec import etesian
from .phenitec import kite
from .phenitec import plugins
from .phenitec import stratus1
from .phenitec import devices

Cfg.Configuration.popDefaultPriority()

tagConfModules()

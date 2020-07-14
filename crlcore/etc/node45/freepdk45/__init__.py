
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2019, All Rights Reserved
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


import Cfg
import helpers.io
helpers.io.vprint( 1, '  o  Loading "node45.freepdk45" technology.' )
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

from   Hurricane import DataBase
from   CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

DataBase.create()
System.get()

import node45.freepdk45.misc
import node45.freepdk45.technology
import node45.freepdk45.display
import node45.freepdk45.analog
import node45.freepdk45.alliance
import node45.freepdk45.etesian
import node45.freepdk45.kite
import node45.freepdk45.plugins
import node45.freepdk45.stratus1
import node45.freepdk45.devices

Cfg.Configuration.popDefaultPriority()

helpers.tagConfModules()


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
# |  Python      :       "./etc/node600/phenitec/__init__.py"       |
# +-----------------------------------------------------------------+


import Cfg
import helpers.io
helpers.io.vprint( 1, '  o  Loading "node600/phenitec" technology.' )
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

from   Hurricane import DataBase
from   CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

if not DataBase.getDB(): DataBase.create()
System.get()

import node600.phenitec.misc
import node600.phenitec.technology
import node600.phenitec.display
import node600.phenitec.analog
import node600.phenitec.alliance
import node600.phenitec.etesian
import node600.phenitec.kite
import node600.phenitec.plugins
import node600.phenitec.stratus1
import node600.phenitec.devices

Cfg.Configuration.popDefaultPriority()

helpers.tagConfModules()

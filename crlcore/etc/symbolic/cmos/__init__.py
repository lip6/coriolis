
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
# |  Python      :       "./etc/symbolic/cmos/__init__.py"          |
# +-----------------------------------------------------------------+


import Cfg
import helpers
import helpers.io
helpers.io.vprint( 1, '  o  Loading "symbolic.cmos" technology.' )
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

from   Hurricane  import DataBase
from   CRL        import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

if not DataBase.getDB(): DataBase.create()
System.get()

import symbolic.cmos.misc
import symbolic.cmos.technology
import symbolic.cmos.display
import symbolic.cmos.analog
import symbolic.cmos.alliance
import symbolic.cmos.etesian
import symbolic.cmos.kite
import symbolic.cmos.plugins
import symbolic.cmos.stratus1

Cfg.Configuration.popDefaultPriority()

helpers.tagConfModules()

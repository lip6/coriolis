
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
# |  Python      :       "./etc/symbolic/cmos45/__init__.py"        |
# +-----------------------------------------------------------------+


import Cfg
import helpers.io
helpers.io.vprint( 1, '  o  Loading "symbolic.cmos45" technology.' )
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

from   Hurricane import DataBase
from   CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

DataBase.create()
System.get()

import symbolic.cmos45.misc
import symbolic.cmos45.technology
import symbolic.cmos45.display
import symbolic.cmos45.analog
import symbolic.cmos45.alliance
import symbolic.cmos45.etesian
import symbolic.cmos45.kite
import symbolic.cmos45.plugins
import symbolic.cmos45.stratus1

Cfg.Configuration.popDefaultPriority()

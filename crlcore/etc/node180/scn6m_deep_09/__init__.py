
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
# |  Python      :       "./etc/node180/scn6m_deep_09/__init__.py"  |
# +-----------------------------------------------------------------+


import Cfg
import helpers.io
helpers.io.vprint( 1, '  o  Loading "node180.scn6m_deep_09" technology.' )
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

from   Hurricane import DataBase
from   CRL       import System

Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )

DataBase.create()
System.get()

import node180.scn6m_deep_09.misc
import node180.scn6m_deep_09.technology
import node180.scn6m_deep_09.display
import node180.scn6m_deep_09.analog
import node180.scn6m_deep_09.alliance
import node180.scn6m_deep_09.etesian
import node180.scn6m_deep_09.kite
import node180.scn6m_deep_09.plugins
import node180.scn6m_deep_09.stratus1
import node180.scn6m_deep_09.devices
import node180.scn6m_deep_09.dtr_scn6m_deep_09

Cfg.Configuration.popDefaultPriority()

helpers.tagConfModules()

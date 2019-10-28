
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
# |  Python      :       "./etc/symbolic/cmos/stratus1.py"          |
# +-----------------------------------------------------------------+
 

import Cfg
import common.stratus1


Cfg.getParamString( "stratus1.format"    ).setString( "vst"    )
Cfg.getParamString( "stratus1.simulator" ).setString( "asimut" )

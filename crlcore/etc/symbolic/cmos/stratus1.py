
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
 

import os.path
import Cfg
import helpers
import helpers.io
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )

import common.stratus1


Cfg.getParamString( "stratus1.format"      ).setString( "vst"    )
Cfg.getParamString( "stratus1.simulator"   ).setString( "asimut" )
Cfg.getParamString( "stratus1.mappingName" ).setString( os.path.join(helpers.sysConfDir,'symbolic/cmos/stratus2sxlib.xml') )

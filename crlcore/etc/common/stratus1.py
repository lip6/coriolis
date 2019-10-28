
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2019, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/common/stratus1.py"                 |
# +-----------------------------------------------------------------+


import Cfg

layout  = Cfg.Configuration.get().getLayout()
layout.addTab      ( 'stratus1', 'Stratus1' )
layout.addTitle    ( 'stratus1', 'Stratus1 - Netlist & Layout Capture' )
layout.addParameter( 'stratus1', 'stratus1.mappingName', 'Virtual Library Translation', 0, 2 )
layout.addParameter( 'stratus1', 'stratus1.format'     , 'Netlist Format (vst, vhd)'  , 0, 2 )
layout.addParameter( 'stratus1', 'stratus1.simulator'  , 'Simulator'                  , 0, 2 )

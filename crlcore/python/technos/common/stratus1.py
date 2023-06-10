
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
# |  Python      :       "./etc/common/stratus1.py"                 |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg

layout  = Cfg.Configuration.get().getLayout()
layout.addTab      ( 'Netlist', 'Stratus1' )
layout.addTitle    ( 'Netlist', 'Stratus1 - Netlist & Layout Capture' )
layout.addParameter( 'Netlist', 'stratus1.mappingName', 'Virtual Library Translation', 0, 2 )
layout.addParameter( 'Netlist', 'stratus1.format'     , 'Netlist Format (vst, vhd)'  , 0, 2 )
layout.addParameter( 'Netlist', 'stratus1.simulator'  , 'Simulator'                  , 0, 2 )

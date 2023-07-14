
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
# |  Python      :       "./etc/common/analog.py"                   |
# +-----------------------------------------------------------------+


import coriolis.Cfg     as Cfg
from   coriolis.helpers import technoDir

p = Cfg.getParamString( 'analog.techno' )
p.setString( 'Analog_technology_has_not_been_set' )
p.flags = Cfg.Parameter.Flags.NeedRestart|Cfg.Parameter.Flags.MustExist

#Cfg.getParamString( 'analog.devices' ).setString( technoDir+'/devices.conf' )

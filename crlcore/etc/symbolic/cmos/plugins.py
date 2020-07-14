
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
# |  Python      :       "./etc/symbolic/cmos/plugins.py"           |
# +-----------------------------------------------------------------+
 

import Cfg
from   helpers import l, u, n
import helpers.io
helpers.io.vprint( 2, '     - "%s".' % helpers.truncPath(__file__) )


Cfg.getParamInt   ( "chip.block.rails.count"    ).setInt   (     5  )
Cfg.getParamInt   ( "chip.block.rails.hWidth"   ).setInt   ( l( 12) )
Cfg.getParamInt   ( "chip.block.rails.vWidth"   ).setInt   ( l( 12) )
Cfg.getParamInt   ( "chip.block.rails.hSpacing" ).setInt   ( l(  6) )
Cfg.getParamInt   ( "chip.block.rails.vSpacing" ).setInt   ( l(  6) )
Cfg.getParamInt   ( 'clockTree.minimumSide'     ).setInt   ( l(600) )
Cfg.getParamString( 'clockTree.buffer'          ).setString( 'buf_x2')
Cfg.getParamString( 'clockTree.placerEngine'    ).setString( 'Etesian')
Cfg.getParamInt   ( 'block.spareSide'           ).setInt   ( 10 )

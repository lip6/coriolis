
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
# |  Python      :       "./etc/symbolic/cmos/plugins.py"           |
# +-----------------------------------------------------------------+
 

import coriolis.Cfg as Cfg
from   coriolis.helpers    import truncPath, l, u, n
from   coriolis.helpers.io import vprint
vprint( 2, '     - "%s".' % truncPath(__file__) )

Cfg.getParamInt   ( "chip.block.rails.count"    ).setInt   (      6  )
Cfg.getParamInt   ( "chip.block.rails.hWidth"   ).setInt   ( l(  24) )
Cfg.getParamInt   ( "chip.block.rails.vWidth"   ).setInt   ( l(  24) )
Cfg.getParamInt   ( "chip.block.rails.hSpacing" ).setInt   ( l(  12) )
Cfg.getParamInt   ( "chip.block.rails.vSpacing" ).setInt   ( l(  12) )
Cfg.getParamInt   ( 'clockTree.minimumSide'     ).setInt   ( l(1200) )
Cfg.getParamString( 'clockTree.buffer'          ).setString( 'buf_x8')
Cfg.getParamString( 'clockTree.placerEngine'    ).setString( 'Etesian')
Cfg.getParamString( 'spares.buffer'             ).setString( 'buf_x8')
Cfg.getParamInt   ( 'spares.maxSinks'           ).setInt   (      31 )


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
# |  Python      :       "./etc/symbolic/lcmos/plugins.py"          |
# +-----------------------------------------------------------------+
 

import coriolis.Cfg as Cfg
from   coriolis.helpers    import truncPath, l, u, n
from   coriolis.helpers.io import vprint
vprint( 2, '     - "%s".' % truncPath(__file__) )

Cfg.getParamInt   ( "chip.block.rails.count"    ).setInt   (     5  )
Cfg.getParamInt   ( "chip.block.rails.hWidth"   ).setInt   ( l( 12) )
Cfg.getParamInt   ( "chip.block.rails.vWidth"   ).setInt   ( l( 12) )
Cfg.getParamInt   ( "chip.block.rails.hSpacing" ).setInt   ( l(  3) )
Cfg.getParamInt   ( "chip.block.rails.vSpacing" ).setInt   ( l(  3) )
Cfg.getParamBool  ( "chip.useAbstractPads"      ).setBool  ( True )
Cfg.getParamInt   ( 'clockTree.minimumSide'     ).setInt   ( l(600) )
Cfg.getParamString( 'clockTree.buffer'          ).setString( 'buf_x2')
Cfg.getParamString( 'clockTree.placerEngine'    ).setString( 'Etesian')
Cfg.getParamInt   ( 'block.spareSide'           ).setInt   ( 10 )
Cfg.getParamString( 'spares.buffer'             ).setString( 'buf_x8')
Cfg.getParamInt   ( 'spares.maxSinks'           ).setInt   (      31 )

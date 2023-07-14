
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
# |  Python      :       "./etc/symbolic/lcmos/alliance.py"         |
# +-----------------------------------------------------------------+


import os
import os.path
from   coriolis.helpers    import truncPath
from   coriolis.helpers.io import vprint
vprint( 2, '     - "{}".'.format(truncPath(__file__)) )

from coriolis.CRL import Environment, AllianceFramework

allianceTop = None
if 'ALLIANCE_TOP' in os.environ:
    allianceTop = os.environ['ALLIANCE_TOP']
    if not os.path.isdir(allianceTop):
        allianceTop = None
if not allianceTop: allianceTop = '/soc/alliance'

cellsTop = allianceTop+'/cells'
af       = AllianceFramework.get()
env      = af.getEnvironment()

env.setSCALE_X        ( 100 )
env.setCATALOG        ( 'CATAL' )
env.setIN_LO          ( 'vst'   )
env.setIN_PH          ( 'ap'    )
env.setOUT_LO         ( 'vst'   )
env.setOUT_PH         ( 'ap'    )
env.setPOWER          ( 'vdd'   )
env.setGROUND         ( 'vss'   )
env.setCLOCK          ( '.*ck.*|.*nck.*'   )
env.setBLOCKAGE       ( 'blockage[Nn]et.*' )
env.setPad            ( '.*_px$'           )
env.setRegister       ( 'sff.*' )

env.setWORKING_LIBRARY( '.' )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/sxlib'   , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/dp_sxlib', mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/ramlib'  , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/romlib'  , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/rflib'   , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/rf2lib'  , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/pxlib'   , mode=Environment.Append )
env.addSYSTEM_LIBRARY ( library=cellsTop+'/padlib'  , mode=Environment.Append )

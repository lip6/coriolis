
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
# |  Python      :       "./node180/scn6m_deep_09/devices.py"       |
# +-----------------------------------------------------------------+


from   coriolis.helpers    import truncPath, sysConfDir
from   coriolis.helpers.io import vprint
vprint( 2, '     - "{}".'.format(truncPath(__file__)) )

from ...common         import devices
from ...common.devices import addDevice

chamsDir = sysConfDir + '/share/coriolis2/'
spiceDir = chamsDir + 'spice/'

addDevice( name       = 'DifferentialPairBulkConnected'
         , spice      = spiceDir+'DiffPairBulkConnected.spi'
         , connectors = ( 'D1', 'D2', 'G1', 'G2', 'S' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.DP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.DP_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.DP_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.DP_interdigitated.py'  )
                        , ('WIP DP'         , 'coriolis.oroshi.wip_dp.py'             )
                        )
         )
addDevice( name       = 'DifferentialPairBulkUnconnected'
         , spice      = spiceDir+'DiffPairBulkUnconnected.spi'
         , connectors = ( 'D1', 'D2', 'G1', 'G2', 'S', 'B' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.DP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.DP_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.DP_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.DP_interdigitated.py'  )
                        , ('WIP DP'         , 'coriolis.oroshi.wip_dp.py'             )
                        )
         )
addDevice( name       = 'LevelShifterBulkUnconnected'
         , spice      = spiceDir+'LevelShifterBulkUnconnected.spi'
         , connectors = ( 'D1', 'D2', 'S1', 'S2', 'B' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.LS_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.LS_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.LS_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.LS_interdigitated.py'  )
                        )
         )
addDevice( name       = 'TransistorBulkConnected'
         , spice      = spiceDir+'TransistorBulkConnected.spi'
         , connectors = ( 'D', 'G', 'S' )
         , layouts    = ( ('Rotate transistor', 'coriolis.oroshi.Transistor_rotate.py')
                        , ('Common transistor', 'coriolis.oroshi.Transistor_common.py')
                        , ('WIP Transistor'   , 'coriolis.oroshi.wip_transistor.py'   )
                        )
         )
addDevice( name       = 'TransistorBulkUnconnected'
         , spice      = spiceDir+'TransistorBulkUnconnected.spi'
         , connectors = ( 'D', 'G', 'S', 'B' )
         , layouts    = ( ('Rotate transistor', 'coriolis.oroshi.Transistor_rotate.py')
                        , ('Common transistor', 'coriolis.oroshi.Transistor_common.py')
                        , ('WIP Transistor'   , 'coriolis.oroshi.wip_transistor.py'   )
                        )
         )
addDevice( name       = 'CrossCoupledPairBulkConnected'
         , spice      = spiceDir+'CCPairBulkConnected.spi'
         , connectors = ( 'D1', 'D2', 'S' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CCP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.CCP_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.CCP_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.CCP_interdigitated.py'  )
                        )
         )
addDevice( name       = 'CrossCoupledPairBulkUnconnected'
         , spice      = spiceDir+'CCPairBulkUnconnected.spi'
         , connectors = ( 'D1', 'D2', 'S', 'B' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CCP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.CCP_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.CCP_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.CCP_interdigitated.py'  )
                        )
         )
addDevice( name       = 'CommonSourcePairBulkConnected'
         , spice      = spiceDir+'CommonSourcePairBulkConnected.spi'
         , connectors = ( 'D1', 'D2', 'S', 'G' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CSP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.CSP_symmetrical.py'     )
                        , ('Interdigitated' , 'coriolis.oroshi.CSP_interdigitated.py'  )
                        , ('WIP CSP'        , 'coriolis.oroshi.wip_csp.py'             )
                        )
         )
addDevice( name       = 'CommonSourcePairBulkUnconnected'
         , spice      = spiceDir+'CommonSourcePairBulkUnconnected.spi'
         , connectors = ( 'D1', 'D2', 'S', 'G', 'B' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.CSP_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.CSP_symmetrical.py'     )
                        , ('Interdigitated' , 'coriolis.oroshi.CSP_interdigitated.py'  )
                        , ('WIP CSP'        , 'coriolis.oroshi.wip_csp.py'             )
                        )
         )
addDevice( name       = 'SimpleCurrentMirrorBulkConnected'
         , spice      = spiceDir+'CurrMirBulkConnected.spi'
         , connectors = ( 'D1', 'D2', 'S' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.SCM_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.SCM_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.SCM_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.SCM_interdigitated.py'  )
                        )
         )
addDevice( name       = 'SimpleCurrentMirrorBulkUnconnected'
         , spice      = spiceDir+'CurrMirBulkUnconnected.spi'
         , connectors = ( 'D1', 'D2', 'S', 'B' )
         , layouts    = ( ('Horizontal M2'  , 'coriolis.oroshi.SCM_horizontalM2.py'    )
                        , ('Symmetrical'    , 'coriolis.oroshi.SCM_symmetrical.py'     )
                        , ('Common centroid', 'coriolis.oroshi.SCM_2DCommonCentroid.py')
                        , ('Interdigitated' , 'coriolis.oroshi.SCM_interdigitated.py' )
                        )
         )
addDevice( name       = 'MultiCapacitor'
        #, spice      = spiceDir+'MIM_OneCapacitor.spi'
        #, connectors = ( 'T1', 'B1' )
         , layouts    = ( ('Matrix', 'coriolis.oroshi.capacitormatrix.py' ),
                        )
         )
addDevice( name       = 'Resistor'
        #, spice      = spiceDir+'MIM_OneCapacitor.spi'
         , connectors = ( 'PIN1', 'PIN2' )
         , layouts    = ( ('Snake', 'coriolis.oroshi.resistorsnake.py' ),
                        )
         )


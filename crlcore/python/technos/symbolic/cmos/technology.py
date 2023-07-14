
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
# |  Python      :       "./etc/symbolic/cmos/technology.py"        |
# +-----------------------------------------------------------------+


from   coriolis.helpers    import l, u, n, truncPath
from   coriolis.helpers.io import WarningMessage, vprint
vprint( 2, '     - "{}".'.format(truncPath(__file__)) )

from   coriolis.Hurricane import DbU, DataBase, Technology


tech = DataBase.getDB().getTechnology()
if tech:
    print( WarningMessage( 'cmos.technology: Technology already exists, "{}"'.format(tech.getName()) ))
else:
    tech = Technology.create( DataBase.getDB(), 'cmos' )

DbU.setPrecision           ( 2 )
DbU.setPhysicalsPerGrid    ( 0.5, DbU.UnitPowerMicro )
DbU.setGridsPerLambda      ( 2 )
DbU.setSymbolicSnapGridStep( DbU.fromLambda(1.0) )
DbU.setPolygonStep         ( DbU.fromGrid  (2.0) )


from ...common import technology, loadGdsLayers


gdsLayersTable = \
    [ ("nWell"   , "LNWELL"  ,  1, 0)
    , ("nImplant", "LNIF"    ,  3, 0)
    , ("pImplant", "LPDIF"   ,  4, 0)
    , ("active"  , "LACTIVE" ,  2, 0)
    , ("poly"    , "LPOLY"   ,  7, 0)
    , ("cut0"    , "LCONT"   , 10, 0)
    , ("metal1"  , "LALU1"   , 11, 0)
    , ("cut1"    , "LVIA"    , 14, 0)
    , ("metal2"  , "LALU2"   , 16, 0)
    , ("cut2"    , "LVIA2"   , 18, 0)
    , ("metal3"  , "LALU3"   , 19, 0)
    , ("cut3"    , "LVIA3"   , 21, 0)
    , ("metal4"  , "LALU4"   , 22, 0)
    , ("cut4"    , "LVIA4"   , 25, 0)
    , ("metal5"  , "LALU5"   , 26, 0)
    , ("cut5"    , "LVIA5"   , 28, 0)
    , ("metal6"  , "LALU6"   , 29, 0)
    ]


loadGdsLayers( gdsLayersTable )

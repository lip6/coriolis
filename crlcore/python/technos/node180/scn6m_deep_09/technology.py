
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./etc/node180.scn6m_deep_09/technology.py"     |
# +-----------------------------------------------------------------+


from   coriolis.helpers    import truncPath, l, u, n
from   coriolis.helpers.io import vprint
vprint( 2, '     - "{}".'.format(truncPath(__file__)) )

from   coriolis.Hurricane import DbU, DataBase, Technology

tech = DataBase.getDB().getTechnology()
if tech:
    print( WarningMessage( 'cmos.technology: Technology already exists, "{}"'.format(tech.getName()) ))
else:
    tech = Technology.create( DataBase.getDB(), 'scn6m_deep_09' )

DbU.setPrecision           ( 2 )
DbU.setPhysicalsPerGrid    ( 0.005, DbU.UnitPowerMicro )
DbU.setGridsPerLambda      ( 18 )
DbU.setSymbolicSnapGridStep( DbU.fromLambda( 1.0) )
DbU.setPolygonStep         ( DbU.fromGrid  ( 9.0) )

from ...common import loadGdsLayers
from ...common.technology import *

# Redefine all size from the "cmos" common part.
NWELL.setExtentionCap( nWell, l(4.0) )
PWELL.setExtentionCap( pWell, l(4.0) )

NTIE.setMinimalSize   (           l( 3.0) )
NTIE.setExtentionCap  ( nWell   , l( 3.0) )
NTIE.setExtentionWidth( nWell   , l( 2.0) )
NTIE.setExtentionCap  ( nImplant, l( 2.5) )
NTIE.setExtentionWidth( nImplant, l( 1.5) )
NTIE.setExtentionCap  ( active  , l( 0.5) )
NTIE.setExtentionWidth( active  , l(-0.5) )

PTIE.setMinimalSize   (           l( 3.0) )
PTIE.setExtentionCap  ( nWell   , l( 3.0) )
PTIE.setExtentionWidth( nWell   , l( 2.0) )
PTIE.setExtentionCap  ( nImplant, l( 2.5) )
PTIE.setExtentionWidth( nImplant, l( 1.5) )
PTIE.setExtentionCap  ( active  , l( 0.5) )
PTIE.setExtentionWidth( active  , l(-0.5) )

NDIF.setMinimalSize   (           l(3.0) )
NDIF.setExtentionCap  ( nImplant, l(4.0) )
NDIF.setExtentionWidth( nImplant, l(2.0) )
NDIF.setExtentionCap  ( active  , l(2.0) )
NDIF.setExtentionWidth( active  , l(0.0) )

PDIF.setMinimalSize   (           l(3.0) )
PDIF.setExtentionCap  ( pImplant, l(4.0) )
PDIF.setExtentionWidth( pImplant, l(2.0) )
PDIF.setExtentionCap  ( active  , l(2.0) )
PDIF.setExtentionWidth( active  , l(0.0) )

GATE.setMinimalSize   (           l(2.0) )
GATE.setExtentionCap  ( poly    , l(2.5) )

NTRANS.setMinimalSize   (           l( 2.0) )
NTRANS.setExtentionCap  ( nImplant, l( 2.0) )
NTRANS.setExtentionWidth( nImplant, l( 7.0) )
NTRANS.setExtentionCap  ( active  , l( 0.0) )
NTRANS.setExtentionWidth( active  , l( 3.0) )
NTRANS.setExtentionCap  ( poly    , l( 3.0) )
NTRANS.setExtentionWidth( poly    , l( 0.0) )

PTRANS.setMinimalSize   (           l( 2.0) )
PTRANS.setExtentionCap  ( nWell   , l( 2.5) )
PTRANS.setExtentionWidth( nWell   , l( 7.5) )
PTRANS.setExtentionCap  ( pImplant, l( 2.0) )
PTRANS.setExtentionWidth( pImplant, l( 7.0) )
PTRANS.setExtentionCap  ( active  , l( 0.0) )
PTRANS.setExtentionWidth( active  , l( 3.0) )
PTRANS.setExtentionCap  ( poly    , l( 3.0) )
PTRANS.setExtentionWidth( poly    , l( 0.0) )

POLY .setMinimalSize   (           l(2.0) )
POLY .setExtentionCap  ( poly    , l(1.0) )
POLY2.setMinimalSize   (           l(2.0) )
POLY2.setExtentionCap  ( poly    , l(1.0) )

# Routing Layers (symbolic).
METAL1 .setMinimalSize   (           l( 2.0) )
METAL1 .setExtentionCap  ( metal1  , l( 2.0) )
METAL1 .setExtentionWidth( metal1  , l( 1.0) )
METAL1 .setMinimalSpacing(           l( 6.0) )
METAL2 .setMinimalSize   (           l( 4.0) )
METAL2 .setExtentionCap  ( metal2  , l( 2.0) )
METAL2 .setMinimalSpacing(           l( 6.0) )
METAL3 .setMinimalSize   (           l( 4.0) )
METAL3 .setExtentionCap  ( metal3  , l( 2.0) )
METAL3 .setMinimalSpacing(           l( 6.0) )
METAL4 .setMinimalSize   (           l( 4.0) )
METAL4 .setExtentionCap  ( metal4  , l( 2.0) )
METAL4 .setMinimalSpacing(           l( 6.0) )
METAL5 .setMinimalSize   (           l( 4.0) )
METAL5 .setExtentionCap  ( metal5  , l( 2.0) )
METAL5 .setMinimalSpacing(           l( 6.0) )
METAL6 .setMinimalSize   (           l(10.0) )
METAL6 .setExtentionCap  ( metal6  , l( 5.0) )
METAL6 .setMinimalSpacing(           l( 6.0) )
#METAL7 .setMinimalSize   (           l( 2.0) )
#METAL7 .setExtentionCap  ( metal7  , l( 1.0) )
#METAL8 .setMinimalSize   (           l( 2.0) )
#METAL8 .setExtentionCap  ( metal8  , l( 1.0) )
#METAL9 .setMinimalSize   (           l( 2.0) )
#METAL9 .setExtentionCap  ( metal9  , l( 1.0) )
#METAL10.setMinimalSize   (           l( 2.0) )
#METAL10.setExtentionCap  ( metal10 , l( 1.0) )

# Contacts (i.e. Active <--> Metal) (symbolic).
CONT_BODY_N.setMinimalSize(           l( 2.0) )
CONT_BODY_N.setEnclosure  ( nWell   , l( 4.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( nImplant, l( 3.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( active  , l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )

CONT_BODY_P.setMinimalSize(           l( 2.0) )
CONT_BODY_P.setEnclosure  ( pWell   , l( 4.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( pImplant, l( 3.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( active  , l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )

CONT_DIF_N.setMinimalSize(           l( 2.0) )
CONT_DIF_N.setEnclosure  ( nImplant, l( 4.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_N.setEnclosure  ( active  , l( 2.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_N.setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )

CONT_DIF_P.setMinimalSize(           l( 2.0) )
CONT_DIF_P.setEnclosure  ( pImplant, l( 4.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_P.setEnclosure  ( active  , l( 2.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_P.setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )

CONT_POLY.setMinimalSize(           l( 2.0) )
CONT_POLY.setEnclosure  ( poly    , l( 2.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_POLY.setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )

# VIAs (i.e. Metal <--> Metal) (symbolic).
VIA12 .setMinimalSize(           l( 2.0) )
VIA12 .setEnclosure  ( metal1  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA12 .setEnclosure  ( metal2  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA23 .setMinimalSize(           l( 2.0) )
VIA23 .setEnclosure  ( metal2  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA23 .setEnclosure  ( metal3  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA34 .setMinimalSize(           l( 2.0) )
VIA34 .setEnclosure  ( metal3  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA34 .setEnclosure  ( metal4  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA45 .setMinimalSize(           l( 2.0) )
VIA45 .setEnclosure  ( metal4  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA45 .setEnclosure  ( metal5  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA56 .setMinimalSize(           l( 5.0) )
VIA56 .setEnclosure  ( metal5  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
VIA56 .setEnclosure  ( metal6  , l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA67 .setMinimalSize(           l( 1.0) )
#VIA67 .setEnclosure  ( metal6  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA67 .setEnclosure  ( metal7  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA78 .setMinimalSize(           l( 1.0) )
#VIA78 .setEnclosure  ( metal7  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA78 .setEnclosure  ( metal8  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA89 .setMinimalSize(           l( 1.0) )
#VIA89 .setEnclosure  ( metal8  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA89 .setEnclosure  ( metal9  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA910.setMinimalSize(           l( 1.0) )
#VIA910.setEnclosure  ( metal9  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
#VIA910.setEnclosure  ( metal10 , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

# Blockages (symbolic).
BLOCKAGE1 .setMinimalSize   (             l( 4.0) )
BLOCKAGE1 .setExtentionCap  ( blockage1 , l( 2.0) )
BLOCKAGE1 .setExtentionWidth( blockage1 , l( 0.5) )
BLOCKAGE2 .setMinimalSize   (             l( 4.0) )
BLOCKAGE2 .setExtentionCap  ( blockage2 , l( 2.0) )
BLOCKAGE3 .setMinimalSize   (             l( 4.0) )
BLOCKAGE3 .setExtentionCap  ( blockage3 , l( 2.0) )
BLOCKAGE4 .setMinimalSize   (             l( 4.0) )
BLOCKAGE4 .setExtentionCap  ( blockage4 , l( 2.0) )
BLOCKAGE5 .setMinimalSize   (             l( 4.0) )
BLOCKAGE5 .setExtentionCap  ( blockage5 , l( 2.0) )
BLOCKAGE6 .setMinimalSize   (             l( 8.0) )
BLOCKAGE6 .setExtentionCap  ( blockage6 , l( 2.5) )
#BLOCKAGE7 .setMinimalSize   (             l( 2.0) )
#BLOCKAGE7 .setExtentionCap  ( blockage7 , l( 1.0) )
#BLOCKAGE8 .setMinimalSize   (             l( 2.0) )
#BLOCKAGE8 .setExtentionCap  ( blockage8 , l( 1.0) )
#BLOCKAGE9 .setMinimalSize   (             l( 2.0) )
#BLOCKAGE9 .setExtentionCap  ( blockage9 , l( 1.0) )
#BLOCKAGE10.setMinimalSize   (             l( 2.0) )
#BLOCKAGE10.setExtentionCap  ( blockage10, l( 1.0) )


gdsLayersTable = \
    [ ("pWell"   , "CWN"    , 41, 0)
    , ("nWell"   , "CWP"    , 42, 0)
    , ("active"  , "CAA"    , 43, 0)
    , ("pImplant", "CSP"    , 44, 0)
    , ("nImplant", "CSN"    , 45, 0)
    , ("poly"    , "CPG"    , 46, 0)  
    , ("poly2"   , "CM1"    , 49, 0)  # poly2 is in fact metal1.
    , ("cut0"    , "CCC"    , 25, 0)
    , ("metal1"  , "CM1"    , 49, 0)
    , ("cut1"    , "CV1"    , 50, 0)
    , ("metal2"  , "CM2"    , 51, 0)
    , ("cut2"    , "CV2"    , 61, 0)
    , ("metal3"  , "CM3"    , 62, 0)
    , ("cut3"    , "CV3"    , 30, 0)
    , ("metal4"  , "CM4"    , 31, 0)
    , ("cut4"    , "CV4"    , 32, 0)
    , ("metal5"  , "CM5"    , 33, 0)
    , ("cut5"    , "CV5"    , 36, 0)
    , ("metal6"  , "CM6"    , 37, 0)
    ]


loadGdsLayers( gdsLayersTable )

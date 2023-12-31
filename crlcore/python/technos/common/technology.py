
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
# |  Python      :       "./etc/common/technology.py"               |
# +-----------------------------------------------------------------+


from coriolis.helpers   import l, u, n
from coriolis.Hurricane import DataBase, Technology, Layer, BasicLayer, \
                               DiffusionLayer, TransistorLayer,         \
                               RegularLayer, ContactLayer, ViaLayer


tech = DataBase.getDB().getTechnology()


def createBL ( layerName, material ):
    global tech
    return BasicLayer.create( tech, layerName, BasicLayer.Material(material) )

           
nWell      = createBL( 'nWell'     , BasicLayer.Material.nWell    )  # Non-Routing Layers.
pWell      = createBL( 'pWell'     , BasicLayer.Material.pWell    )
nImplant   = createBL( 'nImplant'  , BasicLayer.Material.nImplant )
pImplant   = createBL( 'pImplant'  , BasicLayer.Material.pImplant )
active     = createBL( 'active'    , BasicLayer.Material.active   )
poly       = createBL( 'poly'      , BasicLayer.Material.poly     )
poly2      = createBL( 'poly2'     , BasicLayer.Material.poly     )
cut0       = createBL( 'cut0'      , BasicLayer.Material.cut      )  # Routing Layers & VIA Cuts.
metal1     = createBL( 'metal1'    , BasicLayer.Material.metal    )  # WARNING: order *is* meaningful.
cut1       = createBL( 'cut1'      , BasicLayer.Material.cut      )
metal2     = createBL( 'metal2'    , BasicLayer.Material.metal    )
metcap     = createBL( 'metcap'    , BasicLayer.Material.other    )
cut2       = createBL( 'cut2'      , BasicLayer.Material.cut      )
metal3     = createBL( 'metal3'    , BasicLayer.Material.metal    )
cut3       = createBL( 'cut3'      , BasicLayer.Material.cut      )
metal4     = createBL( 'metal4'    , BasicLayer.Material.metal    )
cut4       = createBL( 'cut4'      , BasicLayer.Material.cut      )
metal5     = createBL( 'metal5'    , BasicLayer.Material.metal    )
cut5       = createBL( 'cut5'      , BasicLayer.Material.cut      )
metal6     = createBL( 'metal6'    , BasicLayer.Material.metal    )
cut6       = createBL( 'cut6'      , BasicLayer.Material.cut      )
metal7     = createBL( 'metal7'    , BasicLayer.Material.metal    )
cut7       = createBL( 'cut7'      , BasicLayer.Material.cut      )
metal8     = createBL( 'metal8'    , BasicLayer.Material.metal    )
cut8       = createBL( 'cut8'      , BasicLayer.Material.cut      )
metal9     = createBL( 'metal9'    , BasicLayer.Material.metal    )
cut9       = createBL( 'cut9'      , BasicLayer.Material.cut      )
metal10    = createBL( 'metal10'   , BasicLayer.Material.metal    )

blockage1  = createBL( 'blockage1' , BasicLayer.Material.blockage )
blockage2  = createBL( 'blockage2' , BasicLayer.Material.blockage )
blockage3  = createBL( 'blockage3' , BasicLayer.Material.blockage )
blockage4  = createBL( 'blockage4' , BasicLayer.Material.blockage )
blockage5  = createBL( 'blockage5' , BasicLayer.Material.blockage )
blockage6  = createBL( 'blockage6' , BasicLayer.Material.blockage )
blockage7  = createBL( 'blockage7' , BasicLayer.Material.blockage )
blockage8  = createBL( 'blockage8' , BasicLayer.Material.blockage )
blockage9  = createBL( 'blockage9' , BasicLayer.Material.blockage )
blockage10 = createBL( 'blockage10', BasicLayer.Material.blockage )

metal1 .setBlockageLayer( blockage1  )
metal2 .setBlockageLayer( blockage2  )
metal3 .setBlockageLayer( blockage3  )
metal4 .setBlockageLayer( blockage4  )
metal5 .setBlockageLayer( blockage5  )
metal6 .setBlockageLayer( blockage6  )
metal7 .setBlockageLayer( blockage7  )
metal8 .setBlockageLayer( blockage8  )
metal9 .setBlockageLayer( blockage9  )
metal10.setBlockageLayer( blockage10 )

textCell  = createBL( 'text.cell'    , BasicLayer.Material.other )  # Misc. non-physical layers.
textInst  = createBL( 'text.instance', BasicLayer.Material.other )  # Used by the software for visualization
SPL1      = createBL( 'SPL1'         , BasicLayer.Material.other )  # purposes only.
AutoLayer = createBL( 'AutoLayer'    , BasicLayer.Material.other )
gmetalh   = createBL( 'gmetalh'      , BasicLayer.Material.metal )  # Special BasicLayers for Knik & Kite Routers.
gcut      = createBL( 'gcut'         , BasicLayer.Material.cut   )  # *Must be after all others*
gmetalv   = createBL( 'gmetalv'      , BasicLayer.Material.metal )

# VIAs for real technologies.
via12  = ViaLayer.create( tech, 'via12' , metal1, cut1, metal2  )
via23  = ViaLayer.create( tech, 'via23' , metal2, cut2, metal3  )
via34  = ViaLayer.create( tech, 'via34' , metal3, cut3, metal4  )
via45  = ViaLayer.create( tech, 'via45' , metal4, cut4, metal5  )
via56  = ViaLayer.create( tech, 'via56' , metal5, cut5, metal6  )
via67  = ViaLayer.create( tech, 'via67' , metal6, cut6, metal7  )
via78  = ViaLayer.create( tech, 'via78' , metal7, cut7, metal8  )
via89  = ViaLayer.create( tech, 'via89' , metal8, cut8, metal9  )
via910 = ViaLayer.create( tech, 'via910', metal9, cut9, metal10 )

# Composite/Symbolic layers.
NWELL       = RegularLayer   .create( tech, 'NWELL'      , nWell    )
PWELL       = RegularLayer   .create( tech, 'PWELL'      , pWell    )
NTIE        = DiffusionLayer .create( tech, 'NTIE'       , nImplant , active, nWell)
PTIE        = DiffusionLayer .create( tech, 'PTIE'       , pImplant , active, pWell)
NDIF        = DiffusionLayer .create( tech, 'NDIF'       , nImplant , active, None )
PDIF        = DiffusionLayer .create( tech, 'PDIF'       , pImplant , active, None )
GATE        = DiffusionLayer .create( tech, 'GATE'       , poly     , active, None )
NTRANS      = TransistorLayer.create( tech, 'NTRANS'     , nImplant , active, poly, None )
PTRANS      = TransistorLayer.create( tech, 'PTRANS'     , pImplant , active, poly, nWell )
POLY        = RegularLayer   .create( tech, 'POLY'       , poly     )
POLY2       = RegularLayer   .create( tech, 'POLY2'      , poly2    )
METAL1      = RegularLayer   .create( tech, 'METAL1'     , metal1   )
METAL2      = RegularLayer   .create( tech, 'METAL2'     , metal2   )
metcapdum   = RegularLayer   .create( tech, 'metcapdum'  , metcap   )
metbot      = RegularLayer   .create( tech, 'metbot'     , metal2   )
METAL3      = RegularLayer   .create( tech, 'METAL3'     , metal3   )
METAL4      = RegularLayer   .create( tech, 'METAL4'     , metal4   )
METAL5      = RegularLayer   .create( tech, 'METAL5'     , metal5   )
METAL6      = RegularLayer   .create( tech, 'METAL6'     , metal6   )
METAL7      = RegularLayer   .create( tech, 'METAL7'     , metal7   )
METAL8      = RegularLayer   .create( tech, 'METAL8'     , metal8   )
METAL9      = RegularLayer   .create( tech, 'METAL9'     , metal9   )
METAL10     = RegularLayer   .create( tech, 'METAL10'    , metal10  )
CONT_BODY_N = ContactLayer   .create( tech, 'CONT_BODY_N', nImplant , active, cut0, metal1, nWell )
CONT_BODY_P = ContactLayer   .create( tech, 'CONT_BODY_P', pImplant , active, cut0, metal1, pWell )
CONT_DIF_N  = ContactLayer   .create( tech, 'CONT_DIF_N' , nImplant , active, cut0, metal1, None )
CONT_DIF_P  = ContactLayer   .create( tech, 'CONT_DIF_P' , pImplant , active, cut0, metal1, None )
CONT_POLY   = ViaLayer       .create( tech, 'CONT_POLY'  ,              poly, cut0, metal1 )

# VIAs for symbolic technologies.
VIA12      = ViaLayer    .create( tech, 'VIA12'     , metal1, cut1, metal2  )
VIA23      = ViaLayer    .create( tech, 'VIA23'     , metal2, cut2, metal3  )
VIA23cap   = ViaLayer    .create( tech, 'VIA23cap'  , metcap, cut2, metal3  )
VIA34      = ViaLayer    .create( tech, 'VIA34'     , metal3, cut3, metal4  )
VIA45      = ViaLayer    .create( tech, 'VIA45'     , metal4, cut4, metal5  )
VIA56      = ViaLayer    .create( tech, 'VIA56'     , metal5, cut5, metal6  )
VIA67      = ViaLayer    .create( tech, 'VIA67'     , metal6, cut6, metal7  )
VIA78      = ViaLayer    .create( tech, 'VIA78'     , metal7, cut7, metal8  )
VIA89      = ViaLayer    .create( tech, 'VIA89'     , metal8, cut8, metal9  )
VIA910     = ViaLayer    .create( tech, 'VIA910'    , metal9, cut9, metal10 )
BLOCKAGE1  = RegularLayer.create( tech, 'BLOCKAGE1' , blockage1  )
BLOCKAGE2  = RegularLayer.create( tech, 'BLOCKAGE2' , blockage2  )
BLOCKAGE3  = RegularLayer.create( tech, 'BLOCKAGE3' , blockage3  )
BLOCKAGE4  = RegularLayer.create( tech, 'BLOCKAGE4' , blockage4  )
BLOCKAGE5  = RegularLayer.create( tech, 'BLOCKAGE5' , blockage5  )
BLOCKAGE6  = RegularLayer.create( tech, 'BLOCKAGE6' , blockage6  )
BLOCKAGE7  = RegularLayer.create( tech, 'BLOCKAGE7' , blockage7  )
BLOCKAGE8  = RegularLayer.create( tech, 'BLOCKAGE8' , blockage8  )
BLOCKAGE9  = RegularLayer.create( tech, 'BLOCKAGE9' , blockage9  )
BLOCKAGE10 = RegularLayer.create( tech, 'BLOCKAGE10', blockage10 )
gcontact   = ViaLayer    .create( tech, 'gcontact'  , gmetalh   , gcut, gmetalv )

tech.setSymbolicLayer( CONT_BODY_N.getName() )
tech.setSymbolicLayer( CONT_BODY_P.getName() )
tech.setSymbolicLayer( CONT_DIF_N .getName() )
tech.setSymbolicLayer( CONT_DIF_P .getName() )
tech.setSymbolicLayer( CONT_POLY  .getName() )
tech.setSymbolicLayer( POLY       .getName() )
tech.setSymbolicLayer( POLY2      .getName() )
tech.setSymbolicLayer( METAL1     .getName() )
tech.setSymbolicLayer( METAL2     .getName() )
tech.setSymbolicLayer( METAL3     .getName() )
tech.setSymbolicLayer( METAL4     .getName() )
tech.setSymbolicLayer( METAL5     .getName() )
tech.setSymbolicLayer( METAL6     .getName() )
tech.setSymbolicLayer( METAL7     .getName() )
tech.setSymbolicLayer( METAL8     .getName() )
tech.setSymbolicLayer( METAL9     .getName() )
tech.setSymbolicLayer( METAL10    .getName() )
tech.setSymbolicLayer( BLOCKAGE1  .getName() )
tech.setSymbolicLayer( BLOCKAGE2  .getName() )
tech.setSymbolicLayer( BLOCKAGE3  .getName() )
tech.setSymbolicLayer( BLOCKAGE4  .getName() )
tech.setSymbolicLayer( BLOCKAGE5  .getName() )
tech.setSymbolicLayer( BLOCKAGE6  .getName() )
tech.setSymbolicLayer( BLOCKAGE7  .getName() )
tech.setSymbolicLayer( BLOCKAGE8  .getName() )
tech.setSymbolicLayer( BLOCKAGE9  .getName() )
tech.setSymbolicLayer( BLOCKAGE10 .getName() )
tech.setSymbolicLayer( VIA12      .getName() )
tech.setSymbolicLayer( VIA23      .getName() )
tech.setSymbolicLayer( VIA34      .getName() )
tech.setSymbolicLayer( VIA45      .getName() )
tech.setSymbolicLayer( VIA56      .getName() )
tech.setSymbolicLayer( VIA67      .getName() )
tech.setSymbolicLayer( VIA78      .getName() )
tech.setSymbolicLayer( VIA89      .getName() )
tech.setSymbolicLayer( VIA910     .getName() )
tech.setSymbolicLayer( gcut       .getName() )
tech.setSymbolicLayer( gmetalh    .getName() )
tech.setSymbolicLayer( gmetalv    .getName() )
tech.setSymbolicLayer( gcontact   .getName() )

NWELL.setExtentionCap( nWell, l(0.0) )
PWELL.setExtentionCap( pWell, l(0.0) )

NTIE.setMinimalSize   (           l(3.0) )
NTIE.setExtentionCap  ( nWell   , l(1.5) )
NTIE.setExtentionWidth( nWell   , l(0.5) )
NTIE.setExtentionCap  ( nImplant, l(1.0) )
NTIE.setExtentionWidth( nImplant, l(0.5) )
NTIE.setExtentionCap  ( active  , l(0.5) )
NTIE.setExtentionWidth( active  , l(0.0) )

PTIE.setMinimalSize   (           l(3.0) )
PTIE.setExtentionCap  ( nWell   , l(1.5) )
PTIE.setExtentionWidth( nWell   , l(0.5) )
PTIE.setExtentionCap  ( nImplant, l(1.0) )
PTIE.setExtentionWidth( nImplant, l(0.5) )
PTIE.setExtentionCap  ( active  , l(0.5) )
PTIE.setExtentionWidth( active  , l(0.0) )

NDIF.setMinimalSize   (           l(3.0) )
NDIF.setExtentionCap  ( nImplant, l(1.0) )
NDIF.setExtentionWidth( nImplant, l(0.5) )
NDIF.setExtentionCap  ( active  , l(0.5) )
NDIF.setExtentionWidth( active  , l(0.0) )

PDIF.setMinimalSize   (           l(3.0) )
PDIF.setExtentionCap  ( pImplant, l(1.0) )
PDIF.setExtentionWidth( pImplant, l(0.5) )
PDIF.setExtentionCap  ( active  , l(0.5) )
PDIF.setExtentionWidth( active  , l(0.0) )

GATE.setMinimalSize   (           l(1.0) )
GATE.setExtentionCap  ( poly    , l(1.5) )

NTRANS.setMinimalSize   (           l( 1.0) )
NTRANS.setExtentionCap  ( nImplant, l(-1.0) )
NTRANS.setExtentionWidth( nImplant, l( 2.5) )
NTRANS.setExtentionCap  ( active  , l(-1.5) )
NTRANS.setExtentionWidth( active  , l( 2.0) )

PTRANS.setMinimalSize   (           l( 1.0) )
PTRANS.setExtentionCap  ( nWell   , l(-1.0) )
PTRANS.setExtentionWidth( nWell   , l( 4.5) )
PTRANS.setExtentionCap  ( pImplant, l(-1.0) )
PTRANS.setExtentionWidth( pImplant, l( 4.0) )
PTRANS.setExtentionCap  ( active  , l(-1.5) )
PTRANS.setExtentionWidth( active  , l( 3.0) )

POLY .setMinimalSize   (           l(1.0) )
POLY .setExtentionCap  ( poly    , l(0.5) )
POLY2.setMinimalSize   (           l(1.0) )
POLY2.setExtentionCap  ( poly    , l(0.5) )

METAL1 .setMinimalSize   (           l(1.0) )
METAL1 .setExtentionCap  ( metal1  , l(0.5) )
METAL2 .setMinimalSize   (           l(1.0) )
METAL2 .setExtentionCap  ( metal2  , l(1.0) )
METAL3 .setMinimalSize   (           l(1.0) )
METAL3 .setExtentionCap  ( metal3  , l(1.0) )
METAL4 .setMinimalSize   (           l(1.0) )
METAL4 .setExtentionCap  ( metal4  , l(1.0) )
METAL4 .setMinimalSpacing(           l(3.0) )
METAL5 .setMinimalSize   (           l(2.0) )
METAL5 .setExtentionCap  ( metal5  , l(1.0) )
METAL6 .setMinimalSize   (           l(2.0) )
METAL6 .setExtentionCap  ( metal6  , l(1.0) )
METAL7 .setMinimalSize   (           l(2.0) )
METAL7 .setExtentionCap  ( metal7  , l(1.0) )
METAL8 .setMinimalSize   (           l(2.0) )
METAL8 .setExtentionCap  ( metal8  , l(1.0) )
METAL9 .setMinimalSize   (           l(2.0) )
METAL9 .setExtentionCap  ( metal9  , l(1.0) )
METAL10.setMinimalSize   (           l(2.0) )
METAL10.setExtentionCap  ( metal10 , l(1.0) )

# Contacts (i.e. Active <--> Metal) (symbolic).
CONT_BODY_N.setMinimalSize(           l( 1.0) )
CONT_BODY_N.setEnclosure  ( nWell   , l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( nImplant, l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( active  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_N.setEnclosure  ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

CONT_BODY_P.setMinimalSize(           l( 1.0) )
CONT_BODY_P.setEnclosure  ( pWell   , l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( pImplant, l( 1.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( active  , l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_BODY_P.setEnclosure  ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

CONT_DIF_N.setMinimalSize(           l( 1.0) )
CONT_DIF_N.setEnclosure  ( nImplant, l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_N.setEnclosure  ( active  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_N.setEnclosure  ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

CONT_DIF_P.setMinimalSize(           l( 1.0) )
CONT_DIF_P.setEnclosure  ( pImplant, l( 1.0), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_P.setEnclosure  ( active  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_DIF_P.setEnclosure  ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

CONT_POLY.setMinimalSize(           l( 1.0) )
CONT_POLY.setEnclosure  ( poly    , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
CONT_POLY.setEnclosure  ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )

# VIAs (i.e. Metal <--> Metal) (symbolic).
VIA12 .setMinimalSize   (           l( 1.0) )
VIA12 .setEnclosure     ( metal1  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA12 .setEnclosure     ( metal2  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA12 .setMinimalSpacing(           l( 4.0) )
VIA23 .setMinimalSize   (           l( 1.0) )
VIA23 .setEnclosure     ( metal2  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA23 .setEnclosure     ( metal3  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA23 .setMinimalSpacing(           l( 4.0) )
VIA34 .setMinimalSize   (           l( 1.0) )
VIA34 .setEnclosure     ( metal3  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA34 .setEnclosure     ( metal4  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA34 .setMinimalSpacing(           l( 4.0) )
VIA45 .setMinimalSize   (           l( 1.0) )
VIA45 .setEnclosure     ( metal4  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA45 .setEnclosure     ( metal5  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA45 .setMinimalSpacing(           l( 4.0) )
VIA56 .setMinimalSize   (           l( 1.0) )
VIA56 .setEnclosure     ( metal5  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA56 .setEnclosure     ( metal6  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA56 .setMinimalSpacing(           l( 4.0) )
VIA67 .setMinimalSize   (           l( 1.0) )
VIA67 .setEnclosure     ( metal6  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA67 .setEnclosure     ( metal7  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA67 .setMinimalSpacing(           l( 4.0) )
VIA78 .setMinimalSpacing(           l( 4.0) )
VIA78 .setMinimalSize   (           l( 1.0) )
VIA78 .setEnclosure     ( metal7  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA78 .setEnclosure     ( metal8  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA78 .setMinimalSpacing(           l( 4.0) )
VIA89 .setMinimalSize   (           l( 1.0) )
VIA89 .setEnclosure     ( metal8  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA89 .setEnclosure     ( metal9  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA89 .setMinimalSpacing(           l( 4.0) )
VIA910.setMinimalSize   (           l( 1.0) )
VIA910.setEnclosure     ( metal9  , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA910.setEnclosure     ( metal10 , l( 0.5), Layer.EnclosureH|Layer.EnclosureV )
VIA910.setMinimalSpacing(           l( 4.0) )

# Blockages (symbolic).
BLOCKAGE1 .setMinimalSize (             l( 1.0) )
BLOCKAGE1 .setExtentionCap( blockage1 , l( 0.5) )
BLOCKAGE2 .setMinimalSize (             l( 2.0) )
BLOCKAGE2 .setExtentionCap( blockage2 , l( 0.5) )
BLOCKAGE3 .setMinimalSize (             l( 2.0) )
BLOCKAGE3 .setExtentionCap( blockage3 , l( 0.5) )
BLOCKAGE4 .setMinimalSize (             l( 2.0) )
BLOCKAGE4 .setExtentionCap( blockage4 , l( 0.5) )
BLOCKAGE5 .setMinimalSize (             l( 2.0) )
BLOCKAGE5 .setExtentionCap( blockage5 , l( 1.0) )
BLOCKAGE6 .setMinimalSize (             l( 2.0) )
BLOCKAGE6 .setExtentionCap( blockage6 , l( 1.0) )
BLOCKAGE7 .setMinimalSize (             l( 2.0) )
BLOCKAGE7 .setExtentionCap( blockage7 , l( 1.0) )
BLOCKAGE8 .setMinimalSize (             l( 2.0) )
BLOCKAGE8 .setExtentionCap( blockage8 , l( 1.0) )
BLOCKAGE9 .setMinimalSize (             l( 2.0) )
BLOCKAGE9 .setExtentionCap( blockage9 , l( 1.0) )
BLOCKAGE10.setMinimalSize (             l( 2.0) )
BLOCKAGE10.setExtentionCap( blockage10, l( 1.0) )
    

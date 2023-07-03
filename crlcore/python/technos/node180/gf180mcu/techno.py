
import sys
import os.path
from   coriolis                         import Cfg, Hurricane, Viewer, CRL
from   coriolis.Hurricane               import Technology, DataBase, DbU, Library, Layer,   \
                                               BasicLayer, Cell, Net, Horizontal, Vertical, \
                                               Rectilinear, Box, Point, NetExternalComponents
from   coriolis.technos.common.colors   import toRGB
from   coriolis.technos.common.patterns import toHexa
from   coriolis.helpers                 import l, u, trace, io
from   coriolis.helpers.technology      import createBL, createVia, setEnclosures
from   coriolis.helpers.overlay         import CfgCache


__all__ = [ 'setup' ]


def _setup_techno():
    io.vprint( 1, '  o  Setup GF180MCU technology.' )
    io.vprint( 2, '     (__file__="{}")'.format( os.path.abspath( __file__ )))

    db = DataBase.create()
    CRL.System.get()

    tech = Technology.create( db, 'GF180MCU' )

    DbU.setPrecision( 2 )
    DbU.setPhysicalsPerGrid( 0.005, DbU.UnitPowerMicro )
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.gdsDriver.metricDbu = 1e-09
        cfg.gdsDriver.dbuPerUu  = 0.001
    DbU.setGridsPerLambda      ( 10 )
    DbU.setSymbolicSnapGridStep( DbU.fromGrid( 1.0 ))
    DbU.setPolygonStep         ( DbU.fromGrid( 1.0 ))
    DbU.setStringMode          ( DbU.StringModePhysical, DbU.UnitPowerMicro )

    Nwell    = createBL( tech, 'Nwell'   , BasicLayer.Material.nWell    , size=u(0.86), spacing=u(0.74), gds2Layer=21  )
    LVPwell  = createBL( tech, 'LVPwell' , BasicLayer.Material.pWell    , size=u(0.74), spacing=u(1.7 ), gds2Layer=204 )
    Nplus    = createBL( tech, 'Nplus'   , BasicLayer.Material.nImplant , size=u(0.4 ), spacing=u(0.4 ), gds2Layer=32  )
    Pplus    = createBL( tech, 'Pplus'   , BasicLayer.Material.pImplant , size=u(0.4 ), spacing=u(0.4 ), gds2Layer=31  )
    COMP     = createBL( tech, 'COMP'    , BasicLayer.Material.active   , size=u(0.3 ), spacing=u(0.36), gds2Layer=22  )
    Poly2    = createBL( tech, 'Poly2'   , BasicLayer.Material.poly     , size=u(0.2 ), spacing=u(0.24), gds2Layer=30  )
    Cont     = createBL( tech, 'Contact' , BasicLayer.Material.cut      , size=u(0.22), spacing=u(0.25), gds2Layer=33  )
    Metal1   = createBL( tech, 'Metal1'  , BasicLayer.Material.metal    , size=u(0.23), spacing=u(0.23), gds2Layer=34  )
    Via1     = createBL( tech, 'Via1'    , BasicLayer.Material.cut      , size=u(0.26), spacing=u(0.26), gds2Layer=35  )
    Metal2   = createBL( tech, 'Metal2'  , BasicLayer.Material.metal    , size=u(0.28), spacing=u(0.28), gds2Layer=36  )
    Via2     = createBL( tech, 'Via2'    , BasicLayer.Material.cut      , size=u(0.26), spacing=u(0.26), gds2Layer=38  )
    Metal3   = createBL( tech, 'Metal3'  , BasicLayer.Material.metal    , size=u(0.28), spacing=u(0.28), gds2Layer=42  )
    Via3     = createBL( tech, 'Via3'    , BasicLayer.Material.cut      , size=u(0.26), spacing=u(0.26), gds2Layer=40  )
    Metal4   = createBL( tech, 'Metal4'  , BasicLayer.Material.metal    , size=u(0.28), spacing=u(0.28), gds2Layer=46  )
    Via4     = createBL( tech, 'Via4'    , BasicLayer.Material.cut      , size=u(0.26), spacing=u(0.26), gds2Layer=41  )
    Metal5   = createBL( tech, 'Metal5'  , BasicLayer.Material.metal    , size=u(0.28), spacing=u(0.28), gds2Layer=81  )
    Via5     = createBL( tech, 'Via5'    , BasicLayer.Material.cut      , size=u(0.26), spacing=u(0.26), gds2Layer=82  )
    MetalTop = createBL( tech, 'MetalTop', BasicLayer.Material.metal    , size=u(0.36), spacing=u(0.38), gds2Layer=53  )

    Poly2_Dummy    = createBL( tech, 'Poly2_Dummy'   , BasicLayer.Material.poly , gds2Layer=30, gds2DataType=4 )
    Metal1_Dummy   = createBL( tech, 'Metal1_Dummy'  , BasicLayer.Material.metal, gds2Layer=34, gds2DataType=4 )
    Metal2_Dummy   = createBL( tech, 'Metal2_Dummy'  , BasicLayer.Material.metal, gds2Layer=36, gds2DataType=4 )
    Metal3_Dummy   = createBL( tech, 'Metal3_Dummy'  , BasicLayer.Material.metal, gds2Layer=42, gds2DataType=4 )
    Metal4_Dummy   = createBL( tech, 'Metal4_Dummy'  , BasicLayer.Material.metal, gds2Layer=46, gds2DataType=4 )
    Metal5_Dummy   = createBL( tech, 'Metal5_Dummy'  , BasicLayer.Material.metal, gds2Layer=81, gds2DataType=4 )
    MetalTop_Dummy = createBL( tech, 'MetalTop_Dummy', BasicLayer.Material.metal, gds2Layer=53, gds2DataType=4 )

    Poly2_Label    = createBL( tech, 'Poly2_Label'   , BasicLayer.Material.info, gds2Layer=30, gds2DataType=10 )
    Metal1_Label   = createBL( tech, 'Metal1_Label'  , BasicLayer.Material.info, gds2Layer=34, gds2DataType=10 )
    Metal2_Label   = createBL( tech, 'Metal2_Label'  , BasicLayer.Material.info, gds2Layer=36, gds2DataType=10 )
    Metal3_Label   = createBL( tech, 'Metal3_Label'  , BasicLayer.Material.info, gds2Layer=42, gds2DataType=10 )
    Metal4_Label   = createBL( tech, 'Metal4_Label'  , BasicLayer.Material.info, gds2Layer=46, gds2DataType=10 )
    Metal5_Label   = createBL( tech, 'Metal5_Label'  , BasicLayer.Material.info, gds2Layer=81, gds2DataType=10 )
    MetalTop_Label = createBL( tech, 'MetalTop_Label', BasicLayer.Material.info, gds2Layer=53, gds2DataType=10 )

    Metal1_BLK   = createBL( tech, 'Metal1_BLK'  , BasicLayer.Material.blockage, gds2Layer=34, gds2DataType=5 )
    Metal2_BLK   = createBL( tech, 'Metal2_BLK'  , BasicLayer.Material.blockage, gds2Layer=36, gds2DataType=5 )
    Metal3_BLK   = createBL( tech, 'Metal3_BLK'  , BasicLayer.Material.blockage, gds2Layer=42, gds2DataType=5 )
    Metal4_BLK   = createBL( tech, 'Metal4_BLK'  , BasicLayer.Material.blockage, gds2Layer=46, gds2DataType=5 )
    Metal5_BLK   = createBL( tech, 'Metal5_BLK'  , BasicLayer.Material.blockage, gds2Layer=81, gds2DataType=5 )
    MetalTop_BLK = createBL( tech, 'MetalTop_BLK', BasicLayer.Material.blockage, gds2Layer=53, gds2DataType=5 )

    CONT = createVia( tech, 'CONT_POLY2', 'Poly2', 'Contact', 'Metal1', u(0.22) )
    setEnclosures( CONT, Poly2 , u(0.07) )
    setEnclosures( CONT, Metal1, u(0.12) )
    VIA12 = createVia( tech, 'VIA12', 'Metal1', 'Via1', 'Metal2', u(0.26) )
    setEnclosures( VIA12, Metal1, (u(0.06), u(0.00)) )
    setEnclosures( VIA12, Metal2, (u(0.06), u(0.01)) )
    VIA23 = createVia( tech, 'VIA23', 'Metal2', 'Via2', 'Metal3', u(0.26) )
    setEnclosures( VIA23, Metal2, (u(0.06), u(0.01)) )
    setEnclosures( VIA23, Metal3, (u(0.01), u(0.06)) )
    VIA34 = createVia( tech, 'VIA34', 'Metal3', 'Via3', 'Metal4', u(0.26) )
    setEnclosures( VIA34, Metal3, (u(0.01), u(0.06)) )
    setEnclosures( VIA34, Metal4, (u(0.06), u(0.01)) )
    VIA45 = createVia( tech, 'VIA45', 'Metal4', 'Via4', 'Metal5', u(0.26) )
    setEnclosures( VIA45, Metal4, (u(0.06), u(0.01)) )
    setEnclosures( VIA45, Metal5, (u(0.01), u(0.06)) )
    VIA5T = createVia( tech, 'VIA5T', 'Metal5', 'Via5', 'MetalTop', u(0.26) )
    setEnclosures( VIA5T, Metal5  , (u(0.01), u(0.06)) )
    setEnclosures( VIA5T, MetalTop, (u(0.06), u(0.01)) )

    Border = createBL( tech, 'Border', BasicLayer.Material.other, gds2Layer=63  )

    Metal1  .setBlockageLayer( Metal1_BLK )
    Metal2  .setBlockageLayer( Metal2_BLK )
    Metal3  .setBlockageLayer( Metal3_BLK )
    Metal4  .setBlockageLayer( Metal4_BLK )
    Metal5  .setBlockageLayer( Metal5_BLK )
    MetalTop.setBlockageLayer( MetalTop_BLK )

    # Coriolis internal layers
    createBL( tech, 'text.cell'    , BasicLayer.Material.other )
    createBL( tech, 'text.instance', BasicLayer.Material.other )
    createBL( tech, 'SPL1'         , BasicLayer.Material.other )
    createBL( tech, 'AutoLayer'    , BasicLayer.Material.other )
    createBL( tech, 'gmetalh'      , BasicLayer.Material.metal )
    createBL( tech, 'gcontact'     , BasicLayer.Material.cut   )
    createBL( tech, 'gmetalv'      , BasicLayer.Material.metal )


def _setup_display():
    # ----------------------------------------------------------------------
    # Style: Coriolis [black]

    threshold = 0.2 if Viewer.Graphics.isHighDpi() else 0.1
    scale     = 1.0

    style = Viewer.DisplayStyle( 'Coriolis [black]' )
    style.setDescription( 'Coriolis Look - black background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    # Viewer.
    style.addDrawingStyle( group='Viewer', name='fallback'      , color=toRGB('Gray238'    ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('Gray50'     ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'    , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'        , color=toRGB('192,0,192'  ), border=4, threshold=0.02 )
    style.addDrawingStyle( group='Viewer', name='phantom'       , color=toRGB('Seashell4'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries'    , color=toRGB('wheat1'     ), border=2, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'        , color=toRGB('80,250,80'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('White'      ), border=1, threshold=2.0 )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('White'      ), border=2, threshold=6.0 )
    style.addDrawingStyle( group='Viewer', name='ghost'         , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('White'      ), border=1, threshold=  0.0 )
    style.addDrawingStyle( group='Viewer', name='text.instance' , color=toRGB('White'      ), border=1, threshold=400.0 )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('White'      ), border=1, threshold=200.0 )
    style.addDrawingStyle( group='Viewer', name='undef'         , color=toRGB('Violet'     ), border=0, pattern='2244118822441188' )
    style.addDrawingStyle( group='Viewer', name='Border'        , color=toRGB('wheat1'     ), border=1, pattern='0000000000000000', threshold=0 )
    
    # Group: Active Layer.
    style.addDrawingStyle( group='Active Layer', name='Nwell'   , color=toRGB('Tan'        ), pattern='55AA55AA55AA55AA'   , threshold=1.5 *scale )
    style.addDrawingStyle( group='Active Layer', name='LVPwell' , color=toRGB('LightYellow'), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='Nplus'   , color=toRGB('LawnGreen'  ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='Pplus'   , color=toRGB('Yellow'     ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='COMP'    , color=toRGB('White'      ), pattern=toHexa('antihash1.8'), threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='Poly2'   , color=toRGB('Red'        ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    
    # Group: Routing Layer.
    style.addDrawingStyle( group='Routing Layer', name='Metal1'  , color=toRGB('Blue'     ), pattern=toHexa('poids2.8'         ), threshold=0.80*scale )
    style.addDrawingStyle( group='Routing Layer', name='Metal2'  , color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='Metal3'  , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='Metal4'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='Metal5'  , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='MetalTop', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    
    # Group: Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Contact', color=toRGB('0,150,150'), threshold=1.50*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via1'   , color=toRGB('Aqua'     ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via2'   , color=toRGB('LightPink'), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via3'   , color=toRGB('Green'    ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via4'   , color=toRGB('Yellow'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via5'   , color=toRGB('Violet'   ), threshold=0.80*scale )

    # Group: Fillers.
    style.addDrawingStyle( group='Fillers', name='Poly2_Dummy'   , color=toRGB('Red'      ), pattern='55AA55AA55AA55AA'         , border=0, threshold=1.50*scale )
    style.addDrawingStyle( group='Fillers', name='Metal1_Dummy'  , color=toRGB('Blue'     ), pattern=toHexa('poids2.8'         ), border=0, threshold=0.80*scale )
    style.addDrawingStyle( group='Fillers', name='Metal2_Dummy'  , color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Fillers', name='Metal3_Dummy'  , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Fillers', name='Metal4_Dummy'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Fillers', name='Metal5_Dummy'  , color=toRGB('Yellow'   ), pattern='1144114411441144'         , border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Fillers', name='MetalTop_Dummy', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), border=0, threshold=0.02*scale )
      
    # Group: Blockages.
    style.addDrawingStyle( group='Blockages', name='Metal1_BLK'  , color=toRGB('Blue'     ), pattern='006070381c0e0703'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='Metal2_BLK'  , color=toRGB('Aqua'     ), pattern='8103060c183060c0'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='Metal3_BLK'  , color=toRGB('LightPink'), pattern=toHexa('poids4.8'         ), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='Metal4_BLK'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='Metal5_BLK'  , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='MetalTop_BLK', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )

    # Group: Text.
    style.addDrawingStyle( group='Text', name='Poly2_Label'   , color=toRGB('Red'      ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Text', name='Metal1_Label'  , color=toRGB('Blue'     ), pattern=toHexa('poids2.8'         ), threshold=0.80*scale )
    style.addDrawingStyle( group='Text', name='Metal2_Label'  , color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Text', name='Metal3_Label'  , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Text', name='Metal4_Label'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Text', name='Metal5_Label'  , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.02*scale )
    style.addDrawingStyle( group='Text', name='MetalTop_Label', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )

    # Knick & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('antislash2.32'    ), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcontact'       , color=toRGB('255,255,190'),                                      border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000'         , border=4, threshold=0.02 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,255,190'), pattern='0000000000000000'         , border=2, threshold=threshold )

    Viewer.Graphics.addStyle( style )

    # ----------------------------------------------------------------------
    # Style: Alliance.Classic [white].

    style = Viewer.DisplayStyle( 'Coriolis [white]' )
    style.inheritFrom( 'Coriolis [black]' )
    style.setDescription ( 'Coriolis Look - white background' )
    style.setDarkening   ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    style.addDrawingStyle( group='Viewer', name='background', color=toRGB('White'), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground', color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries', color=toRGB('Black'), border=1, pattern='0000000000000000' )
    Viewer.Graphics.addStyle( style )

    # ----------------------------------------------------------------------
    # Style: Alliance.Classic [black]
    
    style = Viewer.DisplayStyle( 'Alliance.Classic [black]' )
    style.setDescription( 'Alliance Classic Look - black background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )

    # Viewer.
    style.addDrawingStyle( group='Viewer', name='fallback'      , color=toRGB('Gray238'    ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('Gray50'     ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'    , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'        , color=toRGB('192,0,192'  ), border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Viewer', name='phantom'       , color=toRGB('Seashell4'  ), border=1 )
   #style.addDrawingStyle( group='Viewer', name='boundaries'    , color=toRGB('208,199,192'), border=2, threshold=0 )
    style.addDrawingStyle( group='Viewer', name='boundaries'    , color=toRGB('wheat1')    , border=2, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'        , color=toRGB('80,250,80'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('White'      ), border=1, threshold=8.0*scale )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('White'      ), border=2, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='ghost'         , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('White'      ), border=1, threshold=  0.0*scale )
    style.addDrawingStyle( group='Viewer', name='text.instance' , color=toRGB('White'      ), border=1, threshold=400.0*scale )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('White'      ), border=1, threshold=200.0*scale )
    style.addDrawingStyle( group='Viewer', name='undef'         , color=toRGB('Violet'     ), border=0, pattern='2244118822441188' )
    style.addDrawingStyle( group='Viewer', name='Border'        , color=toRGB('wheat1'     ), border=1, pattern='0000000000000000', threshold=0 )

    # Active Layers.
    style.addDrawingStyle( group='Active Layers', name='Nwell'   , color=toRGB('Tan'        ), pattern=toHexa('urgo.8'     ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='LVPwell' , color=toRGB('LightYellow'), pattern=toHexa('urgo.8'     ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='Nplus'   , color=toRGB('LawnGreen'  ), pattern=toHexa('antihash0.8'), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='Pplus'   , color=toRGB('Yellow'     ), pattern=toHexa('antihash0.8'), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='COMP'    , color=toRGB('White'      ), pattern=toHexa('antihash1.8'), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='Poly2'   , color=toRGB('Red'        ), pattern=toHexa('poids2.8'   ), border=1, threshold=0.00*scale )
    
    # Routing Layers.
    style.addDrawingStyle( group='Routing Layers', name='Metal1'  , color=toRGB('Blue'         ), pattern=toHexa('slash.8'  ), border=1, threshold=0.80*scale )
    style.addDrawingStyle( group='Routing Layers', name='Metal2'  , color=toRGB('Aqua'         ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='Metal3'  , color=toRGB('LightPink'    ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='Metal4'  , color=toRGB('Green'        ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='Metal5'  , color=toRGB('Yellow'       ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='MetalTop', color=toRGB('Violet'       ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )

    # Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Contact', color=toRGB('0,150,150'), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via1'   , color=toRGB('Aqua'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via2'   , color=toRGB('LightPink'), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via3'   , color=toRGB('Green'    ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via4'   , color=toRGB('Yellow'   ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='Via5'   , color=toRGB('Violet'   ), threshold=0.0*scale )
    
    # Fillers.
    style.addDrawingStyle( group='Fillers', name='Poly2_Dummy'   , color=toRGB('Red'      ), pattern=toHexa('poids2.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Fillers', name='Metal1_Dummy'  , color=toRGB('Blue'     ), pattern=toHexa('slash.8'  ), border=0, threshold=0.80*scale )
    style.addDrawingStyle( group='Fillers', name='Metal2_Dummy'  , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Fillers', name='Metal3_Dummy'  , color=toRGB('LightPink'), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Fillers', name='Metal4_Dummy'  , color=toRGB('Green'    ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Fillers', name='Metal5_Dummy'  , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Fillers', name='MetalTop_Dummy', color=toRGB('Violet'   ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    
    # Labels.
    style.addDrawingStyle( group='Labels', name='Poly2_Label'   , color=toRGB('Red'      ), pattern=toHexa('poids2.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Labels', name='Metal1_Label'  , color=toRGB('Blue'     ), pattern=toHexa('slash.8'  ), border=0, threshold=0.80*scale )
    style.addDrawingStyle( group='Labels', name='Metal2_Label'  , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Labels', name='Metal3_Label'  , color=toRGB('LightPink'), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Labels', name='Metal4_Label'  , color=toRGB('Green'    ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Labels', name='Metal5_Label'  , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )
    style.addDrawingStyle( group='Labels', name='MetalTop_Label', color=toRGB('Violet'   ), pattern=toHexa('poids4.8' ), border=0, threshold=0.00*scale )

    # Blockages.
    style.addDrawingStyle( group='Blockages', name='Metal1_BLK'  , color=toRGB('Blue'     ), pattern=toHexa('light_antislash0.8'), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='Metal2_BLK'  , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='Metal3_BLK'  , color=toRGB('LightPink'), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='Metal4_BLK'  , color=toRGB('Green'    ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='Metal5_BLK'  , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='MetalTop_BLK', color=toRGB('Violet'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )

    # Knick & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gcontact'       , color=toRGB('255,255,190'),                                      border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('antislash2.32'    ), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcut'           , color=toRGB('255,255,190'),                                      border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000'         , border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,255,190'), pattern='0000000000000000'         , border=2, threshold=0.10*scale )
                          
    Viewer.Graphics.addStyle( style )
    Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )


def setup():
    _setup_techno()
    _setup_display()
    try:
        from .techno_fix import fix
    except:
        pass
    else:
        fix()

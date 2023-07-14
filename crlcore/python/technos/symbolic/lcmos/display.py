
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
# |  Python      :       "./etc/symbolic/lcmos/display.py"          |
# +-----------------------------------------------------------------+


from   coriolis.helpers    import truncPath
from   coriolis.helpers.io import vprint
import coriolis.Cfg     as Cfg
import coriolis.Viewer  as Viewer
from   coriolis.helpers                  import overlay, l, u, n
from   coriolis.technos.common.colors    import toRGB
from   coriolis.technos.common.patterns  import toHexa
vprint( 2, '     - "%s".' % truncPath(__file__) )


def createStyles ( scale=1.0 ):
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.viewer.minimumSize = 500 
        cfg.viewer.pixelThreshold = 5 

    # ----------------------------------------------------------------------
    # Style: Alliance.Coriolis [black].
    
    style = Viewer.DisplayStyle( 'Alliance.Coriolis [black]' )
    style.setDescription( 'Alliance Coriolis Look - black background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    style.addDrawingStyle( group='Viewer', name='fallback'       , color=toRGB('Gray238'    ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'     , color=toRGB('Gray50'     ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'     , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'         , color=toRGB('192,0,192'  ), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Viewer', name='phantom'        , color=toRGB('Seashell4'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries'     , color=toRGB('208,199,192'), border=1, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'         , color=toRGB('80,250,80'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw'  , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill'  , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'           , color=toRGB('White'      ), border=1, threshold=2.0*scale )
    style.addDrawingStyle( group='Viewer', name='spot'           , color=toRGB('White'      ), border=2, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='ghost'          , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'     , color=toRGB('White'      ), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Viewer', name='text.instance'  , color=toRGB('Black'      ), border=1, threshold=4.0*scale )
    style.addDrawingStyle( group='Viewer', name='text.reference' , color=toRGB('White'      ), border=1, threshold=20.0*scale )
    style.addDrawingStyle( group='Viewer', name='undef'          , color=toRGB('Violet'     ), border=0, pattern='2244118822441188' )
    style.addDrawingStyle( group='Viewer', name='mauka.container', color=toRGB('Magenta4'   ), border=4, pattern='0000000000000000', goMatched=False )
    
    # Group: Active Layer.
    style.addDrawingStyle( group='Active Layer', name='nWell'   , color=toRGB('Tan'        ), pattern='55AA55AA55AA55AA'   , threshold=1.5 *scale )
    style.addDrawingStyle( group='Active Layer', name='pWell'   , color=toRGB('LightYellow'), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='nImplant', color=toRGB('LawnGreen'  ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='pImplant', color=toRGB('Yellow'     ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='active'  , color=toRGB('White'      ), pattern=toHexa('antihash1.8'), threshold=1.50*scale )
    style.addDrawingStyle( group='Active Layer', name='poly'    , color=toRGB('Red'        ), pattern='55AA55AA55AA55AA'   , threshold=1.50*scale )
    
    # Group: Routing Layer.
    style.addDrawingStyle( group='Routing Layer', name='metal1' , color=toRGB('Blue'     ), pattern=toHexa('poids2.8'         ), threshold=0.80*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal2' , color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal3' , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal4' , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal5' , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal6' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal7' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal8' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal9' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal10', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.02*scale )
    
    # Group: Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut0', color=toRGB('0,150,150'), threshold=1.50*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut1', color=toRGB('Aqua'     ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut2', color=toRGB('LightPink'), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut3', color=toRGB('Green'    ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut4', color=toRGB('Yellow'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut5', color=toRGB('Violet'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut6', color=toRGB('Violet'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut7', color=toRGB('Violet'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut8', color=toRGB('Violet'   ), threshold=0.80*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut9', color=toRGB('Violet'   ), threshold=0.80*scale )
    
    # Group: MIM6.
    style.addDrawingStyle( group='MIM6', name='metbot_r', color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIM6', name='cut6'    , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIM6', name='metal7'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale )
      
    # Group: Blockages.
    style.addDrawingStyle( group='Blockages', name='blockage1' , color=toRGB('Blue'     ), pattern='006070381c0e0703'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage2' , color=toRGB('Aqua'     ), pattern='8103060c183060c0'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage3' , color=toRGB('LightPink'), pattern=toHexa('poids4.8'         ), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage4' , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage5' , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage6' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage7' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage8' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage9' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage10', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
      
    # Group: Knik & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('light_antihash0.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcut'           , color=toRGB('255,255,190'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000', threshold=0.02*scale, border=4 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,0,0'    ), pattern='0000000000000000', threshold=0.02*scale, border=4 )
    
    Viewer.Graphics.addStyle( style )
    
                          
    # ----------------------------------------------------------------------
    # Style: Alliance.Coriolis [white].
    
    style = Viewer.DisplayStyle( 'Alliance.Coriolis [white]' )
    style.inheritFrom( 'Alliance.Coriolis [black]' )
    style.setDescription( 'Alliance Coriolis Look - white background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    style.addDrawingStyle( group='Viewer', name='fallback'      , color=toRGB('Gray238'    ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('Gray50'     ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'    , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'        , color=toRGB('192,0,192'  ), border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Viewer', name='phantom'       , color=toRGB('Seashell4'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries'    , color=toRGB('208,199,192'), border=1, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'        , color=toRGB('80,250,80'  ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill' , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('White'      ), border=1, threshold=2.0*scale )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('White'      ), border=2, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='ghost'         , color=toRGB('White'      ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('White'      ), border=1, threshold=0.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.instance' , color=toRGB('White'      ), border=1, threshold=400.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('White'      ), border=1, threshold=200.0*scale )
    style.addDrawingStyle( group='Viewer', name='undef'         , color=toRGB('Violet'     ), border=0, pattern='2244118822441188' )
    
    # Active Layers.
    style.addDrawingStyle( group='Active Layer', name='nWell'   , color=toRGB('Tan'        ), pattern=toHexa('urgo.8'     ), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='pWell'   , color=toRGB('LightYellow'), pattern=toHexa('urgo.8'     ), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='nImplant', color=toRGB('LawnGreen'  ), pattern=toHexa('antihash0.8'), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='pImplant', color=toRGB('Yellow'     ), pattern=toHexa('antihash0.8'), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='active'  , color=toRGB('White'      ), pattern=toHexa('antihash1.8'), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='poly'    , color=toRGB('Red'        ), pattern=toHexa('poids2.8'   ), border=1, threshold=0*scale )
    style.addDrawingStyle( group='Active Layer', name='poly2'   , color=toRGB('Orange'     ), pattern=toHexa('poids2.8'   ), border=1, threshold=0*scale )
    
    # Routing Layers.
    style.addDrawingStyle( group='Routing Layer', name='metal1' , color=toRGB('Blue'         ), pattern=toHexa('slash.8' ), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal2' , color=toRGB('Aqua'         ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metcap' , color=toRGB('DarkTurquoise'), pattern=toHexa('poids2.8'), border=2, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal3' , color=toRGB('LightPink'    ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal4' , color=toRGB('Green'        ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal5' , color=toRGB('Yellow'       ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal6' , color=toRGB('Violet'       ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal7' , color=toRGB('Red'          ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal8' , color=toRGB('Blue'         ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal9' , color=toRGB('Blue'         ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    style.addDrawingStyle( group='Routing Layer', name='metal10', color=toRGB('Blue'         ), pattern=toHexa('poids4.8'), border=1, threshold=0.0*scale )
    
     
    # Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut0', color=toRGB('0,150,150'), threshold=0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut1', color=toRGB('Aqua'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut2', color=toRGB('LightPink'), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut3', color=toRGB('Green'    ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut4', color=toRGB('Yellow'   ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut5', color=toRGB('Violet'   ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut6', color=toRGB('Red'      ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut7', color=toRGB('Blue'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut8', color=toRGB('Blue'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut9', color=toRGB('Blue'     ), threshold=0.0*scale )
     
    # MIM6.
    style.addDrawingStyle( group='MIM6', name='metbot_r', color=toRGB('Aqua' ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIM6', name='metal7'  , color=toRGB('Green'), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale )
     
    # Blockages.
    style.addDrawingStyle( group='Blockages', name='blockage1' , color=toRGB('Blue'     ), pattern=toHexa('light_antislash0.8'), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage2' , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage3' , color=toRGB('LightPink'), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage4' , color=toRGB('Green'    ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage5' , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage6' , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage7' , color=toRGB('Red'      ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage8' , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage9' , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage10', color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    
    # Knick & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('antislash2.32'    ), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcut'           , color=toRGB('255,255,190'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000', border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,255,190'), pattern='0000000000000000', border=2, threshold=0.10*scale )
                          
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

    # Active Layers.
    style.addDrawingStyle( group='Active Layers', name='nWell'   , color=toRGB('Tan'        ), pattern=toHexa('urgo.8'     ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='pWell'   , color=toRGB('LightYellow'), pattern=toHexa('urgo.8'     ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='nImplant', color=toRGB('LawnGreen'  ), pattern=toHexa('antihash0.8'), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='pImplant', color=toRGB('Yellow'     ), pattern=toHexa('antihash0.8'), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='active'  , color=toRGB('White'      ), pattern='0000000000000000'   , border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='poly'    , color=toRGB('Red'        ), pattern=toHexa('poids2.8'   ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Active Layers', name='poly2'   , color=toRGB('Orange'     ), pattern=toHexa('poids2.8'   ), border=1, threshold=0.00*scale )
    
    # Routing Layers.
    style.addDrawingStyle( group='Routing Layers', name='metal1' , color=toRGB('Blue'         ), pattern=toHexa('slash.8'  ), border=1, threshold=0.80*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal2' , color=toRGB('Aqua'         ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metcap' , color=toRGB('DarkTurquoise'), pattern=toHexa('poids2.8' ), border=2, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal3' , color=toRGB('LightPink'    ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal4' , color=toRGB('Green'        ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal5' , color=toRGB('Yellow'       ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal6' , color=toRGB('Violet'       ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal7' , color=toRGB('Red'          ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal8' , color=toRGB('Blue'         ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal9' , color=toRGB('Blue'         ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal10', color=toRGB('Blue'         ), pattern=toHexa('poids4.8' ), border=1, threshold=0.00*scale )

    # Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut0', color=toRGB('0,150,150'), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut1', color=toRGB('Aqua'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut2', color=toRGB('LightPink'), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut3', color=toRGB('Green'    ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut4', color=toRGB('Yellow'   ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut5', color=toRGB('Violet'   ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut6', color=toRGB('Red'      ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut7', color=toRGB('Blue'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut8', color=toRGB('Blue'     ), threshold=0.0*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut9', color=toRGB('Blue'     ), threshold=0.0*scale )

    # MIM6.
    style.addDrawingStyle( group='MIMI6', name='metbot_r', color=toRGB('Aqua' ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIMI6', name='metal7'  , color=toRGB('Green'), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale )

    # Blockages.
    style.addDrawingStyle( group='Blockages', name='blockage1' , color=toRGB('Blue'     ), pattern=toHexa('light_antislash0.8'), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage2' , color=toRGB('Aqua'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage3' , color=toRGB('LightPink'), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage4' , color=toRGB('Green'    ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage5' , color=toRGB('Yellow'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage6' , color=toRGB('Violet'   ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage7' , color=toRGB('Red'      ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage8' , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage9' , color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )
    style.addDrawingStyle( group='Blockages', name='blockage10', color=toRGB('Blue'     ), pattern=toHexa('poids4.8'          ), threshold=0.80*scale, border=4 )

    # Knick & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('antislash2.32'    ), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcut'           , color=toRGB('255,255,190'),                                      border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000'         , border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('255,255,190'), pattern='0000000000000000'         , border=2, threshold=0.10*scale )
                          
    Viewer.Graphics.addStyle( style )

                          
    # ----------------------------------------------------------------------
    # Style: Alliance.Classic [white].
    
    style = Viewer.DisplayStyle( 'Alliance.Classic [white]' )
    style.inheritFrom( 'Alliance.Classic [black]' )
    style.setDescription( 'Alliance Classic Look - white background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    # Group: Viewer.
    style.addDrawingStyle( group='Viewer', name='fallback'      , color=toRGB('Black'), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('White'), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'    , color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw' , color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill' , color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('Black'), border=1, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('Black'), border=1, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='ghost'         , color=toRGB('Black'), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('Black'), border=1, threshold=0.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.instance' , color=toRGB('Black'), border=1, threshold=4.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('Black'), border=1, threshold=20.0*scale )
    style.addDrawingStyle( group='Viewer', name='undef'         , color=toRGB('Black'), border=0, pattern='2244118822441188' )
                          
    Viewer.Graphics.addStyle( style )
    
                          
    # ----------------------------------------------------------------------
    # Style: Layout Design [black]
    
    style = Viewer.DisplayStyle( 'Layout Design [black]' )
    style.inheritFrom( 'Alliance.Classic [black]' )
    style.setDescription( 'Alliance Classic Look - white background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    # Active Layers.
    style.addDrawingStyle( group='Active Layers', name='nWell'   , color=toRGB('Tan'        ), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    style.addDrawingStyle( group='Active Layers', name='pWell'   , color=toRGB('LightYellow'), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    style.addDrawingStyle( group='Active Layers', name='nImplant', color=toRGB('LawnGreen'  ), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    style.addDrawingStyle( group='Active Layers', name='pImplant', color=toRGB('Yellow'     ), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    style.addDrawingStyle( group='Active Layers', name='active'  , color=toRGB('White'      ), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    style.addDrawingStyle( group='Active Layers', name='poly'    , color=toRGB('Red'        ), pattern='0000000000000000', threshold=1.50*scale, border=2 )
    
    # Routing Layers.
    style.addDrawingStyle( group='Routing Layers', name='metal1' , color=toRGB('Blue'     ), pattern='0000000000000000', threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal2' , color=toRGB('Aqua'     ), pattern='0000000000000000', threshold=0.40*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal3' , color=toRGB('LightPink'), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal4' , color=toRGB('Green'    ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal5' , color=toRGB('Yellow'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal6' , color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal7' , color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal8' , color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal9' , color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    style.addDrawingStyle( group='Routing Layers', name='metal10', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.02*scale, border=2 )
    
    # Cuts (VIA holes).
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut0', color=toRGB('0,150,150'), pattern=toHexa('poids4.8'), threshold=1.50*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut1', color=toRGB('Aqua'     ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut2', color=toRGB('LightPink'), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut3', color=toRGB('Green'    ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut4', color=toRGB('Yellow'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut5', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut6', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut7', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut8', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut9', color=toRGB('Violet'   ), pattern='0000000000000000', threshold=0.80*scale, border=1 )
                          
    Viewer.Graphics.addStyle( style )
    
                          
    # ----------------------------------------------------------------------
    # Style: Layout Design [white]
    
    style = Viewer.DisplayStyle( 'Layout Design [white]' )
    style.inheritFrom( 'Layout Design [black]' )
    style.setDescription( 'Layout Design Look - white background' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    
    # Group: Viewer.
    style.addDrawingStyle( group='Viewer', name='background'    , color=toRGB('White'), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'          , color=toRGB('Black'), border=1, threshold=2.0 *scale )
    style.addDrawingStyle( group='Viewer', name='spot'          , color=toRGB('Black'), border=1, threshold=2.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.ruler'    , color=toRGB('Black'), border=1, threshold=0.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.reference', color=toRGB('Black'), border=1, threshold=20.0*scale )
    
    # Group: Active Layers.
    style.addDrawingStyle( group='Active Layers', name='active', color=toRGB('175,175,175'), pattern='0000000000000000', threshold=1.50*scale, border=2 )
                          
    Viewer.Graphics.addStyle( style )
    
                          
    # ----------------------------------------------------------------------
    # Style: For Printers [white]
    
    style = Viewer.DisplayStyle( 'For Printers' )
    style.setDescription( 'For Printers' )
    style.setDarkening  ( Viewer.DisplayStyle.HSVr(1.0, 3.0, 2.5) )
    
    # Group: Viewer.
    style.addDrawingStyle( group='Viewer', name='fallback'       , color=toRGB('Gray238'  ), border=1, pattern='55AA55AA55AA55AA' )
    style.addDrawingStyle( group='Viewer', name='background'     , color=toRGB('White'    ), border=1 )
    style.addDrawingStyle( group='Viewer', name='foreground'     , color=toRGB('Black'    ), border=1 )
    style.addDrawingStyle( group='Viewer', name='rubber'         , color=toRGB('192,0,192'), border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Viewer', name='phantom'        , color=toRGB('Seashell4'), border=1 )
    style.addDrawingStyle( group='Viewer', name='boundaries'     , color=toRGB('Black'    ), border=1, pattern='0000000000000000', threshold=0 )
    style.addDrawingStyle( group='Viewer', name='marker'         , color=toRGB('80,250,80'), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionDraw'  , color=toRGB('Black'    ), border=1 )
    style.addDrawingStyle( group='Viewer', name='selectionFill'  , color=toRGB('Black'    ), border=1 )
    style.addDrawingStyle( group='Viewer', name='grid'           , color=toRGB('Black'    ), border=1, threshold=2.0*scale )
    style.addDrawingStyle( group='Viewer', name='spot'           , color=toRGB('Black'    ), border=2, threshold=6.0*scale )
    style.addDrawingStyle( group='Viewer', name='ghost'          , color=toRGB('Black'    ), border=1 )
    style.addDrawingStyle( group='Viewer', name='text.ruler'     , color=toRGB('Black'    ), border=1, threshold=0.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.instance'  , color=toRGB('Black'    ), border=1, threshold=4.0 *scale )
    style.addDrawingStyle( group='Viewer', name='text.reference' , color=toRGB('Black'    ), border=1, threshold=20.0*scale )
    style.addDrawingStyle( group='Viewer', name='undef'          , color=toRGB('Violet'   ), border=0, pattern='2244118822441188' )
    style.addDrawingStyle( group='Viewer', name='mauka.container', color=toRGB('Magenta4' ), border=4, pattern='0000000000000000', goMatched=False )
    
    # Group: Active Layers.
    style.addDrawingStyle( group='Active Layers', name='nWell'   , color=toRGB('Tan'        ), pattern=toHexa('urgo.32'      ), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='pWell'   , color=toRGB('LightYellow'), pattern=toHexa('antipoids2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='nImplant', color=toRGB('LawnGreen'  ), pattern=toHexa('diffusion.32' ), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='pImplant', color=toRGB('Yellow'     ), pattern=toHexa('diffusion.32' ), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='active'  , color=toRGB('White'      ), pattern=toHexa('active.32'    ), border=0, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='poly'    , color=toRGB('Red'        ), pattern=toHexa('antipoids2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Active Layers', name='poly2'   , color=toRGB('Orange'     ), pattern=toHexa('antipoids2.32'), border=1, threshold=0.02*scale )
    
    # Group: Routing Layers.
    style.addDrawingStyle( group='Routing Layers', name='metal1' , color=toRGB('Blue'         ), pattern=toHexa('slash.32'     ), border=4, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal2' , color=toRGB('Aqua'         ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metcap' , color=toRGB('DarkTurquoise'), pattern=toHexa('poids2.32'    ), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal3' , color=toRGB('LightPink'    ), pattern=toHexa('antislash3.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal4' , color=toRGB('Green'        ), pattern=toHexa('antislash4.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal5' , color=toRGB('Yellow'       ), pattern=toHexa('antislash5.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal6' , color=toRGB('Violet'       ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal7' , color=toRGB('Violet'       ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal8' , color=toRGB('Violet'       ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal9' , color=toRGB('Violet'       ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    style.addDrawingStyle( group='Routing Layers', name='metal10', color=toRGB('Violet'       ), pattern=toHexa('antislash2.32'), border=1, threshold=0.02*scale )
    
    # Group: Cuts (VIA holes)
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut0', color=toRGB('Blue'     ), pattern=toHexa('poids2.8'    ), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut1', color=toRGB('Aqua'     ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut2', color=toRGB('LightPink'), pattern=toHexa('poids2.8'    ), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut3', color=toRGB('Green'    ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut4', color=toRGB('Yellow'   ), pattern=toHexa('poids2.8'    ), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut5', color=toRGB('Violet'   ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut6', color=toRGB('Violet'   ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut7', color=toRGB('Violet'   ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut8', color=toRGB('Violet'   ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    style.addDrawingStyle( group='Cuts (VIA holes)', name='cut9', color=toRGB('Violet'   ), pattern=toHexa('antipoids2.8'), border=2, threshold=0.02*scale )
    
    # Group: MIM6.
    style.addDrawingStyle( group='MIM6', name='metbot_r', color=toRGB('Aqua'     ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIM6', name='cut6'    , color=toRGB('LightPink'), pattern=toHexa('light_antihash1.8'), threshold=0.80*scale )
    style.addDrawingStyle( group='MIM6', name='metal7'  , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale )
    
    # Group: Blockages.
    style.addDrawingStyle( group='Blockages', name='blockage1' , color=toRGB('Blue'     ), pattern='006070381c0e0703'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage2' , color=toRGB('Aqua'     ), pattern='8103060c183060c0'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage3' , color=toRGB('LightPink'), pattern=toHexa('poids4.8'         ), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage4' , color=toRGB('Green'    ), pattern=toHexa('light_antihash2.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage5' , color=toRGB('Yellow'   ), pattern='1144114411441144'         , threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage6' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage7' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage8' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage9' , color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    style.addDrawingStyle( group='Blockages', name='blockage10', color=toRGB('Violet'   ), pattern=toHexa('light_antihash0.8'), threshold=0.80*scale, border=2 )
    
    # Group: Knik & Kite.
    style.addDrawingStyle( group='Knik & Kite', name='SPL1'           , color=toRGB('Red'        ) )
    style.addDrawingStyle( group='Knik & Kite', name='AutoLayer'      , color=toRGB('Magenta'    ) )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalh'        , color=toRGB('128,255,200'), pattern=toHexa('light_antihash0.8') , border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gmetalv'        , color=toRGB('200,200,255'), pattern=toHexa('light_antihash1.8') , border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='gcut'           , color=toRGB('255,255,190'), border=1 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::Edge' , color=toRGB('255,255,190'), pattern='0000000000000000', border=2 )
    style.addDrawingStyle( group='Knik & Kite', name='Anabatic::GCell', color=toRGB('Black'      ), pattern='0000000000000000', border=2, threshold=0.80*scale )
    
    Viewer.Graphics.addStyle( style )


    Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )

createStyles( scale=1.0 )

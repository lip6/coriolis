
import Viewer

# Black & White.
Black       = "0,0,0"
Gray50      = "50,50,50"
Gray238     = "238,238,238"
White       = "255,255,255"
Seashell4   = "139,134,130"

# Shade of Reds.
Red         = "255,0,0"
LightPink   = "255,182,193"

# Shade of Green.
Green       = "0,255,0"
LawnGreen   = "124,252,0"

# Shade of Blue.
Blue        = "0,0,255"
Aqua        = "0,255,255"

# Shade of Violets.
Violet      = "238;130;238"
Magenta     = "255,0,255"
Magenta4    = "139,0,139"

# Shade of Browns
Tan         = "210,180,140"

# Shade of Yellows.
Yellow      = "255,255,0"
LightYellow = "255,255,224"

# Patterns.
Urgo = "feffffffefffffff"


# =============================================================================
# Aliance.Coriolis [black]

def loadAllianceCoriolisBlackStyle ():
  style = Viewer.DisplayStyle("Alliance.Coriolis [black]")
  style.setDescription("Alliance Coriolis Look - black background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))
  
  # Viewer group.
  style.addDrawingStyle(group="Viewer", name="fallback"       , color=Gray238      , border=1, pattern="55AA55AA55AA55AA")
  style.addDrawingStyle(group="Viewer", name="background"     , color=Gray50       , border=1)
  style.addDrawingStyle(group="Viewer", name="foreground"     , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="rubber"         , color="192,0,192"  , border=2, threshold=0.02)
  style.addDrawingStyle(group="Viewer", name="phantom"        , color=Seashell4    , border=1)
  style.addDrawingStyle(group="Viewer", name="boundaries"     , color="208,199,192", border=1, pattern="0000000000000000", threshold=0)
  style.addDrawingStyle(group="Viewer", name="marker"         , color="80,250,80"  , border=1)
  style.addDrawingStyle(group="Viewer", name="selectionDraw"  , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="selectionFill"  , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="grid"           , color=White        , border=1, threshold=2.0)
  style.addDrawingStyle(group="Viewer", name="spot"           , color=White        , border=2, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="ghost"          , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="text.ruler"     , color=White        , border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.instance"  , color=Black        , border=1, threshold=4.0)
  style.addDrawingStyle(group="Viewer", name="text.reference" , color=White        , border=1, threshold=20.0)
  style.addDrawingStyle(group="Viewer", name="undef"          , color=Violet       , border=0, pattern="2244118822441188")
  style.addDrawingStyle(group="Viewer", name="mauka.container", color=Magenta4     , border=4, pattern="0000000000000000", goMatched="false")
  
  # Active Layer group.
  style.addDrawingStyle(group="Active Layer", name="nWell"   , color=Tan        , pattern="55AA55AA55AA55AA", threshold=1.5)
  style.addDrawingStyle(group="Active Layer", name="pWell"   , color=LightYellow, pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layer", name="nImplant", color=LawnGreen  , pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layer", name="pImplant", color=Yellow     , pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layer", name="active"  , color=White      , pattern="bbddee77bbddee77", threshold=1.50)
  style.addDrawingStyle(group="Active Layer", name="poly"    , color=Red        , pattern="55AA55AA55AA55AA", threshold=1.50)
  
  # Routing Layer group.
  style.addDrawingStyle(group="Routing Layer", name="metal1" , color=Blue     , pattern="AA55AA55AA55AA55", threshold=0.80)
  style.addDrawingStyle(group="Routing Layer", name="metal2" , color=Aqua     , pattern="8822882288228822", threshold=0.40)
  style.addDrawingStyle(group="Routing Layer", name="metal3" , color=LightPink, pattern="4411441144114411", threshold=0.02)
  style.addDrawingStyle(group="Routing Layer", name="metal4" , color=Green    , pattern="2288228822882288", threshold=0.02)
  style.addDrawingStyle(group="Routing Layer", name="metal5" , color=Yellow   , pattern="1144114411441144", threshold=0.02)
  style.addDrawingStyle(group="Routing Layer", name="metal6" , color=Violet   , pattern="8822882288228822", threshold=0.02)
  
  # Cuts (VIA holes) group.
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut0", color="0,150,150", threshold=1.50)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut1", color=Aqua       , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut2", color=LightPink  , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut3", color=Green      , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut4", color=Yellow     , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut5", color=Violet     , threshold=0.80)
  
  # MIM6 group.
  style.addDrawingStyle(group="MIM6", name="topmim6", color=Blue     , pattern="AA55AA55AA55AA55", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="botmim6", color=Aqua     , pattern="8822882288228822", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="padopen", color=LightPink, pattern="4411441144114411", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="alucap" , color=Green    , pattern="2288228822882288", threshold=0.80)
  
  # Blockages group.
  style.addDrawingStyle(group="Blockages", name="blockage1", color=Blue     , pattern="006070381c0e0703", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage2", color=Aqua     , pattern="8103060c183060c0", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage3", color=LightPink, pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage4", color=Green    , pattern="2288228822882288", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage5", color=Yellow   , pattern="1144114411441144", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage6", color=Violet   , pattern="8822882288228822", threshold=0.80, border=2)
  
  # Knik & Kite group.
  style.addDrawingStyle(group="Knik & Kite", name="SPL1"     , color=Red)
  style.addDrawingStyle(group="Knik & Kite", name="AutoLayer", color=Magenta)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalh"  , color="128,255,200", pattern="8822882288228822", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalv"  , color="200,200,255", pattern="4411441144114411", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gcut"     , color="255,255,190", border=1)
  
  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Aliance.Coriolis [white]

def loadAllianceCoriolisWhiteStyle ():
  style = Viewer.DisplayStyle("Alliance.Coriolis [white]")
  style.setDescription("Alliance Coriolis Look - white background")
  style.inheritFrom("Alliance.Coriolis [black]")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  style.addDrawingStyle(group="Viewer", name="fallback"     , color=Black, border=1, pattern="55AA55AA55AA55AA")
  style.addDrawingStyle(group="Viewer", name="background"   , color=White, border=1)
  style.addDrawingStyle(group="Viewer", name="foreground"   , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="selectionDraw", color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="selectionFill", color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="grid"         , color=Black, border=1, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="spot"         , color=Black, border=1, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="ghost"        , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="text.ruler"   , color=Black, border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.instance", color=Black, border=1, threshold=4.0)
  style.addDrawingStyle(group="Viewer", name="undef"        , color=Black, border=0, pattern="2244118822441188")

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Aliance.Classic [black]

def loadAllianceClassicBlackStyle ():
  style = Viewer.DisplayStyle("Alliance.Classic [black]")
  style.setDescription("Alliance Standard Look - black background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Viewer.
  style.addDrawingStyle(group="Viewer", name="fallback"      , color=Gray238      , border=1, pattern="55AA55AA55AA55AA")
  style.addDrawingStyle(group="Viewer", name="background"    , color=Gray50       , border=1)
  style.addDrawingStyle(group="Viewer", name="foreground"    , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="rubber"        , color="192,0,192"  , border=4, threshold=0.02)
  style.addDrawingStyle(group="Viewer", name="phantom"       , color=Seashell4    , border=1)
  style.addDrawingStyle(group="Viewer", name="boundaries"    , color="208,199,192", border=1, pattern="0000000000000000", threshold=0)
  style.addDrawingStyle(group="Viewer", name="marker"        , color="80,250,80"  , border=1)
  style.addDrawingStyle(group="Viewer", name="selectionDraw" , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="selectionFill" , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="grid"          , color=White        , border=1, threshold=2.0)
  style.addDrawingStyle(group="Viewer", name="spot"          , color=White        , border=2, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="ghost"         , color=White        , border=1)
  style.addDrawingStyle(group="Viewer", name="text.ruler"    , color=White        , border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.instance" , color=White        , border=1, threshold=4.0)
  style.addDrawingStyle(group="Viewer", name="text.reference", color=White        , border=1, threshold=20.0)
  style.addDrawingStyle(group="Viewer", name="undef"         , color=Violet       , border=0, pattern="2244118822441188")

  # Active Layers.
  style.addDrawingStyle(group="Active Layers", name="nWell"   , color=Tan        , pattern=Urgo, border=1, threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="pWell"   , color=LightYellow, pattern=Urgo, border=1, threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="nImplant", color=LawnGreen  , pattern="77bbddee77bbddee", border=1, threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="pImplant", color=Yellow     , pattern="77bbddee77bbddee", border=1, threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="active"  , color=White      , pattern="bbddee77bbddee77", border=1, threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="poly"    , color=Red        , pattern="aa55aa55aa55aa55", border=1, threshold=1.50)

  # Routing Layers.
  style.addDrawingStyle(group="Routing Layers", name="metal1", color=Blue     , pattern="8844221188442211", border=1, threshold=0.80)
  style.addDrawingStyle(group="Routing Layers", name="metal2", color=Aqua     , pattern="8800220088002200", border=1, threshold=0.40)
  style.addDrawingStyle(group="Routing Layers", name="metal3", color=LightPink, pattern="8800220088002200", border=1, threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal4", color=Green    , pattern="8800220088002200", border=1, threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal5", color=Yellow   , pattern="8800220088002200", border=1, threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal6", color=Violet   , pattern="8800220088002200", border=1, threshold=0.02)

  # Cuts (VIA holes).
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut0", color="0,150,150", threshold=1.50)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut1", color=Aqua       , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut2", color=LightPink  , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut3", color=Green      , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut4", color=Yellow     , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut5", color=Violet     , threshold=0.80)

  # MIM6.
  style.addDrawingStyle(group="MIM6", name="topmim6", color=Blue     , pattern="AA55AA55AA55AA55", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="botmim6", color=Aqua     , pattern="8822882288228822", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="padopen", color=LightPink, pattern="4411441144114411", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="alucap" , color=Green    , pattern="2288228822882288", threshold=0.80)

  # Blockages.
  style.addDrawingStyle(group="Blockages", name="blockage1", color=Blue     , pattern="8844221188442211", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage2", color=Aqua     , pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage3", color=LightPink, pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage4", color=Green    , pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage5", color=Yellow   , pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage6", color=Violet   , pattern="8800220088002200", threshold=0.80, border=2)

  # Knik & Kite.
  style.addDrawingStyle(group="Knik & Kite", name="SPL1"     , color=Red)
  style.addDrawingStyle(group="Knik & Kite", name="AutoLayer", color=Magenta)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalh"  , color="128,255,200", pattern="8822882288228822", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalv"  , color="200,200,255", pattern="4411441144114411", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gcut"     , color="255,255,190", border=1)

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Aliance.Classic [white]

def loadAllianceClassicWhiteStyle ():
  style = Viewer.DisplayStyle("Alliance.Classic [white]")
  style.inheritFrom("Alliance.Classic [black]")
  style.setDescription("Alliance Standard Look - white background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Viewer.
  style.addDrawingStyle(group="Viewer", name="fallback"      , color=Black, border=1, pattern="55AA55AA55AA55AA")
  style.addDrawingStyle(group="Viewer", name="background"    , color=White, border=1)
  style.addDrawingStyle(group="Viewer", name="foreground"    , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="selectionDraw" , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="selectionFill" , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="grid"          , color=Black, border=1, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="spot"          , color=Black, border=1, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="ghost"         , color=Black, border=1)
  style.addDrawingStyle(group="Viewer", name="text.ruler"    , color=Black, border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.instance" , color=Black, border=1, threshold=4.0)
  style.addDrawingStyle(group="Viewer", name="text.reference", color=Black, border=1, threshold=20.0)
  style.addDrawingStyle(group="Viewer", name="undef"         , color=Black, border=0, pattern="2244118822441188")

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# ISPD Global Route.

def loadIspdGlobalRouteBlackStyle ():
  style = Viewer.DisplayStyle("Ispd Global Route")
  style.inheritFrom("Alliance.Classic [black]")
  style.setDescription("ISPD Global Route Look -black background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Viewer.
  style.addDrawingStyle(group="Viewer", name="background", color=Black      , border=1)
  style.addDrawingStyle(group="Viewer", name="rubber"    , color="192,0,192", border=1, threshold=0)

  # Knik & Kite.
  style.addDrawingStyle(group="Knik & Kite", name="gmetalh" , color="128,255,200", pattern="8822882288228822", border=1, threshold=0)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalv" , color="200,200,255", pattern="4411441144114411", border=1, threshold=0)
  style.addDrawingStyle(group="Knik & Kite", name="gcontact", color="255,255,190", border=1, threshold=0)

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Layout Design [black].

def loadLayoutDesignBlackStyle ():
  style = Viewer.DisplayStyle("Layout Design")
  style.inheritFrom("Alliance.Classic [black]")
  style.setDescription("Useful for debugging layout - black background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Active Layers.
  style.addDrawingStyle(group="Active Layers", name="nWell"   , color=Tan        , pattern="0000000000000000", threshold=1.50, border=2)
  style.addDrawingStyle(group="Active Layers", name="pWell"   , color=LightYellow, pattern="0000000000000000", threshold=1.50, border=2)
  style.addDrawingStyle(group="Active Layers", name="nImplant", color=LawnGreen  , pattern="0000000000000000", threshold=1.50, border=2)
  style.addDrawingStyle(group="Active Layers", name="pImplant", color=Yellow     , pattern="0000000000000000", threshold=1.50, border=2)
  style.addDrawingStyle(group="Active Layers", name="active"  , color=White      , pattern="0000000000000000", threshold=1.50, border=2)
  style.addDrawingStyle(group="Active Layers", name="poly"    , color=Red        , pattern="0000000000000000", threshold=1.50, border=2)

  # Routing Layers.
  style.addDrawingStyle(group="Routing Layers", name="metal1", color=Blue     , pattern="0000000000000000", threshold=0.80, border=2)
  style.addDrawingStyle(group="Routing Layers", name="metal2", color=Aqua     , pattern="0000000000000000", threshold=0.40, border=2)
  style.addDrawingStyle(group="Routing Layers", name="metal3", color=LightPink, pattern="0000000000000000", threshold=0.02, border=2)
  style.addDrawingStyle(group="Routing Layers", name="metal4", color=Green    , pattern="0000000000000000", threshold=0.02, border=2)
  style.addDrawingStyle(group="Routing Layers", name="metal5", color=Yellow   , pattern="0000000000000000", threshold=0.02, border=2)
  style.addDrawingStyle(group="Routing Layers", name="metal6", color=Violet   , pattern="0000000000000000", threshold=0.02, border=2)

  # Cuts (VIA holes)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut0", color="0,150,150", pattern="8800220088002200", threshold=1.50, border=1)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut1", color=Aqua       , pattern="0000000000000000", threshold=0.80, border=1)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut2", color=LightPink  , pattern="0000000000000000", threshold=0.80, border=1)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut3", color=Green      , pattern="0000000000000000", threshold=0.80, border=1)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut4", color=Yellow     , pattern="0000000000000000", threshold=0.80, border=1)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut5", color=Violet     , pattern="0000000000000000", threshold=0.80, border=1)

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Layout Design [white].

def loadLayoutDesignWhiteStyle ():
  style = Viewer.DisplayStyle("Layout Design White")
  style.inheritFrom("Layout Design")
  style.setDescription("Useful for debugging layout - white background")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Viewer.
  style.addDrawingStyle(group="Viewer", name="background"    , color=White, border=1)
  style.addDrawingStyle(group="Viewer", name="grid"          , color=Black, border=1, threshold=2.0)
  style.addDrawingStyle(group="Viewer", name="spot"          , color=Black, border=1, threshold=2.0)
  style.addDrawingStyle(group="Viewer", name="text.ruler"    , color=Black, border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.reference", color=Black, border=1, threshold=20.0)

  # Active Layers.
  style.addDrawingStyle(group="Active Layers", name="active", color="175,175,175", pattern="0000000000000000", threshold=1.50, border=2)

  Viewer.Graphics.addStyle ( style )
  return


# =============================================================================
# Layout Design [white].

def loadPrinterCoriolisStyle ():
  style = Viewer.DisplayStyle("Printer.Coriolis")
  style.setDescription("Coriolis Look for Printers")
  style.setDarkening(Viewer.DisplayStyle.HSVr( 1.0, 3.0, 2.5 ))

  # Viewer.
  style.addDrawingStyle(group="Viewer", name="fallback"       , color=Gray238    , border=1, pattern="55AA55AA55AA55AA")
  style.addDrawingStyle(group="Viewer", name="background"     , color=White      , border=1)
  style.addDrawingStyle(group="Viewer", name="foreground"     , color=Black      , border=1)
  style.addDrawingStyle(group="Viewer", name="rubber"         , color="192,0,192", border=4, threshold=0.02)
  style.addDrawingStyle(group="Viewer", name="phantom"        , color=Seashell4  , border=1)
  style.addDrawingStyle(group="Viewer", name="boundaries"     , color=Black      , border=1, pattern="0000000000000000", threshold=0)
  style.addDrawingStyle(group="Viewer", name="marker"         , color="80,250,80", border=1)
  style.addDrawingStyle(group="Viewer", name="selectionDraw"  , color=Black      , border=1)
  style.addDrawingStyle(group="Viewer", name="selectionFill"  , color=Black      , border=1)
  style.addDrawingStyle(group="Viewer", name="grid"           , color=Black      , border=1, threshold=2.0)
  style.addDrawingStyle(group="Viewer", name="spot"           , color=Black      , border=2, threshold=6.0)
  style.addDrawingStyle(group="Viewer", name="ghost"          , color=Black      , border=1)
  style.addDrawingStyle(group="Viewer", name="text.ruler"     , color=Black      , border=1, threshold=0.0)
  style.addDrawingStyle(group="Viewer", name="text.instance"  , color=Black      , border=1, threshold=4.0)
  style.addDrawingStyle(group="Viewer", name="text.reference" , color=Black      , border=1, threshold=20.0)
  style.addDrawingStyle(group="Viewer", name="undef"          , color=Violet     , border=0, pattern="2244118822441188")
  style.addDrawingStyle(group="Viewer", name="mauka.container", color=Magenta4   , border=4, pattern="0000000000000000", goMatched="false")

  # Active Layers.
  style.addDrawingStyle(group="Active Layers", name="nWell"   , color=Tan        , pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="pWell"   , color=LightYellow, pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="nImplant", color=LawnGreen  , pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="pImplant", color=Yellow     , pattern="55AA55AA55AA55AA", threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="active"  , color=White      , pattern="bbddee77bbddee77", threshold=1.50)
  style.addDrawingStyle(group="Active Layers", name="poly"    , color=Red        , pattern="55AA55AA55AA55AA", threshold=1.50)

  # Routing Layers.
  style.addDrawingStyle(group="Routing Layers", name="metal1", color=Black        , pattern="AA55AA55AA55AA55", threshold=0.80)
  style.addDrawingStyle(group="Routing Layers", name="metal2", color=Black        , pattern="8822882288228822", threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal3", color=Black        , pattern="4411441144114411", threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal4", color="127,127,127", pattern="2288228822882288", threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal5", color="127,127,127", pattern="1144114411441144", threshold=0.02)
  style.addDrawingStyle(group="Routing Layers", name="metal6", color=Black        , pattern="8822882288228822", threshold=0.02)

  # Cuts (VIA holes)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut0", color=Black          , threshold=1.50)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut1", color=Black          , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut2", color=Black          , threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut3", color="127,127,12  7", threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut4", color="127,127,12  7", threshold=0.80)
  style.addDrawingStyle(group="Cuts (VIA holes)", name="cut5", color=Black        , threshold=0.80)

  # MIM6.
  style.addDrawingStyle(group="MIM6", name="topmim6", color=Blue     , pattern="AA55AA55AA55AA55", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="botmim6", color=Aqua     , pattern="8822882288228822", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="padopen", color=LightPink, pattern="4411441144114411", threshold=0.80)
  style.addDrawingStyle(group="MIM6", name="alucap" , color=Green    , pattern="2288228822882288", threshold=0.80)

  # Blockages.
  style.addDrawingStyle(group="Blockages", name="blockage1", color=Blue     , pattern="006070381c0e0703", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage2", color=Aqua     , pattern="8103060c183060c0", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage3", color=LightPink, pattern="8800220088002200", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage4", color=Green    , pattern="2288228822882288", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage5", color=Yellow   , pattern="1144114411441144", threshold=0.80, border=2)
  style.addDrawingStyle(group="Blockages", name="blockage6", color=Violet   , pattern="8822882288228822", threshold=0.80, border=2)

  # Knik & Kite.
  style.addDrawingStyle(group="Knik & Kite", name="SPL1"     , color=Red)
  style.addDrawingStyle(group="Knik & Kite", name="AutoLayer", color=Magenta)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalh"  , color="128,255,200", pattern="8822882288228822", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gmetalv"  , color="200,200,255", pattern="4411441144114411", border=1)
  style.addDrawingStyle(group="Knik & Kite", name="gcut"     , color="255,255,190", border=1)

  Viewer.Graphics.addStyle ( style )
  return


print "          - Loading Display configuration."

#loadAllianceCoriolisBlackStyle()
#loadAllianceCoriolisWhiteStyle()
#loadAllianceClassicBlackStyle()
#loadAllianceClassicWhiteStyle()
#loadIspdGlobalRouteBlackStyle ()
#loadLayoutDesignBlackStyle()
#loadLayoutDesignWhiteStyle()
loadPrinterCoriolisStyle()

# Select the default style.
Viewer.Graphics.setStyle ( "Alliance.Classic [black]" )

#print 'AVAILABLE STYLES:'
#for style in Viewer.Graphics.getStyles():
#    print '    ', style.getName()
#
#style = Viewer.Graphics.getStyle("Alliance.Classic [white]")
#print style
#for dg in style.getDrawingGroups():
#    print dg.getName()
#    for ds in dg.getDrawingStyles():
#        print '    ', ds.getName(), ds.getPattern()

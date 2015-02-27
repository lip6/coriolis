
import Cfg

print "          - Loading Mauka configuration."
  
# Etesian parameters.
Cfg.getParamPercentage("etesian.aspectRatio" ).setPercentage(100      )
Cfg.getParamPercentage("etesian.spaceMargin" ).setPercentage(5      )
Cfg.getParamBool      ("etesian.uniformDensity").setBool      (False  )
Cfg.getParamBool      ("etesian.routingDriven").setBool      (False  )

layout = Cfg.Configuration.get().getLayout()
# Etesian tab layout.
layout.addTab       ( "Etesian", "etesian" )

layout.addTitle     ( "Etesian", "Placement area" )
layout.addParameter ( "Etesian", "etesian.aspectRatio"      , "Aspect Ratio, X/Y (%)", 0 )
layout.addParameter ( "Etesian", "etesian.spaceMargin"      , "Space Margin"         , 1 )
layout.addRule      ( "Etesian" )

layout.addTitle     ( "Etesian", "Etesian - Placer" )
layout.addParameter ( "Etesian", "etesian.uniformDensity"   , "Occupy whole placement area"  , 0 )
layout.addParameter ( "Etesian", "etesian.routingDriven"    , "Routing driven"               , 0 )
layout.addParameter ( "Etesian", "etesian.effort"           , "Placement effort"             , 1 )
layout.addParameter ( "Etesian", "etesian.graphics"         , "Placement view"               , 1 )
layout.addRule      ( "Etesian" )


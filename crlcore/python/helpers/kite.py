
import Cfg

print "          - Loading Katabatic & Kite configuration."
 
# Katabatic parameters.
Cfg.getParamInt       ("katabatic.globalLengthThreshold").setInt       (1450   )
Cfg.getParamPercentage("katabatic.saturateRatio"        ).setPercentage(80     )
Cfg.getParamInt       ("katabatic.saturateRp"           ).setInt       (8      )
Cfg.getParamInt       ("kite.borderRipupLimit"          ).setInt       (26     )

# Alliance parameters.
Cfg.getParamString    ("kite.routingGauge"              ).setString    ('sxlib')

# Kite parameters.
Cfg.getParamPercentage("kite.edgeCapacity"        ).setPercentage(65     )
Cfg.getParamPercentage("kite.edgeCapacity"        ).setMin       (0      )
Cfg.getParamPercentage("kite.edgeCapacity"        ).setMax       (110    )
Cfg.getParamInt       ("kite.eventsLimit"         ).setInt       (4000002)
Cfg.getParamInt       ("kite.ripupCost"           ).setInt       (3      )
Cfg.getParamInt       ("kite.ripupCost"           ).setMin       (0      )

Cfg.getParamInt       ("kite.globalRipupLimit"    ).setInt       (5      )
Cfg.getParamInt       ("kite.globalRipupLimit"    ).setMin       (1      )

Cfg.getParamInt       ("kite.localRipupLimit"     ).setInt       (7      )
Cfg.getParamInt       ("kite.localRipupLimit"     ).setMin       (1      )

Cfg.getParamInt       ("kite.longGlobalRipupLimit").setInt       (5      )
Cfg.getParamInt       ("kite.longGlobalRipupLimit").setMin       (1      )

Cfg.getParamInt       ("kite.strapRipupLimit"     ).setInt       (16     )

Cfg.getParamInt       ("kite.strapRipupLimit"     ).setMin       (1      )

Cfg.getParamDouble    ("kite.metal1MinBreak"      ).setDouble    (100    )
Cfg.getParamDouble    ("kite.metal2MinBreak"      ).setDouble    (100    )
Cfg.getParamDouble    ("kite.metal3MinBreak"      ).setDouble    (100    )
Cfg.getParamDouble    ("kite.metal4MinBreak"      ).setDouble    (1450   )
Cfg.getParamDouble    ("kite.metal5MinBreak"      ).setDouble    (1450   )
Cfg.getParamDouble    ("kite.metal6MinBreak"      ).setDouble    (1450   )
Cfg.getParamDouble    ("kite.metal7MinBreak"      ).setDouble    (1450   )

# Kite tab layout.
layout = Cfg.Configuration.get().getLayout()
layout.addTab       ( "Kite", "kite" )
layout.addTitle     ( "Kite", "Katabatic - Routing Database" )
layout.addParameter ( "Kite", "katabatic.saturateRatio"        , "Saturate Ratio (%)"     , 0 )
layout.addParameter ( "Kite", "katabatic.saturateRp"           , "Saturate RoutingPad"    , 0 )
layout.addParameter ( "Kite", "katabatic.globalLengthThreshold", "Global Length Threshold", 0 )
layout.addRule      ( "Kite" )
layout.addTitle     ( "Kite", "Kite - Detailed Router" )
layout.addParameter ( "Kite", "kite.edgeCapacity"  , "Edge Capacity (%)"      , 0 )
layout.addParameter ( "Kite", "kite.eventsLimit"   , "Events Limit"           , 0 )
layout.addParameter ( "Kite", "kite.ripupCost"     , "Ripup Cost"             , 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addParameter ( "Kite", "kite.metal1MinBreak", "METAL1 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal2MinBreak", "METAL2 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal3MinBreak", "METAL3 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal4MinBreak", "METAL4 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal5MinBreak", "METAL5 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal6MinBreak", "METAL6 Length Min Break", 0 )
layout.addParameter ( "Kite", "kite.metal7MinBreak", "METAL7 Length Min Break", 0 )
layout.addSection   ( "Kite", "Ripup Limits", 1 )
layout.addParameter ( "Kite", "kite.borderRipupLimit"    , "Borders"     , 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addParameter ( "Kite", "kite.strapRipupLimit"     , "Straps"      , 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addParameter ( "Kite", "kite.localRipupLimit"     , "Locals"      , 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addParameter ( "Kite", "kite.globalRipupLimit"    , "Globals"     , 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addParameter ( "Kite", "kite.longGlobalRipupLimit", "Long Globals", 1, 1, Cfg.ParameterWidgetFlags.UseSpinBox )
layout.addRule      ( "Kite" )


import Cfg

print "          - Loading Nimbus configuration."
  
# Nimbus parameters.
param = Cfg.getParamPercentage("nimbus.aspectRatio")
param.setPercentage(100)
param.setMin(  10)
param.setMax(1000)
Cfg.getParamBool      ("nimbus.pinsPlacement").setBool      (False  )
Cfg.getParamPercentage("nimbus.spaceMargin"  ).setPercentage(40     )
Cfg.getParamString    ("nimbus.cellGauge"    ).setString    ("sxlib", Cfg.Parameter.Flags.AllRequirements)
Cfg.getParamString    ("nimbus.routingGauge" ).setString    ("sxlib", Cfg.Parameter.Flags.AllRequirements)

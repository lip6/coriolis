
import Cfg

print "          - Loading Stratus 1 configuration."
  
# Status1 parameters.
Cfg.getParamString("stratus1.mappingName").setString ("./stratus2sxlib.xml" )
Cfg.getParamString("stratus1.mappingName").flags |=  Cfg.Parameter.Flags.NeedRestart|Cfg.Parameter.Flags.MustExist
  
# Stratus1 tab layout.
layout = Cfg.Configuration.get().getLayout()
layout.addTab       ( "Stratus1", "stratus1" )
layout.addTitle     ( "Stratus1", "Stratus1 - Netlist & Layout Capture" )
layout.addParameter ( "Stratus1", "stratus1.mappingName", "Virtual Library Translation", 0, 2 )

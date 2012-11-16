
import Cfg

print "          - Loading Misc. configuration."

# Misc. parameters.
Cfg.getParamBool("misc.catchCore"    ).setBool(False)
Cfg.getParamBool("misc.info"         ).setBool(True )
Cfg.getParamBool("misc.logMode"      ).setBool(True )
Cfg.getParamBool("misc.verboseLevel1").setBool(True )
Cfg.getParamBool("misc.verboseLevel2").setBool(False)
Cfg.getParamInt ("misc.traceLevel"   ).setInt (1000 )
param = Cfg.getParamInt ("misc.traceLevel")
param.setMin(0)

# Misc. tab layout.
layout = Cfg.Configuration.get().getLayout()
layout.addTab       ( "Misc.", "misc" )
layout.addTitle     ( "Misc.", "Miscellaneous" )
layout.addParameter ( "Misc.", "misc.catchCore"    , "Catch Core Dumps", 1 )
layout.addParameter ( "Misc.", "misc.verboseLevel1", "Verbose"         , 0 )
layout.addParameter ( "Misc.", "misc.verboseLevel2", "Very Verbose"    , 0 )
layout.addParameter ( "Misc.", "misc.info"         , "Show Info"       , 0 )
layout.addParameter ( "Misc.", "misc.logMode"      , "Output is a TTY" , 0 )
layout.addParameter ( "Misc.", "misc.traceLevel"   , "Trace Level"     , 1 )

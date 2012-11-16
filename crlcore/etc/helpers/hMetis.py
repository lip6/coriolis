
import Cfg

print "          - Loading hMetis configuration."
  
# hMETIS parameters.
Cfg.getParamInt ("metis.globalConnectionsWeightRatio"  ).setInt (1    )
Cfg.getParamInt ("metis.numberOfInstancesStopCriterion").setInt (100  )
Cfg.getParamInt ("metis.numberOfTriedBisections"       ).setInt (10   )
Cfg.getParamBool("metis.partOrKWayHMetis"              ).setBool(True )
Cfg.getParamBool("metis.tuneHMetisParameters"          ).setBool(False)
Cfg.getParamInt ("metis.ubFactor"                      ).setInt (0    )

param = Cfg.getParamEnumerate("metis.CType")
param.setInt(1)
param.addValue("Hybrid First Choice", 1)
param.addValue("First Choice"       , 2)
param.addValue("Greedy First"       , 3)
param.addValue("Hyper Edge"         , 4)
param.addValue("Edge"               , 5)

param = Cfg.getParamEnumerate("metis.RType")
param.setInt(1)
param.addValue("Fiduccia-Mattheyses"           , 1)
param.addValue("One Way Fidducia-Mattheyses"   , 2)
param.addValue("Early Exit Fidducia-Mattheyses", 3)

param = Cfg.getParamEnumerate("metis.VCycle")
param.setInt(0)
param.addValue("No V-Cycle Refinement"         , 0)
param.addValue("On Each Final Bisections"      , 1)
param.addValue("On Best Intermediate Solutions", 2)
param.addValue("On All Intermediate Solutions" , 3)

param = Cfg.getParamEnumerate("metis.Reconst")
param.setInt(0)
param.addValue("Ignore cuts Hyper Edges"       , 0)
param.addValue("Keep parts of cuts Hyper Edges", 1)

param = Cfg.getParamEnumerate("metis.debug")
param.setInt(0)
param.addValue("Disabled"            ,  0)
param.addValue("Coarsening Stage"    ,  1)
param.addValue("Initial Partitioning",  2)
param.addValue("Refinement"          ,  4)
param.addValue("Multiple Runs"       ,  8)
param.addValue("More Multiples Runs" , 16)

master = Cfg.getParamBool ("metis.tuneHMetisParameters")
master.addSlave ( "metis.CType" )
master.addSlave ( "metis.numberOfTriedBisections" )
master.addSlave ( "metis.CType" )
master.addSlave ( "metis.RType" )
master.addSlave ( "metis.VCycle" )
master.addSlave ( "metis.Reconst" )
master.addSlave ( "metis.debug" )

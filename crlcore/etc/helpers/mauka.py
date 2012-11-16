
import Cfg

print "          - Loading Mauka configuration."
  
# Mauka parameters.
Cfg.getParamPercentage("mauka.annealingBinMult" ).setPercentage(5      )
Cfg.getParamPercentage("mauka.annealingNetMult" ).setPercentage(90     )
Cfg.getParamPercentage("mauka.annealingRowMult" ).setPercentage(5      )
Cfg.getParamBool      ("mauka.ignorePins"       ).setBool      (False  )
Cfg.getParamBool      ("mauka.insertFeeds"      ).setBool      (True   )
Cfg.getParamBool      ("mauka.plotBins"         ).setBool      (True   )
Cfg.getParamPercentage("mauka.searchRatio"      ).setPercentage(50     )
Cfg.getParamBool      ("mauka.standardAnnealing").setBool      (False  )
  

layout = Cfg.Configuration.get().getLayout()
# Mauka tab layout.
layout.addTab       ( "Mauka", "mauka" )
# hMETIS part.
layout.addTitle     ( "Mauka", "hMETIS - Partionner" )
layout.addParameter ( "Mauka", "metis.partOrKWayHMetis"              , "Recursive 2-Parts (vs. K-Way)", 0 )
layout.addParameter ( "Mauka", "metis.numberOfInstancesStopCriterion", "Partition Size Stop"          , 0 )
layout.addParameter ( "Mauka", "metis.globalConnectionsWeightRatio"  , "Global Connections Weight"    , 0 )
layout.addParameter ( "Mauka", "metis.ubFactor"                      , "UB Factor"                    , 0 )
layout.addParameter ( "Mauka", "metis.tuneHMetisParameters"          , "Tune hMETIS Parameters"       , 1 )
layout.addParameter ( "Mauka", "metis.numberOfTriedBisections"       , "# of tried bisections"        , 1 )
layout.addParameter ( "Mauka", "metis.CType"                         , "CType"                        , 1 )
layout.addParameter ( "Mauka", "metis.RType"                         , "RType"                        , 1 )
layout.addParameter ( "Mauka", "metis.VCycle"                        , "V-Cycle"                      , 1 )
layout.addParameter ( "Mauka", "metis.Reconst"                       , "Reconst"                      , 1 )
# Nimbus part.
layout.addTitle     ( "Mauka", "Mauka - Placer" )
layout.addParameter ( "Mauka", "nimbus.pinsPlacement", "Pins Placement"       , 0 )
layout.addParameter ( "Mauka", "nimbus.aspectRatio"  , "Aspect Ratio, X/Y (%)", 1 )
layout.addParameter ( "Mauka", "nimbus.spaceMargin"  , "Space Margin"         , 1 )
layout.addRule      ( "Mauka" )
# Mauka part.
layout.addParameter ( "Mauka", "mauka.standardAnnealing", "Standart Annealing"    , 0 )
layout.addParameter ( "Mauka", "mauka.ignorePins"       , "Ignore Pins"           , 0 )
layout.addParameter ( "Mauka", "mauka.plotBins"         , "Plot Bins"             , 0 )
layout.addParameter ( "Mauka", "mauka.insertFeeds"      , "Insert Feeds"          , 0 )
layout.addParameter ( "Mauka", "mauka.searchRatio"      , "Search Ratio (%)"      , 1 )
layout.addParameter ( "Mauka", "mauka.annealingNetMult" , "Annealing Net Mult (%)", 1 )
layout.addParameter ( "Mauka", "mauka.annealingBinMult" , "Annealing Bin Mult (%)", 1 )
layout.addParameter ( "Mauka", "mauka.annealingRowMult" , "Annealing Row Mult (%)", 1 )
layout.addRule      ( "Mauka" )

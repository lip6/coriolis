
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
# |  Python      :       "./etc/node45/freepdk45/kite.py"           |
# +-----------------------------------------------------------------+


import coriolis.Cgf as Cfg
from   coriolis.helpers    import truncPath, l, u, n
from   coriolis.helpers.io import vprint
vprint( 2, '     - "%s".' % truncPath(__file__) )

from coriolis.Hurricane import DataBase
from coriolis.CRL       import AllianceFramework, RoutingGauge, \
                               RoutingLayerGauge, CellGauge
from ...common import kite


p = Cfg.getParamDouble    ( 'lefImport.minTerminalWidth'      ).setDouble    ( 0.0      )
p = Cfg.getParamString    ( 'katabatic.routingGauge'          ).setString    ( 'LEF.CoreSite' ) 
p = Cfg.getParamInt       ( "katabatic.globalLengthThreshold" ).setInt       ( 1450     )
p = Cfg.getParamPercentage( "katabatic.saturateRatio"         ).setPercentage( 80       ) 
p = Cfg.getParamInt       ( "katabatic.saturateRp"            ).setInt       ( 8        )
p = Cfg.getParamString    ( 'katabatic.topRoutingLayer'       ).setString    ( 'metal5' )

 # Kite parameters.
p = Cfg.getParamInt( "kite.hTracksReservedLocal" ); p.setInt( 4       ); p.setMin( 0 ); p.setMax( 18 )
p = Cfg.getParamInt( "kite.vTracksReservedLocal" ); p.setInt( 3       ); p.setMin( 0 ); p.setMax( 18 )
p = Cfg.getParamInt( "kite.eventsLimit"          ); p.setInt( 4000002 ) 
p = Cfg.getParamInt( "kite.ripupCost"            ); p.setInt( 3       ); p.setMin( 0 )
p = Cfg.getParamInt( "kite.strapRipupLimit"      ); p.setInt( 16      ); p.setMin( 1 )
p = Cfg.getParamInt( "kite.localRipupLimit"      ); p.setInt( 9       ); p.setMin( 1 )
p = Cfg.getParamInt( "kite.globalRipupLimit"     ); p.setInt( 5       ); p.setMin( 1 )
p = Cfg.getParamInt( "kite.longGlobalRipupLimit" ); p.setInt( 5       ); p.setMin( 1 )

# Anabatic & Katana parameters are temporarily hosted here.
p = Cfg.getParamString    ( 'crlcore.groundName'             ); p.setString    ( 'gnd!'   )
p = Cfg.getParamString    ( 'crlcore.powerName'              ); p.setString    ( 'vdd!'   )
p = Cfg.getParamString    ( 'anabatic.routingGauge'          ); p.setString    ( 'LEF.CoreSite' )
p = Cfg.getParamInt       ( "anabatic.globalLengthThreshold" ); p.setInt       ( 1450     )
p = Cfg.getParamPercentage( "anabatic.saturateRatio"         ); p.setPercentage( 90       )
p = Cfg.getParamInt       ( "anabatic.saturateRp"            ); p.setInt       ( 10       )
p = Cfg.getParamString    ( 'anabatic.topRoutingLayer'       ); p.setString    ( 'metal5' )
p = Cfg.getParamInt       ( "anabatic.edgeLength"            ); p.setInt       ( 48       )
p = Cfg.getParamInt       ( "anabatic.edgeWidth"             ); p.setInt       ( 8        )
p = Cfg.getParamDouble    ( "anabatic.edgeCostH"             ); p.setDouble    ( 9.0      )
p = Cfg.getParamDouble    ( "anabatic.edgeCostK"             ); p.setDouble    ( -10.0    )
p = Cfg.getParamDouble    ( "anabatic.edgeHInc"              ); p.setDouble    ( 1.0      ) 
p = Cfg.getParamDouble    ( "anabatic.edgeHScaling"          ); p.setDouble    ( 1.0      ) 
p = Cfg.getParamInt       ( "anabatic.globalIterations"      ); p.setInt       ( 10       ); p.setMin(1); p.setMax(100)
p = Cfg.getParamEnumerate ( "anabatic.gcell.displayMode"     ); p.setInt       ( 1        )
p.addValue( "Boundary", 1 ) 
p.addValue( "Density" , 2 )

p = Cfg.getParamInt   ( "katana.hTracksReservedLocal" ); p.setInt   ( 4       ); p.setMin(0); p.setMax(20)
p = Cfg.getParamInt   ( "katana.vTracksReservedLocal" ); p.setInt   ( 3       ); p.setMin(0); p.setMax(20)
p = Cfg.getParamInt   ( "katana.termSatReservedLocal" ); p.setInt   ( 8       ) 
p = Cfg.getParamInt   ( "katana.termSatThreshold"     ); p.setInt   ( 9       )
p = Cfg.getParamInt   ( "katana.eventsLimit"          ); p.setInt   ( 4000002 ) 
p = Cfg.getParamInt   ( "katana.ripupCost"            ); p.setInt   ( 3       ); p.setMin(0)
p = Cfg.getParamInt   ( "katana.strapRipupLimit"      ); p.setInt   ( 16      ); p.setMin(1)
p = Cfg.getParamInt   ( "katana.localRipupLimit"      ); p.setInt   ( 9       ); p.setMin(1)
p = Cfg.getParamInt   ( "katana.globalRipupLimit"     ); p.setInt   ( 5       ); p.setMin(1)
p = Cfg.getParamInt   ( "katana.longGlobalRipupLimit" ); p.setInt   ( 5       ); p.setMin(1)
p = Cfg.getParamString( 'chip.padCoreSide'            ); p.setString( 'South' )


tech = DataBase.getDB().getTechnology()
af   = AllianceFramework.get()
rg   = RoutingGauge.create( 'LEF.CoreSite' )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal1')       # metal.
                                          , RoutingLayerGauge.Horizontal  # preferred routing direction.
                                          , RoutingLayerGauge.PinOnly     # layer usage.
                                          , 0                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.190)                      # track pitch.
                                          , u(0.065)                      # wire width.
                                          , u(0.065)                      # VIA side (that is VIA12).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal2')       # metal.
                                          , RoutingLayerGauge.Vertical    # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 1                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.190)                      # track pitch.
                                          , u(0.070)                      # wire width.
                                          , u(0.070)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal3')       # metal.
                                          , RoutingLayerGauge.Horizontal  # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 2                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.190)                      # track pitch.
                                          , u(0.070)                      # wire width.
                                          , u(0.070)                      # VIA side (that is VIA34).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal4')       # metal.
                                          , RoutingLayerGauge.Vertical    # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 3                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.285)                      # track pitch.
                                          , u(0.140)                      # wire width.
                                          , u(0.140)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal5')       # metal.
                                          , RoutingLayerGauge.Horizontal  # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 4                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.285)                      # track pitch.
                                          , u(0.140)                      # wire width.
                                          , u(0.140)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal6')       # metal.
                                          , RoutingLayerGauge.Vertical    # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 5                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.285)                      # track pitch.
                                          , u(0.140)                      # wire width.
                                          , u(0.140)                      # VIA side (that is VIA23).
                                          , u(8)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal7')       # metal.
                                          , RoutingLayerGauge.Horizontal  # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 5                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.855)                      # track pitch.
                                          , u(0.400)                      # wire width.
                                          , u(0.400)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal8')       # metal.
                                          , RoutingLayerGauge.Vertical    # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 5                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(0.855)                      # track pitch.
                                          , u(0.400)                      # wire width.
                                          , u(0.400)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal9')       # metal.
                                          , RoutingLayerGauge.Horizontal  # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 5                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(1.710)                      # track pitch.
                                          , u(0.800)                      # wire width.
                                          , u(0.800)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

rg.addLayerGauge( RoutingLayerGauge.create( tech.getLayer('metal10')      # metal.
                                          , RoutingLayerGauge.Vertical    # preferred routing direction.
                                          , RoutingLayerGauge.Default     # layer usage.
                                          , 5                             # depth.
                                          , 0.0                           # density (deprecated).
                                          , u(0)                          # track offset from AB.
                                          , u(1.710)                      # track pitch.
                                          , u(0.800)                      # wire width.
                                          , u(0.800)                      # VIA side (that is VIA23).
                                          , u(0)                          # obstacle dW.
                                          ) )

af.addRoutingGauge( rg )
af.setRoutingGauge( 'LEF.CoreSite' )

# Gauge for standard cells.
cg = CellGauge.create( 'LEF.CoreSite'
                     , 'metal2'  # pin layer name.
                     , u(0.38)   # pitch.
                     , u(2.47)   # cell slice height.
                     , u(0.38)   # cell slice step.
                     )
af.addCellGauge( cg )
af.setCellGauge( 'LEF.CoreSite' )

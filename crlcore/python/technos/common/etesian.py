
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
# |  Python      :       "./etc/common/etesian.py"                  |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg


param = Cfg.getParamDouble( 'etesian.aspectRatio' )
param.setDouble( 1.0 )

Cfg.getParamDouble    ( 'etesian.spaceMargin'      ).setPercentage( 0.05 )
Cfg.getParamDouble    ( 'etesian.densityVariation' ).setPercentage( 0.05 )
Cfg.getParamBool      ( 'etesian.routingDriven'    ).setBool      ( False )
Cfg.getParamString    ( 'etesian.feedNames'        ).setString    ( 'tie_x0,rowend_x0' )
Cfg.getParamString    ( 'etesian.cell.zero'        ).setString    ( 'zero_x0' )
Cfg.getParamString    ( 'etesian.cell.one'         ).setString    ( 'one_x0' )
Cfg.getParamString    ( 'etesian.bloat'            ).setString    ( 'disabled' )

param = Cfg.getParamEnumerate( 'etesian.effort' )
param.setInt( 2 )
param.addValue( 'Fast'    , 1 )
param.addValue( 'Standard', 3 )
param.addValue( 'High'    , 6 )
param.addValue( 'Extreme' , 9 )

param = Cfg.getParamEnumerate( 'etesian.graphics' )
param.setInt( 3 )
param.addValue( 'Show every step' , 1 )
param.addValue( 'Show lower bound', 2 )
param.addValue( 'Show result only', 3 )

layout  = Cfg.Configuration.get().getLayout()
layout.addTab      ( 'Placer', 'etesian' )
layout.addTitle    ( 'Placer', 'Placement area' )
layout.addParameter( 'Placer', 'etesian.aspectRatio'   , 'Aspect Ratio, X/Y' , 0 )
layout.addParameter( 'Placer', 'etesian.spaceMargin'   , 'Space Margin'      , 1 )
layout.addRule     ( 'Placer' )
layout.addTitle    ( 'Placer', 'Etesian - Placer')
layout.addParameter( 'Placer', 'etesian.densityVariation' , 'Density variation' , 0 )
layout.addParameter( 'Placer', 'etesian.routingDriven'    , 'Routing driven'    , 0 )
layout.addParameter( 'Placer', 'etesian.effort'           , 'Placement effort'  , 1 )
layout.addParameter( 'Placer', 'etesian.graphics'         , 'Placement view'    , 1 )
layout.addRule     ( 'Placer' )


# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/common/katana.py"                     |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg


layout  = Cfg.Configuration.get().getLayout()

# Kite Layout.
layout.addTab      ( 'Router', 'katana' )
layout.addTitle    ( 'Router', 'Katabatic - Routing Database' )
layout.addParameter( 'Router', 'katabatic.saturateRatio'        , 'Saturate Ratio (%)'     , 0, 1 )
layout.addParameter( 'Router', 'katabatic.saturateRp'           , 'Saturate RoutingPad'    , 0, 1 )
layout.addParameter( 'Router', 'katabatic.globalLengthThreshold', 'Global Length Threshold', 0, 1 )
layout.addParameter( 'Router', 'katabatic.topRoutingLayer'      , 'Top Routing Layer'      , 0, 1 )
layout.addParameter( 'Router', 'anabatic.gcell.displayMode'     , 'GCell Display Mode'     , 1, 1 )
layout.addRule     ( 'Router' )
layout.addTitle    ( 'Router', 'Katana - Detailed Router' )
layout.addParameter( 'Router', 'katana.hTracksReservedMin'  , 'Min Vert. Reserved Tracks', 0 )
layout.addParameter( 'Router', 'katana.vTracksReservedMin'  , 'Min Hor. Reserved Tracks' , 0 )
layout.addParameter( 'Router', 'katana.hTracksReservedLocal', 'Max Vert. Reserved Tracks', 5 )
layout.addParameter( 'Router', 'katana.vTracksReservedLocal', 'Max Hor. Reserved Tracks' , 5 )
layout.addParameter( 'Router', 'katana.eventsLimit'         , 'Events Limit'             , 0 )
layout.addParameter( 'Router', 'katana.ripupCost'           , 'Ripup Cost'               , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addSection  ( 'Router', 'Ripup Limits', 1 )
layout.addParameter( 'Router', 'katana.strapRipupLimit'     , 'Straps'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'katana.localRipupLimit'     , 'Locals'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'katana.globalRipupLimit'    , 'Globals'     , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'katana.longGlobalRipupLimit', 'Long Globals', 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addRule     ( 'Router' )

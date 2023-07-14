
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
# |  Python      :       "./etc/common/kite.py"                     |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg


layout  = Cfg.Configuration.get().getLayout()

# Kite Layout.
layout.addTab      ( 'Router', 'kite' )
layout.addTitle    ( 'Router', 'Katabatic - Routing Database' )
layout.addParameter( 'Router', 'katabatic.saturateRatio'        , 'Saturate Ratio (%)'     , 0, 1 )
layout.addParameter( 'Router', 'katabatic.saturateRp'           , 'Saturate RoutingPad'    , 0, 1 )
layout.addParameter( 'Router', 'katabatic.globalLengthThreshold', 'Global Length Threshold', 0, 1 )
layout.addParameter( 'Router', 'katabatic.topRoutingLayer'      , 'Top Routing Layer'      , 0, 1 )
layout.addParameter( 'Router', 'anabatic.gcell.displayMode'     , 'GCell Display Mode'     , 1, 1 )
layout.addRule     ( 'Router' )
layout.addTitle    ( 'Router', 'Kite - Detailed Router' )
layout.addParameter( 'Router', 'kite.hTracksReservedLocal', 'Vert. Locally Reserved Tracks', 0 )
layout.addParameter( 'Router', 'kite.vTracksReservedLocal', 'Hor. Locally Reserved Tracks' , 0 )
layout.addParameter( 'Router', 'kite.eventsLimit'         , 'Events Limit'                 , 0 )
layout.addParameter( 'Router', 'kite.ripupCost'           , 'Ripup Cost'                   , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addSection  ( 'Router', 'Ripup Limits', 1 )
layout.addParameter( 'Router', 'kite.strapRipupLimit'     , 'Straps'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'kite.localRipupLimit'     , 'Locals'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'kite.globalRipupLimit'    , 'Globals'     , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Router', 'kite.longGlobalRipupLimit', 'Long Globals', 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addRule     ( 'Router' )


# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2019-2019, All Rights Reserved
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


import Cfg


layout  = Cfg.Configuration.get().getLayout()

# Kite Layout.
layout.addTab      ( 'Kite', 'kite' )
layout.addTitle    ( 'Kite', 'Katabatic - Routing Database' )
layout.addParameter( 'Kite', 'katabatic.saturateRatio'        , 'Saturate Ratio (%)'     , 0, 1 )
layout.addParameter( 'Kite', 'katabatic.saturateRp'           , 'Saturate RoutingPad'    , 0, 1 )
layout.addParameter( 'Kite', 'katabatic.globalLengthThreshold', 'Global Length Threshold', 0, 1 )
layout.addParameter( 'Kite', 'katabatic.topRoutingLayer'      , 'Top Routing Layer'      , 0, 1 )
layout.addParameter( 'Kite', 'anabatic.gcell.displayMode'     , 'GCell Display Mode'     , 1, 1 )
layout.addRule     ( 'Kite' )
layout.addTitle    ( 'Kite', 'Kite - Detailed Router' )
layout.addParameter( 'Kite', 'kite.hTracksReservedLocal', 'Vert. Locally Reserved Tracks', 0 )
layout.addParameter( 'Kite', 'kite.vTracksReservedLocal', 'Hor. Locally Reserved Tracks' , 0 )
layout.addParameter( 'Kite', 'kite.eventsLimit'         , 'Events Limit'                 , 0 )
layout.addParameter( 'Kite', 'kite.ripupCost'           , 'Ripup Cost'                   , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addSection  ( 'Kite', 'Ripup Limits', 1 )
layout.addParameter( 'Kite', 'kite.strapRipupLimit'     , 'Straps'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Kite', 'kite.localRipupLimit'     , 'Locals'      , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Kite', 'kite.globalRipupLimit'    , 'Globals'     , 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addParameter( 'Kite', 'kite.longGlobalRipupLimit', 'Long Globals', 1, 1, Cfg.Parameter.Flags.UseSpinBox )
layout.addRule     ( 'Kite' )


# -*- Mode:Python -*-


defaultStyle = 'Alliance.Classic [black]'
 
parametersTable = \
    ( ('misc.catchCore'           , TypeBool      , False   )
    , ('misc.info'                , TypeBool      , False   )
    , ('misc.paranoid'            , TypeBool      , False   )
    , ('misc.bug'                 , TypeBool      , False   )
    , ('misc.logMode'             , TypeBool      , False   )
    , ('misc.verboseLevel1'       , TypeBool      , False   )
    , ('misc.verboseLevel2'       , TypeBool      , True    )
    , ('misc.traceLevel'          , TypeInt       , 1000    )
    , ('etesian.spaceMargin'      , TypePercentage, 30.0    )
    , ('etesian.aspectRatio'      , TypePercentage, 100.0   )
   # Kite parameters.
    , ("kite.eventsLimit"         , TypeInt       , 1000000 )
    , ('katabatic.topRoutingLayer', TypeString    , 'METAL5')
    , ("kite.hTracksReservedLocal", TypeInt       , 4       )
    , ("kite.vTracksReservedLocal", TypeInt       , 3       )
    )

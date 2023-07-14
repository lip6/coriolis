
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
# |  Python      :       "./etc/common/misc.py"                     |
# +-----------------------------------------------------------------+


import coriolis.Cfg as Cfg


Cfg.getParamBool( 'misc.catchCore'    ).setBool( False )
Cfg.getParamBool( 'misc.info'         ).setBool( False )
Cfg.getParamBool( 'misc.bug'          ).setBool( False )
Cfg.getParamBool( 'misc.logMode'      ).setBool( False )
Cfg.getParamBool( 'misc.verboseLevel1').setBool( True  )
Cfg.getParamBool( 'misc.verboseLevel2').setBool( True  )

param = Cfg.getParamInt( 'misc.minTraceLevel' )
param.setInt( 100000 )
param.setMin( 0 )

param = Cfg.getParamInt( 'misc.maxTraceLevel' )
param.setInt( 0 )
param.setMin( 0 )

Cfg.getParamInt( 'viewer.minimumSize'   ).setInt( 500  )
Cfg.getParamInt( 'viewer.pixelThreshold').setInt(   5 )

param = Cfg.getParamInt( 'viewer.printer.DPI' )
param.setInt( 150 )
param.setMin( 100 )

param = Cfg.getParamEnumerate( 'viewer.printer.mode' )
param.addValue( 'Cell mode'  , 1 )
param.addValue( 'Design mode', 2 )
param.setInt  ( 1 )

# Those enumerated values *must* match QPrinter::Orientation.
param = Cfg.getParamEnumerate( 'viewer.printer.orientation' )
param.addValue( 'Portrait' , 0 )
param.addValue( 'Landscape', 1 )
param.setInt  ( 0 )

# Those enumerated values *must* match QPrinter::PaperSize.
param = Cfg.getParamEnumerate( 'viewer.printer.paper' )
param.addValue( "A4"       ,  0 )
param.addValue( "B5"       ,  1 )
param.addValue( "Letter"   ,  2 )
param.addValue( "Legal"    ,  3 )
param.addValue( "Executive",  4 )
param.addValue( "A0"       ,  5 )
param.addValue( "A1"       ,  6 )
param.addValue( "A2"       ,  7 )
param.addValue( "A3"       ,  8 )
param.addValue( "A5"       ,  9 )
param.addValue( "A6"       , 10 )
param.addValue( "A7"       , 11 )
param.addValue( "A8"       , 12 )
param.addValue( "A9"       , 13 )
param.addValue( "B0"       , 14 )
param.addValue( "B1"       , 15 )
param.addValue( "B10"      , 16 )
param.addValue( "B2"       , 17 )
param.addValue( "B3"       , 18 )
param.addValue( "B4"       , 19 )
param.addValue( "B6"       , 20 )
param.addValue( "B7"       , 21 )
param.addValue( "B8"       , 22 )
param.addValue( "B9"       , 23 )
param.addValue( "C5E"      , 24 )
param.addValue( "Comm10E"  , 25 )
param.addValue( "DLE"      , 26 )
param.addValue( "Folio"    , 27 )
param.addValue( "Ledger"   , 28 )
param.addValue( "Tabloid"  , 29 )
param.addValue( "Custom"   , 30 )
param.setInt  ( 0 )

layout  = Cfg.Configuration.get().getLayout()
layout.addTab      ( 'Misc', 'Misc.' )
layout.addTitle    ( 'Misc', 'Miscellaneous' )
layout.addParameter( 'Misc', 'misc.catchCore'            , 'Catch Core Dumps'     , 1 )
layout.addParameter( 'Misc', 'misc.verboseLevel1'        , 'Verbose'              , 0 )
layout.addParameter( 'Misc', 'misc.verboseLevel2'        , 'Very Verbose'         , 0 )
layout.addParameter( 'Misc', 'misc.info'                 , 'Show Info'            , 0 )
layout.addParameter( 'Misc', 'misc.logMode'              , 'Output is a TTY'      , 0 )
layout.addParameter( 'Misc', 'misc.minTraceLevel'        , 'Min. Trace Level'     , 1 )
layout.addParameter( 'Misc', 'misc.maxTraceLevel'        , 'Max. Trace Level'     , 1 )
layout.addTitle    ( 'Misc', 'Print/Snapshot Parameters' )
layout.addParameter( 'Misc', 'viewer.printer.mode'       , 'Printer/Snapshot Mode', 1 )
layout.addParameter( 'Misc', 'viewer.printer.paper'      , 'Paper Size'           , 0 )
layout.addParameter( 'Misc', 'viewer.printer.orientation', 'Orientation'          , 0 )
layout.addParameter( 'Misc', 'viewer.printer.DPI'        , 'DPI'                  , 0 )



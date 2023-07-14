#!/usr/bin/env python3
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2015-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |     C o r i o l i s  -  Generic Program Launcher                |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./src/cgt.py"                                  |
# +-----------------------------------------------------------------+

try:
    import sys
    import os.path
    import optparse
    from coriolis import helpers
    helpers.loadUserSettings()
    from coriolis import Cfg, Hurricane, Viewer, CRL, Etesian, Anabatic, \
                         Katana, Tramontana, Bora, Tutorial, Unicorn
except Exception as e:
    helpers.io.showPythonTrace( sys.argv[0], e )
    sys.exit(2)


def setCgtBanner ( banner ):
    banner.setName('cgt')
    banner.setPurpose('Coriolis Graphical Tool')
    return banner


def credits ():
    s  = ''
    s +=  '        Tool Credits\n'
    s +=  '        Hurricane .................... Remy Escassut & Christian Masson\n'
    s +=  '        Etesian - Placer .............................. Gabriel Gouvine\n'
    s +=  '        Knik - Global Router ............................ Damien Dupuis\n'
    s +=  '        Kite - Detailed Router ....................... Jean-Paul Chaput\n\n'

    s +=  '        Contributors\n'
    s +=  '        Sophie Belloeil, Hugo Clement, Marek Sroka, Wu Yifei\n'

    s +=  '        Coloquinte software credits (used by Etesian)\n'
    s +=  '        Author ........................................ Gabriel Gouvine\n'

    s +=  '        FLUTE software credits (used by Knik)\n'
    s +=  '        Author ........................................ Chris C. N. CHU\n'
    s +=  '        Prof. Ident. ............................ Iowa State University\n'
    s +=  '        URL ........................ http://home.eng.iastate.edu/~cnchu\n\n'
    return s


def runScript ( scriptPath, editor ):
  try:
      sys.stdout.flush()
      sys.stderr.flush()

      kw = { }
      if editor: kw[ 'editor' ] = editor
      sys.path.insert( 0, os.path.dirname(scriptPath) )
    
      module = __import__( os.path.basename(scriptPath), globals(), locals() )
      if not 'scriptMain' in module.__dict__:
          print( '[ERROR] Script module is missing function scriptMain().' )
          print( '        "{}"'.format( scriptPath ))
          return
      if not callable( module.__dict__['scriptMain'] ):
          print( '[ERROR] Script module symbol scriptMain is not callable (not a function?).' )
          print( '        "{}"'.format( scriptPath ))
          return
    
      module.__dict__['scriptMain']( **kw )

  except ImportError as e:
     #module = str(e).split()[-1]
     #print( '[ERROR] The "{}" script cannot be loaded.'.format(os.path.basename(scriptPath)) )
     #print( '        Please check your design hierarchy or the Python syntax.' )
     #print( '        Error was:' )
     #print( '          {}\n'.format(e))
      helpers.io.catch( e )
  except Exception as e:
      helpers.io.catch( e )
  return


if __name__ == '__main__':

    try:
      usage  = str(setCgtBanner(CRL.Banner()))
      usage += '\ncgt [options]'

      parser = optparse.OptionParser(usage)
      parser.add_option(       '--no-init'          , action='store_true', dest='noInit'         , help='Do not load any initialization.')
      parser.add_option( '-c', '--cell'             , type='string'      , dest='cell'           , help='The name of the cell to load, without extension.')
      parser.add_option(       '--acm-sigda-89'     , type='string'      , dest='acmSigdaName'   , help='An ACM/SIGDA 89 bench name to load, without extension.')
      parser.add_option(       '--blif'             , type='string'      , dest='blifName'       , help='A Blif (Yosys) design name to load, without extension.')
      parser.add_option(       '--ispd-05'          , type='string'      , dest='ispd05name'     , help='An ISPD 05 bench (placement) name to load, without extension.')
      parser.add_option(       '--script'           , type='string'      , dest='script'         , help='Run a Python or Stratus script.')
      parser.add_option( '-v', '--verbose'          , action='store_true', dest='verbose'        , help='First level of verbosity.')
      parser.add_option( '-V', '--very-verbose'     , action='store_true', dest='veryVerbose'    , help='Second level of verbosity.')
      parser.add_option( '-i', '--info'             , action='store_true', dest='info'           , help='Display lots of informational messages.')
      parser.add_option(       '--paranoid'         , action='store_true', dest='paranoid'       , help='Display everything that *may be* suspicious...')
      parser.add_option( '-b', '--bug'              , action='store_true', dest='bug'            , help='Display bug related messages.')
      parser.add_option(       '--show-conf'        , action='store_true', dest='showConf'       , help='Display Kite configuration.')
      parser.add_option( '-D', '--core-dump'        , action='store_true', dest='coreDump'       , help='Enable core-dump when a crash occurs.')
      parser.add_option( '-L', '--log-mode'         , action='store_true', dest='logMode'        , help='Disable ANSI escape sequences in console output.')
      parser.add_option( '-t', '--text'             , action='store_true', dest='textMode'       , help='Run in command line mode.')
      parser.add_option( '-m', '--margin'           , type='float'       , dest='margin'         , help='Percentage of free area to add to the minimal placement area.')
      parser.add_option( '-P', '--place'            , action='store_true', dest='place'          , help='Run the analytical placer (Etesian).')
      parser.add_option( '-G', '--global-route'     , action='store_true', dest='globalRoute'    , help='Run the global router (Knik).')
      parser.add_option( '-g', '--load-global'      , action='store_true', dest='loadGlobal'     , help='Reload a global routing from disk.')
      parser.add_option(       '--save-global'      , action='store_true', dest='saveGlobal'     , help='Save the global routing solution.')
      parser.add_option(       '--htracks-local'    , type='int'         , dest='hTracksLocal'   , help='The amount of horizontal tracks reserved for the GCell local routing.')
      parser.add_option(       '--vtracks-local'    , type='int'         , dest='vTracksLocal'   , help='The amount of vertical tracks reserved for the GCell local routing .')
      parser.add_option(       '--events-limit'     , type='int'         , dest='eventsLimit'    , help='The maximum number of iterations (events) that the router is allowed to perform.')
      parser.add_option( '-R', '--detail-route'     , action='store_true', dest='detailRoute'    , help='Run the detailed router (Kite).')
      parser.add_option( '-M', '--dump-measures'    , action='store_true', dest='dumpMeasures'   , help='Dump some statistical measurements on the disk.')
      parser.add_option( '-s', '--save-design'      , type='string'      , dest='saveDesign'     , help='Save the routed design.')
      parser.add_option(       '--top-routing-layer', type='string'      , dest='topRoutingLayer', help='Sets the top (upper) routing layer.')
      parser.add_option(       '--vst-use-concat'   , action='store_true', dest='vstUseConcat'   , help='The VST driver will use "&" (concat) in PORT MAP.')
      (options, args) = parser.parse_args()
      args.insert(0, 'cgt')

      flags     = 0
      if options.noInit:
        flags |= CRL.AllianceFramework.NoPythonInit

      af = CRL.AllianceFramework.create( flags )
      if helpers.io.isVL(2): print( af.getEnvironment().getPrint() )

      Cfg.Configuration.pushDefaultPriority(Cfg.Parameter.Priority.CommandLine)

      if options.coreDump:        Cfg.getParamBool      ('misc.catchCore'    ).setBool(False)
      if options.verbose:         Cfg.getParamBool      ('misc.verboseLevel1').setBool(True)
      if options.veryVerbose:     Cfg.getParamBool      ('misc.verboseLevel2').setBool(True)
      if options.info:            Cfg.getParamBool      ('misc.info'         ).setBool(True)
      if options.paranoid:        Cfg.getParamBool      ('misc.paranoid'     ).setBool(True)
      if options.bug:             Cfg.getParamBool      ('misc.bug'          ).setBool(True)
      if options.logMode:         Cfg.getParamBool      ('misc.logMode'      ).setBool(True)
      if options.showConf:        Cfg.getParamBool      ('misc.showConf'     ).setBool(True)
      if options.margin:          Cfg.getParamPercentage('etesian.spaceMargin').setPercentage(options.margin)
      if options.hTracksLocal:    Cfg.getParamInt       ('katana.hTracksReservedLocal').setInt(options.hTracksLocal)
      if options.vTracksLocal:    Cfg.getParamInt       ('katana.vTracksReservedLocal').setInt(options.vTracksLocal)
      if options.eventsLimit:     Cfg.getParamInt       ('katana.eventsLimit'         ).setInt(options.eventsLimit)
      if options.topRoutingLayer: Cfg.getParamString    ('anabatic.topRoutingLayer'   ).setString(options.topRoutingLayer)

      loadGlobal     = options.loadGlobal
      saveGlobal     = options.saveGlobal
      globalRoute    = options.globalRoute
      detailRoute    = options.detailRoute
      runEtesianTool = options.place

      Cfg.Configuration.popDefaultPriority()
      
      cell = None
      if options.acmSigdaName:
          cell = CRL.AcmSigda.load(options.acmSigdaName)
      elif options.ispd05name:
          cell = CRL.Ispd05.load(options.ispd05name)
      elif options.blifName:
          cell = CRL.Blif.load(options.blifName)
      elif options.cell:
          cell = af.getCell(options.cell, CRL.Catalog.State.Views)
      else:
          runEtesianTool = False
          loadGlobal     = False
          saveGlobal     = False
          globalRoute    = False
          detailRoute    = False
      
      if not options.textMode:
         # Run in graphic mode.
          ha = Viewer.HApplication.create(args)
          Viewer.Graphics.enable()
          
          unicorn = Unicorn.UnicornGui.create()
          unicorn.setApplicationName  ('cgt')
          unicorn.registerTool        (Etesian.GraphicEtesianEngine.grab())
          unicorn.registerTool        (Katana.GraphicKatanaEngine.grab())
          unicorn.registerTool        (Tramontana.GraphicTramontanaEngine.grab())
          unicorn.registerTool        (Bora.GraphicBoraEngine.grab())
          unicorn.registerTool        (Tutorial.GraphicTutorialEngine.grab())
         #unicorn.setAnonNetSelectable(False)
          unicorn.setLayerVisible     ("grid"          , False);
          unicorn.setLayerVisible     ("text.instance" , False);
          unicorn.setLayerVisible     ("text.component", False);
          
          if options.script:
              runScript(options.script,unicorn)
      else:
         # Run in command line mode.
          if options.script: runScript(options.script,None)

      kiteSuccess = True
      
      if runEtesianTool:
          etesian = Etesian.EtesianEngine.create(cell)
         #if options.showConf: etesian.printConfiguration()
          etesian.place()
          etesian.toHurricane()

      if detailRoute and not (loadGlobal or globalRoute): globalRoute = True
      runKiteTool = loadGlobal or globalRoute or detailRoute

      if runKiteTool:
        runKiteTool = False

        katana = Katana.KatanaEngine.create( cell )
       #katana.printConfiguration   ()
        katana.digitalInit          ()
       #katana.runNegociatePreRouted()
        katana.runGlobalRouter      ( Katana.Flags.NoFlags )
        katana.loadGlobalRouting    ( Anabatic.EngineLoadGrByNet )
        katana.layerAssign          ( Anabatic.EngineNoNetLayerAssign )
        katana.runNegociate         ( Katana.Flags.NoFlags )
        kiteSuccess = katana.isDetailedRoutingSuccess()
       #katana.finalizeLayout()
        katana.destroy()

      if options.saveDesign:
          views = CRL.Catalog.State.Physical
          if options.vstUseConcat: views |= CRL.Catalog.State.VstUseConcat
          if options.saveDesign != cell.getName():
              cell.setName(options.saveDesign)
              views |= CRL.Catalog.State.Logical
          af.saveCell(cell, views)
 
      if not options.textMode:
          setCgtBanner(unicorn.getBanner())

          if cell: unicorn.setCell(cell)
          unicorn.show()
          ha.qtExec()
      else:
            sys.exit(not kiteSuccess)

    except Exception as e:
        helpers.io.showPythonTrace( sys.argv[0], e )

    sys.exit(0)

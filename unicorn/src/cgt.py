#!/usr/bin/env python


try:
  import sys
  import os.path
  import optparse
  import Cfg
  import Hurricane
  import Viewer
  import CRL
  import Etesian
  import Katabatic
  import Kite
  import Unicorn
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
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

    s +=  '        Coloquinte software credits (used by Etesian)\n'
    s +=  '        Author ........................................ Gabriel Gouvine\n'
    s +=  '        URL .............. https://github.com/Coloquinte/Coloquinte.git\n\n'

    s +=  '        FLUTE software credits (used by Knik)\n'
    s +=  '        Author ........................................ Chris C. N. CHU\n'
    s +=  '        Prof. Ident. ............................ Iowa State University\n'
    s +=  '        URL ........................ http://home.eng.iastate.edu/~cnchu\n\n'
    return s


def runScript ( scriptPath, editor ):
    try:
        script = __import__(scriptPath)
    except Exception, e:
      print '[ERROR] An exception occured while loading the Stratus script module:'
      print '        <%s>\n' % (scriptPath)
      print '        You should check for simple python errors in this module.'
      print '        Error was:'
      print '          %s\n' % e
      print '        Trying to continue anyway...'
      return

    if not hasattr(script,'ScriptMain'):
        print '[ERROR] Script module is missing function ScriptMain().'
        print '        <%s>' % scriptPath
        return

    script.ScriptMain(editor)

    return


if __name__ == '__main__':

    try:
      usage  = str(setCgtBanner(CRL.Banner()))
      usage += '\ncgt [options]'

      parser = optparse.OptionParser(usage)
      parser.add_option( '-c', '--cell'             , type='string'      , dest='cell'           , help='The name of the cell to load, whithout extension.')
      parser.add_option(       '--acm-sigda-89'     , type='string'      , dest='acmSigdaName'   , help='An ACM/SIGDA 89 bench name to load, whithout extension.')
      parser.add_option(       '--blif'             , type='string'      , dest='blifName'       , help='A Blif (Yosys) design name to load, whithout extension.')
      parser.add_option(       '--ispd-05'          , type='string'      , dest='ispd05name'     , help='An ISPD 05 bench (placement) name to load, whithout extension.')
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
      (options, args) = parser.parse_args()
      args.insert(0, 'cgt')

      af = CRL.AllianceFramework.get()
      print af.getEnvironment().getPrint()

     #Hurricane.trace(True)
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
      if options.hTracksLocal:    Cfg.getParamInt       ('kite.hTracksReservedLocal').setInt(options.hTracksLocal)
      if options.vTracksLocal:    Cfg.getParamInt       ('kite.vTracksReservedLocal').setInt(options.vTracksLocal)
      if options.eventsLimit:     Cfg.getParamInt       ('kite.eventsLimit'         ).setInt(options.eventsLimit)
      if options.topRoutingLayer: Cfg.getParamString    ('katabatic.topRoutingLayer').setString(options.topRoutingLayer)

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
          unicorn.registerTool        (Kite.GraphicKiteEngine.grab())
         #unicorn.setAnonNetSelectable(False)
          unicorn.setLayerVisible     ("grid"          , False);
          unicorn.setLayerVisible     ("text.instance" , False);
          unicorn.setLayerVisible     ("text.component", False);
          
          if options.script:
              runScript(options.script,unicorn)
          
          setCgtBanner(unicorn.getBanner())
          print unicorn.getBanner()
          print credits()
    
          if cell: unicorn.setCell(cell)
          unicorn.show()
          ha.qtExec()
      else:
         # Run in command line mode.
          kiteSuccess = False
          
          if runEtesianTool:
              etesian = Etesian.EtesianEngine.create(cell)
             #if options.showConf: etesian.printConfiguration()
              etesian.place()

          if detailRoute and not (loadGlobal or globalRoute): globalRoute = True
          runKiteTool = loadGlobal or globalRoute or detailRoute

          if runKiteTool:
              if loadGlobal: globalFlags = Kite.KtLoadGlobalRouting
              else:          globalFlags = Kite.KtBuildGlobalRouting

              routingNets = []
              kite = Kite.KiteEngine.create(cell)
              if options.showConf: kite.printConfiguration()
    
              kite.runGlobalRouter(globalFlags)
              if saveGlobal: kite.saveGlobalSolution()
    
              if detailRoute:
                  kite.loadGlobalRouting( Katabatic.EngineLoadGrByNet, routingNets )
                  kite.layerAssign      ( Katabatic.EngineNoNetLayerAssign )
                  kite.runNegociate     ()
                  kiteSuccess = kite.getToolSuccess()
                  kite.finalizeLayout()
                  if options.dumpMeasures:
                      kite.dumpMeasures()
                  kite.destroy()
    
          if options.saveDesign:
              views = CRL.Catalog.State.Physical
              if options.saveDesign != cell.getName():
                  cell.setName(options.saveDesign)
                  views |= CRL.Catalog.State.Logical
              af.saveCell(cell, views)

          sys.exit(not kiteSuccess)

    except Exception, e:
      print e

    sys.exit(0)

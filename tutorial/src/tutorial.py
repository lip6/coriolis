#!/usr/bin/env python3


try:
   import sys
   import os.path
   import optparse
   from   coriolis import Cfg
   from   coriolis import Hurricane
   from   coriolis import Viewer
   from   coriolis import helpers.io
   from   coriolis import CRL
   from   coriolis import Unicorn
   from   coriolis import Tutorial
except ImportError as e:
   serror = str(e)
   if serror.startswith('No module named'):
      module = serror.split()[-1]
      print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(module) )
      print( '        Please check the integrity of the <coriolis> package.' )
   if serror.find('cannot open shared object file'):
      library = serror.split(':')[0]
      print( '[ERROR] The "{}" shared library cannot be loaded.'.format(library) )
      print( '        Under RHEL 6, you must be under devtoolset-2.' )
      print( '        (scl enable devtoolset-2 bash)' )
   sys.exit(1)
except Exception as e:
   print( '[ERROR] A strange exception occurred while loading the basic Coriolis/Python' )
   print( '        modules. Something may be wrong at Python/C API level.\n' )
   print( '        {}'.format(e) )
   sys.exit(2)


def setCgtBanner ( banner ):
    banner.setName('tutu')
    banner.setPurpose('Tutorial for Users')
    return banner


def credits ():
    s  = ''
    s +=  '        Tool Credits\n'
    s +=  '        Hurricane .................... Remy Escassut & Christian Masson\n'
    return s


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
        parser.add_option( '-v', '--verbose'          , action='store_true', dest='verbose'        , help='First level of verbosity.')
        parser.add_option( '-V', '--very-verbose'     , action='store_true', dest='veryVerbose'    , help='Second level of verbosity.')
        parser.add_option( '-i', '--info'             , action='store_true', dest='info'           , help='Display lots of informational messages.')
        parser.add_option(       '--paranoid'         , action='store_true', dest='paranoid'       , help='Display everything that *may be* suspicious...')
        parser.add_option( '-b', '--bug'              , action='store_true', dest='bug'            , help='Display bug related messages.')
        parser.add_option(       '--show-conf'        , action='store_true', dest='showConf'       , help='Display Kite configuration.')
        parser.add_option( '-D', '--core-dump'        , action='store_true', dest='coreDump'       , help='Enable core-dump when a crash occurs.')
        parser.add_option( '-L', '--log-mode'         , action='store_true', dest='logMode'        , help='Disable ANSI escape sequences in console output.')
        (options, args) = parser.parse_args()
        args.insert(0, 'tutu')
      
        flags = 0
        if options.noInit:
          flags |= CRL.AllianceFramework.NoPythonInit
      
        af = CRL.AllianceFramework.create( flags )
        print( af.getEnvironment().getPrint() )
      
        Cfg.Configuration.pushDefaultPriority(Cfg.Parameter.Priority.CommandLine)
        if options.coreDump:    Cfg.getParamBool('misc.catchCore'    ).setBool(False)
        if options.verbose:     Cfg.getParamBool('misc.verboseLevel1').setBool(True)
        if options.veryVerbose: Cfg.getParamBool('misc.verboseLevel2').setBool(True)
        if options.info:        Cfg.getParamBool('misc.info'         ).setBool(True)
        if options.paranoid:    Cfg.getParamBool('misc.paranoid'     ).setBool(True)
        if options.bug:         Cfg.getParamBool('misc.bug'          ).setBool(True)
        if options.logMode:     Cfg.getParamBool('misc.logMode'      ).setBool(True)
        if options.showConf:    Cfg.getParamBool('misc.showConf'     ).setBool(True)
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
        
       # Run in graphic mode.
        ha = Viewer.HApplication.create(args)
        Viewer.Graphics.enable()
        unicorn = Unicorn.UnicornGui.create()
        unicorn.setApplicationName  ('tutu')
        unicorn.registerTool        (Tutorial.GraphicTutorialEngine.grab())
        unicorn.setLayerVisible     ("grid"          , False);
        unicorn.setLayerVisible     ("text.instance" , False);
        unicorn.setLayerVisible     ("text.component", False);
        
        setCgtBanner(unicorn.getBanner())
        print( unicorn.getBanner() )
        print( credits() )
        if cell: unicorn.setCell(cell)
        unicorn.show()
        ha.qtExec()
    except Exception as e:
        helpers.io.catch( e )
    sys.exit(0)

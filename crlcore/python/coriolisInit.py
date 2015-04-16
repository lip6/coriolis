
try:
  import sys
  import os
  import os.path
  import copy
  import Cfg
  import Hurricane
  import Viewer
  import CRL
  from   CRL                     import Environment
  from   CRL                     import RoutingLayerGauge
  from   helpers                 import Configuration
  from   helpers                 import ErrorMessage
  from   helpers                 import WarningMessage
  from   helpers.Configuration   import TypeBool
  from   helpers.Configuration   import TypeInt
  from   helpers.Configuration   import TypeEnumerate
  from   helpers.Configuration   import TypePercentage
  from   helpers.Configuration   import TypeDouble
  from   helpers.Configuration   import TypeString
  from   helpers.Configuration   import TypeTab
  from   helpers.Configuration   import TypeTitle
  from   helpers.Configuration   import TypeSection
  from   helpers.Configuration   import TypeRule
  from   helpers.Configuration   import TypeOption
  from   helpers                 import Patterns
  from   helpers                 import Display
  from   helpers.Display         import Style
  from   helpers.Display         import Inherit
  from   helpers.Display         import Darkening
  from   helpers.Display         import Group
  from   helpers.Display         import Drawing
  from   helpers                 import Alliance
  from   helpers                 import SymbolicTechnology
  from   helpers                 import RealTechnology
  import helpers.Display
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


helpers.staticInitialization()
moduleGlobals       = globals()

SystemFile          = 0x0001
AllianceHelper      = 0x0002
SymbolicHelper      = 0x0004
RealHelper          = 0x0008
PatternsHelper      = 0x0010
DisplayHelper       = 0x0020
ConfigurationHelper = 0x0040
KiteHelper          = 0x0080
HelpersMask         = AllianceHelper      \
                    | SymbolicHelper      \
                    | RealHelper          \
                    | PatternsHelper      \
                    | DisplayHelper       \
                    | ConfigurationHelper \
                    | KiteHelper

SystemMandatory     = 0x0100


def coriolisConfigure():
  global symbolicTechno

  confHelpers  = ( ('allianceConfig'     , Alliance.loadAllianceConfig          , SystemMandatory|AllianceHelper)
                 , ('routingGaugesTable' , Alliance.loadRoutingGaugesTable      , SystemMandatory|KiteHelper)
                 , ('cellGaugesTable'    , Alliance.loadCellGaugesTable         , SystemMandatory|KiteHelper)
                 , ('viewerConfig'       , SymbolicTechnology.loadViewerConfig  , SystemMandatory|SymbolicHelper)
                 , ('realLayersTable'    , SymbolicTechnology.loadRealLayers    , SystemMandatory|SymbolicHelper)
                 , ('symbolicLayersTable', SymbolicTechnology.loadSymbolicLayers, SystemMandatory|SymbolicHelper)
                 , ('symbolicRulesTable' , SymbolicTechnology.loadSymbolicRules , SystemMandatory|SymbolicHelper)
                 , ('workingLayersTable' , SymbolicTechnology.loadWorkingLayers , SystemMandatory|SymbolicHelper)
                 , ('technoConfig'       , RealTechnology.loadTechnoConfig      , SystemMandatory|RealHelper)
                 , ('gdsLayersTable'     , RealTechnology.loadGdsLayers         , SystemMandatory|RealHelper)
                 , ('patternsTable'      , Patterns.loadPatterns                , SystemMandatory|PatternsHelper)
                 , ('stylesTable'        , Display.loadStyles                   , SystemMandatory|DisplayHelper)
                 , ('defaultStyle'       , Display.loadDefaultStyle             , SystemMandatory|DisplayHelper)
                 , ('parametersTable'    , Configuration.loadParameters         , ConfigurationHelper)
                 , ('layoutTable'        , Configuration.loadLayout             , ConfigurationHelper)
                 )

  print '       o  Running configuration hook: coriolisConfigure().'
 #print '          - sysConfDir: <%s>' % helpers.sysConfDir

  Cfg.Configuration.pushDefaultPriority ( Cfg.Parameter.Priority.ConfigurationFile )

  confFiles = [ (helpers.symbolicDir+'/alliance.conf'  , SystemFile|AllianceHelper)
              , (helpers.symbolicDir+'/technology.conf', SystemFile|SymbolicHelper)
              , (helpers.realDir    +'/technology.conf', SystemFile|RealHelper)
              , (helpers.symbolicDir+'/patterns.conf'  , SystemFile|PatternsHelper)
              , (helpers.symbolicDir+'/display.conf'   , SystemFile|DisplayHelper)
              , (helpers.symbolicDir+'/misc.conf'      , SystemFile|ConfigurationHelper)
              , (helpers.symbolicDir+'/etesian.conf'   , SystemFile|ConfigurationHelper)
              , (helpers.symbolicDir+'/kite.conf'      , SystemFile|ConfigurationHelper|KiteHelper)
              , (helpers.symbolicDir+'/stratus1.conf'  , SystemFile|ConfigurationHelper)
              , (helpers.symbolicDir+'/plugins.conf'   , SystemFile|ConfigurationHelper)
              ]
  if os.getenv('HOME'):
    confFiles   += [ (os.getenv('HOME')+'/.coriolis2/settings.py', 0) ]
  else:
    w = WarningMessage(['The <HOME> environment variable is not defined, this is most unusual.'
                       ,'It prevents the loading of ${HOME}/.coriolis2/settings.py'])
    print w

  confFiles   += [ (os.getcwd()+'/.coriolis2/settings.py', 0) ]

  for confFile, confFlags in confFiles:
    if confFile.endswith('settings.py'):
      Cfg.Configuration.pushDefaultPriority ( Cfg.Parameter.Priority.UserFile )
    try:
      if not os.path.isfile(confFile):
        if confFlags & SystemFile:
          print '[ERROR] Missing mandatory Coriolis2 system file:'
          print '        <%s>' % confFile
          print '        Your installation may be broken. Trying to continue anyway...'
        continue

      print '          - Loading \"%s\".' % helpers.truncPath(confFile)
      execfile(confFile,moduleGlobals)
    except Exception, e:
      print '[ERROR] An exception occured while loading the configuration file:'
      print '        <%s>\n' % (confFile)
      print '        You should check for simple python errors in this file.'
      print '        Error was:'
      print '        %s\n' % e
      print '        Trying to continue anyway...'

    for symbol, loader, loaderFlags in confHelpers:
      if moduleGlobals.has_key(symbol):
        loader( moduleGlobals[symbol], confFile )
        del moduleGlobals[symbol]
      else:
        if confFlags & loaderFlags & HelpersMask:
          if confFlags & SystemFile and loaderFlags & SystemMandatory:
            print '[ERROR] Mandatory symbol <%s> is missing in system configuration file:' % symbol
            print '        <%s>' % confFile
            print '        Trying to continue anyway...'

    if confFile.endswith('settings.py'):
      Cfg.Configuration.popDefaultPriority ()
  
  Cfg.Configuration.popDefaultPriority ()

 #sys.stdout.write(CRL.AllianceFramework.get().getEnvironment().getPrint())

  return

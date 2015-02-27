
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
PatternsHelper      = 0x0004
DisplayHelper       = 0x0008
ConfigurationHelper = 0x0010
HelpersMask         = AllianceHelper|PatternsHelper|DisplayHelper|ConfigurationHelper

SystemMandatory     = 0x0100
DisabledByXml       = 0x0200


def coriolisConfigure():
  global symbolicTechno

  confHelpers  = ( ('allianceConfig'    , Alliance.loadAllianceConfig    , SystemMandatory|DisabledByXml|AllianceHelper)
                 , ('routingGaugesTable', Alliance.loadRoutingGaugesTable, SystemMandatory|DisabledByXml|AllianceHelper)
                 , ('cellGaugesTable'   , Alliance.loadCellGaugesTable   , SystemMandatory|DisabledByXml|AllianceHelper)
                 , ('patternsTable'     , Patterns.loadPatterns          , SystemMandatory|DisabledByXml|PatternsHelper)
                 , ('stylesTable'       , Display.loadStyles             , SystemMandatory|DisabledByXml|DisplayHelper)
                 , ('defaultStyle'      , Display.loadDefaultStyle       , SystemMandatory|DisabledByXml|DisplayHelper)
                 , ('parametersTable'   , Configuration.loadParameters   , DisabledByXml|ConfigurationHelper)
                 , ('layoutTable'       , Configuration.loadLayout       , DisabledByXml|ConfigurationHelper)
                 )

  helpers.xmlCompatMode = False

  print '       o  Running configuration hook: coriolisConfigure().'
 #print '          - sysConfDir: <%s>' % helpers.sysConfDir

  Cfg.Configuration.pushDefaultPriority ( Cfg.Parameter.Priority.ConfigurationFile )

  technoFiles  = [ helpers.sysConfDir+'/techno.conf' ]
  if os.getenv('HOME'):
    technoFiles += [ os.getenv('HOME')+'/.coriolis2/techno.py' ]
  technoFiles += [ os.getcwd()+'/.coriolis2/techno.py' ]

  technoFiles.reverse()
  for technoFile in technoFiles:
    if os.path.isfile(technoFile):
      print '          - Loading \"%s\".' % helpers.truncPath(technoFile)
      execfile(technoFile,moduleGlobals)
      break
  if moduleGlobals.has_key('symbolicTechno'):
    helpers.symbolicTechno = symbolicTechno
    helpers.symbolicDir    = os.path.join( helpers.sysConfDir, symbolicTechno )
  else:
    print '[ERROR] The symbolic technology name is not set. Using <cmos>.' 
  if moduleGlobals.has_key('realTechno'):
    helpers.realTechno = realTechno
    helpers.realDir    = os.path.join( helpers.sysConfDir, realTechno )
  else:
    print '[ERROR] The real technology name is not set. Using <hcmos9>.' 
    

  confFiles    = [ (helpers.sysConfDir+'/'+symbolicTechno+'/alliance.conf', SystemFile|AllianceHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/patterns.conf', SystemFile|PatternsHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/display.conf' , SystemFile|DisplayHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/misc.conf'    , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/hMetis.conf'  , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/nimbus.conf'  , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/mauka.conf'   , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/etesian.conf' , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/kite.conf'    , SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/stratus1.conf', SystemFile|ConfigurationHelper)
                 , (helpers.sysConfDir+'/'+symbolicTechno+'/plugins.conf' , SystemFile|ConfigurationHelper)
                 ]
  if os.getenv('HOME'):
    confFiles   += [ (os.getenv('HOME')+'/.coriolis2/settings.py', 0) ]
  else:
    w = WarningMessage(['The <HOME> environment variable is not defined, this is most unusual.'
                       ,'It prevents the loading of ${HOME}/.coriolis2/settings.py'])
    print w

  confFiles   += [ (os.getcwd()+'/.coriolis2/settings.py', 0) ]


  if helpers.xmlCompatMode:
    Alliance.loadCompatXml()
    Configuration.loadCompatXml()

  for confFile, confFlags in confFiles:
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
      if not (loaderFlags & DisabledByXml and helpers.xmlCompatMode):
        if moduleGlobals.has_key(symbol):
          loader( moduleGlobals[symbol], confFile )
          del moduleGlobals[symbol]
        else:
          if confFlags & loaderFlags & HelpersMask:
            if confFlags & SystemFile and loaderFlags & SystemMandatory:
              print '[ERROR] Mandatory symbol <%s> is missing in system configuration file:' % symbol
              print '        <%s>' % confFile
              print '        Trying to continue anyway...'
  
  Cfg.Configuration.popDefaultPriority ()

 #sys.stdout.write(CRL.AllianceFramework.get().getEnvironment().getPrint())

  return

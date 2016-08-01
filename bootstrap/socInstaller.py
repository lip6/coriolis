#!/usr/bin/env python
#
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2015-2016, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |  C o r i o l i s  /  C h a m s   I n s t a l l e r              |
# |                                                                 |
# |  Authors     :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./socInstaller.py"                            |
# +-----------------------------------------------------------------+
#
# WARNING:
#   This script has been designed only for internal use in the
#   LIP6/SoC department. If you want to use it you will need to
#   change the hardwired configuration.


showTrace = True

try:
  import sys
  import os.path
  import shutil
  import optparse
  import time
  import traceback
  import distutils.sysconfig
  import subprocess
  import socket
  import re
  import smtplib
  from email.mime.text        import MIMEText
  from email.mime.multipart   import MIMEMultipart
  from email.mime.application import MIMEApplication
except ImportError, e:
  module = str(e).split()[-1]


class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
      self._code   = code
      self._errors = [ 'Malformed call to ErrorMessage()', '%s' % str(arguments) ]

      text = None
      if len(arguments) == 1:
        if isinstance(arguments[0],Exception): text = str(arguments[0]).split('\n')
        else:
          self._errors = arguments[0]
      elif len(arguments) > 1:
        text = list(arguments)

      if text:
        self._errors = []
        while len(text[0]) == 0: del text[0]

        lstrip = 0
        if text[0].startswith('[ERROR]'): lstrip = 8

        for line in text:
          if line[0:lstrip  ] == ' '*lstrip or \
             line[0:lstrip-1] == '[ERROR]':
            self._errors += [ line[lstrip:] ]
          else:
            self._errors += [ line.lstrip() ]
      return

    def __str__ ( self ):
        if not isinstance(self._errors,list):
          return "[ERROR] %s" % self._errors

        formatted = "\n"
        for i in range(len(self._errors)):
          if i == 0: formatted += "[ERROR] %s" % self._errors[i]
          else:      formatted += "        %s" % self._errors[i]
          if i+1 < len(self._errors): formatted += "\n"
        return formatted

    def addMessage ( self, message ):
        if not isinstance(self._errors,list):
          self._errors = [ self._errors ]
        if isinstance(message,list):
          for line in message:
              self._errors += [ line ]
        else:
          self._errors += [ message ]
        return

    def terminate ( self ):
        print self
        sys.exit(self._code)

    @property
    def code ( self ): return self._code


class BadBinary ( ErrorMessage ):

    def __init__ ( self, binary ):
      ErrorMessage.__init__( self, 1, "Binary not found: <%s>." % binary )
      return


class BadReturnCode ( ErrorMessage ):

    def __init__ ( self, status ):
      ErrorMessage.__init__( self, 1, "Command returned status:%d." % status )
      return


class Command ( object ):

    def __init__ ( self, arguments, fdLog=None ):
      self.arguments = arguments
      self.fdLog     = fdLog

      if self.fdLog != None and not isinstance(self.fdLog,file):
        print '[WARNING] Command.__init__(): <fdLog> is neither None or a file.'
      return

    def _argumentsToStr ( self, arguments ):
      s = ''
      for argument in arguments:
        if argument.find(' ') >= 0: s += ' "' + argument + '"'
        else:                       s += ' '  + argument
      return s

    def log ( self, text ):
      print text[:-1]
      sys.stdout.flush()
      sys.stderr.flush()
      if isinstance(self.fdLog,file):
        self.fdLog.write( text )
        self.fdLog.flush()
      return

    def execute ( self ):
      global conf
      sys.stdout.flush()
      sys.stderr.flush()

      homeDir = os.environ['HOME']
      workDir = os.getcwd()
      if homeDir.startswith(homeDir):
         workDir = '~' + workDir[ len(homeDir) : ]
      prompt = '%s@%s:%s$' % (os.environ['USER'],conf.masterHost,workDir)

      try:
        self.log( '%s%s\n' % (prompt,self._argumentsToStr(self.arguments)) )
        child = subprocess.Popen( self.arguments, stdout=subprocess.PIPE, stderr=subprocess.STDOUT )

        while True:
          line = child.stdout.readline()
          if not line: break

          self.log( line )
      except OSError, e:
        raise BadBinary( self.arguments[0] )

      (pid,status) = os.waitpid( child.pid, 0 )
      status >>= 8
      if status != 0:
        raise BadReturnCode( status )

      return


class GitRepository ( object ):

    @staticmethod
    def getLocalRepository ( url ):
      localRepo = url.split( '/' )[-1]
      if localRepo.endswith('.git'):
        localRepo = localRepo[:-4]
      return localRepo

    def __init__ ( self, url, cloneDir, fdLog=None ):
      self.url       = url
      self.cloneDir  = cloneDir
      self.localRepo = GitRepository.getLocalRepository( url )
      self.fdLog     = fdLog
      return

    @property
    def localRepoDir ( self ): return self.cloneDir+'/'+self.localRepo

    def removeLocalRepo ( self ):
      if os.path.isdir(self.localRepoDir):
        print 'Removing Git local repository: <%s>' % self.localRepoDir
        shutil.rmtree( self.localRepoDir )
      return

    def clone ( self ):
      print 'Clone/pull from:', self.url
      if not os.path.isdir(self.cloneDir):
        os.makedirs( self.cloneDir )

      if not os.path.isdir(self.localRepoDir):
        os.chdir( self.cloneDir )
        Command( [ 'git', 'clone', self.url ], self.fdLog ).execute()
      else:
        os.chdir( self.localRepoDir )
        Command( [ 'git', 'pull' ], self.fdLog ).execute()
      return

    def checkout ( self, branch ):
      os.chdir( self.localRepoDir )
      Command( [ 'git', 'checkout', branch ], self.fdLog ).execute()
      return


class Configuration ( object ):

    PrimaryNames = \
        [ 'sender'      , 'receivers'
        , 'coriolisRepo', 'chamsRepo' , 'benchsRepo', 'supportRepos'
        , 'homeDir'     , 'masterHost'
        , 'debugArg'    , 'nightlyMode'
        , 'rmSource'    , 'rmBuild', 'doGit', 'doBuild', 'doBenchs', 'doSendReport'
        , 'success'     , 'rcode'
        ]
    SecondaryNames = \
        [ 'rootDir', 'srcDir', 'logDir', 'logs', 'fds'
        ]

    def __init__ ( self ):
      self._sender       = 'Jean-Paul.Chaput@soc.lip6.fr'
      self._receivers    = [ 'Jean-Paul.Chaput@lip6.fr', 'Eric.Lao@lip6.fr' ]
      self._supportRepos = [ 'http://github.com/miloyip/rapidjson' ]
      self._coriolisRepo = 'https://www-soc.lip6.fr/git/coriolis.git'
      self._chamsRepo    = 'file:///users/outil/chams/chams.git'
      self._benchsRepo   = 'https://www-soc.lip6.fr/git/alliance-check-toolkit.git'
      self._homeDir      = os.environ['HOME']
      self._debugArg     = ''
      self._rmSource     = False
      self._rmBuild      = False
      self._doGit        = True
      self._doBuild      = True
      self._doBenchs     = False
      self._doSendReport = True
      self._nightlyMode  = False
      self._logs         = { 'build':None, 'benchs':None }
      self._fds          = { 'build':None, 'benchs':None }
      self._masterHost   = self._detectMasterHost()
      self._success      = False
      self._rcode        = 0

      self._updateSecondaries()
      return

    def __setattr__ ( self, attribute, value ):
      if attribute in Configuration.SecondaryNames:
        print ErrorMessage( 1, 'Attempt to write in read-only attribute <%s> in Configuration.'%attribute )
        return

      if attribute == 'masterHost' or attribute == '_masterHost':
        if value == 'lepka':
          print 'Never touch the Git tree when running on <lepka>.'
          self._rmSource     = False
          self._rmBuild      = False
          self._doGit        = False
          self._doSendReport = False
          self._targets      = { 'SL6'   :None
                               , 'SL6_64':None
                               , 'SL7_64':'lepka'
                               }
        else:
          self._targets      = { 'SL6'   :'rock'
                               , 'SL6_64':'bip'
                               , 'SL7_64':None
                               }

      if attribute[0] == '_':
        self.__dict__[attribute] = value
        return

      if   attribute == 'homeDir': value = os.path.expanduser(value)

      self.__dict__['_'+attribute] = value
      self._updateSecondaries()
      return

    def __getattr__ ( self, attribute ):
      if attribute[0] != '_': attribute = '_'+attribute
      if not self.__dict__.has_key(attribute):
        raise ErrorMessage( 1, 'Configuration has no attribute <%s>.'%attribute )
      return self.__dict__[attribute]

    def _updateSecondaries ( self ):
      if self._nightlyMode:
        self._targets['SL6'] = None
        self._rootDir = self._homeDir + '/nightly/coriolis-2.x'
      else:
        if self._masterHost != 'lepka':
          self._targets['SL6'] = 'rock'
        self._rootDir = self._homeDir + '/coriolis-2.x'
      self._srcDir    = self._rootDir + '/src'
      self._logDir    = self._srcDir  + '/logs'
      return

    def _detectMasterHost ( self ):
      masterHost = 'unknown'
      hostname   = socket.gethostname()
      hostAddr   = socket.gethostbyname(hostname)
    
      if hostname == 'lepka' and hostAddr == '127.0.0.1':
        masterHost = 'lepka'
      else:
        masterHost = hostname.split('.')[0]
      return masterHost

    def openLog ( self, stem ):
      if not os.path.isdir(self._logDir):
        os.makedirs( self._logDir )
  
      index   = 0
      timeTag = time.strftime( "%Y.%m.%d" )
      while True:
          logFile = os.path.join(self._logDir,"%s-%s-%02d.log" % (stem,timeTag,index))
          if not os.path.isfile(logFile):
              print "Report log: <%s>" % logFile
              break
          index += 1
      fd = open( logFile, "w" )
      self._logs[stem] = logFile
      self._fds [stem] = fd
      return

    def closeLogs ( self ):
      for fd in self._fds.values():
        if fd: fd.close()
      return


class Report ( object ):

    def __init__ ( self, conf ):
      self.conf = conf

      commaspace = ', '
      date       = time.strftime( "%A %d %B %Y" )
      stateText  = 'FAILED'
      modeText   = 'SoC installation'
      if self.conf.success:     stateText = 'SUCCESS'
      if self.conf.nightlyMode: modeText  = 'Nightly build'

      self.message = MIMEMultipart()
      self.message['Subject'] = '[%s] Coriolis & Chams %s %s' % (stateText,modeText,date)
      self.message['From'   ] = self.conf.sender
      self.message['To'     ] = commaspace.join( self.conf.receivers )
      self.attachements = []

      self.mainText  = '\n'
      self.mainText += 'Salut le Crevard,\n'
      self.mainText += '\n'
      if self.conf.nightlyMode:
        self.mainText += 'This is the nightly build report of Coriolis & Chams.\n'
      else:
        self.mainText += 'SoC installer report of Coriolis & Chams.\n'
      self.mainText += '%s\n' % date
      self.mainText += '\n'
      if self.conf.success:
        self.mainText += 'Build was SUCCESSFUL\n'
      else:
        self.mainText += 'Build has FAILED, please have a look to the attached log file(s).\n'
      self.mainText += '\n'
      self.mainText += 'Complete log file(s) can be found here:\n'
      return

    def attachLog ( self, logFile ):
      if not logFile: return

      fd = open( logFile, 'rb' )
      try:
        fd.seek( -1024*100, os.SEEK_END )
      except IOError, e:
        pass
      tailLines = ''
      for line in fd.readlines()[1:]:
        tailLines += line
      fd.close()
      self.mainText += '    <%s>\n' % logFile

      attachement = MIMEApplication(tailLines)
      attachement.add_header( 'Content-Disposition', 'attachment', filename=os.path.basename(logFile) )

      self.attachements.append( attachement )
      return

    def send ( self ):
      self.message.attach( MIMEText(self.mainText) )
      for attachement in self.attachements:
        self.message.attach( attachement )

      print "Sending mail report to <%s>" % self.conf.receivers
      session = smtplib.SMTP( 'localhost' )
      session.sendmail( self.conf.sender, self.conf.receivers, self.message.as_string() )
      session.quit()
      return


# ------------------------------------------------------------------- 
# <socInstaller> Main Part.


parser = optparse.OptionParser ()  
parser.add_option ( "--debug"       , action="store_true" ,                dest="debug"        , help="Build a <Debug> aka (-g) version." )
parser.add_option ( "--no-git"      , action="store_true" ,                dest="noGit"        , help="Do not pull/update Git repositories before building." )
parser.add_option ( "--no-build"    , action="store_true" ,                dest="noBuild"      , help="Do not rebuild the tools, must have already be done." )
parser.add_option ( "--no-report"   , action="store_true" ,                dest="noReport"     , help="Do not send a final report." )
parser.add_option ( "--nightly"     , action="store_true" ,                dest="nightly"      , help="Perform a nighly build." )
parser.add_option ( "--benchs"      , action="store_true" ,                dest="benchs"       , help="Run the <alliance-checker-toolkit> sanity benchs." )
parser.add_option ( "--rm-build"    , action="store_true" ,                dest="rmBuild"      , help="Remove the build/install directories." )
parser.add_option ( "--rm-source"   , action="store_true" ,                dest="rmSource"     , help="Remove the Git source repositories." )
parser.add_option ( "--rm-all"      , action="store_true" ,                dest="rmAll"        , help="Remove everything (source+build+install)." )
parser.add_option ( "--root"        , action="store"      , type="string", dest="rootDir"      , help="The root directory (default: <~/coriolis-2.x/>)." )
(options, args) = parser.parse_args ()

conf = Configuration()

try:
    if options.debug:                     conf.debugArg     = '--debug' 
    if options.nightly:                   conf.nightlyMode  = True
    if options.noGit:                     conf.doGit        = False
    if options.noBuild:                   conf.doBuild      = False
    if options.benchs:                    conf.doBenchs     = True
    if options.noReport:                  conf.doSendReport = False
    if options.rmSource or options.rmAll: conf.rmSource     = True
    if options.rmBuild  or options.rmAll: conf.rmBuild      = True

    gitSupports = []
    for supportRepo in conf.supportRepos:
      gitSupports.append( GitRepository( supportRepo, conf.srcDir+'/support' ) )
    gitCoriolis = GitRepository( conf.coriolisRepo, conf.srcDir, conf.fds['build'] )
    gitChams    = GitRepository( conf.chamsRepo   , conf.srcDir, conf.fds['build'] )
    gitBenchs   = GitRepository( conf.benchsRepo  , conf.srcDir, conf.fds['build'] )

    if conf.doGit:
      for gitSupport in gitSupports:
        if conf.rmSource: gitSupport.removeLocalRepo()
        gitSupport.clone()
       #if gitSupport.url.endswith('rapidjson'):
       #  gitSupport.checkout( 'a1c4f32' )

      if conf.rmSource: gitCoriolis.removeLocalRepo()
      gitCoriolis.clone   ()
      gitCoriolis.checkout( 'devel_anabatic' )
      
      if conf.rmSource: gitChams.removeLocalRepo()
      gitChams.clone   ()
      gitChams.checkout( 'devel' )

      if conf.rmSource: gitBenchs.removeLocalRepo()
      gitBenchs.clone()

    if conf.rmBuild:
      for entry in os.listdir(conf.rootDir):
        if entry.startswith('Linux.'):
          buildDir = conf.rootDir+'/'+entry
          print 'Removing OS build directory: <%s>' % buildDir
          shutil.rmtree( buildDir )

    ccbBin = gitCoriolis.localRepoDir+'/bootstrap/ccb.py'
    if not os.path.isfile( ccbBin ):
      raise ErrorMessage( 1, [ 'Cannot find <ccb.py>, should be here:'
                             , '   <%s>' % ccbBin
                             ] )

    if conf.doBuild:  conf.openLog( 'build' )
    if conf.doBenchs: conf.openLog( 'benchs' )

    buildCommand  = '%s --root=%s --project=support --project=coriolis --project=chams --make="-j%%d install" %%s' \
                     % (ccbBin,conf.rootDir)
    benchsCommand = 'cd %s/benchs && ./bin/go.sh clean && ./bin/go.sh lvx' \
                     % (gitBenchs.localRepoDir)

    commands = \
      [ ( conf.targets['SL7_64'], buildCommand % (3,conf.debugArg)                        , conf.fds['build' ] )
      , ( conf.targets['SL7_64'], buildCommand % (1,conf.debugArg+' --doc')               , conf.fds['build' ] )
      , ( conf.targets['SL7_64'], benchsCommand                                           , conf.fds['benchs'] )
      , ( conf.targets['SL6_64'], buildCommand % (6,conf.debugArg+' --devtoolset-2')      , conf.fds['build' ] )
      , ( conf.targets['SL6_64'], buildCommand % (1,conf.debugArg+' --devtoolset-2 --doc'), conf.fds['build' ] )
      , ( conf.targets['SL6_64'], benchsCommand                                           , conf.fds['benchs'] )
      , ( conf.targets['SL6']   , buildCommand % (2,conf.debugArg+' --devtoolset-2')      , conf.fds['build' ] )
      , ( conf.targets['SL6']   , buildCommand % (1,conf.debugArg+' --devtoolset-2 --doc'), conf.fds['build' ] )
      , ( conf.targets['SL6']   , benchsCommand                                           , conf.fds['benchs'] )
      ]

    for host,command,fd in commands:
      if host and fd:
        print 'Executing command on <%s>:' % host
        print ' %s' % command
        Command( [ 'ssh', host, command ], fd ).execute()

    conf.closeLogs()
    conf.success = True

except ErrorMessage, e:
  print e
  conf.closeLogs()
  conf.success = False

  if showTrace:
      print '\nPython stack trace:'
      traceback.print_tb( sys.exc_info()[2] )
  conf.rcode = e.code

if conf.doSendReport:
  report = Report( conf )
  report.attachLog( conf.logs['build' ] )
  report.attachLog( conf.logs['benchs'] )
  report.send()

sys.exit( conf.rcode )

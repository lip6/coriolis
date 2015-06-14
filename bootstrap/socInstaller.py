#!/usr/bin/env python
#
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2015-2015, All Rights Reserved
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

    def execute ( self ):
      sys.stdout.flush()
      sys.stderr.flush()

      try:
        child = subprocess.Popen( self.arguments, stdout=subprocess.PIPE, stderr=subprocess.STDOUT )

        while True:
          line = child.stdout.readline()
          if not line: break

          print line[:-1]
          sys.stdout.flush()

          if isinstance(self.fdLog,file):
            self.fdLog.write( line )
            self.fdLog.flush()
      except OSError, e:
        raise BadBinary( self.arguments[0] )

      (pid,status) = os.waitpid( child.pid, 0 )
      status >>= 8
      if status != 0:
        raise BadReturnCode( status )

      return


class GitRepository ( object ):

    @staticmethod
    def getLocalRepository ( gitRepository ):
      localRepo = gitRepository.split( '/' )[-1]
      if localRepo.endswith('.git'):
        localRepo = localRepo[:-4]
      return localRepo

    def __init__ ( self, url, cloneDir ):
      self.url       = url
      self.cloneDir  = cloneDir
      self.localRepo = GitRepository.getLocalRepository( url )
      return

    @property
    def localRepoDir ( self ): return self.cloneDir+'/'+self.localRepo

    def removeLocalRepo ( self ):
      if os.path.isdir(self.localRepoDir):
        print 'Removing Git local repository: <%s>' % self.localRepoDir
        shutil.rmtree( self.localRepoDir )
      return

    def clone ( self ):
      if not os.path.isdir(self.cloneDir):
        os.makedirs( self.cloneDir )

      if not os.path.isdir(self.localRepoDir):
        os.chdir( self.cloneDir )
        Command( [ 'git', 'clone', self.url ] ).execute()
      else:
        os.chdir( self.localRepoDir )
        Command( [ 'git', 'pull' ] ).execute()
      return

    def checkout ( self, branch ):
      os.chdir( self.localRepoDir )
      Command( [ 'git', 'checkout', branch ] ).execute()
      return


def openLog ( logDir, stem ):
    if not os.path.isdir(logDir):
      os.makedirs( logDir )

    index   = 0
    timeTag = time.strftime( "%Y.%m.%d" )
    while True:
        logFile = os.path.join(logDir,"%s-%s-%02d.log" % (stem,timeTag,index))
        if not os.path.isfile(logFile):
            print "Report log: <%s>" % logFile
            break
        index += 1
    fd = open( logFile, "w" )
    return (fd,logFile)


def sendReport ( state, buildLog, benchsLog, nightlyBuild ):
    sender   = 'Jean-Paul.Chaput@soc.lip6.fr'
    receiver = 'Jean-Paul.Chaput@lip6.fr'
    date     = time.strftime( "%A %d %B %Y" )

    stateText = 'FAILED'
    buildText = 'SoC installation'
    if state:        stateText = 'SUCCESS'
    if nightlyBuild: buildText = 'Nightly build'

    message = MIMEMultipart()
    message['Subject'] = '[%s] Coriolis & Chams %s %s' % (stateText,buildText,date)
    message['From'   ] = sender
    message['To'     ] = receiver

    mainText  = '\n'
    mainText += 'Salut le Crevard,\n'
    mainText += '\n'
    if nightlyBuild:
      mainText += 'This is the nightly build report of Coriolis & Chams.\n'
    else:
      mainText += 'SoC installer report of Coriolis & Chams.\n'
    mainText += '%s\n' % date
    mainText += '\n'
    if state:
      mainText += 'Build was SUCCESSFUL\n'
    else:
      mainText += 'Build has FAILED, please have a look to the attached log file.\n'
    mainText += '\n'
    mainText += 'Complete log file can be found here:\n'
    mainText += '    <%s>\n' % buildLog
    if benchsLog:
      mainText += '    <%s>\n' % benchsLog
    mainText += '\n'
    message.attach( MIMEText(mainText) )

    fd = open( buildLog, 'rb' )
    fd.seek( -1024*100, os.SEEK_END )
    tailLines = ''
    for line in fd.readlines()[1:]:
      tailLines += line
    message.attach( MIMEApplication(tailLines) )
    fd.close()

    if benchsLog:
      fd = open( benchsLog, 'rb' )
      fd.seek( -1024*100, os.SEEK_END )
      tailLines = ''
      for line in fd.readlines()[1:]:
        tailLines += line
      message.attach( MIMEApplication(tailLines) )
      fd.close()

    session = smtplib.SMTP( 'localhost' )
    session.sendmail( sender, receiver, message.as_string() )
    session.quit()
    return


def detectRunningHost ():
    runningHost = 'unknown'
    hostname    = socket.gethostname()
    hostAddr    = socket.gethostbyname(hostname)

    if hostname == 'lepka' and hostAddr == '127.0.0.1':
        print 'Running on <lepka>, watchout mode enabled.'
        runningHost = 'lepka'
    else:
        runningHost = hostname.split('.')[0]

    return runningHost


# ------------------------------------------------------------------- 
# <socInstaller> Main Part.


parser = optparse.OptionParser ()  
parser.add_option ( "--debug"       , action="store_true" ,                dest="debug"        , help="Build a <Debug> aka (-g) version." )
parser.add_option ( "--no-git"      , action="store_true" ,                dest="noGit"        , help="Do not pull/update Git repositories before building." )
parser.add_option ( "--no-report"   , action="store_true" ,                dest="noReport"     , help="Do not send a final report." )
parser.add_option ( "--nightly"     , action="store_true" ,                dest="nightly"      , help="Perform a nighly build." )
parser.add_option ( "--benchs"      , action="store_true" ,                dest="benchs"       , help="Run the <alliance-checker-toolkit> sanity benchs." )
parser.add_option ( "--rm-build"    , action="store_true" ,                dest="rmBuild"      , help="Remove the build/install directories." )
parser.add_option ( "--rm-source"   , action="store_true" ,                dest="rmSource"     , help="Remove the Git source repositories." )
parser.add_option ( "--rm-all"      , action="store_true" ,                dest="rmAll"        , help="Remove everything (source+build+install)." )
parser.add_option ( "--root"        , action="store"      , type="string", dest="rootDir"      , help="The root directory (default: <~/coriolis-2.x/>)." )
(options, args) = parser.parse_args ()

nightlyBuild  = False
fdBuildLog    = None
fdBenchsLog   = None
buildLogFile  = None
benchsLogFile = None

try:
    rmSource     = False
    rmBuild      = False
    doGit        = True
    doBench      = False
    doSendReport = True
    debugArg     = ''
    runningHost  = detectRunningHost()
    targetSL6    = 'rock'
    targetSL6_64 = 'bip'
    targetSL7_64 = None

    if options.debug:    debugArg     = '--debug' 
    if options.nightly:  nightlyBuild = True
    if options.noGit:    doGit        = False
    if options.benchs:   doBenchs     = True
    if options.noReport: doSendReport = False
    if options.rmSource or options.rmAll: rmSource = True
    if options.rmBuild  or options.rmAll: rmBuild  = True

    if runningHost == 'lepka':
      print 'Never touch the Git tree when running on <lepka>.'
      doGit        = False
      rmSource     = False
      rmBuild      = False
      targetSL7_64 = 'lepka'
      targetSL6    = None
      targetSL6_64 = None

    if nightlyBuild:
      targetSL6    = None

    allianceCheckRepo = 'https://www-soc.lip6.fr/git/alliance-check-toolkit.git'
    coriolisRepo      = 'https://www-soc.lip6.fr/git/coriolis.git'
    chamsRepo         = 'file:///users/outil/chams/chams.git'
    homeDir           = os.environ['HOME']
    rootDir           = homeDir + '/coriolis-2.x'
    if nightlyBuild:
      rootDir         = homeDir + '/nightly/coriolis-2.x'
    srcDir            = rootDir + '/src'
    logDir            = srcDir  + '/logs'

    gitCoriolis      = GitRepository( coriolisRepo     , srcDir )
    gitChams         = GitRepository( chamsRepo        , srcDir )
    gitAllianceCheck = GitRepository( allianceCheckRepo, srcDir )

    if doGit:
      if rmSource: gitCoriolis.removeLocalRepo()
      gitCoriolis.clone   ()
      gitCoriolis.checkout( 'devel' )
      
      if rmSource: gitChams.removeLocalRepo()
      gitChams.clone   ()
      gitChams.checkout( 'devel' )

      if rmSource: AllianceCheck.removeLocalRepo()
      AllianceCheck.clone   ()

    if rmBuild:
      for entry in os.listdir(rootDir):
        if entry.startswith('Linux.'):
          buildDir = rootDir+'/'+entry
          print 'Removing OS build directory: <%s>' % buildDir
          shutil.rmtree( buildDir )

    ccbBin = gitCoriolis.localRepoDir+'/bootstrap/ccb.py'
    if not os.path.isfile( ccbBin ):
      raise ErrorMessage( 1, [ 'Cannot find <ccb.py>, should be here:'
                             , '   <%s>' % ccbBin
                             ] )

    fdBuildLog,buildLogFile = openLog( logDir, 'build' )
    if doBenchs:
      fdBenchsLog,benchsLogFile = openLog( logDir, 'benchs' )

    buildCommand  = '%s --root=%s --project=coriolis --project=chams --make="-j%%d install" %%s' \
                     % (ccbBin,rootDir)
    benchsCommand = 'cd %s/benchs && ./bin/go.sh clean && ./bin/go.sh lvx' \
                     % (gitAllianceCheck.localRepoDir)

    if targetSL7_64:
      commands = [ ( targetSL7_64, buildCommand % (3,debugArg)         , fdBuildLog )
                 , ( targetSL7_64, buildCommand % (1,debugArg+' --doc'), fdBuildLog )
                 ]
      if doBenchs:
        commands += [ ( targetSL7_64, benchsCommand, fdBenchsLog ) ]
    if targetSL6_64:
      commands = [ ( targetSL6_64, buildCommand % (6,debugArg+' --devtoolset-2')      , fdBuildLog )
                 , ( targetSL6_64, buildCommand % (1,debugArg+' --devtoolset-2 --doc'), fdBuildLog )
                 ]
      if doBenchs:
        commands += [ ( targetSL6_64, benchsCommand, fdBenchsLog ) ]
    if targetSL6:
      commands = [ ( targetSL6, buildCommand % (2,debugArg+' --devtoolset-2')      , fdBuildLog )
                 , ( targetSL6, buildCommand % (1,debugArg+' --devtoolset-2 --doc'), fdBuildLog )
                 ]
      if doBenchs:
        commands += [ ( targetSL6, benchsCommand, fdBenchsLog ) ]

    for host,command,fd in commands:
      Command( [ 'ssh', host, command ], fd ).execute()

    fdBuildLog.close()
    if doBenchs: fdBenchsLog.close()

    if doSendReport:
      sendReport( True, buildLogFile, benchsLogFile, nightlyBuild )

except ErrorMessage, e:
  print e
  if fdBuildLog:  fdBuildLog.close()
  if fdBenchsLog: fdBenchsLog.close()
  if showTrace:
      print '\nPython stack trace:'
      traceback.print_tb( sys.exc_info()[2] )
  sendReport( False, buildLogFile, benchsLogFile, nightlyBuild )
  sys.exit( e.code )

sys.exit( 0 )

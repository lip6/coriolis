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


def openLog ( logDir ):
    if not os.path.isdir(logDir):
      os.makedirs( logDir )

    index   = 0
    timeTag = time.strftime( "%Y.%m.%d" )
    while True:
        logFile = os.path.join(logDir,"install-%s-%02d.log" % (timeTag,index))
        if not os.path.isfile(logFile):
            print "Report log: <%s>" % logFile
            break
        index += 1
    fd = open( logFile, "w" )
    return (fd,logFile)


def sendReport ( state, installLog ):
    sender   = 'Jean-Paul.Chaput@soc.lip6.fr'
    receiver = 'Jean-Paul.Chaput@lip6.fr'
    date     = time.strftime( "%A %d %B %Y" )

    stateText = 'FAILED'
    if state: stateText = 'SUCCESS'

    message = MIMEMultipart()
    message['Subject'] = '[%s] Coriolis & Chams Nighltly build %s' % (stateText,date)
    message['From'   ] = sender
    message['To'     ] = receiver

    mainText  = '\n'
    mainText += 'Salut le Crevard,\n'
    mainText += '\n'
    mainText += 'This is the nightly build report of Coriolis & Chams.\n'
    mainText += '%s\n' % date
    mainText += '\n'
    if state:
      mainText += 'Build was SUCCESSFUL\n'
    else:
      mainText += 'Build has FAILED, please have a look to the attached log file.\n'
    mainText += '\n'
    message.attach( MIMEText(mainText) )

    fd = open( installLog, 'rb' )
    message.attach( MIMEApplication(fd.read()) )
    fd.close()

    session = smtplib.SMTP( 'localhost' )
    session.sendmail( sender, receiver, message.as_string() )
    session.quit()
    return


# ------------------------------------------------------------------- 
# <socInstaller> Main Part.


parser = optparse.OptionParser ()  
parser.add_option ( "-r", "--release"     , action="store_true" ,                dest="release"      , help="Build a <Release> aka optimized version." )
parser.add_option ( "-d", "--debug"       , action="store_true" ,                dest="debug"        , help="Build a <Debug> aka (-g) version." )
parser.add_option (       "--nightly"     , action="store_true" ,                dest="nightly"      , help="Perform a nighly build." )
parser.add_option (       "--rm-build"    , action="store_true" ,                dest="rmBuild"      , help="Remove the build/install directories." )
parser.add_option (       "--rm-source"   , action="store_true" ,                dest="rmSource"     , help="Remove the Git source repositories." )
parser.add_option (       "--rm-all"      , action="store_true" ,                dest="rmAll"        , help="Remove everything (source+build+install)." )
parser.add_option (       "--root"        , action="store"      , type="string", dest="rootDir"      , help="The root directory (default: <~/coriolis-2.x/>)." )
(options, args) = parser.parse_args ()

fdLog   = None
logFile = None

try:
    nightlyBuild = False
    rmSource     = False
    rmBuild      = False
    if options.nightly: nightlyBuild = True
    if options.rmSource or options.rmAll: rmSource = True
    if options.rmBuild  or options.rmAll: rmBuild  = True

    coriolisRepo = 'https://www-soc.lip6.fr/git/coriolis.git'
    chamsRepo    = 'file:///users/outil/chams/chams.git'
    homeDir      = os.environ['HOME']
    rootDir      = homeDir + '/coriolis-2.x'
    if nightlyBuild:
      rootDir    = homeDir + '/nightly/coriolis-2.x'
    srcDir       = rootDir + '/src'
    logDir       = rootDir + '/log'


    gitCoriolis = GitRepository( coriolisRepo, srcDir )
    if rmSource: gitCoriolis.removeLocalRepo()
    gitCoriolis.clone   ()
    gitCoriolis.checkout( 'devel' )

    gitChams = GitRepository( chamsRepo, srcDir )
    if rmSource: gitChams.removeLocalRepo()
    gitChams.clone   ()
    gitChams.checkout( 'devel' )

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

    commands = \
      [ ( 'bip', '%s --root=%s --project=coriolis --project=chams --devtoolset-2 --make="-j6 install"' \
                  % (ccbBin,rootDir) )
      , ( 'bip', '%s --root=%s --project=coriolis --project=chams --devtoolset-2 --make="-j1 install" --doc' \
                  % (ccbBin,rootDir) )
      ]
    if not nightlyBuild:
      commands = \
        [ ( 'rock', '%s --root=%s --project=coriolis --project=chams --devtoolset-2 --make="-j2 install"' \
                     % (ccbBin,rootDir) )
        , ( 'rock', '%s --root=%s --project=coriolis --project=chams --devtoolset-2 --make="-j1 install" --doc' \
                     % (ccbBin,rootDir) )
        ]

    (fdLog,logFile) = openLog( logDir )

    for host,command in commands:
      Command( [ 'ssh', host, command ], fdLog ).execute()

    fdLog.close()
    sendReport( True, logFile )

except ErrorMessage, e:
  print e
  if fdLog: fdLog.close()
  if showTrace:
      print '\nPython stack trace:'
      traceback.print_tb( sys.exc_info()[2] )
  sendReport( False, logFile )
  sys.exit( e.code )

sys.exit( 0 )

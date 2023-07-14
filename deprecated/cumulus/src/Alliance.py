# -*- Mode:Python; explicit-buffer-name: "Alliance.py<cumulus>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/Alliance.py"                    |
# +-----------------------------------------------------------------+


import os
import os.path
import sys
import time
import stat
import copy
import subprocess
import inspect
import helpers
from   helpers.io import ErrorMessage
from   helpers.io import WarningMessage
from   helpers    import Dots


# Global display flags
ShowCommand  = 0x00001000
ShowLog      = 0x00002000
ShowDots     = 0x00004000
ForceRun     = 0x00008000

commandFlags = ShowCommand | ShowLog


class AddMode ( object ):

  Append  = 1
  Prepend = 2
  Replace = 3


class Gauge ( object ):

  Horizontal = 1
  Vertical   = 2
  PinOnly    = 4
  Default    = 8


class SearchPath ( object ):

   def __init__ ( self ):
     self._pathes = []
     return

   def add ( self, path, mode ):
     name = os.path.basename(path)
     if mode == AddMode.Append:
       self._pathes.append( (name,path) )
     elif mode == AddMode.Prepend:
       self._pathes.insert( 0, (name,path) )
     elif mode == AddMode.Replace:
       for ipath in range(len(self._pathes)):
         if self._pathes[ipath][0] == name:
           self._pathes[ipath] = (name, path)
           return
       self._pathes.append( (name,path) )
     return

   def toUnix ( self ):
     if not len(self._pathes): return ''

     s = ''
     for ipath in range(len(self._pathes)):
       if ipath > 0: s += ':'
       s += self._pathes[ipath][1]
     return s

   def __str__ ( self ):
     return self.toUnix()



class Environment ( object ):

    DefaultEnv    = 0x0001
    CommandEnv    = 0x0002
    Append        = 0x0004

    BehavioralExt = 0x0001
    StructuralExt = 0x0002
    PhysicalExt   = 0x0004
    ScriptExt     = 0x0008
    CSourceExt    = 0x0010
    DirectiveExt  = 0x0020
    IocExt        = 0x0040
    PatExt        = 0x0080
    AllExt        = 0x00ff

    extTable = { BehavioralExt : [ 'vbe' ]
               , StructuralExt : [ 'vst', 'al' ]
               , PhysicalExt   : [ 'ap' , 'cp' ]
               , ScriptExt     : [ 'sh' , 'py' ]
               , CSourceExt    : [ 'c'   ]
               , IocExt        : [ 'ioc' ]
               , DirectiveExt  : [ 'lax', 'boom', 'dly' ]
               , PatExt        : [ 'pat' ]
               }


    @staticmethod
    def symbolToFilename ( name, eflags ):
      extensions = []
      for iflag in range(0,len(Environment.extTable)):
        if eflags & pow(2,iflag): extensions += Environment.extTable[ pow(2,iflag) ]

      for extension in extensions:
        if name.endswith('_'+extension):
          l = len(extension)
          return name[:-l-1], name[-l:]

      return None, None

    @staticmethod
    def isExt ( ext, eflags ):
      extensions = []
      for iflag in range(0,len(Environment.extTable)):
        if eflags & pow(2,iflag): extensions += Environment.extTable[ pow(2,iflag) ]

      if ext in extensions: return True
      return False
       

    def __init__ ( self, flags ):
      self.mbkEnv = {}
      if flags & Environment.DefaultEnv:
        self.mbkEnv[ 'ALLIANCE_TOP'      ] = None
        self.mbkEnv[ 'LD_LIBRARY_PATH'   ] = None
        self.mbkEnv[ 'MBK_TARGET_LIB'    ] = None
        self.mbkEnv[ 'MBK_WORK_LIB'      ] = '.'
        self.mbkEnv[ 'MBK_CATA_LIB'      ] = SearchPath()
        self.mbkEnv[ 'MBK_CATAL_NAME'    ] = 'CATAL'
        self.mbkEnv[ 'MBK_OUT_LO'        ] = 'vst'
        self.mbkEnv[ 'MBK_OUT_PH'        ] = 'ap'
        self.mbkEnv[ 'MBK_IN_LO'         ] = 'vst'
        self.mbkEnv[ 'MBK_IN_PH'         ] = 'ap'
        self.mbkEnv[ 'MBK_SEPAR'         ] = '_'
        self.mbkEnv[ 'MBK_VDD'           ] = 'vdd'
        self.mbkEnv[ 'MBK_VSS'           ] = 'vss'
        self.mbkEnv[ 'RDS_TECHNO_NAME'   ] = None
        self.mbkEnv[ 'GRAAL_TECHNO_NAME' ] = None
      else:
        self.mbkEnv[ 'ALLIANCE_TOP'      ] = None
        self.mbkEnv[ 'LD_LIBRARY_PATH'   ] = None
        self.mbkEnv[ 'MBK_TARGET_LIB'    ] = None
        self.mbkEnv[ 'MBK_WORK_LIB'      ] = None
        self.mbkEnv[ 'MBK_CATA_LIB'      ] = SearchPath()
        self.mbkEnv[ 'MBK_CATAL_NAME'    ] = None
        self.mbkEnv[ 'MBK_OUT_LO'        ] = None
        self.mbkEnv[ 'MBK_OUT_PH'        ] = None
        self.mbkEnv[ 'MBK_IN_LO'         ] = None
        self.mbkEnv[ 'MBK_IN_PH'         ] = None
        self.mbkEnv[ 'MBK_SEPAR'         ] = None
        self.mbkEnv[ 'MBK_VDD'           ] = None
        self.mbkEnv[ 'MBK_VSS'           ] = None
        self.mbkEnv[ 'RDS_TECHNO_NAME'   ] = None
        self.mbkEnv[ 'GRAAL_TECHNO_NAME' ] = None
      return


    def load ( self, allianceConfig, allianceFile ):
      entryNo = 0
      for entry in allianceConfig:
        entryNo += 1
    
        try:
          if len(entry) != 2:
            raise ErrorMessage(1,['Malformed entry in <allianceConfig>.'
                                 ,'Must have exactly two fields ("key", <value>).'
                                 ,str(entry)
                                 ])
    
          key, value = entry
          if key == 'ALLIANCE_TOP':        self.mbkEnv[ key ] = value
          if key == 'GRAAL_TECHNO_NAME':   self.mbkEnv[ key ] = value
          if key == 'RDS_TECHNO_NAME':     self.mbkEnv[ key ] = value
          if key == 'CATALOG':             self.mbkEnv[ key ] = value
          if key == 'SCALE_X':             self.mbkEnv[ key ] = value
          if key == 'IN_LO':               self.mbkEnv[ key ] = value
          if key == 'IN_PH':               self.mbkEnv[ key ] = value
          if key == 'OUT_PH':              self.mbkEnv[ key ] = value
          if key == 'OUT_LO':              self.mbkEnv[ key ] = value
          if key == 'POWER':               self.mbkEnv[ key ] = value
          if key == 'GROUND':              self.mbkEnv[ key ] = value
          if key == 'MBK_TARGET_LIB':      self.mbkEnv[ key ] = value
          if key == 'WORKING_LIBRARY':     self.mbkEnv[ key ] = value
          if key == 'SYSTEM_LIBRARY':
            for libraryEntry in value:
              if len(libraryEntry) != 2:
                  raise ErrorMessage(1,['Malformed system library entry in <allianceConfig>.'
                                       ,'Must have exactly two fields ("path", <mode>).'
                                       ,str(libraryEntry)
                                       ])
              libPath, mode = libraryEntry
              self.mbkEnv['MBK_CATA_LIB'].add( libPath, mode )
    
        except Exception, e:
            e = ErrorMessage( e )
            e.addMessage( 'In %s:<Alliance> at index %d.' % (allianceFile,entryNo) )
            print e

      self.mbkEnv[ 'LD_LIBRARY_PATH' ] = self.mbkEnv[ 'ALLIANCE_TOP' ] + '/lib'
      return


    def clone ( self ):
      return copy.deepcopy(self)


    def toSystemEnv ( self ):
      env = os.environ
      for key in self.mbkEnv.keys():
        if not self.mbkEnv[key]:
          print WarningMessage( 'Environment variable <%s> is not set.' % key )
          continue
        env[ key ] = str(self.mbkEnv[ key ])
      return env


class EnvironmentWrapper ( object ):

    def __init__ ( self, env ):
      self._env = env.clone()
      return

    def toSystemEnv ( self ): return self._env.toSystemEnv()

    @property
    def env ( self ): return self._env

    @property
    def TARGET_LIB        ( self ): return self._env.mbkEnv[ 'MBK_TARGET_LIB' ]
    @property             
    def WORK_LIB          ( self ): return self._env.mbkEnv[ 'MBK_WORK_LIB']
    @property             
    def CATA_LIB          ( self ): return self._env.mbkEnv[ 'MBK_CATA_LIB']
    @property             
    def CATAL_NAME        ( self ): return self._env.mbkEnv[ 'MBK_CATAL_NAME' ]
    @property             
    def OUT_LO            ( self ): return self._env.mbkEnv[ 'MBK_OUT_LO' ]
    @property             
    def OUT_PH            ( self ): return self._env.mbkEnv[ 'MBK_OUT_PH' ]
    @property             
    def IN_LO             ( self ): return self._env.mbkEnv[ 'MBK_IN_LO' ]
    @property             
    def IN_PH             ( self ): return self._env.mbkEnv[ 'MBK_IN_PH' ]
    @property             
    def SEPAR             ( self ): return self._env.mbkEnv[ 'MBK_SEPAR' ]
    @property             
    def VDD               ( self ): return self._env.mbkEnv[ 'MBK_VDD' ]
    @property             
    def VSS               ( self ): return self._env.mbkEnv[ 'MBK_VSS' ]
    @property
    def RDS_TECHNO_NAME   ( self ): return self._env.mbkEnv[ 'RDS_TECHNO_NAME' ]
    @property
    def GRAAL_TECHNO_NAME ( self ): return self._env.mbkEnv[ 'GRAAL_TECHNO_NAME' ]

    @TARGET_LIB.setter
    def TARGET_LIB        ( self, value ): self._env.mbkEnv[ 'MBK_TARGET_LIB' ] = value
    @WORK_LIB.setter             
    def WORK_LIB          ( self, value ): self._env.mbkEnv[ 'MBK_WORK_LIB'] = value
    @CATA_LIB.setter             
    def CATA_LIB          ( self, value ): self._env.mbkEnv[ 'MBK_CATA_LIB'] = value
    @CATAL_NAME.setter             
    def CATAL_NAME        ( self, value ): self._env.mbkEnv[ 'MBK_CATAL_NAME' ] = value
    @OUT_LO.setter             
    def OUT_LO            ( self, value ): self._env.mbkEnv[ 'MBK_OUT_LO' ] = value
    @OUT_PH.setter             
    def OUT_PH            ( self, value ): self._env.mbkEnv[ 'MBK_OUT_PH' ] = value
    @IN_LO.setter             
    def IN_LO             ( self, value ): self._env.mbkEnv[ 'MBK_IN_LO' ] = value
    @IN_PH.setter             
    def IN_PH             ( self, value ): self._env.mbkEnv[ 'MBK_IN_PH' ] = value
    @SEPAR.setter             
    def SEPAR             ( self, value ): self._env.mbkEnv[ 'MBK_SEPAR' ] = value
    @VDD.setter             
    def VDD               ( self, value ): self._env.mbkEnv[ 'MBK_VDD' ] = value
    @VSS.setter             
    def VSS               ( self, value ): self._env.mbkEnv[ 'MBK_VSS' ] = value
    @RDS_TECHNO_NAME.setter
    def RDS_TECHNO_NAME   ( self, value ): self._env.mbkEnv[ 'RDS_TECHNO_NAME' ] = value
    @GRAAL_TECHNO_NAME.setter
    def GRAAL_TECHNO_NAME ( self, value ): self._env.mbkEnv[ 'GRAAL_TECHNO_NAME' ] = value


class ReportLog ( object ):

    def __init__ ( self, baseName ):
      self._reportBase = baseName
      self._reportFile = None
      self._fd         = None
      self._lastLine   = None
      self._chooseName()
      return

    def _chooseName ( self ):
      index   = 0
      timeTag = time.strftime("%Y.%m.%d")
      while True:
          self._reportFile = "./%s-%s-%02d.log" % (self._reportBase,timeTag,index)
          if not os.path.isfile(self._reportFile):
              print "Report log: <%s>" % self._reportFile
              break
          index += 1
      return

    @property
    def baseName ( self ): return self._reportBase
    @baseName.setter
    def baseName ( self, baseName ): self._reportBase = baseName

    def open ( self ):
      if self._fd: return
      self._fd = open( self._reportFile, "a+" )
      return

    def close ( self ):
      if not self._fd: return
      self._fd.close()
      self._fd = None
      return

    def write ( self, line ):
      if not self._fd: return
      self._lastLine = line
      self._fd.write( self._lastLine )
      return


moduleGlobals  = globals()
env            = Environment( Environment.DefaultEnv )

def staticInitialization ():
  global moduleGlobals
  global env

  confFile       = helpers.sysConfDir+'/'+helpers.symbolicTechno+'/alliance.conf'
  symbol         = 'allianceConfig'
  
  if not os.path.isfile(confFile):
    print '[ERROR] Missing mandatory Coriolis2 system file:'
    print '        <%s>' % confFile
    sys.exit( 1 )
  
  try:
    print '     o  Running configuration hook: Alliance.staticInitialization().'
    print '        - Loading \"%s\".' % helpers.truncPath(confFile)
    exec( open(confFile).read() ) #, moduleGlobals )
  except Exception, e:
    print '[ERROR] An exception occured while loading the configuration file:'
    print '        <%s>\n' % (confFile)
    print '        You should check for simple python errors in this file.'
    print '        Error was:'
    print '        %s\n' % e
    sys.exit( 1 )
  
  if moduleGlobals.has_key(symbol):
    env.load( moduleGlobals[symbol], confFile )
    del moduleGlobals[symbol]
  else:
    print '[ERROR] Mandatory symbol <%s> is missing in system configuration file:' % symbol
    print '        <%s>' % confFile
    sys.exit( 1 )

  print
  return


helpers.staticInitialization( quiet=True )
staticInitialization()

report = ReportLog( 'alliance' )


class Node ( EnvironmentWrapper ):

    Unreached  = -1

    __allNodes = []

    @staticmethod
    def getAllNodes (): return Node.__allNodes

    @staticmethod
    def lookup ( symbol ):
      for node in Node.__allNodes:
        if node.ruleName == symbol:
          return node
      return None

    @staticmethod
    def allClean ():
      for node in Node.__allNodes: node.clean()
      return

    @staticmethod
    def findSelfSymbols ():
      for node in Node.__allNodes: node._findSelfSymbol()
      return

    def __init__ ( self ):
      EnvironmentWrapper.__init__( self, env )
      self._targetName    = None
      self._extension     = None
      self._dependencies  = []
      self._childs        = []
      self._depth         = Node.Unreached
      self._depsReacheds  = 0
      self._active        = False
      self._frame         = inspect.stack()[self.frameDepth]
      Node.__allNodes.append( self )
      return

    def _findSelfSymbol ( self ):
      callerGlobals = self._frame[0].f_globals
      for symbol in callerGlobals.keys():
        if self == callerGlobals[symbol]:
          self.setTarget( symbol )
          return

      self.setDefaultTargetName()
     #raise ErrorMessage( 1, 'Node cannot find its symbol in parent frame.' )
      return

    def setTarget ( self, name ):
      baseName, extension = Environment.symbolToFilename( name, Environment.AllExt )
      if baseName:
        self._targetName = baseName
        self._extension  = extension
      else:
        raise ErrorMessage( 1, 'Invalid file format of Node object <%s>' % name )
      return

    @property
    def targetName ( self ): return self._targetName

    def setDefaultTargetName ( self ):
      if self._dependencies == []:
        raise ErrorMessage( 1, 'Node.setDefaultTargetName(): node is neither used nor have dependencies.' )
      self.setTarget( self.getDependency(0)._targetName+'_'+self.toolName.lower() )
      print WarningMessage( 'Node.setDefaultTargetName(): Node is not affected, using: <%s>' % self.targetName )
      return

    def addDependency ( self, dependency ):
      if not isinstance(dependency,Node):
        raise ErrorMessage( 1, 'Node.addDependency(): Not a node %s.' % str(dependency) )
      self._dependencies.append( dependency )
      dependency.addChild( self )
      return

    def getDependency   ( self, i ): return self._dependencies[i]
    def getDependencies ( self    ): return self._dependencies

    def addChild ( self, child ):
      if not child in self._childs: self._childs.append( child )
      return

    def getChilds ( self ): return self._childs
    def getDepth  ( self ): return self._depth

    def resetOrder ( self ):
      self._depth        = Node.Unreached
      self._depsReacheds = 0
      return

    def incDepsReached ( self, depth ):
      if depth > self._depth: self._depth = depth
      if self._depsReacheds < len(self._dependencies):
        self._depsReacheds += 1
      return self._depsReacheds >= len(self._dependencies)

    def allDepsReached ( self ):
      return self._depsReacheds >= len(self._dependencies)

    def checkFile ( self, hardStop=False, errorMessage=[] ):
      if not os.path.isfile(self.fileName):
        error = ErrorMessage( 1, 'File <%s> of node <%s> has not been created.'
                                 % (self.fileName,self._targetName) )
        if errorMessage:
          print
          for line in errorMessage: print line
        if hardStop:
          raise error
        else:
          print error
      return

    def isUptodate ( self ):
      depsMTime = 0
     #print '    Target: %-30s %d' % (self.fileName, self.mtime)
      for dependency in self.getDependencies():
        dependency.checkFile()
        depsMTime = max( depsMTime, dependency.mtime )
       #print '    | Dep: %-31s %d' % (dependency.fileName, dependency.mtime)
      return depsMTime <= self.mtime

    def setActive ( self ):
      self._active = True
      for dependency in self.getDependencies():
        dependency.setActive()

    def isActive ( self ): return self._active

    def setMbkEnv ( self ): return

    def clean ( self ):
      if     not isinstance(self,Source) \
         and not isinstance(self,Probe)  \
         and not isinstance(self,Rule):
        print 'Clean     | %-30s| rm %s' % ( "<%s>"%self.ruleName, self.fileName )

        report.open()
        report.write( 'Clean <%s>: (%s)\n' % (self.ruleName, self.fileName) )
        if os.path.isfile(self.fileName):
          report.write( '  rm %s\n' % self.fileName )
          os.unlink( self.fileName )
        return
        report.close()

    @property
    def name ( self ): return self._targetName

    @property
    def extension ( self ): return self._extension

    @property
    def fileName ( self ): return self._targetName+'.'+self._extension

    @property
    def ruleName ( self ):
      if self._extension:
        return self._targetName+'_'+self._extension
      return self._targetName
      

    @property
    def mtime ( self ):
      if not os.path.isfile(self.fileName): return 0
      return os.stat( self.fileName )[ stat.ST_MTIME ]


class Command ( Node ):

    commandTypes = []

    def __init__ ( self ):
      Node.__init__( self )
      self._flags = 0
      Command.commandTypes.append( type(self) )
      return

    @staticmethod
    def asString ( command ):
      s = ''
      for i in range( len(command) ):
        if i: s += ' '
        s += command[i]
      return s

    @staticmethod
    def indent ( command, indent, width=109 ):
      lines = []
      lines.append( '|' )

      for i in range(len(command)):
        lines[-1] += ' ' + command[i]
        if    (len(lines) == 1 and len(lines[0]) >= width-indent) \
           or len(lines[-1]) >= width:
          if i < len(command)-1:
            lines.append( ' '*indent + '|' + ' '*(len(command[0])+1) )

      s = lines[0]
      for line in lines[1:]:
        s += '\n' + line
      return s

     #s = ''
     #l = '|'
     #for i in range(len(command)):
     #  l += ' ' + command[i]
     #  if len(l) >= width-indent:
     #    if len(s): s += '\n'
     #    s += l
     #    if i < len(command)-1:
     #      l = ' '*indent + '|' + ' '*(len(command[0])+1)
     #    else:
     #      return s
     #if len(s): s += '\n'
     #s += l
     #return s

    @property
    def frameDepth ( self ): return 3
    @property
    def flags ( self ): return self._flags
    @flags.setter
    def flags ( self, flags ): self._flags = flags

    def _run ( self, checkFile=True ):
      global commandFlags

      flags = commandFlags
      if self._flags: flags = self._flags

      command  = self.asCommand()
      ruleName = "<%s>" % self.ruleName

      if self.isActive() and (not self.isUptodate() or flags & ForceRun):
        if flags & ShowCommand:
          print "Executing | %-30s%s" % (ruleName,Command.indent(command,42))
        child = subprocess.Popen( command
                                , env=self.env.toSystemEnv()
                                , stdout=subprocess.PIPE
                                , stderr=subprocess.PIPE
                                )
        
        dots = Dots( ' '*42+'| ', 109 )
        report.open()
        report.write( 'Executing command: %s \n' % ruleName )
        report.write( '  %s\n' % Command.asString(command) )
        
        while True:
          line = child.stdout.readline()
          if not line: break
        
          if flags & ShowLog:
            print "%s" % (line[:-1])
            sys.stdout.flush()
          elif flags & ShowDots:
            dots.dot()
        
          report.write( line )

        errorLines = []
        while True:
          line = child.stderr.readline()
          if not line: break
        
          errorLines.append( line[:-1] )
          sys.stderr.flush()
          report.write( line )

        report.close()
        dots.reset()
        
        (pid,status) = os.waitpid(child.pid, 0)
        status >>= 8
        if status != 0:
          print
          for line in errorLines: print line
          raise ErrorMessage( 1, "%s returned status:%d." % (self.toolName,status) )
        if checkFile:
          self.checkFile( hardStop=True, errorMessage=errorLines )
      else:
        if self.isActive(): action = 'Up to date'
        else:               action = 'Inactive'
        if flags & ShowCommand:
            print "%-10s| %-30s%s" % (action,ruleName,Command.indent(command,42))

        report.open()
        report.write( '%s command:\n' % action )
        report.write( '  %s\n' % Command.asString(command) )
        report.close()
        status = 0

      return status


class StampCommand ( Command ):

    def __init__ ( self ):
      Command.__init__( self )
      return

    def setTarget ( self, name ):
      if not name.startswith('stamp_'):
        raise ErrorMessage( 1, 'Target of stamp commands must start with "stamp_" (<%s>)' % name )
      
      self._targetName    = 'stamp.' + name[6:]
      return

    @property
    def frameDepth ( self ): return 4
    @property
    def fileName ( self ): return self._targetName

    def _run ( self ):
      Command._run( self, checkFile=False )
      if self.isActive():
        with file(self.fileName,'a'): os.utime(self.fileName,None)
      return


class Probe ( Command ):

    def __init__ ( self, script ):
      Command.__init__( self )
      self.addDependency( script )
      return

    @property
    def toolName ( self ): return self.getDependency(0).name+'_'+self.getDependency(0).extension

    def asCommand ( self ):
      return [ os.path.join(os.getcwd(),self.getDependency(0).fileName) ]

    def _run ( self ):
      Command._run( self, checkFile=False )
      return


class Source ( Node ):

    def __init__ ( self ):
      Node.__init__( self )
      return

    @property
    def frameDepth ( self ): return 2


class Rule ( Node ):

    def __init__ ( self, *dependencies ):
      Node.__init__( self )
      for dependency in dependencies:
        self.addDependency( dependency )
      return

    @property
    def frameDepth ( self ): return 2
    @property
    def mtime      ( self ): return 0

    def setTarget ( self, name ):
      self._targetName = name
      return


class Boom ( Command ):

    Verbose           =  0x00000001
    Trace             =  0x00000002
    ReverseOrder      =  0x00000004
    LocalOptimization =  0x00000008

    AlgorithmS        = 's'
    AlgorithmJ        = 'j'
    AlgorithmB        = 'b'
    AlgorithmG        = 'g'
    AlgorithmP        = 'p'
    AlgorithmW        = 'w'
    AlgorithmT        = 't'
    AlgorithmM        = 'm'
    AlgorithmO        = 'o'
    AlgorithmR        = 'r'
    AlgorithmN        = 'n'

    def __init__ ( self, behavioral ):
      Command.__init__( self )
      self._directives   = None
      self._level        = 0
      self._delayPercent = 0
      self._iterations   = None
      self._bddReorder   = None
      self._algorithm    = None

      self.addDependency( behavioral )
      return

    @property
    def toolName ( self ): return 'Boom'

    @property
    def fileName ( self ): return self.name+'.'+self.extension

    @property
    def directives ( self ): return self._directives
    @directives.setter
    def directives ( self, directives ):
      self._directives = directives
      self.addDependency( self._directives )

    @property
    def level       ( self ): return self._level
    @property
    def delayPercent( self ): return self._delayPercent
    @property
    def iterations  ( self ): return self._iterations
    @property
    def bddReorder  ( self ): return self._bddReorder
    @property
    def algorithm   ( self ): return self._algorithm

    @level.setter
    def level       ( self, level        ): self._level = level
    @delayPercent.setter
    def delayPercent( self, delayPercent ): self._delayPercent = delayPercent
    @iterations.setter
    def iterations  ( self, iterations   ): self._iterations = iterations
    @bddReorder.setter
    def bddReorder  ( self, bddReorder   ): self._bddReorder = bddReorder
    @algorithm.setter
    def algorithm   ( self, algorithm    ): self._algorithm = algorithm

    def setMbkEnv ( self ):
      if self.extension != 'vbe':
        raise ErrorMessage( 1, 'Boom(): Optimized file format/ext must be <vbe>, not <%s>.'
                               % self.extension )
      if self.getDependency(0).extension != 'vbe':
        raise ErrorMessage( 1, 'Boom(): Behavioral source file format/ext must be <vbe>, not <%s>.'
                               % self.getDependency(0).extension )
      if self._directives and self._directives.extension != 'boom':
        raise ErrorMessage( 1, 'Boom(): Directive file format/ext must be <boom>, not <%s>.'
                               % self._directives.extension )
      self.IN_LO  = 'vst'
      self.OUT_LO = 'vst'
      return

    def asCommand ( self ):
      command = [ 'boom' ]
     #command = [ './boom.sh' ]
      if self._flags & Boom.Verbose:           command += [ '-V' ]
      if self._flags & Boom.Trace:             command += [ '-T' ]
      if self._flags & Boom.ReverseOrder:      command += [ '-O' ]
      if self._flags & Boom.LocalOptimization: command += [ '-A' ]

      if self._directives:   command += [ '-P', self.getDependency(1).name ]
      if self._level:        command += [ '-l', str(self._level)        ]
      if self._delayPercent: command += [ '-d', str(self._delayPercent) ]
      if self._iterations:   command += [ '-i', str(self._iterations)   ]
      if self._bddReorder:   command += [ '-a', str(self._bddReorder)   ]
      if self._algorithm:    command += [   '-'+self._algorithm         ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Boog ( Command ):

    def __init__ ( self, behavioral ):
      Command.__init__( self )
      self._optimMode    = 5
      self._xschMode     = 2
      self._delayPercent = 0
      self._laxFile      = None
      self._debugFile    = None

      self.addDependency( behavioral )
      return

    @property
    def toolName ( self ): return 'Boog'

    @property
    def fileName ( self ): return self.name+'.'+self.extension

    @property
    def laxFile ( self ): return self._laxNode
    @laxFile.setter
    def laxFile ( self, laxNode ):
      self._laxNode = laxNode
      self.addDependency( self._laxNode )

    @property
    def optimMode( self ): return self._optimMode
    @property
    def xschMode ( self ): return self._xschMode
    @property
    def debugFile( self ): return self._debugFile

    @optimMode.setter
    def optimMode( self, optimMode ): self._optimMode = optimMode
    @xschMode.setter
    def xschMode ( self, xschMode  ): self._xschMode  = xschMode
    @debugFile.setter
    def debugFile( self, debugFile ): self._debugFile = debugFile

    def setMbkEnv ( self ):
      if Environment.isExt(self.extension,Environment.StructuralExt):
        self.OUT_LO = self.extension
      else:
        raise ErrorMessage( 1, 'Boog(): Invalid structural (netlist) file format/ext <%s>.'
                               % self.extension )
      if self.getDependency(0).extension != 'vbe':
        raise ErrorMessage( 1, 'Boog(): Behavioral source file format/ext must be <vbe>, not <%s>.'
                               % self.getDependency(0).extension )
      if self._laxFile and self._laxFile.extension != 'lax':
        raise ErrorMessage( 1, 'Boog(): Lax file format/ext must be <lax>, not <%s>.'
                               % self._directives.extension )
      self.IN_LO  = 'vst'
      return

    def asCommand ( self ):
      command = [ 'boog' ]
      if self._optimMode < 5: command += [ '-m', str(self._optimMode) ]
      if self._xschMode  < 2: command += [ '-x', str(self._xschMode)  ]
      if self._laxFile:       command += [ '-l', self.getDependency(1).name ]
      if self._debugFile:     command += [ '-d', self._debugFile ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Loon ( Command ):

    def __init__ ( self, structural ):
      Command.__init__( self )
      self._optimMode    = 5
      self._xschMode     = 2
      self._laxFile      = None

      self.addDependency( structural )
      return

    @property
    def toolName ( self ): return 'Loon'

    @property
    def fileName ( self ): return self.name+'.'+self.OUT_LO

    @property
    def laxFile ( self ): return self._laxFile
    @laxFile.setter
    def laxFile ( self, laxFile ):
      self._laxFile = laxFile
      self.addDependency( self._laxFile )

    @property
    def optimMode( self ): return self._optimMode
    @property
    def xschMode ( self ): return self._xschMode

    @optimMode.setter
    def optimMode( self, optimMode ): self._optimMode = optimMode
    @xschMode.setter
    def xschMode ( self, xschMode  ): self._xschMode  = xschMode

    def setMbkEnv ( self ):
      if Environment.isExt(self.extension,Environment.StructuralExt):
        self.OUT_LO = self.extension
      else:
        raise ErrorMessage( 1, 'Loon(): Invalid output structural (netlist) file format/ext <%s>.'
                               % self.extension )
      if Environment.isExt(self.getDependency(0).extension,Environment.StructuralExt):
        self.IN_LO  = self.getDependency(0).extension
      else:
        raise ErrorMessage( 1, 'Loon(): Invalid input structural (netlist) file format/ext <%s>.'
                               % self.getDependency(0).extension )
      if self._laxFile and self._laxFile.extension != 'lax':
        raise ErrorMessage( 1, 'Loon(): Lax file format/ext must be <lax>, not <%s>.'
                               % self._directives.extension )
      return

    def asCommand ( self ):
      command = [ 'loon' ]
      if self._optimMode < 5: command += [ '-m', str(self._optimMode) ]
      if self._xschMode  < 2: command += [ '-x', str(self._xschMode)  ]
      if self._laxFile:       command += [ '-l', self.getDependency(1).fileName ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Genlib ( Command ):

    def __init__ ( self, csource ):
      Command.__init__( self )
      self.addDependency( csource )
      return

    @property
    def toolName ( self ): return 'Genlib'

    @property
    def fileName ( self ): return self.name+'.'+self.OUT_LO

    def setMbkEnv ( self ):
      if not Environment.isExt(self.extension,Environment.StructuralExt|Environment.PhysicalExt):
        raise ErrorMessage( 1, 'Genlib(): Invalid output netlist or layout file format/ext <%s>.'
                               % self.extension )
      if not Environment.isExt(self.getDependency(0).extension,Environment.CSourceExt):
        raise ErrorMessage( 1, 'Genlib(): Input file format/ext must be <c>, not <%s>.'
                               % self.getDependency(0).extension )

      if Environment.isExt(self.extension,Environment.StructuralExt): self.OUT_LO = self.extension
      if Environment.isExt(self.extension,Environment.PhysicalExt):   self.OUT_PH = self.extension
      return

    def asCommand ( self ):
      command = [ 'genlib', '-v', '-k' ]
      command += [ self.getDependency(0).name ]
      return command


class Genpat ( Command ):

    def __init__ ( self, csource ):
      Command.__init__( self )
      self.addDependency( csource )
      return

    @property
    def toolName ( self ): return 'Genpat'

    @property
    def fileName ( self ): return self.name+'.pat'

    def setMbkEnv ( self ):
      if self.extension != 'pat':
        raise ErrorMessage( 1, 'Genpat(): Invalid output pattern file format/ext <%s>, must be <pat>.'
                               % self.extension )
      if not Environment.isExt(self.getDependency(0).extension,Environment.CSourceExt):
        raise ErrorMessage( 1, 'Genpat(): Input file format/ext must be <c>, not <%s>.'
                               % self.getDependency(0).extension )
      return

    def asCommand ( self ):
      command = [ 'genpat', '-v' ]
      command += [ self.getDependency(0).name ]
      return command


class Asimut ( Command ):

    RootIsBehavioral = 0x00000001
    UseBdd           = 0x00000002
    CompileOnly      = 0x00000004
    ZeroDelay        = 0x00000008

    def __init__ ( self, model, patterns ):
      Command.__init__( self )
      self._backdelay = None

      self.addDependency( patterns )
      self.addDependency( model )
      return

    @property
    def toolName ( self ): return 'Asimut'

    @property
    def fileName ( self ): return self.name+'.pat'

    @property
    def backdelay ( self ): return self._backdelay
    @backdelay.setter
    def backdelay ( self, backdelay ):
      self._backdelay = backdelay
      self.addDependency( self._backdelay )

    def setMbkEnv ( self ):
      if self.flags & Asimut.RootIsBehavioral:
        if not Environment.isExt(self.getDependency(1).extension,Environment.BehavioralExt):
          raise ErrorMessage( 1, 'Asimut(): Invalid input behavioral file format/ext <%s>, must be <vbe>.'
                                 % self.getDependency(1).extension )
      else:
        if Environment.isExt(self.getDependency(1).extension,Environment.StructuralExt):
          self.IN_LO = self.getDependency(1).extension
        else:
          raise ErrorMessage( 1, 'Asimut(): Invalid input structural (netlist) file format/ext <%s>.'
                                 % self.getDependency(1).extension )

      if self.extension != 'pat':
        raise ErrorMessage( 1, 'Asimut(): Invalid output pattern file format/ext <%s>, must be <pat>.'
                               % self.extension )
      if self.getDependency(0).extension != 'pat':
        raise ErrorMessage( 1, 'Asimut(): Invalid input pattern file format/ext <%s>, must be <pat>.'
                               % self.getDependency(0).extension )
      return

    def asCommand ( self ):
      command = [ 'asimut' ]
      if self.flags & Asimut.RootIsBehavioral: command += [ '-b' ]
      if self.flags & Asimut.UseBdd:           command += [ '-bdd' ]
      if self.flags & Asimut.CompileOnly:      command += [ '-c' ]
      if self.flags & Asimut.ZeroDelay:        command += [ '-zd' ]
      if self._backdelay: command += [ '-backdelay', self.getDependency(1).name ]

      command += [ self.getDependency(1).name ]
      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Ocp ( Command ):

    Ring    = 0x00000001
    Gnuplot = 0x00000002

    def __init__ ( self, netlist  ):
      Command.__init__( self )
      self._partial = None
      self._ioc     = None
      self._margin  = -1

      self.addDependency( netlist )
      return

    @property
    def toolName ( self ): return 'Ocp'

    @property
    def fileName ( self ): return self.name+'.'+self._extension

    @property
    def partial ( self ): return self._partial
    @partial.setter
    def partial ( self, partial ):
      self._partial = partial
      self.addDependency( self._partial )

    @property
    def ioc ( self ): return self._ioc
    @ioc.setter
    def ioc ( self, ioc ):
      self._ioc = ioc
      self.addDependency( self._ioc )

    @property
    def margin ( self ): return self._margin
    @margin.setter
    def margin ( self, margin ):
      if margin < 0 or margin > 100:
        raise ErrorMessage( 1, 'Ocp: Margin must be between 0 and 100% (%d)' % margin )
      self._margin = margin

    def setMbkEnv ( self ):
      if Environment.isExt(self.extension,Environment.PhysicalExt):
        self.OUT_PH = self.extension
      else:
        raise ErrorMessage( 1, 'Ocp(): Invalid output layout file format/ext <%s>.'
                               % self.extension )

      if Environment.isExt(self.getDependency(0).extension,Environment.StructuralExt):
        self.IN_LO = self.getDependency(0).extension
      else:
        raise ErrorMessage( 1, 'Ocp(): Input file format/ext must be structural, not <%s>.'
                               % self.getDependency(0).extension )

      if Environment.isExt(self._partial.extension,Environment.PhysicalExt):
        self.IN_PH = self._partial.extension
      else:
        raise ErrorMessage( 1, 'Ocp(): Invalid input layout file format/ext <%s> for partial placement.'
                               % self._partial.extension )
      if self._ioc.extension != 'ioc':
        raise ErrorMessage( 1, 'Ocp(): Invalid IOC file format/ext <%s>, must be <ioc>.'
                               % self._ioc.extension )

      return

    def asCommand ( self ):
      command = [ 'ocp', '-v' ]
      if self.flags & Ocp.Ring:    command += [ '-ring' ]
      if self.flags & Ocp.Gnuplot: command += [ '-gnuplot' ]
      if self._margin > 0: command += [ '-margin' , str(self._margin) ]
      if self._partial:    command += [ '-partial', self._partial.name ]
      if self._ioc:        command += [ '-ioc'    , self._ioc.name ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Nero ( Command ):

    ForceGlobalOff = 0x00000001
    ForceGlobalOn  = 0x00000002
    Use2Layers     = 0x00000004
    Use3Layers     = 0x00000008
    Use4Layers     = 0x00000010
    Use5Layers     = 0x00000020
    Use6Layers     = 0x00000040

    def __init__ ( self, netlist ):
      Command.__init__( self )
      self._placement = None
      self.addDependency( netlist )
      return

    @property
    def toolName ( self ): return 'Nero'

    @property
    def fileName ( self ): return self.name+'.'+self._extension

    @property
    def placement ( self ): return self._placement
    @placement.setter
    def placement ( self, placement ):
      self._placement = placement
      self.addDependency( self._placement )

    def setMbkEnv ( self ):
      if Environment.isExt(self.extension,Environment.PhysicalExt):
        self.OUT_PH = self.extension
      else:
        raise ErrorMessage( 1, 'Nero(): Invalid output layout file format/ext <%s>.'
                               % self.extension )

      if Environment.isExt(self.getDependency(0).extension,Environment.StructuralExt):
        self.IN_LO = self.getDependency(0).extension
      else:
        raise ErrorMessage( 1, 'Nero(): Input file format/ext must be structural, not <%s>.'
                               % self.getDependency(0).extension )

      if Environment.isExt(self._placement.extension,Environment.PhysicalExt):
        self.IN_PH = self._placement.extension
      else:
        raise ErrorMessage( 1, 'Nero(): Invalid input layout file format/ext <%s> for placement placement.'
                               % self._placement.extension )

      return

    def asCommand ( self ):
      command = [ 'nero', '-V' ]
      if self.flags & Nero.ForceGlobalOff: command += [ '-L' ]
      if self.flags & Nero.ForceGlobalOn:  command += [ '-G' ]
      if self.flags & Nero.Use2Layers:     command += [ '-2' ]
      if self.flags & Nero.Use3Layers:     command += [ '-3' ]
      if self.flags & Nero.Use4Layers:     command += [ '-4' ]
      if self.flags & Nero.Use5Layers:     command += [ '-5' ]
      if self.flags & Nero.Use6Layers:     command += [ '-6' ]
      if self._placement: command += [ '-p', self._placement.name ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Cougar ( Command ):

    TransistorFlatten = 0x00000001
    CatalogFlatten    = 0x00000002
    CapaToGround      = 0x00000004
    RCToGround        = 0x00000008

    def __init__ ( self, layout ):
      Command.__init__( self )
      self.addDependency( layout )
      return

    @property
    def toolName ( self ): return 'Cougar'

    @property
    def fileName ( self ): return self.name+'.'+self._extension

    def setMbkEnv ( self ):
      if Environment.isExt(self.extension,Environment.StructuralExt):
        self.OUT_LO = self.extension
      else:
        raise ErrorMessage( 1, 'Cougar(): Invalid output netlist file format/ext <%s>.'
                               % self.extension )

      if Environment.isExt(self.getDependency(0).extension,Environment.PhysicalExt):
        self.IN_PH = self.getDependency(0).extension
      else:
        raise ErrorMessage( 1, 'Cougar(): Input file format/ext must be physical, not <%s>.'
                               % self.getDependency(0).extension )
      return

    def asCommand ( self ):
      command = [ 'cougar', '-v' ]
      if self.flags & Cougar.TransistorFlatten: command += [ '-t' ]
      if self.flags & Cougar.CatalogFlatten:    command += [ '-f' ]
      if self.flags & Cougar.CapaToGround:      command += [ '-ac' ]
      if self.flags & Cougar.RCToGround:        command += [ '-ar' ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Lvx ( StampCommand ):

    MergeSupplies    = 0x00000001
    OrderConnectors  = 0x00000002
    CheckUnassigneds = 0x00000004
    CatalogFlatten   = 0x00000008

    def __init__ ( self, netlist1, netlist2 ):
      StampCommand.__init__( self )
      self.addDependency( netlist1 )
      self.addDependency( netlist2 )
      return

    @property
    def toolName ( self ): return 'Lvx'

    def setMbkEnv ( self ):
      if not Environment.isExt(self.getDependency(0).extension,Environment.StructuralExt):
        raise ErrorMessage( 1, 'Lvx(): Input file 1 format/ext must be structural (netlist), not <%s>.'
                               % self.getDependency(0).extension )
      if not Environment.isExt(self.getDependency(1).extension,Environment.StructuralExt):
        raise ErrorMessage( 1, 'Lvx(): Input file 2 format/ext must be structural (netlist), not <%s>.'
                               % self.getDependency(1).extension )
      return

    def asCommand ( self ):
      command  = [ 'lvx' ]
      command += [ self.getDependency(0).extension ]
      command += [ self.getDependency(1).extension ]
      command += [ self.getDependency(0).name ]
      command += [ self.getDependency(1).name ]

      if self.flags & Lvx.MergeSupplies:    command += [ '-a' ]
      if self.flags & Lvx.CheckUnassigneds: command += [ '-u' ]
      if self.flags & Lvx.CatalogFlatten:   command += [ '-f' ]
      return command


class Druc ( StampCommand ):

    def __init__ ( self, layout ):
      StampCommand.__init__( self )
      self.addDependency( layout )
      return

    @property
    def toolName ( self ): return 'Druc'

    def setMbkEnv ( self ):
      if not Environment.isExt(self.getDependency(0).extension,Environment.PhysicalExt):
        raise ErrorMessage( 1, 'Druc(): Input file format/ext must be physical (layout), not <%s>.'
                               % self.getDependency(0).extension )
      return

    def asCommand ( self ):
      command  = [ 'druc', '-v' ]
      command += [ self.getDependency(0).name ]
      return command


class Tools ( object ):

   def __init__ ( self ):
     self._sourceNodes = []
     self._toolNodes   = []
     self._ruleNodes   = []
     self._staticOrder = []
     return

   def _findNodes ( self ):
     Node.findSelfSymbols()

     for node in Node.getAllNodes():
       isCommand = False
       for commandType in Command.commandTypes:
         if isinstance(node,commandType):
           self._toolNodes.append( node )
           isCommand = True
           break

       if not isCommand:
         if isinstance(node,Source): self._sourceNodes.append( node )
         if isinstance(node,Rule  ): self._ruleNodes.append( node )
         
 
    # Three level of stack from the user's script python module.
    #callerGlobals = inspect.stack()[2][0].f_globals
    #for symbol in callerGlobals.keys():
    #  isCommand = False
    #  node      = callerGlobals[symbol]
    #  for commandType in Command.commandTypes:
    #    if isinstance(node,commandType):
    #      node.setTarget( symbol )
    #      self._toolNodes.append( node )
    #      isCommand = True
    #      break
    #
    #  if not isCommand \
    #     and (   isinstance(node,Source)
    #          or isinstance(node,Rule  )):
    #    node.setTarget( symbol )
    #    self._sourceNodes.append( node )
    #
    #for node in Node.getAllNodes():
    #  if node.targetName: continue
    #  node.setDefaultTargetName()
    #  if not isinstance(node,Source):
    #    self._toolNodes.append( node )
     return

   def _sortNodes ( self ):
     self._staticOrder = []

     nodeStack = []
     for node in self._sourceNodes:
       node._depth = 0
       nodeStack.append( node )

     while nodeStack:
       reacheds = 0
       for inode in range(len(nodeStack)):
         if nodeStack[inode].allDepsReached():
           reacheds += 1
           node      = nodeStack.pop( inode )
           self._staticOrder.append( node )
           for child in node.getChilds():
             if child.getDepth() < 0: nodeStack.append(child)
             child.incDepsReached( node.getDepth()+1 )
           break
       if not reacheds and nodeStack:
         raise ErrorMessage( 1, 'Loop in dependency graph.' )
     return  

   def _setMbkEnv ( self ):
     for node in self._staticOrder:
       node.setMbkEnv()
     return

   @staticmethod
   def run ( args=[] ):
     tool = Tools()
     tool._findNodes()
     tool._sortNodes()
     tool._setMbkEnv()

     doClean = False
     for arg in args:
       if arg == 'clean': doClean = True
       else:
         node = Node.lookup( arg )
         if node: node.setActive()

     if commandFlags & ShowCommand and not (commandFlags & ShowLog):
       print "==========+===============================+============================================"
       print "  ACTION  |          TARGET/RULE          |  COMMAND"
       print "==========+===============================+============================================"

     if doClean:
       Node.allClean()
       if commandFlags & ShowCommand and not (commandFlags & ShowLog):
         print "----------+-------------------------------+--------------------------------------------"

     for node in tool._staticOrder:
      #for dep in node.getDependencies():
      #  print dep.name
      #  print dep.fileName
       if not isinstance(node,Source) and not isinstance(node,Rule):
         if node._run(): break

     if commandFlags & ShowCommand and not (commandFlags & ShowLog):
       print "----------+-------------------------------+--------------------------------------------"
       
     return

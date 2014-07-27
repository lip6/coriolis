# -*- Mode:Python; explicit-buffer-name: "Alliance.py<cumulus>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2014, All Rights Reserved
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
from   helpers   import ErrorMessage
from   helpers   import WarningMessage


# Global display flags
ShowCommand  = 0x00001000
ShowLog      = 0x00002000
ForceRun     = 0x00004000

commandFlags = ShowCommand | ShowLog


class RoutingLayerGauge ( object ):

    Horizontal = 1
    Vertical   = 2
    PinOnly    = 4
    Default    = 8


class Environment ( object ):

    DefaultEnv = 0001
    CommandEnv = 0002
    Append     = 0004

    @staticmethod
    def splitExtension ( name ):
      if name.endswith('_vbe') or name.endswith('_vst'):
        return name[:-4], name[-3:]
      return None, None
       

    def __init__ ( self, flags ):
      self.mbkEnv = {}
      if flags & Environment.DefaultEnv:
        self.mbkEnv[ 'MBK_TARGET_LIB'    ] = None
        self.mbkEnv[ 'MBK_WORK_LIB'      ] = '.'
        self.mbkEnv[ 'MBK_CATA_LIB'      ] = None
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
        self.mbkEnv[ 'MBK_TARGET_LIB'    ] = None
        self.mbkEnv[ 'MBK_WORK_LIB'      ] = None
        self.mbkEnv[ 'MBK_CATA_LIB'      ] = None
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
              if self.mbkEnv['MBK_CATA_LIB']:
                self.mbkEnv['MBK_CATA_LIB'] = self.mbkEnv['MBK_CATA_LIB'] + ':' + libPath
              else:
                self.mbkEnv['MBK_CATA_LIB'] = libPath
    
        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<Alliance> at index %d.' % (allianceFile,entryNo))
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
    print '          - Loading \"%s\".' % helpers.truncPath(confFile)
    execfile( confFile, moduleGlobals )
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


helpers.staticInitialization()
staticInitialization()
report = ReportLog( 'alliance' )


class Node ( EnvironmentWrapper ):

    Unreached  = -1

    __allNodes = []

    @staticmethod
    def getAllNodes (): return Node.__allNodes

    def __init__ ( self ):
      EnvironmentWrapper.__init__( self, env )
      self._targetName   = None
      self._dependencies = []
      self._childs       = []
      self._depth        = Node.Unreached
      self._depsReacheds = 0
      Node.__allNodes.append( self )
      return

    @property
    def targetName ( self ): return self._targetName

    def setTarget ( self, name ): self._targetName = name

    def setDefaultTargetName ( self ):
      if self._dependencies == None:
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

    def checkFile ( self, hardStop=False ):
      if not os.path.isfile(self.fileName):
        error = ErrorMessage( 1, 'File <%s> of node <%s> has not been created.'
                                 % (self.fileName,self._targetName) )
        if hardStop:
          raise error
        else:
          print error
      return

    def isUptodate ( self ):
      depsMTime = 0
      for dependency in self.getDependencies():
        dependency.checkFile()
        depsMTime = max( depsMTime, dependency.mtime )
      return depsMTime < self.mtime

    @property
    def name ( self ): return self._targetName

    @property
    def fileName ( self ):
      raise ErrorMessage( 1, 'Node.fileName base class attribute must never be called.' )
      return

    @property
    def mtime ( self ):
      if not os.path.isfile(self.fileName): return 0
      return os.stat( self.fileName )[ stat.ST_MTIME ]


class Command ( Node ):

    def __init__ ( self ):
      Node.__init__( self )
      self._flags = 0
      return

    @staticmethod
    def asString ( command ):
      s = ''
      for i in range( len(command) ):
        if i: s += ' '
        s += command[i]
      return s

    @property
    def flags ( self ): return self._flags
    @flags.setter
    def flags ( self, flags ): self._flags = flags

    def _run ( self ):
      global commandFlags

      flags = commandFlags
      if self._flags: flags = self._flags

      command = self.asCommand()

      if not self.isUptodate() or flags & ForceRun:
        if flags & ShowCommand:
          print "Executing: %s" % Command.asString(command)
        child = subprocess.Popen( command
                                , env=self.env.toSystemEnv()
                                , stdout=subprocess.PIPE
                                , stderr=subprocess.PIPE
                                )
        
        report.open()
        report.write( 'Executing command:\n' )
        report.write( '  %s\n' % Command.asString(command) )
        
        while True:
          line = child.stdout.readline()
          if not line: break
        
          if flags & ShowLog:
            print "%s" % (line[:-1])
            sys.stdout.flush()
        
          report.write( line )
        report.close()
        
        (pid,status) = os.waitpid(child.pid, 0)
        status >>= 8
        if status != 0:
          print ErrorMessage( 1, "%s returned status:%d." % (self.toolName,status) )
      else:
        if flags & ShowCommand:
          print "Up to date: %s" % Command.asString(command)
        report.open()
        report.write( 'Up to date command:\n' )
        report.write( '  %s\n' % Command.asString(command) )
        report.close()
        status = 0

      return status


class Probe ( Command ):

    def __init__ ( self, script ):
      Command.__init__( self )
      self._script = script
      return

    @property
    def toolName ( self ): return self._script

    def asCommand ( self ):
      return [ os.path.join(os.getcwd(),self._script) ]


class Source ( Node ):

    def __init__ ( self ):
      Node.__init__( self )
      self._extension = None
      return

    @property
    def fileName ( self ): return self.name+'.'+self._extension

    def setTarget ( self, name ):
      baseName, extension = Environment.splitExtension( name )
      if baseName:
        Node.setTarget( self, baseName )
        self._extension = extension
      else:
        raise ErrorMessage( 1, 'Invalid file format of Source object <%s>' % name )
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
    def fileName ( self ): return self.name+'.vbe'

    @property
    def directives ( self ): return self._directives
    @directives.setter
    def directives ( self, directives ):
      self._directives = directives
      dependency = Source( 'boom' )
      dependency.setTarget( self._directives )
      self.addDependency( dependency )

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

    def asCommand ( self ):
      command = [ 'boom' ]
      if self._flags & Boom.Verbose:           command += [ '-V' ]
      if self._flags & Boom.Trace:             command += [ '-T' ]
      if self._flags & Boom.ReverseOrder:      command += [ '-O' ]
      if self._flags & Boom.LocalOptimization: command += [ '-A' ]

      if self._directives:   command += [ '-P', self.getDependency(1).fileName ]
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
    def fileName ( self ): return self.name+'.vst'

    @property
    def laxFile ( self ): return self._laxFile
    @laxFile.setter
    def laxFile ( self, laxFile ):
      self._laxFile = laxFile
      dependency = Source( 'boog' )
      dependency.setTarget( self._laxFile )
      self.addDependency( dependency )

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

    def asCommand ( self ):
      command = [ 'boog' ]
      if self._optimMode < 5: command += [ '-m', str(self._optimMode) ]
      if self._xschMode  < 2: command += [ '-x', str(self._xschMode)  ]
      if self._laxFile:       command += [ '-l', self.getDependency(1).fileName ]
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
    def fileName ( self ): return self.name+'.vst'

    @property
    def laxFile ( self ): return self._laxFile
    @laxFile.setter
    def laxFile ( self, laxFile ):
      self._laxFile = laxFile
      dependency = Source( 'lax' )
      dependency.setTarget( self._laxFile )
      self.addDependency( dependency )

    @property
    def optimMode( self ): return self._optimMode
    @property
    def xschMode ( self ): return self._xschMode

    @optimMode.setter
    def optimMode( self, optimMode ): self._optimMode = optimMode
    @xschMode.setter
    def xschMode ( self, xschMode  ): self._xschMode  = xschMode

    def asCommand ( self ):
      command = [ 'loon' ]
      if self._optimMode < 5: command += [ '-m', str(self._optimMode) ]
      if self._xschMode  < 2: command += [ '-x', str(self._xschMode)  ]
      if self._laxFile:       command += [ '-l', self.getDependency(1).fileName ]

      command += [ self.getDependency(0).name ]
      command += [ self.name ]
      return command


class Tools ( object ):

   def __init__ ( self ):
     self._sourceNodes = []
     self._toolNodes   = []
     self._staticOrder = []
     return

   def _findNodes ( self ):
    # Three level of stack from the user's script python module.
     callerGlobals = inspect.stack()[2][0].f_globals
     for symbol in callerGlobals.keys():
       node = callerGlobals[symbol]
       if    isinstance(node,Boom) \
          or isinstance(node,Boog) \
          or isinstance(node,Loon) \
          or isinstance(node,Probe):
         node.setTarget( symbol )
         self._toolNodes.append( node )
       elif isinstance(node,Source):
         node.setTarget( symbol )
         self._sourceNodes.append( node )

     for node in Node.getAllNodes():
       if node.targetName: continue
       node.setDefaultTargetName()
       if not isinstance(node,Source):
         self._toolNodes.append( node )
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
       if not reacheds and nodeStack:
         raise ErrorMessage( 1, 'Loop in dependency graph.' )
     return  

   @staticmethod
   def run ():
     tool  = Tools()
     tool._findNodes()
     tool._sortNodes()

     for node in tool._staticOrder:
      #for dep in node.getDependencies():
      #  print dep.name
      #  print dep.fileName
       if not isinstance(node,Source): node._run()
     return

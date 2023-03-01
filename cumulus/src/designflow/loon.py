
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class UnsupportedVHdlSuffix ( Exception ): pass


class Loon ( FlowTask ):

    FlagMask          = 0x11000000
    XschModeCritical  = 0x01000000
    XschModeAll       = 0x02000000
    OptimArea         = 0x04000000
    OptimAreaMostly   = 0x08000000
    OptimBalanced     = 0x10000000
    OptimDelaysMostly = 0x20000000
    OptimDelays       = 0x40000000

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Loon( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 'loon' ]
        #print( 'flags=0x{:08x}'.format( flags ))
        if flags & Loon.XschModeCritical:  self.command += [ '-x', '0' ]
        if flags & Loon.XschModeAll:       self.command += [ '-x', '1' ]
        if flags & Loon.OptimArea:         self.command += [ '-m', '0' ]
        if flags & Loon.OptimAreaMostly:   self.command += [ '-m', '1' ]
        if flags & Loon.OptimBalanced:     self.command += [ '-m', '2' ]
        if flags & Loon.OptimDelaysMostly: self.command += [ '-m', '3' ]
        if flags & Loon.OptimDelays:       self.command += [ '-m', '4' ]
        self.command   += [ self.inputFile.stem, self.outputFile.stem ]
        self.targets.append( self.outputFile.with_suffix('.xsc') )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_OUT_LO' ] = self.outputFile.suffix[1:]
        shellEnv[ 'MBK_IN_LO'  ] = self.inputFile .suffix[1:]
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Loon.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Loon.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

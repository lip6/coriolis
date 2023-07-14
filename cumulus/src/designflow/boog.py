
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class UnsupportedVHdlSuffix ( Exception ): pass


class Boog ( FlowTask ):

    FlagMask          = 0x00110000
    XschModeCritical  = 0x00010000
    XschModeAll       = 0x00020000
    OptimArea         = 0x00040000
    OptimAreaMostly   = 0x00080000
    OptimBalanced     = 0x00100000
    OptimDelaysMostly = 0x00200000
    OptimDelays       = 0x00400000

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Boog( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 'boog' ]
        if flags & Boog.XschModeCritical:  self.command += [ '-x', '0' ]
        if flags & Boog.XschModeAll:       self.command += [ '-x', '1' ]
        if flags & Boog.OptimArea:         self.command += [ '-m', '0' ]
        if flags & Boog.OptimAreaMostly:   self.command += [ '-m', '1' ]
        if flags & Boog.OptimBalanced:     self.command += [ '-m', '2' ]
        if flags & Boog.OptimDelaysMostly: self.command += [ '-m', '3' ]
        if flags & Boog.OptimDelays:       self.command += [ '-m', '4' ]
        self.command += [ self.inputFile.stem, self.outputFile.stem ]
        self.targets.append( self.outputFile.with_suffix('.xsc') )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_OUT_LO' ] = self.outputFile.suffix[1:]
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Boog.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Boog.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

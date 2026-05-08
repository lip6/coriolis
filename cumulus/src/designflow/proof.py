
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class Proof ( FlowTask ):

    KeepAuxiliaries = 0x0001
    DisplayErrors   = 0x0002

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return Proof( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        targets = [ Path( depends[0] ).with_suffix( '.proof-OK' ) ]
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile1 = self.file_depend(0)
        self.inputFile2 = self.file_depend(1)
        self.command    = [ 'proof' ]
        if flags & Proof.KeepAuxiliaries: self.command += [ '-a' ]
        if flags & Proof.DisplayErrors:   self.command += [ '-d' ]
        self.command   += [ self.inputFile1.stem, self.inputFile2.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_IN_LO' ] = 'vst'
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Proof.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        self.file_target(0).touch()
        return self.checkTargets( 'Proof.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }


import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class x2y ( FlowTask ):

    MBK_CATA_LIB = None

    @staticmethod
    def mkRule ( rule, targets, depends ):
        return x2y( rule, targets, depends )

    def __init__ ( self, rule, targets, depends ):
        from .. import CRL

        super().__init__( rule, targets, depends )
        self.originalFile   = self.file_depend(0)
        self.translatedFile = self.file_target(0)
        self.command        = [ 'x2y'
                              , self.originalFile.suffix[1:]
                              , self.translatedFile.suffix[1:]
                              , self.originalFile.stem
                              , self.translatedFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv( 'x2y.doTask()' )
        if x2y.MBK_CATA_LIB is not None:
            shellEnv[ 'MBK_CATA_LIB' ] = x2y.MBK_CATA_LIB + ':' + shellEnv[ 'MBK_CATA_LIB' ]
        shellEnv.export( ShellEnv.Show )
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'x2y.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'x2y.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        


import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Druc ( FlowTask ):

    Verbose = 0x0001

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return Druc( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        super().__init__( rule, [], depends )
        self.flags         = flags
        self.referenceFile = self.file_depend(0)
        self.targets       = [ self.referenceFile.with_suffix('.drc')
                             , Path(self.referenceFile.stem + '_drc.gds')
                             , Path(self.referenceFile.stem + '_rng.gds') ]
        self.command       = [ 'druc', self.referenceFile.stem ]
        if flags & Druc.Verbose: self.command.append( '-v' )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Druc.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Druc.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               }
        

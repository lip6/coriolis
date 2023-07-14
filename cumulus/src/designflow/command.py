
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Command ( FlowTask ):

    @staticmethod
    def mkRule ( rule, targets=[], depends=[], command=None ):
        return Command( rule, targets, depends, command )

    def __init__ ( self, rule, targets, depends, command ):
        super().__init__( rule, targets, depends )
        self.command  = command
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage

        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Command.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Command.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               }

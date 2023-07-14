
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Klayout ( FlowTask ):

    Verbose = 0x0001

    @staticmethod
    def mkRule ( rule, targets=[], depends=[], script=None, variables={}, flags=0 ):
        return Klayout( rule, targets, depends, script, variables, flags )

    def __init__ ( self, rule, targets, depends, script, variables, flags ):
        depends.insert( 0, script )
        super().__init__( rule, targets, depends )
        self.flags    = flags
        self.variable = variables
        self.command  = [ 'klayout' ]
        for name, value in variables.items():
            self.command += [ '-rd', '{}={}'.format(name,value) ]
        self.command += [ '-b', '-r', self.file_depend(0).as_posix() ]
       #self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage

        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Klayout.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Klayout.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

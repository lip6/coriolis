
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass
class BadDrcRules   ( Exception ): pass


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
        self.addClean( self.targets )

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


class DRC ( Klayout ):

    _drcRules = None

    @staticmethod
    def setDrcRules ( rules ):
        if   isinstance(rules,Path): pass
        elif isinstance(rules,str):  rules = Path( rules )
        else:
            raise BadDrcRules( '[ERROR] DRC.setDrcRules(): Should be <str> or <Path> ({})' \
                               .format( rules ))
        if not rules.is_file():
            raise BadDrcRules( '[ERROR] DRC.setDrcRules(): File not found "{}"' \
                               .format( rules ))
        DRC._drcRules = rules

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return DRC( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        from ..helpers.io import ErrorMessage

        depends = FlowTask._normFileList( depends )
        targets = [ depends[0].with_suffix('.kdrc-report.txt') ]
        if not DRC._drcRules:
            raise ErrorMessage( 1, 'DRC.doTask(): No DRC rules defined.' )
        variables = { 'input'  : depends[0]
                    , 'report' : targets[0]
                    }
        super().__init__( rule, targets, depends, DRC._drcRules, variables, flags )
        

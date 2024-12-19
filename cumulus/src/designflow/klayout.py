
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass
class BadDrcRules   ( Exception ): pass
class BadLypFile    ( Exception ): pass


class Klayout ( FlowTask ):

    Verbose = 0x0001

    _lypFile = None

    @staticmethod
    def setLypFile (lypFile ):
        if   isinstance(lypFile,Path): pass
        elif isinstance(lypFile,str):  lypFile = Path( lypFile )
        else:
            raise BadLypFile( '[ERROR] Klayout.setLypFile(): Should be <str> or <Path> ({})' \
                              .format( lypFile ))
        if not lypFile.is_file():
            raise BadLypFile( '[ERROR] Klayout.setLypFile(): File not found "{}"' \
                              .format( lypFile ))
        Klayout._lypFile = lypFile

    @staticmethod
    def mkRule ( rule, targets=[], depends=[], script=None, arguments=[], variables={}, env={}, flags=0 ):
        return Klayout( rule, targets, depends, script, arguments, variables, env, flags )

    def __init__ ( self, rule, targets, depends, script, arguments, variables, env, flags ):
        if script and not isinstance(script,Path):
            script = Path( script )
        if script:
            depends.append( script )
        super().__init__( rule, targets, depends )
        self.flags     = flags
        self.arguments = arguments
        self.variable  = variables
        self.env       = env
        self.command   = [ 'klayout' ] + arguments
        if Klayout._lypFile:
            self.command += [ '-l', Klayout._lypFile.as_posix() ]
        for name, value in variables.items():
            if value is None:
                self.command += [ '-rd', '{}'.format(name) ]
            else:
                self.command += [ '-rd', '{}={}'.format(name,value) ]
        if script:
            self.command += [ '-r', script.as_posix() ]
        if self.file_depend(0) and not (self.file_depend(0) == script):
            self.command += [ self.file_depend(0).as_posix() ]
#       else:
#           if self.file_target(0):
#               self.command += [  self.file_target(0).as_posix() ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        for variable, value in self.env.items():
            shellEnv[ variable ] = value
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Klayout.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Klayout.doTask' )

    def asDoitTask ( self ):
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

        arguments = []
        depends   = FlowTask._normFileList( depends )
        targets   = [ depends[0].with_suffix('.kdrc-report.txt') ]
        if not DRC._drcRules:
            raise ErrorMessage( 1, 'DRC.doTask(): No DRC rules defined.' )
        env       = {}
        variables = { 'in_gds'      : depends[0]
                    , 'input'       : depends[0]
                    , 'report'      : targets[0]
                    , 'report_file' : targets[0]
                    }
        super().__init__( rule, targets, depends, DRC._drcRules, arguments, variables, env, flags )
        

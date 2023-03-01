
import os
import shutil
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Copy ( FlowTask ):

    @staticmethod
    def mkRule ( rule, targets, depends=[] ):
        return Copy( rule, targets, depends )

    def __init__ ( self, rule, targets, depends ):
        super().__init__( rule, targets, depends )
        self.sourceFile = self.file_depend(0)
        self.targetFile = self.targets[0]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<cp {} {}>'.format( self.sourceFile.as_posix(), self.targetFile.as_posix() )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        try:
            shutil.copyfile( self.sourceFile, self.targetFile )
        except Exception as e:
            e = ErrorMessage( 1, [ 'Copy.doTask(): shutil.copyfile failed.'
                                 , str(e) ] )
            return TaskFailed( e )
        return self.checkTargets( 'Copy.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : 'copy_' + self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

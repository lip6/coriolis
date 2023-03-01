
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Flatph ( FlowTask ):

    Transistor = 0x0001
    Catalog    = 0x0002

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Flatph( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags    = flags
        self.instFile = None
        self.hierFile = self.file_depend(0)
        self.flatFile = self.targets[0]
        self.command  = [ 'flatph' ]
        if flags & Flatph.Transistor: self.command.append( '-t' )
        if flags & Flatph.Catalog:    self.command.append( '-r' )
        self.command.append( self.hierFile.stem )
        if len(self.targets) > 1:
            self.instFile = self.targets[1]
            self.command.append( self.instFile.stem )
        self.command.append( self.flatFile.stem )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_IN_PH' ] = self.hierFile.suffix[1:]
        shellEnv[ 'MBK_IN_PH' ] = self.flatFile.suffix[1:]
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Flatph.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Flatph.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

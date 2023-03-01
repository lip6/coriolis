
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Graal ( FlowTask ):

    Debug   = 0x0001
    Xor     = 0x0002
    Install = 0x0004
    Force   = 0x0008

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return Graal( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        super().__init__( rule, [], depends )
        self.flags         = flags
        self.layoutFile    = self.file_depend(0)
        self.command       = [ 'graal', '-l', self.layoutFile.stem ]
        if flags & Graal.Debug:   self.command.append( '-debug' )
        if flags & Graal.Xor:     self.command.append( '-xor' )
        if flags & Graal.Install: self.command.append( '-install' )
        if flags & Graal.Force:   self.command.append( '-force' )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_IN_PH' ] = self.layoutFile.suffix[1:]
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Graal.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Graal.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'uptodate' : [ False ]
               }
        

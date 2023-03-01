
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Dreal ( FlowTask ):

    Debug   = 0x0001
    Xor     = 0x0002
    Install = 0x0004
    Force   = 0x0008

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return Dreal( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        super().__init__( rule, [], depends )
        self.flags         = flags
        self.layoutFile    = self.file_depend(0)
        self.command       = [ 'dreal', '-l', self.layoutFile.stem ]
        if flags & Dreal.Debug:   self.command.append( '-debug' )
        if flags & Dreal.Xor:     self.command.append( '-xor' )
        if flags & Dreal.Install: self.command.append( '-install' )
        if flags & Dreal.Force:   self.command.append( '-force' )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'RDS_IN' ] = self.layoutFile.suffix[1:]
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Dreal.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Dreal.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'uptodate' : [ False ]
               }
        

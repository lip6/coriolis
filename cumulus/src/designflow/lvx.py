
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Lvx ( FlowTask ):

    MergeSupply     = 0x0001
    SaveReorder     = 0x0002
    CheckUnassigned = 0x0004
    Flatten         = 0x0008

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        return Lvx( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        from .. import CRL

        super().__init__( rule, [], depends )
        self.flags         = flags
        self.referenceFile = self.file_depend(0)
        self.checkedFile   = self.file_depend(1)
        self.command       = [ 'lvx'
                             , self.referenceFile.suffix[1:]
                             , self.checkedFile.suffix[1:]
                             , self.referenceFile.stem
                             , self.checkedFile.stem ]
        if flags & Lvx.MergeSupply:     self.command.append( '-a' )
        if flags & Lvx.SaveReorder:     self.command.append( '-o' )
        if flags & Lvx.CheckUnassigned: self.command.append( '-u' )
        if flags & Lvx.Flatten:         self.command.append( '-f' )

        if self.flags & Lvx.SaveReorder:
            env = CRL.AllianceFramework.get().getEnvironment()
            self.targets = [ self.checkedFile.with_suffix('.' + env.getOUT_LO()) ]
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
            e = ErrorMessage( 1, 'Lvx.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Lvx.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

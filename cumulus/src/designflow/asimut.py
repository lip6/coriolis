
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Asimut ( FlowTask ):

    RootIsBehavioral = 0x0001
    UseBdd           = 0x0002
    ZeroDelay        = 0x0004

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Asimut( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.vhdlFile   = self.file_depend(0)
        self.patFile    = self.file_depend(1)
        self.simFile    = self.targets[0]
        self.command    = [ 'asimut' ]
        if flags & Asimut.RootIsBehavioral: self.command.append( '-b' )
        if flags & Asimut.UseBdd:           self.command.append( '-bdd' )
        if flags & Asimut.ZeroDelay:        self.command.append( '-zerodelay' )
        self.command   += [ self.vhdlFile.stem, self.patFile.stem, self.simFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_IN_LO' ] = 'vst'
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Asimut.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Asimut.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }


import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class S2R ( FlowTask ):

    NoDenotch           = 0x0001
    DeleteNames         = 0x0002
    DoBlackboxes        = 0x0004
    NoReplaceBlackboxes = 0x0008
    Verbose             = 0x0010

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return S2R( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 's2r' ]
        if flags & S2R.NoDenotch:           self.command.append( '-t' )
        if flags & S2R.DeleteNames:         self.command.append( '-c' )
        if flags & S2R.DoBlackboxes:        self.command.append( '-1' )
        if flags & S2R.NoReplaceBlackboxes: self.command.append( '-r' )
        if flags & S2R.Verbose:             self.command.append( '-v' )
        self.command   += [ self.inputFile.stem, self.outputFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'RDS_OUT'   ] = self.outputFile.suffix[1:]
        shellEnv[ 'MBK_IN_PH' ] = self.inputFile .suffix[1:]
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'S2R.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'S2R.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

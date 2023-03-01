
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class UnsupportedVHdlSuffix ( Exception ): pass


class Boom ( FlowTask ):

    FlagMask          = 0x00001100
    Verbose           = 0x00000100
    TraceOn           = 0x00000200
    ReverseBdd        = 0x00000400
    LocalOptimization = 0x00000800

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Boom( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 'boom' ]
        if flags & Boom.Verbose:           self.command.append( '-V' )
        if flags & Boom.TraceOn:           self.command.append( '-T' )
        if flags & Boom.ReverseBdd:        self.command.append( '-O' )
        if flags & Boom.LocalOptimization: self.command.append( '-A' )
        self.command += [ self.inputFile.stem, self.outputFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Boom.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Boom.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }


import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class MissingTarget ( Exception ): pass


class Cougar ( FlowTask ):

    Transistor = 0x0001
    Flatten    = 0x0002
    Verbose    = 0x0004
    Core       = 0x0008
    GroundCap  = 0x0010
    WireRC     = 0x0020

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Cougar( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.inputFile  = self.file_depend(0)
        self.outputFile = self.targets[0]
        self.command    = [ 'cougar' ]
        if flags & Cougar.Transistor: self.command.append( '-t' )
        if flags & Cougar.Flatten:    self.command.append( '-f' )
        if flags & Cougar.Verbose:    self.command.append( '-v' )
        if flags & Cougar.Core:       self.command.append( '-c' )
        if flags & Cougar.GroundCap:  self.command.append( '-ac' )
        if flags & Cougar.WireRC:     self.command.append( '-ar' )
        self.command   += [ self.inputFile.stem, self.outputFile.stem ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage

        shellEnv = ShellEnv()
        shellEnv[ 'MBK_OUT_LO' ] = self.outputFile.suffix[1:]
        shellEnv[ 'MBK_IN_PH'  ] = self.inputFile .suffix[1:]
        shellEnv.export()
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Cougar.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Cougar.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        


import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class UnsupportedVHdlSuffix ( Exception ): pass


class Vasy ( FlowTask ):

    FlagMask         = 0x00000011
    Verbose          = 0x00000001
    UseStdLogic      = 0x00000002
    AddPowerSupplies = 0x00000004

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Vasy( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags      = flags
        self.vhdlFile   = self.file_depend(0)
        if len(self.targets) > 1:
            self.outputFile = self.targets[1].with_suffix('.vbe')
        else:
            self.outputFile = self.targets[0]
        self.command = [ 'vasy' ]
        if flags & Vasy.Verbose:          self.command.append( '-V' )
        if flags & Vasy.UseStdLogic:      self.command.append( '-S' )
        if flags & Vasy.AddPowerSupplies: self.command.append( '-p' )

        if self.outputFile.suffix in ('.v', '.vlog'):
            self.command.append( '-v' )
        elif self.outputFile.suffix in ('.vbe', '.vst'):
            self.command.append( '-a' )
        elif self.outputFile.suffix ('.vhd',):
            self.command.append( '-s' )
        if not (self.vhdlFile.suffix in ('.vbe', '.vst', '.vhd', '.vhdl')):
            raise UnsupportedVHdlSuffix( 'Vasy.__init__(): File "{}" has unsupported VHDL suffix (not .vbe, .vst, .vhd, .vhdl).' )
        self.command += [ '-I', self.vhdlFile.suffix[1:] ]
        self.command   += [ self.vhdlFile.stem, self.outputFile.stem ]
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
            e = ErrorMessage( 1, 'Vasy.doTask(): UNIX command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Vasy.doTask' )

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

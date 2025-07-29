
import os
import re
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask, ShellEnv


class UnsupportedVHdlSuffix ( Exception ): pass
class BadStdCellLib         ( Exception ): pass


class Iverilog ( FlowTask ):
    """
    Rule to call the Icarus Verilog simulator. It will call both ``iverilog``
    then ``vvp``. Two files, with non configurable names will be generateds:

    1. ``test_icarus``, the full compiled Verilog file.
    2. ``dump.vcd``, the outputed waveform file.
    """

    _stdCellLib = None

    @staticmethod
    def setStdCellLib ( stdCellLib ):
        """
        Set up the Verilog file supplying all the standard cells modules.
        """
        if   isinstance(stdCellLib,Path): pass
        elif isinstance(stdCellLib,str):  stdCellLib = Path( stdCellLib )
        else:
            raise BadStdCellLib( '[ERROR] Iverilog.setStdCellLib(): Should be <str> or <Path> ({})' \
                              .format( stdCellLib ))
        if not stdCellLib.is_file():
            raise BadStdCellLib( '[ERROR] Iverilog.setStdCellLib(): File not found "{}"' \
                              .format( stdCellLib ))
        Iverilog._stdCellLib = stdCellLib

    @staticmethod
    def mkRule ( rule, depends, flags=0 ):
        """
        Creates a new rule instance (``doit`` task) of Iverilog.

        :param rule:    The name of the rule (``basename`` for ``doit``).
        :param depends: A scalar or a list of file, ``pathlib.Path`` or other rule
                        instances. In the later case all the *targets* of the rules are
                        considered as dependencies.

        There is no target argument. Two targets are generateds : ``test_icarus`` and
        ``dump.vcd``.

        If a Verilog standard cell file has been added with Yosys.setStdCelllib(), it
        will be automatically added to the list of sources files to compile.
        """
        return Iverilog( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        targets = [ 'dump.vcd', 'test_icarus' ]
        super().__init__( rule, targets, depends )
        self.flags      = flags

        self.command = [ 'iverilog', '-o', 'test_icarus' ]
        if Iverilog._stdCellLib:
            self.command.append( Iverilog._stdCellLib.as_posix() )
        for depend in self.file_dep:
            self.command.append( depend.as_posix() )
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL        import AllianceFramework
        from ..helpers.io import ErrorMessage, WarningMessage

        shellEnv = ShellEnv()
        shellEnv.export()
        print( '   -> Running "{}" ...'.format( ' '.join(self.command) ))
        state = subprocess.run( self.command )
        if state.returncode:
            e = ErrorMessage( 1, 'Iverilog.doTask(): UNIX <iverilog> command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        vppCommand = [ 'vvp', 'test_icarus' ]
        print( '   -> Running "{}" ...'.format( ' '.join(vppCommand) ))
        state = subprocess.run( vppCommand )
        if state.returncode:
            e = ErrorMessage( 1, 'Iverilog.doTask(): UNIX <vvp> command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'Iverilog.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

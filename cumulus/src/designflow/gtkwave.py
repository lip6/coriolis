
import os
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   ..helpers.io import ErrorMessage, WarningMessage
from   .task import FlowTask, ShellEnv


class GtkWave ( FlowTask ):
    """
    Rule to call the GtkWave visualization tool.
    """

    @staticmethod
    def mkRule ( rule, depends=[], flags=0 ):
        """
        Creates a new rule instance (``doit`` task) of GtkWave.

        :param rule:    The name of the rule (``basename`` for ``doit``).
        :param depends: A scalar or a list of file, ``pathlib.Path`` or other rule
                        instances. In the later case all the *targets* of the rules are
                        considered as dependencies.

        if the ``depends`` parameter is empty, then the default ``dump.vcd`` will
        be used.

        .. note:: **signals.gtkw**, if this state saving file is present it will be
                  automatically loaded.
        """
        return GtkWave( rule, depends, flags )

    def __init__ ( self, rule, depends, flags ):
        if not depends: depends = [ 'dump.vcd' ]
        super().__init__( rule, [], depends )
        self.vcdFile = None
        for depend in self.file_dep:
            if depend.suffix == '.vcd':
                self.vcdFile = depend
                break
        if not self.vcdFile:
            message = [ 'GtkWave.__init__(): No ".vcd" file found in dependencies:' ]
            for depend in self.file_dep:
                message.append( '- "{}"'.format( depend ))
            raise ErrorMessage( 1, message ) 
        self.flags   = flags
        self.command = [ 'gtkwave', self.vcdFile.as_posix() ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<{}>'.format( ' '.join(self.command) )

    def doTask ( self ):
        from ..CRL import AllianceFramework

        shellEnv = ShellEnv()
        shellEnv.export()
        command = self.command
        logFile = Path( 'signals.gtkw' )
        if logFile.is_file():
            command.append( logFile.as_posix() )
        print( '   -> Running "{}" ...'.format( ' '.join(command) ))
        state = subprocess.run( command )
        if state.returncode:
            e = ErrorMessage( 1, 'GtkWave.doTask(): UNIX <gtkwave> command failed ({}).' \
                                 .format( state.returncode ))
            return TaskFailed( e )
        return self.checkTargets( 'GtkWave.doTask' )

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               , 'uptodate' : [ False ]
               }

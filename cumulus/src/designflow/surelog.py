
import os.path
import shutil
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask


def printCommand ( command ):
    commandBin = command[0]
    print( commandBin, command[1] )
    for arg in command[2:]:
        print( ' '*len(commandBin), arg )


class Surelog ( FlowTask ):

    @staticmethod
    def mkRule ( rule, depends, top, incdirs=[], libdirs=[], defines=[], options=[], flags=0 ):
        return Surelog( rule, depends, top, incdirs, libdirs, defines, options, flags )

    def __init__ ( self, rule, depends, top, incdirs, libdirs, defines, options, flags ):
        self.flags   = flags
        self.top     = top
        self.incdirs = incdirs
        self.libdirs = libdirs
        self.defines = defines
        self.options = options
        self.success = True
        targets = FlowTask._normFileList( [ self.top + '.uhdm' ] )
        depends = FlowTask._normFileList( depends )
        super().__init__( rule, targets, depends )
        self.addClean( targets )

    def __repr__ ( self ):
        return '<surelog {} top={}>'.format( self.main, self.top )

    @property
    def main ( self ):
        return self.file_depend( 0 )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        for incdir in self.incdirs:
            if not Path(incdir).is_dir():
                e = ErrorMessage( 1, [ 'Surelog.doTask(): Include directory not found "{}"'
                                     , '"{}"'.format( incdir ) ] )
                return TaskFailed( e )
        command  = [ 'surelog', '-parse' ]
        command += [ ' '.join( self.options ) ]
        command += [ '-D{}'.format(d) for d in self.defines ]
        command += [ '-I{}'.format(i) for i in self.incdirs ]
        for libdir in self.libdirs:
            command += [ '-L', libdir ]
        command += [ '-top', self.top ]
        command += [ depend.as_posix() for depend in self.depends ]
        printCommand( command )
        status = subprocess.call( command )
        if status != 0: return False
        shutil.move( 'slpp_all/surelog.uhdm', self.file_target(0) )
        return True

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }


import os.path
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask


def printCommand ( command ):
    commandBin = command[0]
    print( commandBin, command[1] )
    for arg in command[2:]:
        print( ' '*len(commandBin), arg )


class Sv2v ( FlowTask ):

    FlagLog = 0x00000001

    @staticmethod
    def mkRule ( rule, targets, depends, top, incdirs=[], libdirs=[], defines=[], flags=0 ):
        return Sv2v( rule, targets, depends, top, incdirs, libdirs, defines, flags )

    def __init__ ( self, rule, targets, depends, top, incdirs, libdirs, defines, flags ):
        self.flags   = flags
        self.top     = top
        self.incdirs = incdirs
        self.libdirs = libdirs
        self.defines = defines
        self.log     = None
        self.success = True
        targets = FlowTask._normFileList( targets )
        depends = FlowTask._normFileList( depends )
        if targets == []:
            targets.append( self.top + '.v' )
       #if self.flags & Sv2v.FlagLog:
       #    self.log = Path( self.top + '.log' )
       #    targets.append( self.log )
        super().__init__( rule, targets, depends )
        self.addClean( targets )

    def __repr__ ( self ):
        return '<sv2v {} top={}>'.format( self.main, self.top )

    @property
    def main ( self ):
        return self.file_depend( 0 )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        for incdir in self.incdirs:
            if not Path(incdir).is_dir():
                e = ErrorMessage( 1, [ 'Sv2v.doTask(): Include directory not found "{}"'
                                     , '"{}"'.format( incdir ) ] )
                return TaskFailed( e )
        command  = [ 'sv2v' ]
        command += [ '--define={}'.format(d) for d in self.defines ]
        command += [ '--incdir={}'.format(i) for i in self.incdirs ]
        command += [ '--libdir={}'.format(l) for l in self.libdirs ]
        command += [ '--top={}'.format( self.top ) ]
        command += [ depend.as_posix() for depend in self.depends ]
        printCommand( command )
        with open( self.file_target(), 'w' ) as fdOut:
            status = subprocess.call( command, stdout=fdOut )
        return status == 0

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }

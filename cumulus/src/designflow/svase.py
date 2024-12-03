
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


class Svase ( FlowTask ):

    FlagLog = 0x00000001

    @staticmethod
    def mkRule ( rule, targets, depends, top, incdirs=[], libdirs=[], defines=[], svargs=[], flags=0 ):
        return Svase( rule, targets, depends, top, incdirs, libdirs, defines, svargs, flags )

    def __init__ ( self, rule, targets, depends, top, incdirs, libdirs, defines, svargs, flags ):
        self.flags   = flags
        self.top     = top
        self.svargs  = svargs
        self.incdirs = incdirs
        self.libdirs = libdirs
        self.defines = defines
        self.log     = None
        self.success = True
        targets = FlowTask._normFileList( targets )
        depends = FlowTask._normFileList( depends )
        if targets == []:
            targets.append( self.top + '.v' )
       #if self.flags & Svase.FlagLog:
       #    self.log = Path( self.top + '.log' )
       #    targets.append( self.log )
        targets.append( './slang-args.txt' )
        super().__init__( rule, targets, depends )
        self.addClean( targets )

    def __repr__ ( self ):
        return '<svase top={} {} {}>'.format( self.top, self.file_target(0), self.main )

    @property
    def main ( self ):
        return self.file_depend( 0 )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        for incdir in self.incdirs:
            if not Path(incdir).is_dir():
                e = ErrorMessage( 1, [ 'Svase.doTask(): Include directory not found.'
                                     , '"{}"'.format( incdir ) ] )
                return TaskFailed( e )
        with open( 'slang-args.txt', 'w' ) as fdSlangArgs:
            for svarg in self.svargs:
                fdSlangArgs.write( '{}\n'.format(svarg) )
            for define in self.defines:
                fdSlangArgs.write( '-D{}\n'.format(define) )
            for incdir in self.incdirs:
                fdSlangArgs.write( '--include-directory {}\n'.format(incdir) )
            for libdir in self.libdirs:
                fdSlangArgs.write( '--libdir {}\n'.format(libdir) )

        command  = [ 'svase' ]
        command += [ '--slang-argfile=slang-args.txt' ]
        command += [ self.top ]
        command += [ self.file_target(0) ]
        command += [ depend.as_posix() for depend in self.depends ]
        printCommand( command )
        status = subprocess.call( command )
        return status == 0

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }

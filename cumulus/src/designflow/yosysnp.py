
import os.path
import subprocess
from   pathlib import Path
from   pyosys          import libyosys as yosys
from   doit.exceptions import TaskFailed
from   .task           import FlowTask


class BadLiberty  ( Exception ): pass


class YosysNp ( FlowTask ):

    FlagLog       = 0x00000001
    FlagQuiet     = 0x00000002
    FlagUseYoWasp = 0x00000004
    _liberty      = None

    @staticmethod
    def setLiberty ( liberty ):
        if   isinstance(liberty,Path): pass
        elif isinstance(liberty,str):  liberty = Path( liberty )
        else:
            raise BadLiberty( '[ERROR] YosysNp.setLiberty(): Should be <str> or <Path> ({})' \
                              .format( liberty ))
        if not liberty.is_file():
            raise BadLiberty( '[ERROR] YosysNp.setLiberty(): File not found "{}"' \
                              .format( liberty ))
        YosysNp._liberty = liberty

    @staticmethod
    def mkRule ( rule, targets, depends, script, top=None, flags=0 ):
        return YosysNp( rule, targets, depends, script, top, flags )

    def __init__ ( self, rule, targets, depends, script, top, flags ):
        self.log     = None
        self.success = True
        self.script  = script
        self.flags   = flags
        targets = FlowTask._normFileList( targets )
        depends = FlowTask._normFileList( depends )
        if top is not None:
            self.top = top
        else:
            self.top = depends[0].stem
        if targets == []:
            targets.append( self.top + '.blif' )
        if self.flags & YosysNp.FlagLog:
            self.log = Path( self.top + '.log' )
            targets.append( self.log )
        targets.append( self.top + '.ys' )
        super().__init__( rule, targets, depends )
        self.addClean( targets )

    def __repr__ ( self ):
        return '<yosysnp {} top={}>'.format( self.main, self.top )

    @property
    def liberty ( self ):
        return YosysNp._liberty

    @property
    def main ( self ):
        return self.file_depend( 0 )

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        if self.liberty is None:
            e = ErrorMessage( 1, [ 'YosysNp.doTask(): "liberty" has not been set' ] )
            return TaskFailed( e )
        if not self.liberty.is_file():
            e = ErrorMessage( 1, [ 'YosysNp.doTask(): File not found "{}"'
                                 , '"{}"'.format( self.liberty.as_posix() ) ] )
            return TaskFailed( e )
        ysFile = Path(self.main).stem + '.ys'
        with open( ysFile, 'w' ) as ysFd:
            ysFd.write( self.script.format( liberty =str(self.liberty)
                                          , cellname=self.main.stem
                                          , top     =self.top ))
        command  = [ 'yowasp-yosys' ] if self.flags & YosysNp.FlagUseYoWasp else [ 'yosys' ]
        if self.flags & YosysNp.FlagQuiet: command += [ '-q' ]
        if self.flags & YosysNp.FlagLog:   command += [ '-l', self.log.as_posix() ]
        command += [ '-s', ysFile ]
        status = subprocess.call( command )
        return status == 0

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }

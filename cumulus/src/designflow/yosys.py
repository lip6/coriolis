
import os.path
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask


class BadLiberty  ( Exception ): pass


class Yosys ( FlowTask ):

    _liberty = None

    @staticmethod
    def setLiberty ( liberty ):
        if   isinstance(liberty,Path): pass
        elif isinstance(liberty,str):  liberty = Path( liberty )
        else:
            raise BadLiberty( '[ERROR] Yosys.setLiberty(): Should be <str> or <Path> ({})' \
                              .format( liberty ))
        if not liberty.is_file():
            raise BadLiberty( '[ERROR] Yosys.setLiberty(): File not found "{}"' \
                              .format( liberty ))
        Yosys._liberty = liberty

    @staticmethod
    def mkRule ( rule, depends, top=None, blackboxes=[], flattens=[] ):
        return Yosys( rule, depends, top, blackboxes, flattens )

    def __init__ ( self, rule, depends, top, blackboxes, flattens ):
        super().__init__( rule, [], depends )
        self.success    = True
        self.blackboxes = blackboxes
        self.flattens   = flattens
        self.depends   += blackboxes
        if top is not None:
            self.top = top
        else:
            self.top = self.main.stem
        self.targets = [ Path( self.top + '.blif') ]
        self.addClean( self.targets )

    def __repr__ ( self ):
        return '<yosys {} top={} blackboxes=[{}] flattens=[{}]>' \
               .format( self.main, self.top, ','.join(self.blackboxes), ','.join(self.flattens) )

    @property
    def liberty ( self ):
        return Yosys._liberty

    @property
    def main ( self ):
        return self.file_depend( 0 )

    def _run_pass ( self, command ):
        from pyosys import libyosys as yosys
        if self.success is not True: return
        yosys.run_pass( command, self.tool )

    def _loadDesign ( self, design ):
        from ..helpers.io import ErrorMessage
        if self.success is not True: return
        if not os.path.isfile(design):
            e = ErrorMessage( 1, 'Yosys._loadDesign(): Can\'t find design file "{}".'.format( design ))
            self.success = TaskFailed( e )
            return
        if   design.suffix == '.v' : self._run_pass( 'read_verilog {}'.format( design.as_posix() ))
        elif design.suffix == '.il': self._run_pass( 'read_ilang   {}'.format( design.as_posix() ))
        else:
            e = ErrorMessage( 1, 'Yosys._loadDesign(): Unsupported input format for "{}".'.format( design ))
            self.success = TaskFailed( e )
            return

    def _loadBlackboxes ( self ):
        if self.success is not True: return
        for blackbox in self.blackboxes:
            self._loadDesign( blackbox )

    def _doFlattens ( self ):
        if self.success is not True: return
        flattens = ' '.join( self.flattens )
        self._run_pass( 'flatten        {}\n'.format( flattens ))
        self._run_pass( 'hierarchy -top {}\n'.format( self.top ))

    def doTask ( self ):
        from pyosys       import libyosys as yosys
        from ..helpers.io import ErrorMessage
        if self.liberty is None:
            e = ErrorMessage( 1, [ 'Yosys.doTask(): "liberty" has not been set' ] )
            return TaskFailed( e )
        if not self.liberty.is_file():
            e = ErrorMessage( 1, [ 'Yosys.doTask(): File not found "{}"'
                                 , '"{}"'.format( self.liberty.as_posix() ) ] )
            return TaskFailed( e )
       #print( 'Yosys.doTask() on "{}"'.format( self.design ))
        self.tool = yosys.Design()
        self._loadBlackboxes()
        self._loadDesign( self.main )
        self._run_pass( 'hierarchy -check -top {}'.format( self.top ))
        self._run_pass( 'synth            -top {}'.format( self.top ))
        self._doFlattens()
        self._run_pass( 'memory' )
        self._run_pass( 'dfflibmap -liberty {}'.format( self.liberty.as_posix() ))
        self._run_pass( 'abc       -liberty {}'.format( self.liberty.as_posix() ))
        self._run_pass( 'clean' )
        self._run_pass( 'write_blif {}'.format( self.targets[0] ))
        return self.success

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }

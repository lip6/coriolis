
import os.path
import shutil
import subprocess
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task           import FlowTask

usePyYosys = True
try:
    from pyosys import libyosys as yosys
except:
    usePyYosys = False


class BadLiberty  ( Exception ): pass


class Yosys ( FlowTask ):

    FlagLog           = 0x00000001
    FlagQuiet         = 0x00000002
    FlagSystemVerilog = 0x00000004
    _liberty          = None

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
    def mkRule ( rule
               , depends
               , top       =None
               , blackboxes=[]
               , flattens  =[]
               , svOptions =None
               , svDefines =None
               , svIncdirs =None
               , svLibdirs =None
               , script    =[]
               , flags     =0 ):
        return Yosys( rule
                    , depends
                    , top
                    , blackboxes
                    , flattens
                    , svOptions
                    , svDefines
                    , svIncdirs
                    , svLibdirs
                    , script
                    , flags )

    def __init__ ( self, rule
                       , depends
                       , top
                       , blackboxes
                       , flattens
                       , svOptions
                       , svDefines
                       , svIncdirs
                       , svLibdirs
                       , script
                       , flags ):
        super().__init__( rule, [], depends )
        self.blackboxes = blackboxes
        self.flattens   = flattens
        self.depends   += blackboxes
        self.svOptions  = svOptions
        self.svDefines  = svDefines
        self.svIncdirs  = svIncdirs
        self.svLibdirs  = svLibdirs
        self.flags      = flags
        self.script     = script
        self.success    = True
        if top is not None:
            self.top = top
        else:
            self.top = self.main.stem
        self.targets = [ Path( self.top + '.blif') ]
        if not usePyYosys:
            self.targets.append( Path( self.top + '.ys' ))
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

    def _loadDesign ( self, design ):
        from ..helpers.io import ErrorMessage
        if self.success is not True: return
        if not os.path.isfile(design):
            e = ErrorMessage( 1, 'Yosys._loadDesign(): Can\'t find design file "{}".'.format( design ))
            self.success = TaskFailed( e )
            return
        design = Path( design )
        if   design.suffix == '.v' : self.script.append( 'read_verilog -sv {}'.format( design.as_posix() ))
        elif design.suffix == '.il': self.script.append( 'read_ilang       {}'.format( design.as_posix() ))
        else:
            e = ErrorMessage( 1, 'Yosys._loadDesign(): Unsupported input format for "{}".'.format( design ))
            self.success = TaskFailed( e )
            return

    def _loadSVDesign ( self ):
        from ..helpers.io import ErrorMessage
        if self.success is not True: return
        self.script.append( 'plugin -i systemverilog' )
        svFileArgs = ''
        for svFile in self.depends:
            if not isinstance(svFile,Path):
                svFile = Path( svFile )
            if not svFile.is_file():
                print( '[WARNING] Can\'t find SV file "{}".'.format( svFile.as_posix() ))
                continue
            svFileArgs += ' {}'.format( svFile.as_posix() )
        defineArgs = ''
        for define in self.svDefines:
            defineArgs += ' -D{}'.format( define )
        includeArgs = ''
        for incdir in self.svIncdirs:
            includeArgs += ' -I{}'.format( incdir )
        libArgs = ''
        for libdir in self.svLibdirs:
            libArgs += ' -L {}'.format( libdir )
        options = ' '.join( self.svOptions )
        scriptArgs = { 'options'  :options
                     , 'defines'  :defineArgs
                     , 'includes' :includeArgs
                     , 'libraries':libArgs
                     , 'svFiles'  :svFileArgs
                     , 'top'      : self.top }
        self.script.append( 'read_systemverilog {options} -top {top} {defines} {includes} {libraries} {svFiles}' \
                            .format( **scriptArgs ))
        

    def _loadBlackboxes ( self ):
        if self.success is not True: return
        for blackbox in self.blackboxes:
            self._loadDesign( blackbox )

    def _doFlattens ( self ):
        if self.success is not True: return
        flattens = ' '.join( self.flattens )
        self.script.append( 'flatten        {}'.format( flattens ))
        self.script.append( 'hierarchy -top {}'.format( self.top ))

    def _runScript ( self ):
        from ..helpers.io import ErrorMessage
        if usePyYosys:
            tool = yosys.Design()
            for command in self.script:
                yosys.run_pass( command, tool )

        if   shutil.which(        'yosys' ): command = [ 'yosys' ]
        elif shutil.which( 'yowasp-yosys' ): command = [ 'yowasp-yosys' ]
        else:
            e = ErrorMessage( 1, [ 'Yosys._runScript(): Neither "yosys" nor "yowasp-yosys" has been found' ] )
            self.success = TaskFailed( e )
            return
        ysFile = self.targets[-1].as_posix()
        with open( ysFile, 'w' ) as ysFd:
            ysFd.write( '\n'.join( self.script ).format( liberty =str(self.liberty)
                                                       , cellname=self.main.stem
                                                       , top     =self.top ))
        if self.flags & Yosys.FlagQuiet: command += [ '-q' ]
        if self.flags & Yosys.FlagLog:   command += [ '-l', self.log.as_posix() ]
        command += [ '-s', ysFile ]
        status = subprocess.call( command )
        self.success = (status == 0)

    def doTask ( self ):
        from ..helpers.io import ErrorMessage
        if self.liberty is None:
            e = ErrorMessage( 1, [ 'Yosys.doTask(): "liberty" has not been set' ] )
            return TaskFailed( e )
        if not self.liberty.is_file():
            e = ErrorMessage( 1, [ 'Yosys.doTask(): File not found "{}"'
                                 , '"{}"'.format( self.liberty.as_posix() ) ] )
            return TaskFailed( e )
       #print( 'Yosys.doTask() on "{}"'.format( self.design ))
        self._loadBlackboxes()
        if self.flags & Yosys.FlagSystemVerilog:
            self._loadSVDesign()
        else:
            self._loadDesign( self.main )
        self.script.append( 'hierarchy -check -top {}'.format( self.top ))
        self.script.append( 'synth            -top {}'.format( self.top ))
        self._doFlattens()
        self.script.append( 'memory' )
        self.script.append( 'dfflibmap -liberty {}'.format( self.liberty.as_posix() ))
        self.script.append( 'abc       -liberty {}'.format( self.liberty.as_posix() ))
        self.script.append( 'clean')
        self.script.append( 'write_blif {}'.format( self.targets[0] ))
        self._runScript()
        return self.success

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'file_dep' : self.file_dep
               , 'targets'  : self.targets
               }

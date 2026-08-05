
import shutil
from pathlib import Path
from doit.exceptions import TaskFailed
from .task import FlowTask


class MissingTarget ( Exception ): pass


class Clean ( FlowTask ):

    @staticmethod
    def mkRule ( extrasClean=[], extrasGlobs=[] ):
        return Clean( extrasClean, extrasGlobs )

    def __init__ ( self, extrasClean, extrasGlobs ):
        super().__init__( 'clean_flow', [], [] )
        self.extrasClean = FlowTask._normFileList( extrasClean )
        self.extrasGlobs = extrasGlobs

    def __repr__ ( self ):
        return '<clean>'

    def doTask ( self, doExtrasClean ):
        print( '   Removing all target files' )
        print( '   =========================' )
        for filePath in FlowTask.cleanTargets:
            if filePath.is_file():
                print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                filePath.unlink()
            elif filePath.is_dir():
                print( '   - {:<40} [removed (directory)]'.format( filePath.as_posix() ))
                shutil.rmtree( filePath )
            else:
                print( '   - {}'.format( filePath.as_posix() ))
        for directory, glob in FlowTask.cleanGlobs:
            for filePath in Path(directory).glob(glob):
                if filePath.is_file():
                    print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                    filePath.unlink()
                elif filePath.is_dir():
                    print( '   - {:<40} [removed (directory)]'.format( filePath.as_posix() ))
                    shutil.rmtree( filePath )
        if doExtrasClean and len(self.extrasClean):
            print( '   Removing extra clean files' )
            print( '   ==========================' )
            for filePath in self.extrasClean:
                if filePath.is_file():
                    print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                    filePath.unlink()
                elif filePath.is_dir():
                    print( '   - {:<40} [removed (directory)]'.format( filePath.as_posix() ))
                    shutil.rmtree( filePath )
                else:
                    print( '   - {}'.format( filePath.as_posix() ))
        if doExtrasClean and len(self.extrasGlobs):
            print( '   Removing extra clean files (by glob)' )
            print( '   ====================================' )
            for directory, glob in self.extrasGlobs:
                for filePath in Path(directory).glob(glob):
                    if filePath.is_file():
                        print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                        filePath.unlink()
        return True

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Clean all generated (targets) files.'
               , 'params'   : [ { 'name'    : 'doExtrasClean'
                                , 'long'    : 'extras'
                                , 'type'    : bool
                                , 'default' : False
                                } ]
               , 'uptodate' : [ False ]
               }


class CustomClean ( FlowTask ):
    """
    Delete files based on names and globs, purely user-defined.
    That is, do not automatically remove targets generated files.
    """

    @staticmethod
    def mkRule ( rule, cleanFiles=[], cleanGlobs=[] ):
        return CustomClean( cleanFiles, cleanGlobs )

    def __init__ ( self, rule, cleanFiles, cleanGlobs ):
        super().__init__( rule, [], [] )
        self.cleanFiles = FlowTask._normFileList( cleanFiles )
        self.cleanGlobs = cleanGlobs

    def __repr__ ( self ):
        return '<CustomClean "{}">'.format( self.basename )

    def doTask ( self ):
        print( '   Removing files' )
        print( '   ==============' )
        if len(self.cleanFiles):
            for filePath in self.cleanFiles:
                if filePath.is_file():
                    print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                    filePath.unlink()
                elif filePath.is_dir():
                    print( '   - {:<40} [removed (directory)]'.format( filePath.as_posix() ))
                    shutil.rmtree( filePath )
                else:
                    print( '   - {}'.format( filePath.as_posix() ))
        if len(self.cleanGlobs):
            for directory, glob in self.cleanGlobs:
                for filePath in Path(directory).glob(glob):
                    if filePath.is_file():
                        print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                        filePath.unlink()
        return True

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Custom clean files & directories.'
               , 'uptodate' : [ False ]
               }
        

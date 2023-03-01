
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
            else:
                print( '   - {}'.format( filePath.as_posix() ))
        if doExtrasClean and len(self.extrasClean):
            print( '   Removing extra clean files' )
            print( '   ==========================' )
            for filePath in self.extrasClean:
                if filePath.is_file():
                    print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                    filePath.unlink()
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

    def create_doit_tasks ( self ):
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
        

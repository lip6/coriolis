
from pathlib import Path
from doit.exceptions import TaskFailed
from .task import FlowTask


class MissingTarget ( Exception ): pass


class Clean ( FlowTask ):

    @staticmethod
    def mkRule ( extrasClean=[] ):
        return Clean( extrasClean )

    def __init__ ( self, extrasClean ):
        super().__init__( 'clean_flow', [], [] )
        self.extrasClean = extrasClean

    def __repr__ ( self ):
        return '<clean>'

    def doTask ( self, doExtrasClean ):
        print( '   Removing all target files' )
        print( '   =========================' )
        for fileName in FlowTask.cleanTargets:
            filePath = Path( fileName )
            if filePath.is_file():
                print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                filePath.unlink()
            else:
                print( '   - {}'.format( filePath.as_posix() ))
        if doExtrasClean and len(self.extrasClean):
            print( '   Removing extra clean files' )
            print( '   ==========================' )
            for fileName in self.extrasClean:
                filePath = Path( fileName )
                if filePath.is_file():
                    print( '   - {:<40} [removed]'.format( filePath.as_posix() ))
                    filePath.unlink()
                else:
                    print( '   - {}'.format( filePath.as_posix() ))
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
        

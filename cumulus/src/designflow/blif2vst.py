
from pathlib import Path
from doit.exceptions import TaskFailed
from .task import FlowTask


class TargetNotVst ( Exception ): pass


def renameNMigen( occurrence ):
    masterCell = occurrence.getEntity().getMasterCell()
    origName   = masterCell.getName()
    replName   = origName.replace( '$$', '_unm' )
    if not masterCell.isTerminalNetlist() and not replName.startswith('cmpt_'):
        replName = 'cmpt_' + replName
    #for letter in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
    #    replName = replName.replace(letter, '{}u'.format(letter))
    if origName != replName:
        print( '  - "{}" => "{}"'.format(origName,replName) )
        masterCell.setName( replName )


def renameNMigenUniquify ( topCell ):
    for occurrence in topCell.getTerminalNetlistInstanceOccurrences():
        renameNMigen(occurrence)
    for occurrence in topCell.getNonTerminalNetlistInstanceOccurrences():
        renameNMigen(occurrence)
    return


class Blif2Vst ( FlowTask ):

    @staticmethod
    def mkRule ( rule, targets, depends=[], flags=0 ):
        return Blif2Vst( rule, targets, depends, flags )

    def __init__ ( self, rule, targets, depends, flags ):
        super().__init__( rule, targets, depends )
        self.flags   = flags
        if not self.targets[0].suffix == '.vst':
            raise TargetNotVst( 'Blif2Vst.__init__(): First target *must* "{}" be a vst file.' \
                                .format( self.targets[0] ))
        self.addClean( self.targets )

    def __repr__ ( self ):
        for d in self.file_dep:
            print( d )
        return '<blif2vst {} depends=[{}]>' \
               .format( self.design, ','.join([f.as_posix() for f in self.file_dep]) )

    @property
    def design ( self ):
        if len(self.targets): return self.targets[0].stem
        return None

    def doTask ( self ):
        from ..Hurricane  import Cell
        from ..           import CRL
        from ..           import Viewer
        from ..helpers.io import ErrorMessage
        from ..plugins    import rsave

        print( 'Blif2Vst.doTask() on "{}"'.format( self.design ))
        views = CRL.Catalog.State.Logical | self.flags
        cell  = CRL.Blif.load( self.file_depend().as_posix() )
        if cell.getName() == 'top':
            print( '  o  Renaming RTLIL anonymous top cell "top" into "{}".'.format(self.design) )
            cell.setName( self.design )
        renameNMigenUniquify( cell )
        CRL.restoreNetsDirection( cell, Cell.Flags_TerminalNetlist )
        kw          = {}
        kw['views'] = views
        kw['cell' ] = cell
        rsave.scriptMain( **kw )

        return self.checkTargets( 'Blif2Vst.doTask' )

    def create_doit_tasks ( self ):
        if self.design: doc = 'Run {}.'.format( self )
        else:           doc = 'Run plain CGT (no loaded design)'
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : doc
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }
        

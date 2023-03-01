
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask

class BadAliasDepend ( Exception ): pass


class Alias ( FlowTask ):
    """
    Create an alias for a rule. Takes only two arguments, the aliased name
    of the rule and the original rule itself.
    """

    @staticmethod
    def mkRule ( rule, depends=[] ):
        return Alias( rule, depends )

    def __init__ ( self, rule, depends ):
        if len(depends) != 1:
            raise BadAliasDepend( 'Alias.__init__(): There must be exactly *one* dependency ({})' \
                                  .format( depends ))
        if not isinstance(depends[0],FlowTask):
            raise BadAliasDepend( 'Alias.__init__(): The dependency must be another *rule* ({})' \
                                  .format( depends[0] ))
        super().__init__( rule, [], depends )

    def __repr__ ( self ):
        return '<Alias "{}" for "{}">'.format( self.basename, self.depends[0].basename )

    def doTask ( self ):
        return True

    def create_doit_tasks ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

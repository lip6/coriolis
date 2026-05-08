
from   pathlib import Path
from   doit.exceptions import TaskFailed
from   .task import FlowTask

class BadGroupDepend ( Exception ): pass


class Group ( FlowTask ):
    """
    Create an alias for a rule. Takes only two arguments, the aliased name
    of the rule and the original rule itself.
    """

    @staticmethod
    def mkRule ( rule, depends=[] ):
        return Group( rule, depends )

    def __init__ ( self, rule, depends ):
        for i in range(len(depends)):
            if not isinstance(depends[i],FlowTask):
                raise BadGroupDepend( f'Group.__init__(): The dependency [{i}] must be another *rule* ({depends[i]})' )
        super().__init__( rule, [], depends )

    def __repr__ ( self ):
        strDepends = self.depends[0].basename
        for depend in self.depends[1:]:
            strDepends = strDepends + ' ' + depend.basename
        return '<Group "{}" for "{}">'.format( self.basename, strDepends )

    def doTask ( self ):
        return True

    def asDoitTask ( self ):
        return { 'basename' : self.basename
               , 'actions'  : [ self.doTask ]
               , 'doc'      : 'Run {}.'.format( self )
               , 'targets'  : self.targets
               , 'file_dep' : self.file_dep
               }

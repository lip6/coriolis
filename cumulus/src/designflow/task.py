
import os
from   pathlib import Path
from   doit.exceptions import TaskFailed

class BadDependency  ( Exception ): pass
class DuplicatedRule ( Exception ): pass


class ShellEnv ( object ):
    """
    Manage and export to the sub-processes the legacy Alliance variables.

    Environment variables stored at class level, should be set once and
    for all at startup:

    * ``ALLIANCE_TOP``, usually identical to ``CORIOLIS_TOP``.
    * ``RDS_TECHNO_NAME``.
    * ``CHECK_TOOLKIT``, where the ``alliance-check-toolkit`` is installed.

    Mutable environment variables, could be changed in each instance.
    Their initial values are extracted from the Coriolis Alliance Framework.
    
    * ``MBK_CATA_LIB``.
    * ``MBK_TARGET_LIB``.
    * ``MBK_SCALE_X``.
    * ``MBK_IN_LO``.
    * ``MBK_OUT_LO``.
    * ``MBK_IN_PH``.
    * ``MBK_OUT_PH``.
    * ``MBK_CATAL_NAME``.
    * ``RDS_IN``.
    * ``RDS_OUT``.
    """

    ALLIANCE_TOP    = None
    RDS_TECHNO_NAME = None
    CHECK_TOOLKIT   = None

    def __init__ ( self ):
        self.shellEnv = {}
        self.capture()

    def __getitem__ ( self, key ):
        return self.shellEnv[ key ]

    def __setitem__ ( self, key, value ):
        self.shellEnv[ key ] = str( value )

    def capture ( self ):
        """
        Get the default values of the environment variables from the
        Coriolis configuration.
        """
        from .. import CRL

        af      = CRL.AllianceFramework.get()
        env     = af.getEnvironment()
        if len(af.getAllianceLibraries()) > 1:
            cataLib = [ lib.getPath() for lib in af.getAllianceLibraries()[1:] ]
            self.shellEnv[ 'MBK_CATA_LIB'   ] = ':'.join( cataLib )
            self.shellEnv[ 'MBK_TARGET_LIB' ] = cataLib[0]
        self.shellEnv[ 'MBK_SCALE_X'    ] = str( env.getSCALE_X() )
        self.shellEnv[ 'MBK_IN_LO'      ] = env.getIN_LO()
        self.shellEnv[ 'MBK_OUT_LO'     ] = env.getOUT_LO()
        self.shellEnv[ 'MBK_IN_PH'      ] = env.getIN_PH()
        self.shellEnv[ 'MBK_OUT_PH'     ] = env.getOUT_PH()
        self.shellEnv[ 'MBK_CATAL_NAME' ] = env.getCATALOG()
        self.shellEnv[ 'RDS_IN'         ] = 'gds'
        self.shellEnv[ 'RDS_OUT'        ] = 'gds'
        self.shellEnv[ 'ALLIANCE_TOP'   ] = ShellEnv.ALLIANCE_TOP

    def export ( self ):
        """
        Write back the variables into the environement for usage by the
        sub-processes.
        """
        for variable, value in self.shellEnv.items():
            if value is None: continue
            os.environ[ variable ] = value
        if ShellEnv.RDS_TECHNO_NAME is not None:
            os.environ[ 'RDS_TECHNO_NAME' ] = ShellEnv.RDS_TECHNO_NAME
        if ShellEnv.CHECK_TOOLKIT is not None:
            os.environ[ 'CHECK_TOOLKIT' ] = ShellEnv.CHECK_TOOLKIT
            

class FlowTask ( object ):
    """
    Add extra features over a doit task. This class *do not* provides the
    ``create_doit_tasks()`` method. It alows task to be chained directly
    between them instead of only through dependency/target files.

    1. Targets management: targets are always file name, stored as strings.

    2. Dependencies management: they can be plain files, pathlib.Path objects
       or other tasks. In the later case, the dependencies are the *targets*
       of said task, which sould be files, as stated on 1.

    3. Perform an early check for homonymous tasks.

    4. Keep track of all the targets of all the tasks to provide them for
       the special ``clean_flow`` task.
    """

    rules        = {}
    cleanTargets = []

    def __init__ ( self, basename, targets, depends ):
        """
        Promote ``targets`` and ``depends`` arguments to list if needed.
        Check for duplicated rules, then register the rule name at class level.
        """
        if FlowTask.hasRule(basename):
            raise DuplicatedRule( 'FlowTask.__init__(): Duplicated rule "{}"'.format(basename) )
        self.basename = basename
        if   depends is None:              self.depends = []
        elif not isinstance(depends,list): self.depends = [ depends ]
        else:                              self.depends =   depends
        if   targets is None:              self.targets = []
        elif not isinstance(targets,list): self.targets = [ targets ]
        else:                              self.targets =   targets
        FlowTask.rules[ self.basename ] = self

    @staticmethod
    def hasRule ( name ):
        if name in FlowTask.rules: return True
        return False

    @property
    def file_dep ( self ):
        """
        Build the list of dependencies to be passed on to doit (file_dep task dict).
        Convert back pathlib.Path object to string. If the dependency is another
        FlowTask, pass on it's own targets.
        """
        files = []
        for depend in self.depends:
            if isinstance(depend,str):
                files += [ depend ]
            elif isinstance(depend,Path):
                files += [ depend.as_posix() ]
            elif isinstance(depend,FlowTask):
                files += depend.targets
            else:
                raise BadDependency( 'FlowTask.file_dep(): Unsupported kind of dependency {}.'.format(depend) )
        return files

    def file_target ( self, tindex=0 ):
        """
        Access a target, by default, the first one.
        """
        if len(self.targets) > tindex:
            return self.targets[ tindex ]
        return None

    def file_depend ( self, dindex=0, tindex=0 ):
        """
        Access a dependency, by default the first one. It can be either a
        string or a pathlib.Path object. If the dependency itself is a
        FlowTask, then it's first *target* is returned. The dependency
        can be selected by a ``dindex``, and a ``tindex`` in case of a
        FlowTask.
        """
        if len(self.depends) > dindex:
            if isinstance(self.depends[dindex],FlowTask):
                return self.depends[ dindex ].file_target( tindex )
            return self.depends[ dindex ]
        return None

    def checkTargets ( self, methodName ):
        """
        Checks that all the the target files have been generated, stop on
        error. This is a helper for derived classes.
        """
        from ..helpers.io import ErrorMessage
        for target in self.targets:
            path = Path( target )
            if not path.is_file():
                e = ErrorMessage( 1, '{}(): The rule "{}" did *not* generate target "{}".' \
                                     .format( methodName, self.basename, target ))
                return TaskFailed( e )
        return True

    def addClean ( self, targets ):
        """
        Add the targets list to the global list. This is a helper method
        that has to be explicitely called in derived classes.
        """
        FlowTask.cleanTargets += targets
        
        

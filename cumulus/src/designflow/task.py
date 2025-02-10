
import os
from   pathlib import Path
from   doit.exceptions import TaskFailed

class BadDependency       ( Exception ): pass
class DuplicatedRule      ( Exception ): pass
class UnsupportedFileType ( Exception ): pass


class ShellEnv ( object ):
    """
    Manage and export to the sub-processes the legacy Alliance variables.

    Environment variables stored at class level, should be set once and
    for all at startup:

    * ``ALLIANCE_TOP``, usually identical to ``CORIOLIS_TOP``.
    * ``RDS_TECHNO_NAME``.
    * ``GRAAL_TECHNO_NAME``.
    * ``DREAL_TECHNO_NAME``.
    * ``CHECK_TOOLKIT``, where the ``alliance-check-toolkit`` is installed.
    * ``PDK_ROOT``, parent directory where all the PDKs should be storeds
    * ``PDK``, name of the selected PDK.
    * ``KLAYOUT_PATH``, technologies & scripts for Klayout.
    * ``KLAYOUT_HOME``, user's work directory for Klayout.

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
    * ``MBK_SPI_MODEL``.
    * ``RDS_IN``.
    * ``RDS_OUT``.
    """

    Show              = 0x0001
    CHECK_TOOLKIT     = None
    ALLIANCE_TOP      = None
    RDS_TECHNO_NAME   = None
    GRAAL_TECHNO_NAME = None
    DREAL_TECHNO_NAME = None
    PDK_ROOT          = None
    PDK               = None
    KLAYOUT_PATH      = None
    KLAYOUT_HOME      = None

    def __init__ ( self, name='unamed_env' ):
        self.name     = name
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
        self.shellEnv[ 'MBK_SPI_MODEL'  ] = 'MBK_SPI_MODEL_not_set'
        self.shellEnv[ 'RDS_IN'         ] = 'gds'
        self.shellEnv[ 'RDS_OUT'        ] = 'gds'
        if ShellEnv.ALLIANCE_TOP:
            self.shellEnv[ 'ALLIANCE_TOP'   ] = ShellEnv.ALLIANCE_TOP
            libPath         = ShellEnv.ALLIANCE_TOP + '/lib'
            LD_LIBRARY_PATH = ''
            if 'LD_LIBRARY_PATH' in os.environ:
                LD_LIBRARY_PATH = os.environ[ 'LD_LIBRARY_PATH' ]
            if LD_LIBRARY_PATH != '':
                libPath += ':' + LD_LIBRARY_PATH
            self.shellEnv[ 'LD_LIBRARY_PATH' ] = libPath

    def export ( self, flags=0 ):
        """
        Write back the variables into the environement for usage by the
        sub-processes.
        """
        if flags & ShellEnv.Show:
            print( 'ShellEnv:' )
            print( '  Name:', self.name )
            print( '  Variable:' )
        for variable, value in self.shellEnv.items():
            if value is None: continue
            os.environ[ variable ] = value
            if flags & ShellEnv.Show:
                print( '    {}: {}'.format( variable, value ))
        if ShellEnv.RDS_TECHNO_NAME is not None:
            os.environ[ 'RDS_TECHNO_NAME' ] = ShellEnv.RDS_TECHNO_NAME
        if ShellEnv.GRAAL_TECHNO_NAME is not None:
            os.environ[ 'GRAAL_TECHNO_NAME' ] = ShellEnv.GRAAL_TECHNO_NAME
        if ShellEnv.DREAL_TECHNO_NAME is not None:
            os.environ[ 'DREAL_TECHNO_NAME' ] = ShellEnv.DREAL_TECHNO_NAME
        if ShellEnv.CHECK_TOOLKIT is not None:
            os.environ[ 'CHECK_TOOLKIT' ] = ShellEnv.CHECK_TOOLKIT
        if ShellEnv.PDK_ROOT is not None:
            os.environ[ 'PDK_ROOT' ] = ShellEnv.PDK_ROOT
        if ShellEnv.PDK is not None:
            os.environ[ 'PDK' ] = ShellEnv.PDK
        if ShellEnv.KLAYOUT_PATH is not None:
            os.environ[ 'KLAYOUT_PATH' ] = ShellEnv.KLAYOUT_PATH
        if ShellEnv.KLAYOUT_HOME is not None:
            os.environ[ 'KLAYOUT_HOME' ] = ShellEnv.KLAYOUT_HOME
            

class Tasks ( object ):
    """
    Gather all the FlowTask to execute and provides them to doit through
    the ``create_doit_tasks()`` class method.

    In order for the tasks to be taken into account by doit, this class
    *must* be imported into the ``dodo.py``:

    .. code:: python

              from coriolis.designflow.task import Tasks
    """

    tasks = []

    @staticmethod
    def hasRule ( name ):
        for rule in Tasks.tasks:
            if name == rule.basename: return True
        return False

    @staticmethod
    def append ( task ):
        Tasks.tasks.append( task )

    @classmethod
    def create_doit_tasks ( selfClass ):
        """
        Return the recorded tasks one by one.
        """
        for task in selfClass.tasks:
            yield task.asDoitTask()
            

class FlowTask ( object ):
    """
    Add extra features over a doit task. This class *do not* provides the
    ``create_doit_tasks()`` method. It alows task to be chained directly
    between them instead of only through dependency/target files.

    1. Targets management: targets can be passed as plain files (string)
       or pathlib.Path, but are all internally converted into Path.

    2. Dependencies management: they can be plain files, pathlib.Path objects
       or other tasks. In the later case, the dependencies are the *targets*
       of said task, which sould be pathlib.Path, as stated on 1.
       plain files are converted into pathlib.Path.

    3. Perform an early check for homonymous tasks.

    4. Keep track of all the targets of all the tasks to provide them for
       the special ``clean_flow`` task.
    """

    cleanTargets = []

    @staticmethod
    def _normFile ( depend ):
        if isinstance(depend,FlowTask) or isinstance(depend,Path): return depend
        if isinstance(depend,str): return Path(depend)
        raise UnsupportedFileType( 'FlowTask._normFile(): Unsupported type for "{}"'.format(depend) )

    @staticmethod
    def _normFileList ( depends ):
        if not depends: return [];
        if isinstance(depends,list):
            return [ FlowTask._normFile(d) for d in depends ]
        return [ FlowTask._normFile(depends) ]

    def __init__ ( self, basename, targets, depends ):
        """
        Promote ``targets`` and ``depends`` arguments to list if needed.
        Check for duplicated rules, then register the rule name at class level.
        """
        if Tasks.hasRule(basename):
            raise DuplicatedRule( 'FlowTask.__init__(): Duplicated rule "{}"'.format(basename) )
        self.basename = basename
        self.depends  = FlowTask._normFileList( depends )
        self.targets  = FlowTask._normFileList( targets )
        Tasks.append( self )

    @staticmethod
    def hasRule ( name ):
        return Tasks.hasRule( name )

    @property
    def file_dep ( self ):
        """
        Build the list of dependencies to be passed on to doit (file_dep task dict).
        If the dependency is another FlowTask, pass on it's own targets.
        All files are pathlib.Path.
        """
        files = []
        for depend in self.depends:
            if isinstance(depend,FlowTask): files += depend.targets
            else:                           files += [ depend ]
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
            if not target.is_file():
                e = ErrorMessage( 1, '{}(): The rule "{}" did *not* generate target "{}".' \
                                     .format( methodName, self.basename, target ))
                return TaskFailed( e )
        return True

    def addClean ( self, targets ):
        """
        Add the targets list to the global list. This is a helper method
        that has to be explicitely called in derived classes.
        """
        FlowTask.cleanTargets += FlowTask._normFileList( targets )
        
        

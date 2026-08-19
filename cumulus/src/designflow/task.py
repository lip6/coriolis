
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
    MBK_SPI_MODEL     = 'MBK_SPI_MODEL_not_set'
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
        self.shellEnv[ 'RDS_IN'         ] = 'gds'
        self.shellEnv[ 'RDS_OUT'        ] = 'gds'
        if ShellEnv.ALLIANCE_TOP:
            self.shellEnv[ 'ALLIANCE_TOP' ] = ShellEnv.ALLIANCE_TOP
            libPath = ShellEnv.ALLIANCE_TOP + '/lib'
            LD_LIBRARY_PATH = ''
            if 'LD_LIBRARY_PATH' in os.environ:
                LD_LIBRARY_PATH = os.environ[ 'LD_LIBRARY_PATH' ]
            if LD_LIBRARY_PATH != '' and LD_LIBRARY_PATH.find( libPath ) < 0:
                libPath += ':' + LD_LIBRARY_PATH
            self.shellEnv[ 'LD_LIBRARY_PATH' ] = libPath

    def export ( self, flags=0 ):
        """
        Write back the variables into the environement for usage by the
        sub-processes.
        """
        def _setEnviron ( variable, value, flags ):
            if value is None: return
            if isinstance( value, Path ):
                value = value.as_posix()
            os.environ[ variable ] = value
            if flags & ShellEnv.Show:
                print( '    {}: {}'.format( variable, value ))

        if flags & ShellEnv.Show:
            print( 'ShellEnv:' )
            print( '  Name:', self.name )
            print( '  Variable:' )
        for variable, value in self.shellEnv.items():
            _setEnviron( variable, value, flags )
        _setEnviron( 'MBK_SPI_MODEL'    , ShellEnv.MBK_SPI_MODEL    , flags )
        _setEnviron( 'RDS_TECHNO_NAME'  , ShellEnv.RDS_TECHNO_NAME  , flags )
        _setEnviron( 'GRAAL_TECHNO_NAME', ShellEnv.GRAAL_TECHNO_NAME, flags )
        _setEnviron( 'DREAL_TECHNO_NAME', ShellEnv.DREAL_TECHNO_NAME, flags )
        _setEnviron( 'CHECK_TOOLKIT'    , ShellEnv.CHECK_TOOLKIT    , flags )
        _setEnviron( 'PDK_ROOT'         , ShellEnv.PDK_ROOT         , flags )
        _setEnviron( 'PDK'              , ShellEnv.PDK              , flags )
        _setEnviron( 'KLAYOUT_PATH'     , ShellEnv.KLAYOUT_PATH     , flags )
        _setEnviron( 'KLAYOUT_HOME'     , ShellEnv.KLAYOUT_HOME     , flags )


class EdaVendorsShellEnv ( object ):
    """
    Manage and export to the sub-processes the EDA vendors variables.

    Environment variables that are common to all vendor tools are stored
    are stored as class attributes and should be set once and for all at
    startup:

    .. code:: python

       edaVendorEnv = EdaVendorsShellEnv()
       edaVendorEnv.CDSDIR = '/root/of/cadence/IC/installation'

    Then, environment variables that are specific to a PDK should be set
    as class *instance* attributes, using the supplied dictionnary-like
    access.

    .. code:: python

       edaVendorEnv = EdaVendorsShellEnv()
       edaVendorEnv[ 'PDK_ROOT' ] = '/root/of/foundry/pdk'

    List of supported common variables:

    * ``CDSDIR``, Cadence IC root directory. Also exported as ``CDS_ROOT``
      and ``CDS_INST_DIR``
    """

    Show                 = 0x0001
    _LC_ALL              = "C"
    _W3264_NO_HOST_CHECK = "1"
    _DD_DONT_DO_OS_LOCKS = "set"
    _CDS_AUTO_64BIT      = "ALL"
    _CDS_Netlisting_Mode = "Analog"
    _CDSDIR              = None
    _CDS_MMSIM_DIR       = None
    _ASSURAHOME          = None
    _PVSHOME             = None
    _QRC_HOME            = None
    _CDS_IUS_DIR         = None
    _CDS_SYNTH_ROOT      = None
    _INNOVUS             = None
    _CDS_LICENSE         = None
    _SIEMENS_LICENSE     = None
    _MGC_HOME            = None
    _MGC_AMS_HOME        = None
    _USE_CALIBRE_VCO     = "aok"
    _opusver             = "618"
    _amsdbtype           = "OpenAccess"
    _PATH                = []
    _LD_LIBRARY_PATH     = []

    def __init__ ( self, name='unamed_env' ):
        self.name     = name
        self.localEnv = {}
        self.capture()

    def capture ( self ):
        """
        Get the default values of the PATH & LD_LIBRARY_PATH environment variables
        """
        self.systemPATH            = os.environ[ 'PATH'            ].split( ':' )
        self.systemLD_LIBRARY_PATH = os.environ[ 'LD_LIBRARY_PATH' ].split( ':' )

    def __setattr__ ( self, attr, value ):
        if attr == 'MGC_HOME':
            EdaVendorsShellEnv._MGC_HOME = value
            EdaVendorsShellEnv._PATH.append( f'{value}/bin' )
        if attr == 'MGC_AMS_HOME':
            EdaVendorsShellEnv._MGC_AMS_HOME = value
            EdaVendorsShellEnv._PATH.append( f'{value}/bin' )
        if attr == 'CDSDIR':
            EdaVendorsShellEnv._CDSDIR = value
            EdaVendorsShellEnv._PATH.append( f'{value}/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/dfII/bin' )
            EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'CDS_MMSIM_DIR':
            EdaVendorsShellEnv._CDS_MMSIM_DIR = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools.lnx86/bin' )
            EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'CDS_IUS_DIR':
            EdaVendorsShellEnv._CDS_IUS_DIR = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/dfII/bin' )
           #EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'CDS_SYNTH_ROOT':
            EdaVendorsShellEnv._SYNTH_ROOT = value
            EdaVendorsShellEnv._PATH.append( f'{value}/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'ASSURAHOME':
            EdaVendorsShellEnv._ASSURAHOME = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/assura/bin' )
           #EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'PVSHOME':
            EdaVendorsShellEnv._PVSHOME = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/dfII/bin' )
           #EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'QRC_HOME':
            EdaVendorsShellEnv._QRC_HOME = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/dfII/bin' )
           #EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib' )
        if attr == 'INNOVUS':
            EdaVendorsShellEnv._INNOVUS = value
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._PATH.append( f'{value}/tools/bin' )
            EdaVendorsShellEnv._LD_LIBRARY_PATH.append( f'{value}/tools/lib/64bits' )
        elif attr == 'USE_CALIBRE_VCO':     EdaVendorsShellEnv._USE_CALIBRE_VCO     = value
        elif attr == 'opusver':             EdaVendorsShellEnv._opusver             = value
        elif attr == 'amsdbtype':           EdaVendorsShellEnv._amsdbtype           = value
        elif attr == 'SIEMENS_LICENSE':     EdaVendorsShellEnv._SIEMENS_LICENSE     = value
        elif attr == 'CDS_LICENSE':         EdaVendorsShellEnv._CDS_LICENSE         = value
        elif attr == 'CDS_Netlisting_Mode': EdaVendorsShellEnv._CDS_Netlisting_Mode = value
        else:
            object.__setattr__( self, attr, value )

    def __getitem__ ( self, key ):
        return self.localEnv[ key ]

    def __setitem__ ( self, key, value ):
        self.localEnv[ key ] = str( value )

    def export ( self, flags=0 ):
        """
        Write back the variables into the environement for usage by the
        sub-processes.
        """
        def _setEnviron ( variable, value, flags ):
            if value is None: return
            if isinstance( value, Path ):
                stringValue = value.as_posix()
            elif isinstance( value, list ):
                stringValue = ':'.join( value )
            else:
                stringValue = value
            os.environ[ variable ] = stringValue
            if flags & EdaVendorsShellEnv.Show:
                if isinstance( value, list ):
                    print( '    {}:'.format( variable ))
                    for component in value:
                        print( '      "{}"'.format( component ))
                else:
                    print( '    {}: "{}"'.format( variable, stringValue ))

        if flags & EdaVendorsShellEnv.Show:
            print( 'EdaVendorsShellEnv:' )
            print( '  Name:', self.name )
            print( '  Variable:' )
        LICENSE = ''
        for license in (EdaVendorsShellEnv._CDS_LICENSE
                       ,EdaVendorsShellEnv._SIEMENS_LICENSE):
            if license is None: continue
            colon = ''
            if LICENSE != '': colon = ':'
            LICENSE = f'{LICENSE}{colon}{license}'
        _setEnviron( 'LM_LICENSE_FILE'    , LICENSE                               , flags )
        _setEnviron( 'LC_ALL'             , EdaVendorsShellEnv._LC_ALL             , flags )
        _setEnviron( 'W3264_NO_HOST_CHECK', EdaVendorsShellEnv._W3264_NO_HOST_CHECK, flags )
        _setEnviron( 'DD_DONT_DO_OS_LOCKS', EdaVendorsShellEnv._DD_DONT_DO_OS_LOCKS, flags )
        _setEnviron( 'CDS_AUTO_64BIT'     , EdaVendorsShellEnv._CDS_AUTO_64BIT     , flags )
        _setEnviron( 'CDS_Netlisting_Mode', EdaVendorsShellEnv._CDS_Netlisting_Mode, flags )
        _setEnviron( 'CDSDIR'             , EdaVendorsShellEnv._CDSDIR             , flags )
        _setEnviron( 'CDS_ROOT'           , EdaVendorsShellEnv._CDSDIR             , flags )
        _setEnviron( 'CDS_INST_DIR'       , EdaVendorsShellEnv._CDSDIR             , flags )
        _setEnviron( 'CDS_MMSIM_DIR'      , EdaVendorsShellEnv._CDS_MMSIM_DIR      , flags )
        _setEnviron( 'CDS_SYNTH_ROOT'     , EdaVendorsShellEnv._CDS_SYNTH_ROOT     , flags )
        _setEnviron( 'MMSIMHOME'          , EdaVendorsShellEnv._CDS_MMSIM_DIR      , flags )
        _setEnviron( 'SPECTRE_ROOT'       , EdaVendorsShellEnv._CDS_MMSIM_DIR      , flags )
        _setEnviron( 'ASSURAHOME'         , EdaVendorsShellEnv._ASSURAHOME         , flags )
        _setEnviron( 'PVSHOME'            , EdaVendorsShellEnv._PVSHOME            , flags )
        _setEnviron( 'QRC_HOME'           , EdaVendorsShellEnv._QRC_HOME           , flags )
        _setEnviron( 'CDS_IUS_DIR'        , EdaVendorsShellEnv._CDS_IUS_DIR        , flags )
        _setEnviron( 'IUSDIR'             , EdaVendorsShellEnv._CDS_IUS_DIR        , flags )
        _setEnviron( 'LDVDIR'             , EdaVendorsShellEnv._CDS_IUS_DIR        , flags )
        _setEnviron( 'INNOVUS'            , EdaVendorsShellEnv._INNOVUS            , flags )
        _setEnviron( 'MGC_HOME'           , EdaVendorsShellEnv._MGC_HOME           , flags )
        _setEnviron( 'CALIBRE_HOME'       , EdaVendorsShellEnv._MGC_HOME           , flags )
        _setEnviron( 'USE_CALIBRE_VCO'    , EdaVendorsShellEnv._USE_CALIBRE_VCO    , flags )
        _setEnviron( 'MGC_AMS_HOME'       , EdaVendorsShellEnv._MGC_AMS_HOME       , flags )
        _setEnviron( 'opusver'            , EdaVendorsShellEnv._opusver            , flags )
        _setEnviron( 'amsdbtype'          , EdaVendorsShellEnv._amsdbtype          , flags )

        if len(self.systemPATH):
            PATH = EdaVendorsShellEnv._PATH + self.systemPATH
        _setEnviron( 'PATH', PATH, flags )

        if len(self.systemLD_LIBRARY_PATH):
            LD_LIBRARY_PATH = EdaVendorsShellEnv._LD_LIBRARY_PATH + self.systemLD_LIBRARY_PATH
        _setEnviron( 'LD_LIBRARY_PATH', LD_LIBRARY_PATH, flags )

        for variable, value in self.localEnv.items():
            _setEnviron( variable, value, flags )
            

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
    cleanGlobs   = []

    @staticmethod
    def _normFile ( depend ):
        if isinstance(depend,FlowTask) or isinstance(depend,Path): return depend
        if isinstance(depend,str): return Path(depend)
        raise UnsupportedFileType( 'FlowTask._normFile(): Unsupported type for target or dependency for "{}"'.format(depend) )

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
        Add the targets to be deleted to the global list. This is a helper method
        that has to be explicitely called in derived classes.
        """
        FlowTask.cleanTargets += FlowTask._normFileList( targets )

    def addCleanGlob ( self, directory, glob ):
        """
        Add pattern of files or directories to be deleted. This is a helper method
        that has to be explicitely called in derived classes.
        """
        FlowTask.cleanGlobs.append(( FlowTask._normFile(directory), glob ))
        
        

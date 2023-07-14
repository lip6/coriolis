
import sys
import os
import socket
from   pathlib import Path
from   .task   import ShellEnv


__all__ = [ 'Where', 'setupCMOS', 'setupCMOS45' ]


class Where ( object ):

    coriolisTop  = None
    allianceTop  = None
    cellsTop     = None
    checkToolkit = None

    def __init__ ( self, checkToolkit=None ):
        if 'CORIOLIS_TOP' in os.environ: Where.coriolisTop = Path( os.environ['CORIOLIS_TOP'] )
        if 'ALLIANCE_TOP' in os.environ: Where.allianceTop = Path( os.environ['ALLIANCE_TOP'] )
        if 'CELLS_TOP'    in os.environ: Where.cellsTop    = Path( os.environ['CELLS_TOP'] )
        if Where.coriolisTop and not Where.allianceTop: Where.allianceTop = Where.coriolisTop
        #print( Where.coriolisTop, Where.allianceTop )
        if not Where.coriolisTop:
            print( 'technos.Where.__init__(): Unable to locate Coriolis top.' )
        if checkToolkit is None:
            checkToolkit = Path.home() / 'coriolis-2.x' / 'src' / 'alliance-check-toolkit'
        else:
            if isinstance(checkToolkit,str):
                checkToolkit = Path( checkToolkit )
            if not Where.cellsTop:
                Where.cellsTop = checkToolkit / 'cells'
        Where.checkToolkit = checkToolkit
        if not Where.cellsTop and Where.allianceTop:
            Where.cellsTop = Where.allianceTop / 'cells'
        ShellEnv.ALLIANCE_TOP = Where.allianceTop.as_posix()

    def __repr__ ( self ):
        if not Where.coriolisTop:
            return '<Where coriolisTop=NOT_FOUND>'
        return '<Where coriolisTop="{}">'.format( Where.coriolisTop.as_posix() )


def setupCMOS ( checkToolkit=None ):
    Where( checkToolkit )
    ShellEnv().export()

    from ..         import Cfg 
    from ..         import Viewer
    from ..         import CRL 
    from ..helpers  import overlay, l, u, n
    from .yosys     import Yosys
    import coriolis.technos.symbolic.cmos
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore              = False
        cfg.misc.info                   = False
        cfg.misc.paranoid               = False
        cfg.misc.bug                    = False
        cfg.misc.logMode                = True
        cfg.misc.verboseLevel1          = True
        cfg.misc.verboseLevel2          = True
        cfg.misc.minTraceLevel          = 1900
        cfg.misc.maxTraceLevel          = 3000
        cfg.katana.eventsLimit          = 1000000
        cfg.katana.termSatReservedLocal = 6 
        cfg.katana.termSatThreshold     = 9 
        Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )
        af  = CRL.AllianceFramework.get()
        env = af.getEnvironment()
        env.setCLOCK( '^ck$|m_clock|^clk$' )

    Yosys.setLiberty( Where.cellsTop / 'sxlib' / 'sxlib.lib' )
    ShellEnv.RDS_TECHNO_NAME = (Where.allianceTop / 'etc' / 'cmos.rds').as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + (Where.allianceTop / 'bin').as_posix()
            break


def setupLCMOS ( checkToolkit=None ):
    Where( checkToolkit )
    ShellEnv().export()

    from ..         import Cfg 
    from ..         import Viewer
    from ..         import CRL 
    from ..helpers  import overlay, l, u, n
    from .yosys     import Yosys
    import coriolis.technos.symbolic.lcmos
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore              = False
        cfg.misc.info                   = False
        cfg.misc.paranoid               = False
        cfg.misc.bug                    = False
        cfg.misc.logMode                = True
        cfg.misc.verboseLevel1          = True
        cfg.misc.verboseLevel2          = True
        cfg.misc.minTraceLevel          = 1900
        cfg.misc.maxTraceLevel          = 3000
        cfg.katana.eventsLimit          = 1000000
        cfg.katana.termSatReservedLocal = 6 
        cfg.katana.termSatThreshold     = 9 
        Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )
        af  = CRL.AllianceFramework.get()
        env = af.getEnvironment()
        env.setCLOCK( '^ck$|m_clock|^clk$' )

    Yosys.setLiberty( Where.checkToolkit / 'cells' / 'lsxlib' / 'lsxlib.lib' )
    ShellEnv.RDS_TECHNO_NAME = (Where.allianceTop / 'etc' / 'cmos.rds').as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + (Where.allianceTop / 'bin').as_posix()
            break


def setupCMOS45 ( useNsxlib=False, checkToolkit=None, cellsTop=None ):
    from   ..        import Cfg 
    from   ..        import Viewer
    from   ..        import CRL 
    from   ..helpers import overlay, l, u, n
    from   .yosys    import Yosys
    import coriolis.technos.symbolic.cmos45

    Where( checkToolkit )
    if cellsTop is None:
        cellsTop = Where.cellsTop
    else:
        if isinstance(cellsTop,str):
            cellsTop = Path( cellsTop )
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore               = False
        cfg.misc.info                    = False
        cfg.misc.paranoid                = False
        cfg.misc.bug                     = False
        cfg.misc.logMode                 = True
        cfg.misc.verboseLevel1           = True
        cfg.misc.verboseLevel2           = True
        cfg.misc.minTraceLevel           = 1900
        cfg.misc.maxTraceLevel           = 3000
        cfg.katana.eventsLimit           = 1000000
        cfg.etesian.graphics             = 3
        cfg.etesian.spaceMargin          = 0.05
        cfg.etesian.aspectRatio          = 1.0 
        cfg.anabatic.edgeLenght          = 24
        cfg.anabatic.edgeWidth           = 8
        if useNsxlib:
            cfg.anabatic.routingGauge    = 'msxlib4'
            cfg.anabatic.topRoutingLayer = 'METAL4'
        cfg.katana.termSatReservedLocal  = 6 
        cfg.katana.termSatThreshold      = 9 

        Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )
        af  = CRL.AllianceFramework.get()
        env = af.getEnvironment()
        env.setCLOCK( '^ck$|m_clock|^clk$' )

        sxlib   = cellsTop / 'nsxlib'
        iolib   = cellsTop / 'niolib'
        liberty = sxlib    / 'nsxlib.lib'
        env.addSYSTEM_LIBRARY( library=iolib.as_posix(), mode=CRL.Environment.Prepend )
        env.addSYSTEM_LIBRARY( library=sxlib.as_posix(), mode=CRL.Environment.Prepend )
        if not sxlib.is_dir():
            print( '[ERROR] technos.setupCMOS45(): sxlib directory do *not* exists:' )
            print( '        "{}"'.format(sxlib.as_posix()) )

    Yosys.setLiberty( liberty )
    ShellEnv.RDS_TECHNO_NAME = (Where.checkToolkit / 'etc' / 'FreePDK45.rds').as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + (Where.allianceTop / 'bin').as_posix()
            break


def setupSky130_c4m ( checkToolkit=None, pdkMasterTop=None ):
    from ..        import Cfg 
    from ..        import Viewer
    from ..        import CRL 
    from ..helpers import setNdaTopDir, overlay, l, u, n
    from .yosys    import Yosys

    if isinstance(pdkMasterTop,str):
        pdkMasterTop = Path( pdkMasterTop )
    ndaDirectory = None
    if pdkMasterTop:
        ndaDirectory = pdkMasterTop / 'libs.tech' / 'coriolis' / 'techno'
    elif not ndaDirectory:
        hostname = socket.gethostname()
        if hostname.startswith('lepka'):
            ndaDirectory = Path( '/dsk/l1/jpc/crypted/soc/techno' )
            if not ndaDirectory.is_dir():
                print ('[ERROR] You forgot to mount the NDA encrypted directory, stupid!')
        else:
            ndaDirectory = Path( '/users/soft/techno/techno' )
        pdkMasterTop = ndaDirectory
    setNdaTopDir( ndaDirectory.as_posix() )
    if not pdkMasterTop.is_dir():
        print( '[ERROR] technos.setupSky130_c4m(): pdkMasterTop directory do *not* exists:' )
        print( '        "{}"'.format(pdkMasterTop.as_posix()) )

    Where( checkToolkit )

    from node130.sky130 import techno, StdCellLib #, LibreSOCIO
    techno.setup()
    StdCellLib.setup()
    #LibreSOCIO.setup()

    cellsTop = pdkMasterTop / 'libs.ref'
    liberty  = cellsTop / 'StdCellLib' / 'liberty' / 'StdCellLib_nom.lib'
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore           = False
        cfg.misc.minTraceLevel       = 12300
        cfg.misc.maxTraceLevel       = 12400
        cfg.misc.info                = False
        cfg.misc.paranoid            = False
        cfg.misc.bug                 = False
        cfg.misc.logMode             = True
        cfg.misc.verboseLevel1       = False
        cfg.misc.verboseLevel2       = False
        cfg.viewer.pixelThreshold    = 5
        cfg.etesian.graphics         = 2
        cfg.anabatic.topRoutingLayer = 'm4'
        cfg.katana.eventsLimit       = 4000000
        af  = CRL.AllianceFramework.get()
        lg5 = af.getRoutingGauge( 'StdCellLib' ).getLayerGauge( 5 ) 
        lg5.setType( CRL.RoutingLayerGauge.PowerSupply )
        env = af.getEnvironment()
        env.setCLOCK( '^sys_clk$|^ck|^jtag_tck$' )

    Yosys.setLiberty( liberty )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()


def setupFreePDK45_c4m ( checkToolkit=None, pdkMasterTop=None ):
    from ..        import Cfg 
    from ..        import Viewer
    from ..        import CRL 
    from ..helpers import setNdaTopDir, overlay, l, u, n
    from .yosys    import Yosys

    if isinstance(pdkMasterTop,str):
        pdkMasterTop = Path( pdkMasterTop )
    if not pdkMasterTop.is_dir():
        print( '[ERROR] technos.setupFreePDK45_c4m(): pdkMasterTop directory do *not* exists:' )
        print( '        "{}"'.format(pdkMasterTop.as_posix()) )
    sys.path.append( (pdkMasterTop / 'coriolis' / 'techno' / 'etc' / 'coriolis2').resolve().as_posix() )

    Where( checkToolkit )

    from NDA.node45.freepdk45_c4m import techno, FlexLib, LibreSOCIO
    techno.setup()
    FlexLib.setup()
    LibreSOCIO.setup()

    liberty  = pdkMasterTop / 'views' / 'FreePDK45' / 'FlexLib' / 'liberty' / 'FlexLib_nom.lib'
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore           = False
        cfg.misc.minTraceLevel       = 12300
        cfg.misc.maxTraceLevel       = 12400
        cfg.misc.info                = False
        cfg.misc.paranoid            = False
        cfg.misc.bug                 = False
        cfg.misc.logMode             = True
        cfg.misc.verboseLevel1       = True
        cfg.misc.verboseLevel2       = True
        cfg.etesian.graphics         = 3
        cfg.etesian.spaceMargin      = 0.10
        cfg.anabatic.topRoutingLayer = 'metal6'
        cfg.katana.eventsLimit       = 4000000
        af  = CRL.AllianceFramework.get()
        lg5 = af.getRoutingGauge('FlexLib').getLayerGauge( 5 )
        lg5.setType( CRL.RoutingLayerGauge.PowerSupply )
        env = af.getEnvironment()
        env.setCLOCK( '^sys_clk$|^ck|^jtag_tck$' )

    Yosys.setLiberty( liberty )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()


def setupTSMC_c180_c4m ( checkToolkit=None, ndaTop=None ):
    from ..        import Cfg 
    from ..        import Viewer
    from ..        import CRL 
    from ..helpers import setNdaTopDir, overlay, l, u, n
    from .yosys    import Yosys

    ndaDirectory = None
    if ndaTop is not None:
        if not isinstance(ndaTop,Path):
            ndaDirectory = ndaTop
        else:
            ndaDirectory = Path( ndaTop )
    if not ndaDirectory:
        hostname = socket.gethostname()
        if hostname.startswith('lepka'):
            ndaDirectory = Path( '/dsk/l1/jpc/crypted/soc/techno' )
            if not ndaDirectory.is_dir():
                print( '[ERROR] You forgot to mount the NDA encrypted directory, stupid!' )
        else:
            ndaDirectory = '/users/soft/techno/techno'
    setNdaTopDir( ndaDirectory )

    Where( checkToolkit )

    from NDA.node180.tsmc_c018 import techno, FlexLib, LibreSOCIO, LibreSOCMem #, pll
    techno.setup()
    FlexLib.setup()
    LibreSOCIO.setup()
    LibreSOCMem.setup()
    #pll.setup()

    liberty  = ndaDirectory / 'etc' / 'coriolis2' / 'NDA' / 'node180' / 'tsmc_c018' / 'FlexLib.lib'

    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore           = False
        cfg.misc.minTraceLevel       = 10100
        cfg.misc.maxTraceLevel       = 10200
        cfg.misc.info                = False
        cfg.misc.paranoid            = False
        cfg.misc.bug                 = False
        cfg.misc.logMode             = True
        cfg.misc.verboseLevel1       = True
        cfg.misc.verboseLevel2       = True
        cfg.etesian.graphics         = 3
        cfg.etesian.densityVariation = 0.04
        cfg.etesian.spaceMargin      = 0.04
        cfg.katana.eventsLimit       = 4000000
        af  = CRL.AllianceFramework.get()
        env = af.getEnvironment()
        env.setCLOCK( '^clk|^reset' )

    Yosys.setLiberty( liberty )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()


def setupGF180MCU_GF ( checkToolkit=None, pdkTop=None ):
    from ..        import Cfg 
    from ..        import Viewer
    from ..        import CRL 
    from ..helpers import setNdaTopDir, overlay, l, u, n
    from .yosys    import Yosys

    if isinstance(pdkTop,str):
        pdkTop = Path( pdkTop )
    if not pdkTop:
        print( '[ERROR] technos.setupGF180MCU_GF(): pdkTop directory has *not* been set.' )
    if not pdkTop.is_dir():
        print( '[ERROR] technos.setupSky130_c4m(): pdkTop directory do *not* exists:' )
        print( '        "{}"'.format(pdkTop.as_posix()) )

    Where( checkToolkit )

    cellsTop = pdkTop / 'libraries' / 'gf180mcu_fd_sc_mcu9t5v0' / 'latest' / 'cells'
   #liberty  = pdkTop / 'libraries' / 'gf180mcu_fd_sc_mcu9t5v0' / 'latest' / 'liberty' / 'gf180mcu_fd_sc_mcu9t5v0__tt_025C_5v00.lib'
    liberty  = pdkTop / 'FULL.lib'

    from coriolis.technos.node180.gf180mcu import techno
    from coriolis.technos.node180.gf180mcu import mcu9t5v0
    techno.setup()
    mcu9t5v0.setup( cellsTop )
    
    with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.catchCore           = False
        cfg.misc.minTraceLevel       = 12300
        cfg.misc.maxTraceLevel       = 12400
        cfg.misc.info                = False
        cfg.misc.paranoid            = False
        cfg.misc.bug                 = False
        cfg.misc.logMode             = True
        cfg.misc.verboseLevel1       = False
        cfg.misc.verboseLevel2       = False
        cfg.etesian.graphics         = 2
        cfg.anabatic.topRoutingLayer = 'm4'
        cfg.katana.eventsLimit       = 4000000
        af  = CRL.AllianceFramework.get()
       #lg5 = af.getRoutingGauge( 'mcu9t' ).getLayerGauge( 5 ) 
       #lg5.setType( CRL.RoutingLayerGauge.PowerSupply )
        env = af.getEnvironment()
        env.setCLOCK( '^sys_clk$|^ck|^jtag_tck$' )

    Yosys.setLiberty( liberty )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()

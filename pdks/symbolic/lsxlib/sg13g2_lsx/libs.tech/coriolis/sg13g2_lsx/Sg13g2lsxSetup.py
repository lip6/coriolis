
import sys
import os
import socket
from   pathlib import Path
from   coriolis.designflow.task   import ShellEnv


__all__ = [ 'Where', 'setupSg13g2_lsx ' ]


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


def setupSg13g2_lsx ( checkToolkit=None ):
    Where( checkToolkit )
    ShellEnv().export()

    pdkDir          = Where.checkToolkit / 'dks' / 'sg13g2_lsx' / 'libs.tech'
    coriolisTechDir = pdkDir / 'coriolis'
    if not pdkDir.is_dir():
        print( '[ERROR] technos.setupSg13g2_lsx(): PDK directory do *not* exists:' )
        print( '        "{}"'.format(techDir.as_posix()) )
    sys.path.append( coriolisTechDir.as_posix() )

    cellsTop  = Where.checkToolkit / 'cells'
    liberty   = cellsTop / 'lsxlib' / 'lsxlib.lib'
#    kdrcRules = pdkDir / 'klayout' / 'drc_sky130.lydrc'

    from coriolis          import Cfg 
    from coriolis          import Viewer
    from coriolis          import CRL 
    from coriolis.helpers   import overlay, l, u, n
    from coriolis.designflow.yosys      import Yosys
    from coriolis.designflow.klayout    import DRC
    from sg13g2_lsx import techno, lsxlib
    techno.setup( coriolisTechDir )
    lsxlib.setup( cellsTop )
    
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

    Yosys.setLiberty( liberty )
#    DRC.setDrcRules( kdrcRules )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + (Where.allianceTop / 'bin').as_posix()
            break



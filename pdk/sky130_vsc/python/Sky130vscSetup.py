import sys
import os
import socket
from   pathlib import Path
from   coriolis.designflow.task   import ShellEnv
from   coriolis.designflow.yosys      import Yosys
from   coriolis.designflow.klayout    import DRC
from   coriolis.designflow.technos    import Where


__all__ = [ 'setupSky130_vsc' ]


def setupSky130_vsc ( checkToolkit=None ):
    Where( checkToolkit )
    ShellEnv().export()

    pdkDir          = Where.coriolisTop / 'pdk'
    coriolisTechDir = pdkDir / 'sky130_vsc/libs.tech'
    if not pdkDir.is_dir():
        print( '[ERROR] technos.setupSky130_vsc(): PDK directory do *not* exists:' )
        print( '        "{}"'.format(pdkDir.as_posix()) )
    sys.path.append( coriolisTechDir.as_posix() )

    cellsTop  = Path('coiolis/cells')
    liberty   = cellsTop / 'vsclib/vsclib.lib'
    #kdrcRules = pdkDir / 'klayout' / 'drc_sky130.lydrc'

    from coriolis           import Cfg 
    from coriolis           import Viewer
    from coriolis           import CRL 
    from coriolis.helpers   import overlay, l, u, n
    from python         import techno, vsclib
    
    techno.setup( coriolisTechDir )
    vsclib.setup( cellsTop )
    
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
    #DRC.setDrcRules( kdrcRules )
    ShellEnv.CHECK_TOOLKIT = Where.checkToolkit.as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + (Where.allianceTop / 'bin').as_posix()
            break



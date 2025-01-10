import os

# Coriolis imports
from   coriolis.designflow.technos      import Where
from   coriolis.designflow.task         import ShellEnv
from   coriolis.designflow.yosys        import Yosys

# Imports from Coriolis C++ bindings
from coriolis           import Cfg
from coriolis           import Viewer
from coriolis           import CRL
from coriolis.helpers   import overlay

# Import from crlcore
import coriolis.technos.symbolic.cmos


__all__ = [ "setup" ]


def setup( checkToolkit=None ):
    Where( checkToolkit )
    ShellEnv().export()
    
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
        env.setCLOCK( '^ck$|m_clock|^clk' )
        env.addSYSTEM_LIBRARY( library=(Where.coriolisTop / 'pdk' / 'symbolic' / 'niolib' / 'cells' ).as_posix()
                             , mode=CRL.Environment.Append )

    Yosys.setLiberty( Where.coriolisTop / 'pdk' / 'symbolic' / 'sxlib' / 'cells' / 'sxlib.lib' )
    ShellEnv.RDS_TECHNO_NAME = ( Where.coriolisTop / 'pdk' / 'cmos' / 'cmos.rds' ).as_posix()

    path = None
    for pathVar in [ 'PATH', 'path' ]:
        if pathVar in os.environ:
            path = os.environ[ pathVar ]
            os.environ[ pathVar ] = path + ':' + ( Where.allianceTop / 'bin' ).as_posix()
            break

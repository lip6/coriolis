
import sys
import os.path
from   coriolis                 import Cfg
from   coriolis.Hurricane       import Technology, DataBase, DbU, Library, Layer,            \
                                       BasicLayer, Cell, Net, Horizontal, Vertical, Contact, \
                                       Rectilinear, Box, Point, Instance, Transformation,    \
                                       NetExternalComponents, Pad, Path, Query
import coriolis.Viewer
from   coriolis.CRL             import AllianceFramework, Gds, LefImport, CellGauge,  \
                                       RoutingGauge, RoutingLayerGauge
from   coriolis.Anabatic        import StyleFlags
from   coriolis.helpers         import trace, l, u, n, overlay, io, ndaTopDir
from   coriolis.helpers.overlay import CfgCache, UpdateSession


__all__ = [ 'setup' ]


def _routing ():
    """
    Define the routing gauge for I/O cells along with the various P&R tool parameters.
    """
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.chip.block.rails.count    = 2 
        cfg.chip.block.rails.hWidth   = u(30.0)
        cfg.chip.block.rails.vWidth   = u(30.0)
        cfg.chip.block.rails.hSpacing = u( 6.0)
        cfg.chip.block.rails.vSpacing = u( 6.0)
        #cfg.chip.padCorner            = 'gf180mcu_fd_io__cor'
        #cfg.chip.padSpacers           = 'gf180mcu_fd_io__fill10,gf180mcu_fd_io__fill5,gf180mcu_fd_io__fill1'
        cfg.chip.padCoreSide          = 'North'
    af = AllianceFramework.get()
    cg = CellGauge.create( 'LEF.GF_IO_Site'
                         , 'Metal2'  # pin layer name.
                         , u(  1.0)  # pitch.
                         , u(350.0)  # cell slice height.
                         , u( 75.0)  # cell slice step.
                         )
    af.addCellGauge( cg )


def _loadIoLib ( pdkDir ):
    """
    Load the I/O cells from the LEF+GDS files.
    """
    af      = AllianceFramework.get()
    db      = DataBase.getDB()
    tech    = db.getTechnology()
    rootlib = db.getRootLibrary()
    ioLib   = Library.create( rootlib, 'iolib' )
    LefImport.setMergeLibrary( ioLib )
    LefImport.load( (pdkDir / 'libraries'
                            / 'gf180mcu_fd_sc_mcu9t5v0'
                            / 'latest'
                            / 'tech'
                            / 'gf180mcu_6LM_1TM_9K_9t_tech.lef').as_posix() )
    print( '  o  Setup GF180MCU I/O library in {}.'.format( ioLib.getName() ))
    io.vprint( 1, '  o  Setup GF180MCU I/O library in {}.'.format( ioLib.getName() ))
    cellsDir = pdkDir / 'libraries' / 'gf180mcu_fd_io' / 'latest' / 'cells'
    for lefFile in cellsDir.glob( '*/*_5lm.lef' ):
        gdsFile = lefFile.with_suffix( '.gds' )
        if gdsFile.is_file():
            Gds.setTopCellName( gdsFile.stem[:-4] )
            Gds.load( ioLib, gdsFile.as_posix(), Gds.Layer_0_IsBoundary|Gds.NoBlockages )
        LefImport.load( lefFile.as_posix() )
    # Demote the VDD/VSS nets until we understand how that works.
    for cell in ioLib.getCells():
        for net in cell.getNets():
            if net.getName() in ('VDD', 'VSS'):
                net.setExternal( False )
                net.setGlobal( False )
    af.wrapLibrary( ioLib, 1 ) 


def setup ( pdkDir ):
   #with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
   #    cfg.misc.minTraceLevel = 100
   #    cfg.misc.maxTraceLevel = 102
    _routing()
    _loadIoLib( pdkDir )


import sys
import os.path
from   coriolis                 import Cfg
from   coriolis.Hurricane       import Technology, DataBase, DbU, Library, Layer,         \
                                       BasicLayer, Cell, Net, Horizontal, Vertical,       \
                                       Rectilinear, Box, Point, Instance, Transformation, \
                                       NetExternalComponents, Pad
import coriolis.Viewer
from   coriolis.CRL             import AllianceFramework, Environment, Gds, LefImport, \
                                       CellGauge, RoutingGauge, RoutingLayerGauge
from   coriolis.helpers         import l, u, n, overlay, io, ndaTopDir
from   coriolis.helpers.overlay import CfgCache, UpdateSession
from   coriolis.Anabatic        import StyleFlags


__all__ = [ "setup" ]


def _routing ():
    """
    Define the routing gauge along with the various P&R tool parameters.
    """
    af   = AllianceFramework.get()
    db   = DataBase.getDB()
    tech = db.getTechnology()
    rg   = RoutingGauge.create('nsxlib2')
    rg.setSymbolic( True )
    dirM1           = RoutingLayerGauge.Vertical
    dirM2           = RoutingLayerGauge.Horizontal
    netBuilderStyle = 'HV,3RL+'
    routingStyle    = StyleFlags.HV
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL1' )         # metal
                                , dirM1                             # preferred routing direction
                                , RoutingLayerGauge.PinOnly         # layer usage
                                , 0                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(10.0)                           # track pitch
                                , l( 3.0)                           # wire width
                                , l( 3.0)                           # perpandicular wire width
                                , l( 2.0)                           # VIA side
                                , l( 7.0) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL2' )         # metal
                                , dirM2                             # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 1                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(10.0)                           # track pitch
                                , l( 3.0)                           # wire width
                                , l( 3.0)                           # perpandicular wire width
                                , l( 2.0)                           # VIA side
                                , l( 8.0) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL3' )         # metal
                                , dirM1                             # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 2                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(10.0)                           # track pitch
                                , l( 3.0)                           # wire width
                                , l( 3.0)                           # perpandicular wire width
                                , l( 2.0)                           # VIA side
                                , l( 8.0) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL4' )         # metal
                                , dirM2                             # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 3                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(10.0)                           # track pitch
                                , l( 4.0)                           # wire width
                                , l( 4.0)                           # perpandicular wire width
                                , l( 2.0)                           # VIA side
                                , l( 8.0) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL5' )         # metal
                                , dirM1                             # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 4                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(10.0)                           # track pitch
                                , l( 4.0)                           # wire width
                                , l( 4.0)                           # perpandicular wire width
                                , l( 4.0)                           # VIA side
                                , l( 8.0) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'METAL6' )         # metal
                                , dirM2                             # preferred routing direction
                                , RoutingLayerGauge.PowerSupply     # layer usage
                                , 5                                 # depth
                                , 0.0                               # density (deprecated)
                                , l( 0.0)                           # track offset from AB
                                , l(15.0)                           # track pitch
                                , l(12.0)                           # wire width
                                , l(12.0)                           # perpandicular wire width
                                , l( 4.0)                           # VIA side
                                , l( 8.0 ) ))                       # obstacle dW
    af.addRoutingGauge( rg )
    af.setRoutingGauge( 'nsxlib2' )

    cg = CellGauge.create( 'nsxlib2'
                         , 'METAL1'   # pin layer name.
                         , l( 10.0)   # pitch.
                         , l(100.0)   # cell slice height.
                         , l( 10.0)   # cell slice step.
                         )
    af.addCellGauge( cg )
    af.setCellGauge( 'nsxlib2' )

    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        # Place & Route setup
        cfg.viewer.minimumSize = 500
        cfg.viewer.pixelThreshold = 2
        cfg.lefImport.minTerminalWidth = 0.0
        cfg.crlcore.groundName  = 'vss'
        cfg.crlcore.powerName   = 'vdd'
        cfg.etesian.bloat       = 'disabled'
        cfg.etesian.aspectRatio = 1.00
        cfg.etesian.aspectRatio = [10, 1000]
        cfg.etesian.spaceMargin = 0.10
        cfg.etesian.densityVariation = 0.05
        cfg.etesian.routingDriven = False
        cfg.etesian.latchUpDistance = l(2000.0)
       #cfg.etesian.diodeName = 'diode'
       #cfg.etesian.antennaInsertThreshold = 0.50
       #cfg.etesian.antennaMaxWL = u(250.0)
        cfg.etesian.feedNames = 'tie_x0,rowend_x0'
        cfg.etesian.defaultFeed = 'tie_x0'
        cfg.etesian.cell.zero = 'zero_x0'
        cfg.etesian.cell.one = 'one_x0'
        cfg.etesian.effort = 2
        cfg.etesian.effort = ( ('Fast'    , 1)
                             , ('Standard', 2)
                             , ('High'    , 3)
                             , ('Extreme' , 4)
                             )
        cfg.etesian.graphics = 2
        cfg.etesian.graphics = ( ('Show every step' , 1)
                               , ('Show lower bound', 2)
                               , ('Show result only', 3)
                               )
        cfg.anabatic.routingGauge = 'nsxlib2'
        cfg.anabatic.cellGauge = 'nsxlib2'
        cfg.anabatic.globalLengthThreshold = 30*l(100.0)
        cfg.anabatic.saturateRatio = 0.90
        cfg.anabatic.saturateRp = 10
        cfg.anabatic.topRoutingLayer = 'METAL5'
        cfg.anabatic.edgeLength = 192
        cfg.anabatic.edgeWidth = 32
        cfg.anabatic.edgeCostH = 9.0
        cfg.anabatic.edgeCostK = -10.0
        cfg.anabatic.edgeHInc = 1.0
        cfg.anabatic.edgeHScaling = 1.0
        cfg.anabatic.globalIterations = 10
        cfg.anabatic.globalIterations = [ 1, 100 ]
        cfg.anabatic.gcell.displayMode = 1
        cfg.anabatic.gcell.displayMode = (("Boundary", 1), ("Density", 2))
        cfg.anabatic.netBuilderStyle = netBuilderStyle
        cfg.anabatic.routingStyle = routingStyle
        cfg.katana.disableStackedVias = False
        cfg.katana.hTracksReservedLocal = 4
        cfg.katana.hTracksReservedLocal = [0, 20]
        cfg.katana.vTracksReservedLocal = 3
        cfg.katana.vTracksReservedLocal = [0, 20]
        cfg.katana.termSatReservedLocal = 8
        cfg.katana.termSatThreshold = 9
        cfg.katana.eventsLimit = 4000002
        cfg.katana.ripupCost = 3
        cfg.katana.ripupCost = [0, None]
        cfg.katana.strapRipupLimit = 16
        cfg.katana.strapRipupLimit = [1, None]
        cfg.katana.localRipupLimit = 9
        cfg.katana.localRipupLimit = [1, None]
        cfg.katana.globalRipupLimit = 5
        cfg.katana.globalRipupLimit = [1, None]
        cfg.katana.longGlobalRipupLimit = 5
        cfg.chip.padCoreSide = 'North'
        # Plugins setup
        cfg.clockTree.minimumSide = l(100.0) * 6
        cfg.clockTree.buffer = 'buf_x8'
        cfg.clockTree.placerEngine = 'Etesian'
        cfg.block.spareSide = 10*l(100.0)
        cfg.spares.buffer = 'buf_x8'
        cfg.spares.maxSinks = 31


def _loadNsxlib2 ( cellsTop ):
    """
    Setup for NSXLIB2 Alliance library. It is an symbolic library
    from which cells are loaded on demand, so we only setup pathes.

    :param cellsTop: The top directory containing the cells views.
    """
    af  = AllianceFramework.get()
    env = af.getEnvironment()
    env.setSCALE_X ( 100 )
    env.setCATALOG ( 'CATAL' )
    env.setPOWER   ( 'vdd'   )   
    env.setGROUND  ( 'vss'   )   
    env.setCLOCK   ( '^ck$|m_clock|^clk$' )
    env.setBLOCKAGE( 'blockage[Nn]et.*' )
    env.setPad     ( '.*_mpx$'          )
    env.setRegister( 'sff.*' )
    env.setWORKING_LIBRARY( '.' )
    env.addSYSTEM_LIBRARY ( library=(cellsTop / 'nsxlib2').as_posix(), mode=Environment.Append )


def setup ( cellsTop ):
    _routing()
    _loadNsxlib2( cellsTop )

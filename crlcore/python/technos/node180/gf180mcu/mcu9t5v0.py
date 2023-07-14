
import sys
import os.path
from   coriolis                 import Cfg
from   coriolis.Hurricane       import Technology, DataBase, DbU, Library, Layer,         \
                                       BasicLayer, Cell, Net, Horizontal, Vertical,       \
                                       Rectilinear, Box, Point, Instance, Transformation, \
                                       NetExternalComponents, Pad
import coriolis.Viewer
from   coriolis.CRL             import AllianceFramework, Gds, LefImport, CellGauge,  \
                                       RoutingGauge, RoutingLayerGauge
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
    rg   = RoutingGauge.create('mcu9t5v0')
    rg.setSymbolic( False )
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'Metal1' )         # metal
                                , RoutingLayerGauge.Horizontal      # preferred routing direction
                                , RoutingLayerGauge.PinOnly         # layer usage
                                , 0                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.56)                           # track pitch
                                , u(0.23)                           # wire width
                                , u(0.23)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'Metal2' )         # metal
                                , RoutingLayerGauge.Vertical        # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 1                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.56)                           # track pitch
                                , u(0.28)                           # wire width
                                , u(0.28)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'Metal3' )         # metal
                                , RoutingLayerGauge.Horizontal      # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 2                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.56)                           # track pitch
                                , u(0.28)                           # wire width
                                , u(0.28)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'Metal4' )         # metal
                                , RoutingLayerGauge.Vertical        # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 3                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.56)                           # track pitch
                                , u(0.28)                           # wire width
                                , u(0.28)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'Metal5' )         # metal
                                , RoutingLayerGauge.Horizontal      # preferred routing direction
                                , RoutingLayerGauge.Default         # layer usage
                                , 4                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.56)                           # track pitch
                                , u(0.28)                           # wire width
                                , u(0.28)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    rg.addLayerGauge(
        RoutingLayerGauge.create( tech.getLayer( 'MetalTop' )       # metal
                                , RoutingLayerGauge.Vertical        # preferred routing direction
                                , RoutingLayerGauge.PowerSupply     # layer usage
                                , 5                                 # depth
                                , 0.0                               # density (deprecated)
                                , u(0.0)                            # track offset from AB
                                , u(0.9)                            # track pitch
                                , u(0.44)                           # wire width
                                , u(0.44)                           # perpandicular wire width
                                , u(0.26)                           # VIA side
                                , u(0.0 ) ))                        # obstacle dW
    af.addRoutingGauge( rg )
    af.setRoutingGauge( 'mcu9t5v0' )

    cg = CellGauge.create( 'LEF.GF018hv5v_green_sc9'
                         , 'Metal1'   # pin layer name.
                         , u( 0.56 )  # pitch.
                         , u( 5.04)   # cell slice height.
                         , u( 0.56)   # cell slice step.
                         )
    af.addCellGauge( cg )
    af.setCellGauge( 'LEF.GF018hv5v_green_sc9' )

    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        env = af.getEnvironment()
        env.setRegister( '.*__dff.*' )
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
        cfg.etesian.uniformDensity = True
        cfg.etesian.routingDriven = False
        cfg.etesian.latchUpDistance = u(30.0 - 1.0)
       #cfg.etesian.diodeName = 'diode'
       #cfg.etesian.antennaInsertThreshold = 0.50
       #cfg.etesian.antennaMaxWL = u(250.0)
        cfg.etesian.feedNames = 'gf180mcu_fd_sc_mcu9t5v0__fill_1,gf180mcu_fd_sc_mcu9t5v0__fill_2,gf180mcu_fd_sc_mcu9t5v0__fill_4,gf180mcu_fd_sc_mcu9t5v0__fill_8'
        cfg.etesian.defaultFeed = 'fill_2'
        cfg.etesian.cell.zero = 'gf180mcu_fd_sc_mcu9t5v0__tieh'
        cfg.etesian.cell.one = 'gf180mcu_fd_sc_mcu9t5v0__tiel'
        cfg.etesian.effort = 2
        cfg.etesian.effort = (
            ('Fast', 1),
            ('Standard', 2),
            ('High', 3 ),
            ('Extreme', 4 ),
        )
        cfg.etesian.graphics = 2
        cfg.etesian.graphics = (
            ('Show every step', 1),
            ('Show lower bound', 2),
            ('Show result only', 3),
        )
        cfg.anabatic.routingGauge = 'mcu9t5v0'
        cfg.anabatic.cellGauge = 'LEF.GF018hv5v_green_sc9'
        cfg.anabatic.globalLengthThreshold = 1450
        cfg.anabatic.saturateRatio = 0.90
        cfg.anabatic.saturateRp = 10
       #cfg.anabatic.topRoutingLayer = 'mt2'
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
        cfg.anabatic.netBuilderStyle = 'VH,3RL+'
        cfg.anabatic.routingStyle = StyleFlags.VH
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
        cfg.chip.padCoreSide = 'South'
        # Plugins setup
        cfg.clockTree.minimumSide = u(5.04) * 6
        cfg.clockTree.buffer = 'gf180mcu_fd_sc_mcu9t5v0__clkbuf_2'
        cfg.clockTree.placerEngine = 'Etesian'
        cfg.block.spareSide = 10
        cfg.spares.buffer = 'gf180mcu_fd_sc_mcu9t5v0__clkbuf_2'
        cfg.spares.maxSinks = 20


def _loadStdLib ( cellsTop ):
    """
    Load the muc9t5v0 library from the GDS files.

    :param cellsTop: The top directory containing the cells views.

    As GDS file is lacking most of the interface informations, apply a
    post-process on each cell to educated guess :

    * Which nets are external, and in which direction (name matching).
    * Blockages: any shape in internals nets in Metal1 or Metal2 layer.
    """
    useGds  = False
    af      = AllianceFramework.get()
    db      = DataBase.getDB()
    tech    = db.getTechnology()
    rootlib = db.getRootLibrary()
    cellLib = Library.create(rootlib, 'mcu9t5v')
    af.wrapLibrary( cellLib, 0 )

    gaugeName    = Cfg.getParamString('anabatic.routingGauge').asString()
    routingGauge = af.getRoutingGauge( gaugeName )
    metal1       = DataBase.getDB().getTechnology().getLayer( 'Metal1' )
    metal2       = DataBase.getDB().getTechnology().getLayer( 'Metal2' )
    blockage1    = metal1.getBlockageLayer()
    blockage2    = metal2.getBlockageLayer()
    hpitch       = 0
    for layerGauge in routingGauge.getLayerGauges():
        if layerGauge.getType() == RoutingLayerGauge.PinOnly:
            continue
        if layerGauge.getDirection() == RoutingLayerGauge.Horizontal:
            hpitch = layerGauge.getPitch()
            break

    if useGds:
        io.vprint( 1, '  o  Setup GF 180 mcu9t5v library in {} [GDS].'.format( cellLib.getName() ))
        io.vprint( 2, '     (__file__="{}")'.format( os.path.abspath( __file__ )))
        for cellDir in cellsTop.iterdir():
            for gdsFile in sorted(cellDir.glob('*.gds')):
                Gds.load( cellLib
                        , gdsFile.as_posix()
                        , Gds.NoGdsPrefix|Gds.Layer_0_IsBoundary )
       #io.vprint( 1, '  o  Skrinking V-AB of {}'.format(DbU.getValueString( hpitch )))
        with overlay.UpdateSession():
            for cell in cellLib.getCells():
                ab = cell.getAbutmentBox()
               #ab.inflate( 0, -hpitch )
                cell.setAbutmentBox( ab )
                cell.setTerminalNetlist( True )
                for net in cell.getNets():
                    if not net.isExternal():
                        blockages = []
                        for component in net.getComponents():
                            if component.getLayer() == metal1 or component.getLayer() == metal1:
                                blockages.append( component )
                        if blockages:
                            io.vprint( 2, '     - Obstacles found in {}'.format( cell ))
                        for component in blockages:
                            bb = component.getBoundingBox()
                            if component.getLayer() == metal1:
                                v = Vertical.create( net
                                                   , blockage1
                                                   , bb.getXCenter()
                                                   , bb.getWidth()
                                                   , bb.getYMin()
                                                   , bb.getYMax() )
                            if component.getLayer() == metal2:
                                h = Horizontal.create( net
                                                     , blockage2
                                                     , bb.getYCenter()
                                                     , bb.getHeight()
                                                     , bb.getXMin()
                                                     , bb.getXMax() )
                        continue
                    if net.isPower() or net.getName() == 'VDD':
                        net.setName( 'VDD' )
                        net.setType( Net.Type.POWER )
                        net.setGlobal( True )
                        net.setDirection( Net.Direction.IN )
                        continue
                    if net.isGround() or net.getName() == 'VSS':
                        net.setName( 'VSS' )
                        net.setType( Net.Type.GROUND )
                        net.setGlobal( True )
                        net.setDirection( Net.Direction.IN )
                        continue
                    if    net.getName() == 'Z'  \
                       or net.getName() == 'ZN' \
                       or net.getName() == 'Q':
                        net.setDirection( Net.Direction.OUT )
                    else:
                        net.setDirection( Net.Direction.IN )
                    toDestroy = []
                    for component in NetExternalComponents.get(net):
                        if isinstance(component,Pad):
                            bb  = component.getBoundingBox()
                            pad = Vertical.create( net
                                                 , component.getLayer()
                                                 , bb.getCenter().getX()
                                                 , bb.getWidth()
                                                 , bb.getYMin()
                                                 , bb.getYMax() )
                            NetExternalComponents.setExternal( pad )
                            toDestroy.append( component )
                    for component in toDestroy:
                        component.destroy()
    else:
        io.vprint( 1, '  o  Setup GF 180 mcu9t5v library in {} [LEF].'.format( cellLib.getName() ))
        io.vprint( 2, '     (__file__="{}")'.format( os.path.abspath( __file__ )))
        LefImport.load( (cellsTop / '..' / 'tech' / 'gf180mcu_6LM_1TM_9K_9t_tech.lef').as_posix() )
        LefImport.setMergeLibrary( cellLib )
        for cellDir in cellsTop.iterdir():
            for lefFile in sorted(cellDir.glob('*.lef')):
                LefImport.load( lefFile.as_posix() )
    af.wrapLibrary( cellLib, 1 )
    return cellLib


def _loadIoLib ():
    """
    Load the IO library from the GDS files.
    """
    af    = AllianceFramework.get()
    ioLib = af.getLibrary( 1 )
    io.vprint( 1, '  o  Loading GDS library in "{}".'.format( ioLib.getName() ))
    Gds.load( ioLib
            , ndaTopDir + '/XXXX.gds'
            , Gds.NoGdsPrefix )


def setup ( cellsTop ):
    _routing()
    _loadStdLib( cellsTop )
    #_loadIoLib()

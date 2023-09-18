#!/usr/bin/env python3

import sys
from coriolis import Cfg
from coriolis.Hurricane import DbU, Point, Box, DataBase, Technology, \
                         BasicLayer, ViaLayer, RegularLayer
from coriolis.helpers.overlay    import CfgCache
from coriolis.helpers.technology import createBL

def flush ():
    sys.stdout.flush()
    sys.stderr.flush()

#def u ( value ): return value
#def l ( value ): return value
def l ( value ): return DbU.fromLambda( value )
def u ( value ): return DbU.fromPhysical( value, DbU.UnitPowerMicro )
def n ( value ): return DbU.fromPhysical( value, DbU.UnitPowerNano  )

def cfg_setup():
    print( "" )
    print( "Test Cfg module" )
    print( "========================================" )
    # Place & Route setup
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.lefImport.minTerminalWidth = 0.0
        cfg.crlcore.groundName = 'vss'
        cfg.crlcore.powerName = 'vdd'
        cfg.etesian.aspectRatio = 1.00
        cfg.etesian.aspectRatio = [10, 1000]
        cfg.etesian.spaceMargin = 0.10
        cfg.etesian.densityVariation = 1.0
        cfg.etesian.routingDriven = False
        #cfg.etesian.latchUpDistance = u(30.0 - 1.0)
        cfg.etesian.latchUpDistance = 0
        cfg.etesian.antennaInsertThreshold = 0.50
        cfg.etesian.antennaMaxWL = u(250.0)
        cfg.etesian.antennaGateMaxWL = u(250.0)
        cfg.etesian.antennaDiodeMaxWL = u(550.0)
        cfg.etesian.tieName = 'tiepoly_x0'
        cfg.etesian.feedNames = 'tiepoly_x0'
        cfg.etesian.cell.zero = 'zero_x0'
        cfg.etesian.cell.one = 'one_x0'
        cfg.etesian.bloat = 'disabled'
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
        cfg.anabatic.routingGauge = 'FlexLib'
        cfg.anabatic.globalLengthThreshold = 1450
        cfg.anabatic.saturateRatio = 0.90
        cfg.anabatic.saturateRp = 10
        cfg.anabatic.topRoutingLayer = 'METAL6'
        cfg.anabatic.edgeLength = 48
        cfg.anabatic.edgeWidth = 8
        cfg.anabatic.edgeCostH = 9.0
        cfg.anabatic.edgeCostK = -10.0
        cfg.anabatic.edgeHInc = 1.0
        cfg.anabatic.edgeHScaling = 1.0
        cfg.anabatic.globalIterations = 10
        cfg.anabatic.globalIterations = [ 1, 100 ]
        cfg.anabatic.gcell.displayMode = 1
        cfg.anabatic.gcell.displayMode = (("Boundary", 1), ("Density", 2))
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
    with CfgCache(priority=Cfg.Parameter.Priority.ConfigurationFile) as cfg:
        cfg.viewer.minimumSize = 500
        cfg.viewer.pixelThreshold = 20
        cfg.chip.block.rails.count = 0
        cfg.chip.block.rails.hWidth = u(2.68)
        cfg.chip.block.rails.vWidth = u(2.68)
        cfg.chip.block.rails.hSpacing = 0 #u(0.7)
        cfg.chip.block.rails.vSpacing = 0 #u(0.7)
        cfg.clockTree.minimumSide = l(600)
        cfg.clockTree.buffer = 'buf_x2'
        cfg.clockTree.placerEngine = 'Etesian'
        cfg.block.spareSide = 10
        cfg.spares.buffer = 'buf_x8'
        cfg.spares.maxSinks = 31


def testDbU ():
    print( "" )
    print( "Test Hurricane::DbU" )
    print( "========================================" )
    DbU.setPrecision( 2 )
    DbU.setPhysicalsPerGrid( 0.5, DbU.UnitPowerMicro )
    DbU.setGridsPerLambda( 2.0 )
    DbU.setSymbolicSnapGridStep( DbU.fromLambda(1.0) )
    DbU.setPolygonStep(DbU.fromGrid( 2.0) )
    DbU.setStringMode( DbU.StringModeSymbolic, DbU.UnitPowerMicro )
    value = DbU.fromPhysical( 10.0, DbU.UnitPowerMicro )
    print( "value={}".format( DbU.getValueString(value) ))


def testPoint ():
    print( "" )
    print( "Test Hurricane::Point" )
    print( "========================================" )
    p1 = Point( 1000, 2000 )
    print( p1 )
    p2 = Point( 1000, 2000 )
    print( p1 == p2 )
    p3 = Point( 2000, 2000 )
    print( p1 == p3 )
    p4 = Point( p3 )
    print( p4 )


def testBox ():
    print( "" )
    print( "Test Hurricane::Box" )
    print( "========================================" )
    b1 = Box()
    print( b1 )
    b1.merge( Point(1000,1000) )
    print( 'b1.merge(Point(1000,1000)={}'.format(b1) )
    b1.merge( Box(Point(2000,2000)) )
    print( 'b1.merge(Box(Point(1000,1000))={}'.format(b1) )
    print( 'b1.getCenter()={}'.format(b1.getCenter()) )
    p1 = Point( 1000, 1000 )
    b2 = Box( p1 )
    print( b2 )


def testDB ():
    print( "" )
    print( "Test Hurricane::DataBase" )
    print( "========================================" )
    db = DataBase.create()
    print( 'DataBase.getDB()={}'.format(DataBase.getDB()) )


def testTechnology ():
    print( "" )
    print( "Test Hurricane::Technology" )
    print( "========================================" )
    tech = Technology.create( DataBase.getDB(), 'test_techno' )
    print( 'tech={}'.format(tech) )



def testBasicLayer ():
    print( "" )
    print( "Test Hurricane::BasicLayer" )
    print( "========================================" )
    db     = DataBase.getDB()
    tech   = db.getTechnology()
    nWell  = createBL( tech, 'nWell' , BasicLayer.Material.nWell )
    pWell  = createBL( tech, 'pWell' , BasicLayer.Material.pWell )
    cut1   = createBL( tech, 'cut1'  , BasicLayer.Material.cut )
    metal1 = createBL( tech, 'metal1', BasicLayer.Material.metal )
    metal2 = createBL( tech, 'metal2', BasicLayer.Material.metal )
    METAL1 = RegularLayer.create( tech, 'METAL1', metal1 )
    METAL2 = RegularLayer.create( tech, 'METAL2', metal2 )
    VIA12  = ViaLayer    .create( tech, 'VIA12' , metal1, cut1, metal2  )
    print( 'nWell={}'.format(nWell) )
    print( 'pWell={}'.format(pWell) )
    print( dir(nWell) )
    print( 'Technology.getLayers():' )
    flush()
    for layer in tech.getLayers():
        flush()
        print( '| basicLayer={}'.format(layer) )
    flush()


if __name__ == '__main__':
    testDbU()
    cfg_setup()
    testPoint()
    testBox()
    testDB()
    testTechnology()
    testBasicLayer()
    sys.exit( 0 )

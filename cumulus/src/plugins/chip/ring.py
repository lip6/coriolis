
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2026-2026.
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/chip/ring.py"                   |
# +-----------------------------------------------------------------+


import sys
import traceback
import os.path
import optparse
import math
import cProfile
import pstats
from   ...                   import Cfg
from   ...Hurricane          import DataBase, DbU ,Point, Transformation, Box,      \
                                    Path, Occurrence, UpdateSession, Breakpoint,    \
                                    Net, RoutingPad, Contact, Horizontal, Vertical, \
                                    Instance, HyperNet, Query
from   ...CRL                import AllianceFramework, Catalog, RoutingLayerGauge
from   ...helpers            import trace
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import UpdateSession
from   ...                   import Etesian, Anabatic, Katana, Unicorn
from   ..block.configuration import IoPin
from   ..chip.chip           import Chip
from   .                     import power       as chipPower
from   .                     import powerplane  as chipPowerplane
from   .                     import corona      as chipCorona
from   .constants            import importConstants

importConstants( globals() )


__all__ = [ 'RingConf' ]

af = AllianceFramework.get()


# --------------------------------------------------------------------
# Class  :  "ring.Side"

class Side ( object ):

    def __init__ ( self, corona, sideType ):
        self.type          = sideType
        self.corona        = corona
        self.pins          = []


# --------------------------------------------------------------------
# Class  :  "ring.Corona"

class Corona ( object ):
    """
    This class is a greatly simplified version of ``pads.Corona``,
    suited for stand-alone blocks *with* and external power ring.

    As we don't connect to chip I/O pad, all the mechanims need to
    create the wiring both at corona and chip level is removed.
    """

    def __init__ ( self, ring ):
        self.ring      = ring
        self.northSide = Side( self, North )
        self.southSide = Side( self, South )
        self.eastSide  = Side( self, East  )
        self.westSide  = Side( self, West  )

    def doPowerLayout ( self ):
        """
        This method is not needed in this class, but is called in
        ``chip.doConnectCore()``, so we provide a dummy.
        """
        pass

    def copySidePowerPins ( self, sideType ):
        if sideType & IoPin.SOUTH:
            ioPinsSide = self.ring.sides[IoPin.SOUTH]
            coronaSide = self.southSide
        elif sideType & IoPin.NORTH:
            ioPinsSide = self.ring.sides[IoPin.NORTH]
            coronaSide = self.northSide
        elif sideType & IoPin.EAST:
            ioPinsSide = self.ring.sides[IoPin.EAST]
            coronaSide = self.eastSide
        elif sideType & IoPin.WEST:
            ioPinsSide = self.ring.sides[IoPin.WEST]
            coronaSide = self.westSide
        for upos, pins in ioPinsSide.pins.items():
            for pin in pins:
                if not pin.getNet().isSupply(): continue
                coronaSide.pins.append( pin )

    def copyPowerPins ( self ):
        """
        Copy the supply pins from the P&R cell. They have been defined
        and made by a prvious call to ``Block.placeIopins()``.

        We only needs to provide the list of pins per side in the
        Corona object.
        """
        self.copySidePowerPins( IoPin.NORTH )
        self.copySidePowerPins( IoPin.SOUTH )
        self.copySidePowerPins( IoPin.EAST )
        self.copySidePowerPins( IoPin.WEST )


# --------------------------------------------------------------------
# Class  :  "ring.Ring"

class Ring ( Chip ):

    def __init__ ( self, conf ):
        trace( 550, ',+', '\tRing.__init__()\n' )
        super(Ring,self).__init__( conf )
        self.conf.minHCorona = 6*self.conf.sliceHeight
        self.conf.minVCorona = 6*self.conf.sliceHeight
        self.padsCorona = Corona( self )
        self.conf.powerRingHorizontalDepth = -1
        self.conf.powerRingVerticalDepth   = -1
        for depth in range(0,self.conf.topLayerDepth+1):
            trace( 550, '\tdepth:{} {}\n'.format(depth,self.conf._routingGauge.getLayerGauge(depth) ))
            if self.conf._routingGauge.getLayerGauge(depth).getType() == RoutingLayerGauge.Unusable:
                continue
            if self.conf._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Horizontal:
                if self.conf.powerRingHorizontalDepth < 0:
                    self.conf.powerRingHorizontalDepth = depth
            if self.conf._routingGauge.getLayerGauge(depth).getDirection() == RoutingLayerGauge.Vertical:
                if self.conf.powerRingVerticalDepth < 0:
                    self.conf.powerRingVerticalDepth = depth
        trace( 550, '\tself.conf.powerRingHorizontalDepth={}\n'.format( self.conf.powerRingHorizontalDepth ))
        trace( 550, '\tself.conf.powerRingVerticalDepth  ={}\n'.format( self.conf.powerRingVerticalDepth ))
        trace( 550, '\tself.conf.railsCount={}\n'.format( self.conf.railsCount ))
        trace( 550, ',-' )
  
    def doRingNetlist ( self ):
        """
        Build the corona level netlist, that will contains the power ring by
        cloning the netlist of the core.
        """
        trace( 550, ',+', '\tRing.doRingNetlist(): "{}"\n'.format(self.conf.cell.getName()) )
        with UpdateSession():
            coronaName = 'ring_{}'.format( self.conf.cell.getName() )
            print( '  o  Build Ring from Core.' )
            print( '     - Core:           "{}".'.format(self.conf.cell.getName()) )
            print( '     - Ring (corona):  "{}".'.format(coronaName) )
            self.conf.corona = af.createCell( coronaName )
            self.conf.icore  = Instance.create( self.conf.corona, 'core', self.conf.core )

            for coreNet in self.conf.core.getNets():
                if not coreNet.isExternal() and not coreNet.isGlobal():
                    continue

                trace( 550, '\tConnect core net "{}"\n'.format(coreNet.getName()) )
                coronaNet = Net.create( self.conf.corona, coreNet.getName() )
                coronaNet.setDirection( coreNet.getDirection() )
                coronaNet.setType     ( coreNet.getType() )
                coronaNet.setExternal ( True )
                if coreNet.isGlobal():
                    coronaNet.setGlobal( coreNet.isGlobal() )
                else:
                    self.conf.icore.getPlug( coreNet ).setNet( coronaNet )
        trace( 550, ',-' )
  
    def doRingFloorplan ( self ):
        """
        Build the corona level placement. Basically place the core inside the
        corona, with one slice height of free space on each side.

        Simplified version of chip.Configuration.setupICore().
        """
        trace( 550, ',+', '\tRing.doRingFloorplan()"\n' )
        with UpdateSession():
            minHCorona = self.conf.minHCorona
            minVCorona = self.conf.minVCorona
            coreAb     = self.conf.coreAb
            coronaAb   = Box( coreAb ).inflate( 0, 0, minHCorona*2, minVCorona*2 )
            trace( 550, f'\tcoreAb={coreAb}"\n' )
            trace( 550, f'\tcoronaAb={coronaAb}"\n' )
            self.conf.core.setAbutmentBox( coreAb )
            self.conf.corona.setAbutmentBox( coronaAb )
            self.conf.icore.setTransformation( Transformation( minHCorona
                                                             , minVCorona
                                                             , Transformation.Orientation.ID ) )
            self.conf.icore.setPlacementStatus( Instance.PlacementStatus.FIXED )
        for side in self.sides.values(): side.setupAb()
        self.placeIoPins()
        self.padsCorona.copyPowerPins()
        self.conf.findPowerAndClockNets()

        
        trace( 550, ',-' )
  
    def doPnR ( self ):
        status = super(Chip,self).doPnR()
        self.conf.refresh( self.conf.cellPnR )
        return status

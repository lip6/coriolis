#
# This file is part of the Coriolis Software.
# Copyright (c) SU 2020-2020, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/clocktree.py"             |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import os.path
import Cfg
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Layer
from   Hurricane import Occurrence
from   Hurricane import Net
from   Hurricane import HyperNet
from   Hurricane import RoutingPad
from   Hurricane import Horizontal
from   Hurricane import Vertical
from   Hurricane import Contact
from   Hurricane import Pin
from   Hurricane import Plug
from   Hurricane import Instance
import CRL
from   CRL             import RoutingLayerGauge
from   helpers         import trace
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.io      import catch
from   helpers.overlay import UpdateSession
from   plugins         import getParameter
from   plugins.alpha   import utils
from   plugins.alpha.block.configuration import GaugeConf
from   plugins.alpha.block.spares        import Spares


# ----------------------------------------------------------------------------
# Class  :  "clocktree.ClockTree".

class ClockTree ( object ):
    """
    Build a clock tree on a block.
    """

    def __init__ ( self, spares, clockNet, index ):
        self.spares     = spares
        self.clockNet   = clockNet
        self.clockIndex = index
        self.subNets    = []
        if not self.clockNet.isClock():
            print( WarningMessage( 'ClockTree.__init__(): Net "{}" is not of CLOCK type.' \
                                   .format(self.clockNet.getName()) ))

    def destroy ( self ):
        trace( 550, ',+', '\tClockTree.destroy() "{}"\n'.format(self.clockNet.getName()) )
        with UpdateSession():
            for subNet in self.subNets + [ self.clockNet ]:
                components = []
                for comp in subNet.getComponents():
                    if isinstance(comp,RoutingPad): components.append( comp )
                    if isinstance(comp,Pin       ): components.append( comp )
                for comp in components:
                    comp.destroy()
                if subNet != self.clockNet:
                    subNet.destroy()
        trace( 550, '-' )

    def _rconnectHTree ( self, qt ):
        if qt.isLeaf(): return False
        qt.rconnectBuffer()
        driverNet = qt.bOutputPlug.getNet()
        for leaf in qt.leafs:
            leaf.bInputPlug.setNet( driverNet )
            self._rconnectHTree( leaf )
        return True

    def _rrouteHTree ( self, qt ):
        """
        Recursively build one HTree branch for all non-terminal nodes of the QuadTree.
        """
        trace( 550, ',+', '\tClockTree._rrouteHTree() {}\n'.format(qt.bOutputPlug.getNet()) )
        trace( 550, '\tOn: {}\n'.format(qt) )
        if qt.isLeaf():
            trace( 550, '-' )
            return False

        gaugeConf  = self.spares.conf
        bufferConf = self.spares.conf.bufferConf
        ckNet      = qt.bOutputPlug.getNet()
        self.subNets.append( ckNet )
        leftSourceContact  = gaugeConf.rpAccessByPlugName( qt.buffer   , bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
        rightSourceContact = gaugeConf.rpAccessByPlugName( qt.buffer   , bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
        blContact          = gaugeConf.rpAccessByPlugName( qt.bl.buffer, bufferConf.input , ckNet )
        brContact          = gaugeConf.rpAccessByPlugName( qt.br.buffer, bufferConf.input , ckNet )
        tlContact          = gaugeConf.rpAccessByPlugName( qt.tl.buffer, bufferConf.input , ckNet )
        trContact          = gaugeConf.rpAccessByPlugName( qt.tr.buffer, bufferConf.input , ckNet )
        leftContact        = gaugeConf.createContact( ckNet, blContact.getX(),  leftSourceContact.getY(), 0 )
        rightContact       = gaugeConf.createContact( ckNet, brContact.getX(), rightSourceContact.getY(), 0 )
        leftSourceX  = gaugeConf.getNearestVerticalTrack  ( qt.root.area,  leftSourceContact.getX(), 0 )
        leftSourceY  = gaugeConf.getNearestHorizontalTrack( qt.root.area,  leftSourceContact.getY(), 0 )
        rightSourceX = gaugeConf.getNearestVerticalTrack  ( qt.root.area, rightSourceContact.getX(), 0 )
        rightSourceY = gaugeConf.getNearestHorizontalTrack( qt.root.area, rightSourceContact.getY(), 0 )
        leftX        = gaugeConf.getNearestVerticalTrack  ( qt.root.area,        leftContact.getX(), 0 )
        rightX       = gaugeConf.getNearestVerticalTrack  ( qt.root.area,       rightContact.getX(), 0 )
        tlY          = gaugeConf.getNearestHorizontalTrack( qt.root.area,          tlContact.getY(), 0 )
        blY          = gaugeConf.getNearestHorizontalTrack( qt.root.area,          blContact.getY(), 0 )
        gaugeConf.setStackPosition(  leftSourceContact,  leftSourceX,  leftSourceY )
        gaugeConf.setStackPosition( rightSourceContact, rightSourceX, rightSourceY )
        gaugeConf.setStackPosition( tlContact,  leftX, tlY )
        gaugeConf.setStackPosition( blContact,  leftX, blY )
        gaugeConf.setStackPosition( trContact, rightX, tlY )
        gaugeConf.setStackPosition( brContact, rightX, blY )
        leftContact .setX(        leftX )
        leftContact .setY(  leftSourceY )
        rightContact.setX(       rightX )
        rightContact.setY( rightSourceY )
        gaugeConf.createHorizontal( leftContact       , leftSourceContact, leftSourceY , 0 )
        gaugeConf.createHorizontal( rightSourceContact, rightContact     , rightSourceY, 0 )
        gaugeConf.createVertical  ( leftContact       , blContact        , leftX       , 0 )
        gaugeConf.createVertical  ( tlContact         , leftContact      , leftX       , 0 )
        gaugeConf.createVertical  ( rightContact      , brContact        , rightX      , 0 )
        gaugeConf.createVertical  ( trContact         , rightContact     , rightX      , 0 )
        if qt.isRoot():
            ckNet = self.clockNet
            trace( 550, '\tRemoving any previous pin...\n' )
            pins = []
            for pin in ckNet.getPins(): pins.append( pin )
            for pin in pins:
                print( WarningMessage('ClockTree._rrouteHTree(): Removing {}.'.format(pin)) )
                pin.destroy()
            if not self.spares.conf.isCoreBlock:
                layerGauge  = gaugeConf.vRoutingGauge
                rootContact = gaugeConf.rpAccessByPlugName( qt.buffer, bufferConf.input, ckNet, 0 )
                x           = gaugeConf.getNearestVerticalTrack  ( qt.area, rootContact.getX(), 0 )
                y           = gaugeConf.getNearestHorizontalTrack( qt.area, rootContact.getY(), 0 )
                rootPin     = Pin.create( ckNet
                                        , ckNet.getName()+'.0'
                                        , Pin.Direction.NORTH
                                        , Pin.PlacementStatus.FIXED
                                        , layerGauge.getLayer()
                                        , x
                                        , qt.area.getYMax() 
                                        , layerGauge.getViaWidth()
                                        , layerGauge.getViaWidth()
                                        )
                gaugeConf.setStackPosition( rootContact, x, y )
                gaugeConf.createVertical( rootContact, rootPin, x, 0 )
        for leaf in qt.leafs:
            self._rrouteHTree( leaf )
        trace( 550, '-' )
        return True
        
    def buildHTree ( self ):
        """
        Create the clock tree netlist in two steps:
        1. Connect the buffers of the spares QuadTree to form a H-Tree.
        2. Detach the all the clock sink point and reconnect them to the
           buffers of the leafs of the QuadTree.
        """
        qt           = self.spares.quadTree
        qt.bufferTag = self.clockNet.getName()
        qt.rselectBuffer( self.clockIndex, self.clockIndex, Spares.CHECK_USED|Spares.MARK_USED)
        with UpdateSession():
            self._rconnectHTree( qt )
            self._rrouteHTree  ( qt )

    def splitClock ( self ):
        """
        Disconnect the registers from the main clock and reconnect them to
        the leaf buffers of the clock tree.
        """
        bufferConf         = self.spares.conf.bufferConf
        quadTree           = self.spares.quadTree
        quadTree.bufferTag = self.clockNet.getName()
        quadTree.rselectBuffer( self.clockIndex, self.clockIndex, 0 )
        with UpdateSession():
            hyperClock = HyperNet.create( Occurrence(self.clockNet) )
            for plugOccurrence in hyperClock.getTerminalNetlistPlugOccurrences():
                quadTree.attachToLeaf( plugOccurrence )
            quadTree.rsplitNetlist()
            if self.spares.conf.isCoreBlock:
                plug = utils.getPlugByName( quadTree.buffer, bufferConf.input )
                plug.setNet( self.clockNet )
                trace( 550, '\tCore mode, setting only root plug "{}"\n'.format(self.clockNet.getName()) )
                trace( 550, '\tPlug of "{}"\n'.format(self.clockNet.getName()) )
                for plug in self.clockNet.getPlugs():
                    trace( 550, '\t| {}\n'.format(plug) )

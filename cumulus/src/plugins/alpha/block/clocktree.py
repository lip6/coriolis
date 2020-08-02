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
from   plugins         import utils
from   plugins.alpha.block.configuration import GaugeConf


# ----------------------------------------------------------------------------
# Class  :  "clocktree.ClockTree".

class ClockTree ( object ):
    """
    Build a clock tree on a block.
    """

    def __init__ ( self, spares, clockNet ):
        self.spares   = spares
        self.clockNet = clockNet

        print( WarningMessage('ClockTree.__init__(): Net "{}" is not of CLOCK type.' \
                              .format(self.clockNet.getName())) )

    def _rconnectHTree ( self, quadTree ):
        if quadTree.isLeaf(): return False
        driverNet = quadTree.bOutputPlug.getNet()
        for leaf in quadTree.leafs:
            leaf.bInputPlug.setNet( driverNet )
            self._rconnectHTree( leaf )
        return True

    def _rrouteHTree ( self, quadT ):
        """
        Recursively build one HTree branch for all non-terminal nodes of the QuadTree.
        """
        trace( 550, ',+', '\tClockTree._rrouteHTree() {}\n'.format(quadT.bOutputPlug.getNet()) )
        if quadT.isLeaf():
            trace( 550, '-' )
            return False

        gaugeConf  = self.spares.state.gaugeConf
        bufferConf = self.spares.state.bufferConf
        ckNet      = quadT.bOutputPlug.getNet()
        
        leftSourceContact  = gaugeConf.rpAccessByPlugName( quadT.buffer   , bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
        rightSourceContact = gaugeConf.rpAccessByPlugName( quadT.buffer   , bufferConf.output, ckNet , GaugeConf.HAccess|GaugeConf.OffsetBottom1 )
        blContact          = gaugeConf.rpAccessByPlugName( quadT.bl.buffer, bufferConf.input , ckNet )
        brContact          = gaugeConf.rpAccessByPlugName( quadT.br.buffer, bufferConf.input , ckNet )
        tlContact          = gaugeConf.rpAccessByPlugName( quadT.tl.buffer, bufferConf.input , ckNet )
        trContact          = gaugeConf.rpAccessByPlugName( quadT.tr.buffer, bufferConf.input , ckNet )
        leftContact        = gaugeConf.createContact( ckNet, blContact.getX(),  leftSourceContact.getY(), 0 )
        rightContact       = gaugeConf.createContact( ckNet, brContact.getX(), rightSourceContact.getY(), 0 )
        
        leftSourceX  = gaugeConf.getNearestVerticalTrack  ( quadT.root.area,  leftSourceContact.getX(), 0 )
        leftSourceY  = gaugeConf.getNearestHorizontalTrack( quadT.root.area,  leftSourceContact.getY(), 0 )
        rightSourceX = gaugeConf.getNearestVerticalTrack  ( quadT.root.area, rightSourceContact.getX(), 0 )
        rightSourceY = gaugeConf.getNearestHorizontalTrack( quadT.root.area, rightSourceContact.getY(), 0 )
        leftX        = gaugeConf.getNearestVerticalTrack  ( quadT.root.area,        leftContact.getX(), 0 )
        rightX       = gaugeConf.getNearestVerticalTrack  ( quadT.root.area,       rightContact.getX(), 0 )
        tlY          = gaugeConf.getNearestHorizontalTrack( quadT.root.area,          tlContact.getY(), 0 )
        blY          = gaugeConf.getNearestHorizontalTrack( quadT.root.area,          blContact.getY(), 0 )
        
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

        for leaf in quadT.leafs:
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
        quadTree           = self.spares.quadTree
        quadTree.bufferTag = self.clockNet.getName()
        quadTree.ruseBuffer()
        with UpdateSession():
            self._rconnectHTree( quadTree )
            self._rrouteHTree  ( quadTree )

    def splitClock ( self ):
        """
        Disconnect the registers from the main clock and reconnect them to
        the leaf buffers of the clock tree.
        """
        quadTree           = self.spares.quadTree
        quadTree.bufferTag = self.clockNet.getName()

        hyperClock = HyperNet.create( Occurrence(self.clockNet) )
        for plugOccurrence in hyperClock.getTerminalNetlistPlugOccurrences():
            quadTree.attachToLeaf( plugOccurrence )
        quadTree.rsplitNetlist()

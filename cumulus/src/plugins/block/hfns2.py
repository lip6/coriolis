
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/hfns.py"                  |
# +-----------------------------------------------------------------+

"""
Manage High Fanout Net Synthesis (HFNS).
"""

import sys
import os.path
import re
from   operator             import itemgetter, attrgetter, methodcaller
from   ...                  import Cfg
from   ...Hurricane         import Breakpoint, DbU, Box, Transformation,           \
                                   Box, Path, Layer, Occurrence, Net, HyperNet,    \
                                   RoutingPad, Horizontal, Vertical, Contact, Pin, \
                                   Plug, Instance
from   ...CRL               import AllianceFramework, RoutingLayerGauge
from   ...helpers           import trace, l, u, n
from   ...helpers.io        import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay   import UpdateSession
from   ..                   import getParameter, utils
from   .configuration       import GaugeConf
from   .spares              import Spares


af = AllianceFramework.get()


# ----------------------------------------------------------------------------
# Class  :  "hfns.Cluster".

class Cluster ( object ):
    """
    Implementation of cluster of RoutingPads.
    """

    def __init__ ( self, bufferTree, leaf, anchor, depth ):
        self.depth         = depth
        self.bufferTree    = bufferTree
        self.leaf          = leaf
        self.anchors       = [ anchor ]
        self.area          = Box( anchor.getCenter() )

    @property
    def size ( self ): return len(self.anchors)

    def getCenter ( self ):
        return self.area.getCenter()

    def addAnchor ( self, anchor ):
        self.anchors.append( anchor )

    def createBuffer ( self ):
        """Perform buffer allocation in the leaf."""
        self.instBuffer = self.leaf.selectFree()
        if self.instBuffer is None:
            raise ErrorMessage( 2, 'Cluster.createBuffer(): No more free buffer in leaf {}.' \
                                   .format(self.leaf) )

    @property
    def bInputPlug ( self ):
        """The input Plug of the buffer."""
        return utils.getPlugByName( self.instBuffer, self.bufferTree.spares.conf.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the buffer."""
        return utils.getPlugByName( self.instBuffer, self.bufferTree.spares.conf.bufferConf.output )

    def createBufInputRp ( self, net ):
        """Create a RoutingPad for the buffer input Plug (terminal)."""
        return RoutingPad.create( net, Occurrence(self.bInputPlug), RoutingPad.BiggestArea )

    def createBufOutputRp ( self, net ):
        """Create a RoutingPad for the buffer output Plug (terminal)."""
        return RoutingPad.create( net, Occurrence(self.bOutputPlug), RoutingPad.BiggestArea )

    def splitNet ( self ):
        """
        Break the top net, re-attach the sinks of the net to the output of
        the buffer of the leaf.
        """
        spares  = self.bufferTree.spares
        netBuff = self.bufferTree.createSubNet()
        self.createBuffer()
        self.bOutputPlug.setNet( netBuff )
        trace( 550, ',+', '\tCluster.splitNet(), size:{} depth:{} driver:{}\n' \
                          .format(self.size,self.depth,netBuff.getName()) )
        trace( 550, '\tSplit: {}\n'.format(self.leaf) )
        if len(self.anchors):
            trace( 550, '\t| Left  :{}\n'.format(self.leaf.getLeft  ()) )
            trace( 550, '\t| Right :{}\n'.format(self.leaf.getRight ()) )
            trace( 550, '\t| Bottom:{}\n'.format(self.leaf.getBottom()) )
            trace( 550, '\t| Top   :{}\n'.format(self.leaf.getTop   ()) )
        if len(self.anchors) > 30:
            print( WarningMessage( 'Cluster of "{}" still has {} sinks.' \
                                   .format(netBuff.getName(),len(self.anchors)) ))
        for anchor in self.anchors:
            if isinstance(anchor,Cluster):
                trace( 550, '\tcluster input: "{}"\n'.format(netBuff) )
                anchor.bInputPlug.setNet( netBuff )
            else:
                plug        = anchor.getPlugOccurrence()
                deepPlug    = spares.raddTransNet( netBuff, plug.getPath() )
                deepNetBuff = deepPlug.getMasterNet() if deepPlug else netBuff
                trace( 550, '\tdeepNetBuf: "{}"\n'.format(deepNetBuff) )
                if isinstance(plug.getEntity(),Pin):
                    print( 'PIN, SKIPPED for {}'.format(deepNetBuff.getName()) )
                    continue
                plug.getEntity().setNet( deepNetBuff )
                anchor.destroy()
        trace( 550, ',-' )


# ----------------------------------------------------------------------------
# Class  :  "hfns.BufferTree".

class BufferTree ( object ):
    """
    Build a buffer tree for a high fanout net.
    """

    patVhdlVector = re.compile( r'(?P<name>.*)\((?P<index>\d+)\)' )

    def __init__ ( self, spares, net ):
        trace( 550, '\tBufferTree.__init__() on "{}".\n'.format(net.getName()) )
        self.spares       = spares
        self.net          = net
        self.isDeepNet    = True
        self.clusterDepth = 0
        self.clusters     = [ [] ]
        self.netCount     = 0
        self.netName      = self.net.getName()
        self.netIndex     = None
        m = BufferTree.patVhdlVector.match( self.net.getName() )
        if m:
            self.netName  = m.group('name')
            self.netIndex = m.group('index')
        trace( 550, '-' )

    def getLeafCluster ( self, leaf ):
        for cluster in self.clusters[0]:
            if cluster.leaf == leaf: return cluster
        return None

    def _addRpToCluster ( self, rp ):
        trace( 550, '\tBufferTree._addRpToCluster(): @{} {}\n' \
                    .format(rp.getPosition(),rp) )
        leaf    = self.spares.quadTree.getLeafUnder( rp.getPosition() )
        cluster = self.getLeafCluster( leaf )
        if cluster:
            cluster.addAnchor( rp )
        else:
            self.clusters[0].append( Cluster( self, leaf, rp, self.clusterDepth ) )
        trace( 550, '\tUsing leaf: {}\n'.format(leaf) )

    def createSubNet ( self ):
        """
        Create a new sub-net for a buffer driver. If the signal is a bit
        from a vector, unvectorize but keep a ``bitX`` tag in it. For example,
        the third (i.e. index 2) auxiliary signal for ``my_vector(3)`` will give
        ``my_vector_hfns_bit3_2``.
        """
        if self.netIndex is None:
            subNetName = '{}_hfns_{}'.format( self.netName, self.netCount )
        else:
            subNetName = '{}_bit{}_hfns_{}'.format( self.netName, self.netIndex, self.netCount )
        net = Net.create( self.spares.conf.cell, subNetName )
        self.netCount += 1
        return net

    def _buildLeafLevel ( self ):
        """
        Build clusters of RP by grouping them by the spare quad-tree leafs.
        """
        trace( 550, ',+', '\tBufferTree._buildLeafLevel() on "{}" ...\n'.format(self.net.getName()) )
        self.rpDriver   = None
        pinRp           = None
        for rp in self.net.getRoutingPads():
            rpOccurrence = rp.getPlugOccurrence()
            entity       = rpOccurrence.getEntity()
            if rpOccurrence.getPath().isEmpty():
                self.isDeepNet = False
            if isinstance(entity,Pin):
                pinRp = rp
                continue
            masterNet = entity.getMasterNet()
            if masterNet.getDirection() & Net.Direction.DirIn:
                self._addRpToCluster( rp )
            else:
                trace( 550, '\tDriver:{}.\n'.format(rp) )
                self.rpDriver = rp
        if pinRp:
            if self.rpDriver is None:
                trace( 550, '\tDriver (externa pin):{}.\n'.format(rp) )
                self.rpDriver = rp
            else:
                self._addRpToCluster( rp )
        trace( 550, '-' )

    def _buildTrunkLevel ( self ):
        trace( 550, ',+', '\tBufferTree._buildTrunkLevel() on "{}" ...\n'.format(self.net.getName()) )
        self.clusterDepth += 1
        trunkCluster = None
        for cluster in self.clusters[0]:
            if trunkCluster:
                trunkCluster.addAnchor( cluster )
                continue
            trunkCluster      = Cluster( self, None, cluster, self.clusterDepth )
            trunkCluster.leaf = self.spares.quadTree.getFreeLeafUnder( trunkCluster.area )
        self.clusters.append( [ trunkCluster ] )
        trace( 550, '-' )

    def _splitNet ( self ):
        """
        Perform the actual splitting of the net into sub-trees. Mostly calls
        ``Cluster.splitNet()`` then connect the top cluster root to the original
        signal.
        """
        for cluster in self.clusters[0]:
            cluster.splitNet()
        self.clusters[1][0].splitNet()
        if self.isDeepNet:
            # Must convert from a DeepNet into a real top Net to be saved.
            driverRpOcc = self.rpDriver.getPlugOccurrence()
            topNetName  = self.net.getName()
            self.net.destroy()
            self.net = Net.create( self.spares.conf.cell, topNetName )
            deepPlug      = self.spares.raddTransNet( self.net, driverRpOcc.getPath() )
            deepDriverNet = deepPlug.getMasterNet()
            driverRpOcc.getEntity().setNet( deepDriverNet )
            RoutingPad.create( self.net, driverRpOcc, RoutingPad.BiggestArea )
        self.clusters[1][0].createBuffer()
        self.clusters[1][0].createBufInputRp( self.net )
        trace( 550, '\tRoot input: {}\n'.format(self.clusters[1][0].bInputPlug) )

    def buildBTree ( self ):
        self._buildLeafLevel()
        self._buildTrunkLevel()
        self._splitNet()

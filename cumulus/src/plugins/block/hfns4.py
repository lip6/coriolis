
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

Fourth variant, quick and simple. Just break the net into subnets,
so each of them is under the fanout threshold. Basic method, do not
take into account the placement or the global wirelength.
"""

import sys
import os.path
import re
from   operator           import itemgetter, attrgetter, methodcaller
from   ...                import Cfg
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Box,  \
                                 Path, Layer, Occurrence, Net, HyperNet,     \
                                 RoutingPad, Horizontal, Vertical, Contact,  \
                                 Pin, Plug, Instance
from   ...CRL             import AllianceFramework, RoutingLayerGauge
from   ...helpers         import trace, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ..                 import getParameter, utils
from   .configuration     import GaugeConf
from   .spares            import Spares


af = AllianceFramework.get()


# ----------------------------------------------------------------------------
# Class  :  "hfns.Cluster".

class Cluster ( object ):
    """
    Cluster of routing pads, with one driver.
    """

    def __init__ ( self, bufferTree, depth ):
        self.bufferTree = bufferTree
        self.buffer     = None
        self.depth      = depth
        self.anchors    = [ ]
        self.childs     = [ ]
        self.parent     = None
        self.bInputRp   = None
        self.bOutputRp  = None
        trace( 550, '\tCluster.__init__(), depth={}\n'.format( self.depth ))

    @property
    def conf ( self ):
        return self.bufferTree.conf

    @property
    def bInputPlug ( self ):
        """The input Plug of the buffer."""
        return utils.getPlugByName( self.buffer, self.conf.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the buffer."""
        return utils.getPlugByName( self.buffer, self.conf.bufferConf.output )

    @property
    def size ( self ):
        return len(self.anchors)

    def isRoot ( self ): return self.parent is None

    def addAnchor ( self, anchor ):
        """Add an anchor. Can be RoutingPad (depth==0) or Cluster (depth>0)."""
        if isinstance(anchor,Cluster) and self.depth == 0:
            print( WarningMessage( 'Cluster.addAnchor(): Should not add a cluster at leaf level.' ) )
        elif isinstance(anchor,RoutingPad) and self.depth > 0:
            print( WarningMessage( 'Cluster.addAnchor(): Should not add a RoutingPad at non-leaf level.' ) )
        self.anchors.append( anchor )
        trace( 550, '\tCluster.addAnchor(), size={} anchor={}\n'.format( self.size, anchor ))

    def createBufInputRp ( self, net ):
        """Create a RoutingPad for the buffer input Plug (terminal)."""
        blockNet = self.createTransNet( net, Path(self.conf.icore) )
        self.bInputPlug.setNet( blockNet )
        self.bInputRp = RoutingPad.create( net
                                         , Occurrence( self.bInputPlug, Path(self.conf.icore) )
                                         , RoutingPad.BiggestArea )
        trace( 550, '\tCluster.createBufInputRp(): rp={}\n'.format(self.bInputRp) )
        return self.bInputRp

    def createBufOutputRp ( self, net ):
        """Create a RoutingPad for the buffer output Plug (terminal)."""
        blockNet = self.createTransNet( net, Path(self.conf.icore) )
        self.bOutputPlug.setNet( blockNet )
        self.bOutputRp = RoutingPad.create( net
                                          , Occurrence( self.bOutputPlug, Path(self.conf.icore) )
                                          , RoutingPad.BiggestArea )
        trace( 550, '\tCluster.createBufOutputRp(): rp={}\n'.format(self.bOutputRp) )
        return self.bOutputRp

    def setRootDriver ( self, net ):
        """Connect the top-level buffer input to the original signal."""
        if not self.isRoot():
            raise ErrorMessage( 2, 'Cluster.setRootDriver(): Must be called only on the top root cluster.' )
        self.createBufInputRp( net )

    def createBuffer ( self ):
        """Create the cluster's buffer."""
        self.buffer = self.conf.createBuffer( self.conf.cell )

    def createTransNet ( self, topNet, path ):
        trace( 550, '\tCluster.createTransNet(): topNet={}, path={}\n'.format(topNet,path) )
        bottomPlug = self.bufferTree.raddTransNet( topNet, path )
        bottomNet  = bottomPlug.getMasterNet() if bottomPlug else topNet
        trace( 550, '\tbottomNet: "{}"\n'.format(bottomNet) )
        return bottomNet

    def splitNet ( self ):
        """
        Perform the actual splitting of the net into subnets.
        """
        driverNet = self.bufferTree.createSubNet()
        self.createBuffer()
        self.createBufOutputRp( driverNet )
        trace( 550, ',+', '\tCluster.splitNet(), size:{} depth:{} driver:{}\n' \
                          .format(self.size,self.depth,driverNet.getName()) )
        if len(self.anchors) > 30:
            print( WarningMessage( 'Cluster.splitNet(): Top cluster of "{}" still has {} sinks.' \
                                   .format(driverNet.getName(),len(self.mergedAnchors)) ))
        for anchor in self.anchors:
            if isinstance(anchor,Cluster):
                anchor.createBufInputRp( driverNet )
            else:
                plug        = anchor.getPlugOccurrence()
                deepPlug    = self.bufferTree.raddTransNet( driverNet, plug.getPath() )
                deepNetBuff = deepPlug.getMasterNet() if deepPlug else driverNet
                trace( 550, '\tdeepNetBuf: "{}"\n'.format(deepNetBuff) )
                if isinstance(plug.getEntity(),Pin):
                    print( 'PIN, SKIPPED for {}'.format(deepNetBuff.getName()) )
                    continue
                plug.getEntity().setNet( deepNetBuff )
                anchor.destroy()
                rp = RoutingPad.create( driverNet, plug, RoutingPad.BiggestArea )
        for component in driverNet.getComponents():
            trace( 550, '\t| {}\n'.format(component) )
        trace( 550, ',-' )

    def show ( self ):
        """Select the RoutingPad of the cluster in the editor."""
        editor = self.bufferTree.spares.conf.editor
        if not editor: return False
        editor.unselectAll()
        editor.setCumulativeSelection( True )
        editor.setShowSelection( True )
        area = Box( self.area )
        area.inflate( l(10.0) )
        editor.reframe( area, False )
       #editor.select( self.anchor.getOccurrence() )
        for anchor in self.mergedAnchors:
            if isinstance(anchor,Cluster):
                continue
            else:
                editor.select( anchor.getOccurrence() )
        return True


# ----------------------------------------------------------------------------
# Class  :  "hfns.BufferTree".

class BufferTree ( object ):
    """
    Recursively break down a Net with a huge fanout. Works at pure netlist
    level, do not take placement or wirelength into account.
    """

    patVhdlVector = re.compile( r'(?P<name>.*)\((?P<index>\d+)\)' )

    def __init__ ( self, spares, net ):
        trace( 550, '\tBufferTree.__init__() on "{}".\n'.format(net.getName()) )
        self.spares       = spares
        self.net          = net
        self.isDeepNet    = True
        self.clusterDepth = 0
        self.clusters     = [ [] ]
        self.bufName      = self.conf.bufferConf.name
        self.netCount     = 0
        self.netName      = self.net.getName()
        self.netIndex     = None
        m = BufferTree.patVhdlVector.match( self.net.getName() )
        if m:
            self.netName  = m.group('name')
            self.netIndex = m.group('index')

    @property
    def conf ( self ):
        return self.spares.conf
            
    @property
    def root ( self ):
        """The root cluster of the tree (must be unique...)"""
        if len(self.clusters[-1]) != 1:
            raise ErrorMessage( 2, 'BufferTree.root: No, or multiple root for "{}".' \
                                   .format(self.net.getName()) )
        return self.clusters[-1][0]

    def createSubNet ( self ):
        """
        Create a new sub-net for a buffer driver. If the signal is a bit
        from a vector, unvectorize but keep a ``bitX`` tag in it. For example,
        the third (i.e. index 2) auxiliary signal for ``my_vector(3)`` will give
        ``my_vector_bit3_2``.
        """
        if self.netIndex is None:
            subNetName = '{}_hfns_{}'.format( self.netName, self.netCount )
        else:
            subNetName = '{}_bit{}_hfns_{}'.format( self.netName, self.netIndex, self.netCount )
        topCell = self.conf.cellPnR
        net     = Net.create( topCell, subNetName )
        self.netCount += 1
        return net

    def raddTransNet ( self, net, path ):
        return self.spares.raddTransNet( net, path )

    def rpartition ( self ):
        """
        Recursively partition the net.
        """
        trace( 550, ',+', '\tBufferTree.rpartition() on "{}" ...\n'.format(self.net.getName()) )
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
                if len(self.clusters[0]) == 0 or self.clusters[0][-1].size >= 10:
                    self.clusters[0].append( Cluster(self,self.clusterDepth) )
                self.clusters[0][-1].addAnchor( rp )
            else:
                trace( 550, '\tDriver:{}.\n'.format(rp) )
                self.rpDriver = rp
        if pinRp:
            if self.rpDriver is None:
                trace( 550, '\tDriver (external pin):{}.\n'.format(rp) )
                self.rpDriver = rp
            else:
                self.clusters[0][-1].addAnchor( pinRp )
        while len(self.clusters[-1]) > 1:
            for cluster in self.clusters[self.clusterDepth]:
                cluster.splitNet()
                if len(self.clusters) < self.clusterDepth+2:
                    self.clusters.append( [] )
                    self.clusters[-1].append( Cluster(self,self.clusterDepth+1) )
                if self.clusters[-1][-1].size >= 10:
                    self.clusters[-1].append( Cluster(self,self.clusterDepth+1) )
                self.clusters[-1][-1].addAnchor( cluster )
            self.clusterDepth += 1
        self.clusters[-1][-1].splitNet()
        trace( 550, '-' )

    def _splitNet ( self ):
        """
        Perform the actual splitting of the net into sub-trees. Mostly calls
        ``BufferTree.rpartition()`` then connect the top cluster root to the original
        signal.
        """
        self.rpartition()
        if self.isDeepNet:
            # Must convert from a DeepNet into a real top Net to be saved.
            topCell     = self.conf.corona if self.conf.isCoreBlock else self.conf.cell
            topNetName  = self.net.getName()
            driverRpOcc = self.rpDriver.getPlugOccurrence()
            self.net.destroy()
            self.net = Net.create( topCell, topNetName )
            deepPlug      = self.spares.raddTransNet( self.net, driverRpOcc.getPath() )
            deepDriverNet = deepPlug.getMasterNet()
            driverRpOcc.getEntity().setNet( deepDriverNet )
            rp = RoutingPad.create( self.net, driverRpOcc, RoutingPad.BiggestArea )
            trace( 550, '\tBufferTree._splitNet(): rp={}\n'.format(rp) )
        self.root.setRootDriver( self.net )
        trace( 550, '\tRoot input: {}\n'.format(self.root.bInputPlug) )

    def buildBTree ( self ):
        self._splitNet()

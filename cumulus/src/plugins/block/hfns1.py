
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
from   ...Hurricane         import Breakpoint, DbU, Box, Transformation, Box,          \
                                   Path, Layer, Occurrence, Net, HyperNet, RoutingPad, \
                                   Horizontal, Vertical, Contact, Pin, Plug, Instance
from   ...CRL               import AllianceFramework, RoutingLayerGauge
from   ...helpers           import trace, l, u, n
from   ...helpers.io        import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay   import UpdateSession
from   ..                   import getParameter, utils
from   .configuration       import GaugeConf
from   .spares              import Spares
from   .                    import timing


timing.staticInit()
af = AllianceFramework.get()


# ----------------------------------------------------------------------------
# Class  :  "hfns.SlicedArea".

class SlicedArea ( object ):
    """
    Perform the buffer creation and insertion for a Cluster. It can request
    a free buffer from the spare set or insert a new one directly in the
    design. The second option is still available but now unused, kept as
    code example (may be needed in the future).
    """

    REPLACE = 0x0001
    WEDGE   = 0x0002

    def __init__ ( self, cluster ):
        """
        Create the sliced area and perform an immediate buffer allocation
        from the spare set. Hint for a position indide of the cluster's area
        but closest to the parent's center area (so, ideally, on the cluster's
        edge).
        """
        state = cluster.bufferTree.spares.conf
        self.cluster    = cluster
        self.rows       = {}
        self.iposition  = None
        if cluster.parent is None:
            attractor = cluster.getCenter()
        else:
            attractor = cluster.parent.area.getCenter()
        self.instBuffer = cluster.bufferTree.spares.getFreeBufferUnder( cluster.area, attractor )

    @property
    def buffer ( self ):
        """The buffer instance."""
        return self.instBuffer

    @property
    def bInputPlug ( self ):
        """The input Plug of the buffer."""
        return utils.getPlugByName( self.buffer, self.cluster.bufferTree.spares.conf.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the buffer."""
        return utils.getPlugByName( self.buffer, self.cluster.bufferTree.spares.conf.bufferConf.output )

    def buildSlicesUnder ( self ):
        """
        UNUSED. Kept as reference example.
        Rebuild slices structure under a specific area (must be small).
        """
        state       = self.cluster.bufferTree.spares.conf
        sliceHeight = state.sliceHeight 
        cell        = state.cell
        cellAb      = cell.getAbutmentBox()
        insertArea  = Box( self.cluster.getCenter() )
        insertArea.inflate( l(50.0*3), 2*l(50.0) )
        for occurrence in cell.getTerminalNetlistInstanceOccurrencesUnder( insertArea ):
            instance   = occurrence.getEntity()
            masterCell = instance.getMasterCell()
            ab         = masterCell.getAbutmentBox()
            transf     = instance.getTransformation()
            occurrence.getPath().getTransformation().applyOn( transf )
            transf.applyOn( ab )
            y = (ab.getYMin() - cellAb.getYMin()) // sliceHeight
            if (ab.getYMin() - cellAb.getYMin()) % sliceHeight:
                print( ErrorMessage( 1, 'SlicedArea.__init__(): Misaligned {}.'.format(occurrence) ))
                continue
            if not y in self.rows:
                self.rows[y] = []
            row = self.rows[ y ]
            row.append( (occurrence,ab) )
        for row in self.rows.values():
            row.sort( key=lambda v: v[1].getXMin() )

    def findBufferSite ( self ):
        """
        UNUSED. Kept as reference example.
        Analyse the slices for spaces and holes into which insert a buffer.
        Look for hole big enough (REPLACE case) or if we do need to shift
        the cells to create one (WEDGE case).
        """
        global af
        catalog           = af.getCatalog()
        bufferLength      = self.cluster.bufferTree.spares.conf.bufferConf.width
        for key in self.rows.keys():
            row = self.rows[ key ]
            trace( 550, '\t+ Row:\n' )
            tieLength         = 0
            holeLength        = 0
            biggestHoleLength = 0
            contiguousTie     = False
            ihole             = 0
            for i in range(len(row)):
                occurrence, ab = row[i]
                masterCell     = occurrence.getEntity().getMasterCell()
                catalogState   = catalog.getState( masterCell.getName() )
                if catalogState.isFeed():
                    trace( 550, '\t| Feed:{}\n'.format(occurrence) )
                    cellLength    = masterCell.getAbutmentBox().getWidth()
                    tieLength    += cellLength
                    holeLength   += cellLength
                    contiguousTie = True
                    if holeLength > biggestHoleLength:
                        biggestHoleLength = holeLength
                        if holeLength == cellLength:
                            ihole = i
                else:
                    holeLength    = 0
                    contiguousTie = False
            if bufferLength <= tieLength:
                trace( 550, '\tbufferLength:{} tieLength:{} biggestHole:{}\n' \
                            .format( DbU.getValueString(bufferLength)
                                   , DbU.getValueString(tieLength)
                                   , DbU.getValueString(biggestHoleLength) ))
                if bufferLength <= biggestHoleLength:
                    trace( 550, '\tHole is big enough, REPLACE\n' )
                    self.iposition = (key, ihole, SlicedArea.REPLACE)
                else:
                    trace( 550, '\tNeeds wedging, WEDGE\n' )
                    self.iposition = (key, ihole, SlicedArea.WEDGE)
                return True
        return False

    def insertBuffer ( self ):
        """
        UNUSED. Kept as reference example.
        Insert a new buffer instance inside the slice area. Uses the informations
        gathered by ``findBufferSite()`` (where to REPLACE or WEDGE).
        """
        if self.iposition is None:
            raise ErrorMessage( 2, 'SlicedArea.insertBuffer(): No position defined to wedge the buffer.' )
        state        = self.cluster.bufferTree.spares.conf
        catalog      = af.getCatalog()
        bufferLength = self.cluster.bufferTree.spares.conf.bufferConf.width
        tieLength    = 0
        transf       = None
        if self.iposition[2] & SlicedArea.REPLACE:
            row = self.rows[ self.iposition[0] ]
            for i in range(self.iposition[1],len(row)):
                occurrence, ab = row[i]
                tieLength     += ab.getWidth()
                tieInstance    = occurrence.getEntity()
                masterCell     = tieInstance.getMasterCell()
                catalogState   = catalog.getState( masterCell.getName() )
                if not catalogState.isFeed():
                    raise ErrorMessage( 2, 'SlicedArea.insertBuffer(): Not a feed cell under wedge position.' )
                if transf is None:
                    transf = tieInstance.getTransformation()
                tieInstance.destroy()
                if tieLength >= bufferLength:
                    break
        self.instBuffer = state.createBuffer()
        self.instBuffer.setTransformation( transf )
        self.instBuffer.setPlacementStatus( Instance.PlacementStatus.FIXED )
        trace( 550, '\tWedged: {} @{}\n'.format(self.instBuffer,transf) )

    def display ( self ):
        """Display the orderded instances under the sliced area."""
        for key in self.rows.keys():
            print( 'Row @{}:'.format(key) )
            row = self.rows[ key ]
            for occurrence, ab in row:
                print( '| {} <- {}'.format(ab,occurrence) )


# ----------------------------------------------------------------------------
# Class  :  "hfns.Cluster".

class Cluster ( object ):
    """
    Implementation of cluster of RoutingPads. This is a disjoint-set data
    structure (ref. https://en.wikipedia.org/wiki/Disjoint-set_data_structure).

    We manage two kind of trees, do not mistake them:

    1. The cluster's own tree, that is, the union set. ``self.root`` and
       ``self.parent`` belongs to that structure.
    2. The tree *of* Clusters. Recursive functions like ``rsplitNet()``
       and ``rcreateBuffer()`` belongs to that super-tree.

    The ``snapshot()`` and ``rrebindRp()`` are kept just in case. They allow
    to keep the cluster partitionning between iterations of the placer by
    replacing RoutingPad (which get erased) by Plug occurrences which are
    stables.
    """

    def __init__ ( self, bufferTree, anchor, depth ):
        self.depth         = depth
        self.bufferTree    = bufferTree
        self.anchor        = anchor
        self.mergedAnchors = [ anchor ]
        self.parent        = None
        self.rank          = 0
        self.size          = 1
        self.area          = Box( anchor.getCenter() )

    def __str__ ( self ):
        parentId = 'None' if self.parent is None else str(self.parent.id)
        s = '<Cluster d:{} par:{} id:{} sz:{} area:{}x{}>' \
            .format( self.depth
                   , parentId
                   , self.id
                   , self.size
                   , DbU.getValueString(self.area.getWidth())
                   , DbU.getValueString(self.area.getHeight()) )
        return s

    def __cmp__ ( self, other ):
        if other is None: return 1
        if self.id < other.id: return -1
        if self.id > other.id: return  1
        return 0

    @property
    def buffer ( self ):
        """The buffer instance (proxy to slicedArea)."""
        return self.slicedArea.buffer

    @property
    def bInputPlug ( self ):
        """The input Plug of the buffer (proxy to slicedArea)."""
        return self.slicedArea.bInputPlug 

    @property
    def bOutputPlug ( self ):
        """The output Plug of the buffer (proxy to slicedArea)."""
        return  self.slicedArea.bOutputPlug

    @property
    def id ( self ):
        if self.anchor is None: return 0
        return self.anchor.getId()

    def getId ( self ):
        return self.id

    def isRoot ( self ): return self.parent is None

    def getCenter ( self ):
        return self.area.getCenter()

    def getRoot ( self ):
        """Find the root, performing simple path compression as it goes."""
       #trace( 550, ',+', '\tCluster.getRoot() of id:{}\n'.format(self.id) )
        root = self
       #trace( 550, '\t+ Finding root:\n' )
        while root.parent is not None:
            root = root.parent
           #trace( 550, '\t| id:{}\n'.format(root.id) )
        node = self
       #trace( 550, '\t+ Compressing path:\n' )
        while node.parent is not None:
            pnode       = node.parent
            node.parent = root
            node        = pnode
           #trace( 550, '\t| id:{}\n'.format(node.id) )
       #trace( 550, ',-', '\t> Root of id:{} is id:{}\n'.format(self.id,root.id) )
        return root

    def merge ( self, other ):
        """Union by rank."""
       #trace( 550, ',+', '\tCluster.merge() id:{} with id:{}\n' \
       #       .format(self.id,other.id) )
        root1 = self.getRoot()
        root2 = other.getRoot()
        if root1 != root2:
            if root1.rank < root2.rank:
                root1, root2 = root2, root1
            if root1.rank != root2.rank:
                root1.rank += 1
            root1.area.merge( root2.area )
            root1.size          += root2.size
            root1.mergedAnchors += root2.mergedAnchors
            root2.parent         = root1
           #trace( 550, ',-', '\tMerge id:{} <= id:{} done\n' \
           #            .format(root1.id,root2.id) )
        else:
            pass
           #trace( 550, ',-', '\tMerge id:{} and id:{} already done\n' \
           #            .format(root1.id,root2.id) )
        return root1

    def snapshot ( self ):
        """
        UNUSED. Kept as reference example.
        Replace the RoutingPad by their occurrences (in place).
        This operation is needed to save the cluster information between
        two runs as RoutingPad will get destroyed/re-created. However,
        the Plug occurrence will remains valid (and stable).
        """
        if isinstance(self.anchor,RoutingPad):
            self.anchor = self.anchor.getPlugOccurrence()
        mergedAnchors = []
        for anchor in self.mergedAnchors:
            if isinstance(anchor,RoutingPad):
                mergedAnchors.append( anchor.getPlugOccurrence() )
                trace( 550, '\t| snapshot:{}\n'.format(anchor.getPlugOccurrence()) )
            else:
                mergedAnchors.append( anchor )
                anchor.snapshot()
        self.mergedAnchors = mergedAnchors

    def rrebindRp ( self, rp ):
        """
        UNUSED. Kept as reference example.
        Associate a RoutingPad ``rp`` to a cluster. This is done by
        matching the plug occurrence of the RoutingPad. The ``snapshot()``
        method must have been called before.
        """
        trace( 550, ',+', '\tCluster.rrebindRp() {}\n'.format(rp.getPlugOccurrence()) )
        bound   = False
        plugOcc = rp.getPlugOccurrence()
        if plugOcc == self.anchor:
            bound = True
            self.anchor = rp
            self.mergedAnchors[0] = rp
            trace( 550, '\t> Bound to: {}\n'.format(self) )
        else:
            trace( 550, '\t+ mergedAnchor: {}\n'.format(len(self.mergedAnchors)) )
            if len(self.mergedAnchors):
                for i in range(len(self.mergedAnchors)):
                    trace( 550, '\t| compare:[{:2}] {}\n'.format(i,self.mergeAnchors[i]) )
                    if plugOcc == self.mergeAnchors[i]:
                        self.mergedAnchors[i] = rp
                        bound = True
                        trace( 550, '\t> Bound to: {}\n'.format(self) )
                        break
        if not bound and self.mergedAnchors is not None:
            for cluster in self.mergedAnchors:
                if isinstance(cluster,Cluster):
                    bound = cluster.rrebindRp(rp)
                    if bound: break
        trace( 550, '-' )
        return bound

    def createBufInputRp ( self, net ):
        """Create a RoutingPad for the buffer input Plug (terminal)."""
        return RoutingPad.create( net, Occurrence(self.bInputPlug), RoutingPad.BiggestArea )

    def createBufOutputRp ( self, net ):
        """Create a RoutingPad for the buffer output Plug (terminal)."""
        return RoutingPad.create( net, Occurrence(self.bOutputPlug), RoutingPad.BiggestArea )

    def setRootDriver ( self, net ):
        """Connect the top-level buffer input to the original signal."""
        if not self.isRoot():
            raise ErrorMessage( 2, 'Cluster.setRootDriver(): Must be called only on the top root cluster.' )
        self.createBufInputRp( net )

    def createBuffer ( self ):
        """Create the SlicedArea which will create/insert the buffer of the cluster."""
        if not self.isRoot():
            raise ErrorMessage( 2, 'Cluster.createBuffer(): Only root cluster should have buffer.' )
        self.slicedArea = SlicedArea( self )

    def rcreateBuffer ( self ):
        """Recursively call ``createBuffer()`` on the whole cluster hierarchy."""
        self.createBuffer()
        for anchor in self.mergedAnchors:
            if isinstance(anchor,Cluster):
                anchor.rcreateBuffer()

    def rsplitNet ( self ):
        """
        Perform the actual splitting of the net into subnets. This is a
        recursive function. One driver net will be created by cluster.
        """
        if not self.isRoot():
            raise ErrorMessage( 2, 'Cluster.connect(): Only root cluster should be connecteds.' )
        spares  = self.bufferTree.spares
        netBuff = self.bufferTree.createSubNet()
        self.bOutputPlug.setNet( netBuff )
        trace( 550, ',+', '\tCluster.rsplitNet(), size:{} depth:{} driver:{}\n' \
                          .format(self.size,self.depth,netBuff.getName()) )
        if len(self.mergedAnchors) > 30:
            print( 'Top cluster of "{}" still has {} sinks.' \
                   .format(netBuff.getName(),len(self.mergedAnchors)) )
        for anchor in self.mergedAnchors:
            if isinstance(anchor,Cluster):
                trace( 550, '\tcluster input: "{}"\n'.format(netBuff) )
                anchor.bInputPlug.setNet( netBuff )
                anchor.rsplitNet()
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
        if netBuff.getName().startswith('abc_75177_new_n3292'):
            trace( 550, '\tNet {}:\n'.format(netBuff.getName()) )
            count = 0
            for component in netBuff.getComponents():
                trace( 550, '\t[{:2}] {}\n'.format(count,component) )
                count += 1
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
# Class  :  "hfns.Edge".

class Edge ( object ):
    """
    Define an Edge between two Clusters. The lenght of the Edge is the
    Manhattan distance of the two centers of the cluster's areas.
    So, as Clusters grows, so does the area and the length of the
    edge change over time. To work on a stable value, the initial
    distance is cached in the ``length`` attribute.
    """

    def __init__ ( self, source, target ):
        self.source = source
        self.target = target
        self.length = self.clusterLength

    @property
    def clusterLength ( self ):
        """
        Manhattan distance, cluster center to cluster center.
        The actual one, not the ``length`` initial cached value.
        """
        sourceCenter = self.source.getCenter()
        targetCenter = self.target.getCenter()
        return targetCenter.manhattanDistance( sourceCenter )

    def __cmp__ ( self, other ):
        """Comparison over the cached initial length value."""
        if self.length < other.length: return -1
        if self.length > other.length: return  1
        return 0


# ----------------------------------------------------------------------------
# Class  :  "hfns.BufferTree".

class BufferTree ( object ):
    """
    Build a buffer tree for a high fanout net. Recursively build clusters
    using Kruskal algorithm (https://en.wikipedia.org/wiki/Kruskal's_algorithm).
    All subnet are created at the design top level (like for clock tree)
    so they are not ``DeepNet``, the driver itself is pulled up to the top
    level if needs be.
    """

    patVhdlVector = re.compile( r'(?P<name>.*)\((?P<index>\d+)\)' )

    def __init__ ( self, spares, net ):
        trace( 550, '\tBufferTree.__init__() on "{}".\n'.format(net.getName()) )
        self.spares       = spares
        self.net          = net
        self.isDeepNet    = True
        self.clusterDepth = 0
        self.clusters     = [ [] ]
        self.bufName      = self.spares.conf.bufferConf.name
        self.netCount     = 0
        self.netName      = self.net.getName()
        self.netIndex     = None
        m = BufferTree.patVhdlVector.match( self.net.getName() )
        if m:
            self.netName  = m.group('name')
            self.netIndex = m.group('index')

    @property
    def root ( self ):
        """The root cluster of the tree (must be unique...)"""
        if len(self.clusters[-1]) != 1:
            raise ErrorMessage( 2, 'BufferTree.root: No, or multiple root for "{}".' \
                                   .format(self.net.getName()) )
        return self.clusters[-1][0]

    @property
    def edgeLimit ( self ):
        """
        Maximum length of Edge to consider. Edges above this threshold will be
        pruned from the set given to Kruskal.
        """
        levelFactor = 1
        if   self.clusterDepth == 0: timing.tech.getWlEstimate( self.bufName, 1 )
        else:                        levelFactor = 4*self.clusterDepth
        return levelFactor*l(700)

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
        net = Net.create( self.spares.conf.cell, subNetName )
        self.netCount += 1
        return net

    def canMerge ( self, clusterA, clusterB ):
        """
        Control the merge criterion between two clusters. For now we check
        that the number of sinks is below 30 and the half-perimeter is not
        too great (see ``edgeLimit``).
        """
        if self.clusterDepth == 0:
            maxWL = timing.tech.getWlEstimate( self.bufName, clusterA.size+clusterB.size )
            area = Box( clusterA.area )
            area.merge( clusterB.area )
            hpWL = (area.getWidth() + area.getHeight()) // 2
            if hpWL >= maxWL:
                return True
                trace( 550, '\t> Reject merge: hpWL >= maxWL ({} >= {}).\n' \
                            .format(DbU.getValueString(hpWL),DbU.getValueString(maxWL)) )
            return True
        
        if clusterA.size + clusterB.size > 30:
            trace( 550, '\t> Reject merge, over size threshold of 30.\n' )
            return False
        area = Box( clusterA.area )
        area.merge( clusterB.area )
        hpwl = (area.getWidth() + area.getHeight()) // 2
        if hpwl > 2*self.edgeLimit:
            trace( 550, '\t> Reject merge, over HPWL threshold of 2*{}.\n' \
                        .format(DbU.getValueString(self.edgeLimit)))
            return False
        else:
            trace( 550, '\t> Accepted merge, future area is {}x{}.\n' \
                        .format( DbU.getValueString(area.getWidth ())
                               , DbU.getValueString(area.getHeight()) ))
        return True

    def doKruskal ( self ):
        """
        Do Kruskal algorithm. We do not perform a complete Krukal as
        *too long* edges are pruned and we do not keep tracks of edges,
        we just want a cluster of close RoutingPad, not a minimum
        spanning tree.
        """
        trace( 550, ',+', '\tBufferTree.doKruskal()\n' )
        trace( 550, '\tBuilding edges, max length:{} ...\n'.format(DbU.getValueString(self.edgeLimit)) )
        clusters = self.clusters[-1]
        edges    = []
        for i in range(len(clusters)):
            for j in range(i+1,len(clusters)):
                edge = Edge( clusters[i], clusters[j] )
                if edge.length < self.edgeLimit:
                    edges.append( edge )
        trace( 550, '\tSorting {} edges ...\n'.format(len(edges)) )
        edges.sort( key=attrgetter('length') )
        trace( 550, '\tProcessing edges ...\n' )
        clustersCount = len(clusters)
        for i in range(len(edges)):
            edge = edges[i]
            trace( 550, '\t| Process [{:3d}], length:{} clusterLength:{}\n' \
                        .format( i, DbU.getValueString(edge.length)
                                  , DbU.getValueString(edge.clusterLength)) )
            sourceRoot = edge.source.getRoot()
            targetRoot = edge.target.getRoot()
            if sourceRoot == targetRoot:
                continue
            if not self.canMerge(sourceRoot,targetRoot):
                continue
            sourceRoot.merge( targetRoot )
            trace( 550, '\t> Merged cluster: {}\n'.format(sourceRoot.getRoot()) )
            clustersCount -= 1
        trace( 550, '\tClusters count: {}\n'.format(clustersCount) )
        for cluster in clusters:
            if cluster.isRoot():
                trace( 550, '\t | Cluster, size:{}, area:{} x {} {}\n' \
                            .format( cluster.size
                                   , DbU.getValueString(cluster.area.getWidth())
                                   , DbU.getValueString(cluster.area.getHeight())
                                   , cluster.area ) ) 
        trace( 550, '-' )
        return clustersCount

    def _rclusterize ( self ):
        """
        Recursively performs the Kruskal algorithm until only *one* root
        cluster remains. First level is clusters of RoutingPad, then
        clusters of clusters.
        """
        trace( 550, ',+', '\tBufferTree.buildBTree() on "{}" ...\n'.format(self.net.getName()) )
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
                self.clusters[0].append( Cluster(self,rp,self.clusterDepth) )
            else:
                trace( 550, '\tDriver:{}.\n'.format(rp) )
                self.rpDriver = rp
        if pinRp:
            if self.rpDriver is None:
                trace( 550, '\tDriver (externa pin):{}.\n'.format(rp) )
                self.rpDriver = rp
            else:
                self.clusters[0].append( Cluster(self,pinRp,self.clusterDepth) )
        while len(self.clusters[-1]) > 1:
            self.doKruskal()
            self.clusters.append( [] )
            for cluster in self.clusters[-2]:
                if cluster.isRoot():
                    self.clusters[-1].append( Cluster(self,cluster,self.clusterDepth+1) )
                   #if cluster.show():
                   #    Breakpoint.stop( 0, 'Showing cluster of {} RoutingPads'.format(cluster.size) )
            editor = self.spares.conf.editor
            if editor:
                editor.unselectAll()
                editor.setCumulativeSelection( False )
                editor.setShowSelection( False )
            self.clusterDepth += 1
        trace( 550, '-' )

    def snapshot ( self ):
        """UNUSED. Kept as reference example. See ``Cluster.snapshot()``."""
        if not self.root:
            raise ErrorMessage( 2, 'BufferTree.snapshot(): Clusters must be built first.' )
        self.root.snapshot()

    def _rcreateBuffer ( self ):
        """Proxy to ``Cluster.rcreateBuffer()``."""
        if not self.root:
            raise ErrorMessage( 2, 'BufferTree.rcreateBuffer(): Clusters must be built first.' )
        self.root.rcreateBuffer()

    def _splitNet ( self ):
        """
        Perform the actual splitting of the net into sub-trees. Mostly calls
        ``Cluster.rsplitNet()`` then connect the top cluster root to the original
        signal.
        """
        if not self.root:
            raise ErrorMessage( 2, 'BufferTree.splitNet(): Clusters must be built first.' )
        self.root.rsplitNet()
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
        self.root.setRootDriver( self.net )
        trace( 550, '\tRoot input: {}\n'.format(self.root.bInputPlug) )

    def buildBTree ( self ):
        self._rclusterize()
        self._rcreateBuffer()
        self._splitNet()


# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/block/rsmt.py"                  |
# +-----------------------------------------------------------------+

"""
Rectilinear Steiner Minimum Spaning Tree (RSMT) Module.

References:

.. [1] A. B. Kahng and G. Robins. A new class of iterative steiner tree
       heuristics with good performance. IEEE Transactions Computer-Aided
       Design, 11(7):893-902, July 1992.

.. [2] A. B. Kahng and G. Robins. On Optimal Interconnections for VLSI.
       Kluwer Academic Publishers, Boston, MA, 1995.

.. [3] Gabriel Robins and Alexander Zelikovsky.
       Minimum Steiner Tree Construction, *reference for myself*.
"""

import sys
from   ...           import Cfg, Flute
from   ...Hurricane  import DbU, Box, Point, Path, Occurrence, Breakpoint,     \
                            DataBase, UpdateSession, Net, RoutingPad, Contact, \
                            Horizontal, Vertical, Instance
from   ...CRL        import RoutingLayerGauge
from   ...helpers    import trace
from   ...helpers.io import ErrorMessage, WarningMessage
from   ...           import plugins


# ----------------------------------------------------------------------------
# Class  :  "rsmt.Node".

class Node ( object ):

    GraphPoint   = 0x0001
    SteinerPoint = 0x0002
    KeepPoint    = 0x0004
    Driver       = 0x0008

    @staticmethod
    def showNodes ( nodes ):
        for node in nodes:
            trace( 542, '\t| {}\n'.format(node) )

    def __init__ ( self, cluster, x, y, flags=0 ):
        self.cluster  = cluster
        self.edges    = []
        self.position = Point( x, y )
        self._back    = None
        self.distance = DbU.fromLambda( 100000.0 )
        self.flags    = flags
        if cluster: self.flags = self.flags|Node.GraphPoint|Node.KeepPoint
        else:       self.flags = self.flags|Node.SteinerPoint
        self.gcontact = None
        return

    def __cmp__ ( self, other ):
        return self.distance - other.distance

    @property
    def x ( self ):
        return self.position.getX()

    @property
    def y ( self ):
        return self.position.getY()

    def __str__ ( self ):
        return '<Node D:{} @[{} {}] d:{} {}>'.format( self.degree
                                                    , DbU.getValueString(self.x)
                                                    , DbU.getValueString(self.y)
                                                    , DbU.getValueString(self.distance)
                                                    , self.cluster )

    @property
    def degree ( self ):
        return len(self.edges)

    def setFlags ( self, flags ):
        self.flags = self.flags | flags

    def unsetFlags ( self, flags ):
        self.flags = self.flags & ~flags

    def addEdge ( self, edge ):
        self.edges.append( edge )

    def delEdge ( self, edge ):
        for i in range(len(self.edges)):
            if self.edges[i] == edge:
                del self.edges[i]

    def isSame ( self, other ):
        return id(self) == id(other)

    def update ( self, node ):
        distance = self.position.manhattanDistance( node.position )
        if distance < self.distance:
            self.distance = distance
            self.back     = node
            return True
        return False

    def check ( self ):
        if self.degree > 4:
            raise ErrorMessage( 2, [ 'Node.check(): Degree of node is over 4,'
                                   , 'On {}.'.format(self) ] )
        if self.degree == 0:
            raise ErrorMessage( 2, [ 'Node.check(): Degree of node is zero (unconnected),'
                                   , 'On {}.'.format(self) ] )
        if self.cluster is None:
            if self.degree < 2:
                raise ErrorMessage( 2, [ 'Node.check(): Degree of Steiner node must be between 2 and 4,'
                                       , 'On {}.'.format(self) ] )

    def createGContact ( self, net ):
        global gcut
        side = DbU.fromLambda( 1.0 )
        self.gcontact = Contact.create( net, gcut, self.x, self.y, side, side )
        trace( 542, '+ {}'.format(self) )
        trace( 542, '| {}'.format(self.gcontact) )
        if self.flags & Node.GraphPoint:
            if self.flags & Node.Driver:
                rp = self.cluster.bOutputRp
            else:
                rp = self.cluster.bInputRp
            rp.getBodyHook().attach( self.gcontact.getBodyHook() )
            trace( 542, '| {}'.format(rp) )


# ----------------------------------------------------------------------------
# Class  :  "rmst.Edge".

class Edge ( object ):

    def __init__ ( self, source, target ):
        self.source = source
        self.target = target
        self.length = self.source.position.manhattanDistance( self.target.position )
        self.source.addEdge( self )
        self.target.addEdge( self )

    def __del__ ( self ):
        self.source.delEdge( self )
        self.target.delEdge( self )

    def __str__ ( self ):
        return '<Edge S:[{} {}] T:[{} {}] len:{}>' \
               .format( DbU.getValueString(self.source.x)
                      , DbU.getValueString(self.source.y)
                      , DbU.getValueString(self.target.x)
                      , DbU.getValueString(self.target.y)
                      , DbU.getValueString(self.length)
                      )

    def isHorizontal ( self ):
        return self.source.y == self.target.y

    def isVertical ( self ):
        return self.source.x == self.target.x

    def isDiagonal ( self ):
        return  (self.source.x != self.target.x) \
            and (self.source.y != self.target.y) \

    def _createGHorizontal ( self, source, target, y ):
        global gmetalh
        DbU.fromLambda( 2.0 )
        if source.getPosition().getX() > target.getPosition().getX():
            source, target = target, source
        return Horizontal.create( source, target, gmetalh, y, DbU.fromLambda(2.0) ) 

    def _createGVertical ( self, source, target, x ):
        global gmetalv
        if source.getPosition().getY() > target.getPosition().getY():
            source, target = target, source
        return Vertical.create( source, target, gmetalv, x, DbU.fromLambda(2.0) ) 

    def createGSegment ( self, net ):
        global gcut
        side = DbU.fromLambda( 1.0 )
        segments = []
        if self.isDiagonal():
            turn = Contact.create( net, gcut, self.target.x, self.source.y, side, side )
            segments.append( self._createGHorizontal( self.source.gcontact
                                                    , turn
                                                    , self.source.y ) )
            segments.append( self._createGVertical( turn
                                                  , self.target.gcontact
                                                  , self.target.x ) )
        elif self.isHorizontal():
            segments.append( self._createGHorizontal( self.source.gcontact
                                                    , self.target.gcontact
                                                    , self.source.y ) )
        else:
            segments.append( self._createGVertical( self.source.gcontact
                                                  , self.target.gcontact
                                                  , self.source.x ) )
        return segments


# ----------------------------------------------------------------------------
# Class  :  "rmst.geoKey".

class GeoKey ( object ):

    def __init__ ( self, x, y ):
        self.x = x
        self.y = y

    def __cmp__ ( self, other ):
        if self.x != other.x: return self.x - other.x 
        if self.y != other.y: return self.y - other.y 
        return 0

    def __hash__ ( self ):
        key =  self.x + self.y
        key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9;
        key = (key ^ (key >> 27)) * 0x94d049bb133111eb;
        key =  key ^ (key >> 31);
        return key;

    def __str__ (self ):
        return '<GeoKey [{} {}]>'.format( DbU.getValueString(self.x)
                                        , DbU.getValueString(self.y) )


# ----------------------------------------------------------------------------
# Class  :  "rmst.Graph".

class Graph ( object ):

    def __init__ ( self, net, name=None ):
        self.nodesLUT = {}
        self.edges    = []
        self.length   = 0
        self.net      = net
        self._name    = name

    def __len__ ( self ):
        return self.length

    @property
    def name ( self ):
        return self.net.getName() if self._name is None else self._name

    @property
    def nodes ( self ):
        return self.nodesLUT.values()

    def addNode  ( self, cluster, x, y, flags=0 ):
        node = Node( cluster, x, y, flags )
        self.nodesLUT[ GeoKey(x,y) ] = node
        return node

    def copyNode ( self, node ):
        self.addNode( node.cluster, node.x, node.y, node.flags )

    def setNodes ( self, nodes ):
        self.__init__( self.net, self.name )
        for node in nodes:
            self.copyNode( node )

    def lookupNode ( self, x, y ):
        geoKey = GeoKey( x, y )
        if geoKey in self.nodesLUT:
            return self.nodesLUT[geoKey]
        return None

    def lookupOrAddNode ( self, x, y ):
        node = self.lookupNode( x, y )
        if node is None:
            node = self.addNode( None, x, y )
            node = self.lookupNode( x, y )
        return node

    def showNodes ( self ):
        trace( 542, '+,+', '\tGraph "{}" nodes:\n'.format(self.name) )
        for node in self.nodes:
            trace( 542, '\t| {}\n'.format(node) )
        trace( 542, '--' )

    def showEdges ( self ):
        trace( 542, '+,+', '\tGraph "{}" Edges:\n'.format(self.name) )
        for i in range(len(self.edges)):
            trace( 542, '\t[{:3}] {}\n'.format(i,self.edges[i]) )
        trace( 542, '--' )


# ----------------------------------------------------------------------------
# Class  :  "rsmt.RMST".

class RMST ( Graph ):
    """
    Build a Rectilinear Minimum Spaning Tree (RMST) using Prim's algorithm.
    The graph structure is supplied by the Graph base class.
    """

    def __init__ ( self, net, name=None ):
        Graph.__init__( self, net, name )

    def reinit ( self ):
        self.edges  = []
        self.length = 0

    def doPrim ( self ):
        self.reinit()
        # Special case a graph of one or two nodes only.
        if len(self.nodes) <  2: return
        if len(self.nodes) == 2:
            self.edges.append( Edge( self.nodes[0], self.nodes[1] ) )
            self.length = self.edges[0].length
            return
        
        trace( 542, '+' )
        unreacheds = []
        self.nodes[0]._distance = 0
        for node in self.nodes[1:]:
            node.update( self.nodes[0] )
            unreacheds.append( node )
        unreacheds.sort()
        trace( 542, '\tPrim "{}" (initial stack)\n'.format(self.name) )
        trace( 542, '\t+ S {}\n'.format(self.nodes[0]) )
        Node.showNodes( unreacheds )
        
        while len(unreacheds):
            nearest = unreacheds.pop(0)
            self.edges.append( Edge( nearest, nearest.back ) )
            trace( 542, '\tAdding {}\n'.format(self.edges[-1]) )
            for node in unreacheds:
                node.update( nearest )
            unreacheds.sort()
            trace( 542, '\tPrim "{}" (current stack)\n'.format(self.name) )
            trace( 542, '\tS {}\n'.format(self.nodes[0]) )
            Node.showNodes( unreacheds )
        
        for edge in self.edges:
            self.length += edge.length
        
        trace( 542, '-' )


# ----------------------------------------------------------------------------
# Class  :  "rsmt.RSMT".

class RSMT ( Graph ):

    def __init__ ( self, net, name=None ):
        Graph.__init__( self, net, name )
        self._hananNodes = []

    def _computeHanan ( self ):
        xs     = []
        ys     = []
        for node in self.nodes:
            if not node.x in xs: xs.append( node.x )
            if not node.y in xs:
                ys.append( node.y )
        xs.sort()
        ys.sort()
        
        trace( 542, '\tHanan matrix: {}x{}\n'.format(len(xs),len(ys)) )
        
        self._hananNodes = []
        for x in xs:
            for y in ys:
                isHanan = True
                for node in self.nodes:
                    if node.x == x and node.y == y:
                        isHanan = False
                        break
                if not isHanan: continue
                self._hananNodes.append( Node( None, x, y ) )

    def addNode  ( self, cluster, x, y, flags=0 ):
        node = Graph.addNode( self, cluster, x, y, flags )
        trace( 542, '\tNew Node: {}\n'.format(node) )
        return node

    def doFlute ( self ):
        trace( 542, ',+', '\tRSMT.doFlute() on "{}".\n'.format(self.net.getName()) )
        self.edges  = []
        self.length = 0
        
        if len(self.nodes) <  2: return
        if len(self.nodes) == 2:
            self.edges.append( Edge( self.nodes[0], self.nodes[1] ) )
            self.length = self.edges[0].length
            return

        points = []
        for node in self.nodes:
            points.append( (node.x,node.y) )
        tree = Flute.flute( points )
        for i in range(len(tree)):
            j      = tree[i][0]
            source = self.lookupOrAddNode( tree[i][1], tree[i][2] )
            target = self.lookupOrAddNode( tree[j][1], tree[j][2] )
            if source.x == target.x and source.y == target.y:
               #print( WarningMessage( ['RSMT.doFlute(): Edge has same source & target.'
               #                       , '({})'.format(source) ]) )
                continue
            self.edges.append( Edge( source, target ) )
            self.length = self.edges[0].length
        for node in self.nodes: node.check()
        return

    def doIteratedOneSteiner ( self ):
        trace( 542, ',+', '\tRSMT.doIteratedSteiner() on "{}".\n'.format(self.net.getName()) )
        self.edges  = []
        self.length = 0
        
        if len(self.nodes) <  2: return
        if len(self.nodes) == 2:
            self.edges.append( Edge( self.nodes[0], self.nodes[1] ) )
            self.length = self.edges[0].length
            return
        
        self._computeHanan()
        count = 0
        minMST = RMST( self.net, 'MST[{}]'.format(count) )
        minMST.setNodes( self.nodes )
        minMST.doPrim()
        trace( 542, '\tInitial "{}" length:{}\n'.format(minMST.name,DbU.getValueString(len(minMST))) )
       #minMST.showEdges()
        
        addedSteiner = True
        while addedSteiner:
            addedSteiner = False
            for steinerNode in self._hananNodes:
                count += 1
                trace( 542, '\tTrying with Steiner point [{} {}]\n' \
                            .format(DbU.getValueString(steinerNode.x)
                                   ,DbU.getValueString(steinerNode.y)) )
                mst = RMST( self.net, 'MST[{}]'.format(count) )
                mst.setNodes( self.nodes )
                mst.copyNode( steinerNode )
                mst.doPrim()
                trace( 542, '\tCurrent "{}" length {}\n' \
                            .format(mst.name,DbU.getValueString(len(mst))) )
               #mst.showEdges()
                if len(mst) < len(minMST):
                    trace( 542, '\tAccept min RST.\n' )
                    minMST       = mst
                    addedSteiner = True
            
            if addedSteiner:
                self.copyNode( minMST.nodes[-1] )
                self.nodes[-1].setFlags( Node.KeepPoint )
                
                i = 0
                while i < len(self.edges):
                    if     self.nodes[i].flags  & Node.SteinerPoint \
                       and self.nodes[i].degree < 3:
                        trace( 542, '\tDeleting unused Steiner point @[{} {}]\n' \
                                    .format(DbU.getValueString(self.nodes[i].x)
                                           ,DbU.getValueString(self.nodes[i].y)) )
                        del self.nodes[i]
                    else:
                        i += 1
        
        self.nodes  = minMST.nodes
        self.edges  = minMST.edges
        self.length = minMST.length
        self.showEdges()
        trace( 542, '-' )

    def createGRSegments ( self ):
        trace( 542, ',+', '\tRSMT.createGRsegments(): "{}"\n'.format(self.net.getName()) )
        for node in self.nodes:
            node.createGContact( self.net )
        for i in range(len(self.edges)):
            segments = self.edges[i].createGSegment( self.net )
            for segment in segments:
                trace( 542, '\t[{:3}] {}\n'.format(i,segment) )
        trace( 542, '-' )
            

# -------------------------------------------------------------------
# Module static initialization

gcut    = None
gmetalh = None
gmetalv = None


def staticInit ():
    """
    Static initialization of the ``rsmt`` module. Allow to postpone
    initialization until Hurricane & CRL database and evironment are
    properly loaded.
    """
    global gcut
    global gmetalh
    global gmetalv
    if gcut is not None: return

    tech    = DataBase.getDB().getTechnology()
    gcut    = tech.getLayer( 'gcontact' )
    gmetalh = tech.getLayer( 'gmetalh' )
    gmetalv = tech.getLayer( 'gmetalv' )
    Flute.readLUT()

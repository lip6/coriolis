
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/clocktree/rsmt.py"              |
# +-----------------------------------------------------------------+


import sys
import Cfg
import Hurricane
from   Hurricane  import DbU
from   Hurricane  import Box
from   Hurricane  import Path
from   Hurricane  import Occurrence
from   Hurricane  import UpdateSession
from   Hurricane  import Breakpoint
from   Hurricane  import Net
from   Hurricane  import RoutingPad
from   Hurricane  import Contact
from   Hurricane  import Horizontal
from   Hurricane  import Vertical
from   Hurricane  import Instance
import Viewer
import CRL
from   CRL        import RoutingLayerGauge
import helpers
from   helpers    import trace
from   helpers.io import ErrorMessage
import plugins


def manhattanDistance ( node1, node2 ):
    return abs(node1.x - node2.x) + abs(node1.y - node2.y)


class Node ( object ):

    GraphPoint   = 0x0001
    SteinerPoint = 0x0002
    KeepPoint    = 0x0004

    @staticmethod
    def showNodes ( level, nodes ):
      for node in nodes:
        trace( level, node )
      return

    def __init__ ( self, component, x, y, flags=0 ):
      self._component = component
      self._edges     = [ ]
      self._x         = x
      self._y         = y
      self._realY     = y
      self._back      = None
      self._distance  = DbU.fromLambda(1000.0)
      self._flags     = flags

      if component: self._flags = self._flags|Node.GraphPoint|Node.KeepPoint
      else:         self._flags = self._flags|Node.SteinerPoint
      return

    def __cmp__ ( self, other ):
      return self.distance - other.distance

    def __str__ ( self ):
      return '<Node P:%d,%d (realY:%d) d:%d %s>' % ( DbU.toLambda(self.x)
                                                   , DbU.toLambda(self.y)
                                                   , DbU.toLambda(self.realY)
                                                   , DbU.toLambda(self.distance)
                                                   , str(self.component)
                                                   )

    @property
    def component  ( self ): return self._component
    @property      
    def x          ( self ): return self._x
    @property      
    def y          ( self ): return self._y
    @property      
    def realX      ( self ): return self._x
    @property      
    def realY      ( self ): return self._realY
    @property      
    def back       ( self ): return self._back
    @property      
    def distance   ( self ): return self._distance
    @property      
    def flags      ( self ): return self._flags
    @property
    def edges      ( self ): return self._edges
    @property
    def degree     ( self ): return len(self._edges)
    @component.setter
    def component  ( self, component ): self._component = component
    @realY.setter
    def realY      ( self, y ): self._realY = y

    def setFlags   ( self, flags ): self._flags = self._flags |  flags
    def unsetFlags ( self, flags ): self._flags = self._flags & ~flags

    def addEdge    ( self, edge ): self._edges.append( edge )
    def delEdge    ( self, edge ):
      for i in range(len(self._edges)):
        if self._edges[i] == edge:
          del self._edges[i]
      return

    def isSame ( self, other ): return id(self) == id(other)

    def update ( self, node ):
      distance = manhattanDistance( self, node )
      if distance < self.distance:
        self._distance = distance
        self._back     = node
        return True
      return False


class Edge ( object ):

    def __init__ ( self, source, target ):
      self._source = source
      self._target = target
      self._length = manhattanDistance( self._source, self._target )
      self._source.addEdge( self )
      self._target.addEdge( self )
      return

    def __del__ ( self ):
      self._source.delEdge( self )
      self._target.delEdge( self )
      return

    def __str__ ( self ):
      return '<Edge S:%d,%d T:%d,%d len:%d>' % ( DbU.toLambda(self.source.x)
                                               , DbU.toLambda(self.source.y)
                                               , DbU.toLambda(self.target.x)
                                               , DbU.toLambda(self.target.y)
                                               , DbU.toLambda(self.length)
                                               )

    @property
    def source ( self ): return self._source
    @property
    def target ( self ): return self._target
    @property
    def length ( self ): return self._length

    def isHorizontal ( self ): return self.source.y == self.target.y
    def isVertical   ( self ): return self.source.x == self.target.x


class Graph ( object ):

    def __init__ ( self, name ):
      self._nodes      = [ ]
      self._edges      = [ ]
      self._length     = 0
      self._name       = name
      return

    def __len__ ( self ): return self._length

    @property
    def name   ( self ): return self._name
    @property
    def length ( self ): return self._length
    @property
    def nodes  ( self ): return self._nodes
    @property
    def edges  ( self ): return self._edges

    def addNode  ( self, component, x, y ):
      self._nodes.append( Node( component, x, y ) )
      return self._nodes[-1]

    def copyNode ( self, node ):
       self.addNode( node.component, node.x, node.y )
       self._nodes[-1].realY = node.realY

    def setNodes ( self, nodes ):
      self.__init__( self.name )
      for node in nodes:
        self.copyNode( node )
      return

    def showNodes ( self, level ):
      trace( level, '+,+', '\tGraph Nodes:\n' )
      for node in self._nodes:
        trace( level, node )
      trace(level, '--')
      return

    def showEdges ( self, level ):
      trace( level, '+,+', '\tGraph Edges:\n' )
      for edge in self._edges:
        trace( level, edge )
      trace(level, '--')
      return


class RMST ( Graph ):

    def __init__ ( self, name ):
      Graph.__init__( self, name )
      return

    def runPrim ( self ):
      self._edges  = [ ]
      self._length = 0

      if len(self._nodes) <  2: return
      if len(self._nodes) == 2:
        self._edges.append( Edge( self._nodes[0], self._nodes[1] ) )
        self._length = self._edges[0].length
        return

      trace(500, '+')

      toReach = [ ]
      self._nodes[0]._distance = 0
      for node in self._nodes[1:]:
        node.update( self._nodes[0] )
        toReach.append( node )
      toReach.sort()

      trace( 500, '+'  , '\tPrim (initial stack)\n' )
      trace( 500, '+,+', '\tS %s\n' % self._nodes[0] )
      Node.showNodes( 500, toReach )
      trace( 500, '---' )

      while len(toReach):
        nearest = toReach.pop(0)
        self._edges.append( Edge( nearest, nearest.back ) )
        trace( 500, '++,--', '\tAdding %s\n' % self._edges[-1] )

        for node in toReach:
          node.update( nearest )
        toReach.sort()

        trace( 500, '+'  , '\tPrim (current stack)\n' )
        trace( 500, '+,+', '\tS %s\n' % self._nodes[0] )
        Node.showNodes( 500, toReach )
        trace( 500, '---' )

      for edge in self._edges:
        self._length += edge.length

      trace( 500, '-' )
      return


class RSMT ( Graph ):

    def __init__ ( self, name ):
      Graph.__init__( self, name )
      self._hananNodes = [ ]
      return

    def _computeHanan ( self ):
      xs     = [ ]
      ys     = [ ]
      realYs = { }
      for node in self._nodes:
        if not node.x in xs: xs.append( node.x )
        if not node.y in xs:
          ys.append( node.y )
          realYs[ node.y ] = node.component.getY()
      xs.sort()
      ys.sort()

      trace( 550, '+,+', '\tHanan matrix: %ix%i' % (len(xs),len(ys)) )

      self._hananNodes = [ ]
      for x in xs:
        trace( 550, '\n' )
        trace( 550, '\t' )
        for y in ys:
          isNode = False
          for node in self._nodes:
            if node.x == x and node.y == y: isNode = True
          if isNode:
            trace( 550, ' -:%04.2d,%04.2d' % (DbU.toLambda(x),DbU.toLambda(y)) )
            continue
          trace( 550, ' H:%04.2d,%04.2d' % (DbU.toLambda(x),DbU.toLambda(y)) )

          self._hananNodes.append( Node( None, x, y ) )
          self._hananNodes[-1].realY = realYs[ y ]
      trace( 550, ',--', "\n" )
      return

    def addNode  ( self, component, x, y ):
      node = Graph.addNode( self, component, x, y )
      trace( 550, '\t  New Node: %s\n' % node )
      return

    def runI1S ( self ):
      self._edges  = [ ]
      self._length = 0

      if len(self._nodes) <  2: return
      if len(self._nodes) == 2:
        self._edges.append( Edge( self._nodes[0], self._nodes[1] ) )
        self._length = self._edges[0].length
        return

      self._computeHanan()
      count = 0

      trace( 550, '++' )
      minMST = RMST( 'MST[%i]' % count )
      minMST.setNodes( self._nodes )
      minMST.runPrim()
      trace( 550, '-,+', '\tInitial %s length %d\n' % (minMST.name,DbU.toLambda(len(minMST))) )
      minMST.showEdges( 550 )
      trace( 550, '-' )

      addedSteiner = True
      while addedSteiner:
        addedSteiner  = False
        for steinerNode in self._hananNodes:
          count += 1
          trace( 550, '\tTrying with Steiner point H:%d,%d\n' \
                      % (DbU.toLambda(steinerNode.x),DbU.toLambda(steinerNode.y)) )
          mst = RMST( 'MST[%i]' % count )
          mst.setNodes( self._nodes )
          mst.copyNode( steinerNode )
          mst.runPrim()
  
          trace( 550, '\tCurrent %s length %d\n' % (mst.name,DbU.toLambda(len(mst))) )
          mst.showEdges( 550 )
          if len(mst) < len(minMST):
            trace( 550, '\tAccept min RST.\n' )
            minMST       = mst
            addedSteiner = True

        if addedSteiner:
          self.copyNode( minMST.nodes[-1] )
          self.nodes[-1].setFlags( Node.KeepPoint )

          i = 0
          while i < len(self._edges):
            if     self._nodes[i].flags  & Node.SteinerPoint \
               and self._nodes[i].degree < 3:
              trace( 550, 'Deleting unused Steiner point H:%d,%d' \
                          % (DbU.toLambda(self._nodes[i].x),DbU.toLambda(self._nodes[i].y)) )
              del self._nodes[i]
            else:
              i += 1

      self._nodes  = minMST.nodes
      self._edges  = minMST.edges
      self._length = minMST.length
      trace( 550, '-' )
      return

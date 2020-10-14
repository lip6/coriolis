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
# |  Python      :       "./plugins/block/spares.py"                |
# +-----------------------------------------------------------------+


import sys
import os.path
import Cfg
from   operator  import itemgetter
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Transformation
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Layer
from   Hurricane import Occurrence
from   Hurricane import Net
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
from   helpers         import dots
from   helpers.io      import ErrorMessage
from   helpers.io      import WarningMessage
from   helpers.io      import catch
from   helpers.overlay import UpdateSession
from   plugins         import getParameter
from   plugins.alpha   import utils


framework = CRL.AllianceFramework.get()


# ----------------------------------------------------------------------------
# Class  :  "spares.BufferPool".

class BufferPool ( object ):
    """
    Manage a matrix of buffer and their allocations. One pool is created
    at the center of each QuadTree level (both terminal and non-terminal).
    """

    def __init__ ( self, quadTree ):
        self.quadTree      = quadTree
        self.columns       = quadTree.spares.conf.bColumns
        self.rows          = quadTree.spares.conf.bRows
        self.area          = Box()
        self.buffers       = []
        self.selectedIndex = None
        for i in range(self.rows*self.columns):
            self.buffers.append( [ 0, None ] )
        self._createBuffers()

    @property
    def selected ( self ):
        """
        Returns the selected buffer instance within the pool. Selection is to
        be made with either select() or selectFree().
        """
        if self.selectedIndex is None:
            raise ErrorMessage( 3, 'BufferPool.selected: No buffer has been selected yet.' )
        return self.buffers[ self.selectedIndex ][ 1 ]

    def toIndex ( self, column, row ): return column + row*self.columns

    def fromIndex ( self, index ): return (index%self.columns, index/self.columns)

    def select ( self, column, row, flags=0 ):
        """
        Select a specific buffer in the pool matrix. The ``flags`` arguments are
        used to specify additional checks & actions. Flags operations are done by
        the low level function _select(). To actually access the selected buffer,
        use the selected property.

        :param column: The colum of the buffer (aka X).
        :param row:    The row of the buffer (aka Y).
        :param flags:  A combination of flags, defined in the Spares object.

        Flags can be used to:

        * CHECK_USED, raise an error if the designated buffer is already used.
        * MARK_USED, tag the designated buffer as USED.
        """

        trace( 540, ',+', '\tBufferPool.select() column:{}, row={}, flags={:x}\n' \
               .format(column,row,flags) )
        if column >= self.columns:
            trace( 540, '-' )
            raise ErrorMessage( 3, 'BufferPool.select(): Column {} is out of range (max:{}).' \
                                   .format(column,self.columns) )
        if row >= self.rows:
            trace( 540, '-' )
            raise ErrorMessage( 3, 'BufferPool.select(): Row {} is out of range (max:{}).' \
                                   .format(row,self.rows) )
        self._select( self.toIndex( column, row ), flags )
        trace( 540, '-' )

    def _select ( self, index, flags ):
        self.selectedIndex = index
        selectedBuffer = self.buffers[ self.selectedIndex ]
        if flags & Spares.CHECK_USED and selectedBuffer[0] & Spares.USED:
            raise ErrorMessage( 3, 'BufferPool.select(): Buffer a index {} is already used.' \
                                   .format(self.selectedIndex) )
        if flags & Spares.MARK_USED:
            selectedBuffer[0] |= Spares.USED

    def selectFree ( self ):
        """
        Select the first free buffer available. Marks the buffer as used
        and return it's instance. If all buffer in the pool are taken,
        return ``None``.
        """
        for i in range(self.rows*self.columns):
            if not (self.buffers[i][0] & Spares.USED):
                self._select( i, Spares.MARK_USED )
                trace( 540, '\tUse buffer from pool {}\n'.format(self.quadTree) )
                return self.buffers[i][1]
        return None

    def hasFree ( self ):
        """Tells if the pool has a free buffer available."""
        for i in range(self.rows*self.columns):
            if not (self.buffers[i][0] & Spares.USED):
                return True
        return False

    def _createBuffers ( self ):
        """Create the matrix of instances buffer."""
        trace( 540, ',+', '\tBufferPool.createBuffers()\n' )

        conf        = self.quadTree.spares.conf
        sliceHeight = conf.sliceHeight 
        x           = self.quadTree.onXPitch( self.quadTree.area.getXCenter()
                                              - (conf.bufferConf.width  * self.columns)/2 )
        y           = self.quadTree.onYSlice( self.quadTree.area.getYCenter()
                                              - (conf.bufferConf.height * self.rows)/2 )
        slice = y / sliceHeight

        trace( 540, '\tSlice height: {}\n'.format(DbU.getValueString(sliceHeight)) )

        for row in range(self.rows):
            orientation = Transformation.Orientation.ID
            y = (slice+row) * sliceHeight
            if (slice+row)%2:
                orientation = Transformation.Orientation.MY
                y          += sliceHeight
            for column in range(self.columns):
                index    = self.toIndex(column,row)
                transf   = Transformation( x + column*conf.bufferConf.width, y, orientation )
                instance = conf.createBuffer()
                instance.setTransformation( transf )
                instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                self.buffers[ index ][1] = instance 
                trace( 540, '\tBuffer[{}]: {} @{}\n'.format(index,self.buffers[index],transf) )
        blBufAb   = self.buffers[ 0][1].getAbutmentBox()
        trBufAb   = self.buffers[-1][1].getAbutmentBox()
        self.area = Box( blBufAb.getXMin(), blBufAb.getYMin()
                       , trBufAb.getXMax(), trBufAb.getYMax() )
        trace( 540, '-' )

    def _destroyBuffers ( self ):
        """Destroy all the buffer instances of the pool."""
        for flags, buffer in self.buffers:
            buffer.destroy()

    def getUse ( self ):
        """Return the pool occupancy, a tuple ``(occupancy,capacity)``."""
        count = 0
        for i in range(self.rows*self.columns):
            if self.buffers[i][0] & Spares.USED:
                count += 1
        return count, self.rows*self.columns

    def showUse ( self, depth ):
        """Display the pool occupancy."""
        occupancy, capacity = self.getUse()
       #header = '| ' if self.quadTree.isLeaf() else '+ '
       #print( '     {}{}Pool {}, usage:{}/{}.'.format( '  '*depth
       #                                              , header
       #                                              , self.quadTree
       #                                              , occupency
       #                                              , capacity )
        return occupancy, capacity


# ----------------------------------------------------------------------------
# Class  :  "spares.QuadTree".

class QuadTree ( object ):
    """
    Recursive quad-tree to partition the design area. If the aspect ratio
    is not close to one, the first levels are only binary trees, either
    horizontals or verticals. We resume quad-tree when the aspect ratio
    is comprised between 1/2 (.5) and 2.0.
    """

    @staticmethod
    def create ( spares ):
        root = QuadTree( spares, None, spares.conf.cell.getAbutmentBox() )
        root.rpartition()
        return root

    def __init__ ( self, spares, parent, area, rtag='root' ):
        self.spares    = spares
        self.area      = area
        self.xcut      = None
        self.ycut      = None
        self.parent    = parent
        self.depth     = parent.depth+1 if parent else 0
        self.maxDepth  = 0
        self.bl        = None
        self.br        = None
        self.tl        = None
        self.tr        = None
        self.position  = [ None, None ]
        self.bufferTag = 'spare'
        self.bufferNet = None
        self.pool      = BufferPool( self )
        self.plugs     = []
        if self.parent and self.parent.rtag != '':
            self.rtag = self.parent.rtag + '_' + rtag
        else:
            self.rtag = rtag

    def destroy ( self ):
        if self.bl: self.bl.destroy()
        if self.br: self.br.destroy()
        if self.tl: self.tl.destroy()
        if self.tr: self.tr.destroy()
        self.pool._destroyBuffers()

    def __str__ ( self ):
        occupancy, capacity = self.pool.getUse()
        s = '<QuadTree [{},{} {},{}] {}/{} "{}">' \
            .format( DbU.getValueString(self.area.getXMin())
                   , DbU.getValueString(self.area.getYMin())
                   , DbU.getValueString(self.area.getXMax())
                   , DbU.getValueString(self.area.getYMax())
                   , occupancy
                   , capacity
                   , self.rtag )
        return s

    def __eq__ ( self, other ):
        return self.rtag == other.rtag

    def rshowPoolUse ( self ):
        rused  = 0
        rtotal = 0
        used, total = self.pool.showUse( self.depth )
        rused  += used
        rtotal += total
        for leaf in self.leafs:
            used, total = leaf.rshowPoolUse()
            rused  += used
            rtotal += total

        if not self.depth:
            global framework
            catalog = framework.getCatalog()
            instancesNb = 0
            bufNb       = 0
            for occurrence in self.spares.conf.cell.getTerminalNetlistInstanceOccurrences():
                cellName = occurrence.getEntity().getMasterCell().getName()
                cstate   = catalog.getState( cellName )
                if cstate and cstate.isFeed(): continue
                if cellName.startswith( 'buf_' ):
                    bufNb += 1
                    continue
                instancesNb += 1
            if bufNb != rtotal:
                print( WarningMessage('QuadTree.rshowPoolUse(): Buffer instance number discrepency, {} vs {}.' \
                                      .format(bufNb,rtotal)) )
            if rtotal:
                print( '  o  Detailed use of spare buffers.' )
                dots( 82
                    , '     - Useds '
                    , ' {}/{} ({:.1%})'.format(rused,rtotal,float(rused)/float(rtotal)) )
                dots( 82
                    , '     - Buffer ratio '
                    , ' {}/{} ({:.1%})'.format( rtotal
                                              , instancesNb+bufNb
                                              , float(rtotal)/float(instancesNb+bufNb)) )
        return rused, rtotal

    @property
    def leafs ( self ):
        activeLeafs = []
        if self.bl: activeLeafs.append( self.bl )
        if self.br: activeLeafs.append( self.br )
        if self.tl: activeLeafs.append( self.tl )
        if self.tr: activeLeafs.append( self.tr )
        return activeLeafs

    @property
    def root ( self ):
        if self.parent is None: return self
        return self.parent.root

    def isHBipart ( self ): return self.ycut is None
    def isVBipart ( self ): return self.xcut is None

    def isRoot ( self ):
        return self.parent is None

    def isLeaf ( self ): 
        for leaf in (self.bl, self.br, self.tl, self.tr):
            if leaf is not None: return False
        return True

    def getParentAt ( self, depth ):
        """
        Return the parent at the given ``depth``. The depth increase starting
        from the root which is labeled 0. So requesting the parent at a depth
        superior to the one of the node is an error...
        """
        if self.depth <= depth:
            raise ErrorMessage( 2, 'QuadTree.getParentAt(): Parent depth must be lower than current depth ({} vs. {})' \
                                   .format(depth,self.depth) )
        parent = self.parent
        while parent.depth > depth:
            parent = parent.parent
        return parent

    @property
    def buffer ( self ):
        """The the currently selected buffer instance in the pool."""
        return self.pool.selected

    @property
    def bInputPlug ( self ):
        """The input Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.conf.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.conf.bufferConf.output )

    def onYSlice ( self, y ):
        """Returns the coordinate of the slice immediately inferior to Y."""
        modulo = (y - self.area.getYMin()) % self.spares.conf.sliceHeight 
        return y - modulo
        
    def onXPitch ( self, x ):
        """Returns the coordinate of the pitch immediately inferior to X."""
        modulo = (x - self.area.getXMin()) % self.spares.conf.sliceStep 
        return x - modulo

    def selectFree ( self ):
        """
        Returns the first free buffer *instance* in the pool or None if
        there isn't any left.
        """
        return self.pool.selectFree()

    def connectBuffer ( self, doLeaf=False ):
        """
        Create output nets for the currently selected buffer, if they do not
        already exists. The nets are created in the top level cell, and their
        names are derived from the `rtag` attribute.
        """
        if self.isLeaf() and not doLeaf: return

        trace( 540, '\tQuadTree.connectBuffer(): rtag:"{}"\n'.format(self.rtag) )
        plug = self.bOutputPlug
        if not plug.getNet():
            outputNetBuff = Net.create( self.spares.conf.cell,'{}_{}' \
                                        .format(self.root.bufferTag,self.rtag) )
            plug.setNet( outputNetBuff )
            trace( 540, '\t| {}\n'.format(plug) )
            trace( 540, '\t| {}\n'.format(outputNetBuff) )

    def rconnectBuffer ( self ):
        """[R]ecursive call of connectBuffer()"""
        self.connectBuffer()
        for leaf in self.leafs:
            leaf.rconnectBuffer()

    def rselectBuffer ( self, column, row, flags=0 ):
        """
        Setup the buffer to be used for the next buffering operation.
        For a more detailed explanation of the parameter, please refer
        to BufferPool.select().
        """
        trace( 540, '+' )
        if self.plugs:
            self.plugs = []
        self.pool.select( column, row, flags )
        if not self.isLeaf():
            for leaf in self.leafs: leaf.rselectBuffer( column, row, flags )
        trace( 540, '-' )

    def partition ( self ):
        """
        Build one level of the quad-tree, if the side of the area is bigger than
        twice the "block.spareSide" value.

        Depending on the initial aspect ratio, the first levels *may* not be a
        quad-tree, but only a vertical or horizontal bi-partition.
        """
        trace( 540, ',+', '\tQuadTree.partition(): {} (spareSide:{})\n' \
                    .format(self.area, DbU.getValueString(self.spares.conf.cfg.block.spareSide)) )

        spareSide   = self.spares.conf.cfg.block.spareSide
        sliceHeight = self.spares.conf.sliceHeight
        side        = float(spareSide)
        aspectRatio = float(self.area.getWidth()) / float(self.area.getHeight())

        if self.area.getHeight() < side*2.0 or self.area.getWidth () < side*2.0:
            trace( 540, '-' )
            return False

        if aspectRatio < 0.5:
            self.ycut = self.spares.toYGCellGrid( self.area.getYMin() + self.area.getHeight()/2 )
            self.bl   = QuadTree( self.spares
                                , self
                                , Box( self.area.getXMin()
                                     , self.area.getYMin()
                                     , self.area.getXMax()
                                     , self.ycut )
                                , 'bl' )
            self.tl   = QuadTree( self.spares
                                , self
                                , Box( self.area.getXMin()
                                     , self.ycut
                                     , self.area.getXMax()
                                     , self.area.getYMax() )
                                , 'tl' )
            trace( 540, '\tVertical bi-partition @Y:{}\n'.format(DbU.getValueString(self.ycut)) )
            trace( 540, '-' )
            return True
        elif aspectRatio > 2.0:
            self.xcut = self.spares.toXGCellGrid( self.area.getXMin() + self.area.getWidth()/2 )
            self.bl   = QuadTree( self.spares
                                , self
                                , Box( self.area.getXMin()
                                     , self.area.getYMin()
                                     , self.xcut
                                     , self.area.getYMax() )
                                , 'bl' )
            self.br   = QuadTree( self.spares
                                , self
                                , Box( self.xcut
                                     , self.area.getYMin()
                                     , self.area.getXMax()
                                     , self.area.getYMax() )
                                , 'br' )
            trace( 540, '\tHorizontal bi-partition @X:{}\n'.format(DbU.getValueString(self.xcut)) )
            trace( 540, '-' )
            return True

        self.ycut = self.spares.toYGCellGrid( self.area.getYMin() + self.area.getHeight()/2 )
        self.xcut = self.spares.toXGCellGrid( self.area.getXMin() + self.area.getWidth ()/2 )
        self.bl   = QuadTree( self.spares
                            , self
                            , Box( self.area.getXMin()
                                 , self.area.getYMin()
                                 , self.xcut
                                 , self.ycut )
                            , 'bl' )
        self.br   = QuadTree( self.spares
                            , self
                            , Box( self.xcut
                                 , self.area.getYMin()
                                 , self.area.getXMax()
                                 , self.ycut )
                            , 'br' )
        self.tl   = QuadTree( self.spares
                            , self
                            , Box( self.area.getXMin()
                                 , self.ycut
                                 , self.xcut
                                 , self.area.getYMax() )
                            , 'tl' )
        self.tr   = QuadTree( self.spares
                            , self
                            , Box( self.xcut
                                 , self.ycut
                                 , self.area.getXMax()
                                 , self.area.getYMax() )
                            , 'tr' )

        trace( 540, '\tQuadri-partition @X:{} + @Y:{}\n'\
               .format(DbU.getValueString(self.xcut),DbU.getValueString(self.ycut)) )
        trace( 540, '-' )
        return True
                
    def rpartition ( self ):
        """"[R]ecursively partition the the area."""
        trace( 540, ',+', '\tQuadTree.rpartition(): {}\n'.format(self.area) )
        if self.partition():
            for leaf in self.leafs:
                trace( 540, '\tLeaf rtag:"{}"\n'.format(leaf.rtag) )
                leaf.rpartition()
        trace( 540, '-' )
        if self.isRoot():
            self._rsetMaxDepth()
            self._rsetPosition( self.maxDepth, [0,0] )

    def _rsetMaxDepth ( self ):
        maxDepth = 0
        trace( 540, ',+', '\tEntering: {}\n'.format(self) )
        if not self.isLeaf():
            for leaf in self.leafs:
                leaf._rsetMaxDepth()
                maxDepth = max( maxDepth, leaf.maxDepth+1 )
        self.maxDepth = maxDepth
        trace( 540, ',-', '\tMaxdepth: {} {}\n'.format(maxDepth,self) )

    def _rsetPosition ( self, maxDepth, position ):
        trace( 540, '+,', '\t+_rsetPosition(): {}\n'.format(self) )
        self.position = position
        trace( 540, '\t| position:{}\n'.format(self.position) )
        if not self.isLeaf():
            leafBit = 1 << (maxDepth - (self.depth + 1))
            trace( 540, '\t| leafBit:{}\n'.format(leafBit) )
            if self.bl: self.bl._rsetPosition( maxDepth, position )
            if self.br: self.br._rsetPosition( maxDepth, [position[0] | leafBit, position[1]] )
            if self.tl: self.tl._rsetPosition( maxDepth, [position[0]          , position[1] | leafBit] )
            if self.tr: self.tr._rsetPosition( maxDepth, [position[0] | leafBit, position[1] | leafBit] )
        trace( 540, '-' )

    def _getLeafAt ( self, maxDepth, stopDepth, position ):
        trace( 540, '\t_getLeafAt(): {}\n'.format(self) )
        trace( 540, '\t| maxDepth:{}, stopDepth:{}, position:{}\n' \
                    .format(maxDepth,stopDepth,position) )
        if self.isLeaf(): return self
        if self.depth >= stopDepth: return leaf
        bitDepth = maxDepth - (self.depth + 1)
        bitMask  = 1 << bitDepth
        trace( 540, '\tbitDepth: {}, bitMask: {:b}\n'.format(bitDepth,bitMask) )
        leafCode = (position[0] & bitMask) + ((position[1] & bitMask) << 1)
        trace( 540, '\tleafCode: {}\n'.format(leafCode) )
        leafCode = leafCode >> bitDepth
        leaf     = None
        trace( 540, '\tleafCode: {}\n'.format(leafCode) )
        if leafCode == 0: leaf = self.bl
        if leafCode == 1: leaf = self.br
        if leafCode == 2: leaf = self.tl
        if leafCode == 3: leaf = self.tr
        if leaf is None: return None
        trace( 540, '+' )
        leaf = leaf._getLeafAt( maxDepth, stopDepth, position )
        trace( 540, '-' )
        return leaf

    def getLeafAt ( self, position, depth=None ):
        return self._getLeafAt( self.root.maxDepth, depth, position )

    def getLeft ( self ):
        trace( 540, '\tgetLeft(): \n'.format(self) )
        shiftedPos = self.position[0] >> (self.root.maxDepth - self.depth)
        deltaPos   = 1 << (self.root.maxDepth - self.depth)
        trace( 540, '\t| position[0] (x): {}\n'.format(self.position[0]) )
        trace( 540, '\t| shiftedPos  (x): {}\n'.format(shiftedPos) )
        trace( 540, '\t| deltaPos    (x): {}\n'.format(deltaPos) )
        if shiftedPos == 0: return None
        return self.root.getLeafAt( [self.position[0]-deltaPos, self.position[1]], self.depth )

    def getRight ( self ):
        trace( 540, '\tgetRight(): \n'.format(self) )
        shiftedPos = self.position[0] >> (self.root.maxDepth - self.depth)
        deltaPos   = 1 << (self.root.maxDepth - self.depth)
        trace( 540, '\t| position[0] (x): {}\n'.format(self.position[0]) )
        trace( 540, '\t| shiftedPos  (x): {}\n'.format(shiftedPos) )
        trace( 540, '\t| deltaPos    (x): {}\n'.format(deltaPos) )
        if shiftedPos+1 >= 1 << self.depth: return None
        return self.root.getLeafAt( [self.position[0]+deltaPos, self.position[1]], self.depth )

    def getBottom ( self ):
        trace( 540, '\tgetBottom(): \n'.format(self) )
        shiftedPos = self.position[1] >> (self.root.maxDepth - self.depth)
        deltaPos   = 1 << (self.root.maxDepth - self.depth)
        trace( 540, '\t| position[0] (x): {}\n'.format(self.position[0]) )
        trace( 540, '\t| shiftedPos  (x): {}\n'.format(shiftedPos) )
        trace( 540, '\t| deltaPos    (x): {}\n'.format(deltaPos) )
        if shiftedPos == 0: return None
        return self.root.getLeafAt( [self.position[0], self.position[1]-deltaPos], self.depth )

    def getTop ( self ):
        trace( 540, '\tgetTop(): \n'.format(self) )
        shiftedPos = self.position[1] >> (self.root.maxDepth - self.depth)
        deltaPos   = 1 << (self.root.maxDepth - self.depth)
        trace( 540, '\t| position[0] (x): {}\n'.format(self.position[0]) )
        trace( 540, '\t| shiftedPos  (x): {}\n'.format(shiftedPos) )
        trace( 540, '\t| deltaPos    (x): {}\n'.format(deltaPos) )
        if shiftedPos+1 >= 1 << self.depth: return None
        return self.root.getLeafAt( [self.position[0], self.position[1]+deltaPos], self.depth )

    def getLeafUnder ( self, position ):
        """Find the QuadTree leaf under ``position``."""
        if self.isLeaf(): return self
        if self.isHBipart():
            if position.getX() < self.xcut: return self.bl.getLeafUnder(position)
            return self.br.getLeafUnder(position)
        if self.isVBipart():
            if position.getY() < self.ycut: return self.bl.getLeafUnder(position)
            return self.tl.getLeafUnder(position)
        if position.getX() < self.xcut:
            if position.getY() < self.ycut: return self.bl.getLeafUnder(position)
            return self.tl.getLeafUnder(position)
        if position.getY() < self.ycut: return self.br.getLeafUnder(position)
        return self.tr.getLeafUnder(position)

    def getFreeLeafUnder ( self, area, attractor=None ):
        """
        Find a free buffer under the given ``area`` the candidates are sorted
        according to their distance to the ``attractor`` point, the closest is
        returned. If no ``attractor`` is supplied, the center of the ``area``
        id used. This function is a frontend to ``_getFreeLeafUnder()``.
        """
        candidates = []
        self._getFreeLeafUnder( area, candidates, attractor )
        if not len(candidates):
            return None
        candidates.sort( key=itemgetter(1) )
        return candidates[0][0]

    def _getFreeLeafUnder ( self, area, candidates, attractor ):
        """Find a free buffer under the given ``area``. See ``getFreeLeafUnder()``."""
        if self.pool.hasFree():
            point = area.getCenter() if attractor is None else attractor
            candidates.append( [ self, self.area.getCenter().manhattanDistance(point) ] )
        for leaf in self.leafs:
            if leaf.area.intersect(area):
                leaf._getFreeLeafUnder( area, candidates, attractor )

    def attachToLeaf ( self, plugOccurrence ):
        """Assign the plug occurrence to the QuadTree leaf it is under."""
        position = plugOccurrence.getBoundingBox().getCenter()
        self.getLeafUnder(position).plugs.append( plugOccurrence )

    def splitNetlist ( self ):
        """
        Reorganize the netlist by connecting all plugs to the output of the
        currently selected buffer of this QuadTree. Use the following steps:

        1. Create the output net of the currently selected buffer in the pool.
           This net will be at the top level of the hierarchy. The name of
           that net is created from the `rtag` and `buseds` attributes.

        2. For each plug to connect:

           2.a. Bore the buffer output net  through all the levels of hierarchy
                of the plug. This way, a new net, connected to the buffer output
                will be created in the same hierarchical level as the plug.

           2.b. Connect the plug to the buffer output net at the same level.

        No net/buffer will be used if the plug list is empty.
        """
        if not self.plugs: return

        trace( 540, ',+', '\tQuadTree.spliNetlist()\n' )
        self.connectBuffer( doLeaf=True )
        netBuff = self.bOutputPlug.getNet()
        trace( 540, '\tBuffer: {}\n'.format(self.buffer) )
        trace( 540, '\tBuffer output: {}\n'.format(netBuff) )
        for plug in self.plugs:
            trace( 540, '\t| Leaf: {}\n'.format(plug) )
            trace( 540, '\t| netBuff: {}\n'.format(netBuff) )
            deepPlug    = self.spares.raddTransNet( netBuff, plug.getPath() )
            trace( 540, '\t| netBuff: {}\n'.format(netBuff) )
            trace( 540, '\t| Deep Plug: {}\n'.format(deepPlug) )
            deepNetBuff = deepPlug.getMasterNet() if deepPlug else netBuff
            trace( 540, '\t| deepNetBuff: {} {}\n'.format(deepNetBuff,netBuff) )
            plug.getEntity().setNet( deepNetBuff )

        maxSinks = self.spares.conf.bufferConf.maxSinks
        if len(self.plugs) > maxSinks:
            print( WarningMessage( 'QuadTree.splitNetlist(): More than {} sink points ({}) on "{}".' \
                                   .format(maxSinks,len(self.plugs),netBuff.getName())) )
        trace( 540, '-' )

    def rsplitNetlist ( self ):
        """Recursive call over splitNetlist()."""
        self.splitNetlist()
        for leaf in self.leafs:
            leaf.rsplitNetlist()


# ----------------------------------------------------------------------------
# Class  :  "spares.Spares".

class Spares ( object ):
    """
    Manages all the spare buffer over a Cell abutment box. Used for clock tree
    synthesis (CTS) and high fanout net synthesis (HFS). 4 buffers are created
    in each pool. Pools are set in a regular matrix over the whole abutment box.
    Excess area is put in the topmost and rightmost pools.
    """

    USED       = 0x00000001
    CHECK_USED = 0x00010000
    MARK_USED  = 0x00020000

    def __init__ ( self, block ):
        self.conf         = block.conf
        self.quadTree     = None
        self.cloneds      = []
        self.strayBuffers = []

    def reset ( self ):
        self.quadTree.destroy()
        for buffer in self.strayBuffers:
            buffer.destroy()
        self.quadTree     = None
        self.cloneds      = []
        self.strayBuffers = []
        self.conf.resetBufferCount()

    def getSpareSpaceMargin ( self ):
        """
        Compute the percentage of margin space to compensate for the spare
        buffers (row*columns) with a supplemental margin factor of 1.3 or 1.4.
        """
        if not self.conf.useSpares: return 0.0
        spareSide    = self.conf.cfg.block.spareSide
        if not spareSide:
            raise ErrorMessage( 3, 'Spares.getSpareSpaceMargin(): "block.spareSide" parameter is zero ({}).' \
                                   .format(spareSide) )
        areaLength   = spareSide * spareSide / self.conf.sliceHeight
        bufferLength = self.conf.bufferConf.width * self.conf.bColumns * self.conf.bRows
        if not areaLength:
            raise ErrorMessage( 3, 'Spares.getSpareSpaceMargin(): Spare leaf area is zero.' )
        return (float(bufferLength) * 1.4) / float(areaLength)

    def toXGCellGrid ( self, x ):
        """Find the nearest X (inferior) on the Cell gauge grid (sliceStep)."""
        dx = x - self.conf.xMin
        return self.conf.xMin + (dx - dx % self.conf.sliceStep)

    def toYGCellGrid ( self, y ):
        """Find the nearest Y (inferior) on the Cell gauge grid (sliceHeight)."""
        dy = y - self.conf.yMin
        return self.conf.yMin + (dy - dy % self.conf.sliceHeight)

    def build ( self ):
        if not self.conf.useSpares: return
        trace( 540, ',+', '\tSpares.build()\n' )
        with UpdateSession():
            self.quadTree = QuadTree.create( self )
        trace( 540, '-' )

    def rshowPoolUse ( self ):
        if self.quadTree:
            self.quadTree.rshowPoolUse()

    def addStrayBuffer ( self, position ):
        """Add a new stray buffer at ``position``."""
        trace( 540, ',+', '\tSpares.addStrayBuffer()\n' )

        sliceHeight = self.conf.sliceHeight 
        x           = self.quadTree.onXPitch( position.getX() )
        y           = self.quadTree.onYSlice( position.getY() )
        slice       = y / sliceHeight
        orientation = Transformation.Orientation.ID
        y = slice * sliceHeight
        if slice % 2:
            orientation = Transformation.Orientation.MY
            y          += sliceHeight
        transf   = Transformation( x, y, orientation )
        instance = self.conf.createBuffer()
        instance.setTransformation( transf )
        instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
        unoverlapDx = self.quadTree.getUnoverlapDx( instance.getAbutmentBox() )
        if unoverlapDx:
            transf = Transformation( x+unoverlapDx, y, orientation )
            instance.setTransformation( transf )
        self.strayBuffers.append( instance )
        trace( 540, '\tBuffer: {} @{}\n'.format(self.strayBuffers[-1],transf) )
        trace( 540, '-' )
        return instance

    def getFreeBufferNear ( self, position ):
        leaf = self.quadTree.getLeafUnder( position )
        return leaf.selectFree()

    def getFreeBufferUnder ( self, area, attractor=None ):
        leaf = self.quadTree.getFreeLeafUnder( area, attractor )
        if leaf is None:
            raise ErrorMessage( 2, 'Spares.getFreeBufferUnder(): No more free buffers under {}.'.format(area) )
        return leaf.selectFree()

    def addClonedCell ( self, masterCell ):
        if not masterCell in self.cloneds:
            trace( 550, '\tNew cloned cell: "{}"\n'.format(masterCell) )
            self.cloneds.append( masterCell )
        return

    def raddTransNet ( self, topNet, path ):
        """
        Add a net through a whole hierarchy of Instance/Cells. The master cells
        of the instances may be modified in the process, if so, they are added
        to the cloned cell list.

        :param topNet: The Net of the topmost cell which should be propagated
                       down the hierarchy.
        :param path:   The ordered list of instances into which the top net
                       must be connected.
        :returns:      The plug associated to the net in the *last* instance
                       (at the bottom of the hierarchy).
        
        This is a recursive function. For each instance level:

        1. If the `path` is empty, it implies that no instance is involved,
           no plug will be created, so return None.

        2. Extract the *head* instance of the `path` and try to find an
           already existing plug connected to the `topNet`

        3. If no plug exists in the instance, then we must add a new external
           net into the master cell of the *head instance*. The creation of
           that external net will trigger the creation of the plug in all
           the instances of it. The new net is created with the same net
           name as the `topNet`, and copy it's type. The new plug is then
           connected to the `topNet`.

        4. If the tail path is empty, we are finished. Returns the current
           plug.

        5. The tail path is not empty, recursively call itself on the
           *plug master net* and the *tail path*.
        """

        if path.isEmpty():
            self.addClonedCell( topNet.getCell() )
            return None
        tailPath     = path.getTailPath()
        headInstance = path.getHeadInstance()
        headPlug     = utils.getPlugByNet(headInstance,topNet)
        if not headPlug:
            masterCell = headInstance.getMasterCell()
            masterNet  = Net.create( masterCell, topNet.getName() )
            masterNet.setExternal ( True )
            masterNet.setType     ( topNet.getType() )
            masterNet.setDirection( Net.Direction.IN )
            headPlug = headInstance.getPlug( masterNet )
            if not headPlug:
                raise ErrorMessage( 3, 'Plug not created for %s on instance %s of %s' \
                                    % (topNet.getName(),headInstance.getName(),masterCell.getName()) )
            headPlug.setNet( topNet )
            self.addClonedCell( masterCell )
            self.addClonedCell( headInstance.getCell() )
        else:
            masterNet = headPlug.getMasterNet()
        if tailPath.isEmpty(): return headPlug
        return self.raddTransNet( masterNet, tailPath )

    def rsave ( self, cell ):
        """
        Save the complete cell hierarchy. Saves only the physical view, except
        for the ones that has been cloned (their names should end up by "_cts"),
        for which logical and physical views are to be saved. They are completely
        new cells.
        """

        flags = CRL.Catalog.State.Physical
        if cell.getName().endswith('_cts'):
            flags = flags | CRL.Catalog.State.Logical
        framework.saveCell( cell, flags )
        
        for instance in  cell.getInstances():
            masterCell = instance.getMasterCell()
            if not masterCell.isTerminal():
                self.rsave( masterCell )
  
    def save ( self ):
        """
        Frontend to Spares.rsave(). Append the "_cts" suffix to the cloned
        cells, then call rsave().
        """
        trace( 550,'\tSpares.save() on "{}"\n'.format(self.conf.cell.getName()) )
        for cell in self.cloneds:
            trace( 550, '\tRenaming cloned cell: "{}"\n'.format(cell) )
            cell.setName( cell.getName()+'_cts' )
        self.rsave( self.conf.cell )
        return

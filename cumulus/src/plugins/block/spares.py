
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
# |  Python      :       "./plugins/block/spares.py"                |
# +-----------------------------------------------------------------+


import sys
import os.path
import math
from   operator           import itemgetter
from   ...                import Cfg
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Box, \
                                 Path, Layer, Occurrence, Net, RoutingPad,  \
                                 Horizontal, Vertical, Contact, Pin, Plug,  \
                                 Instance
from   ...CRL             import AllianceFramework, RoutingLayerGauge
from   ...helpers         import trace, dots, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ..                 import getParameter, utils


framework = AllianceFramework.get()


# ----------------------------------------------------------------------------
# Class  :  "spares.BufferPool".

class BufferPool ( object ):
    """
    Manage a matrix of buffer and their allocations. One pool is created
    at the center of each QuadTree level (both terminal and non-terminal).
    """

    def __init__ ( self, quadTree ):
        self.quadTree        = quadTree
        self.columns         = quadTree.spares.conf.bColumns
        self.rows            = quadTree.spares.conf.bRows
        self.area            = Box()
        self.buffers         = []
        self.selectedIndexes = []
        for i in range(self.rows*self.columns):
            self.buffers.append( [ 0, None ] )
        self._xRight = None
        self._createBuffers()

    @property
    def selecteds ( self ):
        """
        Returns the selected(s) buffer(s) instance(s) within the pool. Selection is to
        be made with either select() or selectFree().
        """
        if not len(self.selectedIndexes):
            raise ErrorMessage( 3, 'BufferPool.selecteds: No buffer has been selected yet.' )
        selectedBuffers = []
        for index in self.selectedIndexes:
            selectedBuffers.append( self.buffers[ index ][ 1 ] )
        return selectedBuffers

    def toIndex ( self, column, row ): return column + row*self.columns

    def fromIndex ( self, index ): return (index%self.columns, index//self.columns)

    def unselect ( self ): self.selectedIndexes = []

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
        for i in range(len(self.selectedIndexes)):
            if index == self.selectedIndexes[i]:
                del self.selectedIndexes[i]
                break
        self.selectedIndexes.append( index )
        selectedBuffer = self.buffers[ self.selectedIndexes[-1] ]
        if flags & Spares.CHECK_USED and selectedBuffer[0] & Spares.USED:
            raise ErrorMessage( 3, 'BufferPool.select(): Buffer a index {} is already used.' \
                                   .format(self.selectedIndexes[-1]) )
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
        trace( 540, ',+', '\tBufferPool.createBuffers() of {}\n'.format(self.quadTree) )
        yoffset = 0
        if self.quadTree.spares.conf.isCoreBlock:
            yoffset = self.quadTree.spares.conf.icore.getTransformation().getTy()
        conf           = self.quadTree.spares.conf
        sliceHeight    = conf.sliceHeight 
        poolHalfWidth  = (conf.bufferConf.width  * self.columns)//2
        poolHalfHeight = (conf.bufferConf.height * self.rows)//2
        if conf.sparesTies:
            poolHalfWidth += conf.feedsConf.tieWidth()
        x = self.quadTree.spares.toXPitch( self.quadTree.area.getXCenter() - poolHalfWidth  )
        y = self.quadTree.spares.toYSlice( self.quadTree.area.getYCenter() - poolHalfHeight )
        slice = (y - yoffset) // sliceHeight
        trace( 540, '\tSlice height: {}\n'.format(DbU.getValueString(sliceHeight)) )
        trace( 540, '\tSlice #{} (y:{})\n'.format(slice,DbU.getValueString(y)) )
        for row in range(self.rows):
            orientation = Transformation.Orientation.ID
            y = (slice+row) * sliceHeight + yoffset
            if (slice+row)%2:
                orientation = Transformation.Orientation.MY
                y          += sliceHeight
            length  = 0
            tieCols = 0
            if conf.sparesTies:
                tieCols = 2
            for column in range(self.columns+tieCols):
                transf = Transformation( x + length, y, orientation )
                if (not conf.sparesTies) or ((column > 0) and (column <= self.columns)):
                    index    = self.toIndex(column-tieCols//2,row)
                    instance = conf.createBuffer()
                    self.buffers[ index ][1] = instance 
                    trace( 540, '\tBuffer[{}]: {} @{}\n'.format(index,self.buffers[index],transf) )
                else:
                    instance = conf.createFeed()
                instance.setTransformation( transf )
                instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                length += instance.getMasterCell().getAbutmentBox().getWidth()
                if self._xRight is None:
                    self._xRight = x + length
                else:
                    self._xRight = max( self._xRight, x+length )
        blBufAb   = self.buffers[ 0][1].getAbutmentBox()
        trBufAb   = self.buffers[-1][1].getAbutmentBox()
        self.area = Box( blBufAb.getXMin(), blBufAb.getYMin()
                       , trBufAb.getXMax(), trBufAb.getYMax() )
        trace( 540, '-' )

    def _createTies ( self ):
        trace( 540, ',+', '\tQuadTree._createTies()\n' )
        conf        = self.quadTree.spares.conf
        sliceHeight = conf.sliceHeight 
        columns     = self.quadTree.area.getWidth() // u(60.0)
        if columns % 2: columns += 1
        stepX = self.quadTree.area.getWidth() // columns
        trace( 540, '\tcolumns:{}, stepX:{}\n' \
                    .format( columns, DbU.getValueString(stepX) ))
        y = self.quadTree.area.getYMin()
        while y < self.quadTree.area.getYMax():
            for column in range(columns):
                feed   = conf.feedsConf.createFeed( conf.corona )
                transf = self._getTransformation \
                             ( self.quadTree.area.getXMin() + stepX//2 + column*stepX, y )
                feed.setTransformation( transf )
                feed.setPlacementStatus( Instance.PlacementStatus.FIXED )
                trace( 540, '\tBulk tie: {}\n'.format(feed) )
                trace( 540, '\ttransf:{}\n'.format(transf) )
            y += sliceHeight
        trace( 540, ',-' )

    def _removeUnuseds ( self ):
        conf = self.quadTree.spares.conf
        for i in range(self.rows*self.columns):
            if not (self.buffers[i][0] & Spares.USED):
                trace( 540, '\tRemove Unused Buffer[{}]: {}\n'.format(i,self.buffers[i]) )
                cell           = self.buffers[i][1].getCell()
                transformation = self.buffers[i][1].getTransformation()
                gapWidth       = self.buffers[i][1].getMasterCell().getAbutmentBox().getWidth()
                self.buffers[i][1].destroy()
                conf.feedsConf.fillAt( cell, transformation, gapWidth )

    def _destroyBuffers ( self ):
        """Destroy all the buffer instances of the pool."""
        for flags, buffer in self.buffers:
            if buffer is not None:
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
        #area = spares.conf.cell.getAbutmentBox()
        #if spares.conf.isCoreBlock:
        #    area = spares.conf.core.getAbutmentBox()
        #    spares.conf.icore.getTransformation().applyOn( area )
        area = spares.conf.placeArea
        if area is None:
            area = spares.conf.cell.getAbutmentBox()
        if spares.conf.isCoreBlock:
            area = spares.conf.placeArea
            if area is None:
                area = spares.conf.core.getAbutmentBox()
            else:
                area = Box( area )
            spares.conf.icore.getTransformation().applyOn( area )
        root = QuadTree._create( spares, None, area, 'root', raiseError=True )
        root.rpartition()
        return root

    @staticmethod
    def isUsedArea ( spares, area, rtag, raiseError ):
        centerArea  = Box( area.getCenter() )
        sliceHeight = spares.conf.sliceHeight
        centerArea.inflate( 4*sliceHeight, sliceHeight )
        trace( 540, '\tQuadTree.isUnderArea(): {} {} of {}\n'.format(rtag,centerArea,spares.conf.cell) )
        trace( 540, '\t{}\n'.format( spares.conf.cellPnR ))
        for occurrence in spares.conf.cellPnR.getTerminalNetlistInstanceOccurrencesUnder( centerArea ):
            if not isinstance(occurrence.getEntity(),Instance):
                continue
            instance   = occurrence.getEntity()
            masterCell = instance.getMasterCell()
            if not masterCell.isTerminalNetlist():
                continue
            trace( 540, '\t| Overlap {}\n'.format(occurrence.getEntity()) )
            if raiseError:
                raise Error%essage( 1, [ 'QuadTree.create(): Unable to create QuadTree under area {}' \
                                         .format(area)
                                       , 'Area center is under fixed block {}' .format()
                                       ] )
            return True
        sys.stdout.flush()
        sys.stderr.flush()
        return False

    @staticmethod
    def _create ( spares, parent, area, tag, raiseError=False ):
        childRtag = parent.rtag+'_'+tag if parent else tag
        if QuadTree.isUsedArea( spares, area, childRtag, raiseError ):
            return None
        qt = QuadTree( spares, parent, area, childRtag )
        return qt

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
        self.rtag      = rtag
        self.rleafX    = [ area.getXCenter() ]

    def destroy ( self ):
        if self.bl: self.bl.destroy()
        if self.br: self.br.destroy()
        if self.tl: self.tl.destroy()
        if self.tr: self.tr.destroy()
        self.pool._destroyBuffers()

    def __str__ ( self ):
        if hasattr(self,'pool'):
            occupancy, capacity = self.pool.getUse()
            rtag = self.rtag
        else:
            occupancy = 'occ'
            capacity  = 'cap'
            rtag      = 'rtag'
        s = '<QuadTree [{},{} {},{}] {}/{} "{}">' \
            .format( DbU.getValueString(self.area.getXMin())
                   , DbU.getValueString(self.area.getYMin())
                   , DbU.getValueString(self.area.getXMax())
                   , DbU.getValueString(self.area.getYMax())
                   , occupancy
                   , capacity
                   , rtag )
        return s

    def __eq__ ( self, other ):
        return self.rtag == other.rtag

    def removeUnusedBuffers ( self ):
        if self.bl: self.bl.removeUnusedBuffers()
        if self.br: self.br.removeUnusedBuffers()
        if self.tl: self.tl.removeUnusedBuffers()
        if self.tr: self.tr.removeUnusedBuffers()
        self.pool._removeUnuseds()

    def getDistance ( self, point ):
        """Return the Manhattan distance between ``point`` and the area center."""
        return self.area.getCenter().manhattanDistance( point )

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
    def buffers ( self ):
        """The the currently selected set of buffer instances in the pool."""
        return self.pool.selecteds

    @property
    def bInputPlugs ( self ):
        """The list of input Plugs of the currently selected buffers in the pool."""
        plugs = []
        for buffer in self.buffers:
            plugs.append( utils.getPlugByName( buffer, self.spares.conf.bufferConf.input ))
        return plugs

    def bInputPlug ( self, index ):
        """The input Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffers[index], self.spares.conf.bufferConf.input )

    def bOutputPlug ( self, index ):
        """The output Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffers[index], self.spares.conf.bufferConf.output )

    def selectFree ( self ):
        """
        Returns the first free buffer *instance* in the pool or None if
        there isn't any left.
        """
        return self.pool.selectFree()

    def connectBuffer ( self, doLeaf=False ):
        """
        Create output nets for the currently selected buffers, if they do not
        already exists. The nets are created in the top level cell, and their
        names are derived from the `rtag` attribute.
        """
        if self.isLeaf() and not doLeaf: return

        trace( 540, '\tQuadTree.connectBuffer(): rtag:"{}"\n'.format(self.rtag) )
        for ibuffer in range(len(self.buffers)):
            plug = self.bOutputPlug(ibuffer)
            if not plug.getNet():
                outputNetBuff = Net.create( self.spares.conf.cellPnR,'{}_{}_{}' \
                                            .format(self.root.bufferTag,self.rtag,ibuffer) )
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
        else:
            if flags & Spares.HEAVY_LEAF_LOAD:
                self.pool.select( column+1, row  , flags )
                self.pool.select( column  , row+1, flags )
        trace( 540, '-' )

    def runselect ( self ):
        """
        Clear any previous buffer selection.
        """
        self.pool.unselect()
        if not self.isLeaf():
            for leaf in self.leafs: leaf.runselect()

    def _mergeLeafX ( self, leafX ):
        for x in leafX:
            if not (x in self.rleafX):
                self.rleafX.append( x )
            self.rleafX.sort()

    def _upMergeLeafX ( self ):
        if self.parent:
            self.parent._mergeLeafX( self.rleafX )
            self.parent._upMergeLeafX()

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
            #self.pool._createTies()
            trace( 540, '-' )
            return False

        if aspectRatio < 0.5:
            self.ycut = self.spares.toYSlice( self.area.getYMin() + self.area.getHeight()//2 )
            self.bl   = QuadTree._create( self.spares
                                        , self
                                        , Box( self.area.getXMin()
                                             , self.area.getYMin()
                                             , self.area.getXMax()
                                             , self.ycut )
                                        , 'bl' )
            self.tl   = QuadTree._create( self.spares
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
            self.xcut = self.spares.toXPitch( self.area.getXMin() + self.area.getWidth()//2 )
            self.bl   = QuadTree._create( self.spares
                                        , self
                                        , Box( self.area.getXMin()
                                             , self.area.getYMin()
                                             , self.xcut
                                             , self.area.getYMax() )
                                        , 'bl' )
            self.br   = QuadTree._create( self.spares
                                        , self
                                        , Box( self.xcut
                                             , self.area.getYMin()
                                             , self.area.getXMax()
                                             , self.area.getYMax() )
                                        , 'br' )
            trace( 540, '\tHorizontal bi-partition @X:{}\n'.format(DbU.getValueString(self.xcut)) )
            trace( 540, '-' )
            return True

        self.ycut = self.spares.toYSlice( self.area.getYMin() + self.area.getHeight()//2 )
        self.xcut = self.spares.toXPitch( self.area.getXMin() + self.area.getWidth ()//2 )
        self.bl   = QuadTree._create( self.spares
                                    , self
                                    , Box( self.area.getXMin()
                                         , self.area.getYMin()
                                         , self.xcut
                                         , self.ycut )
                                    , 'bl' )
        self.br   = QuadTree._create( self.spares
                                    , self
                                    , Box( self.xcut
                                         , self.area.getYMin()
                                         , self.area.getXMax()
                                         , self.ycut )
                                    , 'br' )
        self.tl   = QuadTree._create( self.spares
                                    , self
                                    , Box( self.area.getXMin()
                                         , self.ycut
                                         , self.xcut
                                         , self.area.getYMax() )
                                    , 'tl' )
        self.tr   = QuadTree._create( self.spares
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
        else:
            self._upMergeLeafX()
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
        candidate = None
        minDist   = None
        if self.bl:
            minDist   = self.bl.getDistance( position )
            candidate = self.bl
        if self.br:
            distance = self.br.getDistance( position )
            if (candidate is None) or (distance < minDist):
                minDist   = distance
                candidate = self.br
        if self.tl:
            distance = self.tl.getDistance( position )
            if (candidate is None) or (distance < minDist):
                minDist   = distance
                candidate = self.tl
        if self.tr:
            distance = self.tr.getDistance( position )
            if (candidate is None) or (distance < minDist):
                minDist   = distance
                candidate = self.tr
        return candidate.getLeafUnder(position)

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

    def getBottomLeafs ( self, bottoms=[] ):
        """
        Recursively build a list of all the QuadTree leaf cells at the bottom
        of the tree. Sorted left to right by X position.
        """
        if self.isLeaf():
            bottoms.append( self )
            return
        if   self.bl: self.bl.getBottomLeafs( bottoms )
        elif self.tl: self.tl.getBottomLeafs( bottoms )
        if   self.br: self.br.getBottomLeafs( bottoms )
        elif self.tr: self.tr.getBottomLeafs( bottoms )

    def getTopLeafs ( self, tops=[] ):
        """
        Recursively build a list of all the QuadTree leaf cells at the top
        of the tree. Sorted left to right by X position.
        """
        if self.isLeaf():
            tops.append( self )
            return
        if   self.tl: self.tl.getTopLeafs( tops )
        elif self.bl: self.bl.getTopLeafs( tops )
        if   self.tr: self.tr.getTopLeafs( tops )
        elif self.br: self.br.getTopLeafs( tops )

    def isUnderArea ( self, plugOccurrence ):
        return self.area.contains( plugOccurrence.getBoundingBox().getCenter() )

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

        If the plug list is empty, the buffer will still be used and it's
        output net created.
        """
        trace( 540, ',+', '\tQuadTree.spliNetlist()\n' )
        trace( 540, '\tPlug numbers {}\n'.format(len(self.plugs)) )
        trace( 540, '\tBuffers numbers {}\n'.format(len(self.buffers)) )
        self.connectBuffer( doLeaf=True )
        if not self.plugs:
            trace( 540, '-' )
            return
        coreTransf = Transformation()
        if self.spares.conf.isCoreBlock:
            coreTransf = self.spares.conf.icore.getTransformation()
        maxSinks        = self.spares.conf.bufferConf.maxSinks
       #maxSinks        = 7
        trace( 540, '\tconf.bufferConf.maxSinks={}\n'.format(maxSinks) )
        plugOccsByAngle = []
        areaCenter      = self.area.getCenter()
       #coreTransf.applyOn( areaCenter )
        trace( 540, '\tArea {}\n'.format(self.area) )
        trace( 540, '\tArea center {}\n'.format(areaCenter) )
        for plugOcc in self.plugs:
            instCenter = plugOcc.getEntity().getInstance().getAbutmentBox().getCenter()
            plugOcc.getPath().getTransformation().applyOn( instCenter )
            dx = instCenter.getX() - areaCenter.getX()
            dy = instCenter.getY() - areaCenter.getY()
            angle = math.atan2( dy, dx )
            plugOccsByAngle.append( [ angle, plugOcc ] )
        plugOccsByAngle.sort( key=itemgetter(0) )
        splitIndexes = []
        if (len(plugOccsByAngle) > maxSinks) and (len(self.buffers) > 1):
            partSize = len(plugOccsByAngle) // len(self.buffers)
            trace( 540, '\tpartSize: {}\n'.format(partSize) )
            for isplit in range(1,len(self.buffers)):
                maxdAngle = 0
                maxisplit = partSize*isplit-2
                for i in range(maxisplit,maxisplit+4):
                    dAngle = plugOccsByAngle[i+1][0] - plugOccsByAngle[i][0]
                    trace( 540, '\t{:-2} dAngle={}\n'.format(i,dAngle) )
                    if dAngle > maxdAngle:
                        maxdAngle = dAngle
                        maxisplit = i
                splitIndexes.append( maxisplit )
        splitIndexes.append( len(plugOccsByAngle) )
        for i in range(len(plugOccsByAngle)):
            angle, plugOcc = plugOccsByAngle[i]
            instCenter = plugOcc.getEntity().getInstance().getAbutmentBox().getCenter()
            plugOcc.getPath().getTransformation().applyOn( instCenter )
            dx = instCenter.getX()
            dy = instCenter.getY()
            trace( 540, '\t {:-2} | {:-5.2} : dx:{} dy:{} {}\n' \
                        .format(i,angle,DbU.getValueString(dx),DbU.getValueString(dy),plugOcc) )
        trace( 540, '\tspitIndexes = {}\n'.format(splitIndexes) )
        minIndex = 0
        for i in range(len(splitIndexes)):
            sinks    = splitIndexes[i] - minIndex
            minIndex = splitIndexes[i]
            if sinks > maxSinks:
                print( WarningMessage( 'QuadTree.splitNetlist(): More than {} sink points ({}) on "{}".' \
                                       .format( maxSinks
                                              , sinks
                                              , self.bOutputPlug(i).getNet().getName())) )
        ibuffer  = 0
        netBuff  = self.bOutputPlug(ibuffer).getNet()
        for i in range(len(plugOccsByAngle)):
            angle, plugOcc = plugOccsByAngle[i]
            if i > splitIndexes[ibuffer]:
                ibuffer += 1
                netBuff  = self.bOutputPlug(ibuffer).getNet()
                trace( 540, '\tBuffer: {}\n'.format(self.buffers[ibuffer]) )
                trace( 540, '\tBuffer output: {}\n'.format(netBuff) )
            trace( 540, '\t| Leaf: {}\n'.format(plugOcc) )
            trace( 540, '\t| netBuff: {}\n'.format(netBuff) )
            deepPlug    = self.spares.raddTransNet( netBuff, plugOcc.getPath() )
            trace( 540, '\t| netBuff: {}\n'.format(netBuff) )
            trace( 540, '\t| Deep Plug: {}\n'.format(deepPlug) )
            deepNetBuff = deepPlug.getMasterNet() if deepPlug else netBuff
            trace( 540, '\t| deepNetBuff: {} {}\n'.format(deepNetBuff,netBuff) )
            plugOcc.getEntity().setNet( deepNetBuff )
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
    synthesis (CTS) and high fanout net synthesis (HNFS). 4 buffers are created
    in each pool. Pools are set in a regular matrix over the whole abutment box.
    Excess area is put in the topmost and rightmost pools.
    """

    USED            = 0x00000001
    CHECK_USED      = 0x00010000
    MARK_USED       = 0x00020000
    HEAVY_LEAF_LOAD = 0x00040000
    ROUTE_ROOT_NET  = 0x00080000

    def __init__ ( self, block ):
        self.conf         = block.conf
        self.quadTree     = None
        self.strayBuffers = []

    @property
    def rleafX ( self ):
        if self.quadTree: return self.quadTree.rleafX
        return []

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
        areaLength   = spareSide * spareSide // self.conf.sliceHeight
        bufferLength = self.conf.bufferConf.width * self.conf.bColumns * self.conf.bRows
        if not areaLength:
            raise ErrorMessage( 3, 'Spares.getSpareSpaceMargin(): Spare leaf area is zero.' )
        return (float(bufferLength) * 1.4) / float(areaLength)
        
    def toXPitch ( self, x ):
        """
        Returns the coordinate of the pitch immediately inferior to X.
        If we are in a chip, compute coordinate in the *corona* system.
        """
        offset = 0
        area   = self.conf.coreAb
        if self.conf.isCoreBlock:
            offset = self.conf.icore.getTransformation().getTx()
            self.conf.icore.getTransformation().applyOn( area )
        modulo = (x - offset - area.getXMin()) % self.conf.sliceStep 
        return x - modulo

    def toYSlice ( self, y ):
        """
        Returns the coordinate of the slice immediately inferior to Y.
        If we are in a chip, compute coordinate in the *corona* system.
        """
        offset = 0
        area   = self.conf.coreAb
        if self.conf.isCoreBlock:
            offset = self.conf.icore.getTransformation().getTy()
            self.conf.icore.getTransformation().applyOn( area )
        #trace( 540, '\toffset:{}\n'.format(DbU.getValueString(offset)) )
        modulo = (y - offset - area.getYMin()) % self.conf.sliceHeight 
        return y - modulo

    def _getTransformation ( self, spareX, spareY ):
        """Transform (spareX,spareY) into sliced coordinates relatives to the corona."""
        yoffset = 0
        if self.conf.isCoreBlock:
            yoffset = self.conf.icore.getTransformation().getTy()
        sliceHeight = self.conf.sliceHeight 
        x           = self.toXPitch( spareX )
        y           = self.toYSlice( spareY )
        slice       = (y - yoffset) // sliceHeight
        orientation = Transformation.Orientation.ID
        y = slice * sliceHeight + yoffset
        if slice % 2:
            orientation = Transformation.Orientation.MY
            y          += sliceHeight
        transf = Transformation( x, y, orientation )
        return transf

    def _addCapTies ( self ):
        if self.conf.cfg.etesian.latchUpDistance is None:
            return
        trace( 540, ',+', '\tSpares._addCapTies()\n' )
        area = self.quadTree.area
        #area = self.conf.cell.getAbutmentBox()
        #if self.conf.isCoreBlock:
        #    area = self.conf.core.getAbutmentBox()
        #    self.conf.icore.getTransformation().applyOn( area )
        y           = area.getYMin()
        sliceHeight = self.conf.sliceHeight 
        tieWidth    = self.conf.feedsConf.tieWidth()
        trace( 540, '\tarea:{}, y:{}\n'.format( area, DbU.getValueString(y) ))
        while y < area.getYMax():
            capTie = self.conf.createFeed()
            capTie.setTransformation ( self._getTransformation(area.getXMin(),y) )
            capTie.setPlacementStatus( Instance.PlacementStatus.FIXED )
            trace( 540, '\t{} @{}\n'.format( capTie, capTie.getTransformation() ))
            capTie = self.conf.createFeed()
            capTie.setTransformation ( self._getTransformation(area.getXMax()-tieWidth,y) )
            capTie.setPlacementStatus( Instance.PlacementStatus.FIXED )
            trace( 540, '\t{} @{}\n'.format( capTie, capTie.getTransformation() ))
            y += sliceHeight
        trace( 540, ',-' )

    def build ( self ):
        if not self.conf.useSpares: return
        trace( 540, ',+', '\tSpares.build()\n' )
        if self.conf.cfg.block.spareSide < 7*self.conf.sliceHeight:
            raise ErrorMessage( 2, 'Spares.build(): Minimal block spare side ({}) must be greater than 7*sliceHeight ({}).' \
                                .format( DbU.getValueString(  self.conf.cfg.block.spareSide)
                                       , DbU.getValueString(7*self.conf.sliceHeight ) ))
        with UpdateSession():
            self.quadTree = QuadTree.create( self )
            #self._addCapTies()
            trace( 540, "\tX Centers of the QuadTree leaf\n" )
            for x in self.quadTree.rleafX:
                trace( 540, '\t| {}\n'.format(DbU.getValueString(x) ))
            self._buildPower()
        trace( 540, '-' )

    def _buildPower ( self ):
        trace( 540, '+', '\tSpares._buildPower()\n' )
        if not self.conf.powersConf.power: return
        bottoms = []
        self.quadTree.getBottomLeafs( bottoms )
        trace( 540, '\tBottom leafs of the QuadTree:\n' )
        for leaf in bottoms:
            trace( 540, '\t| {} xRight={}\n'.format( leaf, DbU.getValueString(leaf.pool._xRight) ))
        tops = []
        self.quadTree.getTopLeafs( tops )
        trace( 540, '\tTop leafs of the QuadTree:\n' )
        for leaf in tops:
            trace( 540, '\t| {} xRight={}\n'.format( leaf, DbU.getValueString(leaf.pool._xRight) ))
        if len(tops) != len(bottoms):
            print( ErrorMessage( 2, 'Spares._buildPower(): Discrepency between QuadTree top and bottom leafs lists, {} vs {}.'
                                    .format( len(tops), len(bottoms) )))
            trace( 540, '-' )
            return
        vRailsPeriod = self.conf.cfg.block.vRailsPeriod
        if vRailsPeriod is None:
            vRailsPeriod = 1
        if vRailsPeriod == 0:
            trace( 540, '-' )
            return
        sliceHeight = self.conf.sliceHeight 
        for column in range(len(bottoms)):
            if column % vRailsPeriod: continue
            x    = bottoms[column].pool._xRight
            y    = bottoms[column].area.getYMin()
            ymax = tops   [column].area.getYMax() - sliceHeight
            while y <= ymax:
                power = self.conf.createPower()
                power.setTransformation ( self._getTransformation(x,y) )
                power.setPlacementStatus( Instance.PlacementStatus.FIXED )
                y += sliceHeight
        trace( 540, '-' )

    def rshowPoolUse ( self ):
        if self.quadTree:
            self.quadTree.rshowPoolUse()

    def addStrayBuffer ( self, position ):
        """Add a new stray buffer at ``position``."""
        trace( 540, ',+', '\tSpares.addStrayBuffer()\n' )

        sliceHeight = self.conf.sliceHeight 
        x           = self.toXPitch( position.getX() )
        y           = self.toYSlice( position.getY() )
        slice       = y // sliceHeight
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

    def trackAvoid ( self, box ):
        """
        Protect a vertical track by putting a vertical column of narrow filler
        under it (usually tie). Used by the H-Tree (clock tree) in low-metal
        nodes.
        """
        trace( 540, ',+', '\tSpares.trackAvoid() {}\n'.format( box ))
        if self.conf.isCoreBlock:
            coreCell = self.conf.corona
        else:
            coreCell = self.conf.core
        yoffset = 0
        if self.conf.isCoreBlock:
            yoffset = self.conf.icore.getTransformation().getTy()
        sliceHeight = self.conf.sliceHeight 
        x           = self.toXPitch( box.getXMin() ) - self.conf.sliceStep
        ymin        = self.toYSlice( box.getYMin() )
        ymax        = self.toYSlice( box.getYMax() )
        sliceMin    = (ymin - yoffset) // sliceHeight
        sliceMax    = (ymax - yoffset) // sliceHeight
        orientation = Transformation.Orientation.ID
        ymin        = sliceMin * sliceHeight
        for row in range( sliceMin+1, sliceMax ):
            orientation = Transformation.Orientation.ID
            y = row * sliceHeight + yoffset
            if row%2:
                orientation = Transformation.Orientation.MY
                y          += sliceHeight
            transf   = Transformation( x, y, orientation )
            instance = self.conf.feedsConf.createFeed( coreCell )
            instance.setTransformation( transf )
            instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
            trace( 540, '\ttrackAvoid, feed: {} @{}\n'.format(instance,transf) )
        trace( 540, '-' )

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

        trace( 540, ',+', '\tSpares.raddTransNet() top:{} path:{}\n'.format(topNet,path) )
        if path.isEmpty():
            self.conf.addClonedCell( topNet.getCell() )
            trace( 540, '-' )
            return None
        tailPath     = path.getTailPath()
        headInstance = path.getHeadInstance()
        headPlug     = utils.getPlugByNet( headInstance, topNet )
        if not headPlug:
            masterCell = headInstance.getMasterCell()
            masterNet  = masterCell.getNet( topNet.getName() )
            trace( 540, '\tmasterCell {}\n'.format(masterCell) )
            if masterNet is None:
                trace( 540, '\tcreate Plug in {}\n'.format(headInstance) )
                masterNet = Net.create( masterCell, topNet.getName() )
                masterNet.setType     ( topNet.getType() )
                masterNet.setDirection( Net.Direction.IN )
            trace( 540, '\tmasterNet {}\n'.format(masterNet) )
            masterNet.setExternal( True )
            headPlug = headInstance.getPlug( masterNet )
            if not headPlug:
                raise ErrorMessage( 3, 'Plug not created for %s on instance %s of %s' \
                                    % (topNet.getName(),headInstance.getName(),masterCell.getName()) )
            headPlug.setNet( topNet )
            self.conf.addClonedCell( masterCell )
            self.conf.addClonedCell( headInstance.getCell() )
        else:
            masterNet = headPlug.getMasterNet()
        trace( 540, '\ttailPath {}\n'.format(tailPath) )
        if not tailPath.isEmpty():
            headPlug = self.raddTransNet( masterNet, tailPath )
        trace( 540, '-' )
        return headPlug

    def removeUnusedBuffers ( self ):
        if not self.quadTree: return
        with UpdateSession():
            self.quadTree.removeUnusedBuffers()

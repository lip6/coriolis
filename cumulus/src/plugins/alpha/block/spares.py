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
        self.columns       = quadTree.spares.state.bColumns
        self.rows          = quadTree.spares.state.bRows
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

        trace( 550, ',+', '\tBufferPool.select() column:{}, row={}, flags={:x}\n' \
               .format(column,row,flags) )
        if column >= self.columns:
            trace( 550, '-' )
            raise ErrorMessage( 3, 'BufferPool.select(): Column {} is out of range (max:{}).' \
                                   .format(column,self.columns) )
        if row >= self.rows:
            trace( 550, '-' )
            raise ErrorMessage( 3, 'BufferPool.select(): Row {} is out of range (max:{}).' \
                                   .format(row,self.rows) )
        self._select( self.toIndex( column, row ), flags )
        trace( 550, '-' )

    def _select ( self, index, flags ):
        self.selectedIndex = index
        selectedBuffer = self.buffers[ self.selectedIndex ]
        if flags & Spares.CHECK_USED and selectedBuffer[0] & Spares.USED:
            raise ErrorMessage( 3, 'BufferPool.select(): Buffer a index {} is already used.' \
                                   .format(self.selectedIndex) )
        if flags & Spares.MARK_USED:
            selectedBuffer[0] |= Spares.USED

    def selectFree ( self ):
        """Select the first free buffer available."""
        for i in range(self.rows*self.columns):
            if not (self.buffers[i][0] & Spares.USED):
                self._select( i, Spares.MARK_USED )

    def _createBuffers ( self ):
        """Create the matrix of instances buffer."""
        trace( 550, ',+', '\tBufferPool.createBuffers()\n' )

        state       = self.quadTree.spares.state
        sliceHeight = state.gaugeConf.sliceHeight 
        x           = self.quadTree.onXPitch( self.quadTree.area.getXCenter()
                                              - (state.bufferConf.width  * self.columns)/2 )
        y           = self.quadTree.onYSlice( self.quadTree.area.getYCenter()
                                              - (state.bufferConf.height * self.rows)/2 )
        slice = y / sliceHeight

        trace( 550, '\tSlice height: {}\n'.format(DbU.getValueString(sliceHeight)) )

        for row in range(self.rows):
            orientation = Transformation.Orientation.ID
            y = (slice+row) * sliceHeight
            if (slice+row)%2:
                orientation = Transformation.Orientation.MY
                y          += sliceHeight
            for column in range(self.columns):
                index    = self.toIndex(column,row)
                transf   = Transformation( x + column*state.bufferConf.width, y, orientation )
                instance = state.createBuffer()
                instance.setTransformation( transf )
                instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                self.buffers[ index ][1] = instance 
                trace( 550, '\tBuffer[{}]: {} @{}\n'.format(index,self.buffers[index],transf) )
        trace( 550, '-' )
        


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
        root = QuadTree( spares, None, spares.state.cell.getAbutmentBox() )
        root.rpartition()
        return root

    def __init__ ( self, spares, parent, area, rtag='root' ):
        self.spares    = spares
        self.area      = area
        self.xcut      = None
        self.ycut      = None
        self.parent    = parent
        self.bl        = None
        self.br        = None
        self.tl        = None
        self.tr        = None
        self.bufferTag = 'spare'
        self.bufferNet = None
        self.pool      = BufferPool( self )
        self.plugs     = []
        if self.parent and self.parent.rtag != '':
            self.rtag = self.parent.rtag + '_' + rtag
        else:
            self.rtag = rtag

    def __str__ ( self ):
        s = '<QuadTree [{},{} {},{}] "{}">'.format( DbU.getValueString(self.area.getXMin())
                                                  , DbU.getValueString(self.area.getYMin())
                                                  , DbU.getValueString(self.area.getXMax())
                                                  , DbU.getValueString(self.area.getYMax())
                                                  , self.rtag )
        return s

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

    @property
    def buffer ( self ):
        """The the currently selected buffer instance in the pool."""
        return self.pool.selected

    @property
    def bInputPlug ( self ):
        """The input Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.state.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.state.bufferConf.output )

    def onYSlice ( self, y ):
        """Returns the coordinate of the slice immediately inferior to Y."""
        modulo = (y - self.area.getYMin()) % self.spares.state.gaugeConf.sliceHeight 
        return y - modulo
        
    def onXPitch ( self, x ):
        """Returns the coordinate of the pitch immediately inferior to X."""
        modulo = (x - self.area.getXMin()) % self.spares.state.gaugeConf.sliceStep 
        return x - modulo

    def connectBuffer ( self, doLeaf=False ):
        """
        Create output nets for the currently selected buffer, if they do not
        already exists. The nets are created in the top level cell, and their
        names are derived from the `rtag` attribute.
        """
        if self.isLeaf() and not doLeaf: return

        trace( 550, '\tQuadTree.connectBuffer(): rtag:"{}"\n'.format(self.rtag) )
        plug = self.bOutputPlug
        if not plug.getNet():
            outputNetBuff = Net.create( self.spares.state.cell,'{}_{}' \
                                        .format(self.root.bufferTag,self.rtag) )
            plug.setNet( outputNetBuff )
            trace( 550, '\t| {}\n'.format(plug) )
            trace( 550, '\t| {}\n'.format(outputNetBuff) )

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
        trace( 550, '+' )
        if self.plugs:
            self.plugs = []
        self.pool.select( column, row, flags )
        if not self.isLeaf():
            for leaf in self.leafs: leaf.rselectBuffer( column, row, flags )
        trace( 550, '-' )

    def partition ( self ):
        """
        Build one level of the quad-tree, if the side of the area is bigger than
        twice the "block.spareSide" value.

        Depending on the initial aspect ratio, the first levels *may* not be a
        quad-tree, but only a vertical or horizontal bi-partition.
        """
        trace( 550, ',+', '\tQuadTree.partition(): {} (spareSide:{})\n' \
                    .format(self.area, DbU.getValueString(self.spares.state.cfg.block.spareSide)) )

        spareSide   = self.spares.state.cfg.block.spareSide
        sliceHeight = self.spares.state.gaugeConf.sliceHeight
        side        = float(spareSide)
        aspectRatio = float(self.area.getWidth()) / float(self.area.getHeight())

        if self.area.getHeight() < side*2.0 or self.area.getWidth () < side*2.0:
            trace( 550, '-' )
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
            trace( 550, '\tVertical bi-partition @Y:{}\n'.format(DbU.getValueString(self.ycut)) )
            trace( 550, '-' )
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
            trace( 550, '\tHorizontal bi-partition @X:{}\n'.format(DbU.getValueString(self.xcut)) )
            trace( 550, '-' )
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

        trace( 550, '\tQuadri-partition @X:{} + @Y:{}\n'\
               .format(DbU.getValueString(self.xcut),DbU.getValueString(self.ycut)) )
        trace( 550, '-' )
        return True
                
    def rpartition ( self ):
        """"[R]ecursively partition the the area."""
        trace( 550, ',+', '\tQuadTree.rpartition(): {}\n'.format(self.area) )
        if self.partition():
            for leaf in self.leafs:
                trace( 550, '\tLeaf rtag:"{}"\n'.format(leaf.rtag) )
                leaf.rpartition()
        trace( 550, '-' )

    def getLeafUnder ( self, position ):
        """Find the QuadTree leaf under `position`."""
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

        trace( 550, ',+', '\tQuadTree.spliNetlist()\n' )
        self.connectBuffer( doLeaf=True )
        netBuff = self.bOutputPlug.getNet()
        trace( 550, '\tBuffer: {}\n'.format(self.buffer) )
        trace( 550, '\tBuffer output: {}\n'.format(netBuff) )
        for plug in self.plugs:
            trace( 550, '\t| Leaf: {}\n'.format(plug) )
            trace( 550, '\t| netBuff: {}\n'.format(netBuff) )
            deepPlug    = self.spares.raddTransNet( netBuff, plug.getPath() )
            trace( 550, '\t| netBuff: {}\n'.format(netBuff) )
            trace( 550, '\t| Deep Plug: {}\n'.format(deepPlug) )
            deepNetBuff = deepPlug.getMasterNet() if deepPlug else netBuff
            trace( 550, '\t| deepNetBuff: {} {}\n'.format(deepNetBuff,netBuff) )
            plug.getEntity().setNet( deepNetBuff )

        maxSinks = self.spares.state.bufferConf.maxSinks
        if len(self.plugs) > maxSinks:
            print( WarningMessage( 'QuadTree.splitNetlist(): More than {} sink points ({}) on "{}".' \
                                   .format(maxSinks,len(self.plugs),netBuff.getName())) )
        trace( 550, '-' )

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
        self.state    = block.state
        self.quadTree = None
        self.cloneds  = []

    def getSpareSpaceMargin ( self ):
        """
        Compute the percentage of margin space to compensate for the 4 spare
        buffers.
        """
        if not self.state.useSpares: return 0.0
        spareSide    = self.state.cfg.block.spareSide
        if not spareSide:
            raise ErrorMessage( 3, 'Spares.getSpareSpaceMargin(): "block.spareSide" parameter is zero ({}).' \
                                   .format(spareSide) )
        areaLength   = spareSide * spareSide / self.state.gaugeConf.sliceHeight
        bufferLength = self.state.bufferConf.width * self.state.bColumns * self.state.bRows
        if not areaLength:
            raise ErrorMessage( 3, 'Spares.getSpareSpaceMargin(): Spare leaf area is zero.' )
        return float(bufferLength) / float(areaLength)

    def toXGCellGrid ( self, x ):
        """Find the nearest X (inferior) on the Cell gauge grid (sliceStep)."""
        dx = x - self.state.xMin
        return self.state.xMin + (dx - dx % self.state.gaugeConf.sliceStep)

    def toYGCellGrid ( self, y ):
        """Find the nearest Y (inferior) on the Cell gauge grid (sliceHeight)."""
        dy = y - self.state.yMin
        return self.state.yMin + (dy - dy % self.state.gaugeConf.sliceHeight)

    def build ( self ):
        if not self.state.useSpares: return
        trace( 550, ',+', '\tSpares.build()\n' )
        with UpdateSession():
            self.quadTree = QuadTree.create( self )
        trace( 550, '-' )

    def addClonedCell ( self, masterCell ):
        if not masterCell in self.cloneds: self.cloneds.append( masterCell )
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

        if path.isEmpty(): return None
        
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
  
    def save ( self, topCell ):
        """
        Frontend to Spares.rsave(). Append the "_cts" suffix to the cloned
        cells, then call rsave().
        """
        for cell in self.cloneds:
            cell.setName( cell.getName()+'_cts' )
        self.rsave( topCell )
        return
      

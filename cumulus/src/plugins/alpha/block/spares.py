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
        root.rcreateBuffers()
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
        self.buseds    = 0
        self.bufferTag = 'spare'
        self.bufferNet = None
        self.buffers   = []
        self.plugs     = []
        if self.parent and self.parent.rtag != '':
            self.rtag = self.parent.rtag + '_' + rtag
        else:
            self.rtag = rtag

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

    def isLeaf ( self ): 
        for leaf in (self.bl, self.br, self.tl, self.tr):
            if leaf is not None: return False
        return True

    @property
    def buffer ( self ):
        """The the currently selected buffer instance in the pool."""
        return self.buffers[self.buseds]

    @property
    def bInputPlug ( self ):
        """The input Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.state.bufferConf.input )

    @property
    def bOutputPlug ( self ):
        """The output Plug of the currently selected buffer in the pool."""
        return utils.getPlugByName( self.buffer, self.spares.state.bufferConf.output )

    def useBuffer ( self, doLeaf=False ):
        """
        Create output nets for the currently selected buffer, if they do not
        already exists. The nets are created in the top level cell, and their
        names are derived from the `rtag` attribute.
        """
        if self.isLeaf() and not doLeaf: return

        trace( 550, '\tQuadTree.useBuffer(): rtag:"{}"\n'.format(self.rtag) )
        plug = self.bOutputPlug
        if not plug.getNet():
            outputNetBuff = Net.create( self.spares.state.cell,'{}_{}_{}' \
                                        .format(self.root.bufferTag,self.buseds,self.rtag) )
            plug.setNet( outputNetBuff )
            trace( 550, '\t| {}\n'.format(plug) )
            trace( 550, '\t| {}\n'.format(outputNetBuff) )

    def ruseBuffer ( self ):
        """Recursive call of useBuffer()"""
        self.useBuffer()
        for leaf in self.leafs:
            leaf.ruseBuffer()

    def useNextBuffer ( self ):
        """
        Reset the quadtree buffering cache in order to buffer the next net.
        Flush the plugs list and increase the buffer counter (buseds).
        """
        if self.plugs:
            self.plugs   = []
            self.buseds += 1
        if not self.isLeaf():
            for leaf in self.leafs: leaf.useNextBuffer()

    def partition ( self ):
        trace( 550, ',+', '\tQuadTree.partition(): {}\n'.format(self.area) )

        spareSide   = self.spares.state.cfg.block.spareSide
        sliceHeight = self.spares.state.gaugeConf.sliceHeight
        side        = float(spareSide * sliceHeight)
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
        trace( 550, ',+', '\tQuadTree.rpartition(): {}\n'.format(self.area) )
        if self.partition():
            for leaf in self.leafs:
                trace( 550, '\tLeaf rtag:"{}"\n'.format(leaf.rtag) )
                leaf.rpartition()
        trace( 550, '-' )

    def createBuffers ( self ):
        trace( 550, ',+', '\tQuadTree.createBuffers()\n' )
       #if not self.isLeaf(): return

        state = self.spares.state
        x     = self.area.getXCenter() - state.bufferConf.width
        y     = self.area.getYCenter() - state.bufferConf.height
        slice = y / state.gaugeConf.sliceHeight - 1 

        for slice in range(slice,slice+2):
            orientation = Transformation.Orientation.ID
            yadjust     = 0
            if slice%2:
                orientation = Transformation.Orientation.MY
                yadjust     = 2 * state.gaugeConf.sliceHeight
            for i in range(2):
                instance = state.createBuffer()
                instance.setTransformation( Transformation( x + i*state.bufferConf.width
                                                          , y + yadjust
                                                          , orientation ) )
                instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
                self.buffers.append( instance )
                trace( 550, '\tBuffer: {}\n'.format(self.buffers[-1]) )
        trace( 550, '-' )

    def rcreateBuffers ( self ):
        self.createBuffers()
        trace( 550, ',+' )
        if self.bl: self.bl.rcreateBuffers()
        if self.br: self.br.rcreateBuffers()
        if self.tl: self.tl.rcreateBuffers()
        if self.tr: self.tr.rcreateBuffers()
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
        self.useBuffer( doLeaf=True )
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
        areaLength   = spareSide * spareSide / self.state.gaugeConf.sliceHeight
        bufferLength = self.state.bufferConf.width * 4
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
      

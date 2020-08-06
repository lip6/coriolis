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
# |  Python      :       "./plugins/block/block.py"                 |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import os.path
import Cfg
from   Hurricane import Breakpoint
from   Hurricane import DbU
from   Hurricane import Box
from   Hurricane import Transformation
from   Hurricane import Point
from   Hurricane import Box
from   Hurricane import Path
from   Hurricane import Layer
from   Hurricane import Occurrence
from   Hurricane import Net
from   Hurricane import NetExternalComponents
from   Hurricane import RoutingPad
from   Hurricane import Pad
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
import Etesian
import Anabatic
import Katana
import plugins.rsave
from   plugins                   import getParameter
from   alpha.block.spares        import Spares
from   alpha.block.clocktree     import ClockTree
from   alpha.block.configuration import IoPin
from   alpha.block.configuration import BlockState


# ----------------------------------------------------------------------------
# Class  :  "block.Side".

class Side ( object ):
    """
    Collate the informations about all the IoPin on a side of a block
    and perform pins creation & placement.
    """

    def __init__ ( self, block, side ):
        self.block      = block
        self.side       = side
        self.pinSpecs   = []
        self.expandPins = True
        self.pins       = {}

    @property
    def pinDirection ( self ):
        """Translate the IoPin side into a Pin direction."""
        if   self.side & IoPin.WEST:  return Pin.Direction.WEST
        elif self.side & IoPin.EAST:  return Pin.Direction.EAST
        elif self.side & IoPin.SOUTH: return Pin.Direction.SOUTH
        else:                         return Pin.Direction.NORTH

    def setupAb ( self ):
        """
        Initialise the side coordinate from the block abutmeent box.
        Needless to say that it must be called *after* the abutment
        box has been setup.
        """
        if self.side & IoPin.WEST:
            self.gauge   = self.block.state.gaugeConf.hDeepRG
            self.ubegin  = self.block.state.yMin
            self.uend    = self.block.state.yMax
            self.sidePos = self.block.state.xMin
        elif self.side & IoPin.EAST:
            self.gauge   = self.block.state.gaugeConf.hDeepRG
            self.ubegin  = self.block.state.yMin
            self.uend    = self.block.state.yMax
            self.sidePos = self.block.state.xMax
        elif self.side & IoPin.SOUTH:
            self.gauge   = self.block.state.gaugeConf.vDeepRG
            self.ubegin  = self.block.state.xMin
            self.uend    = self.block.state.xMax
            self.sidePos = self.block.state.yMin
        elif self.side & IoPin.NORTH:
            self.gauge   = self.block.state.gaugeConf.vDeepRG
            self.ubegin  = self.block.state.xMin
            self.uend    = self.block.state.xMax
            self.sidePos = self.block.state.yMax

    def getNextPinPosition ( self, flags, upos, ustep ):
        """
        Return the point of the next available Pin position on
        the side.

        :param flags: Whether to find the next position from the
                      begin of the side (A_BEGIN) or the end (A_END).
        :param upos:  Force the position to use, in that cas the
                      ``flags`` argument is  ignored. To let the
                      function choose, must be set to zero.
        :param ustep: The distance to space from the last created Pin.
        """
        if not upos:
            if not ustep: ustep = self.gauge.getPitch()
            while True:
                if flags & IoPin.A_BEGIN:
                    self.ubegin += ustep
                    pinOffset = self.ubegin
                    if not self.pins.has_key(self.ubegin):
                        break
                else: 
                    self.uend -= ustep
                    pinOffset = self.uend
                    if not self.pins.has_key(self.uend):
                        break
        else:
            pinOffset = upos
        if (self.side & IoPin.WEST) or (self.side & IoPin.EAST):
            return Point( self.sidePos, pinOffset )
        return Point( pinOffset, self.sidePos )

    def append ( self, pin ):
        """
        Append a newly created Pin to a map indexed by the position along
        the side. Create a list for each entry. If everything goes well
        each list will exacly one entry.

        This function is still too simple. It will consider two Pin of
        different layers as overlapping.
        """
        if (self.side & IoPin.WEST) or (self.side & IoPin.EAST):
            upos = pin.getY()
        else:
            upos = pin.getX()
        if not self.pins.has_key(upos):
            self.pins[upos] = [ pin ]
        else:
            self.pins[upos].append( pin )

    def place ( self, ioPin ):
        """
        Performs the actual creation of the Pin from it's ioPin specification.
        Should be called prior to any call to ```Etesian.place()```, so the Pin
        is taken into account when building the RoutingPads. Returns the number
        of *failed* pins, so zero means that all went well... Currently only
        check for out of bounds coordinates.
        """

        status = 0
        if self.side & (IoPin.NORTH | IoPin.SOUTH):
            gauge = self.block.state.gaugeConf.vDeepRG
            upos  = ioPin.upos
            for index in ioPin.indexes:
                pinName  = ioPin.stem.format( index )
                net      = self.block.state.cell.getNet( pinName )
                if net is None:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): No net named "{}".'.format(pinName) ] ))
                    continue
                pinName += '.{}'.format(self.block.state.getIoPinsCounts(net))
                pinPos   = self.getNextPinPosition( ioPin.flags, upos, ioPin.ustep )
                if pinPos.getX() > self.block.state.xMax or pinPos.getX() < self.block.state.xMin:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): Pin "{}" is outside north or south abutment box side.' \
                                              .format(pinName)
                                            , '(x:{}, xAB: [{}:{}])' \
                                              .format( DbU.getValueString(pinPos.getX())
                                                     , DbU.getValueString(self.block.state.xMin)
                                                     , DbU.getValueString(self.block.state.xMax) ) ] ))
                    status += 1
                trace( 550, '\tIoPin.place() N/S @{} "{}" of "{}".\n'.format(pinPos,pinName,net) )
                pin = Pin.create( net
                                , pinName
                                , self.pinDirection
                                , Pin.PlacementStatus.FIXED
                                , gauge.getLayer()
                                , pinPos.getX()
                                , pinPos.getY()
                                , gauge.getWireWidth()
                                , gauge.getWireWidth() / 2
                                )
                NetExternalComponents.setExternal( pin )
                self.append( pin )
                self.block.state.incIoPinsCounts( net )
                if upos: upos += ioPin.ustep
        else:
            gauge = self.block.state.gaugeConf.hDeepRG
            upos  = ioPin.upos
            for index in ioPin.indexes:
                pinName  = ioPin.stem.format(index)
                net      = self.block.state.cell.getNet( pinName )
                if net is None:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): No net named "{}".'.format(pinName) ] ))
                    continue
                pinName += '.{}'.format(self.block.state.getIoPinsCounts(net))
                pinPos   = self.getNextPinPosition( ioPin.flags, upos, ioPin.ustep )
                if pinPos.getY() > self.block.state.yMax or pinPos.getY() < self.block.state.yMin:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): Pin "{}" is outside east or west abutment box side.' \
                                              .format(pinName)
                                              , '(y:{}, yAB: [{}:{}])' \
                                              .format( DbU.getValueString(pinPos.getY())
                                                     , DbU.getValueString(block.state.yMin)
                                                     , DbU.getValueString(block.state.yMax)) ] ))
                    status += 1
                trace( 550, '\tIoPin.place() E/W @{} "{}" of "{}".\n'.format(pinPos,pinName,net) )
                pin = Pin.create( net
                                , pinName
                                , self.pinDirection
                                , Pin.PlacementStatus.FIXED
                                , gauge.getLayer()
                                , pinPos.getX()
                                , pinPos.getY()
                                , gauge.getWireWidth() / 2
                                , gauge.getWireWidth()
                                )
                NetExternalComponents.setExternal( pin )
                self.append( pin )
                self.block.state.incIoPinsCounts( net )
                if upos: upos += ioPin.ustep
        return status

    def expand ( self ):
        """
        After, and only after routing the block, move the I/O pins *outside*
        of the abutment box. THey will stick out for one pitch.
        """
        if not self.expandPins: return
        rg  = self.block.state.gaugeConf.routingGauge
        for pinsAtPos in self.pins.values():
            for pin in pinsAtPos:
                for lg in rg.getLayerGauges():
                    if lg.getLayer().getMask() == pin.getLayer().getMask():
                        offset = lg.getPitch()
                        if   self.side & IoPin.WEST:  pin.setX( pin.getDx()-offset )
                        elif self.side & IoPin.EAST:  pin.setX( pin.getDx()+offset )
                        elif self.side & IoPin.SOUTH: pin.setY( pin.getDy()-offset )
                        elif self.side & IoPin.NORTH: pin.setY( pin.getDy()+offset )

    def checkOverlaps ( self ):
        """
        Check for Pin overlap. For now we are only checking that the are not at the
        exact same position. We should check for neighboring overlap (Pin too close
        from each other, not only exactly at the position).
        """
        if   self.side & IoPin.EAST:  sideName = 'EAST'
        elif self.side & IoPin.WEST:  sideName = 'WEST'
        elif self.side & IoPin.SOUTH: sideName = 'SOUTH'
        elif self.side & IoPin.NORTH: sideName = 'NORTH'
        for upos in self.pins.keys():
            count = len(self.pins[upos])
            if count > 1:
                pinNames = self.pins[upos][0].getName()
                for pin in self.pins[upos][1:]:
                    pinNames += ', ' + pin.getName()
                print( ErrorMessage( 1, [ 'Side.checkOverlap(): On {} side of block "{}", {} pins ovelaps.' \
                                          .format(sideName,self.block.state.cell.getName(),count)
                                        , '(@{}: {})' \
                                          .format(DbU.getValueString(upos),pinNames) ] ) )


# ----------------------------------------------------------------------------
# Class  :  "block.Block".

class Block ( object ):
    """
    The workhorse for turning a Cell into a hierarchically reusable placed and
    routed block. Provide support for:

    * Clock tree.
    """

    LUT = {}

    @staticmethod
    def lookup ( cell ):
        if Block.LUT.has_key(cell): return Block.LUT[cell]
        return None

    @staticmethod
    def create ( cell, ioPins=[] ):
        """Create a Block and it's configuration object."""
        block = Block( BlockState( cell, ioPins ) )
        Block.LUT[ cell ] = block
        return block

    def __init__ ( self, state ):
        """Not to be used directly, please see Block.create()."""
        self.flags          = 0
        self.state          = state
        self.spares         = Spares( self )
        self.clockTrees     = []
        self.blockInstances = []
        self.sides          = { IoPin.WEST  : Side( self, IoPin.WEST  )
                              , IoPin.EAST  : Side( self, IoPin.EAST  )
                              , IoPin.SOUTH : Side( self, IoPin.SOUTH )
                              , IoPin.NORTH : Side( self, IoPin.NORTH )
                              }
        if not self.state.cell.getAbutmentBox().isEmpty():
            print( '  o  Block "{}" is already done, reusing layout.' \
                   .format(self.state.cell.getName()) )
            self.state.cell.setTerminalNetlist( True )
            self.state.isBuilt = True
        else:
            print( '  o  Block "{}" will be generated.' \
                   .format(self.state.cell.getName()) )

    def setUnexpandPins ( self, sides ):
        """
        Prevent Pins from the selected sides to be stick out of one pitch.
        This may be useful if you plan to route by abutment on a side.
        """
        for side in (IoPin.WEST, IoPin.EAST, IoPin.SOUTH, IoPin.NORTH):
            if sides & side: self.sides[side].expandPins = False

    def setupAb ( self ):
        """
        Compute and set the Block's abutment box. Three cases:
        1. The AB is already set. assume the user as already set it by itself
           so do nothing.
        2. The block has *routed* sub-blocks. Then the width and height of the
           AB must be specified, and the placement of the sub-blocks, as the
           placer cannot guess them. P&R block are placed and any unplaced
           block has it's AB & position set to be the same as the one of the
           top cell.
        3. Otherwise, let the placer (Etesian) compute the size accorging the
           various configuration parameters (aspect ratio, space margin, fixed
           height or width, ...).
        """
        if not self.state.cell.getAbutmentBox().isEmpty():
            pass
        elif len(self.blockInstances):
            with UpdateSession():
                ab = Box( 0, 0, self.state.fixedWidth, self.state.fixedHeight )
                self.state.cell.setAbutmentBox( ab )
                for occurrence in self.state.cell.getNonTerminalNetlistInstanceOccurrences():
                    instance = occurrence.getEntity()
                    subCell  = instance.getMasterCell()
                    subCell.setAbutmentBox( ab )
                for occurrence in self.state.cell.getNonTerminalNetlistInstanceOccurrences():
                    instance = occurrence.getEntity()
                    instance.setTransformation( Transformation() )
                for blockInstance in self.blockInstances:
                    blockInstance.place()
        else:
            sysSpaceMargin   = self.state.cfg.etesian.spaceMargin
            blockSpaceMargin = sysSpaceMargin + self.spares.getSpareSpaceMargin()
            self.state.cfg.etesian.spaceMargin = blockSpaceMargin
            self.state.cfg.apply()
            with UpdateSession():
                etesian = Etesian.EtesianEngine.create( self.state.cell )
                if self.state.fixedWidth:  etesian.setFixedAbWidth ( self.state.fixedWidth  )
                if self.state.fixedHeight: etesian.setFixedAbHeight( self.state.fixedHeight )
                etesian.setDefaultAb()
                etesian.destroy()
            self.state.cfg.etesian.spaceMargin = sysSpaceMargin
            self.state.cfg.apply()
        for side in self.sides.values(): side.setupAb()

    def addClockTree ( self, clockNet=None ):
        """Create the trunk of the clock tree (recursive H-Tree)."""
        if not clockNet:
            for net in self.state.cell.getNets():
                if net.isClock():
                    clockNet = net
                    break
            if not clockNet:
                raise ErrorMessage( 3, 'Block.clockTree(): Cell "{}" has no clock net.'.format(self.state.cell.getName()) )
        with UpdateSession():
            self.clockTrees.append( ClockTree(self.spares,clockNet) )
            self.clockTrees[-1].buildHTree()
        return self.clockTrees[-1]

    def splitClocks ( self ):
        """
        Break the clock net and attach all it's Pins to the closest leaf
        if the H-Tree.
        """
        for clockTree in self.clockTrees:
            clockTree.splitClock()

    def placeIoPins ( self ):
        """
        Place the Pins on all the sides. Raise an exception in case of failure.
        (mainly due to Pins outside the side range)
        """
        faileds = 0
        with UpdateSession():
            for ioPin in self.state.ioPins:
                if   ioPin.flags & IoPin.SOUTH: side = self.sides[IoPin.SOUTH]
                elif ioPin.flags & IoPin.NORTH: side = self.sides[IoPin.NORTH]
                elif ioPin.flags & IoPin.EAST:  side = self.sides[IoPin.EAST ]
                else:                           side = self.sides[IoPin.WEST ]
                faileds += side.place( ioPin )
        if faileds:
            raise ErrorMessage( 3, 'Block.placeIoPins(): Cell "{}" has {} badly placed pins.' \
                                   .format(self.state.cell.getName(),faileds) )

    def checkIoPins ( self ):
        """
        Check for Pin overlap and that all external Net have at least one Pin.
        """
        for side in self.sides.values():
            side.checkOverlaps()
        for net in self.state.cell.getNets():
            if not net.isExternal(): continue
            if net.isSupply(): continue
            hasPins = False
            for pin in net.getPins():
                hasPins = True
                break
            if not hasPins:
                print( ErrorMessage( 1, 'Block.checkIoPins() External net "{}" has no pin(s).' \
                                        .format(net.getName() )))

    def expandIoPins ( self ):
        with UpdateSession():
            for side in self.sides.values():
                side.expand()

    def place ( self ):
        etesian = Etesian.EtesianEngine.create( self.state.cell )
        etesian.place()
        etesian.destroy()

    def route ( self ):
        katana = Katana.KatanaEngine.create( self.state.cell )
       #katana.printConfiguration   ()
        katana.digitalInit          ()
       #katana.runNegociatePreRouted()
        katana.runGlobalRouter      ( Katana.Flags.NoFlags )
        katana.loadGlobalRouting    ( Anabatic.EngineLoadGrByNet )
        katana.layerAssign          ( Anabatic.EngineNoNetLayerAssign )
        katana.runNegociate         ( Katana.Flags.NoFlags )
        success = katana.isDetailedRoutingSuccess()
       #Breakpoint.stop( 0, 'Block.route() done, success:{}.'.format(success) )
        katana.finalizeLayout()
        katana.destroy()
        return success

    def addBlockages ( self ):
        with UpdateSession():
            net = self.state.cell.getNet( 'blockagenet' )
            ab  = self.state.cell.getAbutmentBox()
            rg  = self.state.gaugeConf.routingGauge
            for lg in rg.getLayerGauges():
                if lg.getType() == RoutingLayerGauge.PinOnly: continue
                blockage = lg.getBlockageLayer()
                if not blockage: continue
                if lg.getDirection() == RoutingLayerGauge.Horizontal:
                    dxBorder = lg.getPitch() + lg.getWireWidth()
                    Horizontal.create( net
                                     , blockage
                                     , ab.getCenter().getY()
                                     , ab.getHeight()
                                     , ab.getXMin() + dxBorder
                                     , ab.getXMax() - dxBorder
                                     )
                else:
                    dyBorder = lg.getPitch() + lg.getWireWidth()
                    Vertical.create( net
                                   , blockage
                                   , ab.getCenter().getX()
                                   , ab.getWidth()
                                   , ab.getYMin() + dyBorder
                                   , ab.getYMax() - dyBorder
                                   )

    def build ( self ):
        """
        Perform all the steps required to build the layout of the block.
        The first step is to build all the blockInstance it depends upon,
        so they will appear as ``NetListTerminal`` and we can place them
        in their parent cell.
        """
        editor = self.state.editor
        print( '  o  Builing block "{}".'.format(self.state.cell.getName()) )
        for blockInstance in self.blockInstances:
            blockInstance.block.state.editor = editor
            if not blockInstance.block.state.isBuilt:
                print( '     - Build sub-block "{}".' \
                       .format(blockInstance.block.state.cell.getName()) )
                blockInstance.block.build()
        if editor: editor.setCell( self.state.cell )
        self.state.cfg.apply()
        self.setupAb()
        self.placeIoPins()
        self.checkIoPins()
        self.spares.build()
        if editor: editor.fit()
        if self.state.useClockTree: self.addClockTree()
        self.place()
        if self.state.useClockTree: self.splitClocks()
        status = self.route()
        self.addBlockages()
        self.expandIoPins()
        self.state.isBuilt = True
        plugins.rsave.rsave( self.state.cell )
        return status

    def useBlockInstance ( self, instancePathName , transf ):
        """
        Make a sub-block known to the top block. Can be trans-hierarchical.

        :param instancePathName: A string of instances names, separated by
                                 dots ('.') pinpointing the instance of the
                                 block.
        :param transf:           The position to place the instance of the
                                 sub-block.

        Prior to calling this function, a block *must* have been created for
        the master cell of the instance. But it need not to have been placed
        and routed.
        """
        iNames = instancePathName.split('.')
        path   = Path()
        for iName in iNames:
            if path.isEmpty(): parentCell = self.state.cell
            else:              parentCell = path.getTailInstance().getMasterCell()
            instance = parentCell.getInstance( iName )
            if not instance:
                raise ErrorMessage( 1, [ 'Block.useBlockInstance(): Cell "{}" has no instance "{}".' \
                                         .format(parentCell.getName(),iName)
                                       , '(in path:"{})"' \
                                         .format(instancePathName) ] )
            path = Path( path, instance )
        
        tailInstance = path.getTailInstance()
        for blockIns in self.blockInstances:
            if blockIns.instance == tailInstance:
                print( ErrorMessage('Block.useBlockInstance(): In Cell "{}", duplicate call for "{}"' \
                                    .format(self.cell.getName(),tailInstance.getName()) ))
                return
        blockIns = BlockInstance( tailInstance, transf )
        self.blockInstances.append( blockIns )


# ----------------------------------------------------------------------------
# Class  :  "blockinstance.BlockInstance".

class BlockInstance ( object ):
    """
    Light helper class to hold a sub-block information.
    """

    def __init__ ( self, instance, transf ):
        self.transf   = transf
        self.instance = instance
        self.block    = Block.lookup( instance.getMasterCell() )
        if not self.block:
            raise ErrorMessage( 1, [ 'BlockInstance.__init__(): Instance "{}" of cell "{}" has no Block defined.' \
                                     .format( instance.getName(), instance.getMasterCell().getName() )
                                   ] )
        return

    def place ( self ):
        self.instance.setTransformation( self.transf )
        self.instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
        return


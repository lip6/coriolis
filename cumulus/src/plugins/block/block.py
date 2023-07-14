#
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
# |  Python      :       "./plugins/block/block.py"                 |
# +-----------------------------------------------------------------+


import sys
import os.path
from   copy               import deepcopy
from   ...                import Cfg
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Point, \
                                 Box, Path, Layer, Occurrence, Net,           \
                                 NetExternalComponents, RoutingPad, Pad,      \
                                 Horizontal, Vertical, Contact, Pin, Plug,    \
                                 Cell, Instance
from   ...CRL             import AllianceFramework, RoutingLayerGauge
from   ...helpers         import trace, dots, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ...                import Etesian, Anabatic, Katana
from   ..                 import getParameter
from   ..macro.macro      import Macro
from   .                  import timing
from   .spares            import Spares
from   .htree             import HTree
#from   . hfns1            import BufferTree
#from   . hfns2            import BufferTree
#from   . hfns3            import BufferTree
from   .hfns4             import BufferTree
from   .configuration     import IoPin, BlockConf, GaugeConf

timing.staticInit()


# ----------------------------------------------------------------------------
# Class  :  "block.Side".

class Side ( object ):
    """
    Collate the informations about all the IoPin on a side of a block
    and perform pins creation & placement.
    """

    def __init__ ( self, conf, side ):
        self.conf       = conf
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

    def destroy ( self ):
       #with UpdateSession():
       #    for pinsAtPos in self.pins.values():
       #        for pin in pinsAtPos:
       #            pin.destroy()
        self.pins = {}

    def setupAb ( self ):
        """
        Initialise the side coordinate from the block abutmeent box.
        Needless to say that it must be called *after* the abutment
        box has been setup.
        """
        if self.side & IoPin.WEST:
            self.gauge   = self.conf.hDeepRG
            self.ubegin  = self.conf.yMin
            self.uend    = self.conf.yMax
            self.sidePos = self.conf.xMin
        elif self.side & IoPin.EAST:
            self.gauge   = self.conf.hDeepRG
            self.ubegin  = self.conf.yMin
            self.uend    = self.conf.yMax
            self.sidePos = self.conf.xMax
        elif self.side & IoPin.SOUTH:
            self.gauge   = self.conf.vDeepRG
            self.ubegin  = self.conf.xMin
            self.uend    = self.conf.xMax
            self.sidePos = self.conf.yMin
        elif self.side & IoPin.NORTH:
            self.gauge   = self.conf.vDeepRG
            self.ubegin  = self.conf.xMin
            self.uend    = self.conf.xMax
            self.sidePos = self.conf.yMax

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
                    if not self.ubegin in self.pins:
                        break
                else: 
                    self.uend -= ustep
                    pinOffset = self.uend
                    if not self.uend in self.pins:
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
        if not upos in self.pins:
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

        trace( 550, '\tSide.place() {}\n'.format(ioPin) )
        status = 0
        if self.side & (IoPin.NORTH | IoPin.SOUTH):
            gauge = self.conf.vDeepRG
            upos  = ioPin.upos
            for index in ioPin.indexes:
                pinName  = ioPin.stem.format( index )
                net      = self.conf.cell.getNet( pinName )
                if net is None:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): No net named "{}".'.format(pinName) ] ))
                    continue
                if net.isClock() and self.conf.useClockTree:
                    print( WarningMessage( 'Side.place(IoPin): Skipping clock IoPin "{}".'.format(pinName) ))
                    continue
                pinName += '.{}'.format(self.conf.getIoPinsCounts(net))
                pinPos   = self.getNextPinPosition( ioPin.flags, upos, ioPin.ustep )
                if pinPos.getX() > self.conf.xMax or pinPos.getX() < self.conf.xMin:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): Pin "{}" is outside north or south abutment box side.' \
                                              .format(pinName)
                                            , '(x:{}, xAB: [{}:{}])' \
                                              .format( DbU.getValueString(pinPos.getX())
                                                     , DbU.getValueString(self.conf.xMin)
                                                     , DbU.getValueString(self.conf.xMax) ) ] ))
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
                                , gauge.getWireWidth() # // 2
                                )
                NetExternalComponents.setExternal( pin )
                self.append( pin )
                self.conf.incIoPinsCounts( net )
                if upos: upos += ioPin.ustep
        else:
            gauge = self.conf.hDeepRG
            upos  = ioPin.upos
            for index in ioPin.indexes:
                pinName  = ioPin.stem.format(index)
                net      = self.conf.cell.getNet( pinName )
                if net is None:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): No net named "{}".'.format(pinName) ] ))
                    continue
                pinName += '.{}'.format(self.conf.getIoPinsCounts(net))
                pinPos   = self.getNextPinPosition( ioPin.flags, upos, ioPin.ustep )
                if pinPos.getY() > self.conf.yMax or pinPos.getY() < self.conf.yMin:
                    print( ErrorMessage( 1, [ 'Side.place(IoPin): Pin "{}" is outside east or west abutment box side.' \
                                              .format(pinName)
                                              , '(y:{}, yAB: [{}:{}])' \
                                              .format( DbU.getValueString(pinPos.getY())
                                                     , DbU.getValueString(self.conf.yMin)
                                                     , DbU.getValueString(self.conf.yMax)) ] ))
                    status += 1
                trace( 550, '\tIoPin.place() E/W @{} "{}" of "{}".\n'.format(pinPos,pinName,net) )
                pin = Pin.create( net
                                , pinName
                                , self.pinDirection
                                , Pin.PlacementStatus.FIXED
                                , gauge.getLayer()
                                , pinPos.getX()
                                , pinPos.getY()
                                , gauge.getWireWidth() // 2
                                , gauge.getWireWidth()
                                )
                NetExternalComponents.setExternal( pin )
                self.append( pin )
                self.conf.incIoPinsCounts( net )
                if upos: upos += ioPin.ustep
        return status

    def expand ( self ):
        """
        After, and only after routing the block, move the I/O pins *outside*
        of the abutment box. THey will stick out for one pitch.
        """
        if not self.expandPins: return
        rg  = self.conf.routingGauge
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
                                          .format(sideName,self.conf.cell.getName(),count)
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
    FLATTENED = 0x0001

    @staticmethod
    def lookup ( cell ):
        if cell in Block.LUT: return Block.LUT[cell]
        return None

    def __init__ ( self, conf ):
        """
        Create a Block object. The only parameter ``conf`` must be a BlockConf
        object which contains the complete block configuration.
        """
        conf._postInit()
        self.flags            = 0
        self.conf             = conf
        self.spares           = Spares( self )
        self.hTrees           = []
        self.hfnTrees         = []
        self.blockInstances   = []
        self.placeHolderCount = 0
        self.sides            = { IoPin.WEST  : Side( self.conf, IoPin.WEST  )
                                , IoPin.EAST  : Side( self.conf, IoPin.EAST  )
                                , IoPin.SOUTH : Side( self.conf, IoPin.SOUTH )
                                , IoPin.NORTH : Side( self.conf, IoPin.NORTH )
                                }
        self.etesian          = None
        self.excludedNets     = []
        for item in self.conf.hTreeDatas:
            self.excludedNets.append( item[0] )
        if not self.conf.cell.getAbutmentBox().isEmpty():
            isBuilt = True
            for instance in self.conf.cell.getInstances():
                status = instance.getPlacementStatus()
                if status == Instance.PlacementStatus.UNPLACED:
                    isBuilt = False
                    break
            if isBuilt:
                print( '  o  Block "{}" is already done, reusing layout.' \
                       .format(self.conf.cell.getName()) )
                self.conf.cell.setTerminalNetlist( True )
            else:
                print( '  o  Block "{}" is partially placed, reusing layout.' \
                       .format(self.conf.cell.getName()) )
            self.conf.isBuilt = isBuilt
        else:
            print( '  o  Block "{}" will be generated.' \
                   .format(self.conf.cell.getName()) )
        Block.LUT[ self.conf.cell ] = self

        
    @staticmethod
    def abPlace ( instance, transf ):
        """
        Place an instance so it is it's abutment box which will be
        placed at ``transf``. Incomplete implementation for now, only
        Orientation::ID in ``transf`` is supported.
        """
        ab       = instance.getMasterCell().getAbutmentBox()
        abTransf = Transformation( ab.getXMin(), ab.getYMin(), Transformation.Orientation.ID )
        abTransf.invert()
        abTransf.applyOn( transf )
        instance.setTransformation( transf )
        instance.setPlacementStatus( Instance.PlacementStatus.FIXED )


    @staticmethod
    def _getInstance ( cell, pattern, level=0 ):
        """
        getInstanceMatching(): returns first instance with the word being searched.
        """
        for instance in cell.getInstances():
            name = instance.getName()
            if pattern in name:
                trace( 550, '\t{} {} match pattern "{}"\n'.format('  '*level,instance,pattern) )
                return instance
        return None

    @staticmethod
    def _rgetInstance ( cell, path, level=0 ):
        """
        Get the instance designated by path (recursively). The path argument can be
        either a string of instance names separated by dots or directly a list of
        instances names.
        """
        if isinstance(path,str):
            path = path.split( '.' )
        elif not isinstance(path,list):
            raise ErrorMessage( 1, 'Block._rgetInstance(): "path" argument is neither a string or a list ({})"' \
                                   .format(path) )
        instance = Block._getInstance( cell, path[0], level )
        if instance is None:
            raise ErrorMessage( 1, 'Block._rgetInstance(): no instance matching "{}" in cell "{}"' \
                                   .format(path[0],cell.getName()) )
        if len(path) == 1:
            return instance
        return Block._rgetInstance( instance.getMasterCell(), path[1:], level+1 )

    def rgetCoreInstance ( self, path ):
        """
        Get the instance designated by path (recursively). The path argument can be
        either a string of instance names separated by dots or directly a list of
        instances names. The root of the path must be *core* cell.
        """
        return Block._rgetInstance( self.conf.core, path )

    @staticmethod
    def _rinstancesToPath ( path, instances ):
        instance = path.getTailInstance().getMasterCell().getInstance( instances[0] )
        path     = Path( path, instance )
        if len(instances) > 1:
            return Block._rinstanceToPath( path, instances[1:] )
        return path

    @staticmethod
    def _instancesToPath ( cell, instances ):
        path = Path( cell.getInstance( instances[0] ))
        if len(instances) == 1:
            return path
        return Block._rinstancesToPath( path, instances[1:] )

    def getFlattenedNet ( self, path ):
        """
        Find a net in the hierarchy. The path argument is a  list pathname of instance
        endind by a net name, like "instance1.instance2.net_name". The function returns
        a an Occurrence, the instance path and the Net, or None.
        """
        for net in self.conf.cellPnR.getNets():
            if net.getName() == path:
                return Occurrence( net, Path() )
        elements = path.split('.')
        if len(elements) == 1:
            return None
        ipath = Block._instancesToPath( self.conf.cellPnR, elements[:-1] )
        net   = ipath.getTailInstance().getMasterCell().getNet( elements[-1] )
        if net is None:
            raise ErrorMessage( 1, [ 'Block.getFlattenedNet(): On "{}" (deep)net,'.format( path )
                                   , 'Tail {} has no net "{}".' \
                                     .format( ipath.getTailInstance(), elements[-1] )
                                   ] )
        return Occurrence( net, ipath )

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
        trace( 550, '\tBlockConf.setupAb() {}\n'.format(self.conf.cell) )
        if not self.conf.cell.getAbutmentBox().isEmpty():
            pass
        elif not self.conf.coreAb.isEmpty():
            self.conf.core.setAbutmentBox( self.conf.coreAb )
        elif len(self.blockInstances):
            with UpdateSession():
                ab = Box( 0, 0, self.conf.fixedWidth, self.conf.fixedHeight )
                self.conf.cell.setAbutmentBox( ab )
                for occurrence in self.conf.cell.getNonTerminalNetlistInstanceOccurrences():
                    instance = occurrence.getEntity()
                    subCell  = instance.getMasterCell()
                    subCell.setAbutmentBox( ab )
                for occurrence in self.conf.cell.getNonTerminalNetlistInstanceOccurrences():
                    instance = occurrence.getEntity()
                    instance.setTransformation( Transformation() )
                for blockInstance in self.blockInstances:
                    blockInstance.place()
        else:
            sysSpaceMargin   = self.conf.cfg.etesian.spaceMargin
            blockSpaceMargin = sysSpaceMargin + self.spares.getSpareSpaceMargin()
            self.conf.cfg.etesian.spaceMargin = blockSpaceMargin
            self.conf.cfg.apply()
            with UpdateSession():
                etesian = Etesian.EtesianEngine.create( self.conf.cell )
                if self.conf.fixedWidth:  etesian.setFixedAbWidth ( self.conf.fixedWidth  )
                if self.conf.fixedHeight: etesian.setFixedAbHeight( self.conf.fixedHeight )
                etesian.setDefaultAb()
                etesian.destroy()
            self.conf.cfg.etesian.spaceMargin = sysSpaceMargin
            self.conf.cfg.apply()
        ab = self.conf.cell.getAbutmentBox()
        self.conf.coreSize = (ab.getWidth(), ab.getHeight())
        trace( 550, '\tSetting core ab from Cell:{}\n'.format(self.conf.coreAb) )
        for side in self.sides.values(): side.setupAb()
        trace( 550, '\tCORE AB is {}\n'.format(self.conf.cell.getAbutmentBox()) )
        if self.conf.isCoreBlock:
            self.conf.setupICore()
        self.conf.setRoutingBb( self.conf.cellPnR.getAbutmentBox() )

    def flattenNets ( self ):
        if self.flags & Block.FLATTENED: return
        if self.conf.isCoreBlock:
            self.conf.corona.flattenNets( None, self.excludedNets, Cell.Flags_NoClockFlatten )
        else:
            self.conf.cell.flattenNets( None, self.excludedNets, Cell.Flags_NoClockFlatten )
        self.flags |= Block.FLATTENED

    def addHTrees ( self ):
        """Create the trunk of all the clock trees (recursive H-Tree)."""
        print( '  o  Building H-Tree(s).' )
        af = AllianceFramework.get()
        hTreeNets = []
        netOcc    = None
        self.flattenNets()
        for netName, flags in self.conf.hTreeDatas:
            netOcc = self.getFlattenedNet( netName )
           #if self.conf.isCoreBlock:
           #    coreNet = self.conf.cell.getNet( netName )
           #    if coreNet is not None:
           #        trace( 550, '\tFound coreNet={}\n'.format(coreNet) )
           #        for plug in self.conf.icore.getPlugs():
           #            if plug.getMasterNet() == coreNet:
           #                net = plug.getNet()
           #                break
           #else:
           #    net = self.conf.cellPnR.getNet( netName )
            if netOcc is None:
                print( ErrorMessage( 3, 'Block.addHTrees(): Cell "{}" has no H-Tree net "{}".' \
                                        .format( self.conf.cellPnR.getName(), netName )))
                continue
            trace( 550, '\tBlock.addHTrees(): Found H-Tree {}.\n'.format(netOcc) )
            hTreeNets.append( [ netOcc, flags ] )
            self.etesian.exclude( netName )
        with UpdateSession():
            for netOcc, flags in hTreeNets:
                print( '     - "{}".'.format(netOcc.getName()) )
                trace( 550, ',+', '\tBlock.addHTrees(): Build clock tree for {}.\n'.format(netOcc) )
                self.hTrees.append( HTree(self.spares,netOcc,len(self.hTrees),flags) )
                self.hTrees[-1].buildHTree()
                for net in self.hTrees[-1].subNets:
                    self.etesian.exclude( net.getName() )
                    self.excludedNets.append( net.getName() )
                trace( 550, '-' )
        Breakpoint.stop( 100, 'Block.addHTrees() on {} done.'.format(self.conf.cellPnR) )

    def splitHTrees ( self ):
        """
        Break the H-Tree root nets and attach all it's Pins to the closest leaf
        if the H-Tree.
        """
        for hTree in self.hTrees:
            hTree.splitNet()

    def findHfnTrees4 ( self ):
        """Perform simple HFNS, just break nets regardless of placement."""
        print( '  o  Building high fanout nets trees.' )
        if self.spares:
            self.flattenNets()
            beginCount = self.conf.bufferConf.count
            maxSinks   = 10
            dots( 82
                , '     -  Max sinks for buffer "{}"'.format(self.conf.bufferConf.name)
                , ' {}'.format(maxSinks) )
            nets  = []
            block = self.conf.corona if self.conf.isCoreBlock else self.conf.cell
            for net in block.getNets():
                sinksCount = 0
                for rp in net.getRoutingPads(): sinksCount += 1
                if sinksCount > maxSinks:
                    nets.append( (net,sinksCount) )
            with UpdateSession():
                for net,sinksCount in nets:
                    trace( 550, '\tBlock.addHfnTrees4(): Found high fanout net "{}" ({} sinks).\n' \
                                .format(net.getName(),sinksCount) )
                   #if not net.getName().startswith('alu_m_muls_b(1)'): continue
                   #if not net.getName().startswith('abc_75177_new_n12236'): continue
                    sys.stderr.flush()
                    print( '     - "{}", {} sinks.'.format(net.getName(),sinksCount) )
                    sys.stdout.flush()
                    self.hfnTrees.append( BufferTree( self.spares, net ) )
                    self.hfnTrees[-1].buildBTree()
            Breakpoint.stop( 100, 'block.findHfnTrees4() done.' )
        else:
            print( '     (No spares buffers, disabled)' )
        endCount = self.conf.bufferConf.count
        dots( 82, '     -  Added buffers', ' {}'.format(endCount-beginCount) )
        return len(self.hfnTrees)

    def findHfnTrees ( self ):
        """Create the trunk of all the high fanout nets."""
        print( '  o  Building high fanout nets trees.' )
        if self.spares:
            maxSinks = timing.tech.getSinksEstimate( self.conf.bufferConf.name )
            dots( 82
                , '     -  Max sinks for buffer "{}"'.format(self.conf.bufferConf.name)
                , ' {}'.format(maxSinks) )
            nets = []
            for net in self.conf.cell.getNets():
                sinksCount = 0
                for rp in net.getRoutingPads(): sinksCount += 1
                if sinksCount > maxSinks:
                    nets.append( (net,sinksCount) )
            with UpdateSession():
                for net,sinksCount in nets:
                    trace( 550, '\tBlock.addHfnTrees(): Found high fanout net "{}" ({} sinks).\n' \
                                .format(net.getName(),sinksCount) )
                   #if not net.getName().startswith('alu_m_muls_b(1)'): continue
                   #if not net.getName().startswith('abc_75177_new_n12236'): continue
                    sys.stderr.flush()
                    print( '     - "{}", {} sinks.'.format(net.getName(),sinksCount) )
                    sys.stdout.flush()
                    self.hfnTrees.append( BufferTree( self.spares, net ) )
                    self.hfnTrees[-1].buildBTree()
                self.spares.rshowPoolUse()
           #Breakpoint.stop( 0, 'block.findHfnTrees() done.' )
        else:
            print( '     (No spares buffers, disabled)' )
        return len(self.hfnTrees)

    def addHfnBuffers ( self ):
        for hfnTree in self.hfnTrees:
            hfnTree.rcreateBuffer()

    def placeIoPins ( self ):
        """
        Place the Pins on all the sides. Raise an exception in case of failure.
        (mainly due to Pins outside the side range)
        """
        trace( 550, ',+', '\tBlock.placeIoPins().\n' )
        faileds = 0
        with UpdateSession():
            for ioPin in self.conf.ioPins:
                if   ioPin.flags & IoPin.SOUTH: side = self.sides[IoPin.SOUTH]
                elif ioPin.flags & IoPin.NORTH: side = self.sides[IoPin.NORTH]
                elif ioPin.flags & IoPin.EAST:  side = self.sides[IoPin.EAST ]
                else:                           side = self.sides[IoPin.WEST ]
                faileds += side.place( ioPin )
        trace( 550, ',-' )
        if faileds:
            raise ErrorMessage( 3, 'Block.placeIoPins(): Cell "{}" has {} badly placed pins.' \
                                   .format(self.conf.cell.getName(),faileds) )

    def checkIoPins ( self ):
        """
        Check for Pin overlap and that all external Net have at least one Pin.
        """
        for side in self.sides.values():
            side.checkOverlaps()
        for net in self.conf.cell.getNets():
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
        """
        Stick out the block I/O pins of one pitch on all sides.
        See setUnexpandPins() to prevent it to happen on a specific side.
        """
        with UpdateSession():
            for side in self.sides.values():
                side.expand()

    def initEtesian ( self ):
        editor = self.conf.editor
        if self.etesian:
            return
        if self.conf.isCoreBlock:
            self.etesian = Etesian.EtesianEngine.create( self.conf.corona )
            self.etesian.setBlock( self.conf.icore )
            if editor:
                editor.setCell( self.conf.cellPnR )
                Breakpoint.stop( 100, 'Block.place(), corona loaded.')
        else:
            self.etesian = Etesian.EtesianEngine.create( self.conf.cell )
        self.flattenNets()

    def place ( self ):
        self.initEtesian()
        if self.conf.placeArea:
            self.etesian.setPlaceArea( self.conf.placeArea )
        if self.conf.useHFNS:
            self.etesian.doHFNS()
            Breakpoint.stop( 100, 'HFNS (Etesian) done.' )
        self.etesian.place()
        Breakpoint.stop( 100, 'Placement done.' )
        self.etesian.clearColoquinte()

    def placeMacro ( self, ipath, transf ):
        """
        Place the instance refered by ``ipath`` at position ``transformation``.
        Both  parameters are relative to the core cell.
        """
        trace( 550, ',+', '\tBlock.placeMacro() {} @{}\n'.format( ipath, transf ))
        with UpdateSession():
            instance   = self.rgetCoreInstance( ipath )
            macro      = Macro.wrap( instance.getMasterCell()
                                   , self.conf.routingGauge.getName(), 3, 2 )
            instanceAb = instance.getMasterCell().getAbutmentBox()
            coreTransf = self.conf.icore.getTransformation()
            if self.conf.isCoreBlock:
                pnrAb = self.conf.icorona.getMasterCell().getAbutmentBox()
            else:
                pnrAb = self.conf.core.getAbutmentBox()
            trace( 550, '\tpnrAb={}, coreTransf={}\n'.format(pnrAb,coreTransf) )
            macroPosition = transf.getTranslation()
            coreTransf.applyOn( macroPosition )
            xoffset = macroPosition.getX() - pnrAb.getXMin()
            xpitch  = self.conf.vDeepRG.getPitch()
            trace( 550, '\tX offset: {} (X pitch:{})\n' \
                        .format( DbU.getValueString(xoffset)
                               , DbU.getValueString(xpitch) ))
            if xoffset % xpitch:
                xoffset += xpitch - (xoffset % xpitch)
            trace( 550, '\tPitched X offset: {}\n'.format( DbU.getValueString(xoffset) ))
            yoffset = macroPosition.getY() - pnrAb.getYMin()
            ypitch  = self.conf.hDeepRG.getPitch()
            trace( 550, '\tY offset: {} (Y pitch:{})\n' \
                        .format( DbU.getValueString(yoffset)
                               , DbU.getValueString(ypitch) ))
            if yoffset % ypitch:
                yoffset += ypitch - (yoffset % ypitch)
            trace( 550, '\tPitched Y offset: {}\n'.format( DbU.getValueString(yoffset) ))
            macroPosition = Point( xoffset, yoffset )
            coreTransf.invert()
            coreTransf.applyOn( macroPosition )
            trace( 550, '\tPosition in core:{}, instanceAb:{}\n' \
                        .format( macroPosition, instanceAb ))
            Macro.place( instance.getCell()
                       , instance
                       , Transformation( macroPosition.getX()
                                       , macroPosition.getY()
                                       , transf.getOrientation() )
                       , Instance.PlacementStatus.FIXED )
        trace( 550, '-' )

    def addPlaceHolder ( self, area, inCore=False ):
        if area.isEmpty():
            print( ErrorMessage( 1, 'Block.addPlaceHolder(): Request for an empty place holder area.' ))
            return
        af = AllianceFramework.get()
        phCellName = 'placeholder_{}'.format(self.placeHolderCount)
        placeHolder = Cell.create( af.getLibrary(0), phCellName )
        if inCore:
            self.conf.icore.getTransformation().applyOn( area )
        pitchedArea = Box( self.conf.toXPitch(area.getXMin(),True)
                         , self.conf.toYSlice(area.getYMin(),True)
                         , self.conf.toXPitch(area.getXMax())
                         , self.conf.toYSlice(area.getYMax()) )
        placeHolder.setAbutmentBox( Box( u(0.0)
                                       , u(0.0)
                                       , pitchedArea.getWidth()
                                       , pitchedArea.getHeight() ))
        print( 'pitchedArea={}'.format(pitchedArea) )
        placeHolder.setTerminalNetlist( True )
        instance = Instance.create( self.conf.cellPnR, phCellName, placeHolder )
        instance.setTransformation( Transformation( pitchedArea.getXMin()
                                                  , pitchedArea.getYMin()
                                                  , Transformation.Orientation.ID ) )
        instance.setPlacementStatus( Instance.PlacementStatus.FIXED )
        self.placeHolderCount += 1
        return instance

    def route ( self ):
        routedCell = self.conf.corona if self.conf.isCoreBlock else self.conf.cell
        self.katana = Katana.KatanaEngine.create( routedCell )
       #self.katana.printConfiguration()
        self.katana.digitalInit       ()
       #katana.runNegociatePreRouted()
        Breakpoint.stop( 100, 'Block.route() Before global routing.' )
        self.katana.runGlobalRouter  ( Katana.Flags.NoFlags )
        Breakpoint.stop( 100, 'Block.route() After global routing.' )
        self.katana.loadGlobalRouting( Anabatic.EngineLoadGrByNet )
        Breakpoint.stop( 100, 'Block.route() Loaded global into detailed.' )
        self.katana.layerAssign      ( Anabatic.EngineNoNetLayerAssign )
        self.katana.runNegociate     ( Katana.Flags.NoFlags )
        success = self.katana.isDetailedRoutingSuccess()
        Breakpoint.stop( 99, 'Block.route() done, success:{}.'.format(success) )
        self.katana.finalizeLayout()
        if self.conf.cfg.katana.dumpMeasures is True:
            self.katana.dumpMeasures()
        self.katana.destroy()
        self.katana = None
        if self.etesian:
            self.etesian.destroy()
            self.etesian = None
        return success

    def addBlockages ( self ):
        with UpdateSession():
            net = self.conf.cell.getNet( 'blockagenet' )
            ab  = self.conf.cell.getAbutmentBox()
            rg  = self.conf.routingGauge
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

    def doPnR ( self ):
        """
        Perform all the steps required to build the layout of the block.
        The first step is to build all the blockInstance it depends upon,
        so they will appear as ``NetListTerminal`` and we can place them
        in their parent cell.
        """
        editor = self.conf.editor
        print( '  o  Builing block "{}".'.format(self.conf.cell.getName()) )
        for blockInstance in self.blockInstances:
            blockInstance.block.conf.editor = editor
            if not blockInstance.block.conf.isBuilt:
                print( '     - Build sub-block "{}".' \
                       .format(blockInstance.block.conf.cell.getName()) )
                blockInstance.block.build()
        if editor: editor.setCell( self.conf.cellPnR )
        self.conf.cfg.apply()
        iteration = -1
        while True:
            iteration += 1
            if iteration > 0: break
            self.setupAb()
            if editor: editor.fit()
            if not self.conf.isCoreBlock:
                self.placeIoPins()
                self.checkIoPins()
            self.spares.build()
           #if self.conf.useHFNS: self.findHfnTrees4()
            self.initEtesian()
            self.addHTrees()
            sys.stdout.flush()
            sys.stderr.flush()
           #if self.conf.useHFNS: self.addHfnBuffers()
           #Breakpoint.stop( 0, 'Clock tree(s) done.' )
            self.place()
           #if self.conf.useHFNS: self.findHfnTrees()
            break
        self.splitHTrees()
        self.spares.removeUnusedBuffers()
        for trackAvoid in self.conf.trackAvoids:
            self.etesian.addTrackAvoid( trackAvoid )
        self.etesian.toHurricane()
        self.etesian.flattenPower()
        if self.conf.isCoreBlock: self.doConnectCore()
        status = self.route()
        if not self.conf.isCoreBlock:
            self.addBlockages()
            self.expandIoPins()
        self.conf.isBuilt = True
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
            if path.isEmpty(): parentCell = self.conf.cell
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
  
    def save ( self, flags=0 ):
        if not self.conf.validated:
            raise ErrorMessage( 1, 'block.save(): Chip is not valid, aborting.' )
        self.conf.save( flags )


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


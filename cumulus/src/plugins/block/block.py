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
from   ...                import Cfg, CRL
from   ...Hurricane       import Breakpoint, DbU, Box, Transformation, Point, \
                                 Box, Path, Layer, Occurrence, Net,           \
                                 NetExternalComponents, RoutingPad, Pad,      \
                                 Horizontal, Vertical, Contact, Pin, Plug,    \
                                 Cell, Instance, UpdateSession
from   ...CRL             import AllianceFramework, RoutingLayerGauge
from   ...helpers         import trace, dots, l, u, n
from   ...helpers.io      import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay import UpdateSession
from   ...                import Etesian, Anabatic, Katana, Tramontana
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
                                , gauge.getWireWidth() * 2 # // 2
                                )
                NetExternalComponents.setExternal( pin )
                self.append( pin )
                self.conf.incIoPinsCounts( net )
                if upos: upos += ioPin.ustep
        else:
            pinDepth = self.conf.horizontalDeepDepth
            if self.conf.cfg.block.upperEastWestPins:
                pinDepth += 2
            gauge  = self.conf.routingGauge.getLayerGauge( pinDepth )
            ppitch = self.conf.routingGauge.getLayerPitch( pinDepth )
            upos   = ioPin.upos
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
                                , ppitch * 2
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
                        if   self.side & IoPin.WEST:
                            pin.setX    ( pin.getDx()-offset )
                            pin.setWidth( pin.getWidth() + offset*2 )
                        elif self.side & IoPin.EAST:
                            pin.setX    ( pin.getDx()+offset )
                            pin.setWidth( pin.getWidth() + offset*2 )
                        elif self.side & IoPin.SOUTH:
                            pin.setY     ( pin.getDy()-offset )
                            pin.setHeight( pin.getHeight() + offset*2 )
                        elif self.side & IoPin.NORTH:
                            pin.setY     ( pin.getDy()+offset )
                            pin.setHeight( pin.getHeight() + offset*2 )

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
            minHCorona = self.conf.minHCorona
            minVCorona = self.conf.minVCorona
            innerBb    = Box( self.conf.coreAb )
            innerBb.inflate( minHCorona, minVCorona )
            coronaAb = self.conf.corona.getAbutmentBox()
            if innerBb.getWidth() > coronaAb.getWidth():
                raise ErrorMessage( 1, 'Core is too wide to fit into the corona, needs {} but only has {}.' \
                                       .format( DbU.getValueString(innerBb .getWidth())
                                              , DbU.getValueString(coronaAb.getWidth()) ) )
            if innerBb.getHeight() > coronaAb.getHeight():
                raise ErrorMessage( 1, 'Core is too tall to fit into the corona, needs {} but only has {}.' \
                                       .format( DbU.getValueString(innerBb .getHeight())
                                              , DbU.getValueString(coronaAb.getHeight()) ) )
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
    def pin_defined_in_iospec(self, pin_name):
        
      """Check if a pin with pin_name is already defined
       """
      for iopin in self.conf.ioPins:
          if iopin.stem == pin_name:
              return True
      return False 
    
    def createPinPostPlacement(self,workCell,net,direction,pinSide,x,y):
          """create a new pin post placement, user must specify the top level cell,
          pin Name, the access direction (in or out), the pinside 
          (NORTH, SOUTH, WEST ,EAST and the positions x,y in DbU
           """
          net.setExternal(True)
          if direction == 'IN':
            net.setDirection(Net.Direction.IN)
          elif direction == 'OUT':
            net.setDirection(Net.Direction.OUT)

          if   pinSide == 'NORTH':
             side  =  self.sides[IoPin.NORTH]
             gauge =  side.conf.vDeepRG
             width =  gauge.getWireWidth()
             height=  gauge.getWireWidth() * 2

          elif pinSide == 'SOUTH':   
             side  =  self.sides[IoPin.SOUTH]
             gauge =  side.conf.vDeepRG
             width =  gauge.getWireWidth()         
             height=  gauge.getWireWidth() * 2 

          elif pinSide == 'EAST':   
             side     = self.sides[IoPin.EAST]
             pinDepth = self.conf.horizontalDeepDepth
             if side.conf.cfg.block.upperEastWestPins:
                 pinDepth += 2
             gauge  =  side.conf.routingGauge.getLayerGauge(pinDepth)
             ppitch =  side.conf.routingGauge.getLayerPitch(pinDepth)
             width  =  ppitch * 2         
             height =  gauge.getWireWidth()
          elif pinSide == 'WEST':   
             side     = self.sides[IoPin.WEST]
             pinDepth = self.conf.horizontalDeepDepth
             if side.conf.cfg.block.upperEastWestPins:
                 pinDepth += 2
             gauge  = side.conf.routingGauge.getLayerGauge(pinDepth)
             ppitch = side.conf.routingGauge.getLayerPitch(pinDepth)
             width  =  ppitch * 2         
             height =  gauge.getWireWidth()
          pin = Pin.create(  net
                          , net.getName()
                          , side.pinDirection
                          , Pin.PlacementStatus.FIXED
                          , gauge.getLayer()
                          , x
                          , y
                          , width
                          , height
                          )
          NetExternalComponents.setExternal(pin)
          side.append( pin )
          RoutingPad.create(
              net,
              Occurrence(pin),
              RoutingPad.BiggestArea
              )
          print(f'[WARNING] Pin {net.getName()} is created')
          return pin

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
                                   , self.conf.routingGauge.getName(), 3, 3 )
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
                                   , ab.getYMax() - dyBorder)


    def getExternalSignalsSorted(self):
       """
       Collect external non-supply nets and group bus bits together.
       Returns:
           dict:{ index: [signal_name, width, first_bit]}
       """

       signals = {}

       for net in self.conf.cell.getNets():
           if not net.isExternal():
               continue
           if net.isSupply():
               continue

           netName = net.getName()

           if '(' in netName and ')' in netName:
               signalName = netName.split('(')[0]
               bitIndex   = int(netName.split('(')[1].split(')')[0])
           else:
               signalName = netName
               bitIndex   = 0

           if signalName not in signals:
               signals[signalName] = [signalName, 1, bitIndex]
           else:
               signals[signalName][1] += 1
               signals[signalName][2] = min(signals[signalName][2], bitIndex)

       signalsSorted = {}

       for index, signalName in enumerate(signals):
           signalsSorted[index] = signals[signalName]

       return signalsSorted
      
    def getIoTrackWindows(self):
        """
        Get block dimensions and routing gauge information.
        Returns:
            dict with:
                - width, height (um)
                - pitches and offsets per direction
                - usable track capacities per side
                (number of tracks possible per side for pins placement)
        """
    
        ab = self.conf.cell.getAbutmentBox()
    
        xmin = ab.getXMin()
        xmax = ab.getXMax()
        ymin = ab.getYMin()
        ymax = ab.getYMax()
    
        width  = xmax - xmin
        height = ymax - ymin
    
        # ---- Get routing gauges ----
    
        # Horizontal pins (WEST/EAST) → use vertical routing
        hDepth = self.conf.horizontalDeepDepth
        if self.conf.cfg.block.upperEastWestPins:
            hDepth += 2
    
        hGauge = self.conf.routingGauge.getLayerGauge(hDepth)
    
        # Vertical pins (NORTH/SOUTH)
        vGauge = self.conf.vDeepRG
    
        hPitch  = hGauge.getPitch()
        hOffset = hGauge.getOffset()
    
        vPitch  = vGauge.getPitch()
        vOffset = vGauge.getOffset()

        # ---- Margin in tracks (we dont use all the side to place pins) ----
        margin_tracks = 2
        # ---- Convert usable space into track counts ----
    
        # WEST/EAST → vertical axis → height
        total_tracks_vertical = int((height - hOffset) // hPitch)
        tracks_vertical = total_tracks_vertical - 2 * margin_tracks
    
        # NORTH/SOUTH → horizontal axis → width
        total_tracks_horizontal = int((width - vOffset) // vPitch)
        tracks_horizontal = total_tracks_horizontal - 2 * margin_tracks
    
        return {
            'width': width,
            'height': height,
    
            'hPitch': hPitch,
            'hOffset': hOffset,
            'vPitch': vPitch,
            'vOffset': vOffset,
            'tracks_vertical': tracks_vertical,
            'tracks_horizontal': tracks_horizontal
        }
    def numberOfBits(self, signals):
        """
        Return the total number of bits to place.
        """

        total = 0

        for index in signals:
            total += signals[index][1]

        return total    
    def orderOfPlacement(self, signals):
        """
        Sort signals indexes from largest bus to smallest.
        """

        N = len(signals)

        sizes = []
        sorted_indexes = []

        for i in range(N):
            sizes.append(signals[i][1])
            sorted_indexes.append(i)

        for n in range(1, N):
            key_size = sizes[n]
            key_index = sorted_indexes[n]

            j = n - 1
            while j >= 0 and sizes[j] < key_size:
                sizes[j + 1] = sizes[j]
                sorted_indexes[j + 1] = sorted_indexes[j]
                j -= 1
            sizes[j + 1] = key_size
            sorted_indexes[j + 1] = key_index

        return sorted_indexes

    def connectorsNumberPerSide(self, total_bits, tracks_horizontal, tracks_vertical):
        """
        Compute how many pins (bits) should be placed on each side.
        Args:
            total_bits          : total number of bits to place
            tracks_horizontal   : available tracks on NORTH/SOUTH
            tracks_vertical     : available tracks on EAST/WEST
        Returns:
            [west, north, east, south] number of bits per side
        """
        # Total capacity
        total_capacity = 2 * tracks_vertical + 2 * tracks_horizontal

        # Ratio of horizontal vs vertical capacity
        vertical_ratio = (2 * tracks_horizontal) / total_capacity

        # Number of bits on NORTH + SOUTH
        bits_vertical_sides = int(total_bits * vertical_ratio)

        # Split evenly
        north_bits = bits_vertical_sides // 2
        south_bits = bits_vertical_sides // 2

        # Remaining bits → EAST/WEST
        remaining = total_bits - (north_bits + south_bits)

        east_bits = remaining // 2
        west_bits = remaining - east_bits

        return [west_bits, north_bits, east_bits, south_bits]



    def signalsPerSide(self, signals, tracks_horizontal, tracks_vertical):
        """
        Distribute signals bits across the 4 sides.
        Returns:
            [west, north, east, south]
    
        Each side contains:
            [name, start_bit, end_bit, 'bit' or 'vector']
        """
    
        total_bits = self.numberOfBits(signals)
        sorted_indexes = self.orderOfPlacement(signals)
    
        sides_capacity = self.connectorsNumberPerSide(
            total_bits,
            tracks_horizontal,
            tracks_vertical
        )
    
        sides = [[], [], [], []]  # WEST, NORTH, EAST, SOUTH
    
        signal_id = 0
        current_bit = None
        bits_left = 0
    
        def vector_or_bit(size):
            return 'bit' if size == 1 else 'vector'
    
        for side_id in range(4):
            side_capacity = sides_capacity[side_id]
            placed_on_side = 0
    
            while placed_on_side < side_capacity:
                if signal_id >= len(sorted_indexes):
                    break
    
                name, size, first_bit = signals[sorted_indexes[signal_id]]
    
                # Start a new signal.
                if bits_left == 0:
                    current_bit = first_bit
                    bits_left = size
    
                free_slots = side_capacity - placed_on_side
                bits_to_place = min(free_slots, bits_left)
    
                start_bit = current_bit
                end_bit = current_bit + bits_to_place - 1
    
                sides[side_id].append([
                    name,
                    start_bit,
                    end_bit,
                    vector_or_bit(size)
                ])
    
                placed_on_side += bits_to_place
                current_bit += bits_to_place
                bits_left -= bits_to_place
    
                # Signal fully placed, move to next one.
                if bits_left == 0:
                    signal_id += 1
                    current_bit = None
    
        # WEST bottom->top, NORTH left->right, EAST top->bottom, SOUTH right->left.
    
        return sides
    def generate_ioPinsSpec_list(self, sides_connectors, tracks_per_side, margin=2):
        """
        Generate ioPinsSpec with uniform distribution on each side.
        Each bit is generated explicitly (no implicit expansion).
        Pins are centered on each side.
        """
    
        ioPinsSpec = []
        sides = ['WEST', 'NORTH', 'EAST', 'SOUTH']
    
        for s in range(4):
            connectors = sides_connectors[s]
    
            # ---- count total bits ----
            total_pins = sum([c[2] - c[1] + 1 for c in connectors])
            if total_pins == 0:
                continue
    
            usable_tracks = tracks_per_side[s] - 2 * margin
    
            # ---- uniform spacing ----
            ustep = max(1, usable_tracks // total_pins)
    
            # ---- center the pins on the side ----
            occupied = (total_pins - 1) * ustep
            free = usable_tracks - occupied
            start_offset = margin + free // 2
    
            global_k = 0
    
            for name, start_bit, end_bit, kind in connectors:
    
                for bit in range(start_bit, end_bit + 1):
    
                    # ---- stem ----
                    if kind == 'bit':
                        stem = name
                    else:
                        stem = f"{name}({bit})"
    
                    # ---- position ----
                    if sides[s] in ['WEST', 'NORTH']:
                        # normal direction
                        uindex = start_offset + global_k * ustep
                    else:
                        # reversed direction
                        uindex = start_offset + (usable_tracks - (global_k + 1) * ustep)
                    flags = getattr(IoPin, sides[s]) | IoPin.A_BEGIN
    
                    ioPinsSpec.append((flags, stem, uindex, ustep, 1))
    
                    global_k += 1
    
        return ioPinsSpec
    def autoPlaceIoPins(self):
        """
        Automatically generate IO pins uniformly on all sides.
        """
    
        # ---- Get external signals (supply nets are ignored) ----
        signals = self.getExternalSignalsSorted()
    
        # ---- Get track windows and capacities ----
        track_data = self.getIoTrackWindows()
    
        tracks_vertical   = track_data['tracks_vertical']    # WEST/EAST
        tracks_horizontal = track_data['tracks_horizontal']  # NORTH/SOUTH
    
        tracks_per_side = [
            tracks_vertical,    # WEST
            tracks_horizontal,  # NORTH
            tracks_vertical,    # EAST
            tracks_horizontal   # SOUTH
        ]
    
        # ---- Distribute signals per side ----
        sides_connectors = self.signalsPerSide(
            signals,
            tracks_horizontal,
            tracks_vertical
        )
    
        # ---- Generate ioPinsSpec in track indices ----
        ioPinsSpecs = self.generate_ioPinsSpec_list(
            sides_connectors,
            tracks_per_side,
            margin=2
        )
        # ---- Convert specs to IoPin objects ----
        self.conf.ioPins = []
        self.conf.ioPinsInTracks = True
    
        for spec in ioPinsSpecs:
            spec = self.conf._toIoPinSpec(*spec)
            self.conf.ioPins.append(IoPin(*spec))
    
    def doPnR ( self ):
        """
        Perform all the steps required to build the layout of the block.
        The first step is to build all the blockInstance it depends upon,
        so they will appear as ``NetListTerminal`` and we can place them
        in their parent cell.
        """
        editor = self.conf.editor
        print( '  o  Building block "{}".'.format(self.conf.cell.getName()) )
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
                if not self.conf.ioPins:
                #Automatically place pins with uniform spacing, ensuring all sides are utilized.
                    self.conf.ioPinsInTracks = True
                    self.autoPlaceIoPins()
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
        pnrStatus = self.route()
        if not self.conf.isCoreBlock:
            self.addBlockages()
            self.expandIoPins()
        self.conf.isBuilt = True
        lvxStatus = True
        if self.conf.doLvx is not False:
            lvxStatus = self.doLvx()
        if pnrStatus != lvxStatus:
            raise ErrorMessage( 1, [ 'PnR and LVX status incoherency (PnR={}, LVX={})' \
                                     .format( pnrStatus, lvxStatus )
                                   , 'This strongly hints at a bug in the PnR...'
                                   ] )
        return pnrStatus and lvxStatus
    def legalize_pos_on_pitch(self, workCell, side, px, py):
        """
        Pins must align to defined pitches;
        This function ensures this feature
        """
    
        ab = workCell.getAbutmentBox()
    
        xmin = ab.getXMin()
        xmax = ab.getXMax()
        ymin = ab.getYMin()
        ymax = ab.getYMax()
    
        # -------- Get gauge according to side -------
        if side in ('WEST', 'EAST'):
            pinDepth = self.conf.horizontalDeepDepth
            if self.conf.cfg.block.upperEastWestPins:
                pinDepth += 2
            gauge = self.conf.routingGauge.getLayerGauge(pinDepth)
        else:
            gauge = self.conf.vDeepRG
    
        pitch  = gauge.getPitch()
        offset = gauge.getOffset()
    
        # --------  Constraints according to side --------
        if side in ('WEST', 'EAST'):
            py = ((py - offset) // pitch) * pitch + offset
            start_pos = py
            min_pos = ymin
            max_pos = ymax
        else:
            px = ((px - offset) // pitch) * pitch + offset
            start_pos = px
            min_pos = xmin
            max_pos = xmax
    
        # -------- get  pins already placed --------
        if side == 'WEST':
            side_obj = self.sides[IoPin.WEST]
        elif side == 'EAST':
            side_obj = self.sides[IoPin.EAST]
        elif side == 'SOUTH':
            side_obj = self.sides[IoPin.SOUTH]
        else:
            side_obj = self.sides[IoPin.NORTH]
    
        occupied = set(side_obj.pins)
    
        # -------- spacing between pins --------
        min_spacing = pitch   # 2*pitch for more safety
    
        # -------- fonction that detect collision between pins--------
        def is_free(pos):
            if pos < min_pos or pos > max_pos:
                return False
            for p in occupied:
                if abs(pos - p) < min_spacing:
                    return False
            return True
    
        # -------- search for ideal pos --------
        max_iter = 1000
    
        for k in range(max_iter):
    
            if k == 0:
                pos = start_pos
                if is_free(pos):
                    break
    
            else:
                pos1 = start_pos + k * pitch
                if is_free(pos1):
                    pos = pos1
                    break
    
                pos2 = start_pos - k * pitch
                if is_free(pos2):
                    pos = pos2
                    break
    
        else:
            raise RuntimeError("No legal position found for pin")
    
        # -------- new pos --------
        if side in ('WEST', 'EAST'):
            py = pos
        else:
            px = pos
    
        return px, py   
    def legalize_with_fallback(self, workCell, side, px, py):
        """
        Pins must align to defined pitches;
        the method legalize_pos_on_pitch enforces this on a specified side.
        If no legal position exists on that side,
        the function may reassign the pin to a different side.
        """


        ab = workCell.getAbutmentBox()

        xmin = ab.getXMin()
        xmax = ab.getXMax()
        ymin = ab.getYMin()
        ymax = ab.getYMax()

        # inital pos
        x = px
        y = py

        # closest side to position
        dist_west  = x - xmin
        dist_east  = xmax - x
        dist_south = y - ymin
        dist_north = ymax - y

        # sides list
        sides = ["WEST", "EAST", "SOUTH", "NORTH"]
        distances = [dist_west, dist_east, dist_south, dist_north]

        # sort the sides according to position
        for i in range(4):
            for j in range(i+1, 4):
                if distances[j] < distances[i]:
                    tmp_d = distances[i]
                    distances[i] = distances[j]
                    distances[j] = tmp_d

                    tmp_s = sides[i]
                    sides[i] = sides[j]
                    sides[j] = tmp_s

        # legalize
        for s in sides:
            try:
                px_new, py_new = self.legalize_pos_on_pitch(workCell, s, px, py)
                return s, px_new, py_new
            except RuntimeError:
                continue

        raise RuntimeError("No legal position found on any side")

    def project_to_edge(self, workCell, inst, pin_name=None):
     """
     This function does an orthogonal projection of the instance postion
     (or even the pin of the position inside the instance) on the block
     closest side. It is used to find the closest side to an instance. It is
     usefull to place a pin close to an instance.
     """
     # -------- abutment box --------
     ab = workCell.getAbutmentBox()

     xmin = ab.getXMin()
     xmax = ab.getXMax()
     ymin = ab.getYMin()
     ymax = ab.getYMax()

     t = inst.getTransformation()

     # -------- position (pin if given or instance) --------
     if pin_name is not None:
         master = inst.getMasterCell()
         net = master.getNet(pin_name)

         comp = None
         if net is not None:
             for c in net.getExternalComponents():
                 comp = c
                 break

         if comp is not None:
             box = comp.getBoundingBox()

             x = box.getXCenter()
             y = box.getYCenter()
             p = Point(x, y)
             t.applyOn(p)
             x = p.getX()
             y = p.getY()
         else:
             x = t.getTx()
             y = t.getTy()
     else:
         x = t.getTx()
         y = t.getTy()

     # -------- projection --------
     d_west  = x - xmin
     d_east  = xmax - x
     d_south = y - ymin
     d_north = ymax - y

     dmin = min(d_west, d_east, d_south, d_north)

     if dmin == d_west:
         side = 'WEST'
         px = xmin
         py = y

     elif dmin == d_east:
         side = 'EAST'
         px = xmax
         py = y

     elif dmin == d_south:
         side = 'SOUTH'
         px = x
         py = ymin

     else:
         side = 'NORTH'
         px = x
         py = ymax

     return side, px, py

    def create_unique_net(self,cell, base_name):
        """
        Create a unique net name.
        If an instance with the same name already exists,
        a suffix _1, _2, ... is appended.
        """
        name = base_name
        i = 1
        while cell.getNet(name) is not None:
            name = f"{base_name}_{i}"
            i += 1
        return Net.create(cell, name)


    def create_unique_instance(self, cell, base_name, master_cell):
        """
        Create an instance with a unique cell name.
        If an instance with the same name already exists,
        a suffix _1, _2, ... is appended.
        """
        name = base_name
        i = 1
        while cell.getInstance(name) is not None:
            name = f"{base_name}_{i}"
            i += 1
        return Instance.create(cell, name, master_cell)


    def find_best_scan_chain_and_io(self, workCell, cells, sout_buffer):
        """
        This function finds the best stitching configuration of the scan ffs:
        the ffs are already placed. The order of stitching is defined 
        minimizing the total length between a ff candidate (start ff) + the 
        distances between ffs + distance from last ff to sout_buffer
        (this buffer is linked to the output SOUT). There are 4 ffs candidates 
        for the start ff (that will be linked to SIN).
        """
        best_cost = None
        best_order = None
    
        # -------- 4 candidates for start ff (closest FF to the edge) --------
        ab = workCell.getAbutmentBox()
    
        xmin = ab.getXMin()
        xmax = ab.getXMax()
        ymin = ab.getYMin()
        ymax = ab.getYMax()
    
        candidates = [
            min(cells, key=lambda c: c.getTransformation().getTx() - xmin),   # WEST
            min(cells, key=lambda c: xmax - c.getTransformation().getTx()),   # EAST
            min(cells, key=lambda c: c.getTransformation().getTy() - ymin),   # SOUTH
            min(cells, key=lambda c: ymax - c.getTransformation().getTy()),   # NORTH
        ]
    
        # -------- buffer position = end --------
        t_buf = sout_buffer.getTransformation()
        end_x = t_buf.getTx()
        end_y = t_buf.getTy()
    
        xs = []
        ys = []
        for c in cells:
            t = c.getTransformation()
            xs.append(t.getTx())
            ys.append(t.getTy())
    
        # -------- optimization --------
        for inst in candidates:
    
            t = inst.getTransformation()
            start_x = t.getTx()
            start_y = t.getTy()
    
            # --- order  ---
            order = self.etesian.orderScanChain(
                xs,
                ys,
                start_x,
                start_y,
                end_x,
                end_y
            )
    
            # --- cost  ---
            cost = self.etesian.chainLengthFromOrder(
                order,
                xs,
                ys,
                start_x,
                start_y,
                end_x,
                end_y
            )
    
            # --- keep best ---
            if best_cost is None or cost < best_cost:
                best_cost = cost
                best_order = [cells[i] for i in order]
    
        return best_order

    def create_scan_ff(
     self,
     af,
     scan_ff_name,
     dff_name,
     mux_name,
     ff_pins,
     scan_pins,
     mux_pins,
     mux_orientation="ID",
     ff_orientation="ID"
):
     """
     This function creates a scan ff cell from a native mux cell and a native ff 
     cell. It is used for technologies that does not have a native scan ff.
     Names, pins and prefered orientations are specified for building the 
     scan ff cell. This information is given in the technology files
     """

     if mux_orientation not in ["ID", "MX"]:
         raise ErrorMessage(1, 'mux_orientation must be "ID" or "MX"')

     if ff_orientation not in ["ID", "MX"]:
         raise ErrorMessage(1, 'ff_orientation must be "ID" or "MX"')

     dff = af.getCell(dff_name, CRL.Catalog.State.Views)
     mux = af.getCell(mux_name, CRL.Catalog.State.Views)
     if dff is None or mux is None:
         raise Exception("Cannot load dff/mux stdcells")

     scan_ff = af.createCell(scan_ff_name)
     scan_ff.setTerminalNetlist(True)

     # =========================
     # Instances
     # =========================
     mux_0 = Instance.create(scan_ff, "mux_0", mux)
     dff_1 = Instance.create(scan_ff, "dff_1", dff)
 
     # =========================
     # Scan nets
     # =========================

     SI = Net.create(scan_ff, scan_pins["si"])
     SI.setExternal(True)
     SI.setDirection(Net.Direction.IN)

     SE = Net.create(scan_ff, scan_pins["se"])
     SE.setExternal(True)
     SE.setDirection(Net.Direction.IN)

     tmp_D = Net.create(scan_ff, "tmp_D")

     # =============================
     # Recreate ALL FF external nets
     # =============================
     ext_nets = {}

     for net in dff.getExternalNets():
         name = net.getName()
         # skip if this name is used by scan pins
         if name in [scan_pins["si"], scan_pins["se"]]:
          print(f"[WARNING] FF {dff_name} already has scan pin {name}, skipping")
          continue

         new_net = Net.create(scan_ff, name)
         new_net.setExternal(True)

         if net.isGlobal():
             new_net.setGlobal(True)
         else:
             new_net.setDirection(net.getDirection())

         ext_nets[name] = new_net

     # =========================
     # Mux connections
     # =========================
     mux_0.getPlug(mux.getNet(mux_pins["i0"])).setNet(
         ext_nets[ff_pins["d"]]
     )
     mux_0.getPlug(mux.getNet(mux_pins["i1"])).setNet(SI)
     mux_0.getPlug(mux.getNet(mux_pins["sel"])).setNet(SE)
     mux_0.getPlug(mux.getNet(mux_pins["out"])).setNet(tmp_D)

     # connect other mux pins automatically
     for net in mux.getExternalNets():
         name = net.getName()

         if name in [
             mux_pins["i0"],
             mux_pins["i1"],
             mux_pins["sel"],
             mux_pins["out"]
         ]:
             continue

         if name in ext_nets:
             mux_0.getPlug(mux.getNet(name)).setNet(ext_nets[name])

     # =========================
     # FF connections
     # =========================
     for net in dff.getExternalNets():
         name = net.getName()

         if name == ff_pins["d"]:
             dff_1.getPlug(dff.getNet(name)).setNet(tmp_D)
         elif name in ext_nets:
             dff_1.getPlug(dff.getNet(name)).setNet(ext_nets[name])

     # =========================
     # Placement
     # =========================
     mux_ab = mux.getAbutmentBox()
     mux_w  = mux_ab.getWidth()
     mux_h  = mux_ab.getHeight()

     dff_ab = dff.getAbutmentBox()
     dff_w  = dff_ab.getWidth()
     dff_h  = dff_ab.getHeight()

     if mux_h != dff_h:
         raise ErrorMessage(1, "Cells have not the same height")

     # orientation mapping
     ori_map = {
         "ID": Transformation.Orientation.ID,
         "MX": Transformation.Orientation.MX
     }


     # placement X offsets:
     # MX mirrors around origin => needs width compensation
     mux_x = mux_w if mux_orientation == "MX" else 0

     dff_x = mux_w
     if ff_orientation == "MX":
         dff_x += dff_w

     mux_0.setTransformation(
         Transformation(mux_x, 0, ori_map[mux_orientation])
     )
     mux_0.setPlacementStatus(Instance.PlacementStatus.PLACED)

     dff_1.setTransformation(
         Transformation(dff_x, 0, ori_map[ff_orientation])
     )
     dff_1.setPlacementStatus(Instance.PlacementStatus.PLACED)

     scan_ff.setAbutmentBox(Box(0, 0, mux_w + dff_w, dff_h))

     # =========================
     # Pin geometry creation
     # =========================
     gaugeName = Cfg.getParamString('anabatic.routingGauge').asString()
     routingGauge = af.getRoutingGauge(gaugeName)

     metal1 = None
     for layerGauge in routingGauge.getLayerGauges():
         if layerGauge.getType() == CRL.RoutingLayerGauge.PinOnly:
             metal1 = routingGauge.getRoutingLayer(layerGauge.getDepth())
             break

     if metal1 is None:
         raise ErrorMessage(1, "Cannot find PinOnly routing layer")

     for net in scan_ff.getExternalNets():
         if net.isGlobal():
             continue

         plug_found = None
         for plug in net.getPlugs():
             if plug.getInstance() is not None:
                 plug_found = plug
                 break

         if plug_found is None:
             continue

         master_net = plug_found.getMasterNet()
         inst = plug_found.getInstance()

         comp_master = None
         for comp in master_net.getExternalComponents():
             comp_master = comp
             break

         if comp_master is None:
             continue

         bb = Box(comp_master.getBoundingBox())
         inst.getTransformation().applyOn(bb)

         pin = Vertical.create(
             net,
             metal1,
             bb.getCenter().getX(),
             bb.getWidth(),
             bb.getYMin(),
             bb.getYMax()
         )

         NetExternalComponents.setExternal(pin)

     af.saveCell(scan_ff, CRL.Catalog.State.Views)

     return scan_ff
     

    def doPnRDFT ( self ):
        """
        Perform all the steps required to build the layout of the block + inseerting the DFT.
        The FFs are replaced by scan FFs before the placement but not stitched to not impact
        the placement optimization. Then the placement is operated and the SFFs are then stitched
        just before the routing.
        3 cases covered
        """
    
        # PRE-PLACEMENT DFT
        af = CRL.AllianceFramework.get()
        workCell = self.conf.cell
        dft = self.conf.dft_std_cells
    
        mux_name         = dft.mux_name
        buf_name         = dft.buf_name
        ff_pins          = dft.ff_pins
        if "d" not in ff_pins or "q" not in ff_pins:
            raise ErrorMessage(1, "ff_pins must define at least 'd' and 'q'")
        scan_pins        = dft.scan_pins
        ##Check that scan pin was well defined##
        if "q" not in scan_pins:
            scan_pins["q"] = ff_pins["q"]
        mux_pins         = dft.mux_pins
        buf_pins         = dft.buf_pins
        mux_orientation  = dft.mux_orientation
        ff_orientation   = dft.ff_orientation
    
        buf = af.getCell(buf_name, CRL.Catalog.State.Views)
        if buf is None:
            raise Exception("Cannot load buffer stdcell")
    
        with UpdateSession():
    
            # cache to avoid creating the same scan_ff master cell multiple times
            scanff_cache = {}
    
            # all scan FF instances after replacement
            sdffs = []
    
            # all scan FF master cells actually used
            used_scanff_masters = set()
    
            for inst in workCell.getInstances():
    
                mastercell_name = inst.getMasterCell().getName()
    
                # only process FFs declared in the DFT config
                if mastercell_name not in dft.dff_names:
                    continue
    
                # -------- save connections --------
                saved_nets = {}
                for plug in inst.getPlugs():
                    master_net = plug.getMasterNet()
                    if master_net is None:
                        continue
                    saved_nets[master_net.getName()] = plug.getNet()
    
                # -------- case where a scan FF already exists in the library --------
                if mastercell_name in dft.ff_to_scanff:
                    scan_name = dft.ff_to_scanff[mastercell_name]
                    scan_ff_candidate = af.getCell(scan_name, CRL.Catalog.State.Views)
                
                    if scan_ff_candidate is None:
                        raise ErrorMessage(1, f"Scan FF {scan_name} not found")
                
                    scan_pins_names = [net.getName() for net in scan_ff_candidate.getExternalNets()]
                
                    # ---- strict check: must match FF interface ----
                    if ff_pins["d"] not in scan_pins_names:
                        raise ErrorMessage(1, f"{scan_name} missing D pin ({ff_pins['d']})")
                
                    if ff_pins["q"] not in scan_pins_names:
                        raise ErrorMessage(1, f"{scan_name} missing Q pin ({ff_pins['q']})")
                
                    if scan_pins["si"] not in scan_pins_names:
                        raise ErrorMessage(1, f"{scan_name} missing SI pin ({scan_pins['si']})")
                
                    if scan_pins["se"] not in scan_pins_names:
                        raise ErrorMessage(1, f"{scan_name} missing SE pin ({scan_pins['se']})")
                
                    scan_ff_local = scan_ff_candidate
           



                # -------- case where there is no scan FF in library: create mux+FF --------
                else:
                    if mastercell_name not in scanff_cache:
                        #copy of the dict to avoid conflits if other files use the dico
                        local_scan_pins = dict(scan_pins)
                        if "d" not in local_scan_pins or local_scan_pins["d"] != ff_pins["d"]:
                            local_scan_pins["d"] = ff_pins["d"]
                        if "q" not in local_scan_pins or local_scan_pins["q"] != ff_pins["q"]:
                            local_scan_pins["q"] = ff_pins["q"]
                        scanff_cache[mastercell_name] = self.create_scan_ff(
                            af,
                            f"scanff_{mastercell_name}",
                            mastercell_name,
                            mux_name,
                            ff_pins,
                            local_scan_pins,
                            mux_pins,
                            mux_orientation,
                            ff_orientation
                        )
    
                    scan_ff_local = scanff_cache[mastercell_name]
    
                # save used scan FF master cells
                used_scanff_masters.add(scan_ff_local)
    
                # -------- replace --------
                inst.setMasterCell(scan_ff_local)
                sdffs.append(inst)
    
                # -------- reconnect --------
                for plug in inst.getPlugs():
                    master_net = plug.getMasterNet()
                    if master_net is None:
                        continue
    
                    name = master_net.getName()
    
                    # do not connect SE / SI here
                    if name in [scan_pins["si"], scan_pins["se"]]:
                        continue
                    # functional D pin must be reconnected explicitly
                    if name == ff_pins["d"]:
                        if ff_pins["d"] in saved_nets and saved_nets[ff_pins["d"]] is not None:
                            plug.setNet(saved_nets[ff_pins["d"]])
                        continue
                    # all other pins are reconnected by name
                    if name in saved_nets and saved_nets[name] is not None:
                        plug.setNet(saved_nets[name])
    
            if not sdffs:
                raise Exception("No SDFF instances found after replacement")
            # get DFT pins
            #Check that the net/Pin is already defined (via IoPinsSpec in doDesign file)
            sin_defined  = self.pin_defined_in_iospec("SIN")
            sout_defined = self.pin_defined_in_iospec("SOUT")
            se_defined   = self.pin_defined_in_iospec("SE")
            # SIN
            if sin_defined:
            #PIN defined/placed in doDesign
                sin_net = workCell.getNet("SIN")
                if sin_net is None:
                #PIN does not exist in the netlist
                #It has to be created with the same Name
                    sin_net = Net.create(workCell, "SIN")
                    sin_net.setExternal(True)
                    sin_net.setDirection(Net.Direction.IN)
            else:
            #PIN not defined in doDesign
                sin_net = workCell.getNet("SIN")
                if sin_net is None or not sin_net.isExternal():
                #PIN not defined in verilog or defined in verilog but not as an input
                #it can be a different signal than Scan Enable
                    sin_net = self.create_unique_net(workCell, "SIN")
            # SOUT
            if sout_defined:
            #PIN defined/placed in doDesign
                sout_net = workCell.getNet("SOUT")
                if sout_net is None:
                #PIN does not exist in the netlist
                #It has to be created with the same Name
                    sout_net = Net.create(workCell, "SOUT")
                    sout_net.setExternal(True)
                    sout_net.setDirection(Net.Direction.OUT)
            else:
            #PIN not defined in doDesign
                sout_net = workCell.getNet("SOUT")
                if sout_net is None or not sout_net.isExternal():
                #PIN not defined in verilog or defined in verilog but not as an input
                #it can be a different signal than Scan Enable
                    sout_net = self.create_unique_net(workCell, "SOUT")
                           
            # SE
            if se_defined:
            #PIN defined/placed in doDesign
                se_net = workCell.getNet("SE")
                if se_net is None:
                #PIN does not exist in the netlist
                #It has to be created with the same Name
                    se_net = Net.create(workCell, "SE")
                    se_net.setExternal(True)
                    se_net.setDirection(Net.Direction.IN)
            else:
            #PIN not defined in doDesign
                se_net = workCell.getNet("SE")
                if se_net is None or not se_net.isExternal():
                #PIN not defined in verilog or defined in verilog but not as an input
                #it can be a different signal than Scan Enable
                    se_net = self.create_unique_net(workCell, "SE")
                    
            # create a buffer that is driving SE
            se_fanout = self.create_unique_net(workCell, "SE_FANOUT")
            se_buf = self.create_unique_instance(workCell, "scanen_buf", buf)
    
            for plug in se_buf.getPlugs():
                name = plug.getMasterNet().getName()
                if name == buf_pins["z"]:
                    plug.setNet(se_fanout)
                elif name == buf_pins["i"]:
                    plug.setNet(se_net)
    
            # connect SE to all scan FFs
            for inst in sdffs:
                for plug in inst.getPlugs():
                    pin = plug.getMasterNet().getName()
                    if pin == scan_pins["se"]:
                        plug.setNet(se_fanout)
            # create a buffer that is driving SOUT
            sout_buf = self.create_unique_instance(workCell, "scanout_buf", buf)
            for plug in sout_buf.getPlugs():
                name = plug.getMasterNet().getName()
                if name == buf_pins["z"]:
                    plug.setNet(sout_net)
                # input is connected later, after scan chain ordering
                elif name == buf_pins["i"]:
                    pass             
    
        # END OF PRE-PLACEMENT DFT
        editor = self.conf.editor
        print( '  o  Building block "{}".'.format(self.conf.cell.getName()) )
        for blockInstance in self.blockInstances:
            blockInstance.block.conf.editor = editor
            if not blockInstance.block.conf.isBuilt:
                print( '     - Build sub-block "{}".'
                       .format(blockInstance.block.conf.cell.getName()) )
                blockInstance.block.build()
        if editor: editor.setCell(self.conf.cellPnR)
        self.conf.cfg.apply()
        iteration = -1
        while True:
            iteration += 1
            if iteration > 0: break
            self.setupAb()
            if editor: editor.fit()
            if not self.conf.isCoreBlock:
                if not self.conf.ioPins:
                #Automatically place pins with uniform spacing, ensuring all sides are utilized.
                #DFT pins are not placed in this step. There placement is optimized.
                    self.conf.ioPinsInTracks = True
                    self.autoPlaceIoPins()

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
            self.etesian.addTrackAvoid(trackAvoid)
        self.etesian.toHurricane()
        self.etesian.flattenPower()
        if self.conf.isCoreBlock:
            self.doConnectCore()
    
        # POST-PLACEMENT DFT
        # Reorder flip-flops according to placement to minimize stitching length
        sdffs = self.find_best_scan_chain_and_io(workCell, sdffs,sout_buf)
        side_sin , px_sin , py_sin  = self.project_to_edge(workCell, sdffs[0],  scan_pins["si"])
        side_sout, px_sout, py_sout = self.project_to_edge(workCell, sout_buf, buf_pins["z"])
    
        with UpdateSession():
    
            for scanff_master in used_scanff_masters:
                scanff_master.setTerminalNetlist(False)
    
            # -------- placement SE --------
            side_se, px_se, py_se = self.project_to_edge(workCell, se_buf, buf_pins["i"])
            new_side_se, new_px_se, new_py_se = self.legalize_with_fallback(workCell, side_se, px_se, py_se)

            if not se_defined:
             se_pin = self.createPinPostPlacement(
                workCell, se_net, "IN", new_side_se, new_px_se, new_py_se
            )
    
            # -------- placement SIN / connect SIN --------
            for plug in sdffs[0].getPlugs():
                if plug.getMasterNet().getName() == scan_pins["si"]:
                    plug.setNet(sin_net)
    
            new_side_sin, new_px_sin, new_py_sin = self.legalize_with_fallback(workCell, side_sin, px_sin, py_sin)

            if not sin_defined:
             sin_pin = self.createPinPostPlacement(
                workCell, sin_net, "IN", new_side_sin, new_px_sin, new_py_sin
            )
    
            # -------- placement SOUT / connect SOUT --------
            last_q_net = None
            for plug in sdffs[-1].getPlugs():
                if plug.getMasterNet().getName() == scan_pins["q"]:
                    last_q_net = plug.getNet()
                    break
    
            if last_q_net is None:
                raise ErrorMessage(1, "Cannot find last FF Q net")
            for plug in sout_buf.getPlugs():
                if plug.getMasterNet().getName() == buf_pins["i"]:
                    plug.setNet(last_q_net)
            new_side_sout, new_px_sout, new_py_sout = self.legalize_with_fallback(workCell,side_sout,px_sout,py_sout)
           
            if not sout_defined:
                sout_pin = self.createPinPostPlacement(
                    workCell,sout_net,"OUT",new_side_sout,new_px_sout,new_py_sout
                )         
    
            # scan chain stitching
            for k in range(len(sdffs)-1):
                qnet = None
                for plug in sdffs[k].getPlugs():
                    if plug.getMasterNet().getName() == scan_pins["q"]:
                        qnet = plug.getNet()
                for plug in sdffs[k+1].getPlugs():
                    if plug.getMasterNet().getName() == scan_pins["si"]:
                        plug.setNet(qnet)
    
            # add routing pad for the SI plugs
            for inst in sdffs:
                for plug in inst.getPlugs():
                    master = plug.getMasterNet()
                    if master is not None and master.getName() == scan_pins["si"]:
                        net = plug.getNet()
                        RoutingPad.create(net, Occurrence(plug), RoutingPad.BiggestArea)
            # routing pad for SOUT buffer input
            for plug in sout_buf.getPlugs():
                if plug.getMasterNet().getName() == buf_pins["i"]:
                    net = plug.getNet()
                    RoutingPad.create(net, Occurrence(plug), RoutingPad.BiggestArea)             
        self.dump_json_scan_chain(workCell,sdffs,sin_net,sout_net,se_net, scan_pins)
        # END OF POST-PLACEMENT DFT
        pnrStatus = self.route()
        if not self.conf.isCoreBlock:
            self.addBlockages()
            self.expandIoPins()
        self.conf.isBuilt = True
        lvxStatus = True
        if self.conf.doLvx is not False:
            lvxStatus = self.doLvx()
        if pnrStatus != lvxStatus:
            raise ErrorMessage( 1, [ 'PnR and LVX status incoherency (PnR={}, LVX={})'
                                     .format( pnrStatus, lvxStatus )
                                   , 'This strongly hints at a bug in the PnR...'
                                   ] )
        ####DEBUG that print all suspicious nets####
        #self.debug_check_routed_nets(workCell)
        #self.congestion(workCell, af)
        return pnrStatus and lvxStatus
    

    def congestion(self,workCell,af):
     """
     This function computes an approximation of the top cell congestion.
     It is computed by dividing the area in squares of 2 x 2 gcells.
     Then the function computes the number of wires that crosses each square.
     """
     import numpy as np
     
     # =========================
     # 1. GCell info
     # =========================
     cellGauge = af.getCellGauge()
     
     gcellHeight = cellGauge.getSliceHeight()
     gcellStep   = cellGauge.getSliceStep()
     
     gcellWidth = cellGauge.getSliceHeight()
     if gcellWidth % gcellStep:
         gcellWidth = ((gcellWidth // gcellStep) + 1) * gcellStep
     
     print("=== GCell Info ===")
     print("Height :", DbU.getValueString(gcellHeight), "| raw:", gcellHeight)
     print("Width  :", DbU.getValueString(gcellWidth),  "| raw:", gcellWidth)
     print("Step   :", DbU.getValueString(gcellStep),   "| raw:", gcellStep)
     
     
     # =========================
     # 2. Chip area
     # =========================
     box = workCell.getAbutmentBox()
     
     xmin = box.getXMin()
     ymin = box.getYMin()
     xmax = box.getXMax()
     ymax = box.getYMax()
     
     width  = xmax - xmin
     height = ymax - ymin
     
     print("\n=== Chip Area ===")
     print("xmin:", DbU.getValueString(xmin), "| raw:", xmin)
     print("ymin:", DbU.getValueString(ymin), "| raw:", ymin)
     print("xmax:", DbU.getValueString(xmax), "| raw:", xmax)
     print("ymax:", DbU.getValueString(ymax), "| raw:", ymax)
     print("Width :", DbU.getValueString(width),  "| raw:", width)
     print("Height:", DbU.getValueString(height), "| raw:", height)
     
     
     # =========================
     # 3.  GCells number
     # =========================
     nx_gcell = width  // gcellWidth
     ny_gcell = height // gcellHeight
     
     print("\n=== GCell Grid ===")
     print("Nb GCells X:", nx_gcell)
     print("Nb GCells Y:", ny_gcell)
     
     
     # =========================
     # 4. Bin = 2x2 GCells
     # =========================
     bin_size_x = 2 * gcellWidth
     bin_size_y = 2 * gcellHeight
     
     nx_bin = width  // bin_size_x
     ny_bin = height // bin_size_y
     
     print("\n=== Bins (2x2 GCells) ===")
     print("Bin size X:", DbU.getValueString(bin_size_x), "| raw:", bin_size_x)
     print("Bin size Y:", DbU.getValueString(bin_size_y), "| raw:", bin_size_y)
     print("Nb bins X :", nx_bin)
     print("Nb bins Y :", ny_bin)
     
     
     # =========================
     # 5. grid
     # =========================
     grid = [[0 for _ in range(int(nx_bin))] for _ in range(int(ny_bin))]
     
     print("\n=== Grid created ===")
     print("Grid size:", len(grid), "x", len(grid[0]) if grid else 0)
     
     
     # =========================
     # 6.  Segments number per grid
     # =========================
     for nett in workCell.getNets():
         for segg in nett.getSegments():
     
             sx = segg.getSourceX()
             sy = segg.getSourceY()
             tx = segg.getTargetX()
             ty = segg.getTargetY()
     
             dx = tx - sx
             dy = ty - sy
     
             steps = max(abs(dx), abs(dy)) // gcellStep + 1
     
             if steps == 0:
                 steps = 1
     
             for i in range(int(steps) + 1):
                 x = sx + dx * i // steps
                 y = sy + dy * i // steps
     
                 bx = (x - xmin) // bin_size_x
                 by = (y - ymin) // bin_size_y
     
                 if 0 <= bx < nx_bin and 0 <= by < ny_bin:
                     grid[int(by)][int(bx)] += 1
     
     
     # =========================
     # 7. Grid print
     # =========================
     print("\n=== Congestion Grid ===")
     
     for y in range(int(ny_bin)):
         for x in range(int(nx_bin)):
             print(f"{grid[y][x]:4}", end=" ")
         print()
     
     
     # =========================
     # 9. Stats 
     # =========================
     arr = np.array(grid)
     
     print("\n=== Stats ===")
     print("Max :", arr.max())
     print("Mean:", arr.mean())
     print("Std :", arr.std())
     

    def debug_check_routed_nets(self, workCell):
         """
         Paranoid debug that helps to find unrouted segments when developping complexe features
         using routing pads and segments
         """

         for net in workCell.getNets():
           plugs = len(list(net.getPlugs()))
           segs  = len(list(net.getSegments()))
           rps   = len(list(net.getRoutingPads()))
                                                                              
           if segs == 0:
               print(f"❌ NET {net.getName()} NOT ROUTED")
                                                                              
           if net.isExternal():
             if rps != plugs+1 :
                      print(f"⚠️ NET {net.getName()} PROBABLY MISSING ROUTING PADS, {rps,plugs}") 
           else :        
             if rps != plugs :
                print(f"⚠️ NET {net.getName()} PROBABLY MISSING ROUTING PADS  {rps,plugs}") 
    
    def dump_json_scan_chain(self, workCell, sdffs, sin_net, sout_net, se_net, scan_pins):
        """
        Dump the scan chain structure into a JSON file for external usage (testbenches, debug).
        The chain is described in logical order with SI/Q connectivity reconstructed from stitching.
        """
        import json
        chain_dump = []
        # -------- SI --------
        prev_q = sin_net.getName()

        for i, inst in enumerate(sdffs):

            inst_name = inst.getName()
            q_net_name = None

            # -------- get the sff pin Q --------
            for plug in inst.getPlugs():
                if plug.getMasterNet().getName() == scan_pins["q"]:
                    net = plug.getNet()
                    if net:
                        q_net_name = net.getName()
                    break

            if q_net_name is None:
                raise ErrorMessage(1, f"Missing Q net on instance {inst_name}")

            # -------- start ff --------
            chain_dump.append({
                "index": i,
                "instance": inst_name,
                "si_net": prev_q,
                "q_net": q_net_name
            })

            # -------- propagation SI -> Q --------
            prev_q = q_net_name

        data = {
            "sin": sin_net.getName(),
            "sout": sout_net.getName(),
            "se": se_net.getName(),
            "chain": chain_dump
        }

        with open("scan_chain.json", "w") as f:
            json.dump(data, f, indent=2) 
        
    def doLvx ( self ):
        """
        Performs and optional gate-level extraction and LVX to independently
        ensure that the PnR has not gone wrong. Return a boolean status.
        """
        if self.conf.isCoreBlock:
            if self.conf.doLvx == 'corona':
                topCell = self.conf.corona
            else:
                topCell = self.conf.chip
        else:
            topCell = self.conf.cell
            self.conf.cfg.tramontana.mergeSupplies = True
        self.conf.cfg.apply()
        self.tramontana = Tramontana.TramontanaEngine.create( topCell )
        self.tramontana.printConfiguration()
        self.tramontana.extract()
        self.tramontana.printSummary()
        status = self.tramontana.getSuccessState()
        Breakpoint.stop( 100, 'Block.doLvx() Extract+LVS status={}, before Tramontana.destroy().' \
                              .format( status ))
        if status:
            self.tramontana.destroy()
            self.tramontana = None
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


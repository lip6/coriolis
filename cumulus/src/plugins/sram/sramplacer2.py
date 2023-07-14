
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2022-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/sram/sramplacer.py"             |
# +-----------------------------------------------------------------+


import sys
import re
import traceback
from   ...helpers.io        import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay   import UpdateSession
from   ...helpers           import setTraceLevel, trace, l, u, n
import ...cumulus           import plugins
from   ...Hurricane         import Breakpoint, DbU, Box, Net, Cell, Instance, \
                                   Transformation, PythonAttributes
from   ...Foehn             import FoehnEngine, DagExtension
from   ..chip.configuration import GaugeConf


"""
Automatic placement of a Yosys generated SRAM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* We were expecting the output decoder to be the same for each bit
  line, allowing us to rebuild a matrix-like placement. This is not so.
  Each output mux equation is synthetised differently. Knowing that
  we did create a row-based placement, with reordering capabilities
  so we can optimize the mux placement.

* Alas, the previous effort was doomed from the start. If you have
  the same multiplexing function for all the bits, the command signals
  from the decoder are the same. For example, to mux 256 words,
  assuming we use only mux2, we need 8 bits (control lines).
  Given that we have also to take into account "ce", "we", "rst"
  and "oe", there are more of them, but not so much. Let's say 20.
 
    When running placeSRAM and looking at the last level (5) of the
  DAG's decoder, we see that it contains 832 gates, which means as
  much command signals. That is 26 control signals *per* bit.
  This is the direct consequence that *each* multiplexer has it's
  own structure. 26 signals takes up more than half the horizontal
  routing capacity of a slice (40), this result in an unroutable
  design, the bits are kept into one row each.
    832 gates is for the TSMC 180nm, for SkyWater 130nm we got
  976 gates on the third level.

Conclusions
~~~~~~~~~~~

1. A Yosys generated SRAM cannot be regularly  placed, neither in
   2-D matrix fashion nor in simple bit-line organization.

2. Worse, a thorough analysis of the generated netlist shows it is
   highly sub-optimal. Yosys generate *way* too much signals to
   achieve it, resulting in a bloated design.

3. Creating a small generator of SRAM, even based on standard cells
   would be a great improvement over the Yosys generated one.
   (the simpler OpenRAM approach)

Looking backward, as we were using Yosys generated SRAM in the LibreSOC,
that explain lot of the observed congestion.
"""


# --------------------------------------------------------------------
# Class :  StdCellConf.

class StdCellConf ( object ):
    """
    Gives meta informations about the standard cell library.
    """

    reDFF     = re.compile( r'^sff' )
    reMux     = re.compile( r'^n?mx[0-9]_' )
    reDataIn  = re.compile( r'^i[0-9]?' )
    reDataOut = re.compile( r'^n?q' )

    def isRegister ( self, cell ):
        """Returns True if the cell is a register."""
        m = StdCellConf.reDFF.match( cell.getName() )
        if m: return True
        return False

    def isMux ( self, cell ):
        """Returns True if the cell is a multiplexer."""
        m = StdCellConf.reMux.match( cell.getName() )
        if m: return True
        return False

    def isDataIn ( self, net ):
        """Returns True if the net is a data input."""
        m = StdCellConf.reDataIn.match( net.getName() )
        if m:
            if not (net.getDirection() & Net.Direction.DirIn):
                print( Warning( [ 'StdCellConf.isDataIn(): Match input pattern "^i[0_9]" but not in input direction.'
                                , 'On {}'.format(net) ] ))
            return True
        return False

    def isDataOut ( self, net ):
        """Returns True if the net is a data output."""
        m = StdCellConf.reDataOut.match( net.getName() )
        if m:
            if not (net.getDirection() & Net.Direction.DirOut):
                print( Warning( [ 'StdCellConf.isDataOut(): Match output pattern "^i[0_9]" but not in output direction.'
                                , 'On {}'.format(net) ] ))
            return True
        return False

    def isData ( self, net ):
        """Returns True if the net is a data flow (i.e. not a control)."""
        return self.isDataIn(net) or self.isDataOut(net)


# --------------------------------------------------------------------
# Class :  DAG.

class DAG ( object ):

    def __init__ ( self, rootInst ):
        self.rootInst     = rootInst
        self.reacheds     = [ rootInst ]
        self.processeds   = []
        self.tree         = []
        self.propagDir    = Net.Direction.DirIn
        self.externalNets = []
        self.libConf      = StdCellConf()

    def _isInPropagationDir ( self, plug ):
        trace( 600, ',+', '\tDAG._isInPropagationDir() on {}\n'.format( plug ))
        masterNet = plug.getMasterNet()
        if masterNet.isSupply():
            trace( 600, ',-', '\tFalse (Supply) {}\n'.format( masterNet ))
            return False
        if masterNet.isClock():
            trace( 600, ',-', '\tFalse (Clock) {}\n'.format( masterNet ))
            return False
        if (masterNet.getDirection() & self.propagDir) ^ self.propagDir:
            trace( 600, ',-', '\tFalse {}\n'.format( masterNet ))
            return False
        if not self.libConf.isData(masterNet):
            trace( 600, ',-', '\tFalse (not data-flow) {}\n'.format( masterNet ))
            return False
        trace( 600, ',-', '\tTrue {}\n'.format( masterNet ))
        return True

    def _processInst ( self ):
        rvalue = False
        if not self.reacheds:
            return rvalue
        current = self.reacheds[ 0 ]
        trace( 600, ',+', '\tDAG._processInst() on {}\n'.format(current) )
        self.reacheds.remove( current )
        self.processeds.append( current )
        for currentPlug in current.getPlugs():
            trace( 600, ',+', '\tLooking for {}\n'.format( currentPlug ))
            if not self._isInPropagationDir(currentPlug):
                trace( 600, ',-', '\tMaster net not in propag dir {}\n'.format( currentPlug.getMasterNet() ))
                continue
            net = currentPlug.getNet()
            trace( 600, ',+', '\tReached {}\n'.format( net ))
            if net.isExternal():
                trace( 600, ',--', '\tExternal net reached: {}\n'.format( net ))
                self.externalNets.append( net )
                continue
            for plug in net.getPlugs():
                if plug == currentPlug:
                    continue
                if self._isInPropagationDir(plug):
                    continue
                reachedInst = plug.getInstance()
                if not reachedInst in self.reacheds and not reachedInst in self.processeds:
                    trace( 600, '\tInstance reached: {}\n'.format( reachedInst ))
                    self.reacheds.append( reachedInst )
                    rvalue = True
            trace( 600, ',--' )
        trace( 600, ',-', '\treturn {}\n'.format( rvalue ))
        return rvalue

    def _backtrackNet ( self, net ):
        trace( 600, ',+', '\tDAG._backtraceNet() {}\n'.format( net ))
        for plug in net.getPlugs():
            inst = plug.getInstance()
            if inst in self.processeds:
                trace( 600, '\tbacktrack: {}\n'.format( inst ))
                self.processeds.remove( inst )
                if inst == self.rootInst:
                    continue
                self.tree.append( inst )
                for instPlug in inst.getPlugs():
                    if instPlug.getNet() == net:
                        continue
                    if not self._isInPropagationDir(instPlug):
                        continue
                    self._backtrackNet( instPlug.getNet() )
        trace( 600, ',-', '\tDone\n' )

    def _backtrack ( self ):
        for net in self.externalNets:
            self._backtrackNet( net )

    def build ( self ):
        while self._processInst():
            pass
        self._backtrack()

    def show ( self ):
        print( 'DAG Tree:' )
        for inst in self.tree:
            print( '| {}'.format( inst ))
                
    


# --------------------------------------------------------------------
# Class :  RowLeaf.

class RowLeaf ( object ):
    """
    Manage one leaf (atomic Instance) in a whole bit row.
    """
    KIND_WORD       = 0x0001
    KIND_INPUT_MUX  = 0x0002
    KIND_OUTPUT_MUX = 0x0004
    KIND_GROUP      = 0x0008

    def __init__ ( self, instance, kind, tag, bit ):
        self.tag      = tag
        self.parent   = None
        self.kind     = kind
        self.instance = instance
        self.width    = instance.getMasterCell().getAbutmentBox().getWidth()
        self.depth    = 0
        PythonAttributes.enable( self.instance )
        self.instance.row = self
        self.instance.bit = bit

    @property
    def root ( self ):
        return self.parent

    @property
    def bit ( self ):
        return self.instance.bit

    def __str__ ( self ):
        return '<RowLeaf d={} {} {}>'.format( self.depth, self.tag, self.parent )

    def getDriver ( self ):
        for plug in self.instance.getPlugs():
            if plug.getMasterNet().getDirection() & Net.Direction.DirOut:
                return plug.getNet()
        return None

    def findChild ( self, tag ):
        if self.tag == tag: return self
        return None

    def placeAt ( self, transf ):
        self.instance.setTransformation( transf )
        self.instance.setPlacementStatus( Instance.PlacementStatus.PLACED )
        return self.instance.getAbutmentBox()

    def showTree ( self, depth  ):
        trace( 610, '\t{}| {}\n'.format( '  '*depth, self ))


# --------------------------------------------------------------------
# Class :  Row.

class Row ( object ):
    """
    Manage a group of RowLeaf and/or Row (recursive).
    """
    
    def __init__ ( self, tag ):
        """
        Initialize an *empty* column group. Sub-group or columns must be
        added afterwards.
        """
        self.tag    = tag
        self.parent = None
        self.depth  = 0
        self.childs = []

    def __iter__ ( self ):
        return RowIterator( self )

    @property
    def kind ( self ): return RowLeaf.KIND_GROUP

    @property
    def width ( self ):
        width = 0
        for child in self.childs:
            width += child.width
        return width

    @property
    def root ( self ):
        if self.parent:
            return self.parent.root
        return self

    @property
    def bit ( self ):
        next(iter( self )).bit

    def __str__ ( self ):
        return '<Row d={} {} {}>'.format( self.depth, self.tag, self.parent )

    def group ( self, child ):
        """ Add a new child to the group. """
        self.childs.append( child )
        child.parent = self
        self.depth = max( self.depth, child.depth+1 )

    def unGroup ( self, child ):
        """ Remove a child from the group (the child is *not* deleted). """
        if child in self.childs:
            self.childs.remove( child )
        child.parent = None
        self.depth = 0
        for child in self.childs:
            self.depth = max( self.depth, child.depth )
        self.depth += 1

    def findChild ( self, tag ):
        """ Recursively find a child in a goup by it's tag name. """
        if self.tag == tag: return self
        for child in self.childs:
            rchild = child.findChild( tag )
            if rchild: return rchild
        return None

    def placeAt ( self, groupTransf ):
        """
        Place childs/colums from left to rigth.
        """
        width = 0
        bb    = Box()
        for child in self.childs:
            bb.merge( child.placeAt( Transformation( groupTransf.getTx()+width
                                                   , groupTransf.getTy()
                                                   , groupTransf.getOrientation() )))
            width += child.width
        return bb

    def showTree ( self, depth=0 ):
        trace( 610, '\t{}+ {}\n'.format( '  '*depth, self ))
        for child in self.childs:
            child.showTree( depth+1 )


# --------------------------------------------------------------------
# Class :  RowIterator.

class RowIterator ( object ):
    """
    Iterator over a Row. The iterator returns *only* the RowLeafs,
    not the intermediate Row groups.
    """

    def __init__ ( self, row ):
        self.row       = row
        self.index     = 0
        self.childIter = None

    def __next__ ( self ):
        while True:
            try:
                if self.childIter:
                    return next( self.childIter )
            except StopIteration:
                self.childIter = None
            if self.index < len(self.row.childs):
                child = self.row.childs[ self.index ]
                self.index += 1
                if isinstance(child,Row):
                    self.childIter = iter( child )
                    continue
                return child
            else:
                raise StopIteration


# --------------------------------------------------------------------
# Class :  SRAMPlacer.

class SRAMPlacer ( object ):
    """
    Takes a Yosys generated SRAM and place it on a regular matrix.
    """

    patIgnoreMasterNet = '^cmd[0-9]?$'
    reIgnoreMasterNet  = None

    def __init__ ( self, cell ):
        """
        Initialize a completely empty SRAM placer. Build functions must
        be called separately.
        """
        self.gaugeConf    = GaugeConf()
        self.libConf      = StdCellConf()
        self.cell         = cell
        self.rows         = []
        self.totalInsts   = 0
        self.placedInsts  = 0
        self.totalLength  = 0
        self.placedLength = 0
        self.decodDff     = []
        self.foehn        = FoehnEngine.create( self.cell )
        if not SRAMPlacer.reIgnoreMasterNet:
            SRAMPlacer.reIgnoreMasterNet = re.compile( SRAMPlacer.patIgnoreMasterNet )

    def __del__ ( self ):
        if self.foehn: self.foehn.destroy()

    def _groupInputRows ( self, instance ):
        """
        Group this instance with all it's drivers input into a single new group.
        """
        if not isinstance(instance,Instance):
            return
        if hasattr(instance,'row'):
            trace( 610, '\t         | Already grouped in {}\n'.format( instance.row ))
            return

        drivers = []
        for plug in instance.getPlugs():
            if not plug.getNet():
                continue
            if not (plug.getMasterNet().getDirection() & Net.Direction.DirIn):
                continue
            m = SRAMPlacer.reIgnoreMasterNet.match( plug.getMasterNet().getName() )
            if m:
                continue
            driverInst = DagExtension.getDriver( plug.getNet() )
            if not driverInst:
                continue
            if not hasattr(driverInst,'row') or driverInst.row is None:
                continue
            drivers.append( driverInst )
            trace( 610, '\t         | driver {}\n'.format( driverInst.row ))
        if not len(drivers):
            return
        bit   = drivers[0].bit
        words = []
        for driver in drivers:
            if driver.row.tag.startswith('word'):
                words.append( int( driver.row.tag[4:] ))
            elif driver.row.tag.startswith('mux'):
                words += [ int(words) for words in driver.row.tag[3:].split( '_' ) ]
        words.sort()
        muxTag = 'mux'
        for i in range(len(words)):
            if i: muxTag += '_'
            muxTag += str( words[i] )
        muxGroup = Row( muxTag+'_g' )
        muxRow   = RowLeaf( instance, RowLeaf.KIND_OUTPUT_MUX, muxTag, bit )
        midRange = len(drivers) // 2
        for i in range(len(drivers)):
            if i == midRange:
                muxGroup.group( muxRow )
            self.rows[bit].unGroup( drivers[i].row.parent )
            muxGroup.group( drivers[i].row.parent )
        self.rows[bit].group( muxGroup )
        trace( 610, '\t         | Grouped in {}\n'.format( muxTag ))
                
    def findChild ( self, bit, tag ):
        """ Search for a leaf row named ``tag`` in row ``bit`` """
        return self.createRow( bit ).findChild( tag )

    def createRow ( self, bit ):
        """
        Returns the row at index ``bit``, expand the list and create it
        if needed. All intermediate missing rows are also createds.
        """
        if len(self.rows) > bit:
            return self.rows[ bit ]
        while len(self.rows) <= bit:
            self.rows.append( Row( 'root{}'.format(bit) ))
        return self.rows[-1]

    def addMemBit ( self, addr, bit, inst ):
        """
        Add a *memory element* in the matrix.

        :param addr: The address of the word the bit is part of (X).
        :param bit:  The bit in the word (Y).
        :param inst: The register (DFF) instance.
        """
        row     = self.createRow( bit )
        wordTag = 'word{:d}'.format( addr )
        rowLeaf = row.findChild( wordTag )
        if not rowLeaf:
            word    = Row( wordTag+'_g' )
            rowLeaf = RowLeaf( inst, RowLeaf.KIND_WORD, wordTag, bit )
            word.group( rowLeaf )
            row .group( word )
        else:
            print( ErrorMessage( 1, 'SRAMPlacer.addMemBit(): duplicated bit at ({},{})' \
                                    .format( addr, bit )))

    def findHighFanout ( self ):
        """
        Find high-fanout nets (more than 20 sinks).
        """
        trace( 610, '+,', '\tFind high fanout nets (above 20 sinks).\n' )
        for net in self.cell.getNets():
            if net.isSupply(): continue
            if net.isClock(): continue
            sinks = 0
            for plug in net.getPlugs():
                if plug.getMasterNet().getDirection() & Net.Direction.DirIn:
                    sinks += 1
            if sinks > 20:
                trace( 610, '\t- {} sinks on {}\n'.format( sinks, net ))
        trace( 610, '-,' )

    def findMemBits ( self ):
        """
        Match the memory bits (DFF) into the matrix. Match is as follow:

        * ``byte`` : the byte number. This is 4 for words of 32 bits size.
        * ``bit``  : the bit inside a byte (should always be 8).
        * ``word`` : the word identifier, this is the numerical value of
                     the address. 

        Yosys seems to split 32 bits value in a per byte fashion.
        """
        reMem = re.compile( r'^mem(?P<byte>\d+)_(?P<word>\d+)_(?P<bit>\d+)' )
        for instance in self.cell.getInstances():
            PythonAttributes.enable( instance )
            cellLength = instance.getAbutmentBox().getWidth()
            self.totalInsts  += 1
            self.totalLength += cellLength
            for plug in instance.getPlugs():
                if plug.getMasterNet().getDirection() & Net.Direction.DirOut:
                    m = reMem.match( plug.getNet().getName() )
                    if m:
                        self.addMemBit( int(m.group('word'))
                                      , int(m.group('byte'))*8 + int(m.group('bit'))
                                      , instance )
                        continue

    def findDecodDff ( self ):
        """
        Match the DFF holding the decoded read address. Match is as follow:

        Yosys seems to split 32 bits value in a per byte fashion.
        """
        trace( 610, '+,', '\tFind decoder related DFFs.\n' )
        reMem1 = re.compile( r'^mem(?P<byte>\d+)_rdreg_(?P<word>\d+)_q_(?P<bit>\d+)' )
        reMem2 = re.compile( r'^raddr\((?P<byte>\d+)\)' )
        for instance in self.cell.getInstances():
            cellLength = instance.getAbutmentBox().getWidth()
            for plug in instance.getPlugs():
                if plug.getMasterNet().getDirection() & Net.Direction.DirOut:
                    m1 = reMem1.match( plug.getNet().getName() )
                    if not m1:
                        m2 = reMem2.match( plug.getNet().getName() )
                        if not m2:
                            continue
                    self.decodDff.append( instance )
        for instance in self.decodDff:
            trace( 610, '\t| {}\n'.format( instance ))
        trace( 610, '-,' )

    def findInputMuxes ( self ):
        """
        Perform a DAG propagation to find the input muxes attached to each
        memory bits.
        """
        placeds = []
        for inst in self.cell.getInstances():
            if not hasattr(inst,'row') or inst.row is None:
                continue
            if inst.row.kind != RowLeaf.KIND_WORD:
                continue
            dag = DAG( inst )
            dag.build()
           #dag.show()
            muxTag  = 'mux_{}'.format(inst.row.tag)
            rootRow = inst.row.root
            muxRow  = rootRow.findChild( muxTag )
            if not muxRow:
                muxRow = RowLeaf( dag.tree[0], RowLeaf.KIND_INPUT_MUX, muxTag, inst.bit )
                inst.row.parent.group( muxRow )
            placeds.append( (muxRow, dag.tree[0]) )

    def findDecod ( self ):
        """
        Perform a DAG propagation to find the output muxes attached to each
        memory bits.
        """
        trace( 610, '+,', '\tDecoder DAG\n' )
        dagDecoder = self.foehn.newDag( 'decoder' )
        dagDecoder.addDStart( self.cell.getNet( 'rst' ))
        dagDecoder.addDStart( self.cell.getNet( 'ce' ))
        dagDecoder.addDStart( self.cell.getNet( 'oe' ))
        dagDecoder.addDStart( self.cell.getNet( 'we(0)' ))
        dagDecoder.addDStart( self.cell.getNet( 'we(1)' ))
        dagDecoder.addDStart( self.cell.getNet( 'we(2)' ))
        dagDecoder.addDStart( self.cell.getNet( 'we(3)' ))
        for net in self.cell.getNets():
            if not net.isExternal(): continue
            if not (net.getDirection() & Net.Direction.DirIn): continue
            if not net.getName().startswith('addr'): continue
            dagDecoder.addDStart( net )
        for inst in self.decodDff:
            dagDecoder.addDStart( inst )
        dagDecoder.dpropagate()
        self.runDag( dagDecoder )
        order = 0
        for dbo in dagDecoder.getDOrder():
            if isinstance(dbo,Instance):
                PythonAttributes.enable( dbo )
                dbo.row = None
            order += 1
            if order > 2600:
                if isinstance(dbo,Instance):
                    self.getSupportNets( dbo )
        dagDecoder.resetDepths()
        self.dagDecoder = dagDecoder
        trace( 610, '-,' )

    def findOutputMuxes ( self ):
        """
        Perform a direct DAG propagation to find the output muxes attached
        to each memory bits.
        """
        trace( 610, '+,', '\tFinding output muxes\n' )
        dagDecoded = self.foehn.newDag( 'decoded' )
        dagDecoded.addDStart( self.cell.getNet( 'rst' ))
        for inst in self.decodDff:
            dagDecoded.addDStart( inst )
        dagDecoded.dpropagate()
        dagDecoded.resetDepths()
        for index in range(32):
            trace( 610, '+,', '\tProcessing bit {}\n'.format( index ))
            dagOMux = self.foehn.newDag( 'outputMuxes_{}'.format(index) )
            dagOMux.setIgnoredMasterNetRe( SRAMPlacer.patIgnoreMasterNet )
            for inst in self.cell.getInstances():
                if not hasattr(inst,'row') or inst.row is None:
                    continue
                if inst.bit != index:
                    continue
                if not self.libConf.isRegister(inst.getMasterCell()):
                    continue
                dagOMux.addDStart( inst )
            dagOMux.dpropagate()
            self.runDag( dagOMux, self._groupInputRows )
            dagOMux.resetDepths()
            trace( 610, '-,' )
        trace( 610, '-,' )

    def runDag ( self, dag, callback=None ):
        """
        Run ``callback()`` on every Instance element of the DAG
        in topological order. Can also additionnaly display the
        order of the DAG.
        """
        order     = 0
        depth     = 0
        depthSize = 0
        trace( 610, '+,' )
        for dbo in dag.getDOrder():
            minDepth = DagExtension.getMinDepth( dbo )
            if minDepth != depth:
                trace( 610, '\tDepth {} has {} gates.\n'.format( depth, depthSize ))
                depth     = minDepth
                depthSize = 0
            trace( 610, '\t{:05d} {:02d} | {}\n'.format( order, minDepth, dbo ))
            if hasattr(dbo,'row'):
                trace( 610, '\t         | {}\n'.format( dbo.row ))
            if isinstance(dbo,Net):
                trace( 610, '\t         | driver={}\n'.format( DagExtension.getDriver( dbo )))
            if isinstance(dbo,Instance):
                if callback: callback( dbo )
                depthSize += 1
            order += 1
        trace( 610, '\tDepth {} has {} gates.\n'.format( depth, depthSize ))
        trace( 610, '-,' )

    def getSupportNets ( self, rootInst ):
        """
        Find the set of *entry* nets (of the DAG) the ``rootInst`` is depending upon.
        """
        support = set()
        stack   = [ rootInst ]
        istack  = 0
        while istack < len(stack):
           #print( 'istack={} on {}'.format( istack, stack[istack], flush=True ))
            instance = stack[ istack ]
            istack  += 1
            for plug in instance.getPlugs():
                if not plug.getNet(): continue
                if not (plug.getMasterNet().getDirection() & Net.Direction.DirIn): continue
                if not DagExtension.isPresent(plug.getNet()):
                    support.add( plug.getNet() )
                    continue
                driver = DagExtension.getDriver( plug.getNet() )
                if not driver:
                    support.add( plug.getNet() )
                    continue
                stack.append( driver )
        trace( 610, '\tSupport of {}\n'.format( rootInst ))
        trace( 610, '\t  => ' )
        for net in support:
            trace( 610, '{} '.format(net.getName() ))
        trace( 610, '\n' )

    def show ( self ):
        """
        Display the matrix contents.
        """
        trace( 610, '+,', '\tSRAM Tree of {}\n'.format(self.cell) )
        self.rows[0].showTree()
        trace( 610, '-,' )
        trace( 610, '+', '\tSRAM Matrix of {}\n'.format(self.cell) )
        trace( 610, '\tCells: {}, placed: {} {:.1%}, remains {}, area placed: {:.1%}\n' \
                    .format( self.totalInsts
                           , self.placedInsts
                           , self.placedInsts / self.totalInsts
                           , self.totalInsts - self.placedInsts
                           , self.placedLength/self.totalLength ))
        for bit in range(len(self.rows)):
            trace( 610, ',+', '\tRow/bit {}\n'.format( bit ))
            for leaf in self.rows[bit]:
                trace( 610, '\tLeaf [{},{}] -> {} @{}\n' \
                            .format( 'X'
                                   , bit
                                   , leaf.getDriver().getName()
                                   , leaf.instance.getTransformation()
                                   ))
            trace( 610, '-,' )
        trace( 610, '\tSRAM non placed instances:\n' )
        for instance in self.cell.getInstances():
            if not DagExtension.isPresent(instance): continue
            if hasattr(instance,'row'): continue
            trace( 610, '\t| {}\n'.format( instance ))
            if self.libConf.isRegister(instance.getMasterCell()):
                trace( 610, '\t| REGISTER\n' )
        trace( 610, '\tSRAM non matched instances:\n' )
        for instance in self.cell.getInstances():
            if DagExtension.isPresent(instance): continue
            if hasattr(instance,'row'): continue
            trace( 610, '\t| {}\n'.format(instance) )
            if self.libConf.isRegister(instance.getMasterCell()):
                trace( 610, '\t| REGISTER\n' )

    def placeAt ( self, matrixTransf=Transformation() ):
        """
        Place the matrix. Each row on top of each other.
        """
        bb           = self.placeDecoder( matrixTransf )
        matrixTransf = Transformation( matrixTransf.getTx() + bb.getWidth()
                                     , matrixTransf.getTy()
                                     , matrixTransf.getOrientation() )
        sliceHeight = next(iter( self.rows[0] )).instance.getMasterCell().getAbutmentBox().getHeight()
        with UpdateSession():
            for bit in range(len(self.rows)):
                y      = bit*sliceHeight
                orient = Transformation.Orientation.ID
                if bit % 2:
                    y     += sliceHeight
                    orient = Transformation.Orientation.MY
                transf = Transformation( 0, y, orient )
                matrixTransf.applyOn( transf )
                bb.merge( self.rows[bit].placeAt( transf ))
            self.cell.setAbutmentBox( bb )
        for row in self.rows:
            for leaf in row:
                self.placedLength += leaf.instance.getMasterCell().getAbutmentBox().getWidth()
                self.placedInsts  += 1

    def placeDecoder ( self, matrixTransf=Transformation() ):
        """
        Place the decoder part of the SRAM. Simple layered implementation
        for now.
        """
        sliceHeight = next(iter( self.rows[0] )).instance.getMasterCell().getAbutmentBox().getHeight()
        nbRows      = len( self.rows )
        decRows     = []
        for bit in range(nbRows):
            y      = bit*sliceHeight
            orient = Transformation.Orientation.ID
            if bit % 2:
                y     += sliceHeight
                orient = Transformation.Orientation.MY
            transf = Transformation( 0, y, orient )
            matrixTransf.applyOn( transf )
            decRows.append( [ transf, 0, [] ] )
        order = 0
        bb    = Box()
        with UpdateSession():
            for dbo in self.dagDecoder.getDOrder():
                if not isinstance(dbo,Instance):
                    continue
                irow      = order % nbRows
                rowTransf = decRows[ irow ][ 0 ]
                rowWidth  = decRows[ irow ][ 1 ]
                transf    = Transformation( rowTransf.getTx() + rowWidth
                                          , rowTransf.getTy()
                                          , rowTransf.getOrientation() )
                dbo.setTransformation( transf )
                dbo.setPlacementStatus( Instance.PlacementStatus.PLACED )
                decRows[ irow ][ 1 ] += dbo.getAbutmentBox().getWidth()
                bb.merge( dbo.getAbutmentBox() )
                self.placedLength += dbo.getMasterCell().getAbutmentBox().getWidth()
                self.placedInsts  += 1
                order += 1
        return bb
                

# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    """
    The mandatory function to make the plugin appears in the menus.
    """
    plugins.kwUnicornHook( 'tools.SRAMPlacer'
                         , 'Soft SRAM Placer'
                         , 'Placer dedicated to Yosys generated SRAM'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return
            

def scriptMain ( **kw ):
    """
    The mandatory function from which a plugin will be called by Coriolis CGT/Unicorn.
    """

    rvalue = True
    try:
        DbU.setStringMode( DbU.StringModeReal, DbU.UnitPowerMicro )
       #setTraceLevel( 500 )
        cell, editor = plugins.kwParseMain( **kw )
        placer = SRAMPlacer( cell )
        placer.findMemBits()
        placer.placeAt()
        placer.show()
        if editor: editor.fit()
        return True
    except Exception as e:
        catch( e )
        rvalue = False
    sys.stdout.flush()
    sys.stderr.flush()
    return rvalue

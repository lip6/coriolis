
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
# |  Python      :       "./plugins/sram/sramplacer1.py"            |
# +-----------------------------------------------------------------+


import sys
import re
import traceback
from   ...helpers.io        import ErrorMessage, WarningMessage, catch
from   ...helpers.overlay   import UpdateSession
from   ...helpers           import trace, l, u, n
from   ...cumulus           import  plugins
from   ...Hurricane         import Breakpoint, DbU, Box, Net, Cell, Instance, \
                                   Transformation, PythonAttributes
from   ...Foehn             import FoehnEngine, DagExtension
from   ..chip.configuration import GaugeConf


"""
Automatic placement of a Yosys generated SRAM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2-D matrix like flavor.

For the documentation of this trial, please refer to sramplacer2.py.
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
# Class :  Column.

class Column ( object ):
    """
    Manage one column in the SRAM matrix.
    """
    KIND_WORD       = 0x0001
    KIND_INPUT_MUX  = 0x0002
    KIND_OUTPUT_MUX = 0x0004
    KIND_GROUP      = 0x0008

    def __init__ ( self, kind, tag=None ):
        self.tag     = tag
        self.parent  = None
        self.kind    = kind
        self.bits    = []
        self.width   = 0
        self.depth   = 0

    @property
    def root ( self ):
        return self.parent

    @property
    def busWidth ( self ):
        return len( self.bits )

    def __str__ ( self ):
        return '<Column d={} {} {} {}>'.format( self.depth, self.tag, len(self.bits), self.parent )

    def getBit ( self, bit ):
        if bit >= len(self.bits): return None
        return self.bits[bit]

    def addToEmptyRow ( self, bit, inst ):
        print( 'self.width={} ABwidth={}'.format(
            DbU.getValueString(self.width), DbU.getValueString(inst.getMasterCell().getAbutmentBox().getWidth()) ))
        if inst.getMasterCell().getAbutmentBox().getWidth() != self.width:
            print( 'Rejected' )
            return False
        if self.getBit(bit):
            return False
        self.addRow( bit, inst )
        return True

    def addRow ( self, bit, inst ):
        if len(self.bits) > bit:
            self.bits[ bit ] = inst
        else:
            while len(self.bits) < bit:
                self.bits.append( None )
            self.bits.append( inst )
        self.width = max( self.width, inst.getMasterCell().getAbutmentBox().getWidth() )

    def getDriver ( self, bit ):
        for plug in self.bits[bit].getPlugs():
            if plug.getMasterNet().getDirection() & Net.Direction.DirOut:
                return plug.getNet()
        return None

    def findChild ( self, tag ):
        if self.tag == tag: return self
        return None

    def placeAt ( self, colTransf ):
        for bit in range(len(self.bits)):
            if self.bits[bit] is not None:
                sliceHeight  = self.bits[bit].getMasterCell().getAbutmentBox().getHeight()
        bb = Box()
        for bit in range(len(self.bits)):
            if self.bits[bit] is None:
                continue
            y      = bit*sliceHeight
            orient = Transformation.Orientation.ID
            if bit % 2:
                y     += sliceHeight
                orient = Transformation.Orientation.MY
            transf = Transformation( 0, y, orient )
            colTransf.applyOn( transf )
            self.bits[bit].setTransformation( transf )
            self.bits[bit].setPlacementStatus( Instance.PlacementStatus.PLACED )
            bb.merge( self.bits[bit].getAbutmentBox() )
        return bb

    def showTree ( self, depth  ):
        print( '{}| {}'.format( '  '*depth, self ))


# --------------------------------------------------------------------
# Class :  ColGroup.

class ColGroup ( object ):
    """
    Manage a group of Column and/or ColGroup (recursive).
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
        return ColGroupIterator( self )

    @property
    def kind ( self ): return Column.KIND_GROUP

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
    def busWidth ( self ):
        busWidth = 0
        for child in self.childs:
            busWidth = max( busWidth, child.busWidth )
        return busWidth

    def __str__ ( self ):
        return '<ColGroup d={} {} {}>'.format( self.depth, self.tag, self.parent )

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
        print( '{}+ {}'.format( '  '*depth, self ))
        for child in self.childs:
            child.showTree( depth+1 )


# --------------------------------------------------------------------
# Class :  ColGroupIterator.

class ColGroupIterator ( object ):

    def __init__ ( self, colGroup ):
        self._colGroup = colGroup
        self._index    = 0
        self._subIter  = None

    def __next__ ( self ):
        while True:
            try:
                if self._subIter:
                    column = next( self._subIter )
                    return column
            except StopIteration:
                self._columnIter = None
            if self._index < len(self._colGroup.childs):
                column = self._colGroup.childs[self._index]
                self._index += 1
                if isinstance(column,ColGroup):
                    self._subIter = iter( column )
                    continue
                return column
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
    CREATE_COLUMNS     = 1
    REUSE_COLUMNS      = 2

    def __init__ ( self, cell ):
        """
        Initialize a completely empty SRAM placer. Build functions must
        be called separately.
        """
        self.gaugeConf    = GaugeConf()
        self.libConf      = StdCellConf()
        self.cell         = cell
        self.matrix       = ColGroup( 'root' )
        self.totalInsts   = 0
        self.placedInsts  = 0
        self.totalLength  = 0
        self.placedLength = 0
        self.decodDff     = []
        if not SRAMPlacer.reIgnoreMasterNet:
            SRAMPlacer.reIgnoreMasterNet = re.compile( SRAMPlacer.patIgnoreMasterNet )

    def _groupInputColumns ( self, instance, mode ):
        """
        Group this instance with all it's drivers input into a single new group.
        """
        if hasattr(instance,'column'):
            print( '       | Already grouped in {}'.format( instance.column ))
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
            if not hasattr(driverInst,'column'):
                continue
            drivers.append( driverInst )
            print( '       | driver {}'.format( driverInst.column ))
        if not len(drivers):
            return
        bits = []
        for driver in drivers:
            if driver.column.tag.startswith('word'):
                bits.append( int( driver.column.tag[4:] ))
            elif driver.column.tag.startswith('mux'):
                bits += [ int(bit) for bit in driver.column.tag[3:].split( '_' ) ]
        bits.sort()
        muxTag = 'mux'
        for i in range(len(bits)):
            if i: muxTag += '_'
            muxTag += str( bits[i] )
        if mode == SRAMPlacer.CREATE_COLUMNS:
            muxGroup = ColGroup( muxTag+'_g' )
            muxCol   = Column( Column.KIND_OUTPUT_MUX, muxTag )
            muxCol.addRow( drivers[0].bit, instance )
            instance.column = muxCol
            instance.bit    = drivers[0].bit
            midRange = len(drivers) // 2
            for i in range(len(drivers)):
                if i == midRange:
                    muxGroup.group( muxCol )
                self.matrix.unGroup( drivers[i].column.parent )
                muxGroup.group( drivers[i].column.parent )
            self.matrix.group( muxGroup )
            print( '       | Grouped in {}'.format( muxTag ))
        else:
            muxCol = self.matrix.findChild( muxTag )
            if not muxCol:
                maxDepth = len(bits) + 1
                bitTag   = 'word{}'.format( bits[0] )
                groupCol = self.matrix.findChild( bitTag ).parent
                while groupCol.depth <= maxDepth:
                    groupCol = groupCol.parent
                bits  = [ int(bit) for bit in groupCol.tag[3:-2].split( '_' ) ]
                added = False
                print( '       | mismatched maxDepth={} muxTag="{}"'.format( maxDepth, muxTag ))
                for bit in bits:
                    print( '       | bit {}'.format( bit ))
                    bitTag   = 'word{}'.format( bit )
                    groupCol = self.matrix.findChild( bitTag ).parent
                    print( '       | Look in childs of {}'.format( groupCol ))
                    while groupCol.depth <= maxDepth:
                        for child in groupCol.childs:
                            print( '       | try {}'.format( child ))
                            if child.tag.startswith('mux') and not child.tag.endswith('_g'):
                                if child.addToEmptyRow( drivers[0].bit, instance ):
                                    print( '       | added in="{}"'.format( child.tag ))
                                    added = True
                                    break
                        if added: break
                        groupCol = groupCol.parent
                        print( '       | Look in childs of {}'.format( groupCol ))
                        if not groupCol: break
                    if added: break
            else:
                print( '       | matched muxTag="{}"'.format( muxTag ))
                muxCol.addRow( drivers[0].bit, instance )
                
    def findChild ( self, tag ):
        """ Search for a leaf column named ``tag`` """
        return self.matrix.findChild( tag )

    def addMemBit ( self, addr, bit, inst ):
        """
        Add a *memory element* in the matrix.

        :param addr: The address of the word the bit is part of (X).
        :param bit:  The bit in the word (Y).
        :param inst: The register (DFF) instance.
        """
        wordTag = 'word{:d}'.format( addr )
        column  = self.findChild( wordTag )
        if not column:
            wordGroup = ColGroup( wordTag+'_g' )
            column    = Column( Column.KIND_WORD, wordTag )
            wordGroup.group( column )
            self.matrix.group( wordGroup )
        column.addRow( bit, inst )
        inst.column = column
        inst.bit    = bit

    def findMemBits ( self ):
        """
        Match the memory bits (DFF) into the matrix. Match is as follow:

        * ``byte`` : the byte number. This is 4 for words of 32 bits size.
        * ``bit``  : the bit inside a byte (should always be 8).
        * ``word`` : the word identifier, this is the numerical value of
                     the address. 

        Yosys seems to split 32 bits value in a per byte fashion.
        """
        reMem  = re.compile( r'^mem(?P<byte>\d+)_(?P<word>\d+)_(?P<bit>\d+)' )
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
                        self.placedLength += cellLength
                        self.placedInsts  += 1
                        continue

    def findDecodDff ( self ):
        """
        Match the DFF holding the decoded read address. Match is as follow:

        Yosys seems to split 32 bits value in a per byte fashion.
        """
        reMem  = re.compile( r'^mem(?P<byte>\d+)_rdreg_(?P<word>\d+)_q_(?P<bit>\d+)' )
        for instance in self.cell.getInstances():
            cellLength = instance.getAbutmentBox().getWidth()
            for plug in instance.getPlugs():
                if plug.getMasterNet().getDirection() & Net.Direction.DirOut:
                    m = reMem.match( plug.getNet().getName() )
                    if m:
                        self.decodDff.append( instance )
                        continue
        print( 'Decoder DFFs:' )
        for instance in self.decodDff:
            print( '| ', instance )

    def findInputMuxes ( self ):
        """
        Perform a DAG propagation to find the input muxes attached to each
        memory bits.
        """
        placeds = []
        for inst in self.cell.getInstances():
            if not hasattr(inst,'column'):
                continue
            dag = DAG( inst )
            dag.build()
           #dag.show()
            muxTag  = 'mux_{}'.format(inst.column.tag)
            rootCol = inst.column.root
            muxCol  = rootCol.findChild( muxTag )
            if not muxCol:
                muxCol = Column( Column.KIND_INPUT_MUX, muxTag )
                inst.column.parent.group( muxCol )
            muxCol.addRow( inst.bit, dag.tree[0] )
            placeds.append( (muxCol, dag.tree[0], inst.bit) )
        for column, inst, bit in placeds:
            cellLength = inst.getAbutmentBox().getWidth()
            self.placedLength += cellLength
            self.placedInsts  += 1
            inst.column = column
            inst.bit    = bit

    def findOutputMuxes ( self ):
        """
        Perform a DAG propagation to find the output muxes attached to each
        memory bits.
        """
        foehn = FoehnEngine.create( self.cell )
        dagDecod = foehn.newDag( 'decoder' )
        dagDecod.addDStart( self.cell.getNet( 'rst' ))
        for inst in self.decodDff:
            dagDecod.addDStart( inst )
        dagDecod.dpropagate()
        dagDecod.resetDepths()
        for index in range(256):
            print( 'PROCESSING BIT {}'.format( index ))
            mode = SRAMPlacer.REUSE_COLUMNS
            if index == 0:
                mode = SRAMPlacer.CREATE_COLUMNS
            dagOMux = foehn.newDag( 'outputMuxes_{}'.format(index) )
            dagOMux.setIgnoredMasterNetRe( SRAMPlacer.patIgnoreMasterNet )
            for inst in self.cell.getInstances():
                if not hasattr(inst,'column'):
                    continue
                if inst.bit != index:
                    continue
                if not self.libConf.isRegister(inst.getMasterCell()):
                    continue
                dagOMux.addDStart( inst )
            dagOMux.dpropagate()
            order     = 0
            depth     = 0
            depthSize = 0
            for dbo in dagOMux.getDOrder():
                minDepth = DagExtension.getMinDepth( dbo )
                if minDepth != depth:
                    print( 'Depth {} has {} gates.'.format( depth, depthSize ))
                    depth     = minDepth
                    depthSize = 0
                print( '{:03d} {:02d} | {}'.format( order, minDepth, dbo ))
                if hasattr(dbo,'column'):
                    print( '       | {}'.format( dbo.column ))
                if isinstance(dbo,Net):
                    print( '       | driver={}'.format( DagExtension.getDriver( dbo )))
                if isinstance(dbo,Instance):
                    self._groupInputColumns( dbo, mode )
                    depthSize += 1
                order += 1
            dagOMux.resetDepths()
        foehn.destroy()

    def show ( self ):
        """
        Display the matrix contents.
        """
        print( 'SRAM Tree of {}'.format(self.cell) )
        self.matrix.showTree()
        print( 'SRAM Matrix of {}'.format(self.cell) )
        print( '  Cells: {}, placed: {:.1%}, area placed: {:.1%}' \
               .format( self.totalInsts
                      , self.placedInsts /self.totalInsts
                      , self.placedLength/self.totalLength ))
        for column in self.matrix:
            print( '  Column {}'.format( column ))
            sys.stdout.flush()
            sys.stderr.flush()
            for yindex in range(len(column.bits)):
                if not column.bits[yindex]: continue
                print( '    Row [{},{}] {} -> {} @{}' \
                       .format( 'X'
                              , yindex
                              , column.bits[yindex]
                              , column.getDriver(yindex).getName()
                              , column.bits[yindex].getTransformation()
                              ))

    def placeAt ( self, matrixTransf=Transformation() ):
        """
        Place the matrix (regularly).
        """
        with UpdateSession():
            bb = Box()
            x  = 0
            for column in self.matrix:
                print( 'place {}'.format( column ))
                sys.stdout.flush()
                sys.stderr.flush()
                colTransf = Transformation( x, 0, Transformation.Orientation.ID )
                matrixTransf.applyOn( colTransf )
                bb.merge( column.placeAt( colTransf ))
                x += column.width
            self.cell.setAbutmentBox( bb )


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

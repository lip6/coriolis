
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
# |  Python      :       "./plugins/sram.py"                        |
# +-----------------------------------------------------------------+


"""
The ``sram`` module provide base classes for SRAM assemby.
"""


import sys
import re
import traceback
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import UpdateSession
from   ...helpers            import trace, l, u, n
from   ...Hurricane          import DataBase, Breakpoint, DbU, Box, Net, Cell, \
                                    Instance, Transformation, PythonAttributes
from   ...                   import CRL
from   ..block.configuration import GaugeConf


af = CRL.AllianceFramework.get()


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

    def __init__ ( self ):
        self.techName = DataBase.getDB().getTechnology().getName()

    def __repr__ ( self ):
        return '<StdCellConf "{}">'.format( self.techName )

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

    def getStdCellName ( self, name ):
        if self.techName == 'Sky130':
            if name == 'na2_x1': name = 'nand2_x0'
            if name == 'no2_x1': name = 'nor2_x0'
            if name == 'no3_x1': name = 'nor3_x0'
        return name

    def getStdCell ( self, name ):
        return af.getCell( self.getStdCellName(name), CRL.Catalog.State.Views )


# --------------------------------------------------------------------
# Class :  Bus.

class Bus ( object ):
    """
    A set of Net to be manipulated as a bus.
    """

    def __init__ ( self, sram, netFmt, bits ):
        self.sram = sram
        self.nets = []
        if isinstance(bits,int):
            bits = range( bits )
        for bit in bits:
            self.nets.append( self.sram.getNet( netFmt.format( bit )))
        self.sram.busses[ netFmt ] = self

    @property
    def busWidth ( self ):
        return len( self.nets )

    def __getitem__ ( self, i ):
        return self.nets[ i ]


# --------------------------------------------------------------------
# Class :  Column.

class Column ( object ):
    """
    Build & manage a column of identical master cells.
    """
    KIND_COLUMN = 0x0001
    KIND_BLOCK  = 0x0002
    KIND_GROUP  = 0x0004

    def __init__ ( self, sram, masterCell, tag, bitNaming, busWidth ):
        """
        Build (instanciate) a column of identical insts of ``masterCell``.
        Instances names are generated through the ``instNaming`` callable.
        """
        self.sram      = sram
        self.tag       = tag
        self.parent    = None
        self.order     = None
        self.bitNaming = bitNaming
        self.busWidth  = busWidth
        self.insts     = []
        self.busPlugs  = {}
        self.revert    = False
        naming = ColNaming( self.tag + self.bitNaming )
        for bit in range(busWidth):
            self.insts.append( Instance.create( self.sram.cell, naming(bit), masterCell ))
        for net in masterCell.getNets():
            if self.sram.confLib.isDataIn(net) or self.sram.confLib.isDataOut(net):
                busPlug = []
                for inst in self.insts:
                    busPlug.append( inst.getPlug( net ))
                self.busPlugs[ net.getName() ] = busPlug

    def __del__ ( self ):
        """ Needed to disable Python attribute uses on reference instance. """
        PythonAttributes.disable( self.insts[0] )

    @property
    def kind ( self ): return Column.KIND_COLUMN

    @property
    def depth ( self ): return 0

    @property
    def width ( self ):
        """ Width of the Column. """
        return self.insts[0].getMasterCell().getAbutmentBox().getWidth()

    @property
    def root ( self ):
        """ Return the root of the tree. """
        if self.parent:
            return self.parent.root
        return self

    def __repr__ ( self ):
        return '<Column "{}" d={} {} {}>'.format( self.insts[0].getMasterCell().getName()
                                                , self.depth
                                                , self.tag
                                                , self.busWidth )

    def findChild ( self, tag ):
        """
        Terminal function for ColGroup.findChild(), display itsef if ``tag`` match.
        """
        if self.tag == tag: return self
        return None

    def setBusNet ( self, busName, busNet ):
        """
        Connect a bus to the column. ``busName`` is the name of the master net
        in the reference cell of the column.
        """
        if not busName in self.busPlugs:
            raise ErrorMessage( 1, 'Column.setBusNet(): {} has no bus named "{}".' \
                                   .format( self.tag, busName ))
        busPlug = self.busPlugs[ busName ]
        if busPlug[0].getNet() and busPlug[0].getNet() != busNet[0]:
            print( Warning( 'Column.setBusNet(): Overrode {} {} -> {} with {}' \
                            .format( busPlug[0].getInstance()
                                   , busName
                                   , busPlug[0].getNet()
                                   , busNet[0] )))
        for i in range(self.busWidth):
            busPlug[ i ].setNet( busNet[i] )
        trace( 605, '\tsetBusNet {} {} -> {}\n'.format( busPlug[0].getInstance(), busName, busNet[0] ))

    def setCmdNet ( self, cmd, net ):
        masterNet = self.insts[0].getMasterCell().getNet( cmd )
        for bus in range(self.busWidth):
            self.insts[ bus ].getPlug( masterNet ).setNet( net )

    def setCutCost ( self, order ):
        self.order = order + 1
        return self.order

    def reverse ( self ):
        """ reverse() toggle a MX symmetry to the column. """
        self.revert = not self.revert

    def place ( self ):
        """ Perform the placement of the column at ``x``. """
        trace( 610, ',+', "\tColumn.place() tag={}\n".format( self.tag ))
        bb = Box()
        self.sram.doFoldAtColumn( self )
        x, irow = self.sram.foldState.getPosition()
        for bit in range(self.busWidth):
            if bit == 0:
                PythonAttributes.enable( self.insts[0] )
                self.insts[0].fold = self.sram.foldState.fold
            trace( 610, "\tx={} irow={} tag={}\n" \
                        .format( DbU.getValueString(x), irow+bit, self.tag ))
            self.sram.placeInstance( self.insts[ bit ], x, irow+bit, self.sram.foldState.direction, self.revert )
            bb.merge( self.insts[bit].getAbutmentBox() )
        self.sram.foldState.addWidth( self.width )
        trace( 610, '-,' )
        return bb

    def showTree ( self, depth  ):
        """ Terminal function for ColGroup.showTree(), display itsef. """
        print( '{}| {}'.format( '  '*depth, self ))



# --------------------------------------------------------------------
# Class :  ColBlock.

class ColBlock ( object ):
    """
    Manage an column made of irregular instances, more than one instance
    can be put on each slice.
    """

    def __init__ ( self, sram, tag, busWidth ):
        """
        Manage an column made of irregular instances, more than one instance
        can be put on each slice.
        """
        self.sram      = sram
        self.tag       = tag
        self.parent    = None
        self.busWidth  = busWidth
        self.width     = 0
        self.widths    = []
        self.rows      = []
        for i in range(self.busWidth):
            self.rows.append( [] )
            self.widths.append( 0 )

    @property
    def kind ( self ): return Column.KIND_BLOCK

    @property
    def depth ( self ): return 0

    @property
    def root ( self ):
        """ Return the root of the tree. """
        if self.parent:
            return self.parent.root
        return self

    def __repr__ ( self ):
        return '<ColBlock d={} {} {}>'.format( self.depth, self.tag, self.busWidth )

    def showTree ( self, depth  ):
        """ Terminal function for ColGroup.showTree(), display itsef. """
        print( '{}| {}'.format( '  '*depth, self ))

    def addInstance ( self, irow, inst ):
        """
        Add an instance to the column block. Try to add the instance in
        rows ``[irow,irow+3]`` so the width increase will be minimized.
        """
        inarrower = irow
        for i in range( irow+1, min( irow+4, len(self.rows))):
            if self.widths[ i ] < self.widths[ inarrower ]:
                inarrower = i
        self.rows  [ inarrower ].append( inst )
        self.widths[ inarrower ] += inst.getMasterCell().getAbutmentBox().getWidth()
        if self.widths[ inarrower ] > self.width:
            self.width = self.widths[ inarrower ]

    def place ( self, x ):
        """ Perform the placement of the column block at ``x``. """
        bb = Box()
        for irow in range(self.busWidth):
            xrow = x
            for inst in self.rows[irow]:
                self.sram.placeInstance( inst, xrow, irow )
                xrow += inst.getMasterCell().getAbutmentBox().getWidth()
                bb.merge( inst.getAbutmentBox() )
        return bb


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
        self.tag        = tag
        self.parent     = None
        self.order      = None
        self.depth      = 0
        self.childs     = []
        self.isReversed = False

    def __iter__ ( self ):
        return ColGroupIterator( self )

    def __repr__ ( self ):
        return '<ColGroup d={} {} childs={}>'.format( self.depth, self.tag, len(self.childs ))

    @property
    def kind ( self ): return Column.KIND_GROUP

    @property
    def width ( self ):
        """ Width of the whole ColGroup (sum of all it's children's width). """
        width = 0
        for child in self.childs:
            width += child.width
        return width

    @property
    def root ( self ):
        """ Return the root of the tree. """
        if self.parent:
            return self.parent.root
        return self

    @property
    def busWidth ( self ):
        """
        Return the width of the group. This the widest bus width of all the childs.
        """
        busWidth = 0
        for child in self.childs:
            busWidth = max( busWidth, child.busWidth )
        return busWidth

    def group ( self, newChild, after=None, before=None ):
        """ Add a new child to the group. """
        inserted = False
        if after is not None:
            for i in range(len(self.childs)):
                if self.childs[i] == after:
                    self.childs.insert( i+1, newChild )
                    inserted = True
                    break
        if before is not None:
            for i in range(len(self.childs)):
                if self.childs[i] == before:
                    self.childs.insert( i, newChild )
                    inserted = True
                    break
        if not inserted:
            self.childs.append( newChild )
        newChild.parent = self
        self.depth = max( self.depth, newChild.depth+1 )

    def unGroup ( self, child=None ):
        """ Remove a child from the group (the child is *not* deleted). """
        if child is None:
            if not self.parent:
                return
            self.parent.unGroup( self )
            return
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

    def setCutCost ( self, order ):
        order += 1
        self.order = order
        for child in self.childs:
            order = child.setCutCost( order )
        order += 1
        return order

    def reverse (self ):
        """
        Reverse the order of the childs of the group *and* perform it recursively on each
        child itself.
        """
        for child in self.childs:
            child.reverse()
        self.childs.reverse()
        self.isReversed = not self.isReversed

    def place ( self ):
        """ Place childs/colums from left to rigth. """
        bb = Box()
        for child in self.childs:
            bb.merge( child.place() )
        return bb

    def showTree ( self, depth=0 ):
        """ Display the tree rooted at this ColGroup. """
        print( '{}+ {}'.format( '  '*depth, self ))
        for child in self.childs:
            child.showTree( depth+1 )


# --------------------------------------------------------------------
# Class :  ColGroupIterator.

class ColGroupIterator ( object ):
    """
    Provide an iterator over all the *leaf* instances of a ColGroup tree.
    (all intermediate ColGroup are ignored)
    """

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
                self._subIter = None
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
# Class :  ColNaming.

class ColNaming ( object ):
    """
    Callable to generate the individual instances name of each bit
    in a regular column.
    """

    def __init__ ( self, instFormat ):
        self.instFormat = instFormat

    def __call__ ( self, bit ):
        return self.instFormat.format( wbit=bit, bbit=(bit%8), byte=(bit//8) )


# --------------------------------------------------------------------
# Class :  HeaderRow.

class HeaderRow ( object ):
    """
    Build the top row of the memory area datapath. Usually containing
    the buffers for the local command drivers.
    """

    def __init__ ( self, sram ):
        self.sram = sram
        self.row  = {}

    def addInstanceAt ( self, inst, refInst ):
        """
        Add an Instance in the header with it's reference instance, taken
        from the to be associated Column. The reference instance gives a
        hint about the X position the cell must be placed.
        """
        if refInst not in self.row:
            self.row[ refInst ] = []
        self.row[ refInst ].append( inst )

    def place ( self, xstart, irow, direction ):
        """
        Perform the placement of the header at ``(x,irow)``. Instances
        in the row are placed as close as possible to the X position of
        their reference instance (part of a Column).
        """
        def getXMin ( inst ):
            return inst.getAbutmentBox().getXMin()

        def getXMax ( inst ):
            return inst.getAbutmentBox().getXMax()


        trace( 610, ',+', '\tHeaderRow.place() {}\n'.format( DbU.getValueString(xstart) ))
        xsorteds = [] 
        reverse  = (direction != BaseSRAM.TO_RIGHT)
        for refInst in self.row.keys():
            trace( 610, '\trefInst.getXMin() {}\n'.format( DbU.getValueString(refInst.getAbutmentBox().getXMin()) ))
            xsorteds.append( refInst )
        bb   = Box()
        xsorteds.sort( key=getXMin, reverse=reverse )
        if direction == BaseSRAM.TO_RIGHT:
            x = xstart
            for refInst in xsorteds:
                xmin = getXMin( refInst )
                if xmin > x:
                    x = xmin
                for inst in self.row[ refInst ]:
                    self.sram.placeInstance( inst, x, irow, direction )
                    bb.merge( inst.getAbutmentBox() )
                    x += inst.getMasterCell().getAbutmentBox().getWidth()
        else:
            x = xstart
            for refInst in xsorteds:
                xmin = getXMax( refInst )
                if xmin < x:
                    x = xmin 
                for inst in self.row[ refInst ]:
                    self.sram.placeInstance( inst, x, irow, direction )
                    bb.merge( inst.getAbutmentBox() )
                    x -= inst.getMasterCell().getAbutmentBox().getWidth()
        trace( 610, '-,' )
        return bb
        

# --------------------------------------------------------------------
# Class :  FoldState.

class FoldState ( object ):
    """
    Manage information about how to fold the group tree and the
    current information during the placement process.
    """

    def __init__ ( self, sram, fold ):
        self.sram   = sram
        self.foldNb = fold
        self.fold   = 0

    def setupDimensions ( self ):
        """
        Compute folding coordinates. Must be called at the beginning
        of the placement stage, after all instances have been created.
        """
        foldedWidth    = self.sram.rootGroup.width // self.foldNb
        self.xmin      = self.sram.decoder.width
        self.xmax      = self.sram.decoder.width + foldedWidth
        self.x         = self.xmin
        self.irow      = 0
        self.direction = BaseSRAM.TO_RIGHT

    def getPosition ( self ):
        """ Return the position to put the next column. """
        return self.x, self.irow

    def forceFold ( self ):
        if self.direction == BaseSRAM.TO_RIGHT:
            self.direction = BaseSRAM.TO_LEFT
            self.x         = self.xmax
        else:
            self.direction = BaseSRAM.TO_RIGHT
            self.x         = self.xmin
        self.irow += self.sram.rootGroup.busWidth + 2
        self.fold += 1

    def addWidth ( self, width ):
        """ Increment the currently placed width and update ``(x,irow)``. """
        if self.direction == BaseSRAM.TO_RIGHT:
            self.x += width
        else:
            self.x -= width
        #if self.direction == BaseSRAM.TO_RIGHT:
        #    if self.x + width <= self.xmax:
        #        self.x += width
        #        return
        #    self.direction = BaseSRAM.TO_LEFT
        #    self.x         = self.xmax
        #else:
        #    if self.x - width >= self.xmin:
        #        self.x -= width
        #        return
        #    self.direction = BaseSRAM.TO_RIGHT
        #    self.x         = self.xmin
        #self.irow += self.sram.rootGroup.busWidth + 1
        #self.fold += 1
        

# --------------------------------------------------------------------
# Class :  BaseSRAM.

class BaseSRAM ( object ):
    """
    Base class for SRAM builder. Provides utility functions.
    """
    TO_RIGHT = 1
    TO_LEFT  = 2

    def __init__ ( self, fold ):
        """
        Create the base structure of a SRAM, which contains :

        * ``self.dffCell``   : The DFF standard cell which provides the single
                               bit storing.
        * ``self.rootGroup`` : A root column group named ``rootSRAM``, for the
                               matrix part (datapath).
        * ``self.decoder``   : The soft block to store decoder cells.
        * ``self.headers``   : The headers for the columns command and buffer
                               lines.

        The overall relative placement is organized as follow : ::

            +---------+-------------------------------------------+
            |         |            headers[4] (1 row)             |
            |         +-------------------------------------------+
            |         |            headers[3] (1 row)             |
            |         +-------------------------------------------+
            |         |                                           |
            |         |       Column area, fold 1 (N rows)        |
            |         |                                           |
            | decoder +-------------------------------------------+
            |         |            headers[1] (1 row)             |
            |         +-------------------------------------------+
            |         |            headers[0] (1 row)             |
            |         +-------------------------------------------+
            |         |                                           |
            |         |       Column area, fold 0 (N rows)        |
            |         |                                           |
            +---------+-------------------------------------------+
        """
        self.confLib     = StdCellConf()
        self.foldState   = FoldState( self, fold )
        self.cell        = None
        self.foldTags    = []
        self.dffCell     = af.getCell( 'sff1_x4', CRL.Catalog.State.Views )
        self.position    = Transformation()
        self.sliceHeight = self.dffCell.getAbutmentBox().getHeight()
        self.rootGroup   = ColGroup( 'rootSRAM' )
        self.busses      = {}
        self.decoder     = None
        self.toHeaders   = []
        self.headers     = [ HeaderRow( self ) for row in range(fold*2) ]

    @property
    def fold ( self ):
        return self.foldState.fold

    def doFoldAtColumn ( self, column ):
        if column.tag in self.foldTags:
            self.foldState.forceFold()

    def getBus ( self, fmt ):
        """ Find a bus by it's formatting string. """
        if fmt in self.busses:
            return self.busses[ fmt ]
        return None

    def getNet ( self, name, create=True ):
        """
        Find a Net by name. If it doesn't exists and ``create`` is set to ``True``,
        add it to the netlist.
        """
        net = self.cell.getNet( name )
        if net:
            return net
        if not create:
            return None
        return Net.create( self.cell, name )

    def addExternalNet ( self, name, direction, kind=Net.Type.LOGICAL ):
        """
        Add a Net to the cell interface.
        """
        net = self.getNet( name )
        net.setExternal ( True )
        net.setDirection( direction )
        net.setType     ( kind )
        if kind == Net.Type.POWER or kind == Net.Type.GROUND:
            net.setGlobal( True )
        return net

    def addInstance ( self, masterName, instName, netMapNames ):
        """
        Create an Instance named ``instName`` of model ``masterName`` and
        connect it's Plugs according to the dictionary ``netMapNames``.

        .. code:: python

           self.addInstance( 'a2_x2'
                           , 'decod_a2_1'
                           , { 'i0' : 'net_input_0'
                             , 'i1' : 'net_input_1'
                             ,  'q' : 'net_output_X' } )
        """
        masterCell = af.getCell( masterName, CRL.Catalog.State.Views )
        if not masterCell:
            raise ErrorMessage( 1, 'BaseSRAM.addInstance(): Cannot find cell "{}".'.format( masterName ))
        inst = Instance.create( self.cell, instName, masterCell )
        for masterNetName, netName in netMapNames.items():
            masterNet = masterCell.getNet( masterNetName )
            net       = self.getNet( netName )
            plug      = inst.getPlug( masterNet )
            plug.setNet( net )
        return inst

    def connect ( self, instName, masterNetName, netName ):
        """
        Connect the Plug ``masterNetName`` of instance ``instName`` to the
        net ``netName``.
        """
        inst      = self.cell.getInstance( instName )
        masterNet = inst.getMasterCell().getNet( masterNetName )
        net       = self.getNet( netName )
        inst.getPlug( masterNet ).setNet( net )

    def placeInstance ( self, inst, x, irow, direction=TO_RIGHT, reverse=False ):
        """
        Place an instance at a position defined by ``(x,irow)`` where :

        * ``x``    : the usual X coordinate.
        * ``irow`` : the row into which to put the cell, the Y coordinate
                     is computed from it, accounting for the X axis
                     flipping that occurs on one row over two.

        The position is relative to the bottom left corner of the design
        given by ``self.position``.

        .. note:: ``self.position`` should not contains rotations, unmanaged
                  for now.
        """
        orients = { BaseSRAM.TO_RIGHT : [ Transformation.Orientation.ID
                                        , Transformation.Orientation.MY ]
                  , BaseSRAM.TO_LEFT  : [ Transformation.Orientation.MX
                                        , Transformation.Orientation.R2 ]
                  }
        if reverse:
            if direction == BaseSRAM.TO_RIGHT:
                x += inst.getMasterCell().getAbutmentBox().getWidth()
                direction = BaseSRAM.TO_LEFT
            else:
                x -= inst.getMasterCell().getAbutmentBox().getWidth()
                direction = BaseSRAM.TO_RIGHT
        y      = irow*self.sliceHeight
        orient = orients[ direction ][ 0 ]
        if irow % 2:
            y     += self.sliceHeight
            orient = orients[ direction ][ 1 ]
        trace( 610, '\tBaseSRAM.placeInstance() x={} y={} orient={} {}\n' \
                    .format( DbU.getValueString(x), DbU.getValueString(y), orient, inst ))
        transf = Transformation( x, y, orient )
        self.position.applyOn( transf )
        inst.setTransformation( transf )
        inst.setPlacementStatus( Instance.PlacementStatus.FIXED )
        return inst.getAbutmentBox()

    def findFoldColumns ( self ):
        """
        Find the cuts between columns where the wiring is minimal.
        Based on the ColGroup tree, assuming that the deeper a column is
        in the tree the more they are closely connected and must not be
        separateds.
        """
        trace( 610, ',+', '\tBaseSRAM.findFoldcolumns()\n' )
        self.rootGroup.setCutCost( 0 )
        prevOrder = 0
        cutCosts  = {}
        count     = 0
        for column in self.rootGroup:
            cutCost = column.order-prevOrder
            if cutCost in cutCosts:
                cutCosts[ cutCost ].append(( count, column.tag ))
            else:
                cutCosts[ cutCost ] = [( count, column.tag )]
            trace( 610, '\t{:>4} {:>4} {:>4} {}\n'.format( count
                                                         , column.order
                                                         , column.order-prevOrder
                                                         , column ))
            prevOrder = column.order
            count += 1
        keys = list( cutCosts.keys() )
        keys.sort()
        for key in keys:
            trace( 610, '\tcutCost[ {} ] = \n'.format( key ))
            for order, tag in cutCosts[key]:
                trace( 610, '\t    | {:>3d} {}\n'.format( order, tag ))
        trace( 610, '-,' )

    def placeAt ( self, position=Transformation() ):
        """
        Perform the placement of all the various area of the SRAM.
        For the overall layout, see ``__init__()``.
        """
        self.findFoldColumns()
        self.position = position
        self.foldState.setupDimensions()
        with UpdateSession():
            bb = Box()
            bb.merge( self.decoder.place( 0 ) )
            bb.merge( self.rootGroup.place() )
            for inst, refInst, headerRow in self.toHeaders:
                self.headers[ refInst.fold*2 + headerRow ].addInstanceAt( inst, refInst )
            for i in range(len(self.headers)):
                trace( 610, ',+', 'Place row header {} {}\n'.format( i, self.headers[i].row ))
                if i//2 % 2:
                    xstart    = bb.getXMax()
                    direction = BaseSRAM.TO_LEFT
                else:
                    xstart    = self.decoder.width
                    direction = BaseSRAM.TO_RIGHT
                bb.merge( self.headers[i].place( xstart
                                               , self.rootGroup.busWidth*(i//2 + 1) + i
                                               , direction ))
                trace( 610, '-,' )
            self.cell.setAbutmentBox( bb )

    def showTree ( self ):
        """
        Display the Column tree of the SRAM.
        """
        self.rootGroup.showTree()

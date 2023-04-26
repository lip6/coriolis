
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
# |  Python      :       "./plugins/sram_256x32.py"                 |
# +-----------------------------------------------------------------+


"""
Verilog description of ``spram_256x32`` (256 words of 32 bits).
This descripion is the ASIC part of the SPRAM exctracted from
FreeCores : ::

    https://github.com/freecores/ethmac.git

.. code:: Verilog

   module eth_spram_256x32(
     // Generic synchronous single-port RAM interface
     clk, rst, ce, we, oe, addr, di, dato

     // Generic synchronous single-port RAM interface
     input            clk;  // Clock, rising edge
     input            rst;  // Reset, active high
     input            ce;   // Chip enable input, active high
     input   [ 3: 0]  we;   // Write enable input, active high
     input            oe;   // Output enable input, active high
     input   [ 7: 0]  addr; // address bus inputs
     input   [31: 0]  di;   // input data bus
     output  [31: 0]  dato; // output data bus

     reg     [ 7: 0]  mem0 [255:0];
     reg     [15: 8]  mem1 [255:0];
     reg     [23:16]  mem2 [255:0];
     reg     [31:24]  mem3 [255:0];
     wire    [31: 0]  q;
     reg     [ 7: 0]  raddr;
     
     // Data output drivers
     assign dato = (oe & ce) ? q : {32{1'bz}};
     
     // RAM read and write
     // read operation
     always@(posedge clk)
       if (ce)
         raddr <=  addr; // read address needs to be registered to read clock
     
     assign  q = rst ? {32{1'b0}} : { mem3[raddr]
                                    , mem2[raddr]
                                    , mem1[raddr]
                                    , mem0[raddr] };
     
     // write operation
     always@(posedge clk)
     begin
       if (ce && we[3]) mem3[addr] <=  di[31:24];
       if (ce && we[2]) mem2[addr] <=  di[23:16];
       if (ce && we[1]) mem1[addr] <=  di[15: 8];
       if (ce && we[0]) mem0[addr] <=  di[ 7: 0];
     end
   endmodule


Provisional results
~~~~~~~~~~~~~~~~~~~

.. note:: All length are in micro-meters.

+--------+--------------+-----------------------------+-----------------------------+
| Arch   | Kind         | Generator                   | Yosys                       |
+========+==============+=============================+=============================+
|  Mux   | # Gates      | 23209      (-25.4%)         | 32121                       |
+--------+--------------+-----------------------------+                             |
|  Nao   | # Gates      | 34637      (+7.8%)          |                             |
+--------+--------------+-----------------------------+-----------------------------+
|                                       1 Fold                                      |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 7182 x 330  (-5.5%)         | 7380 x 340                  |
|  Mux   +--------------+-----------------------------+-----------------------------+
|        | Wirelength   | 1841036     (-4.3%)         | 1924153                     |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 6680 x 340  (-14.9%)        |                             |
|  Nao   +--------------+-----------------------------+                             |
|        | Wirelength   | 1637781     (-14.9%)        |                             |
+--------+--------------+-----------------------------+-----------------------------+
|                                       2 Fold                                      |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 3599 x 660  (-5.3%)         | 3690 x 680                  |
|  Mux   +--------------+-----------------------------+-----------------------------+
|        | Wirelength   | 1670455     (-6.3%)         | 1782558                     |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 3350 x 680  (-9.2%)         |                             |
|  Nao   +--------------+-----------------------------+                             |
|        | Wirelength   | 1548358     (-13.1%)        |                             |
+--------+--------------+-----------------------------+-----------------------------+
|                                       4 Fold                                      |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 1812 x 1320 (-4.6%)         | 1900 x 1320                 |
|  Mux   +--------------+-----------------------------+-----------------------------+
|        | Wirelength   | 1699810     (-1.5%)         | 1726436                     |
+--------+--------------+-----------------------------+-----------------------------+
|        | Area         | 1692 x 1360 (-8.2%)         |                             |
|  Nao   +--------------+-----------------------------+                             |
|        | Wirelength   | 1512107     (-12.4%)        |                             |
+--------+--------------+-----------------------------+-----------------------------+


The difference between the two implementations resides only in the *output*
multiplexer. With a 4 inputs mux made of mux2+mux3 or 2 inputs multiplexer
made of alternate layers of nand2+nor2.

Conclusions for the mux2+mux3 implementation :

1. The generator version uses subtantially less gates than the Yosys one.
   As the both SRAM uses the exact same number of SFFs, the difference is
   only due to the decoder for the control of input and output muxes.

2. Notwithanding having less gates the generator version uses similar areas,
   which means that we use fewer but significantly *bigger* cells.

3. The FlexLib library supplied for SkyWater 130nm do not contains all
   SxLib one, effectively restricting our choices.

   In particular, to build the output multiplexer we only have mx2 and
   mx3 cells, which are large. The density of the SRAM could be much
   increased if we did have nmx2 and nmx3.

   Furthermore for the output multiplexers, as it is a controlled case,
   we may also uses three-state drivers cells (which have not been
   ported either).

Conclusion for the nand2+nor2 implementation:

1. The multiplexer allows us for a much more compact area and noticeably
   lesser wire length. With an increased number of cells (not an issue).

2. The total wire length is extremely sensitive to the placement, which
   in our case is just a column ordering. To optimize, the binary tree
   (for the netlist) is not placed fully symmetrically but slightly
   "askew".


.. note:: Cell width in the SkyWater 130 port of FlexLib:

          ==============  =====
          Cell            Width
          ==============  =====
          inv_x2          2
          mx2_x2          7
          mx3_x2          11
          a3_x2           5
          nand2_x0        2
          nand3_x0        3
          nand4_x0        4
          nor2_x0         2
          nor3_x0         3
          sff1_x4         15
          ==============  =====

          Differrent ways of implementing the output multiplexer :

          1. mx2_x2 + mx3_x2         = 18
          2. 9 * nand2_x0            = 18
          3. 4 * nand3_x0 + nand4_x0 = 16
          4. 6 * nand2_x0 + nor2_x0  = 14
"""


import sys
import re
import traceback
from   ...helpers.io         import ErrorMessage, WarningMessage
from   ...helpers.overlay    import UpdateSession
from   ...helpers            import trace, l, u, n
from   ...Hurricane          import Breakpoint, DbU, Box, Net, Cell, Instance, \
                                    Transformation, PythonAttributes
from   ...                   import CRL
from   ..block.configuration import GaugeConf
from   .sram                 import Bus, Column, ColBlock, ColGroup, \
                                    HeaderRow, BaseSRAM 


"""
Simple Standard cells based SRAM generator.
"""


af = CRL.AllianceFramework.get()
        

# --------------------------------------------------------------------
# Class :  SRAM_256x32.

class SRAM_256x32 ( BaseSRAM ):
    """
    Build & place a SRAM of 256 words of 32 bits.
    """
    BIT_GROUP_FMT = 'bit_addr{:04d}_g'
    MUX_GROUP_FMT = 'bits_{}_g'

    def __init__ ( self, fold, name=None ):
        BaseSRAM.__init__( self, fold )
        if fold == 1:
            pass
        elif fold == 2:
           #self.foldTags = [ 'imux_addr0128' ]
            self.foldTags = [ 'imux_addr0192' ]
        elif fold == 4:
           #self.foldTags = [ 'omux_0_to_127', 'imux_addr0128', 'imux_addr0240' ]
            self.foldTags = [ 'imux_addr0096', 'imux_addr0192', 'imux_addr0160' ]
        else:
            raise ErrorMessage( 1, 'SRAM_256x32.__init__(): Unsupported fold {}, valid values are 1, 2, 4.'.format( fold ))
        if not name:
            name = 'spram_256x32'
        self.cell    = af.createCell( name )
        self.mx2Cell = self.confLib.getStdCell( 'mx2_x2' )
        self.mx3Cell = self.confLib.getStdCell( 'mx3_x2' )
        self.na2Cell = self.confLib.getStdCell( 'na2_x1' )
        self.no2Cell = self.confLib.getStdCell( 'no2_x1' )
        with UpdateSession():
            self.buildInterface()
            self.decoder = ColBlock( self, 'decod', 33 )
            for addr in range(256):
                bitGroup = ColGroup( SRAM_256x32.BIT_GROUP_FMT.format( addr ))
                self.rootGroup.group( bitGroup )
                bitGroup.group( Column( self
                                      , self.mx2Cell
                                      , 'imux_addr{:04d}'.format( addr )
                                      , '_byte{byte}_{bbit}'
                                      , 32 ))
                bitGroup.group( Column( self
                                      , self.dffCell
                                      , 'bit_addr{:04d}'.format( addr )
                                      , '_byte{byte}_{bbit}'
                                      , 32 ))
                bus = Bus( self, 'imux_addr{:04d}_q({{}})'.format(addr), 32 )
                bitGroup.childs[0].setBusNet( 'q', bus )
                bitGroup.childs[1].setBusNet( 'i', bus )
                bus = Bus( self, 'bit_addr{:04d}_q({{}})'.format(addr), 32 )
                bitGroup.childs[0].setBusNet( 'i0', bus )
                bitGroup.childs[1].setBusNet(  'q', bus )
                bus = Bus( self, 'di({})', 32 )
                bitGroup.childs[0].setBusNet( 'i1', bus )
                bitGroup.childs[1].setCmdNet( 'ck', self.getNet( 'clk' ))
            self.buildDecoder()
           #self._buildOutputMux_mx23()
            self._buildOutputMux_nao23()
           #inst = self.addInstance( 'inv_x2'
           #                        , 'nrst_inv'
           #                        , { 'i'  : 'rst'
           #                          , 'nq' : 'n_rst'
           #                          }
           #                       )
           #self.decoder.addInstance( 0, inst )
            inst = self.addInstance( 'inv_x2'
                                    , 'nce_inv'
                                    , { 'i'  : 'ce'
                                      , 'nq' : 'n_ce'
                                      }
                                   )
            self.decoder.addInstance( 0, inst )
            for child in self.rootGroup.childs[0].childs:
                if child.kind == Column.KIND_COLUMN:
                    if child.insts[0].getMasterCell() == self.mx3Cell:
                        rstCol = Column( self
                                       , af.getCell( 'a2_x2', CRL.Catalog.State.Views )
                                       , 'omux_n_rst'
                                       , '_byte{byte}_{bbit}'
                                       , 32 )
                        busOMux = Bus( self, child.tag+'_q({})', 32 )
                        busDato = Bus( self, 'dato({})', 32 )
                        child .setBusNet( 'q' , busOMux )
                        rstCol.setBusNet( 'i0', busOMux )
                        rstCol.setCmdNet( 'i1', self.getNet('n_rst') )
                        rstCol.setBusNet( 'q' , busDato )
                        self.rootGroup.group( rstCol )
                        break
            omuxRoot = self.rootGroup.findChild( 'omux_0_to_255' )
            rstCol = Column( self
                           , self.no2Cell
                           , 'omux_rst'
                           , '_byte{byte}_{bbit}'
                           , 32 )
            busOMux = Bus( self, 'omux_0_to_255_nq({})', 32 )
            busDato = Bus( self, 'dato({})', 32 )
            omuxRoot.setBusNet( 'nq', busOMux )
            rstCol.setBusNet( 'i0', busOMux )
            rstCol.setCmdNet( 'i1', self.getNet('rst') )
            rstCol.setBusNet( 'nq', busDato )
            omuxRoot.parent.group( rstCol, after=omuxRoot )
            af.saveCell( self.cell, CRL.Catalog.State.Logical )
            self.cell.updatePlacedFlag()

    def _buildOutputMux_nao23 ( self ):
        """
        Build the complete output mux based on successive layers of NAND2
        then NOR2. More compact than the mux based version.

        Use an "askew" tree to minimize wiring.
        """
        muxDepth = 0
        levels   = [ [], ]
        for addr in range(256):
            oneHotName = 'rdecod_' + self._getDecodNetName( addr, 8 ).replace('_n_','_')
            tag        = SRAM_256x32.BIT_GROUP_FMT.format( addr )
            bitGroup   = self.rootGroup.findChild(tag)
            bitGroup.unGroup()
            tag = 'sel_' + tag[:-2]
            nand2Col  = Column( self
                              , self.na2Cell
                              , tag
                              , '_byte{byte}_{bbit}'
                              , 32 )
            nand2Col.setCmdNet( 'i0', self.getNet( oneHotName ))
            busDff = self.getBus( 'bit_addr{:04d}_q({{}})'.format(addr) )
            nand2Col.setBusNet( 'i1', busDff )
            bitGroup.group( nand2Col )
            levels[0].append( (bitGroup, nand2Col ) )
        while len(levels[muxDepth]) > 1:
            levels.append( [] )
            childIndex = 1 if muxDepth else 2
            for i in range(len( levels[muxDepth]) // 2 ):
                naoCell     = self.no2Cell if muxDepth%2 else self.na2Cell
                childs      = [ levels[muxDepth][i*2][0], levels[muxDepth][i*2+1][0] ]
                leftRoot    = levels[muxDepth][i*2    ][1]
                rightRoot   = levels[muxDepth][i*2 + 1][1]
                tags        = [ childs[0].tag, childs[1].tag ]
                naoTag      = SRAM_256x32._mergeOMuxTags( tags )
                naoGroup    = ColGroup( naoTag+'_g' )
                trace( 610, ',+', '\tSRAM_256x32._buildOutputmux() {} + {} -> {}\n' \
                                  .format( tags[0], tags[1], naoTag ))
                nao2Col  = Column( self
                                 , naoCell
                                 , naoTag
                                 , '_byte{byte}_{bbit}'
                                 , 32 )
                naoGroup.group( childs[0] )
                naoGroup.group( childs[1] )
                bus0   = Bus( self, tags[0][:-2]+'_nq({})', 32 )
                bus1   = Bus( self, tags[1][:-2]+'_nq({})', 32 )
                busNao = Bus( self, naoTag+'_nq({})', 32 )
                leftRoot .setBusNet( 'nq', bus0 )
                rightRoot.setBusNet( 'nq', bus1 )
                nao2Col.setBusNet( 'i0', bus0 )
                nao2Col.setBusNet( 'i1', bus1 )
                childs[1].reverse()
                rightRoot.parent.group( nao2Col, before=rightRoot )
                trace( 610, '\tInsert mux {} before {}\n'.format( nao2Col, rightRoot.parent ))
                levels[muxDepth+1].append( (naoGroup, nao2Col) )
                trace( 610, '-,' )
            muxDepth += 1
        self.rootGroup.group( levels[muxDepth][0][0] )

    def _buildOutputMux_mx23 ( self ):
        """
        Build the complete output mux based on successive layers of mux4,
        each mux4 beeing built upon a mux2 + mux3 (_doMux4_mux23).
        """
        omuxGroupsCurr = []
        omuxGroupsNext = []
        muxDepth       = 0
        for i in range(256//4):
            childs = []
            for addr in range(i*4, (i+1)*4):
                tag = SRAM_256x32.BIT_GROUP_FMT.format( addr )
                childs.append( self.rootGroup.findChild( tag ))
                childs[-1].unGroup()
            omuxGroupsCurr.append( self._doMux4_mx23( childs, muxDepth ))
        while len(omuxGroupsCurr) >= 4:
            trace( 610, '\tGrouping {} elements.\n'.format( len(omuxGroupsCurr )))
            muxDepth += 1
            for i in range(len(omuxGroupsCurr)//4):
                omuxGroupsNext.append( self._doMux4_mx23( omuxGroupsCurr[i*4:(i+1)*4], muxDepth ))
            omuxGroupsCurr = omuxGroupsNext
            omuxGroupsNext = []
        for group in omuxGroupsCurr:
            self.rootGroup.group( group )

    def _doMux4_mx23 ( self, childs, muxDepth ):
        """
        Build a 4 entry mux. It uses a mux2 / mux3 combination.
        Returns a newly build group.
        Entry selection given (cmd0,cmd1) : :: 

            00 ==> i0 (mux2.i0)
            01 ==> i1 (mux2.i1)
            10 ==> i2 (mux3.i2)
            11 ==> i3 (mux3.i1)
        """
        tags = []
        for child in childs:
            tags.append( child.tag )
        childIndex = 1 if muxDepth == 0 else 4
        muxTag     = SRAM_256x32._mergeOMuxTags( tags )
        mux2Tag    = SRAM_256x32._mergeOMuxTags( tags[0:2] )
        mux3Tag    = SRAM_256x32._mergeOMuxTags( tags )
        muxGroup   = ColGroup( muxTag+'_g' )
        trace( 610, ',+', '\tSRAM_256x32._doMux4_mx23() {} + {} -> {}\n' \
                          .format( mux2Tag, mux3Tag, muxTag ))
        mux2Col  = Column( self
                         , self.mx2Cell
                         , mux2Tag
                         , '_byte{byte}_{bbit}'
                         , 32 )
        mux2Col.setCmdNet( 'cmd', self.getNet( 'raddr({})'.format(muxDepth*2) ))
        mux3Col  = Column( self
                         , self.mx3Cell
                         , mux3Tag
                         , '_byte{byte}_{bbit}'
                         , 32 )
        mux3Col.setCmdNet( 'cmd0', self.getNet( 'raddr({})'.format(muxDepth*2 + 1) ))
        mux3Col.setCmdNet( 'cmd1', self.getNet( 'raddr({})'.format(muxDepth*2    ) ))
        muxGroup.group( childs[0] )
        muxGroup.group( mux2Col )
        muxGroup.group( childs[1] )
        muxGroup.group( childs[2] )
        muxGroup.group( mux3Col )
        muxGroup.group( childs[3] )
        bus0   = Bus( self, tags[0][:-2]+'_q({})', 32 )
        bus1   = Bus( self, tags[1][:-2]+'_q({})', 32 )
        bus2   = Bus( self, tags[2][:-2]+'_q({})', 32 )
        bus3   = Bus( self, tags[3][:-2]+'_q({})', 32 )
        busMx2 = Bus( self, mux2Tag+'_q({})', 32 )
        childs[0].childs[ childIndex ].setBusNet( 'q', bus0 )
        childs[1].childs[ childIndex ].setBusNet( 'q', bus1 )
        childs[2].childs[ childIndex ].setBusNet( 'q', bus2 )
        childs[3].childs[ childIndex ].setBusNet( 'q', bus3 )
        mux2Col.setBusNet( 'i0', bus0 )
        mux2Col.setBusNet( 'i1', bus1 )
        mux2Col.setBusNet( 'q' , busMx2 )
        mux3Col.setBusNet( 'i0', busMx2 )
        mux3Col.setBusNet( 'i2', bus2 )
        mux3Col.setBusNet( 'i1', bus3 )
        childs[1].reverse()
        childs[3].reverse()
        trace( 610, '-,' )
        return muxGroup

    @staticmethod
    def _mergeOMuxTags ( tags ):
        """
        Merge two output mux column tags. We assume that we merge only
        contiguous tags.

        Example: ::

            'omux_0_to_1' + 'omux_2_to_3' ==> 'omux_0_to_3'
        """
        vectorRe = re.compile( '^omux_(?P<lsb>\d+)_to_(?P<msb>\d+)' )
        addrs    = []
        for tag in tags:
            end = -2 if tag.endswith('_g') else 0
            if tag.startswith('bit_addr'):
                addrs.append( int( tag[8:end] ))
            if tag.startswith('sel_bit_addr'):
                addrs.append( int( tag[12:end] ))
            elif tag.startswith('omux'):
                m = vectorRe.match( tag )
                addrs += [ int(m.group('lsb')), int(m.group('msb')) ]
        addrs.sort()
        omuxTag = 'omux'
        omuxTag = 'omux_{}_to_{}'.format( addrs[0], addrs[-1] )
        return omuxTag

    def buildInterface ( self ):
        """ Build the interface of the SRAM. """
        self.addExternalNet( 'clk', Net.Direction.DirIn, Net.Type.CLOCK ) 
        self.addExternalNet( 'rst', Net.Direction.DirIn ) 
        self.addExternalNet( 'ce' , Net.Direction.DirIn ) 
        for bit in range(4):
            self.addExternalNet(   'we({})'.format(bit) , Net.Direction.DirIn ) 
        self.addExternalNet( 'oe' , Net.Direction.DirIn ) 
        for bit in range(8):
            self.addExternalNet( 'addr({})'.format(bit) , Net.Direction.DirIn ) 
        for bit in range(32):
            self.addExternalNet(   'di({})'.format(bit) , Net.Direction.DirIn ) 
        for bit in range(32):
            self.addExternalNet( 'dato({})'.format(bit) , Net.Direction.DirOut ) 
        self.addExternalNet( 'vdd' , Net.Direction.DirIn, Net.Type.POWER  ) 
        self.addExternalNet( 'vss' , Net.Direction.DirIn, Net.Type.GROUND ) 

    def _getDecodNetName ( self, oneHot, addrWidth ):
        """
        Build a net name for a particular oneHot bit in the range covered by addrWidth.
        The first part is the address lines and the second the value they decod.
        If the oneHot value exceed 2^addrWidth, we uses the *next* address lines.

        ========  ===========  ========================
        oneHot    addrWidth    net name
        ========  ===========  ========================
        0         4            'decod_3_2_1_0_0000'
        1         4            'decod_3_2_1_0_0001'
        2         4            'decod_3_2_1_0_0010'
        3         4            'decod_3_2_1_0_0011'
        4         4            'decod_3_2_1_0_0100'
        15        4            'decod_3_2_1_0_1111'
        16        4            'decod_7_6_5_4_0000'
        17        4            'decod_7_6_5_4_0001'
        ========  ===========  ========================
        """
        netName = ''
        indexFirstBit = (oneHot >> addrWidth) * addrWidth
        for bit in range(indexFirstBit, indexFirstBit + addrWidth):
            netName = '{}_'.format(str( bit )) + netName
        divider = 1 << addrWidth
        netName = '{}{:0{width}b}'.format( netName, oneHot % divider, width=addrWidth )
        return netName

    def _getDecodInstConf ( self, oneHot, addrWidth ):
        """
        Compute the informations needed to instanciate one cell of one level of
        the decoder. For the first level of one hot (addrWidth == 2), the inputs
        are just direct or inverted addresses bits. For the upper level we
        combine the outputs of the previous one hot level, that is the one with
        addrWidth/2 to generate the current one.
        """
        instConf = []
        if addrWidth == 2:
            indexFirstBit = (oneHot >> addrWidth) * addrWidth
            valueAddr     =  oneHot % (1 << addrWidth)
            trunkName     = 'n_'+self._getDecodNetName( oneHot, addrWidth )
            instConf.append( self.confLib.getStdCellName('na2_x1') )
            instConf.append( 'decod_nand2_{}'.format( trunkName ))
            instConf.append( {} )
            for i in range(2):
                inv  = '' if (valueAddr & (1 << i)) else 'n_'
                instConf[2][ 'i{}'.format(i) ] = '{}addr({})'.format( inv, indexFirstBit+i )
            instConf[2][ 'nq' ] = 'decod_'+trunkName
        elif addrWidth == 4 or addrWidth == 8:
            halfWidth = addrWidth>>1
            halfMask  = 0
            for i in range(halfWidth):
                halfMask |= 1 << i
            indexFirstBit = (oneHot >> addrWidth) * addrWidth
            valueAddr     =  oneHot % (1 << addrWidth)
            trunkName     = self._getDecodNetName( oneHot, addrWidth )
            gate          = 'no2_x1' if addrWidth == 4 else 'na2_x1'
            instConf.append( self.confLib.getStdCellName(gate) )
            instConf.append( 'decod_{}_{}'.format( gate[:-3], trunkName ))
            instConf.append( {} )
            offset  = (oneHot >> addrWidth) << (halfWidth+1)
            oneHot0 = (oneHot & halfMask) + offset
            instConf[2][ 'i0' ] = 'decod_n_'+self._getDecodNetName( oneHot0, halfWidth )
            oneHot1 = ((oneHot >> halfWidth) & halfMask) + (1<<(halfWidth)) + offset
            instConf[2][ 'i1' ] = 'decod_n_'+self._getDecodNetName( oneHot1, halfWidth )
            instConf[2][ 'nq' ] = 'decod_n_'+trunkName
            trace( 610, '\t{:08b} {:3d}:{} + {:3d}:{} => {:3d}::{:08b}:{}\n' \
                        .format( halfMask
                               , oneHot0, self._getDecodNetName( oneHot0, halfWidth )
                               , oneHot1, self._getDecodNetName( oneHot1, halfWidth )
                               , oneHot , oneHot, trunkName ))
        return instConf

    def buildDecoder ( self ):
        trace( 610, ',+', '\tSRAM_256x32.buildDecoder()\n' )
        for bit in range(8):
            inst = self.addInstance( 'mx2_x2'
                                   , 'raddr_imux_{}'.format(bit)
                                   , { 'cmd' : 'ce'
                                     , 'i0'  :        'raddr({})'.format(bit)
                                     , 'i1'  :         'addr({})'.format(bit)
                                     , 'q'   : 'raddr_imux_q({})'.format(bit)
                                     }
                                   )
            self.decoder.addInstance( bit * 4 + 1, inst )
            inst = self.addInstance( 'sff1_x4'
                                   , 'raddr_sff_{}'.format(bit)
                                   , { 'i' : 'raddr_imux_q({})'.format(bit)
                                     , 'q' :        'raddr({})'.format(bit)
                                     }
                                   )
            self.decoder.addInstance( bit * 4, inst )
            self.connect( 'raddr_sff_{}'.format(bit), 'ck', 'clk' )
        for we in range(4):
            inst = self.addInstance( 'inv_x1'
                                   , 'decod_n_we_{}'.format(we)
                                   , {  'i' :   'we({})'.format(we)
                                     , 'nq' : 'n_we({})'.format(we)
                                     }
                                   )
            self.decoder.addInstance( we*4 + 1, inst )
        for bit in range(8):
            inst = self.addInstance( 'inv_x1'
                                   , 'decod_inv_{}'.format(bit)
                                   , {  'i' :   'addr({})'.format(bit)
                                     , 'nq' : 'n_addr({})'.format(bit)
                                     }
                                   )
            self.decoder.addInstance( bit*4 + 1, inst )
        for oneHot in range(16):
            trace( 610, '\t{}\n'.format( self._getDecodNetName(oneHot,2) ))
            instDatas = self._getDecodInstConf( oneHot, 2 )
            print( instDatas )
            inst = self.addInstance( instDatas[0], instDatas[1], instDatas[2] )
            self.decoder.addInstance( oneHot*2 + 1, inst )
        for oneHot in range(32):
            instDatas = self._getDecodInstConf( oneHot, 4 )
            inst = self.addInstance( instDatas[0], instDatas[1], instDatas[2] )
            self.decoder.addInstance( oneHot + (oneHot+1)%2, inst )
        for oneHot in range(256):
            bitTag    = 'bit_addr{:04d}'.format( oneHot )
            imuxTag   = 'imux_addr{:04d}'.format( oneHot )
            instDatas = self._getDecodInstConf( oneHot, 8 )
            inst      = self.addInstance( instDatas[0], instDatas[1], instDatas[2] )
            dffCol    = self.rootGroup.findChild( bitTag )
            imuxCol   = self.rootGroup.findChild( imuxTag )
            self.toHeaders.append(( inst, imuxCol.insts[0], 0 ))
            for we in range(4):
                cmdNetName = 'decod_addr{:04d}_we({})'.format( oneHot, we )
                inst = self.addInstance( self.confLib.getStdCellName('no3_x1')
                                       , 'decod_no3_we_{}_{}'.format(we,oneHot)
                                       , { 'i0' : instDatas[2]['nq']
                                         , 'i1' : 'n_ce'
                                         , 'i2' : 'n_we({})'.format(we)
                                         , 'nq' : cmdNetName
                                         }
                                       )
                self.toHeaders.append(( inst, imuxCol.insts[0], 0 ))
                for bit in range(8):
                    self.connect( 'imux_addr{:04d}_byte{byte}_{bbit}'.format( oneHot, byte=we, bbit=bit )
                                , 'cmd'
                                , cmdNetName
                                )
            oneHotName = instDatas[2]['nq'].replace('_n_','_')
            inst = self.addInstance( 'inv_x1'
                                   , 'omux_onehot_inv_{:04d}'.format(oneHot)
                                   , {  'i' : instDatas[2]['nq']
                                     , 'nq' : oneHotName
                                     }
                                   )
            self.toHeaders.append(( inst, imuxCol.insts[0], 0))
            sffName = 'omux_onehot_dff_{:04d}'.format(oneHot)
            inst = self.addInstance( 'sff1_x4'
                                   , sffName
                                   , { 'i' :     oneHotName
                                     , 'q' : 'r'+oneHotName
                                     }
                                   )
            self.connect( sffName, 'ck', 'clk' )
            self.toHeaders.append(( inst, imuxCol.insts[0], 1 ))
        trace( 610, '-,' )

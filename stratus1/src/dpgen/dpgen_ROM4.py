
# This file is part of the Coriolis Project.
# Copyright (C) Laboratoire LIP6 - Departement ASIM
# Universite Pierre et Marie Curie
#
# Main contributors :
#        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
#        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
#        Hugo Clement                   <Hugo.Clement@lip6.fr>
#        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
#        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
#        Christian Masson           <Christian.Masson@lip6.fr>
#        Marek Sroka                     <Marek.Sroka@lip6.fr>
# 
# The  Coriolis Project  is  free software;  you  can redistribute  it
# and/or modify it under the  terms of the GNU General Public License
# as published by  the Free Software Foundation; either  version 2 of
# the License, or (at your option) any later version.
# 
# The  Coriolis Project is  distributed in  the hope  that it  will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
# GNU General Public License for more details.
# 
# You should have  received a copy of the  GNU General Public License
# along with the Coriolis Project;  if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA
#
# License-Tag
# Authors-Tag
# ===================================================================
#
# x-----------------------------------------------------------------x 
# |                                                                 |
# |                   C O R I O L I S                               |
# |    S t r a t u s   -  Netlists/Layouts Description              |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./dpgen_ROM4.py"                          |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus  import *

class DpgenRom4 ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :

    self._nbit = self._param['nbit']
    
    self._sel0 = SignalIn  ( "sel0", 1 )
    self._sel1 = SignalIn  ( "sel1", 1 )
    
    self._q    = SignalOut (    "q", self._nbit )
    
    self._vdd  = VddIn ( "vdd" )
    self._vss  = VssIn ( "vss" )
    
  ## Netlist ##
  #############
  def Netlist ( self ) :
    global LV_xl0, LV_xl1, LV_xl2, LV_xl3, LV_iLeafCell

    # Parses the constants
    LV_xl0 = newxl ()
    LV_xl0.atoxl ( self._param['val0'] )
    LV_xl0._size = self._nbit
  
    LV_xl1 = newxl ()
    LV_xl1.atoxl ( self._param['val1'] )
    LV_xl1._size = self._nbit

    LV_xl2 = newxl ()
    LV_xl2.atoxl ( self._param['val2'] )
    LV_xl2._size = self._nbit
  
    LV_xl3 = newxl ()
    LV_xl3.atoxl ( self._param['val3'] )
    LV_xl3._size = self._nbit

    
    LV_iLeafCell = CELLS_ROM2
    

    # Signal
    i0x  = Signal (  "i0x", 1 )
    ni0x = Signal ( "ni0x", 1 )
    i1x  = Signal (  "i1x", 1 )
    ni1x = Signal ( "ni1x", 1 )

    sI0  = Signal (  "si0" )
    sI1  = Signal (  "si1" )

    
    # Loop for all the data slices
    self.Cell = {}
    
    for iSlice in range ( self._nbit ) :     
      iCell     = get_index_Cell ( iSlice, DPGEN_ROM4, LV_xl0, LV_xl1, LV_xl2, LV_xl3 )
      sModel    = get_model_Cell ( iSlice, DPGEN_ROM4, iCell,  LV_iLeafCell )
      
      sI0 = i0x
      sI1 = i1x
  
      # Select the inputs
      if   iCell == 8 or iCell == 9 or iCell == 10 or iCell == 14 :
        sI0 = i0x
        sI1 = i1x
      elif iCell == 2 or iCell == 11 :
        sI0 = i0x
        sI1 = ni1x
      elif iCell == 4 or iCell == 13 :
        sI0 = ni0x
        sI1 = i1x
      elif iCell == 1 or iCell == 5 or iCell == 6 or iCell == 7 :
        sI0 = ni0x
        sI1 = ni1x
      elif iCell == 3 :
        sI0 = ni1x
      elif iCell == 12 :
        sI0 = i1x
  
      # Select the gate
      if sModel[0:4] == "zero" :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'nq'  : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif sModel[0:3] == "one" :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'q'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif sModel[0:3] == "buf" :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : sI0
                                         , 'q'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif sModel[0:3] == "inv" :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'    : sI0
                                         , 'nq'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif sModel[0:2] in ( "a2", "o2" ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : sI0
                                         , 'i1'  : sI1
                                         , 'q'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
            
      elif sModel[0:11] == "dp_rom4_xr2" or sModel[0:12] == "dp_rom4_nxr2" :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0x'  :  i0x
                                         , 'i1x'  :  i1x
                                         , 'ni0x' : ni0x
                                         , 'ni1x' : ni1x
                                         , 'q'    : self._q[iSlice]
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
    

    # Add the buffer
    if iSlice % 2 : iSlice += 1
  
    sModel    = get_model_Buffer ( DPGEN_ROM4, LV_iLeafCell )
  
    self.Cell[iSlice] = Inst ( sModel
                             , "cell_%ld" % iSlice
                             , map = { 'i0'   : self._sel0
                                     , 'i1'   : self._sel1
                                     , 'i0x'  : i0x
                                     , 'i1x'  : i1x
                                     , 'ni0x' : ni0x
                                     , 'ni1x' : ni1x
                                     , 'vdd'  : self._vdd
                                     , 'vss'  : self._vss
                                     }
                             )


  ## Layout ##
  ############
  def Layout (self ) :

    # Loop for all the data slices
    for iSlice in range ( self._nbit ) :
      Place ( self.Cell[iSlice]
            , slice_to_sym ( iSlice )
            , XY ( 0, slice_to_y ( iSlice ) )
            )
  
    # Add the buffer cell
    if iSlice % 2 : iSlice += 1
  
    Place ( self.Cell[iSlice]
          , slice_to_sym ( iSlice )
          , XY ( 0, slice_to_y ( iSlice ) )
          )
  
  
  ## VBE ##
  #########
  def Vbe ( self ) :
    global LV_xl0
    global LV_xl1
    global LV_xl2
    global LV_xl3

    flags = self._param['flags']
      
    fileName = self._name + ".vbe"
    
    file = open ( fileName, "w+" )
  
    sBusWide = VHDL_BUS_WIDE ( self._nbit - 1, 0, self._nbit, flag )
  
    ## Entity description ##
    file.write ( "\nENTITY %s IS\n  PORT (\n" % self._name )
  
    ## Controls Terminals ##
    file.write ( "%14s : in    BIT;\n" % "sel1" )
     
    file.write ( "%14s : in    BIT;\n" % "sel0" )
  
    ## Output data bus ##
    file.write ( "%14s :   out BIT_VECTOR %s;\n" % ( "q",  sBusWide ) )
  
    ## Power supplies terminals ##
    file.write ( "%14s : in    BIT;\n" % "vdd" )
    file.write ( "%14s : in    BIT\n"  % "vss" )
  
    ## End of entity description ##
    file.write ( "  );\nEND %s;\n\n\n" % self._name )
  
    ## Architecture description ##
    file.write ( "ARCHITECTURE VBE OF %s IS\n\n" % self._name )
  
    file.write ( "BEGIN\n\n")
  
    ## Behavior ##
    file.write ( "  WITH  sel1 & sel0  SELECT\n" )
    file.write ( "    q <= %s WHEN B\"00\",\n"   % LV_xl0.xltovhdl () )
    file.write ( "         %s WHEN B\"01\",\n"   % LV_xl1.xltovhdl () )
    file.write ( "         %s WHEN B\"10\",\n"   % LV_xl2.xltovhdl () )
    file.write ( "         %s WHEN B\"11\";\n\n" % LV_xl3.xltovhdl () )
  
    ## Assert ##
    file.write ( "  ASSERT (vdd = '1')\n" )
    file.write ( "    REPORT \"Power supply is missing on vdd of Model %s.\"\n" % self._name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    file.write ( "  ASSERT (vss = '0')\n" )
    file.write ( "    REPORT \"Power supply is missing on vss of Model %s.\"\n" % self._name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    ## End of architecture description ##
    file.write ( "END VBE;\n" )
  
    file.close ()

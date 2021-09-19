
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
# |  Py Module   :       "./dpgen_ROM2.py"                          |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus  import *

class DpgenRom2 ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :

    self._nbit = self._param['nbit']
    
    self._sel0 = SignalIn  ( "sel0", 1 )
    self._q    = SignalOut (    "q", self._nbit )
    
    
    self._vdd  = VddIn ( "vdd" )
    self._vss  = VssIn ( "vss" )


  ## Netlist ##
  ##################
  def Netlist ( self ) :
    global LV_xl0, LV_xl1, LV_iLeafCell

    val  = []
    val.append ( self._param['val0'] )
    val.append ( self._param['val1'] )
  
    # Parses the constants
    LV_xl0 = newxl ()
    LV_xl0.atoxl ( val[0] )
    LV_xl0._size = self._nbit
  
    LV_xl1 = newxl ()
    LV_xl1.atoxl ( val[1] )
    LV_xl1._size = self._nbit
    
    LV_iLeafCell = 0

    # Signal
    ni0x = Signal ( "ni0x", 1 )

  
    # Loop for all the data slices
    self.Cell = {}
    
    for iSlice in range ( self._nbit ) :
      iCell     = get_index_Cell ( iSlice, DPGEN_ROM2, LV_xl0, LV_xl1      )
      sModel    = get_model_Cell ( iSlice, DPGEN_ROM2, iCell, LV_iLeafCell )
      
      if iCell == 0 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'nq'  : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif iCell == 3 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'q'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif iCell == 1 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : ni0x
                                         , 'q'   : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif iCell == 2 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : ni0x
                                         , 'nq'  : self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
    # Add the buffer
    if iSlice % 2 : iSlice += 1
  
    sModel = get_model_Buffer ( DPGEN_ROM2, LV_iLeafCell )
 
    self.Cell[iSlice] = Inst ( sModel
                             , "cell_%ld" % iSlice
                             , map = { 'i'   : self._sel0
                                     , 'nix' : ni0x
                                     , 'vdd' : self._vdd
                                     , 'vss' : self._vss
                                     }
                             )
  

  ## Layout ##
  ############
  def Layout ( self ) :

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


  ########## uROM_VHDL ##########
  ###############################
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
    file.write ( "  WITH  sel0  SELECT\n" )
    file.write ( "    q <= %s WHEN  \'0\',\n"   % LV_xl0.xltovhdl () )
    file.write ( "         %s WHEN  \'1\';\n\n" % LV_xl1.xltovhdl () )
  
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

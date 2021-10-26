
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
# |        S t r a t u s   -  Netlists/Layouts Description          |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./dpgen_Shifter.py"                       |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class DpgenShifter ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :
    global Slices
    global BitIndex

    self._nbit = self._param['nbit']

    Slices      = ln2p     ( self._nbit )

    self._op    = SignalIn  (    "op",          2 )
    self._shamt = SignalIn  ( "shamt",     Slices )
    
    self._i     = SignalIn  (     "i", self._nbit )

    self._o     = SignalOut (     "o", self._nbit )
    
    self.vdd    = VddIn    ( "vdd" )
    self.vss    = VssIn    ( "vss" )

    
  ## Netlist ##
  #############
  def Netlist ( self ) :
    global SliceIndex

    AndIndex  = 0
    
    # Signals
    MuxOutput = Signal ( "muxoutput", self._nbit * Slices + self._nbit )

    c0        = Signal (        "c0", Slices )
    c1        = Signal (        "c1", Slices )
    
    msb       = Signal (       "msb", 1 )
    
    
    # Arithmetic or input extension
    self.A0 = Inst ( "a2_x2"
                   , "a_0"
                   , map = { 'i0'  : self._i[self._nbit-1]
                           , 'i1'  : self._op[1]
                           , 'q'   : msb
                           , 'vdd' : self.vdd
                           , 'vss' : self.vss
                           }
                   )


    # Normalizing the input signals names
    for BitIndex in range ( self._nbit ) : self._i[BitIndex].Alias ( MuxOutput[BitIndex] )

    self.M = {}
   
    # Connecting the mux matrix
    for SliceIndex in range ( Slices ) :
      for BitIndex in range ( self._nbit ) :
          
        LeftIndex  = BitIndex - ( 1 << SliceIndex )
        RightIndex = BitIndex + ( 1 << SliceIndex )

        if LeftIndex >= 0 :
          if RightIndex < self._nbit : 
            self.M[SliceIndex*self._nbit+BitIndex] = Inst ( "mx3_x2"
                                                          , "m_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                          , map = { 'cmd0' : c0[SliceIndex]
                                                                  , 'cmd1' : c1[SliceIndex]
                                                                  , 'i0'   : MuxOutput[SliceIndex * self._nbit + LeftIndex]
                                                                  , 'i1'   : MuxOutput[SliceIndex * self._nbit + BitIndex]
                                                                  , 'i2'   : MuxOutput[SliceIndex * self._nbit + RightIndex]
                                                                  , 'q'    : MuxOutput[( SliceIndex + 1 ) * self._nbit + BitIndex]
                                                                  , 'vdd'  : self.vdd
                                                                  , 'vss'  : self.vss
                                                                  }
                                                          )
          else :
            self.M[SliceIndex*self._nbit+BitIndex] = Inst ( "mx3_x2"
                                                          , "m_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                          , map = { 'cmd0' : c0[SliceIndex]
                                                                  , 'cmd1' : c1[SliceIndex]
                                                                  , 'i0'   : MuxOutput[SliceIndex * self._nbit + LeftIndex]
                                                                  , 'i1'   : MuxOutput[SliceIndex * self._nbit + BitIndex]
                                                                  , 'i2'   : msb
                                                                  , 'q'    : MuxOutput[( SliceIndex + 1 ) * self._nbit + BitIndex]
                                                                  , 'vdd'  : self.vdd
                                                                  , 'vss'  : self.vss
                                                                  }
                                                          )
        else :
          if RightIndex < self._nbit : 
            self.M[SliceIndex*self._nbit+BitIndex] = Inst ( "mx3_x2"
                                                          , "m_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                          , map = { 'cmd0' : c0[SliceIndex]
                                                                  , 'cmd1' : c1[SliceIndex]
                                                                  , 'i0'   : self._op[0]
                                                                  , 'i1'   : MuxOutput[SliceIndex * self._nbit + BitIndex]
                                                                  , 'i2'   : MuxOutput[SliceIndex * self._nbit + RightIndex]
                                                                  , 'q'    : MuxOutput[( SliceIndex + 1 ) * self._nbit + BitIndex]
                                                                  , 'vdd'  : self.vdd
                                                                  , 'vss'  : self.vss
                                                                  }
                                                          )
          else :
            self.M[SliceIndex*self._nbit+BitIndex] = Inst ( "mx3_x2"
                                                          , "m_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                          , map = { 'cmd0' : c0[SliceIndex]
                                                                  , 'cmd1' : c1[SliceIndex]
                                                                  , 'i0'   : self._op[0]
                                                                  , 'i1'   : MuxOutput[SliceIndex * self._nbit + BitIndex]
                                                                  , 'i2'   : msb
                                                                  , 'q'    : MuxOutput[( SliceIndex + 1 ) * self._nbit + BitIndex]
                                                                  , 'vdd'  : self.vdd
                                                                  , 'vss'  : self.vss
                                                                  }
                                                          )          
        
    # Normalizing the output signals names
    for BitIndex in range ( self._nbit ) : self._o[BitIndex] <= MuxOutput[Slices * self._nbit + BitIndex]

    # Building the control signals
    if   self._nbit < 16 : Model = "inv_x2"
    elif self._nbit < 32 : Model = "inv_x4"
    else                 : Model = "inv_x8"

    self.I = {}
    self.O = {}
                                     
    for SliceIndex in range ( Slices ) :
      self.I[SliceIndex] = Inst  ( Model
                                 , "i_%d" % SliceIndex
                                 , map = { 'i'   : self._shamt[SliceIndex]
                                         , 'nq'  : c1[SliceIndex]
                                         , 'vdd' : self.vdd
                                         , 'vss' : self.vss
                                         }
                                 )
      self.O[SliceIndex] = Inst ( "o2_x2"
                                , "o_%d" % SliceIndex
                                , map = { 'i0'  : c1[SliceIndex]
                                        , 'i1'  : self._op[0]
                                        , 'q'   : c0[SliceIndex]
                                        , 'vdd' : self.vdd
                                        , 'vss' : self.vss
                                        }
                                )


  ########## Layout ##########
  ############################
  def Layout ( self ) :
    global SliceIndex
    global BitIndex

    Slices =  ln2p ( self._nbit )
  
    if self._nbit & 1 :
      sym = SYM_Y
      mys = NOSYM
    else :
      sym = NOSYM
      mys = SYM_Y
  
    BitIndex   = self._nbit - 1
    SliceIndex = 0
    
    Place ( self.M[SliceIndex * self._nbit + BitIndex]
          , SYM ( BitIndex, sym, mys )
          , XY ( 0, 0 )
          )
          
    for SliceIndex in range ( 1, Slices ) :
      PlaceRight ( self.M[SliceIndex * self._nbit + BitIndex]
                 , SYM ( BitIndex, sym, mys )
                 )
  
    for SliceIndex in range ( Slices ) :
      SetRefIns ( self.M[SliceIndex * self._nbit + self._nbit - 1] )
      
      for BitIndex in range ( self._nbit - 2, -1, -1 ) :
        PlaceBottom ( self.M[SliceIndex * self._nbit + BitIndex]
                    , SYM ( BitIndex, sym, mys )
                    )
  
  
    BitIndex   = self._nbit  # Upper line, this is necessary for the computation of the symetry 
    SliceIndex = 0
    
    SetRefIns ( self.M[SliceIndex * self._nbit + self._nbit - 1] )
    
    PlaceTop  ( self.A0
              , SYM ( BitIndex, sym, mys )
              )

    for SliceIndex in range ( Slices ) :
      if SliceIndex == 0 : SetRefIns ( self.A0 )
      
          
      PlaceRight ( self.I[SliceIndex]
                , SYM ( BitIndex, sym, mys )
                )
  
      PlaceRight ( self.O[SliceIndex]
                , SYM ( BitIndex, sym, mys )
                )

  ## VBE ##
  #########
  def Vbe ( self ) :
    print( "vbe of DpgenShifter not implemented" )

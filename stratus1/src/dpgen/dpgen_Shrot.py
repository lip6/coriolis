
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
# |  Py Module   :       "./dpgen_Shrot.py"                         |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class DpgenShrot ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :
    global Slices
    global AndIndex

    self._nbit = self._param['nbit']

    Slices   = ln2p ( self._nbit )

    self._op    = SignalIn  (    "op", 3 )
    self._shamt = SignalIn  ( "shamt", Slices )
    
    self._i     = SignalIn  (     "i", self._nbit )
    self._o     = SignalOut (     "o", self._nbit )
    
    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )

    
  ## Netlist ##
  #############
  def Netlist ( self ) :
    global Slices

    AndIndex  = 0

    # Signals
    MuxOutput = Signal ( "muxoutput", self._nbit * Slices + self._nbit )

    c0        = Signal (        "c0", Slices               )
    c1        = Signal (        "c1", Slices               )
    
    msb       = Signal (       "msb", 1                    )
    op2b      = Signal (      "op2b", 1                    )

    x         = Signal (         "x", Slices * self._nbit  )

    MuxInput0 = Signal ( "muxinput0" )
    MuxInput2 = Signal ( "muxinput2" )

    self.I   = {}
    self.A3  = {}
    self.A   = {}
    self.OA  = {}
    self.O   = {}
    self.M   = {}
    self.Tie = {}
    
    
    # Arithmetic or input extension
    self.I[Slices] = Inst ( "inv_x2"
                          , "i_%d" % Slices
                          , map = { 'i'   : self._op[2]
                                  , 'nq'  : op2b
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )
    
    self.A3[Slices] = Inst ( "a3_x2"
                           , "a3_%d" % Slices
                           , map = { 'i0'  : self._i[self._nbit-1]
                                   , 'i1'  : op2b
                                   , 'i2'  : self._op[1]
                                   , 'q'   : msb
                                   , 'vdd' : self._vdd
                                   , 'vss' : self._vss
                                   }
                           )

    # Normalizing the input signals names
    for BitIndex in range ( self._nbit ) : self._i[BitIndex].Alias ( MuxOutput[BitIndex] )

    # Connecting the mux matrix
    for SliceIndex in range ( Slices ) :
      for BitIndex in range ( self._nbit ) :
          
        LeftIndex  = BitIndex - ( 1 << SliceIndex )
        RightIndex = BitIndex + ( 1 << SliceIndex )
        
        if LeftIndex >= 0 :
#          MuxOutput[SliceIndex * self._nbit + LeftIndex].Alias ( MuxInput0 )
          MuxInput0 = MuxOutput[SliceIndex * self._nbit + LeftIndex]
        else :
#          x[AndIndex].Alias ( MuxInput0 )
          MuxInput0 = x[AndIndex]
          
          self.A[SliceIndex*self._nbit+BitIndex]= Inst ( "a2_x2"
                                                       , "a_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                       , map = { 'i0'  : self._op[2]
                                                               , 'i1'  : MuxOutput[SliceIndex * self._nbit + self._nbit + LeftIndex]
                                                               , 'q'   : x[AndIndex]
                                                               , 'vdd' : self._vdd
                                                               , 'vss' : self._vss
                                                               }
                                                       )
          
          AndIndex += 1
          
        if RightIndex < self._nbit:
#          MuxOutput[SliceIndex * self._nbit + RightIndex].Alias ( MuxInput2 )
          MuxInput2 = MuxOutput[SliceIndex * self._nbit + RightIndex]
        else :
#          x[AndIndex].Alias ( MuxInput2 )
          MuxInput2 = x[AndIndex]
          
          self.OA[SliceIndex*self._nbit+BitIndex] = Inst ( "oa22_x4"
                                                         , "oa_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                         , map = { 'i0'  : self._op[2]
                                                                 , 'i1'  : MuxOutput[SliceIndex * self._nbit + RightIndex - self._nbit]
                                                                 , 'i2'  : msb
                                                                 , 'q'   : x[AndIndex]
                                                                 , 'vdd' : self._vdd
                                                                 , 'vss' : self._vss
                                                                 }
                                                         )
          
          AndIndex += 1
        
        self.M[SliceIndex*self._nbit+BitIndex] = Inst ( "mx3_x2"
                                                      , "m_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                      , map = { 'cmd0' : c0[SliceIndex]
                                                              , 'cmd1' : c1[SliceIndex]
                                                              , 'i0'   : MuxInput0
                                                              , 'i1'   : MuxOutput[SliceIndex * self._nbit + BitIndex]
                                                              , 'i2'   : MuxInput2
                                                              , 'q'    : MuxOutput[( SliceIndex + 1 ) * self._nbit + BitIndex]
                                                              , 'vdd'  : self._vdd
                                                              , 'vss'  : self._vss
                                                              }
                                                      )


    # Normalizing the output signals names
    for BitIndex in range ( self._nbit ) :
      MuxOutput[Slices * self._nbit + BitIndex]._temp = 1
      self._o[BitIndex] <= MuxOutput[Slices * self._nbit + BitIndex]

    # Building the control signals
    for SliceIndex in range ( Slices ) :
      self.I[SliceIndex] = Inst ( "inv_x2"
                                , "i_%d" % SliceIndex
                                , map = { 'i'   : self._shamt[SliceIndex]
                                        , 'nq'  : c1[SliceIndex]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
      
      self.O[SliceIndex]= Inst ( "o2_x2"
                               , "o_%d" % SliceIndex
                               , map = { 'i0' : c1[SliceIndex]
                                       , 'i1' : self._op[0]
                                       , 'q'  : c0[SliceIndex]
                                       , 'vdd' : self._vdd
                                       , 'vss' : self._vss
                                       }
                               )

    for SliceIndex in range ( Slices ) :
      for BitIndex in range ( self._nbit - 1, -1, -1 ) :
        l = 0
        r = 0          
        LeftIndex  = BitIndex - ( 1 << SliceIndex )
        RightIndex = BitIndex + ( 1 << SliceIndex )
        
        if RightIndex >= self._nbit : r = 1
        if LeftIndex  <     0 : l = 1
  
        if l == 0 and r == 0 :
          self.Tie[SliceIndex*self._nbit+BitIndex] = Inst ( "tie_x0"
                                                          , "tie_%d" % ( SliceIndex * self._nbit + BitIndex )
                                                          , map = { 'vdd' : self._vdd
                                                                  , 'vss' : self._vss
                                                                  }
                                                          )


  ########## Layout ##########
  ############################
  def Layout ( self ) :

    Slices =  ln2p ( self._nbit )
  
    if self._nbit & 1 :
      sym = SYM_Y
      mys = NOSYM
    else :
      sym = NOSYM
      mys = SYM_Y
  
    BitIndex   = self._nbit - 1
    SliceIndex = 0
    
    Place ( self.OA[SliceIndex * self._nbit + BitIndex]
          , SYM ( BitIndex, sym, mys )
          , XY ( 0, 0 )
          )
    PlaceRight ( self.M[SliceIndex * self._nbit + BitIndex]
               , SYM ( BitIndex, sym, mys )
               )
  
    for SliceIndex in range ( 1, Slices ) :
      PlaceRight ( self.OA[SliceIndex * self._nbit + BitIndex]
                 , SYM ( BitIndex, sym, mys )
                 )
  
      PlaceRight ( self.M[SliceIndex * self._nbit + BitIndex]
                 , SYM ( BitIndex, sym, mys )
                 )
  
    for SliceIndex in range ( Slices ) :
      SetXYIns ( self.OA[SliceIndex * self._nbit + self._nbit - 1] )
  
      for BitIndex in range ( self._nbit - 1, -1, -1 ) :
        l = 0
        r = 0          
        LeftIndex  = BitIndex - ( 1 << SliceIndex )
        RightIndex = BitIndex + ( 1 << SliceIndex )
        
        if RightIndex >= self._nbit :
          if BitIndex != ( self._nbit - 1 ) :
            PlaceBottom ( self.OA[SliceIndex * self._nbit + BitIndex]
                        , SYM ( BitIndex, sym, mys )
                        )
            
          r = 1
      
        if LeftIndex < 0 :
          if r == 1 :
            PlaceRight ( self.A[SliceIndex * self._nbit + BitIndex ]
                       , SYM ( BitIndex, sym, mys )
                       )
            SetXYIns ( self.OA[SliceIndex * self._nbit + BitIndex] )
            
          else :
            PlaceBottom ( self.A[SliceIndex * self._nbit + BitIndex]
                         , SYM ( BitIndex, sym, mys )
                         )
  
          l = 1
  
        if l == 0 and r == 0 :
          PlaceBottom ( self.Tie[SliceIndex * self._nbit + BitIndex]
                       , NOSYM
                       )
  
      SetRefIns ( self.M[SliceIndex * self._nbit + self._nbit - 1] )
      
      for BitIndex in range ( self._nbit - 2, -1, -1 ) :
        PlaceBottom ( self.M[SliceIndex * self._nbit + BitIndex]
                     , SYM ( BitIndex, sym, mys )
                     )
  
  
    BitIndex   = self._nbit  # Upper line, this is necessary for the computation of the symetry 
    SliceIndex = 0
    
    SetRefIns ( self.OA[SliceIndex * self._nbit + self._nbit - 1] )
    
    PlaceTop ( self.I[Slices]
             , SYM ( BitIndex, sym, mys )
             )
    PlaceRight ( self.A3[Slices]
               , SYM ( BitIndex, sym, mys )
               )
  
    for SliceIndex in range ( Slices ) :
      if SliceIndex > 0 :
        SetRefIns ( self.OA[SliceIndex * self._nbit + self._nbit - 1] )
      
      if SliceIndex == 0: 
        PlaceRight  ( self.I[SliceIndex]
                    , SYM ( BitIndex, sym, mys )
                    )
      else :
        PlaceTop ( self.I[SliceIndex]
                 , SYM ( BitIndex, sym, mys )
                 )
        
      PlaceRight ( self.O[SliceIndex]
                  , SYM ( BitIndex, sym, mys )
                  )

  ## VBE ##
  #########
  def Vbe ( self ) :
    print( "vbe of DpgenShrot not implemented" )

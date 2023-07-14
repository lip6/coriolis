
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
# |  Py Module   :       "./dpgen_Nul.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class DpgenNul ( Model ) :
    
  ## Interface ##
  ###############
  def Interface ( self ) :

    self._nbit = self._param['nbit']

    self._i0  = SignalIn  (  "i0", self._nbit )
    self._nul = SignalOut ( "nul", 1 )
  
    self._vdd = VddIn    ( "vdd" )
    self._vss = VssIn    ( "vss" )

 
  ## Netlist ##
  #############
  def Netlist ( self ) :
    global LV_prevTreeOut
    global LV_oneShift

    global nul
    
    LV_prevTreeOut = -1
    LV_oneShift    =  0
  
    offset    = 0
    depthNext = get_depth ( self._nbit, 0 )
  
    nul = []
    for i in range ( self._nbit ) :
      nul += [Signal ( "nul_%d" % i, 1 )]
  
    self.Nul = {}
    
    while ( 1 ) :
      depth     = depthNext
      depthNext = get_depth ( self._nbit, depth + 1 )

      offset = self.create_Netlist_Tree ( offset, depth, depthNext )

      if depthNext == 0 : break  # Simulation d'une boucle do ... while


      
  ########## Layout ##########
  ############################
  def Layout ( self ) :
    global LV_prevTreeOut
    global LV_oneShift

    mDepth = ilog2 ( self._nbit )
  
    LV_prevTreeOut = -1
    LV_oneShift    =  0
  
    offset         =  0
    depthNext      =  get_depth ( self._nbit, 0 )
 
    while ( 1 ) :
      depth     = depthNext
      depthNext = get_depth ( self._nbit, depth + 1 )
  
      offset    = self.create_Layout_Tree ( offset, depth, depthNext )
  
      if depthNext == 0 : break



  ########## VBE ##########
  #########################
  def Vbe ( self ) :
      
    flags = self._param['flags']
  
    fileName = self._name + ".vbe"
    
    file = open ( fileName, "w+" )
  
    sBusWide = VHDL_BUS_WIDE ( self._nbit - 1, 0, self._nbit, flags )
  
    ## Entity description ##
    file.write ( "\nENTITY %s IS\n  PORT (\n" % self._name )
  
    file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide ) )
    file.write ( "%14s : out   BIT;\n" % "nul" )
  
    file.write ( "%14s : in    BIT;\n" % "vdd" )
    file.write ( "%14s : in    BIT\n"  % "vss" )
  
    ## End of entity description ##
    file.write ( "  );\nEND %s;\n\n\n" % self._name )
  
    ## Architecture description ##
    file.write ( "ARCHITECTURE VBE OF %s IS\n\n" % self._name )
  
    file.write ( "BEGIN\n\n" )
  
    xl_zero       = newxl ()
    xl_zero._size = self._nbit
  
    file.write ( "    nul <= \'1\' WHEN (i0 = %s)\n" % xl_zero.xltovhdl () )
    file.write ( "               ELSE \'0\';\n\n\n" )
  
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
    

      

  ########## create_Netlist_Tree ##########
  #########################################
  def create_Netlist_Tree ( self, aOffset, amDepth, amDepthNext ) :
    global LV_prevTreeOut
    global LV_oneShift

    global nul
      
    if amDepthNext > 0 : polarity = amDepthNext % 2
    else               : polarity = amDepth     % 2
    
    if not ( amDepth ) :
      if not ( amDepthNext ) :
        # Pathological case : the zero detect is reduced to only one bit! This is only a inversor.
        self.Nul[aOffset] = Inst ( "inv_x4"
                                 , "nul_%ld" % aOffset
                                 , map = { 'i'   : self._i0[aOffset]
                                         , 'nq'  : self._nul
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
      else :
        LV_prevTreeOut = aOffset
  
        if ( amDepth % 2 ) == polarity :
          # This branch will skip an even number of layer, we keep the same polarity.
          self._i0[aOffset].Alias ( nul[aOffset] )
        else :
          # There is a polarity inversion.
          self.Nul[aOffset] = Inst ( "inv_x4"
                                   , "nul_%ld" % aOffset
                                   , map = { 'i'   : self._i0[aOffset]
                                           , 'nq'  : nul[aOffset]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
          
        LV_oneShift   = 1
        aOffset      += 1
  
      aOffset += 1
      return aOffset
  
  
    # Build the tree.
    for depth in range ( amDepth ) :
      mk = iexp2 ( amDepth - depth  - 1 )
  
      for k in range ( mk ) :
        if depth : i_offset = iexp2 ( depth - 1 ) - 1
        else     : i_offset = 0
            
        cell_index = iexp2 ( depth + 1 ) * k + iexp2 ( depth ) - 1        + aOffset
        
        i1_index   = iexp2 ( depth + 1 ) * k + iexp2 ( depth ) + i_offset + aOffset
        i0_index   = iexp2 ( depth + 1 ) * k                   + i_offset + aOffset
  
        if depth % 2 : sModel = "na2_x1"
        else         : sModel = "no2_x1"
  
        thisMap   = {}
  
        thisMap['nq'] = nul[cell_index]

        if depth :
          thisMap['i0'] = nul[i0_index]
          thisMap['i1'] = nul[i1_index]
        else :
          thisMap['i0'] = self._i0[i0_index - LV_oneShift]
          thisMap['i1'] = self._i0[i1_index - LV_oneShift]
  
        # Special case of the last layer.
        if mk == 1 and LV_prevTreeOut < 0 :
          if not ( amDepthNext ) :
            # This is the only tree in the operator.
            if depth % 2 :
              sModel = "a2_x4"
              
              thisMap['q'] = self._nul
              if 'nq' in thisMap : del thisMap['nq']
            else :
              sModel = "no2_x4"
              thisMap['nq'] = self._nul
  
          else :
            if depth % 2 == polarity :
              # Only for the first tree : adjust the polarity.
              if depth % 2 : sModel = "a2_x2"
              else         : sModel = "o2_x2"
  
              if 'nq' in thisMap : del thisMap['nq']
              thisMap['q'] = nul[cell_index]

        thisMap['vdd'] = self._vdd
        thisMap['vss'] = self._vss
 
        self.Nul[cell_index] = Inst ( sModel
                                    , "nul_%ld" % cell_index
                                    , map = thisMap
                                    )
  
    depth += 1 # Attention : difference entre les boucles en c et les boucles en python
  
    # Check if there is a ripple part.
    if LV_prevTreeOut < 0 :
      LV_prevTreeOut = cell_index
      
      return ( aOffset + iexp2 ( amDepth ) )
  
    # Add the ripple part.
    cell_index = aOffset - 1
  
    thisMap = {}
  
    if not ( amDepthNext ) :
      if depth % 2 :
        sModel = "a2_x4"
        thisMap['q'] = self._nul
      else :
        sModel = "no2_x4"
        thisMap['nq'] = self._nul
          
    else :            
      if depth % 2 : sModel = "na2_x1"
      else         : sModel = "no2_x1"
      
      if depth % 2 != polarity :
        thisMap['nq'] = nul[cell_index]
      else :
        thisMap['q'] = nul[cell_index]
  
      if depth % 2 == polarity :
        if depth % 2 : sModel = "a2_x2"
        else         : sModel = "o2_x2"
  
    i1_index  = iexp2 ( amDepth - 1 ) - 1 + aOffset
    i0_index  = LV_prevTreeOut
  
    thisMap['i0'] = nul[i0_index]
    thisMap['i1'] = nul[i1_index]
    
    thisMap['vdd'] = self._vdd
    thisMap['vss'] = self._vss
              
    self.Nul[aOffset-1] = Inst ( sModel
                               , "nul_%ld" % ( aOffset - 1 )
                               , map = thisMap
                               )
  
    LV_prevTreeOut = cell_index
    
    return ( aOffset + iexp2 ( amDepth ) )
  
  
  
  ########## create_Layout_Tree ##########
  ########################################
  def create_Layout_Tree ( self, aOffset, amDepth, amDepthNext ) :
    global LV_oneShift
    global LV_prevTreeOut
  
    if amDepthNext > 0 : polarity = amDepthNext % 2
    else               : polarity = amDepth     % 2
        
    if not ( amDepth ) :
      if not ( amDepthNext ) :
        # Cas pathologique : detection a un seul bit : c'est un inverseur
        dpgen_place ( self.Nul[aOffset], 0, aOffset * 50 )
      else :
        LV_prevTreeOut = aOffset
  
        if ( amDepth % 2 ) != polarity : dpgen_place ( self.Nul[aOffset], 0, aOffset * 50 )
  
        LV_oneShift  = 1
        aOffset     += 1
  
      aOffset += 1
  
      return aOffset
  
  
    # Build the tree.
    for depth in range ( amDepth ) :
      mk = iexp2 ( amDepth - depth  - 1 )
  
      for k in range ( mk ) :
        cell_index = iexp2 ( depth + 1 ) * k + iexp2 ( depth ) - 1 + aOffset
  
        if depth % 2 : sModel = "na2_x1"
        else         : sModel = "no2_x1"

        # Special case of the last layer.
        if mk == 1 and LV_prevTreeOut < 0 :
          if not ( amDepthNext ) :
            # This is the only tree in the operator.
            if depth % 2 : sModel = "a2_x4"
            else         : sModel = "no2_x4"
  
          else :
            if depth % 2 == polarity :
              # Only for the first tree : adjust the polarity.
              if depth % 2 : sModel = "a2_x2"
              else         : sModel = "o2_x2"
 
        dpgen_place ( self.Nul[cell_index], 0, cell_index * 50 )
  
    if LV_prevTreeOut < 0 :
      LV_prevTreeOut = cell_index
      
      return ( aOffset + iexp2 ( amDepth ) )

    
    if not ( amDepthNext ) :
      if depth % 2 : sModel = "a2_x4"
      else         : sModel = "no2_x4"
          
    else :            
      if depth % 2 : sModel = "na2_x1"
      else         : sModel = "no2_x1"
      
      if depth % 2 == polarity :
        if depth % 2 : sModel = "a2_x2"
        else         : sModel = "o2_x2"
  
    cell_index = aOffset - 1
  
    dpgen_place ( self.Nul[aOffset-1], 0, ( aOffset - 1 ) * 50 )
  
    LV_prevTreeOut = cell_index
  
    return ( aOffset + iexp2 ( amDepth ) )

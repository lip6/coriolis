
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
# |  Author      :                        Sophie BELLOEIL           |
# |  E-mail      :           Sophie.Belloeil@asim.lip6.fr           |
# | =============================================================== |
# |  Py Module   :       "./st_mult.py"                             |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

from st_mult_matrix       import Matrix, Matrix_Inv
from st_func_gen_wallace  import genWallace

class Multiplier ( Model ) :
    
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )

    if 'nbit' in param :
      self.nbit0 = param['nbit']
      self.nbit1 = param['nbit']
    else :
      self.nbit0 = param['nbit0']
      self.nbit1 = param['nbit1']

    if 'signed' in param :
       self._signed = param['signed']
    else :
       self._signed = True
    
    if self.nbit0 < 3 or self.nbit1 < 3 : raise SizeError( 'input arities must be greater than 2' )
    
    self.type = "nr"
    self.nType = 1
    
  def Interface ( self ) :
    self._i0 = SignalIn  ( "i0", self.nbit0 )
    self._i1 = SignalIn  ( "i1", self.nbit1 )
    
    if self._signed :
      self._o  = SignalOut ( "o", self.nbit0+self.nbit1 )
    else :
      self._o  = SignalOut ( "o", self.nbit0+self.nbit1-2 )

    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )

  def Netlist ( self ) :
    size_A = self.nbit0 + 2
    
    if self.nbit1 % 2 == 0 : size_B =   self.nbit1     // 2
    else                   : size_B = ( self.nbit1+1 ) // 2
      
    if self.nbit1 % 2 == 0 : parite = 1
    else                   : parite = 0
      
    # Signaux internes
    sig_nul  = Signal (  "sig_nul",          1 )
    opB      = Signal (      "opb", 2*size_B-1 )
    NUL      = Signal (      "nul",     size_B )
    DECA     = Signal (     "deca",     size_B )
    COMP     = Signal (     "comp",     size_B )
    
    # Matrice de Produits Partiels
    matrice_v = []
    for i in range ( size_B ) :
      matrice_v_bis = []
      for j in range ( size_A ) : matrice_v_bis += [Signal ( "matrice_%d_%d" % ( i, j ), 1 )]
      matrice_v += [matrice_v_bis]

    matrice      = []
    position     = []
    NEW_matrice  = []
    NEW_position = []
    PPmatrix     = []
        
    sig_nul <= Zero ( 1 )
    
    for i in range ( size_B ) :
      matrice.append  ( [] )
      position.append ( [] )
    
      # Premiere ligne
      myMap = { 'nul'  : NUL[i]
              , 'deca' : DECA[i]
              , 'comp' : COMP[i]
              , 'vdd'  : self._vdd
              , 'vss'  : self._vss
              }
    
      if i == 0 :
        myMap['bminus'] = sig_nul
        myMap['b']      = self._i1[i]
        myMap['bplus']  = self._i1[i+1]
      elif i == ( size_B - 1 ) :
        myMap['bminus'] = self._i1[(2*i)-1]
        myMap['b']      = self._i1[2*i]
        if parite : myMap['bplus'] = self._i1[(2*i)+1]
        else      : myMap['bplus'] = self._i1[2*i]
      else :
        myMap['bminus'] = self._i1[(2*i)-1]
        myMap['b']      = self._i1[2*i]
        myMap['bplus']  = self._i1[(2*i)+1]
      
      Generate ( 'st_mult_blocs.booth_decoder', 'booth_decoder' )
      Inst ( 'booth_decoder' , map = myMap )
      
      for j in range ( size_A ) :
        myMap = { 'nul'  : NUL[i]
                , 'deca' : DECA[i]
                , 'comp' : COMP[i]
                , 'pp'   : matrice_v[i][j]
                , 'vdd'  : self._vdd
                , 'vss'  : self._vss
                }

        if j == 0 :
          myMap['a'] = sig_nul
          myMap['b'] = self._i0[j]
        elif j > ( self._i0._arity - 1 ) :
          myMap['a'] = self._i0[self.nbit0-1]
          myMap['b'] = self._i0[self.nbit0-1]
        else :
          myMap['a'] = self._i0[j-1]
          myMap['b'] = self._i0[j]

        Generate ( 'st_mult_blocs.booth_mux', 'booth_mux' )
        Inst ( 'booth_mux', map = myMap )
        
        matrice[i]  = matrice[i] + [matrice_v[i][j]] 
        position[i] = position[i] + [1]
          
    # Adaptation de la matrice de produits partiels pour  
    MX           = Matrix     ( matrice, position, COMP, size_A, size_B )
    NEW_matrice  = Matrix_Inv ( MX[0],( size_B + 1 ) )
    NEW_position = Matrix_Inv ( MX[1],( size_B + 1 ) )
    
    # WALLACE
    PPmatrix = genWallace ( self, NEW_matrice, NEW_position )

    # Conversion pour que la sortie soit en non redondant
    #temp_out  = Signal ( "tempout", PPmatrix[0] )
    #temp_out <= PPmatrix[1] + PPmatrix[2]
    #self._o  <= temp_out[self._o._arity-1:0]
    self._o <= PPmatrix[1][self._o._arity-1:0] + PPmatrix[2][self._o._arity-1:0]

  def GetModelName ( cls, param ) :
    modelName = "multca2_"
    
    if   'nbit'  in param :
      modelName += str(param['nbit'])
    elif 'nbit1' in param :
      if param['nbit0'] != param['nbit1'] :
          modelName += str(param['nbit0'])
          modelName += "x"
          modelName += str(param['nbit1'])      
      else :
        modelName += str(param['nbit0'])
    modelName += "bits"
    
    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetParam ( cls ):

    return {'nbit' : 'integer', 'nbit0' : 'integer', 'nbit1' : 'integer'}

  GetParam = classmethod ( GetParam )


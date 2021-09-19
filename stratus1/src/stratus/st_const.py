
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
# |  Py Module   :       "./st_const.py"                            |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


from Hurricane   import *

from st_net      import *
from st_instance import Inst
from st_generate import Generate
from util_Misc   import ilog2, iexp2

import types

##################
def One ( nbit ) :
  global CELLS
  from st_model import CELLS
      
  cell = CELLS[-1]
  
  if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise "\n[Stratus ERROR] : there is no alim.\n"
  
  num_net             = len ( cell._TAB_NETS_OUT )
  cell._TAB_NETS_OUT += [Signal ( "one_s%d" % num_net, nbit )]

  #   "1 bit constant" => directly instanciate the virtual cell "One" cell
  # "> 1 bit constant" => generate a model
#  if nbit == 1:
#    inst_name = "one"    
#  else:
  inst_name = "one_%d" % nbit
  Generate ( "One", inst_name, param = { 'nbit' : nbit } )  
  Inst ( inst_name
         , map = { 'q'   : cell._TAB_NETS_OUT[num_net]
                 , 'vdd' : cell._st_vdds[0]
                 , 'vss' : cell._st_vsss[0]
                 }
         )
              
  return cell._TAB_NETS_OUT[num_net]

###################
def Zero ( nbit ) :
  global CELLS
  from st_model import CELLS
      
  cell = CELLS[-1]
  
  if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise "\n[Stratus ERROR] : there is no alim.\n"
  
  num_net             = len ( cell._TAB_NETS_OUT )
  cell._TAB_NETS_OUT += [Signal ( "zero_s%d" % num_net, nbit )]

  #   "1 bit constant" => directly instanciate the virtual cell "Zero" cell
  # "> 1 bit constant" => generate a model
#  if nbit == 1:
#    inst_name = "zero"    
#  else:
  inst_name = "zero_%d" % nbit
  Generate ( "Zero", inst_name, param = { 'nbit' : nbit } )  
  Inst ( inst_name
         , map = { 'nq'   : cell._TAB_NETS_OUT[num_net]
                 , 'vdd' : cell._st_vdds[0]
                 , 'vss' : cell._st_vsss[0]
                 }
         )
          
  return cell._TAB_NETS_OUT[num_net]

######################
class Constant ( Model ) :

  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )

    self.string = Constant.getString ( param )
    if 'nbit' in param: self.nbit = param['nbit']
    else: self.nbit = len(self.string)
    self.string = self.string[0] * (self.nbit - len(self.string)) + self.string

  def Interface ( self ) :
    self.outPut = SignalOut ( "o", self.nbit )

    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self ) :
    i = 0
    for letter in self.string[::-1] :
      if letter == "1" : self.outPut[i] <= One  ( 1 )
      else             : self.outPut[i] <= Zero ( 1 )
      i += 1

  def getModelName ( cls, param ) :
    modelName = "constant"
    
    modelName += "_"
    if param['nb'][0] == '-':
       modelName = modelName + 'm' + param['nb'][1:]
    else:
      modelName += param['nb']
        
    return modelName
    
  getModelName = classmethod ( getModelName )

  def getString ( param ) :
    nb = param['nb']
    
    # Error : if nb is not a string
    if not isinstance(nb,str) and not isinstance(nb,int): raise "\n[Stratus ERROR] Constant : the argument must be a string, int or long.\n"
      
    ### String representing a binary number ( from the LSB to the MSB ) ###
    if isinstance(nb,str):
      bina = re.search (         "0[bB]([0-1]+)", nb )
      hexa = re.search ( "0[xX]([0-9,A-F,a-f]+)", nb )
      oct  = re.search (         "0[oO]([0-7]+)", nb )
      dec  = re.search (              "([0-9]+)", nb )
    
      # The parameter is a binary number
      if bina :
        result = bina.group(1)
        base = 2
      # The parameter is an hexadecimal number
      elif hexa :
        result = hexa.group(1)
        base = 16
      # The parameter is an octal number
      elif oct :
        result = oct.group(1)
        base = 8
      # The parameter is a decimal number
      elif dec :
        result = oct.group(1)
        base = 10
            
      # Error if the string does not belong to the previous categories
      else :
        raise "\n[Stratus ERROR] Const : the argument must be a string representing a number in decimal, binary (0b) or hexa (0x).\n"
     
      nbit = len(result)
      nbint = int(result,base)

    ### nb is int or long
    else:
      nbint = nb
      nbit = nb.bit_length()

    if (nbint < 0):
        string = '1' + bin(nbint & (2**(nbit+1)-1))[2:]
    else:
        tmp = bin(nbint)[2:]
        string = '0'*(nbit - len(tmp)) + tmp

    return string
    
  getString = staticmethod ( getString )

  def GetParam ( cls ):

    return {'nb' : 'StringInteger'}

  GetParam = classmethod ( GetParam )

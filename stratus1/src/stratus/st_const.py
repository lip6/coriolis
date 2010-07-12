#!/usr/bin/python

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

import types

##################
def One ( nbit ) :
  global CELLS
  from st_model import CELLS
      
  cell = CELLS[-1]
  
  if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise "\n[Stratus ERROR] : there is no alim.\n"
  
  num_net             = len ( cell._TAB_NETS_OUT )
  cell._TAB_NETS_OUT += [Signal ( "one_%d" % num_net, nbit )]

  #   "1 bit constant" => directly instanciate the virtual cell "One" cell
  # "> 1 bit constant" => generate a model
#  if nbit == 1:
#    inst_name = "one"    
#  else:
  inst_name = "one_%dbits" % nbit
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
  cell._TAB_NETS_OUT += [Signal ( "zero_%d" % num_net, nbit )]

  #   "1 bit constant" => directly instanciate the virtual cell "Zero" cell
  # "> 1 bit constant" => generate a model
#  if nbit == 1:
#    inst_name = "zero"    
#  else:
  inst_name = "zero_%dbits" % nbit
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
    self.nbit   = len ( self.string )

  def Interface ( self ) :
    self.outPut = SignalOut ( "output0", self.nbit )

    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self ) :
    i = 0
    for letter in self.string :
      if letter == "1" : self.outPut[i] <= One  ( 1 )
      else             : self.outPut[i] <= Zero ( 1 )
      i += 1

  def GetModelName ( cls, param ) :
    modelName = "constant"
    
    modelName += "_"
    modelName += param['nb']
        
    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetString ( param ) :
    nb = param['nb']
    
    # Error : if nb is not a string
    if type ( nb ) != types.StringType : raise "\n[Stratus ERROR] Constant : the argument must be a string.\n"
      
    ### String representing a binary number ( from the LSB to the MSB ) ###
    bin  = re.search (     "0[bB]([0-1]+)", nb )
    hexa = re.search ( "0[xX]([0-9,A-F]+)", nb )
    oct  = re.search (     "0[oO]([0-7]+)", nb )
    dec  = re.search (          "([0-9]+)", nb )
    
    # The parameter is a binary number
    if bin :
      result = bin.group(1)
      string = ""
      for i in range ( len (result)-1, -1, -1 ) : string += result[i]
    # The parameter is an hexadecimal number
    elif hexa :
      result = hexa.group(1)
      string = ""
      raise "Hexa not ready yet."
    # The parameter is an octal number
    elif oct :
      result = oct.group(1)
      string = ""
      raise "Octal not ready yet."
    # The parameter is a decimal number
    elif dec :
      num = int ( nb )
      string = ""
      
      if not num :
        string = "0"
      else :
        while num :
          num2 = num
          num /= 2
          if ( num * 2 ) != num2 : string += "1"
          else                   : string += "0"
          
    # Error if the string does not belong to the previous categories
    else :
      raise "\n[Stratus ERROR] Const : the argument must be a string representing a number in decimal, binary (0b) or hexa (0x).\n"

    return string
    
  GetString = staticmethod ( GetString )

  def GetParam ( cls ):

    return {'nb' : 'StringInteger'}

  GetParam = classmethod ( GetParam )

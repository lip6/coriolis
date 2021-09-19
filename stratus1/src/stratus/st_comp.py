
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
# |  Py Module   :       "./st_comp.py"                             |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

import re, types

class Comp ( Model ) :

  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    
    self.nbit = param['nbit']
    self.nb   = param['nb']
    self.egal = param['egal']
    
    if self.egal : self.func = "Eq"
    else         : self.func = "Ne"    

    # Error : if net is not a string    
    if isinstance(self.nb,str):
      err = "\n[Stratus ERROR] " + self.func + " : the argument must be a string.\n"
      raise Exception ( err )

  def Interface ( self ) :
    self.netIn  = SignalIn  (  "netin", self.nbit )
    self.netOut = SignalOut ( "netout", 1 )

    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )
    
  def Netlist ( self ) :
    netConst    = Signal ( "netconst" )

    extendConst = Signal ( "extendconst", self.nbit )
    tempXor     = Signal ( "tempxor", self.nbit )

    netConst <= self.nb
 
    # Error : if the number is too big for the arity of the net
    if netConst._arity > self.nbit :
      err = "\n[Stratus ERROR] " + self.func + " : the number does not match with the net's lenght.\n"
      raise Exception ( err )
    # Otherwise, completion of the net
    elif netConst._arity < self.nbit : extendConst <= netConst.Extend ( self.nbit, 'zero' )
    else                             : netConst.Alias ( extendConst )
    
    # Xor bit per bit
    tempXor <= self.netIn ^ extendConst
   
    if self.nbit > 1 :
      if self.egal :
        Generate ( "st_comp.BoolToOne", "bto1_%s_%d" % ( "nor", tempXor._arity ), param = { 'nbit' : tempXor._arity, 'func' : "Nor" } )
        Inst ( "bto1_%s_%d" % ( "nor", tempXor._arity )
             , map   = { 'i'   : tempXor
                       , 'q'   : self.netOut
                       , 'vdd' : self.vdd
                       , 'vss' : self.vss
                       }
             )
      else :
        Generate ( "st_comp.BoolToOne", "bto1_%s_%d" % ( "or", tempXor._arity ), param = { 'nbit' : tempXor._arity, 'func' : "Or" } )
        Inst ( "bto1_%s_%d" % ( "or", tempXor._arity )
             , map   = { 'i'   : tempXor
                       , 'q'   : self.netOut
                       , 'vdd' : self.vdd
                       , 'vss' : self.vss
                       }
             )      
    else :
      if self.egal : self.netOut <= ~tempXor
      else         : self.netOut <= tempXor
      
  def GetModelName ( cls, param ) :
    modelName = "comparator"
    
    modelName += "_"
    
    if param['egal'] : modelName += "eq"
    else             : modelName += "ne" 

    modelName += "_"
    modelName += param['nb']
        
    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetParam ( cls ):

    return {'nbit' : 'integer', 'nb' : 'StringInteger' , 'egal' : 'boolean'}

  GetParam = classmethod ( GetParam )

class BoolToOne ( Model ) :

  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    
    self.nbit = param['nbit']
    
    if self.nbit < 2 : raise "\n[Stratus ERROR] booltoone : the number of bits of the inputs must be greater than 1.\n"

    self.func = param['func']
    
    if self.func not in ( "And", "Nand", "Or", "Nor", "Xor", "Nxor" ) :
      err = "\n[Stratus ERROR] " + self.func + " : the argument must be a string.\n"
      raise err

  def Interface ( self ) :
    self._in = SignalIn (  "i", self.nbit )
    self._q  = SignalOut ( "q", 1 )
    
    self._vdd = VddIn  ( "vdd" )
    self._vss = VssIn  ( "vss" )
    
  def Netlist ( self ) :
    nbStages = log2(self.nbit)
    
    def bool_function ( net1, net2 ) :
      if   self.func == 'And'  : return  ( net1 & net2 )
      elif self.func == 'Nand' : return ~( net1 & net2 )
      elif self.func == 'Or'   : return  ( net1 | net2 )
      elif self.func == 'Nor'  : return ~( net1 | net2 )
      elif self.func == 'Xor'  : return  ( net1 ^ net2 )
      elif self.func == 'Nxor' : return ~( net1 ^ net2 )
    
    out = {}
    out[0] = self._in[0]
    for i in range ( 1, self.nbit ) :
      out[i] = Signal("out%d" % i , 1 )
      out[i] <= bool_function ( out[i-1], self._in[i] )
    self._q <= out[self.nbit-1]

  def GetModelName ( cls, param ) :
    modelName = str(param['func'])
    
    modelName += "_"
    modelName += str(param['nbit'])
    modelName += "bto1"

    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetParam ( cls ):

    return {'nbit' : 'integer', 'func' : ["And", "Nand", "Or", "Nor", "Xor", "Nxor"]}

  GetParam = classmethod ( GetParam )


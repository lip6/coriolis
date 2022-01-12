
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
# |  Py Module   :       "./st_net.py"                              |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


import CRL
from Hurricane   import *

from st_model    import Model
from st_instance import Inst
from st_generate import Generate

import re, types, inspect

## Class of nets ##
PORT = ( "<cass 'st_net.SignalIn'>"
       , "<cass 'st_net.SignalOut'>"
       , "<cass 'st_net.SignalInOut'>"
       , "<cass 'st_net.SignalUnknown'>"
       , "<cass 'st_net.TriState'>"
       )

#####################
##### Class net #####
#####################
class net :
   
  # Creation of the instance : coded for each herited class 
  def __init__ ( self, nbit ) : pass

  # Deletion of the instance : the same for each net
  def Delete ( self ) :
    # Erasement of the references to the net
    if str ( self.__class__ ) in PORT :
      for i in range ( len ( self._st_cell._st_ports ) ) :
        if self._st_cell._st_ports[i] == self :
          del self._st_cell._st_ports[i]
          break
    else :
      for i in range ( len ( self._st_cell._st_sigs ) ) :
        if self._st_cell._st_sigs[i] == self :
          del self._st_cell._st_sigs[i]
          break

    for net in self._st_cell._st_sigs :
      if "_to_merge" in net.__dict__ :
        if len (  net._to_merge ) :
          for i in range ( len (  net._to_merge ) ):
            if net._to_merge[i] != 0 :
              if net._to_merge[i][0] == self : net._to_merge[i] = 0 # Erasement of a reference of the net
          non = False
          # Erasement of the net which wes going to be merged to the net being deleted
          for i in range ( len (  net._to_merge ) ) :
            if net._to_merge[i] != 0 :
              non = True
              break
          if not non : net.Delete()

    # Erasement in the connections
    for inst in self._st_cell._st_insts :
      for pin in inst._map :
        if self == inst._map[pin] :
          del inst._map[pin]
          break

    # Erasement of the huricane nets associated
    for hurNet in self._hur_net :
      if hurNet : hurNet.Delete()

  
  ##### For buses #####
  def __getitem__ ( self, indice ) :
    if isinstance(indice,slice):
      return self.getslice( indice.start, indice.stop )
    if indice == -1:
        return Sig ( self, self._ind + self._arity - 1 )
    if ( indice < self._ind ) or ( indice >= ( self._ind + self._arity ) ) :
      err = "\n[Stratus ERROR] [] : bad index " + str(indice) + " for net : " \
          + self._name + " of arity " + str(self._arity)
      if self._ind != 0 : err += " and LSB " + str(self._ind)
      err += ".\n"
      raise Exception ( err )
    
    return Sig ( self, indice )
    
  def getslice ( self, ind1, ind2 ) :
    if ind1 < ind2 :
      indmin = ind1
      indmax = ind2
    else :
      indmin = ind2
      indmax = ind1
    
    # Errors 
    if ( (indmax-indmin) < 1 ) or ( (indmax-indmin) > self._arity ) :
      err = "\n[Stratus ERROR] [:] : bad indexes " + str(indmax) + ", and " + str(indmin) \
          + " for net : " + self._name + " of arity " + str(self._arity)
      if self._ind != 0 : err += " and LSB " + str(self._ind)
      err += ".\n"
      raise Exception ( err )
    if ( indmax >= ( self._ind + self._arity ) ) or ( indmin < self._ind ) :
      err = "\n[Stratus ERROR] [:] : bad indexes " + str(indmax) + ", and " + str(indmin) \
          + " for net : " + self._name + " of arity " + str(self._arity)
      if self._ind != 0 : err += " and LSB " + str(self._ind)
      err += ".\n"
      raise Exception ( err )

    return Sig ( self, ind1, ind2 )

  ##### To affect a signal : <= #####
  def __le__ ( self, net ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]

    ### Initialisation of net representing a constant ###
    if isinstance(net,str):
      from st_const import Constant
      
      if not ( cell._st_vdds ) or not ( cell._st_vsss ) : 
        err = "\n[Stratus ERROR] : there is no alim.\n"
        raise Exception ( err )
    
      constParam = { 'nb' : net }
      string = Constant.getString ( constParam )
      
      num_net             = len ( cell._TAB_NETS_OUT )
      cell._TAB_NETS_OUT += [Signal ( "cst_o%d" % num_net, len ( string ) )]

      # 3 possible constant operator output name (nq,q,output) => 3 differents map
      if string == "0" :
        inst_name = "zero"
        map_cst = { 'nq' : cell._TAB_NETS_OUT[num_net]
                  , 'vdd': cell._st_vdds[0]
                  , 'vss': cell._st_vsss[0]
                  }
      elif string == "1" :
        inst_name = "one"
        map_cst = { 'q'  : cell._TAB_NETS_OUT[num_net]
                  , 'vdd': cell._st_vdds[0]
                  , 'vss': cell._st_vsss[0]
                  }
      else :
        inst_name = Constant.getModelName ( constParam )
        map_cst = { 'o': cell._TAB_NETS_OUT[num_net]
                  , 'vdd': cell._st_vdds[0]
                  , 'vss': cell._st_vsss[0]
                  }
        Generate ( "Constant", inst_name, param = constParam )

      Inst ( inst_name
           , map = map_cst
           )

      net = cell._TAB_NETS_OUT[num_net]
      
    ### Merging of two nets ###
    # Resizement of the output net if needed thanks to the input net's lenght
    if not ( self._arity ) : self.create_net ( self._name, net._arity )

    # Error if the nets don t have the same size
    if self._arity - net._arity :
      err = "\n[Stratus ERROR] <= : the nets " + self._name + " " + str(self._arity) + " and " + net._name + " " + str(net._arity) \
          + " must have the same lenght\n"
      raise Exception ( err )

    # If the nets are virtual, Let s work with the corresponding real nets
    if self._real_net : netInCell  = self._real_net
    else              : netInCell  = self

    if net._real_net  : netToMerge = net._real_net
    else              : netToMerge = net
    
    # Error if self is an input net
    if ( netInCell._ext ) and ( netInCell._direct == "IN" ) :
      err = "\n[Stratus ERROR] <= : " + self._name + " One can not give a value to an input net.\n"
      raise Exception ( err )
      
    if netToMerge._ext :
      err = "\n[Stratus ERROR] <= : " + self._name
      # Error if net is an output net
      if   netToMerge._direct == "OUT" : err += " One can not initialise a net with an output net.\n"
      # Error if net is an input net
      elif netToMerge._direct == "IN" :  err += " One can not initialise a net with an input net. The method Buffer() should be used.\n"
      raise Exception ( err )

    # Construction of the tab if needed
    if not ( len ( netToMerge._to_merge ) ) :
      for i in range ( netToMerge._arity ) : netToMerge._to_merge.append ( 0 )

    # Initialisation of _to_merge
    for i in range ( net._arity ) : netToMerge._to_merge[i+net._ind] = [netInCell, i+self._ind]
   
    # Puts the net in the list of nets to merge in order to have them in the right order
    self._st_cell._st_merge.append ( net ) 
   
    if self._st_cell._hur_cell :
      net.hur_merge()
      #for i in range ( net._arity ) :
      #  CRL.createPartRing ( self._st_cell._hur_cell, netInCell._hur_net[i+self._ind].getName() ) # FIXME plante avec le adder mixed dans un cas particulier indetermine ....

  ##### Instanciation of a buffer #####
  def Buffer ( self ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]
    
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) :
      err = "\n[Stratus ERROR] : there is no alim.\n"
      raise Exception ( err )

    # Creation of the output net with the right size
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "buf_o%d" % num_net, self._arity )]
 
    buffMap = { 'q'   : cell._TAB_NETS_OUT[num_net]
              , 'vdd' : cell._st_vdds[0]
              , 'vss' : cell._st_vsss[0]
              }
    
    if self._st_cell._buff == 'DpgenBuff' : buffMap['i0'] = self
    else                                  : buffMap['i']  = self

#    if ( self._st_cell._buff == "Buf" ) and ( self._arity == 1 ) :
#      inst_name = self._st_cell._buff.lower()
#    else :
    inst_name = self._st_cell._buff.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(self._arity)
 
    Generate ( self._st_cell._buff, inst_name, param = { 'nbit' : self._arity } )

    Inst ( inst_name, map = buffMap )
      
    return cell._TAB_NETS_OUT[num_net]
    
  ##### Boolean operators #####
  def __and__ ( self, other_net ) : return self.bool ( other_net, self._st_cell._and )
  def __or__  ( self, other_net ) : return self.bool ( other_net, self._st_cell._or  )
  def __xor__ ( self, other_net ) : return self.bool ( other_net, self._st_cell._xor )

  def bool ( self, other_net, model ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]
    
    if   model == self._st_cell._and : f = "&"
    elif model == self._st_cell._or  : f = "|"
    elif model == self._st_cell._xor : f = "^"
    if self._arity - other_net._arity :
      err = "\n[Stratus ERROR] " + f + " : the nets " + self._name + " and " + other_net._name + " must have the same lenght.\n"
      raise Exception ( err )

    # Creation of the output net with the right size
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "bool_o%d" % num_net, self._arity )]

    if not ( cell._st_vdds ) or not ( cell._st_vsss ) :
      err = "\n[Stratus ERROR] : there is no alim.\n"
      raise Exception ( err )

#    if ( model in ( "A2", "O2", "Xr2" ) ) and ( self._arity == 1 ) and ( other_net._arity == 1 ) :
#      inst_name = model.lower()
#        
#    else :
    inst_name = model.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(self._arity)
    
    Generate ( model, inst_name, param = { 'nbit' : self._arity } )
    
    Inst ( inst_name
         , map = { 'i0'   : self
                 , 'i1'   : other_net
                 , 'q'    : cell._TAB_NETS_OUT[num_net]
                 , 'vdd'  : cell._st_vdds[0]
                 , 'vss'  : cell._st_vsss[0]
                 }
         )

    return cell._TAB_NETS_OUT[num_net]
      
  def __invert__ ( self ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]
    
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) :
      err = "\n[Stratus ERROR] : there is no alim.\n"
      raise Exception ( err )
  
    # Creation of the output net with the right size
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "inv_o%d" % num_net, self._arity )]
    
    invMap = { 'nq'  : cell._TAB_NETS_OUT[num_net]
             , 'vdd' : cell._st_vdds[0]
             , 'vss' : cell._st_vsss[0]
             }
    if self._st_cell._not == 'DpgenInv' : invMap['i0'] = self
    else                                : invMap['i']  = self
  
#    if ( self._st_cell._not == "Inv" ) and ( self._arity == 1 ) :
#      inst_name = self._st_cell._not.lower()
#    else : 
    inst_name = self._st_cell._not.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(self._arity)
  
    Generate ( self._st_cell._not, inst_name, param = { 'nbit' : self._arity } )

    Inst ( inst_name, map = invMap )

    return cell._TAB_NETS_OUT[num_net]

  ##### Arithmetic operators #####
  def __add__ ( self, other_net ) : return self.arithgen ( other_net, self._st_cell._add, {'signed' : self._st_cell._signed, 'extended' : self._st_cell._extended} )
  def __sub__ ( self, other_net ) : return self.arithgen ( other_net, self._st_cell._sub, {'signed' : self._st_cell._signed, 'extended' : self._st_cell._extended} )
  def __mul__ ( self, other_net ) : return self.arithgen ( other_net, self._st_cell._mult, {'signed' : self._st_cell._signed} )
  def __div__ ( self, other_net ) : return self.arithgen ( other_net, self._st_cell._div  )

  def arithgen ( self, other_net, function, parameter = {} ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]

    if not function :
      err = "\n[Stratus ERROR] / : to be done.\n"
      raise Exception ( err )

    if not ( cell._st_vdds ) or not ( cell._st_vsss ) :
      err = "\n[Stratus ERROR] there is no alim in cell %s.\n" % str(cell._name)
      raise Exception ( err )

    # Creation of the output net with the right size
    num_net = len ( cell._TAB_NETS_OUT )

    if   function == self._st_cell._add  : 
      if self._st_cell._extended :
        cell._TAB_NETS_OUT += [Signal ( "add_o%d" % num_net, max ( self._arity, other_net._arity )+1 )]
      else:
        cell._TAB_NETS_OUT += [Signal ( "add_o%d" % num_net, max ( self._arity, other_net._arity ) )]
    elif function == self._st_cell._sub  : 
      if self._st_cell._extended :
        cell._TAB_NETS_OUT += [Signal ( "sub_o%d" % num_net, max ( self._arity, other_net._arity )+1 )]
      else:
        cell._TAB_NETS_OUT += [Signal ( "sub_o%d" % num_net, max ( self._arity, other_net._arity ) )]
    elif function == self._st_cell._mult : cell._TAB_NETS_OUT += [Signal ( "mul_o%d" % num_net, self._arity+other_net._arity )]    
    
    arithParam = parameter
    if not self._st_cell._signed and function == self._st_cell._mult :
       name1 = re.sub(r"\[([0-9]+):([0-9]+)\]",r"\1\2", self._name) + "ext"
       name2 = re.sub(r"\[([0-9]+):([0-9]+)\]",r"\1\2", other_net._name) + "ext"
       i0ext = Signal(name1, self._arity+1)
       i1ext = Signal(name2, other_net._arity+1)
       i0ext <= self.Extend(self._arity+1, 'zero')
       i1ext <= other_net.Extend(other_net._arity+1, 'zero')

       arithParam['nbit0'] = self._arity+1
       arithParam['nbit1'] = other_net._arity+1
       
       arithMap = { 'i0'  : i0ext
                  , 'i1'  : i1ext
                  , 'o'   : cell._TAB_NETS_OUT[num_net]
                  , 'vdd' : cell._st_vdds[0]
                  , 'vss' : cell._st_vsss[0]
                  }

    else:

       arithParam['nbit0'] = self._arity
       arithParam['nbit1'] = other_net._arity
       
       arithMap = { 'i0'  : self
                  , 'i1'  : other_net
                  , 'o'   : cell._TAB_NETS_OUT[num_net]
                  , 'vdd' : cell._st_vdds[0]
                  , 'vss' : cell._st_vsss[0]
                  }

    inst_name = function.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    for p in arithParam : inst_name += "_%s_%s" % ( str(p).lower(), str(arithParam[p]).lower() )
   
    Generate ( function, inst_name, param = arithParam )
    Inst     ( inst_name, map = arithMap )

    return cell._TAB_NETS_OUT[num_net]

  ##### Shifter #####
  def Shift ( self, inputNet, direction, type ) :
    global CELLS
    from st_model import CELLS

    cell = CELLS[-1]

    if not inputNet._arity : raise Exception ( "\n[Stratus ERROR] Shift : The input net does not have a positive arity.\n" )
    if not self._arity     : raise Exception ( "\n[Stratus ERROR] Shift : The command net does not have a positive arity.\n" )
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise Exception ( "\n[Stratus ERROR] there is no alim.\n" )
    # Wrong parameters :
    if direction not in ( "left", "right" ) : raise Exception ( "\n[Stratus ERROR] Shift : The direction parameter must be \"left\" or \"right\".\n" )
    if type not in ( "logical", "arith", "circular" ) : raise Exception ( "\n[Stratus ERROR] Shift : The type parameter must be \"logical\" or \"arith\" or \"circular\".\n" )

    # Creation of the output net with the right size
    num_net             =  len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "sh_o%d" % num_net, inputNet._arity )]

    # Initialisation of shiftType
    if direction == "left" :
      if   type == "logical" : shiftType = 0x12
      elif type == "arith"   : shiftType = 0xa
      else                   : shiftType = 0x6
    else :
      if   type == "logical" : shiftType = 0x11
      elif type == "arith"   : shiftType = 0x9
      else                   : shiftType = 0x5

    inst_name = self._st_cell._shift.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += type
    inst_name += "_"
    inst_name += str(inputNet._arity)

    Generate ( self._st_cell._shift, inst_name, param = { 'nbit' : inputNet._arity, 'type' : shiftType } )
    Inst ( inst_name
         , map = { 'cmd' : self
                 , 'i'   : inputNet
                 , 's'   : cell._TAB_NETS_OUT[num_net]
                 , 'vdd' : cell._st_vdds[0]
                 , 'vss' : cell._st_vsss[0]
                 }
         )

    return cell._TAB_NETS_OUT[num_net]

  ##### Register #####
  def Reg ( self, inputNet ) :
    global CELLS
    from st_model import CELLS

    cell = CELLS[-1]

    if not inputNet._arity : raise Exception ( "\n[Stratus ERROR] Reg : The input net does not have a positive arity.\n" )
    if not self._arity     : raise Exception ( "\n[Stratus ERROR] Reg : The clock does not have a positive arity.\n" )
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise Exception ( "\n[Stratus ERROR] there is no alim.\n" )

    # Creation of the output net with the right size
    num_net             =  len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "reg_o%d" % num_net, inputNet._arity )]

#    if ( self._st_cell._reg == "Sff1" ) and ( inputNet._arity == 1 ) :
#      inst_name = "sff1"
#    else :
#      inst_name = self._st_cell._reg.lower()
#      inst_name = re.sub ( "\.", "_", inst_name )
#      inst_name += "_"
#      inst_name += str(inputNet._arity)
#  
#      Generate ( self._st_cell._reg, inst_name, param = { 'nbit' : inputNet._arity } )

    inst_name = self._st_cell._reg.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(inputNet._arity)

    Generate ( self._st_cell._reg, inst_name, param = { 'nbit' : inputNet._arity } )

    Inst ( inst_name
         , map = { 'i'   : inputNet
                 , 'ck'  : self
                 , 'q'   : cell._TAB_NETS_OUT[num_net]
                 , 'vdd' : cell._st_vdds[0]
                 , 'vss' : cell._st_vsss[0]
                 }
         )

    return cell._TAB_NETS_OUT[num_net]

  ##### Multiplexor #####
  def Mux ( self, nets ) :

    maxPossibility = pow ( 2, self._arity ) - 1

    ### List ###
    if   isinstance(nets,list):
      if len ( nets ) !=  ( maxPossibility + 1 ) :
        raise Exception ( "\n[Stratus ERROR] Mux : when using a list, all the nets must be precised. Maybe one should use a dictionnary.\n" )
    
      return self.muxList ( nets )

    ### Dictionnary : Creation of the corresponding list ###
    elif isinstance(nets,dict):

      # Initialisation of the by default to 0 net if no default net given
      if "default" not in nets : nets["default"] = 0
    
      # Traitement of lists, intervals and numbers
      for net in nets.keys () :
        if re.search ( "-", net ) or re.search ( ",", net ) or re.search ( "#", net ) :
          chaine = net
          while chaine :
            interval = re.search ( "^([0-9]+)-([0-9]+)", chaine )
            chiffre  = re.search ( "^([0-9]+)",          chaine )
            diese    = re.search ( "^#([0-9\?]+)",       chaine )
  
            #############
            if interval :
              nb1 = int ( interval.group(1) )
              nb2 = int ( interval.group(2) )

              # Error : wrong interval
              if nb1 >= nb2 :
                raise Exception ( "\n[Stratus ERROR] Mux : when an interval is specified, the second number of the interval must be the greater one.\n" )
              # Error : if the interval does not correspond to the lenght of the command
              if ( nb1 > maxPossibility ) or ( nb2 > maxPossibility ) :
                raise Exception ( "\n[Stratus ERROR] Mux : One key does not match with the arity of the input nets.\n" )
    
              for i in range ( nb1, nb2+1 ) : nets[i] = nets[net]
      
              # Next element
              temp = re.search ( "^[0-9]+-[0-9]+,(.+)$", chaine )
              if temp : chaine = str ( temp.group(1) )
              else    : chaine = None
                
            ##############
            elif chiffre :
              if int ( chiffre.group(0) ) > maxPossibility :
                raise Exception ( "\n[Stratus ERROR] Mux : One key does not match with the arity of the input nets.\n" )

              nets[int(chiffre.group(0))] = nets[net]
  
              # Next element
              temp = re.search ( "^[0-9]+,(.+)$", chaine )
              if temp : chaine = str ( temp.group(1) )
              else    : chaine = None
            
            ############
            elif diese :
              binaire = diese.group ( 1 )

              # Error : if the binary number does not correspond to the lenght of the command
              if len ( binaire ) != self._arity :
                err = "\n[Stratus ERROR] Mux : the binary number " + str(binaire) \
                       + " does not match with the lenght of the command. It has to be a " + str(self._arity) + "bits number.\n"
                raise Exception ( err )

              # Error : if the number is not binary
              for n in binaire :
                if n not in ( "0", "1", "?", "#" ) :
                  err = "\n[Stratus ERROR] Mux : after #, the number has to be binary.\n"
                  raise Exception ( err )
               
              nombrebit = self._arity

              nb = 0
              for i in range ( -1, -nombrebit - 1, -1 ) :
                if binaire[i] == "1" : nb += pow ( 2, -i - 1 )
             
              nombres        = []
              premierpassage = 1
              for i in range ( -1, -nombrebit - 1, -1 ) :
                if binaire[i] == "?" :
                  if premierpassage :
                    nombres += [nb]
                    nombres += [nb + pow ( 2, -i - 1 )]
                    premierpassage = 0
                  else :
                    enplus = []
                    for chaquenombre in nombres : enplus += [chaquenombre + pow ( 2, -i - 1 )]
                    nombres += enplus
                    
              for chaquenombre in nombres : nets[chaquenombre] = nets[net]
  
              # Next element
              temp = re.search ( "^#.*,(.+)$", chaine )
              if temp : chaine = str ( temp.group (1) )
              else    : chaine = None
    
            else :
              err = "\n[Stratus ERROR] Mux : wrong key.\n"
              raise Exception ( err )
          
          del nets[net]

        # Otherwise it must be a number :
        elif re.search ( "^[0-9]+$", net ) :
          chaine  = re.search ( "^([0-9]+)$", net )
          chiffre = str ( chaine.group ( 1 ) )

          if int ( chiffre ) > maxPossibility :
            err = "\n[Stratus ERROR] Mux : One key does not match with the arity of the input nets.\n"
            raise Exception ( err )
    
          nets[int(chiffre)] = nets[chiffre]
          del nets[chiffre]
          
        else :
          if net != "default" :
            err = "\n[Stratus ERROR] Mux : wrong key.\n"
            raise Exception ( err )
      
      clefs =  nets.keys ()
      clefs.sort ()
   
      # Covering of the keys wich are not default
      lenClefs = 0 
      for c in clefs :
        if c != "default" : lenClefs += 1
    
      liste    = []
      decalage = 0
      for i in range ( lenClefs ) :
        nb  = clefs[i]
        nb2 = i + decalage
        if nb2 < nb :
          while nb2 < nb :
            liste.append ( nets["default"] )
            nb2      += 1
            decalage += 1
          liste.append ( nets[clefs[i]] ) # it is the right one
        else :
          liste.append ( nets[clefs[i]] )

      # Number of wanted nets :
      lenList = pow ( 2, self._arity )

      # If nets are missing theu are default nets
      while len ( liste ) < lenList : liste.append ( nets["default"] )

      # Creation of the list corresponding to the dictionnary
      return self.muxList ( liste )

    # Error : wrong argument
    else :
      err = "\n[Stratus ERROR] Mux : wrong argument type.\n"
      raise Exception ( err )

    
  ### List ###
  def muxList ( self, nets ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]
    
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise Exception ( "\n[Stratus ERROR] : there is no alim.\n" )
    
    long = 0
    for i in range ( len ( nets ) ) :
      if nets[i] : long = nets[i]._arity

    # Error : if no input net
    if not ( long ) : raise Exception ( "\n[Stratus ERROR] Mux : there are no input nets.\n" )
    
    # Instanciation of a zero cell if needed 
    for net in nets :
      if net == 0 :
        from st_const import Zero
        num_net             = len ( cell._TAB_NETS_OUT )
        cell._TAB_NETS_OUT += [Signal ( "sig0_%d" % num_net, long )]
        cell._TAB_NETS_OUT[num_net] <= Zero ( long )
        break

    # Creation of the map
    map_mux = { 'cmd' : self
              , 'vdd' : cell._st_vdds[0]
              , 'vss' : cell._st_vsss[0]
              }
        
    i = 0
    for net in nets :
      if net : map_mux['i%d' % i] = net
      else   : map_mux['i%d' % i] = cell._TAB_NETS_OUT[num_net]
    
      i += 1

    # Initialisation of the output net
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "net_out_%d" % num_net, long )]
    
    map_mux ['q']  = cell._TAB_NETS_OUT[num_net]
    
    inst_name = self._st_cell._mux.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(long)
    inst_name += "_"
    inst_name += str(self._arity)

    Generate ( self._st_cell._mux, inst_name, param = { 'nbit' : long, 'nbit_cmd' : self._arity } )
    Inst     ( inst_name, map = map_mux )

    return cell._TAB_NETS_OUT[num_net]

    
  ##### Comparison functions #####
  def Eq ( self, nb ) : return self.comparaison ( nb,  True )
  def Ne ( self, nb ) : return self.comparaison ( nb, False )

  # TODO : it could work with un string representing binary, decimal, or hexadecimal
  def comparaison ( self, nb, egal ) :
    global CELLS
    from st_model import CELLS
        
    cell = CELLS[-1]
    
    if not ( cell._st_vdds ) or not ( cell._st_vsss ) : raise Exception ( "\n[Stratus ERROR] : threre is no alim.\n" )

    # Initialisation of the output net
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "net_out_%d" % num_net, 1 )]
  
    inst_name = self._st_cell._comp.lower()
    inst_name = re.sub ( "\.", "_", inst_name )
    inst_name += "_"
    inst_name += str(self._arity)
    inst_name += "_"
    inst_name += str(nb)
    if egal : inst_name += "eq"

    Generate ( self._st_cell._comp, inst_name, param = { 'nbit' : self._arity, 'nb' : nb, 'egal' : egal } )
    Inst ( inst_name
         , map = { "netin"  : self
                 , "netout" : cell._TAB_NETS_OUT[num_net]
                 , 'vdd'    : cell._st_vdds[0]
                 , 'vss'    : cell._st_vsss[0]
                 }
         )

    return cell._TAB_NETS_OUT[num_net]

      
  ##### Method Alias #####
  def Alias ( self, net ) :
   
    # Error : if the net already has an alias
    if net._alias :
      err = "\n[Stratus ERROR] Alias : the net " + net._name + " is already an alias.\n"
      raise Exception ( err )
      
    # Resizement of the net if needed
    if not ( net._arity ) : net.create_net ( net._name, self._arity )
    
    # Error : if the nets don't have the same lenght
    if self._arity != net._arity :
      err = "\n[Stratus ERROR] Alias : the nets " + self._name + " and " + net._name + " must have the same lenght\n"
      raise Exception ( err )

    # If nets are virtual, let's work with the corresponding real nets
    if net._real_net : realNet = net._real_net
    else             : realNet = net
    
    if self._real_net : realSelf = self._real_net
    else              : realSelf = self
    
    # Construction of the tab if needed
    if not ( len ( realNet._alias ) ) :
      for i in range ( realNet._arity ) : realNet._alias.append ( 0 )
                                          
    # Initialisation of _alias
    for i in range ( net._ind, net._ind + net._arity ) :
        
      if not realSelf._alias :
        realNet._alias[i] = {realSelf : i + self._ind - net._ind}
      # Alias of an alias
      else :
        err = "\n[Stratus ERROR] Alias : 2 Alias in a row are not supported.\n"
        raise Exception ( err )
        # FIXME bug : for now, Alias of Alias are not permitted       
#        if realSelf._alias[i + self._ind - net._ind] :
#          netAlias = realSelf._alias[i + self._ind - net._ind].keys()[0]
#          bitAlias = realSelf._alias[i + self._ind - net._ind][netAlias]
#          if netAlias._real_net : netAlias = netAlias._real_net
#          realNet._alias[i] = {netAlias : bitAlias}
#          # Error : more than 2 Alias in a row not supported
#          if netAlias._alias :
#            err = "\n[Stratus ERROR] Alias : more than 2 Alias in a row are not supported.\n"
#            raise err
#        else :
#          realNet._alias[i] = {realSelf : i  + self._ind - net._ind}

  ##### Method Extend #####
  def Extend ( self, width, type ) :
    global CELLS

    from st_model import CELLS
    from st_const import Zero, One
        
    cell = CELLS[-1]

    if self._arity >= width :
      err = "\n[Stratus ERROR] Extend : the net " + self._name + \
            " can not be extended to " + str(width) + " bits, it's arity is already " + str(self._arity) + ".\n"
      raise Exception ( err )
    
    num_net             = len ( cell._TAB_NETS_OUT )
    cell._TAB_NETS_OUT += [Signal ( "%s_ext_%d_%d" % ( self._name, width, num_net ), width )]

    name = "extend_%d_%d_%s" % ( self._arity, width, type )

    Generate ( "Extend", name, param = { 'nbit0' : self._arity, 'nbit1' : width, 'type' : type } )

    Inst ( name
         , map = { 'i'   : self
                 , 'o'   : cell._TAB_NETS_OUT[num_net]
                 , 'vdd' : cell._st_vdds[0]
                 , 'vss' : cell._st_vsss[0]
                 }
         )

    return cell._TAB_NETS_OUT[num_net]
    
  ##################################################################################################################################
  ##### Initialisation of nets #####
  def create_net ( self, nom, nbit, indice = 0, extern = False, direction = None, hType = None ) :
    global CELLS
    from st_model import CELLS
    
    cell = CELLS[-1]

    # Error :
    if re.search ( "[A-Z]", nom ) :
      print( "[Stratus Warning] : Upper case letters are not supported, the name", nom, "is lowered." )
      nom = nom.lower ()
    if re.search ( " ", nom ) :
      chaine = re.search ( "st_net\.(.*)", str ( self.__class__ ) )
      classe = chaine.group(1)
      err = "\n[Stratus ERROR] " + classe + " : \"" + nom + "\" the name of the net can not contain a space.\n"
      raise Exception ( err ) 
      
    self._st_cell  = cell
    self._name     = nom
    self._arity    = nbit
    self._ind      = indice
    self._hur_net  = []
    self._alias    = []
    self._to_merge = []
    self._to_cat   = []
    self._real_net = None
    self._ext      = extern
    self._h_type   = hType
    if extern : self._direct = direction

    # Error :
    if ( nbit == 1 ) and indice :
      err = "\n[Stratus ERROR] " + str ( self.__class__ ) + " : " + self._name + " : one can not put an indice for a 1 bit net.\n"
      raise Exception ( err )
    
    if hType :
      if   hType ==  "POWER" : self._st_cell._st_vdds.append ( self )
      elif hType == "GROUND" : self._st_cell._st_vsss.append ( self )
      elif hType ==  "CLOCK" : self._st_cell._st_cks.append  ( self )
      
    # The net is put in the nets list of the cell
    else :
      if nbit :
        if extern : self._st_cell._st_ports.append ( self )
        else      : self._st_cell._st_sigs.append  ( self )
    
    # Creation of the hurricane net if in hurricane mode
    if self._st_cell._hur_cell : self.create_hur_net()
   
  ##### Initialisation of the hurricane nets for part nets #####
  def create_part_hur_net ( self ) :
    if self._ind2 < self._ind : pas = -1
    else                      : pas =  1
  
    if self._real_net._hur_net : # FIXME a priori pb du a l outil d optimisation
      for i in range ( self._ind, self._ind2 + pas, pas ) : self._hur_net.append ( self._real_net._hur_net[i] )
    
  ##### Initialisation of the hurricane nets #####
  def create_hur_net ( self ) :
    if self._arity == 1 :
      self.hur_net ( self._name, 0 )
    else :
     #for i in range ( self._ind+self._arity, self._ind, -1 ):
      for i in range ( self._ind, self._ind+self._arity, 1 ):
       #print( 'create %s(%d)' % (self._name,i) )
        self.hur_net ( '%s(%d)' % (self._name,i), i )

  ##### hur_net one by one #####
  def hur_net ( self, name, ind ) :
    if ( self._alias ) and ( self._alias[ind] ) :
      self._hur_net += [list(self._alias[ind].keys())[0]] # put the right hur_net
      return
    elif ( self._to_cat ) and ( self._to_cat[ind] ) :
      cat = self._to_cat[ind]
      self._hur_net += [cat[0]._hur_net[cat[1]]]
      return
 
    net = Net.create ( self._st_cell._hur_cell, name )
      
    net.setType ( Net.Type.LOGICAL )
    
    if self._ext : net.setExternal ( True )
    else         : net.setExternal ( False )

    if self._ext :
      if   self._direct ==       "IN" : net.setDirection ( Net.Direction.IN        )
      elif self._direct ==      "OUT" : net.setDirection ( Net.Direction.OUT       )
      elif self._direct ==    "INOUT" : net.setDirection ( Net.Direction.INOUT     )
      elif self._direct == "TRISTATE" : net.setDirection ( Net.Direction.TRISTATE  )
      elif self._direct ==  "UNKNOWN" : net.setDirection ( Net.Direction.UNDEFINED )
      
    if '_h_type' in self.__dict__ :
      if   self._h_type ==  "POWER" : net.setType ( Net.Type.POWER  ); net.setGlobal( True )
      elif self._h_type == "GROUND" : net.setType ( Net.Type.GROUND ); net.setGlobal( True )
      elif self._h_type ==  "CLOCK" : net.setType ( Net.Type.CLOCK  ); net.setGlobal( True )
    
    self._hur_net += [net]

  ##### Hurricane merge #####
  def hur_merge ( self ) :
    if self._real_net : realNet = self._real_net
    else              : realNet = self

    if self._to_cat :
      err =  "\n[Stratus ERROR] HurricanePlug <= : net " + self._name + " is a Cat net. Forbidden utilisation of <= and Cat.\n"
      raise Exception ( err )          
    
    for i in range ( self._ind, self._arity + self._ind ) :
      if realNet._to_merge[i] :
      
        selfToMerge = realNet._to_merge[i][0]
        bitToMerge  = realNet._to_merge[i][1]
      
        if selfToMerge._hur_net == [] :
          print( "[Stratus Warning] HurricanePlug <= : net", selfToMerge._name, "has no hurricane net." )
          return

        if realNet._hur_net == [] :
          err =  "\n[Stratus ERROR] HurricanePlug <= : net " + realNet._name + " has no hurricane net.\n"
          raise Exception ( err )          
        
        if bitToMerge > ( len ( selfToMerge._hur_net ) - 1 ) :
          err = "\n[Stratus ERROR] HurricanePlug <= : net " + selfToMerge._name + " with hur_net : " + str(selfToMerge._hur_net) \
              + " with asked bit : " + str(bitToMerge) + ".\n"
          raise Exception ( err )
        if i > ( len ( realNet._hur_net ) - 1 ) :
          err = "\n[Stratus ERROR] HurricanePlug <= : net " + realNet._name + " with hur_net : " + str(realNet._hur_net) \
              + " with asked bit : " + str(i) + ".\n"
          raise Exception ( err )
     
        selfToMerge._hur_net[bitToMerge].merge ( realNet._hur_net[i] )
#        realNet._hur_net[i] = 0

#    realNet.Delete()
#    del realNet

  ##### Initialisation from a net hurricane #####
  def create_from_hur ( self, hur_net ) : 
    global CELLS
    from st_model import CELLS
   
    cell = CELLS[-1]
                        
    self._st_cell  = cell
    self._alias    = []
    self._to_merge = []
    self._to_cat   = []
    self._real_net = None
    
    self._arity    = 1
    self._ind      = 0

    self._name     = str ( hur_net.getName() )
    
    if hur_net.isExternal() : self._ext = True
    else                    : self._ext = False
    
    if hur_net.isExternal() :
      if   hur_net.getDirection() == Net.Direction.IN        : self._direct = "IN"
      elif hur_net.getDirection() == Net.Direction.OUT       : self._direct = "OUT"
      elif hur_net.getDirection() == Net.Direction.INOUT     : self._direct = "INOUT"
      elif hur_net.getDirection() == Net.Direction.TRISTATE  : self._direct = "TRISTATE"
      elif hur_net.getDirection() == Net.Direction.UNDEFINED : self._direct = "UNDEFINED"
    
    if   hur_net.getType() == Net.Type.POWER  : self._h_type = "POWER"
    elif hur_net.getType() == Net.Type.GROUND : self._h_type = "GROUND"
    elif hur_net.getType() == Net.Type.CLOCK  : self._h_type = "CLOCK"
                                                              
    self._hur_net  = [hur_net]
      
    if ( self._ext ) and ( '_h_type' in self.__dict__ ) :
      if self._h_type == "CLOCK" : cell._st_ports.append ( self ) # not alimentations
    else :
      cell._st_sigs.append ( self )
    
###########
# SignalIn #
###########
class SignalIn ( net ) :
  def __init__ ( self, nom, nbit, indice = 0 ) :

    if nbit < 1 : raise Exception ( "\n[Stratus ERROR] SignalIn : the lenght of the net must be a positive value\n" )

    self.create_net ( nom, nbit, indice, True, "IN" )
    
class SignalInFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )

############
# SignalOut #
############
class SignalOut ( net ) :
  def __init__ ( self, nom, nbit, indice = 0 ) :
    
    if nbit < 1 : raise Exception ( "\n[Stratus ERROR] SignalOut : the lenght of the net must be a positive value\n" )
      
    self.create_net ( nom, nbit, indice, True, "OUT" )
    
class SignalOutFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )

##############
# SignalInOut #
##############
class SignalInOut ( net ) :
  def __init__ ( self, nom, nbit, indice = 0 ) :
    
    if nbit < 1 : raise Exception ( "\n[Stratus ERROR] SignalInOut : the lenght of the net must be a positive value\n" )
      
    self.create_net ( nom, nbit, indice, True, "INOUT" )
    
class SignalInOutFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )

################
# SignalUnknown #
################
class SignalUnknown ( net ) :
  def __init__ ( self, nom, nbit, indice = 0 ) :
    
    if nbit < 1 : raise Exception ( "\n[Stratus ERROR] SignalUnknown : the lenght of the net must be a positive value\n" )
      
    self.create_net ( nom, nbit, indice, True, "UNKNOWN" )
    
class SignalUnknownFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
    
############
# TriState #
############
class TriState ( net ) :
  def __init__ ( self, nom, nbit, indice = 0 ) :
    
    if nbit < 1 : raise Exception ( "\n[Stratus ERROR] TriState : the lenght of the net must be a positive value\n" )

    self.create_net ( nom, nbit, indice, True, "TRISTATE" )
    
class SignalTriStateFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
    
#########
# Clock #
#########
class CkIn ( net ) :
  def __init__ ( self, nom ) :
    self.create_net ( nom, 1, extern = True, direction = "IN", hType = "CLOCK" )
    
class CkInFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
    
#########################
# Internal Clock Signal #
#########################
class SignalCk ( net ) :
  def __init__ ( self, nom ) :
    self.create_net ( nom, 1, direction = "IN", hType = "CLOCK" )
    
class SignalCkFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
        
###################
# Internal Signal #
###################
class Signal ( net ) :
  def __init__ ( self, nom, nbit = 0, indice = 0 ) :

    if nbit < 0 : raise Exception ( "\n[Stratus ERROR] Signal : the lenght of the net must be a positive value\n" )
      
    self.create_net ( nom, nbit, indice )
    
class SignalFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )

################
# Alimentation #
################
class VddIn ( net ) :
  def __init__ ( self, nom ) : self.create_net ( nom, 1, extern = True, direction = "IN", hType = "POWER" )
class VssIn ( net ) :
  def __init__ ( self, nom ) : self.create_net ( nom, 1, extern = True, direction = "IN", hType = "GROUND" )

class VddInFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
class VssInFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )

class SignalVdd ( net ) :
  def __init__ ( self, nom ) : self.create_net ( nom, 1, direction = "IN", hType = "POWER" )
class SignalVss ( net ) :
  def __init__ ( self, nom ) : self.create_net ( nom, 1, direction = "IN", hType = "GROUND" )

class SignalVddFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
class SignalVssFromHur ( net ) :
  def __init__ ( self, hur_net ) : self.create_from_hur ( hur_net )
      
##################
# Virtual signal #
##################
class Sig ( net ) :
  def __init__ ( self, net, indiceFort, indiceFaible = -1 ) :
    global CELLS
    from st_model import CELLS

    if indiceFaible == -1 : indiceFaible = indiceFort
    
    self._st_cell  = CELLS[-1]
    self._ind      = indiceFaible
    self._ind2     = indiceFort
    self._alias    = []
    self._to_merge = []
    self._to_cat   = []
    self._real_net = net
    self._ext      = net._ext
    self._arity    = abs ( indiceFort - indiceFaible ) + 1

    if indiceFort < indiceFaible : self._reverse = True
    else                         : self._reverse = False
    
    self._hur_net  = []

    self._st_cell._st_partsigs.append  ( self )

    # Initialisation of _hur_net if in hurricanePlug mode
#    if CELLS[0]._hur_plug : self.create_part_hur_net()

    if self._ext : self._direct = net._direct

    if '_h_type' in net.__dict__ : self._h_type = net._h_type
    
    if indiceFort == indiceFaible : self._name = net._name + "[" +                         str(indiceFaible) + "]"
    else                          : self._name = net._name + "[" + str(indiceFort) + ":" + str(indiceFaible) + "]"

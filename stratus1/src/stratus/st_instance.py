
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
# |  Py Module   :       "./st_instance.py"                         |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



import CRL
from Hurricane        import *
#from st_net           import Signal, SignalIn, SignalOut, SignalInOut,      \
#                             SignalUnknown, TriState, CkIn, SignalCk,       \
#                             Sig, VddIn, VssIn, VddInFromHur, VssInFromHur, \
#                             SignalVdd, SignalVss
from st_model         import Model, MODELMAP
from st_getrealmodel  import GetRealModel, InitBV

import sys, re, types, os

## Alliance's libraries ##
SXLIB   = "_x[0-8]"
PXLIB   = "_px"
RAMLIB  = "ram_"
RFLIB   = "rf_"
RF2LIB  = "rf2_"
ROMLIB  = "rom_"
DPSXLIB = "dp_.*_x[1-8]"

## Puts MBK_CATA_LIB at the begining of PYTHONPATH ##
#cata_lib = os.environ['MBK_CATA_LIB']
#chaine = re.search ( "([^:]*):(.*)", cata_lib )
#while chaine :
#  rep      = chaine.group ( 1 )
#  cata_lib = chaine.group ( 2 )
#
#  sys.path.insert ( 0, rep )
#
#  chaine = re.search ( "([^:]*):(.*)", cata_lib )

## Class of nets ##
NET = ( "<class 'st_net.SignalIn'>"
      , "<class 'st_net.SignalOut'>"
      , "<class 'st_net.SignalInOut'>"
      , "<class 'st_net.SignalUnknown'>"
      , "<class 'st_net.TriState'>"
      , "<class 'st_net.CkIn'>"
      , "<class 'st_net.SignalCk'>"
      , "<class 'st_net.Signal'>"
      , "<class 'st_net.Sig'>"
      , "<class 'st_net.VddIn'>"
      , "<class 'st_net.VssIn'>"
      , "<class 'st_net.VddInFromHur'>"
      , "<class 'st_net.VssInFromHur'>"
      , "<class 'st_net.SignalVdd'>"
      , "<class 'st_net.SignalVss'>"
      )
ALIM_NET = ( "<class 'st_net.VddIn'>"
           , "<class 'st_net.VssIn'>"
           , "<class 'st_net.VddInFromHur'>"
           , "<class 'st_net.VssInFromHur'>"
           )

################
##### Inst #####
################
class Inst :
  
  ################  
  ##### Init #####
  ################
  def __init__ ( self, model, name = None, map = {}, cell = None ) :
    global MODELMAP, CELLS, FRAMEWORK
    
    from st_model   import MODELMAP, CELLS, FRAMEWORK
    from util_Place import UNPLACED, NOSYM
    
    if not cell : cell = CELLS[-1]

    ##### Virtual library #####
    global BV
    from st_parser import BV
    if BV == [] : InitBV()
    
    if model in BV : model, self._inout = GetRealModel ( model )

    ##### Attributes of the instance #####
    self._model        = model.lower()
    self._st_cell      = cell
    self._hur_instance = None
    self._plac         = UNPLACED
    self._map          = map

    ##### Name of the instance ######
    if not name :
      name = "%s_i%d" % ( self._model, cell._NB_INST )
      cell._NB_INST += 1
    self._name = name
    
    ##### The instance is put in the instances' list of the cell #####
    cell._st_insts.append ( self )
    
    ##### Errors #####
    # Error : if the model is not a string
    if not isinstance(model,str):
      err = "\n[Stratus ERROR] Inst : the model must be described in a string.\n"
      raise Exception ( err )
    # Warning : the model can not contain capitalized letters
    if re.search ( "[A-Z]", model ) :
      print( "[Stratus Warning] Inst : Upper case letters are not supported, the name", model, "is lowered.")
      model = model.lower()
    # Error : spaces are forbidden
    if re.search ( " ", model ) :
      err = "\n[Stratus ERROR] Inst : " + name + " the name of the model \"" + model + "\" can not contain a space.\n"
      raise Exception ( err )
    if name :
      if re.search ( " ", name ) :
        err = "\n[Stratus ERROR] Inst : \"" + name + "\" the name of the instance can not contain a space.\n"
        raise Exception ( err )
      # Warning : the name can not contain capitalized letters
      if re.search ( "[A-Z]", name ) :
        print( "[Stratus Warning] : Upper case letters are not supported, the name", name, "is lowered." )
        name = name.lower ()
    
    # Error : if map[pin] is not a net
    if map :
      for pin in map :
        if (str(type(map[pin])) not in NET):
          err = "\n[Stratus ERROR] Inst : \"" + name + "\" one argument is not a net : "
          err += "pin is : " + pin + " and is associated to : "
          if map[pin] : err += str(map[pin])
          else        : err += "None"
          err += "\n"
          raise Exception ( err )

    ##### MasterCell #####
    self._hur_masterCell = FRAMEWORK.getCell ( self._model, CRL.Catalog.State.Views )
    
    self._st_masterCell  = None
    for c in CELLS[0]._underCells :
      if self._model == c._name :
        self._st_masterCell = c
        break
#    if not( self._hur_masterCell or self._st_masterCell ) : print( "\n[Stratus Warning] Inst : no master cell found for instance " + self._name )

    # Creation of the hurricane instance 
    if CELLS[0]._hur_plug : self.create_hur_inst ( model )
                                         
  ##############                         
  ### Delete ###                         
  ##############                         
  def Delete ( self ) :                  
    # Erasement of the references to the  instance
    for i in range ( len ( self._st_cell._st_insts ) ) :
      if self._st_cell._st_insts[i] == self :
        del self._st_cell._st_insts[i]
        break
    # Erasement of the hurricane instance associated
    if self._hur_instance : self._hur_instance.Delete()

  ##############################################
  ##### Creation of the hurricane instance #####
  ##############################################
  def create_hur_inst ( self, model ) :
    if not self._hur_masterCell :
     #self._hur_masterCell = FRAMEWORK.getCell ( self._model, CRL.Catalog.State.Views )
      self._hur_masterCell = FRAMEWORK.getCell ( model, CRL.Catalog.State.Views )

    if not self._hur_masterCell :
      err = '[ERROR] stratus.create_hur_inst(): Framework unable to load master cell "%s".' % model
      raise Exception ( err )

    if not self._st_masterCell :
      if str ( self._hur_masterCell ) in MODELMAP:
        self._st_masterCell = MODELMAP[str ( self._hur_masterCell )]
      else :
        self._st_masterCell = Model ( str ( self._hur_masterCell.getName() ), hurCell = self._hur_masterCell )

    if not self._st_cell._hur_cell :
      err = "\n[Stratus ERROR] HurricanePlug : Problem of hurricane cell.\nTry to contact Coriolis team.\n"
      raise Exception ( err )
     
    UpdateSession.open()
    inst = Instance.create ( self._st_cell._hur_cell
                           , self._name
                           , self._hur_masterCell
                           )
    # The hurricane instance is connected to the stratus instance
    self._hur_instance = inst
    
    ##### Connection #####
    ##### Function to be applied on each pin 
    def connectPin ( pin ) : 
      # Error : if there is a space in the name of the pin (usually done at the end of the pin ...)
      if re.search ( " ", pin ) :
        err = "\n[Stratus ERROR] Inst : " + self._name + " the keys of the connection map can not contain a space : \"" + pin + "\".\n"
        raise Exception ( err )
        
      # Error : if the net to connect does not have it's arity defined
      if not ( mapNet._arity ) :
        err = "\n[Stratus ERROR] Inst : " + self._name + " : the size of " + mapNet._name + " has not been defined properly.\n"
        raise Exception ( err )
        
      wrong_pin  = 1
      for net in self._hur_masterCell.getExternalNets():
        if ( pin == str(net.getName()) ) or re.search ( "%s\(" % pin, str(net.getName()) ) :
          wrong_pin = 0
          break

      # Error : if the pin doesn't exist in the model's ports
      if wrong_pin :
        err = "\n[Stratus ERROR] Inst : " + str(self._name) + " : port " + pin \
               + " does not exist in model " + str(self._hur_masterCell.getName()) + ". Pins are :"
        for pin in self._hur_masterCell.getExternalNets():
          err += str ( pin.getName() )
          err += ","
        sea = re.search ( "(.*),$", err )
        if not sea : err += "No pins found ..."
        else       : err  = sea.group(1)
        raise Exception ( err )
        
      # Error : if the arities of the nets don't correspond
      tabPins = self._st_masterCell._st_ports + self._st_masterCell._st_cks + self._st_cell._st_vdds + self._st_cell._st_vsss
      for net in tabPins :
        if net._name == pin :
          if net._arity != mapNet._arity :
            err = "\n[Stratus ERROR] Inst : " + str(self._name) + " : The arity of the net " + mapNet._name + " " + str(mapNet._arity) \
                + " does not correspond to the arity of the port of the cell : "+ net._name + " " + str(net._arity) + ".\n"
            raise Exception ( err )
 
      # If the port of the masterCell doesn't have a LSB which is 0
      lsb = 0
      if mapNet._arity > 1 :
        netBus = None
        while not ( netBus ) :
          netBus = self._hur_masterCell.getNet ( pin.lower() + "(" + str ( lsb ) + ")" )
          lsb += 1
          if lsb > 20 : # value chosen in order to avoid infinite loop in case of a problem (may be higher)
            err = "\n[Stratus ERROR] Inst : " + str(self._name) + " : Probem of map, check the arities of your nets.\n"
            raise Exception ( err )
        if lsb : lsb -= 1
            
      ### Connection ###
      if mapNet._real_net : realNet = mapNet._real_net
      else                : realNet = mapNet

      # range 1: given the indice's net
      nbit1 = mapNet._ind - realNet._ind
      # range 2: deal with reverse nets
      if ( '_reverse' in mapNet.__dict__ ) and ( mapNet._reverse ) :
        nbit2 = nbit1 - mapNet._arity
        pas   = -1
      else :
        nbit2 = nbit1 + mapNet._arity
        pas   =  1
      
      j = 0
      for i in range ( nbit1, nbit2, pas ) :
        # If the net has to be concatened
        if ( realNet._to_cat ) and ( realNet._to_cat[i] ) :
          net = realNet._to_cat[i][0]
          bit = realNet._to_cat[i][1]
          if net._real_net : net = net._real_net
          
          # If the net which is concatened is an alias
          if ( net._alias ) and ( net._alias[bit] ) :
            netA = list(net._alias[bit].keys())[0]
            bitA = net._alias[bit][netA]
            if net._real_net : netA = netA._real_net
          
            hurNet = netA._hur_net[bitA-net._ind]
          else :
            hurNet = net._hur_net[bit-net._ind]
          
        # If the net is an alias
        elif ( realNet._alias ) and ( realNet._alias[i] ) :
          net = list(realNet._alias[i].keys())[0]
          bit = realNet._alias[i][net]
          if net._real_net : net = net._real_net
          
          hurNet = net._hur_net[bit+net._ind]

        else :
          hurNet = realNet._hur_net[i]

        if mapNet._arity == 1: hurNetName = pin.lower()
        else:                  hurNetName = pin.lower() + "(" + str(j+lsb) + ")"
        j += 1

        tempNet = self._hur_masterCell.getNet ( hurNetName )
      
        if not ( tempNet ) :
          err = '[ERROR] stratus.Inst(): Problem in map. Check that the arities of your nets are correct ("%s").' % hurNetName
          raise Exception ( err )
        
        plug = self._hur_instance.getPlug ( tempNet )
        plug.setNet ( hurNet )
       
        # In order to see the ring
        #if str ( realNet.__class__ ) not in ALIM_NET : CRL.createPartRing ( self._st_cell._hur_cell, hurNet.getName() )

    ##### Loop on each pin
    for pin in self._map :
      mapNet = self._map[pin]

      ### Virtual library ###
      if "_inout" in self.__dict__ :
        import types
        if isinstance( self._inout[pin], list ):
          for realpin in self._inout[pin] :
            connectPin ( realpin )
        else :
          realpin = self._inout[pin]
          connectPin ( realpin )
      ### Other ###
      else :
        connectPin ( pin )
    
    # Error message if the connection is not correct (detection before vst driver)
    # Not for vdd/vss in case of utilisation of SetGlobal
    # The detection is done with vst driver in this case ...
    for plug in self._hur_instance.getUnconnectedPlugs():
      if plug.getMasterNet().getType() not in ( Net.Type.POWER, Net.Type.GROUND ) :
        name = str(plug.getMasterNet().getName())
        chaine = re.search ( "(.*)\(", name )
        if chaine : name = chaine.group(1)
    
        err = "\n[Stratus ERROR] Inst : plug " + name + " of instance " + self._name + " must be connected.\n"
        err += '               (raw name: <%s>)\n' % str(plug.getMasterNet().getName())
        raise Exception ( err )

    UpdateSession.close()

  ##############
  ### Prints ###
  ##############
  def printInstance ( self ) :
    print( "  => model", self._model )
    print( "  => map" )
    for pin in self._map :
      n = self._map[pin]
      if n._to_merge : n = n._to_merge[0][0]
      print( "    ", pin, "->", n._name )

  def printMap ( self ) :
    print( "Map:", self._name )
    for pin in self._map :
      print( "  ", pin, self._map[pin]._name )

#########################
#### SetCurrentModel ####
#########################
def SetCurrentModel ( instance ) :
  global CELLS
  from st_model import CELLS

  if not instance :
    err = "\n[Stratus ERROR] SetCurrentModel : argument given does not exist.\n"
    raise Exception ( err )
  cell = instance._st_masterCell
  
  if not cell :
    err = "\n[Stratus ERROR] SetCurrentModel : cannot find model for instance " + str ( getName ( instance ) ) + ".\n"
    raise Exception ( err )

  CELLS.append ( cell )
    
  return cell

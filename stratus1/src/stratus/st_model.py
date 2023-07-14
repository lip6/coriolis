
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |  S t r a t u s   -  Netlists/Layouts Description                |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./st_model.py"                            |
# +-----------------------------------------------------------------+


import re
import types
import string
import Cfg
import CRL
import Viewer
from   Hurricane  import *


FRAMEWORK = None
EDITOR    = None

MODELMAP  = {}
CELLS     = []

STRATUS   = 100
LOGICAL   = CRL.Catalog.State.Logical
PHYSICAL  = CRL.Catalog.State.Physical
VIEWS     = CRL.Catalog.State.Views

def setEditor ( editor ):
  global EDITOR
  
  EDITOR = editor
  return

#######################
##### Class Model #####
#######################
class Model ( object ) :
 
  ##########################
  ##### Initialisation #####
  ##########################
  def __init__ ( self, nom, param = {}, hurCell = None, hurricane_plug = True ) :
    global FRAMEWORK, CELLS

    # Look up the editor
    if '__editor' in globals(): setEditor ( __editor )

    self._name         = nom
    self._param        = param
    self.pat           = None

    self._hur_cell     = hurCell

    self._st_insts     = [] # Tab : list of the instances
    self._st_ports     = [] # Tab : list of the nets
    self._st_sigs      = [] # Tab : list of the nets
    
    self._st_partsigs  = [] # Tab : list of the partial nets
    self._st_merge     = [] # Tab : list of the nets to merge, in order to have them in the right order

    self._hierarchy    = 0  # Modified in ModelCreation
    
    self._TAB_NETS_OUT = []
    self._TAB_NETS_CAT = []
    self._NB_INST      = 0
    
    # List of the generators used for overload
    self._and          = 'A2'
    self._or           = 'O2'
    self._xor          = 'Xr2'
    self._not          = 'Inv'
    self._buff         = 'Buf'
    self._reg          = 'Sff1'
    # Stratus generators
    self._mux          = 'Smux'
    self._shift        = 'Shifter'
    self._comp         = 'Comp'
    # Arithmetic generators
    self._add          = 'Slansky'
    self._sub          = 'Sub'
    self._mult         = 'Multiplier'
    self._div          = None

    self._signed       = True
    self._extended     = False

    self._model_map    = {}
    
    # List of the under-cells of a cell
    self._underCells   = {}
#    self._tabPlacement = []
    # Instance reference for placement
    self._insref       = None

    # Place and Route
    self._nb_alims_verticales    = 0  
    self._nb_pins                = 0  # to number the pins of the core  
    self._nb_vdd_pins            = 0  # to number the pins vdd of the core  
    self._nb_vss_pins            = 0  # to number the pins vss of the core 
    self.standard_instances_list = []
    self.pad_north               = [] # list to save the pads names 
    self.pad_south               = []
    self.pad_east                = []
    self.pad_west                = []
    
    # Creation of the database 
    if not FRAMEWORK :
     #Initialize()
      FRAMEWORK = CRL.AllianceFramework.get()
    
    self._st_vdds  = []
    self._st_vsss  = []
    
    if hurCell : 
      from st_net import VddInFromHur
      from st_net import VssInFromHur
      
      try:
        netVdd = iter(hurCell.getPowerNets()).__next__()
        self._st_vdds.append ( VddInFromHur ( netVdd ) )
      except StopIteration:
        print( "[Stratus Warning] : Cell", self._name, "does not have a vdd port." )
        pass
      try:
        netVss = iter(hurCell.getGroundNets()).__next__()
        self._st_vsss.append ( VssInFromHur ( netVss ) )
      except StopIteration:
        print( "[Stratus Warning] : Cell", self._name, "does not have a vss port." )
        pass

    self._st_cks = []
    
    if not hurCell :
      # List of the cells 
      CELLS.append ( self )
      # List of the undercells
      if CELLS[0] != self :
        CELLS[0]._underCells[self] = 0
#        CELLS[0]._tabPlacement.append ( self )

    self._hur_plug = hurricane_plug

    # HurricanePLug if : no hurCell, in mode hurricane plug (i.e. hurricane_plug and hurricane_plug of the masterCell)
    if ( not hurCell ) and ( hurricane_plug ) :
      if CELLS[0] == self :
        self.HurricanePlug()
      else :
        if CELLS[0]._hur_plug :
          self.HurricanePlug()

  ##### Delete #####
  def Delete ( self ) :
    UpdateSession.open()

    under_cells = self._underCells
    self._hur_cell.Delete()
    for cell in under_cells : cell._hur_cell.Delete()

    UpdateSession.close()
    
  #####################################################
  ##### Initialisation of attributes for the cell #####
  #####################################################
  def InitGraph ( self ) :
    self._graph = True

    # External nets
    for net in self._st_ports :
      net._st_inst_in   = None
      net._st_insts_out = []
      net._covered      = False
    
    # Internal nets
    for net in self._st_sigs  :
      net._st_inst_in   = None
      net._st_insts_out = []
      net._covered      = False

    # Part nets
    for pnet in self._st_partsigs :
      pnet._st_inst_in   = None
      pnet._st_insts_out = []
      pnet._covered      = False
    
    # Instances
    for inst in self._st_insts :
      inst._st_nets_in  = []
      inst._st_nets_out = []

      # Initialisation of instances and nets
      self.initNet  ( inst )
      self.initInst ( inst )

  ########################################################
  ##### Initialisation of the attributes of the nets #####
  ########################################################
  def initNet ( self, instance ) :
    # For each pin
    for pin in instance._map :
      # Check if the pin is an output or an input
      direct = ""
      direct = self.getDirect ( instance._model, pin )

      # Get the net connected to this pin
      if instance._map[pin]._to_merge : mapNet = instance._map[pin]._to_merge[0][0]
      else                            : mapNet = instance._map[pin]
     
      if len ( instance._map[pin]._to_merge ) > mapNet._arity :
        mapNet._real_arity = len ( instance._map[pin]._to_merge )
        mapNet._inverse_merge = instance._map[pin]
      
      # Initialisation 
      if direct == 'input' :
        if mapNet._real_net :
          mapNet = mapNet._real_net
          
        if '_st_insts_out' not in mapNet.__dict__ : mapNet._st_insts_out = []

        if instance not in mapNet._st_insts_out :   mapNet._st_insts_out += [instance]

      elif direct == 'output' : # FIXME _real_net not taken care of
        mapNet._st_inst_in = instance
        
  #############################################################
  ##### Initialisation of the attributes of the instances #####
  #############################################################
  def initInst ( self, instance ) :
    # For each pin
    for pin in instance._map :
      # Check if the pin is an output or an input
      direct = ""
      direct = self.getDirect ( instance._model, pin )

      # Get the net connected to this pin
      if instance._map[pin]._to_merge : mapNet = instance._map[pin]._to_merge[0][0]
      else                            : mapNet = instance._map[pin]
    
      # Initialisation 
      if   direct == 'input'  : instance._st_nets_in  += [mapNet]
      elif direct == 'output' : instance._st_nets_out += [mapNet]

  ###########################################
  ##### Returns the direction of a port #####
  ###########################################
  def getDirect ( self, model, pin ) :
    global FRAMEWORK
    
    hurCell = FRAMEWORK.getCell ( model, CRL.Catalog.State.Views )
   
    stCell = None 
    if not hurCell :
      for c in self._underCells :
        if c._name == model :
          stCell = c
          break

    if not hurCell and not stCell :
      err = "\nError : no cell found with model %s.\n" % model
      raise Exception ( err )
   
    # Hurricane cell found: 
    if hurCell :
      found = False
      for net in hurCell.getExternalNets():
        direction = net.getDirection()
        name      = str(net.getName())

        if net.getType() in ( Net.Type.POWER, Net.Type.GROUND, Net.Type.CLOCK ) :
          found = True
          continue
          
        chaine = re.search ( "(.*)\([0-9]+\)", name )
        if chaine : name = chaine.group(1)
      
        if name == pin :
          found = True
          if   direction == DirectionOUT : return "output"
          elif direction == DirectionIN  : return "input"
          else :
            err = "\nError : unable to find direction of port %s in model %s. Direction is %d\n" % ( pin, model, direction )
          raise Exception ( err )
          
      if not found :
        err = "\nError : unable to find port %s in model %s.\n" % ( pin, model )
        raise Exception ( err )
          
    # Stratus cell found:
    elif stCell :
      found = False
      
      for net in stCell._st_vdds :
        if net._name == pin :
          found = True
          break
      
      for net in stCell._st_vsss :
        if net._name == pin :
          found = True
          break
      
      for net in stCell._st_cks :
        if net._name == pin :
          found = True
          break

      # _st_ports i.e. not alimentations and not clock
      for net in stCell._st_ports : 
        if net._name == pin :
          found = True
          if   net._direct == "OUT" : return "output"
          elif net._direct == "IN"  : return "input"
          else :
            err = "\nError : unable to find direction of port %s in model %s.\n" % ( pin, model )
            raise Exception ( err )
   
      if not found :
        ports = ""
        for net in stCell._st_ports : ports += net._name + ","
        err = "\nError : unable to find port %s in model %s.\nPorts are : %s\n" % ( pin, model, ports )
        raise Exception ( err )

    else :
      err = "\n[ERROR] InitGraph : no model named " + model + " in the database.\n"
      raise Exception ( err )

  #############################
  ##### Print of the cell #####
  #############################
  def Print ( self ) :
    print( "################## The Cell ##################" )
    for inst in self._st_insts :
      print( " * inst name :", inst._name )
      print( "    inst model :", inst._model )
      for pin in inst._map :
        if pin != 'vdd' and pin != 'vss' :
          net = inst._map[pin]
          print( "     pin :", pin, "net :", net._name, "with arity :", net._arity )
          if net._to_merge :
            net = inst._map[pin]._to_merge[0][0]
            print( "       net merged with :", net._name, "with arity :", net._arity )

  #############################
  ##### Find An Instance  #####
  #############################
  def getInstance ( self, name ):
    for instance in self._st_insts:
      if instance._name == name:
        return instance
    return None

  #########################
  def PrintGraph ( self ) :
    if  "_graph" not in self.__dict__ :
      err = "\n[Stratus ERROR] PrintGraph : The graph does not exist. Use initGraph before.\n"
      raise Exception ( err )
    else :
      if self._graph == False :
        err = "\n[Stratus ERROR] PrintGraph : The graph does not exist. Use initGraph before.\n"
        raise Exception ( err )
      
    print( "################## Cell's Graph features ##################" )
    for inst in self._st_insts :
      print( " => inst name :", inst._name, "( model :", inst._model, ")" )
      print( "    inst st_nets_in :" )
      for net in inst._st_nets_in : print( "        ", net._name )
      print( "    inst st_nets_out :" )
      for net in inst._st_nets_out : print( "        ", net._name )
    for port in self._st_ports :
      print( " => port name :", port._name )
      if port._st_inst_in : print( "    port _st_inst_in :", port._st_inst_in._name )
      print( "    port _st_insts_out :" )
      for p in port._st_insts_out : print( "        ", p._name )
    for sig in self._st_sigs :
      print( " => sig name :", sig._name )
      if sig._st_inst_in : print( "    sig _st_inst_in :", sig._st_inst_in._name )
      print( "    sig _st_insts_out :" )
      for s in sig._st_insts_out : print( "        ", s._name )
    for psig in self._st_partsigs :
      print( " => part sig name :", psig._name )
      if '_st_inst_in' in psig.__dict__ and psig._st_inst_in: print( "   part sig _st_inst_in :", psig._st_inst_in._name )
      if '_st_insts_out' in psig.__dict__ :
        print( "   part sig _st_insts_out :" )
        for s in psig._st_insts_out : print( "        ", s._name )

  #####################
  ##### overloard #####
  #####################
  ##### __getattr #####
#  def __getattr__ ( self, attr ) :
    
  ##### __str #####
  def __str__ ( self ) :
    s = "Model" + str ( self._hur_cell )
    return s
    
  ########################
  ##### VLSI Methods #####
  ########################
  ##### Interface #####
  def Interface ( self ) :
    print( "[Stratus Warning] : Execution of empty Interface method for", self._name, "." )
                           
  ##### Netlist #####
  def Netlist ( self ) :
    print( "[Stratus Warning] : Execution of empty Netlist method for", self._name, "." )
    pass
                              
  ##### Layout #####
  def Layout ( self ) :
    print( "[Stratus Warning] : Execution of empty Layout method for", self._name, "." )
    pass
                        
  ##### Vbe #####
  def Vbe ( self ) :
    self._vbe = True
    
  ##### View : in order to see the in the editor #####
  def View ( self, stopLevel=1, message="Status stop point" ) :
    global EDITOR

    if not self._hur_cell :
      err = "\n[Stratus ERROR] View : Hurricane Cell does not exist.\nCheck CRL_IN_LO/CRL_IN_PH variables.\n"
      raise Exception ( err )

    if EDITOR:
      EDITOR.setCell ( self._hur_cell )
      EDITOR.fit     ()
      if stopLevel > 0:
        Breakpoint.stop ( stopLevel, message )

  def getCell ( self ):
      return self._hur_cell
    
  ##### Save : in order to create the output files #####
  def Save ( self, views = 0, fileName = None ) :
    global FRAMEWORK
    global CELLS

    netlistFormat = Cfg.getParamString('stratus1.format').asString()

    if views == STRATUS :
      self.exportStratus ( fileName )

    elif netlistFormat == 'vst' :
      UpdateSession.open()
  
      hurCell = self._hur_cell
  
      if str ( hurCell.getName() ) != "__Scratch__" :
        FRAMEWORK.saveCell ( hurCell, views|CRL.Catalog.State.Logical|CRL.Catalog.State.VstUseConcat )
  
      if len ( CELLS ) == 0 :
        err = "\n[Stratus ERROR] Save : CELLS stack is empty.\n"
        raise Exception ( err )
  
      CELLS.pop()
      
      UpdateSession.close()

    elif netlistFormat == 'stratus' :
      self.exportStratus ( fileName )

    elif netlistFormat in ['vhd','vlog','json'] :
      self.export(netlistFormat)

    else :
      raise Exception('Unrecognized format %s' % netlistFormat)

  ##### Simul : in order to use simulation tool #####
  def Simul ( self, name = None, tool = 'asimut' ) :
    from utils import runpat
      
    if not name : name = self._name

    simulator = Cfg.getParamString('stratus1.simulator').asString()

    if simulator == 'asimut' :
      runpat ( self._name, name, '-l 1 -p 100 -zerodelay -bdd' )
    elif simulator == 'ghdl' :
      import os
      cmd_str = ('ghdl -c -g -Psxlib --ieee=synopsys *.vhd -r %s_run --vcd=%s.vcd' %(name,name))
      os.system(cmd_str)
    elif tool == 'asimut' : runpat ( self._name, name, '-l 1 -p 100 -zerodelay -bdd' )
    else                : raise Exception ( 'not implemented yet' )

  ##### TestBench : in order to create testbench #####
  def Testbench ( self ) :
    netlistFormat = Cfg.getParamString('stratus1.format').asString()

    import stimuli
    stim = stimuli.Stimuli(self)
    if netlistFormat == 'vhd' :
      # Create stimuli input file
      stim.create_stimuli_text_file()

      # Create testbench
      stim.create_testbench(delay = 20, unit = 'ns', debug = False, downto = True, logic = False)

      # Create testbench and block instance
      stim.create_run(debug = False, downto = True)
    elif netlistFormat == 'vst' :
      stim.create_pat_file()
    else :
      raise Exception('Testbench not yet implemented for format %s' % netlistFormat)

  ##### Create a stratus file given the database #####
  def exportStratus ( self, fileName ) :

    ## Cat formating ##
    def catName ( net, tableau ) :
       
      name = "Cat ( "
      paire = tableau[-1]
      if paire :
        net1  = paire[0]
        bit1  = str(paire[1])
      else :
        net1  = net
        bit1 = str(len(tableau)-1)

      for i in range ( len(tableau)-2, -1, -1 ) :
        change = False
        if tableau[i] and not tableau[i+1] :
          change = True
        elif not tableau[i] and tableau[i+1] :
          change = True
        elif tableau[i] and tableau[i+1] :
          if tableau[i][0] != tableau[i+1][0] :
            change = True

        if change :
          if tableau[i+1] : bit2 = str(tableau[i+1][1])
          else            : bit2 = str(i+1)
        
          name += "self." 
          if int(bit2) == int(net1._ind) and int(bit1) == (net1._arity-1) :
            name += net1._name
          else :
            name += net1._name
            name += "["
            name += bit1
            name += ":"
            name += bit2
            name += "]"
            
          name += ", "
    
          if tableau[i] : net1 = tableau[i][0]
          else          : net1 = net

          if tableau[i] : bit1 = str(tableau[i][1])
          else          : bit1 = str(i)
          
      if tableau[0] :
        bit2 = str(tableau[0][1])
      else :
        bit2 = "0"
        
      name += "self." 
      if int(bit2) == int(net1._ind) and int(bit1) == (net1._arity-1) :
        name += net1._name
      else :
        name += net1._name
        name += "["
        name += bit1
        name += ":"
        name += bit2
        name += "]"        
      
      name += " )"
        
      return name
   
    #########################   
    nom = str(self.__class__)
    chaine = re.search ( "[^\.]*\.(.*)", nom )
    nom = chaine.group(1)

    if not fileName : fileName = nom + "_created"
    fileName += ".py"
    
    file = open ( fileName, "w+" )
    
    file.write ( "#!/usr/bin/env python3\n\n" )
    file.write ( "from stratus import *\n\n" )

    file.write ( "class %s ( Model ) :\n\n" % nom )

    ##### Interface #####
    file.write ( "  def Interface ( self ) :\n\n" )
        
    for net in self._st_ports :
      if   net._direct == "IN"  : classe = "SignalIn"
      elif net._direct == "OUT" : classe = "SignalOut"
      elif net._direct == "INOUT" : classe = "SignalInOut"

      file.write ( "    self.%s = %s ( \"%s\", %d )\n" % ( net._name, classe, net._name, net._arity ) )
    file.write ( "\n" )

    for ck in self._st_cks :
      if ck._ext:
        file.write ( "    self.%s = CkIn ( \"%s\" )\n" % (ck._name, ck._name) )

    file.write ( "    self.vdd = VddIn ( \"vdd\" )\n" )
    file.write ( "    self.vss = VssIn ( \"vss\" )\n" )
    file.write ( "\n" )
      
    ##### Netlist #####
    file.write ( "  def Netlist ( self ) :\n\n" )

    # Signals
    for net in self._st_sigs :
      if net._to_merge :
        towrite = False
        for paire in net._to_merge :
          if paire == 0 : towrite = True
        if not towrite : continue
      file.write ( "    self.%s = Signal ( \"%s\", %d )\n" % ( net._name, net._name, net._arity ) )
    file.write ( "\n" )

    # Generate
    for cell in CELLS[0]._underCells :
      if CELLS[0]._underCells[cell] == 1 :
        classe = str(cell.__class__)
        nom    = cell._name
        param  = cell._param
    
        # Gestion of libraries with specific treatment
        chaine = re.search ( "(.*)\.([^\.]*)", classe )
        if chaine :
          fichier = chaine.group(1)
          if re.search ( "dpgen_", fichier ) or re.search ( "st_", fichier ) :
            classe = chaine.group(2)
          # Special treatment for virtual library
          if classe == "Bool" : 
            nom    = cell._name
            classe = cell._param['model']
            param  = cell._param
            del param['model']
            del param['realModel']

        classe = string.upper(classe[0]) + classe[1:] #FIXME !!!
        if re.search ( "(.*)\.([^\.]*)", classe ) : # ArithLib generator
           classe = string.lower(classe[0]) + classe[1:]

        file.write ( "    Generate ( \"%s\", \"%s\", param = %s )\n" % ( classe, nom, param ) )    
    file.write ( "\n" )

    # Instances
    for inst in self._st_insts :
      #chaine = re.search ( "([^_]*)_(.*)", inst._name ) #XTOF FIXME !!!
      #instName = chaine.group(1) #XTOF FIXME !!
      file.write ( "    self.%s = Inst ( \"%s\"\n" % ( inst._name, inst._model ) )
      file.write ( "                          , \"%s\"\n" % inst._name )
      file.write ( "                          , map = { " )

      # Map
      for pin in inst._map :
        toto = False
        tata = False
      
        netInMap = inst._map[pin]
        nom = "self." + netInMap._name
       
        # <=
        if netInMap._to_merge :
          oneperone = True
          for i in range ( len ( netInMap._to_merge ) ) :
            paire = netInMap._to_merge[i]
            
            if paire :
              net = paire[0]
              if i > 0 :
                if netInMap._to_merge[i-1] != 0 :
                  netbefore = netInMap._to_merge[i-1][0]
                else :
                  oneperone = False
                  break
                if net != netbefore :
                  oneperone = False
                  break
                if paire[1] != i :
                  oneperone = False
                  break
                  
            else :
              toto = True
              oneperone = False
              break

          # "easy" case : 1 per 1 corresponding
          if oneperone :
            netInMap = netInMap._to_merge[0][0]
            nom = "self." + netInMap._name

          # else : Cat of the different signals
          else :
            nom = catName ( netInMap, netInMap._to_merge )

        # Cat
        if netInMap._to_cat :
          tata = True
          nom = catName ( netInMap, netInMap._to_cat )
            
        if toto and tata : print( "Attention est ce un cas bien gere ???" )

        if pin == list(inst._map.keys()[0]) : file.write ( "\"%s\" : %s\n" % ( pin, nom ) )
        else                                : file.write ( "                                  , \"%s\" : %s\n" % ( pin, nom ) )

      file.write ( "                                  }\n" )
      file.write ( "                          )\n\n" )
   
    ##### End ##### 
    file.close ()

  ##### Export the given database in the given format
  def export ( self, format):
    from st_export import write

    if format not in ['vhd','vlog','json']:
      raise "Unrecognized format %s" %(format)
    write(self,format)
    
  ##### Create a IEEE VHDL file given the database #####
  def exportVHD ( self ) :

    file = open ( self._name + '.vhd', "w+" )
    
    file.write ( "--\n" )
    file.write ( "-- Generated by VHDL export\n" )
    file.write ( "--\n" )
    file.write ( "library ieee;\n" )
    file.write ( "use ieee.std_logic_1164.all;\n\n" )
    
    if 'realModel' in self._param :
      file.write ( "library sxlib;\n" )
      file.write ( "use sxlib.all;\n\n" )
     

    ##### Entity #####
    file.write ( "entity %s is\n" % self._name )
    file.write ( "  port(\n" )
        
    strPorts = ""
    for net in self._st_ports :
      if net._arity == 1 :
        strPorts += "    %s : %s std_logic;\n" % ( net._name, net._direct )
      else :
        strPorts += "    %s : %s std_logic_vector(%d downto %d);\n" % ( net._name, net._direct, net._arity-1+net._ind, net._ind )

    for ck in self._st_cks :
      if ck._ext:
        strPorts += "    %s : IN std_logic;\n" % ( ck._name )

    file.write(strPorts[:-2])
    file.write ( "\n  );\n" )
    file.write ( "end %s ;\n\n" % self._name )
      
    ##### Architecture #####
    file.write ( "architecture structural of %s is\n" % self._name )


    # Components
    if 'realModel' in self._param :
      cellList = [self]
    else :
      cellList = self._underCells
    for cell in cellList :
      if 'realModel' in self._param :
        nom = cell._param['realModel']
      else :
        nom = cell._name
      #classe = str(cell.__class__)
      #nom    = cell._name
      #param  = cell._param
      #print( "nom " + nom )
      #print( "classe " + classe )
    
      ## Gestion of libraries with specific treatment
      #chaine = re.search ( "(.*)\.([^\.]*)", classe )
      #if chaine :
      #  fichier = chaine.group(1)
      #  if re.search ( "dpgen_", fichier ) or re.search ( "st_", fichier ) :
      #    classe = chaine.group(2)
      #  # Special treatment for virtual library
      #  if classe == "Bool" : 
      #    #nom    = cell._name
      #    nom    = cell._param['realModel']
      #    classe = cell._param['model']
      #    param  = cell._param
      #    #del param['model']
      #    #del param['realModel']

      #classe = string.upper(classe[0]) + classe[1:] #FIXME !!!
      #if re.search ( "(.*)\.([^\.]*)", classe ) : # ArithLib generator
      #   classe = string.lower(classe[0]) + classe[1:]

      file.write ( "  component %s\n" % nom )    
      file.write ( "    port(\n" )    

      # Ports
      strPorts = ""
      for net in cell._st_ports :
        if net._arity == 1 :
          strPorts += "      %s : %s std_logic;\n" % ( net._name, net._direct )
        else :
          strPorts += "      %s : %s std_logic_vector(%d downto %d);\n" % ( net._name, net._direct, net._arity-1+net._ind, net._ind )

      for ck in cell._st_cks :
        if ck._ext:
          strPorts += "      %s : IN std_logic;\n" % ( ck._name )

      file.write(strPorts[:-2])

      file.write ( "\n    );\n" )    
      file.write ( "  end component;\n\n")    

    # Signals
    for net in self._st_sigs :
      if net._to_merge :
        towrite = False
        for paire in net._to_merge :
          if paire == 0 : towrite = True
        if not towrite : continue
      if net._arity == 1 :
        file.write ( "  signal %s : std_logic;\n" % net._name )
      else :
        file.write ( "  signal %s : std_logic_vector(%d downto %d);\n" % ( net._name, net._arity-1+net._ind, net._ind ) )

    # Instances
    file.write ( "\nbegin\n" )
    for inst in self._st_insts :
      #chaine = re.search ( "([^_]*)_(.*)", inst._name ) #XTOF FIXME !!!
      #instName = chaine.group(1) #XTOF FIXME !!
      file.write ( "  %s : %s\n" % ( inst._name, inst._model ) )
      file.write ( "    port map(\n" )

      # Map
      strMap = ""
      for pin in inst._map :
        toto = False
        tata = False
      
        netInMap = inst._map[pin]
        if netInMap._real_net :
          nom = netInMap._real_net._name
        else :
          nom = netInMap._name
       
        # <=
        if netInMap._to_merge :
          oneperone = True
          for i in range ( len ( netInMap._to_merge ) ) :
            paire = netInMap._to_merge[i]
            
            if paire :
              net = paire[0]
              if i > 0 :
                if netInMap._to_merge[i-1] != 0 :
                  netbefore = netInMap._to_merge[i-1][0]
                else :
                  oneperone = False
                  break
                if net != netbefore :
                  oneperone = False
                  break
                if paire[1] != i :
                  oneperone = False
                  break
                  
            else :
              toto = True
              oneperone = False
              break

          # "easy" case : 1 per 1 corresponding
          if oneperone :
            netInMap = netInMap._to_merge[0][0]
            nom = netInMap._name

          # else : Cat of the different signals
          else :
            nom = catName ( netInMap, netInMap._to_merge )

        # Cat
        if netInMap._to_cat :
          tata = True
          nom = catName ( netInMap, netInMap._to_cat )
            
        if toto and tata : print( "Attention est ce un cas bien gere ???" )

        if netInMap not in self._st_vdds + self._st_vsss :
          if pin == list(inst._map.keys())[0] : strMap += "      %s => %s,\n" % ( pin, nom )
          else                                : strMap += "      %s => %s,\n" % ( pin, nom )

      file.write(strMap[:-2] + '\n')
      file.write ( "    );\n\n" )
   
    ##### End ##### 
    file.write('end structural;')
    file.close ()

    
  #### Quit : to quit the current cell without saving #####
  def Quit ( self ) :
    global CELLS
        
    CELLS.pop ()
    
  #########################################
  ##### GetModelName : Class's method #####
  #########################################
  def GetModelName ( param ) :
    return None
  GetModelName = staticmethod ( GetModelName )
  
  #########################################
  ##### GetParam : Class's method #####
  #########################################
  def GetParam () :
    return None
  GetParam = staticmethod ( GetParam )

  ###############
  ##### Set #####
  ###############
  def SetAnd   ( self, model ) : self._and   = model
  def SetOr    ( self, model ) : self._or    = model
  def SetXor   ( self, model ) : self._xor   = model
  def SetNot   ( self, model ) : self._not   = model
    
  def SetBuff  ( self, model ) : self._buff  = model
  def SetReg   ( self, model ) : self._reg   = model
  def SetMux   ( self, model ) : self._mux   = model
  def SetShift ( self, model ) : self._shift = model
  def SetComp  ( self, model ) : self._comp  = model
       
  def SetAdd   ( self, model ) : self._add   = model
  def SetMult  ( self, model ) : self._mult  = model
  def SetDiv   ( self, model ) : self._div   = model

  def SetSigned ( self, model ) : self._signed = model
  def SetExtended ( self, model ) : self._extended = model

  #############################
  ##### Creation of model #####
  #############################
  def ModelCreation ( self, modele, modele2, args, hierarchy, realModele = None, inOut = None ) :
    global CELLS

    from st_parser     import BVg
  
    ## Virtual library ##
    from st_bool       import Bool
  
    ## Stratus generators ##
    from st_mux        import Smux
    from st_shifter    import Shifter
    from st_comp       import Comp
    from st_const      import Constant
    from st_extend     import Extend
    
    from st_slansky    import Slansky
    from st_slansky    import Sub
    from st_mult       import Multiplier
  
    ## Genlib ##
    from dpgen_Shifter import DpgenShifter
    from dpgen_Shrot   import DpgenShrot
    from dpgen_Nul     import DpgenNul
    from dpgen_RAM     import DpgenRam
    from dpgen_ROM2    import DpgenRom2
    from dpgen_ROM4    import DpgenRom4
    from dpgen_ADSB2F  import DpgenAdsb2f
    from dpgen_RF1     import DpgenRf1, DpgenRf1d, DpgenRf1r0, DpgenRf1dr0, DpgenFifo
    from dpgen_RF2     import DpgenRf2, DpgenRf2d, DpgenRf2r0, DpgenRf2dr0
    from dpgen_Regular import DpgenAnd2, DpgenAnd3, DpgenAnd4, DpgenBuff, DpgenBuse, DpgenConst, DpgenDff, DpgenDfft, DpgenInv, DpgenMux2, DpgenNand2, DpgenNand2mask, DpgenNand3, DpgenNand4, DpgenNbuse, DpgenNmux2, DpgenNor2, DpgenNor2mask, DpgenNor3, DpgenNor4, DpgenOr2, DpgenOr3, DpgenOr4, DpgenSff, DpgenSfft, DpgenXnor2, DpgenXnor2mask, DpgenXor2

    from util_Gen      import F_MSB_FIRST
    
    if not isinstance(args,dict):
      err = "\n[Stratus ERROR] Inst : instanciation of a user's defined generator. The methods' arguments must be dictionnaries.\n"
      raise Exception ( err )
      
    ##### Creation of the instance #####
#   args['flags'] = F_MSB_FIRST # When vst driver permits to do F_LSB_FIRST or F_LSB_FIRST TODO
#   args['flags'] = 0
    
    ## Virtual library ## 
    if modele in BVg :
      args['model']     = modele.lower()
      args['realModel'] = realModele
  
      instCell = Bool ( modele2, args, inOut )
      
    ## Generator ##
    elif re.search ( "\.", modele ) :
      chaine    = re.search ( "(.*)\.([^\.]*)$", modele )
  
      ficName   = chaine.group(1)
      className = chaine.group(2)

      # Roselyne : pour prendre en compte les modeles recursifs
      #import sys
      moduleClass = __import__ ( ficName, globals(), locals(), [className] )
      modeleClass =   getattr  ( moduleClass, className )
      #try :
      #   modeleClass =   getattr  ( sys.modules['__main__'], className )
      #except AttributeError :
      #   moduleClass = __import__ ( ficName, globals(), locals(), [className] )
      #   modeleClass =   getattr  ( moduleClass, className )
      # fin Roselyne
      
      instCell = modeleClass ( modele2, args )
      
    else :
      instCell = eval ( "%s ( \"%s\", %s )" % ( modele, modele2, str(args) ) )
  
    ## MAJ of the hierarchy ##
    instCell._hierarchy = hierarchy

    ##### Methods #####
    if ( self._hur_cell ) and ( not instCell._hur_cell ) : instCell.HurricanePlug()

    instCell.Interface()
    instCell.Netlist()

    if ( 'clean' in args ) and ( args['clean'] != False  ) :
      if 'interactive' not in args : args['interactive'] = False
      instCell.Clean ( args['interactive'], args['clean'] )
    
    if ( 'behavioral' in args ) and ( args['behavioral'] == True  ) :
      instCell.Vbe()
    
    if instCell._hur_cell :
      v = 0
      if ( 'physical' in args ) and ( args['physical'] == True ) :
        instCell.Layout()
        v = CRL.Catalog.State.Physical
      instCell.Save( v|CRL.Catalog.State.Logical )

    else :
      instCell.Quit()

    if CELLS[0] != instCell : CELLS[0]._underCells[instCell] = hierarchy

    return instCell

  #################################################################
  ##### HurricanePlug : in order to create the hurricane cell #####
  #################################################################
  def HurricanePlug ( self ) :
    global FRAMEWORK, CELLS

    if self._hur_cell :
      print( "[Stratus Warning] : The stratus cell already exists." )
      return
    
    self._hur_plug = True

    ##### Creation of all the under cells #####
    # Initialisation of the max depth
    max_depth = 0
    for keyCell in self._underCells :
      if self._underCells[keyCell] > max_depth : max_depth = self._underCells[keyCell]
    # Creation from the leaf to the top cell 
    while max_depth :
      for keyCell in self._underCells :
        if self._underCells[keyCell] == max_depth :
          if not FRAMEWORK.getCell ( keyCell._name, CRL.Catalog.State.Views ) :
            CELLS.append ( keyCell )
            keyCell.CellHurCreation()
      max_depth -= 1

    # Creation of the top cell
    CELLS.append ( self )
    self.CellHurCreation()
    
  ##### CellHurCreation #####
  def CellHurCreation ( self ) :
    global FRAMEWORK, CELLS
      
    # The cell
    hurCell = FRAMEWORK.createCell ( self._name )
    #hurCell.setTerminal ( 0 )
    
    self._hur_cell = hurCell

    MODELMAP [ str ( self._hur_cell ) ] = self
    
    # The nets
    for net in self._st_ports : net.create_hur_net()
    for net in self._st_sigs  : net.create_hur_net()

    
    for net in self._st_cks   : net.create_hur_net()
    
    for net in self._st_vdds  : net.create_hur_net()
    for net in self._st_vsss  : net.create_hur_net()
   
    # The instances and the connection
    for inst in self._st_insts : inst.create_hur_inst ( inst._model )
    
    # Merge
    for net in self._st_merge : net.hur_merge()

    if ( self != CELLS[0] ) and ( self._st_insts ) : # Layout done if HurricanePlug after Netlist method
     #self.Layout() # FIXME trouver une raison de le faire ou pas : instances placees ou pas ... HurricanePlug ne doit plus marcher avec placement A VERIFIER 14/01/08
      if self._st_insts[0]._hur_instance.getPlacementStatus() : self.Save ( CRL.Catalog.State.Physical )
      else                                                    : self.Save ( CRL.Catalog.State.Logical )
    else :
      self.Quit()

  ####################
  ##### Clean Up #####
  ####################
  def Clean ( self, interactive = False, tab_name_net = [] ) :
    
    # Hurricane cell
    hurricane_cell = self._hur_cell
   
    # Useful for print 
    TAB_NETS     = []
    TAB_INSTS    = []
    cpt_inst_del = 0
    cpt_net_del  = 0
    
    ##### Tabs initialisation #####
    net_entree  = []
    net_sortie  = []
    net_interne = []

    for net in hurricane_cell.getNets():
        if   net.getDirection() == DirectionUNDEFINED : net_interne.append ( net )
        elif net.getDirection() == DirectionIN        : net_entree.append  ( net )
        elif net.getDirection() == DirectionOUT       : net_sortie.append  ( net )

    ##### File initialisation #####
    file = []
    
    # Output nets put in the file if there are given as arguments
    if tab_name_net != True :
      for net_name in tab_name_net :
        for net in net_sortie :
          if net_name == str ( net.getName() ) :
            file.insert ( 0, net )
            if interactive : print( "Output Net", net, "has to be erased, it is put in the fifo." )

    ## Internal nets ##
    # Number of plugs of each net :
    # if equal to 0 : del net
    # if equal to 1 : put net in the file
    for net in net_interne :
      nb_plugs = self.count_plugs ( net )
    
      if nb_plugs == 0 :
        if interactive : print( "* One net suppressed (a) :", net )
        TAB_NETS.append ( net.getName () )
        cpt_net_del += 1
        
        net.Delete()
        
      elif nb_plugs == 1 :
        if net.getPlugs().__next__().getMasterNet().getDirection() == DirectionOUT: # output of an instance
          if interactive : print( "* One net put in the fifo :", net )
          file.insert ( 0, net )
    
    ## Ouput nets ##
    # Number of plugs of each net :
    # if equal to 0 : del net
    for net in net_sortie + net_entree :
      cpt_plugs_sor = self.count_plugs ( net )    
      if cpt_plugs_sor == 0 :
        print( "[Stratus Warning] Clean : Interface of", self._name, "changed, net :", net, "is suppressed")
        TAB_NETS.append ( net.getName() )
        cpt_net_del += 1
        
        net.Delete()
    
    ##### Algorithm #####
    while len ( file ) > 0 :
      net_file = file.pop()
      plug     = net_file.getPlugs().__next__()
      inst     = plug.getInstance()
    
      # input nets of the instance
      plugs_inst      = inst.getPlugs()
      net_entree_inst = [] 
      net_sortie_inst = [] 
      cpt_sortie      = 0
      
      for plug_de_inst in plugs_inst:
        # compute the number of outputs of the instance
        if plug_de_inst.getMasterNet().getDirection() == DirectionOUT :
          cpt_sortie += 1 
          # output nets of the instance put in tab, except for the net being worked on
          if net_file !=  plug_de_inst.getNet() : net_sortie_inst.append ( plug_de_inst.getNet() )

        if plug_de_inst.getMasterNet().getDirection() == DirectionIN :
          # ignore vdd and vss
          type = plug_de_inst.getNet().getType()
          if type not in ( Net.Type.POWER, Net.Type.GROUND ) : net_entree_inst.append ( plug_de_inst.getNet() )

      ### Deletion of te instance ###
      # If the instance has only one output
      if cpt_sortie == 1 :
        if interactive : print( "* One net suppressed (b) :", net_file )
        TAB_NETS.append ( net_file.getName() )
        cpt_net_del += 1
        
        net_file.Delete()
        
        if interactive : print( "* One instance suppressed (a) :", inst, inst )
        TAB_INSTS.append(inst.getName())
        cpt_inst_del += 1
        
        UpdateSession.open()
        inst.Delete()
        UpdateSession.close()
        
        for net_ent in net_entree_inst :
          cpt_plugs_in = self.count_plugs ( net_ent )   
          if cpt_plugs_in == 0 : 
            if net_ent in net_entree :
              print( "[Stratus Warning] Clean : Interface of", self._name, "changed, net :", net_ent, "is suppressed" )
              
              TAB_NETS.append ( net_ent.getName() )
              cpt_net_del += 1
              
              net_ent.Delete()
            else :
              if interactive : print( "* One net suppressed (c) : ", net_ent )
              
              TAB_NETS.append ( net_ent.getName() )
              cpt_net_del += 1
              
              net_ent.Delete()
              
          elif cpt_plugs_in == 1 :
            if net_ent.getPlugs().__next__().getMasterNet().getDirection() ==  DirectionOUT : # is an output net of another instance
              if interactive : print( "* One net put in the fifo :", net_ent )
              file.insert ( 0, net_ent )
    
      # If the instance has more than one output
      elif cpt_sortie >= 2 :
        connect = False
        for net_sor in net_sortie_inst:
          cpt_sor = self.count_plugs ( net_sor )
          if cpt_sor >= 2 :
            connect = True # at least one output connected
            break
          elif cpt_sor == 1 :
            if net_sor in net_sortie :
              connect = True # at least one output connected
              break
              
        if not ( connect ) :
          if interactive : print( "* One net suppressed (d) :", net_file )

          TAB_NETS.append ( net_file.getName() )
          cpt_net_del += 1
          
          net_file.Delete()
          
          for net_sor in net_sortie_inst :
            if net_sor in file : file.remove ( net_sor )
    
            if interactive : print( "* One net suppressed (e) :", net_sor )
            TAB_NETS.append ( net_sor.getName() )
            cpt_net_del += 1
            
            net_sor.Delete()
          
          if interactive : print( "* One instance suppressed (b) :", inst )
          TAB_INSTS.append ( inst.getName() )
          cpt_inst_del += 1
          
          UpdateSession.open()
          inst.Delete()
          UpdateSession.close()

          for net_ent in net_entree_inst :
            cpt_plugs_in = self.count_plugs ( net_ent )
    
            if cpt_plugs_in == 0 :
              if net_ent in net_entree :
                print( "[Stratus Warning] Clean : Interface of", self._name, "changed, net :", net_ent, "is suppressed" )
                
                TAB_NETS.append ( net_ent.getName() )
                cpt_net_del += 1
                
                net_ent.Delete()
                
              else :
                if interactive : print( "* One net suppressed (f) :", net_ent )
                
                TAB_NETS.append ( net_ent.getName() )
                cpt_net_del += 1
                
                net_ent.Delete()

            elif cpt_plugs_in == 1 :
              if net_ent.getPlugs().__next__().getMasterNet().getDirection() == DirectionOUT: # in an output net of another instance
                if interactive : print( "* One net net put in the fifo :", net_ent )
                file.insert ( 0, net_ent )
                    
        else :
          if interactive : print( "The net", net_file, "can not be delayed, it may be delayed later" )
    
      else :
        print( "[Warning] Pb in Clean." )
   
    if interactive : 
      print( "" )
      print( "* Number of net suppressed :",       cpt_net_del )
      print( "* List of these nets :",             TAB_NETS )
      print( "" )
      print( "* Number of instances suppressed :", cpt_inst_del )
      print( "* List of these instance :",         TAB_INSTS )
      print( "" )

  ###############################
  def count_plugs ( self, net ) :
    return len(net.getPlugs())


  #############################################################################
  ############################## Place and route ##############################
  #############################################################################
 #def getCore ( self ) :
 #  '''This function returns the instance "core" : the only one which is not a pad'''

 #  from placeandroute import isPad

 #  cores = []
 #  for instance in self._hur_cell.getInstances():
 #    if not isPad ( instance ):
 #      cores.append ( instance )

 #  if   len(cores) == 0 : err = "\n[Stratus ERROR] getCore : No core found.\n"
 #  elif len(cores)  > 1 : err = "\n[Stratus ERROR] getCore : More than one core found.\n"
 #  if len(cores) != 1 : raise Exception ( err )

 #  return cores[0]

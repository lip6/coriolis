
import os
import re

from   Hurricane import *
from   helpers   import ErrorMessage
import CRL
import Mauka

#import Knik
#import Kite
#import Zephyr
#import Lightning
#import Zephiros

#from Anemometer import *
#from Metisse    import *

#from Mistral    import *
#from Nimbus     import *

from math import *


missingCORIOLIS_TOP = "Missing environment variable CORIOLIS_TOP"

global POWER, OCCUPIED, FREE
POWER    = 2
OCCUPIED = 1
FREE     = 0

global PITCH, SLICE
PITCH = 5
SLICE = 50

global RING_WIDTH
RING_WIDTH = 12

global RING_INTERVAL
RING_INTERVAL =  3

global standard_instances_masque

global standard_instances_list
standard_instances_list = []

global pad_north, pad_south, pad_east, pad_west
pad_north = []
pad_south = []
pad_east  = []
pad_west  = []

global nb_alims_verticales
nb_alims_verticales = 0

global nb_vdd_pins, nb_vss_pins
nb_vdd_pins = 0
nb_vss_pins = 0

global ck_contact_list_to_create
ck_contact_list_to_create = []


def getNetFromPlug ( plug ):
  net = plug.getNet()
  if net: return net

  masterNet = plug.getMasterNet()
  if masterNet.getName() in [ 'vddi', 'vssi', 'vdde', 'vsse' ]:
    cell = plug.getCell()
    net  = cell.getNet(masterNet.getName())

  return net

##################
## PlaceCentric ##
##################
def pyPlaceCentric ( cell, instance ) :
  '''This function places an instance in the middle of the abutment box of a cell'''
  
  UpdateSession.open()
  
  if not instance :
    raise ErrorMessage(2,"PlaceCentric: the instance does not exist.")

  w = cell.getAbutmentBox().getWidth()
  h = cell.getAbutmentBox().getHeight()
  
  if ( w < instance.getAbutmentBox().getWidth() ) or ( h < instance.getAbutmentBox().getHeight() ) : 
    raise ErrorMessage(2,"PlaceCentric : the instance's size is greater than this model.")

  XCenter  = cell.getAbutmentBox().getXCenter()
  YCenter  = cell.getAbutmentBox().getYCenter()
    
  New_Xmin = XCenter - instance.getAbutmentBox().getWidth()  / 2
  New_Ymin = YCenter - instance.getAbutmentBox().getHeight() / 2

  # One must place the cell on a pitch grid
  New_Xmin = New_Xmin - ( New_Xmin % DbU_lambda(PITCH) )
  New_Ymin = New_Ymin - ( New_Ymin % DbU_lambda(PITCH) )
             
  Tx       = New_Xmin - instance.getAbutmentBox().getXMin() 
  Ty       = New_Ymin - instance.getAbutmentBox().getYMin() 
  
  instance.setTransformation ( Transformation ( Tx, Ty ).getTransformation ( instance.getTransformation() ) )  
  instance.setPlacementStatus ( PlacementStatusPLACED )
    
  UpdateSession.close()
    
#######################
##     PlaceGlue     ##
#######################
def pyPlaceGlue ( cell ) :
  '''This function places the glue (unplaced leaf instances) in current cell.
     The placement will be made in the region given by the core argument:
       - if core is None, the placement region will be the cell abox.
       - if core is a cell sub-instance, the placement region will be the sub-instance abox.'''    

  mauka = Mauka.MaukaEngine.create ( cell )
  mauka.run ()
  temporarySave ()

       
#############
## RouteCk ##
#############
def pyRouteCk ( cell, netCk ) :
  '''This function route signel Ck to standard cells'''

  global pad_north, pad_south, pad_east, pad_west

  UpdateSession.open()
   
  # Error if Pads have not already been placed
  if pad_north == [] : raise ErrorMessage(2,"RouteCk : Pads in the north haven't been placed.")
  if pad_south == [] : raise ErrorMessage(2,"RouteCk : Pads in the south haven't been placed.")
  if pad_east  == [] : raise ErrorMessage(2,"RouteCk : Pads in the east haven't been placed.")
  if pad_west  == [] : raise ErrorMessage(2,"RouteCk : Pads in the west haven't been placed.")
 
  # pad_list contains all pads
  pad_list = []
  pad_list.extend ( pad_north ) 
  pad_list.extend ( pad_south ) 
  pad_list.extend ( pad_east  ) 
  pad_list.extend ( pad_west  )
    
  net_list = dict()

  for ins in pad_list :
    # For each pad that is a clock pad -> adding corresponding net in net_list if not already existing
    if isInternalClockPad ( ins ) or isExternalClockPad ( ins ):
      clockPlug = None
      for plug in ins.getPlugs():
        if plug.getNet() == netCk :
          clockPlug = plug
          break

      if clockPlug and not net_list.has_key ( str ( clockPlug.getNet().getName() ) ) : net_list[str ( clockPlug.getNet().getName() )] = ( clockPlug.getNet(), cell ) 
  
  map ( createGrid, net_list.values() )
     
  UpdateSession.close()

######################
## AlimVerticalRail ##
######################
def pyAlimVerticalRail ( cell, xcoord ) :
  '''x is in pitch, it is where the vertical alimentation call back are placed'''

  print 'pyAlimVerticalRail'

  global PITCH, SLICE
  global standard_instances_list, nb_alims_verticales, nb_lignes, standard_instances_masque
  global nb_vdd_pins, nb_vss_pins
  
  UpdateSession.open()
  
  box = cell.getAbutmentBox()  
   
  # Error message if wrong abutment box 
  if ( box.getXMin() == box.getXMax() ) or ( box.getYMin() == box.getYMax() ) :
    message = "AlimVerticalRail : Can't place the rail vertical in the abutment box of model %s." %str(cell.getName()) \
        + "     The abutment box doesn't exist !" \
        + "     Maybe you should use DefAb function or ResizeAb function to define un box before the placement of Rail Vertical."
    raise ErrorMessage(2,message)

  # Check the value of x
  nb_col = cell.getAbutmentBox().getWidth() / DbU_lambda(PITCH)
  if ( xcoord >=  nb_col ) or ( xcoord < 0 ) : 
    print 'This is it'
    message = "AlimVerticalRail : Illegal argument x , x must be between %d and %d\n" % ( 0, nb_col )  
    raise ErrorMessage(2,message)

  # To get the informations about the placement
  reference = getStandardInstancesMasque ( cell )
  
  if not reference : 
    message = "AlimVerticalRail : No instance is placed in the model " + str(cell.getName()) + ".\n" \
        + "Please place some instances before the placement of Rail Vertical.\n"
    raise ErrorMessage(2,message)
  
  # Placement verification
  #verifyPlacement ( cell )  # NON !!
  # delete elements in standard_instances_list
  del standard_instances_list[0:]
  # get YMin of reference
  Ymin = reference[1]
  # get orientation code
  orientation = reference[2]

  nb_alims_verticales += 1
  
  # get the opposite of orientation ID=>MY MY=>ID
  if orientation in [OrientationID, OrientationMX] :
    inv_orientation = OrientationMY
    orientation     = OrientationID
  elif orientation in [OrientationMY, OrientationR2] :
    inv_orientation = OrientationID     
    orientation     = OrientationMY
  else :
    raise ErrorMessage(2,"AlimVerticalRail : Strawberry.")
    
#  print "Placement of vertical rail" 
#  print "Reference  ", reference
  
  power_cell = CRL.AllianceFramework.get().getCell ( "powmid_x0", CRL.Catalog.State.Views )
  
  Height_power = power_cell.getAbutmentBox().getHeight()
  Width_power  = power_cell.getAbutmentBox().getWidth()
    
  nb_slices    = Height_power / DbU_lambda(SLICE)
  nb_pitchs    = Width_power  / DbU_lambda(PITCH)
 
  # skim through column of index x
  for i in range ( nb_lignes ) :
    espace_enough = True
    
    # check the space left
    for j in range ( nb_slices ) :
      for k in range ( nb_pitchs ) :  
        if standard_instances_masque[i + j][xcoord + k] != FREE  :  
          espace_enough = False 
          break 
          
    if espace_enough :
      # avoir le nombre de slices entre la reference
      distance = Ymin / DbU_lambda(SLICE) - i 
      
      if distance < 0 : distance = -distance 
      
      name_inst = "powmid_%d_%d" % ( nb_alims_verticales , i )
      Powmid = create_inst ( "powmid_x0", name_inst, cell )
      x = xcoord * DbU_lambda(PITCH)
      y = i      * DbU_lambda(SLICE)
      if distance % 2 : my_orientation = inv_orientation
      else            : my_orientation = orientation
      place ( Powmid, int(x), int(y), my_orientation )
  
  # place pins (connectors) in metal3 on top and bottom 
  metal3 = getDataBase().getTechnology().getLayer( "METAL3" )

  powerNet = None
  for net in cell.getPowerNets():
    if powerNet:
      raise ErrorMessage(2,"AlimVerticalRail : more than 1 Power Net found !")
    powerNet = net
  if not powerNet:
    raise ErrorMessage(2,"AlimVerticalRail : no Power Net found !")

  groundNet = None
  for net in cell.getGroundNets():
    if groundNet:
      raise ErrorMessage(2,"AlimVerticalRail : more than 1 Ground Net found !")
    groundNet = net
  if not groundNet:
    raise ErrorMessage(2,"AlimVerticalRail : no Ground Net found !")
  
  pin_height = 10
  pin_width  = 10
  pin_x1     = xcoord * DbU_lambda(PITCH) + DbU_lambda(10)
  pin_x2     = xcoord * DbU_lambda(PITCH) + DbU_lambda(25)
  pin_y1     = ( nb_lignes * DbU_lambda(SLICE) ) - DbU_lambda(pin_height/4)
  pin_y2     = 0 + DbU_lambda(pin_height/4)
    
  # Top
  if standard_instances_masque[nb_lignes-1][xcoord] == FREE :
    pin_name       = str(powerNet.getName()) + "." + str(nb_vdd_pins)
    pin            = Pin ( powerNet, pin_name, 1, 2, metal3, pin_x1, pin_y1, DbU_lambda(pin_width), DbU_lambda(pin_height) )
    nb_vdd_pins   += 1 
  
    pin_name       = str(groundNet.getName()) + "." + str(nb_vss_pins)
    pin            = Pin ( groundNet, pin_name, 1, 2, metal3, pin_x2, pin_y1, DbU_lambda(pin_width), DbU_lambda(pin_height) )
    nb_vss_pins   += 1 
  
  # Bottom
  if standard_instances_masque[0][xcoord] == FREE :          
    pin_name       = str(powerNet.getName()) + "." + str(nb_vdd_pins)
    pin            = Pin ( powerNet, pin_name, 2 , 2, metal3, pin_x1, pin_y2, DbU_lambda(pin_width), DbU_lambda(pin_height) )
    nb_vdd_pins   += 1 
  
    pin_name       = str(groundNet.getName()) + "." + str(nb_vss_pins)
    pin            = Pin ( groundNet, pin_name, 2 , 2, metal3, pin_x2, pin_y2, DbU_lambda(pin_width), DbU_lambda(pin_height) )
    nb_vss_pins   += 1

  UpdateSession.close()    

########################
## AlimHorizontalRail ##
########################
def pyAlimHorizontalRail ( cell, ycoord ) :
  '''This function places the horizontal alimentation call back in Alu 4 
  1. the two pins in Alu 4 which name is VDD or VSS are placed
  2. the horizontal in Alu 4 is placed, and is locked on the two pins 
  y is in slice''' 

  global PITCH, SLICE, RING_WIDTH
  global standard_instances_list, nb_alims_verticales, nb_lignes, standard_instances_masque
  global nb_vdd_pins, nb_vss_pins  
  
  UpdateSession.open()
  
  box = cell.getAbutmentBox()
    
  # Error message if wrong abutment box
  if ( box.getXMin() == box.getXMax() ) or ( box.getYMin() == box.getYMax() ) :
    err = "\nAlimHorizontalRail :  Can't place the rail horizontal in the abutment box of model " + str(cell.getName()) \
        + "\n     The abutment box doesn't exist !" \
        + "\n     Maybe you should use DefAb function or ResizeAb function to define un box before the placement of Rail Horizontal.\n"
    raise ErrorMessage(2,err)

  # Check the value of y
  if ( ycoord >= nb_lignes ) or ( ycoord < 0 )  : 
    err = "\nAlimHorizontalRail : Illegal argument y, y must be between %d and %d\n" % ( 0, nb_lignes )
    raise ErrorMessage(2,err)

  # To get the informations about the placement
  reference = getStandardInstancesMasque ( cell )    

  if not reference : 
    err = "AlimHorizontalRail : No instance is placed in the model " + str(cell.getName()) + ".\n"
    raise ErrorMessage(2,err)
  
  # get layers     
  metal4 = getDataBase().getTechnology().getLayer ( "METAL4" )
  via1 = getDataBase().getTechnology().getLayer ( "VIA12" )
  via2 = getDataBase().getTechnology().getLayer ( "VIA23" )
  via3 = getDataBase().getTechnology().getLayer ( "VIA34" )
  
  # Know if it is vdd or vss 
  string = getVddVss ( cell, ycoord )  
  
  pin_width  = DbU_lambda(RING_WIDTH)
  pin_height = DbU_lambda(RING_WIDTH)
  pin_x1     = 0 + ( pin_height / 4 )
  pin_x2     = cell.getAbutmentBox().getWidth() - ( pin_height / 4 )
  pin_y      = ycoord * DbU_lambda(SLICE)
  
  # Create two pins
  if string == "vdd" :
    net = cell.getNet ( "vdd" )   
    
    pin_name      = str(net.getName()) + "." + str(nb_vdd_pins)
    pin1          = Pin ( net, pin_name, 4, 2, metal4, pin_x1, pin_y, pin_width, pin_height )
    nb_vdd_pins  += 1 
    
    pin_name      = str(net.getName()) + "." + str(nb_vdd_pins)
    pin2          = Pin ( net, pin_name, 3, 2, metal4, pin_x2, pin_y, pin_width, pin_height ) 
    nb_vdd_pins  += 1 
  
  else : 
    net = cell.getNet ( "vss" )   

    pin_name      = str(net.getName()) + "." + str(nb_vss_pins)
    pin1          = Pin ( net, pin_name, 4, 2, metal4, pin_x1, pin_y, pin_width, pin_height )
    nb_vss_pins  += 1 
    
    pin_name      = str(net.getName()) + "." + str(nb_vss_pins)
    pin2          = Pin ( net, pin_name, 3, 2, metal4, pin_x2, pin_y, pin_width, pin_height ) 
    nb_vss_pins  += 1 
  
  # Create horizontal rail 
  rail_horizontal = Horizontal ( pin1 , pin2 , metal4 , pin_y, pin_height, 0 , 0 )
   
  power_hur_cell  = CRL.AllianceFramework.get().getCell ( "powmid_x0", CRL.Catalog.State.Views )
  Width_power     = power_hur_cell.getAbutmentBox().getWidth()  
  nb_pitchs       = Width_power / DbU_lambda(PITCH)       # avoir l'interval du cellule power  
  
  # create contacts in via1 , via2 , et via3
  i = 0  
  while i < nb_colonnes :
   # if it is a power cell, make the via3, and pass the power cell     
   if standard_instances_masque[ycoord][i] == POWER :
     contact_side = DbU_lambda(RING_WIDTH) - DbU_lambda(1.0)
      
     if string == "vdd" : contact3 = Contact ( rail_horizontal, via3, i*DbU_lambda(PITCH) + DbU_lambda(10), 0, contact_side, DbU_lambda(2) ) 
     else               : contact3 = Contact ( rail_horizontal, via3, i*DbU_lambda(PITCH) + DbU_lambda(25), 0, contact_side, DbU_lambda(2) ) 
     i += nb_pitchs
  
   i+=1  
   
  UpdateSession.close()

####################
## AlimConnectors ##
####################
def pyAlimConnectors ( cell ) :
  '''Creates the METAL1 connectors on the east & west sides of the core''' 

  global nb_lignes, nb_vdd_pins, nb_vss_pins

  UpdateSession.open()
  
  box = cell.getAbutmentBox()

  # Error message if wrong abutment box    
  if ( box.getXMin() == box.getXMax() ) or ( box.getYMin() == box.getYMax() ) :  
    err = "\nAlimConnectors : can't place connectors. The abutment box don't exist.\n" \
        + "The abutment box don't exist !\n" \
        + "Maybe you should use DefAb function or ResizeAb function to define un box before the placement of alim connectors.\n"
    raise ErrorMessage(2,err)
    
  # avoir les infos actuelles de placement 
  getStandardInstancesMasque ( cell ) 

  if not reference : 
    err = "\nAlimConnectors : no instance placed in the model " + str(cell.getName()) + "." \
        + "Please place some instances before the placement of alim connectors.\n"
    raise ErrorMessage(2,err)
    
  metal1 = getDataBase().getTechnology().getLayer("METAL1")
  string = getVddVss ( cell, 0 )
     
  if re.search ( "vdd", string ) :
    inv_string     = "vss"
    nb_string_pins = nb_vdd_pins
    nb_inv_pins    = nb_vss_pins
  else :
    inv_string  = "vdd"
    nb_string_pins = nb_vss_pins
    nb_inv_pins    = nb_vdd_pins
  
  netPair = cell.getNet ( string )
  if not netPair:
    raise ErrorMessage(2,"AlimConnectors : can't get net %s."%string)
  
  netImpair = cell.getNet ( inv_string )
  if not netImpair:
    raise ErrorMessage(2,"AlimConnectors : can't get net %s.\n"%inv_string)
    
  for i in range ( nb_lignes + 1 ) :
    pin_width  = DbU_lambda(12)
    pin_height = DbU_lambda(12)
    pin_y      = i * DbU_lambda(SLICE)

    if i == 0 :
      pin_height = pin_height / 2
      pin_width  = pin_width / 2
      pin_y      = pin_y + ( pin_height / 2 )
    elif i == nb_lignes :
      pin_height = pin_height / 2
      pin_width  = pin_width / 2
      pin_y      = pin_y - ( pin_height / 2 )

    pin_x1     = 0 + ( pin_width / 2 )
    pin_x2     = cell.getAbutmentBox().getWidth() - ( pin_width / 2 )

    # Ligne impaire
    if i % 2 :                              
      pin_name       = str(netImpair.getName()) + "." + str(nb_inv_pins)
      pin1           = Pin ( netImpair, pin_name, 4, 2, metal1, pin_x1, pin_y, pin_width, pin_height ) 
      nb_inv_pins   += 1 

      pin_name       = str(netImpair.getName()) + "." + str(nb_inv_pins)
      pin2           = Pin ( netImpair, pin_name, 3, 2, metal1, pin_x2, pin_y, pin_width, pin_height ) 
      nb_inv_pins   += 1
      Horizontal ( pin1, pin2, metal1, pin1.getY(), pin_height )
    # Ligne paire   
    else :
      pin_name        = str(netPair.getName()) + "." + str(nb_string_pins)
      pin1            = Pin ( netPair, pin_name, 4, 2, metal1, pin_x1, pin_y, pin_width, pin_height ) 
      nb_string_pins += 1 

      pin_name        = str(netPair.getName()) + "." + str(nb_string_pins)
      pin2            = Pin ( netPair, pin_name, 3, 2, metal1, pin_x2, pin_y, pin_width, pin_height )
      nb_string_pins += 1
      Horizontal ( pin1, pin2, metal1, pin1.getY(), pin_height )
      
  if re.search ( "vdd", string ) :
    nb_vdd_pins = nb_string_pins
    nb_vss_pins = nb_inv_pins
  else :
    nb_vss_pins = nb_string_pins
    nb_vdd_pins = nb_inv_pins

  UpdateSession.close()

#######################
##    GlobalRoute    ##
#######################
#def pyGlobalRoute ( cell ):
#    """Perform global routing"""
#
#  # Initialize Nimbus Grid (50x50 lambdas).
#    Kite.setupNimbusGrid ( cell )    # ca a pas l'air de faire ce qu'il faut
#
#    # la il faudrait rajouter un petit appel a cell->FlattenNets() pour que ce soit correct !
#
#   # Global routing.
#    globalRouter = Knik.getKnik ( cell
#                                , 1      # congestion.
#                                , 2      # preCongestion.
#                                , False  # benchMode.
#                                , True   # useSegments.
#                                )
#    globalRouter.Route ()
#    globalRouter.Delete ()

#######################
##    DetailRoute    ##
#######################
#def pyDetailRoute ( cell ):
#    """Perform detailed routing"""
#
#    viewer = getCEditor ( cell )
#
#    TOOL_TOP = os.getenv ( "CORIOLIS_TOP" )
#    if not TOOL_TOP:
#        raise missingCORIOLIS_TOP
#    config = TOOL_TOP + "/share/etc/kite.conf.xml"
#
#    Kite.setDemoMode ( True )
#    Kite.ConfigurationCreate ( config )
#    detailedRouter = Kite.Create ( cell, viewer )
#    detailedRouter.setGlobalThreshold ( 9*DbU_lambda(50.0) )
#    detailedRouter.setRipupLimit      ( 10 )
#    detailedRouter.LoadGlobalRouting  ( Kite.LOAD_GR_BY_NET )
#    if viewer: viewer.Refresh ()
#
#    detailedRouter.LayerAssign ( Kite.LAYER_ASSIGN_BY_TRUNK )
#    if viewer: viewer.Refresh ()
#
#    detailedRouter.ComputeNetConstraints ()
#    if viewer: viewer.Refresh ()
#
#    detailedRouter.ComputeNetOptimals ()
#    if viewer: viewer.Refresh ()
#
#    detailedRouter.RunNegociate ()
#    if viewer: viewer.Refresh ()
#
#   # Save detailed routing.
#    detailedRouter.Delete ()
#    if viewer: viewer.Refresh ()
#
#    return

#def pyTimingStaticAnalysis ( cell ):
#    """Perfom timing static analysis"""
#
#    TOOL_TOP = os.getenv ( "CORIOLIS_TOP")
#    if not TOOL_TOP:
#        raise missingCORIOLIS_TOP
#    config = TOOL_TOP + "/share/cells/sxlib/sxlib_timing.lib"
#
#    LibertyParse ( config )
#
#    lightning = Lightning.getLightning ( cell )
#    timingStaticAnalysis = Zephyr.getZephyr ( cell )
#    #zephiros = Zephiros.getZephiros ( cell )
#
#    timingStaticAnalysis.Update()
#    #zephiros.PrintMaxDelay()
#    
#    return
    


##############
## PadNorth ##
##############
def pyPadNorth ( cell, core, args ) :
  '''This function places pads up north of the cell. It leaves an empty squarre of CARRE * CARRE, CARRE is defined in macro'''

  global pad_north

  # Sauvegarder la liste des plots
  pad_north = args

  UpdateSession.open()
  
  # avoir le largeur de l espace au nord pour placer les pads 
  north_espace_width    = cell.getAbutmentBox().getYMax() - core.getAbutmentBox().getYMax()
  north_espace_longueur = cell.getAbutmentBox().getWidth() - (2 * getPadHeight ( cell ) )     # FIXME D2 

  # avoir le nombre de pads 
  nb_pads = len ( args )
  
  # avoir le largeur total des pads 
  largeur = 0  
  for ins in args : 
    if ins.getPlacementStatus() :
      raise ErrorMessage(2,"PadNorth : the instance %s is already placed."%str(ins.getName()))

    if ins.getAbutmentBox().getHeight() >= north_espace_width :
      raise ErrorMessage(2,"PadNorth : not enough space for all pads.")
  
    largeur += ins.getAbutmentBox().getWidth() 
  
  if largeur > north_espace_longueur :
    raise ErrorMessage(2,"PadNorth : not enough space for all pads.")
  
  # calculer l interval entre les pads 
  interval = ( north_espace_longueur - largeur ) / ( nb_pads + 1 )
  
  # placer les pads  
  Xmin_cell   = cell.getAbutmentBox().getXMin()
  Ymin_cell   = cell.getAbutmentBox().getYMin()
  Height_cell = cell.getAbutmentBox().getHeight()
  
  x_init = Xmin_cell + getPadHeight ( cell ) + interval 
  
  ##### Parcours des pads #####
  for ins in args :
    pad_height = ins.getAbutmentBox().getHeight()
    pad_width  = ins.getAbutmentBox().getWidth()
  
    _y = Ymin_cell + Height_cell - pad_height # la position finale dans la cell
    _x = x_init 

    # ATTENTION LES PLOTS DOIVENT ETRE PLACES SUR UNE GRILLE DE 5 LAMBDAS !!!!!!!!!!!!!!
    _x = _x - ( _x % DbU_lambda(PITCH) )

    # ATTENTION ON NE PLACE PAS DES PLOTS D ALIM EN DEHORS DE LA COURONNE INTERNE
    if isInternalGroundPad ( ins ) or isInternalPowerPad ( ins ) \
    or isExternalGroundPad ( ins ) or isExternalPowerPad ( ins ) :
      cellAB     = cell.getAbutmentBox()
      coreAB     = core.getAbutmentBox()
      pad_height = getPadHeight ( cell )
      if _x <= ( ( cellAB.getXMin() + pad_height ) + coreAB.getXMin() ) / 2 \
      or _x >= ( ( cellAB.getXMax() - pad_height ) + coreAB.getXMax() ) / 2 :
        raise ErrorMessage(2,"PadNorth : pad %s must be closer to the center.\n"%str(ins.getName()))
 
    # x calcule pour le pad suivant
    x_init = _x + interval + pad_width
  
    # avoir x y
    box   = ins.getMasterCell().getAbutmentBox()
    _difx = box.getXMin()
    _dify = box.getYMin()
  
    ins.setTransformation ( Transformation ( _x - _difx
                                           , _y - _dify
                                           , OrientationID
                                           )
                          )
  
    ins.setPlacementStatus ( PlacementStatusFIXED )

  UpdateSession.close()
    
##############
## PadSouth ##
##############
def pyPadSouth ( cell, core, args ) :
  '''This function places pads down south of the cell. It leaves an empty squarre of CARRE * CARRE, CARRE is defined in macro'''

  global pad_south

  # Sauvegarder la liste des plots
  pad_south = args

  UpdateSession.open()
    
  # avoir le largeur du espace disponible du nord pour placer les pads 
  south_espace_width    = core.getAbutmentBox().getYMin() - cell.getAbutmentBox().getYMin()
  south_espace_longueur = cell.getAbutmentBox().getWidth() - ( 2 * getPadHeight ( cell ) )

  # avoir le nombre de pads 
  nb_pads = len ( args )
  
  # avoir le largeur total des pads 
  largeur = 0  
  for ins in args : 
    if ins.getPlacementStatus() :
      raise ErrorMessage(2,"PadSouth : the instance %s is already placed."%str(ins.getName()))
  
    if ins.getAbutmentBox().getHeight() >= south_espace_width :
      raise ErrorMessage(2,"PadSouth : not enough space for all pads.")
  
    largeur += ins.getAbutmentBox().getWidth() 
  
  if largeur > south_espace_longueur :
    raise ErrorMessage(2,"PadSouth : not enough space for all pads.")
  
  # calculer l'interval entre les pads 
  interval = ( south_espace_longueur - largeur ) / ( nb_pads + 1 ) 
  
  # placer les pads  
  Xmin_cell = cell.getAbutmentBox().getXMin()
  Ymin_cell = cell.getAbutmentBox().getYMin()
    
  x_init = Xmin_cell + getPadHeight ( cell ) + interval 
  
  ##### Parcours des pads #####
  for ins in args : 
    pad_height = ins.getAbutmentBox().getHeight()
    pad_width  = ins.getAbutmentBox().getWidth()
  
    _y = Ymin_cell # la position finale dans la cell 
    _x = x_init 
  
    # ATTENTION LES PLOTS DOIVENT ETRE PLACES SUR UNE GRILLE DE 5 LAMBDAS !!!!!!!!!!!!!!
    _x = _x - ( _x % DbU_lambda(PITCH) )

    # ATTENTION ON NE PLACE PAS DES PLOTS D ALIM EN DEHORS DE LA COURONNE INTERNE
    if isInternalGroundPad ( ins ) or isInternalPowerPad ( ins ) \
    or isExternalGroundPad ( ins ) or isExternalPowerPad ( ins ) :
      cellAB     = cell.getAbutmentBox()
      coreAB     = core.getAbutmentBox()
      pad_height = getPadHeight ( cell )
      if _x <= ( ( cellAB.getXMin() + pad_height ) + coreAB.getXMin() ) / 2 \
      or _x >= ( ( cellAB.getXMax() - pad_height ) + coreAB.getXMax() ) / 2 :
        raise ErrorMessage(2,"PadSouth : pad %s must be closer to the center."%str(ins.getName()))
 
    # x calcule pour le pad suivant
    x_init = _x + interval + pad_width
  
    # avoir x y apres orientation
    box   = ins.getMasterCell().getAbutmentBox()
    _difx = Transformation ( 0, 0, OrientationMY ).getBox ( box ).getXMin()
    _dify = Transformation ( 0, 0, OrientationMY ).getBox ( box ).getYMin() 
  
    ins.setTransformation ( Transformation ( _x - _difx
                                           , _y - _dify
                                           , OrientationMY
                                           )
                          )
  
    ins.setPlacementStatus ( PlacementStatusFIXED )  
    
  UpdateSession.close()

#############
## PadEast ##
#############
def pyPadEast ( cell, core, args ) :
  '''This function places pads at the east of the cell. It leaves an empty squarre of CARRE * CARRE, CARRE is defined in macro''' 

  global pad_east

  # Sauvegarder la liste des plots
  pad_east = args

  UpdateSession.open()
      
  # avoir le largeur du espace disponible du nord pour placer les pads 
  east_espace_width    = cell.getAbutmentBox().getXMax() - core.getAbutmentBox().getXMax()
  east_espace_longueur = cell.getAbutmentBox().getHeight() - ( 2 * getPadHeight ( cell ) ) 
  
  # avoir le nombre de pads 
  nb_pads = len ( args )
  
  # avoir le largeur total des pads 
  largeur = 0  
  for ins in args :
    if ins.getPlacementStatus() :
      raise ErrorMessage(2,"PadEast : the instance %s is already placed."%str(insGetName()))
  
    if ins.getAbutmentBox().getHeight() >= east_espace_width :
      raise ErrorMessage(2,"PadEast : not enough space for pads.")
  
    largeur += ins.getAbutmentBox().getWidth() 
  
  if largeur > east_espace_longueur :
    raise ErrorMessage(2,"PadEast : not enough space for all pads.")
  
  # calculer l'interval entre les pads 
  interval = ( east_espace_longueur - largeur ) / ( nb_pads + 1 ) 
  
  # placer les pads  
  Xmin_cell = cell.getAbutmentBox().getXMin()
  Ymin_cell = cell.getAbutmentBox().getYMin()
  Xmax_cell = cell.getAbutmentBox().getXMax()
  
  y_init = Ymin_cell + getPadHeight ( cell ) + interval 
  
  ##### Parcours des pads #####
  for ins in args : 
    pad_height = ins.getAbutmentBox().getHeight()
    pad_width  = ins.getAbutmentBox().getWidth()
  
    Xmin_pad   = ins.getMasterCell().getAbutmentBox().getXMin()
    Ymin_pad   = ins.getMasterCell().getAbutmentBox().getYMin()
  
    _x = Xmax_cell - pad_height   # la position finale dans la cell 
    _y = y_init 
  
    # ATTENTION LES PLOTS DOIVENT ETRE PLACES SUR UNE GRILLE DE 5 LAMBDAS !!!!!!!!!!!!!!
    _y = _y - ( _y % DbU_lambda(PITCH) )

    # ATTENTION ON NE PLACE PAS DES PLOTS D ALIM EN DEHORS DE LA COURONNE INTERNE
    if isInternalGroundPad ( ins ) or isInternalPowerPad ( ins ) \
    or isExternalGroundPad ( ins ) or isExternalPowerPad ( ins ) :
      cellAB     = cell.getAbutmentBox()
      coreAB     = core.getAbutmentBox()
      pad_height = getPadHeight ( cell )
      if _y <= ( ( cellAB.getYMin() + pad_height ) + coreAB.getYMin() ) / 2 \
      or _y >= ( ( cellAB.getYMax() - pad_height ) + coreAB.getYMax() ) / 2 :
        raise ErrorMessage(2,"PadEast : pad %s must be closer to the center.\n"%str(ins.getName()))
 
    # y calcule pour le pad suivant
    y_init = _y + interval + pad_width
  
    # avoir le x y apres orientation
    box   = ins.getMasterCell().getAbutmentBox()
    _difx = Transformation ( 0, 0, 7 ).getBox ( box ).getXMin()     
    _dify = Transformation ( 0, 0, 7 ).getBox ( box ).getYMin() 
  
    ins.setTransformation ( Transformation ( _x - _difx
                                           , _y - _dify
                                           , OrientationYR
                                           )
                          )
  
    ins.setPlacementStatus ( PlacementStatusFIXED )  
    
  UpdateSession.close()

#############
## PadWest ##
#############
def pyPadWest ( cell, core, args ) :
  '''This function places pads at the west of the cell. It leaves an empty squarre of CARRE * CARRE, CARRE is defined in macro''' 

  global pad_west

  # Sauvegarder la liste des plots
  pad_west = args

  UpdateSession.open()

  # avoir le largeur du espace disponible du nord pour placer les pads 
  west_espace_width    = core.getAbutmentBox().getXMin() - cell.getAbutmentBox().getXMin()  
  west_espace_longueur = cell.getAbutmentBox().getHeight() - ( 2 * getPadHeight ( cell ) )
  
  # avoir le nombre de pads 
  nb_pads = len ( args )
  
  # avoir le largeur total des pads 
  largeur = 0  
  for ins in args : 
    if ins.getPlacementStatus() :
      raise ErrorMessage(2,"PadWest : the instance %s is already placed."%str(ins.getName()))
      
    if ins.getAbutmentBox().getHeight() >= west_espace_width :
      raise ErrorMessage(2,"PadWest : not enough space for pads.")
  
    largeur += ins.getAbutmentBox().getWidth() 
  
  if largeur > west_espace_longueur :
    raise ErrorMessage(2,"PadWest : not enough space for pads.")
  
  # calculer l'interval entre les pads 
  interval = int ( ( west_espace_longueur - largeur ) / ( nb_pads + 1 ) ) 
  
  # placer les pads  
  Xmin_cell  = cell.getAbutmentBox().getXMin()  
  Ymin_cell  = cell.getAbutmentBox().getYMin()
  
  y_init = Ymin_cell + getPadHeight ( cell ) + interval
  
  ##### Parcours des pads #####
  for ins in args :
    pad_height = ins.getAbutmentBox().getHeight()  
    pad_width  = ins.getAbutmentBox().getWidth()                
  
    _x = Xmin_cell # la position finale dans la cell 
    _y = y_init 
  
    # ATTENTION LES PLOTS DOIVENT ETRE PLACES SUR UNE GRILLE DE 5 LAMBDAS !!!!!!!!!!!!!!
    _y = _y - ( _y % DbU_lambda(PITCH) )

    # ATTENTION ON NE PLACE PAS DES PLOTS D ALIM EN DEHORS DE LA COURONNE INTERNE
    if isInternalGroundPad ( ins ) or isInternalPowerPad ( ins ) \
    or isExternalGroundPad ( ins ) or isExternalPowerPad ( ins ) :
      cellAB     = cell.getAbutmentBox()
      coreAB     = core.getAbutmentBox()
      pad_height = getPadHeight ( cell )

      if _y <= ( ( cellAB.getYMin() + pad_height ) + coreAB.getYMin() ) / 2 \
      or _y >= ( ( cellAB.getYMax() - pad_height ) + coreAB.getYMax() ) / 2 :
        raise ErrorMessage(2,"PadWest : pad %s must be closer to the center.\n"%str(ins.getName()))
        
    # y calcule pour le pad suivant
    y_init = _y + interval + pad_width   
  
    # avoir x y apres orientation
    box   = ins.getMasterCell().getAbutmentBox()
    _difx = Transformation ( 0, 0, 1 ).getBox ( box ).getXMin() 
    _dify = Transformation ( 0, 0, 1 ).getBox ( box ).getYMin() 
  
    ins.setTransformation ( Transformation ( _x - _difx
                                           , _y - _dify
                                           , OrientationR1
                                           )
                          )
  
    ins.setPlacementStatus ( PlacementStatusFIXED )  
    
  UpdateSession.close()

###############
## PowerRing ##
###############
def pyPowerRing ( cell, core, n ) :
  '''This function places power rings around the core and around the plots
  n is the number of rings (n vdd, n+1 vss) 
  , Distance between rings is defined by macro DbU_lambda ( RING_INTERVAL )
  Width of rings is defined by macro DbU_lambda ( RING_WIDTH )'''

  global pad_north, pad_south, pad_east, pad_west
  global RING_INTERVAL, RING_WIDTH
  
  UpdateSession.open()
  
  if pad_north == [] : raise ErrorMessage(2,"PowerRing : Pads in the north haven't been placed")
  if pad_south == [] : raise ErrorMessage(2,"PowerRing : Pads in the south haven't been placed")
  if pad_east  == [] : raise ErrorMessage(2,"PowerRing : Pads in the east haven't been placed")
  if pad_west  == [] : raise ErrorMessage(2,"PowerRing : Pads in the west haven't been placed")
  
  #############################
  #  PARAMETRE DU PLACEMENT  ##
  #############################
  
  # le largeur du fil de routage 
  routage_segment_width = DbU_lambda(12)
  
  # la distance entre les deux centres des couronnes  
  decalage = DbU_lambda(RING_WIDTH) + DbU_lambda(RING_INTERVAL)     
  
  # Recuperer les informations sur le core
  core_width  = core.getAbutmentBox().getWidth()
  core_height = core.getAbutmentBox().getHeight()
  core_Xmin   = core.getAbutmentBox().getXMin()
  core_Ymin   = core.getAbutmentBox().getYMin()
  core_Xmax   = core.getAbutmentBox().getXMax()
  core_Ymax   = core.getAbutmentBox().getYMax()
  mips_core   = core.getMasterCell()

  # Recuperer la hauteur des plots 
  pad_height  = getPadHeight ( cell )
  
  # Recuperer les layers ( Vias , Alus )  
  db   = getDataBase()
  metal4 = db.getTechnology().getLayer ( "METAL4"  )
  metal3 = db.getTechnology().getLayer ( "METAL3"  )
  metal2 = db.getTechnology().getLayer ( "METAL2"  )
  metal1 = db.getTechnology().getLayer ( "METAL1"  )
  via1 = db.getTechnology().getLayer ( "VIA12" )
  via2 = db.getTechnology().getLayer ( "VIA23" )
  via3 = db.getTechnology().getLayer ( "VIA34" )
               
  # Recuperer les nets ( qui connectent les connectors du plot : vdde , vsse , vddi , vssi , ck  )
  instance = cell.getInstance( pad_north[0].getName() )
  model    = instance.getMasterCell()
  vddp     = instance.getPlug ( model.getNet("vdde") ).getNet()
  vssp     = instance.getPlug ( model.getNet("vsse") ).getNet()
  vdd      = instance.getPlug ( model.getNet("vddi") ).getNet()
  vss      = instance.getPlug ( model.getNet("vssi") ).getNet()
  ck       = instance.getPlug ( model.getNet("ck"  ) ).getNet()
 # If nets are globals, get them from the netlist.
  if not vddp: vddp = cell.getNet('vdde')
  if not vssp: vssp = cell.getNet('vsse')
  if not vdd:  vdd  = cell.getNet('vddi')
  if not vss:  vss  = cell.getNet('vssi')
  if not ck:   ck   = cell.getNet('cki')
                 
  # Prendre la hauteur, la longueur, Xmin et Ymin de ce modele 
  cell_height = cell.getAbutmentBox().getHeight()
  cell_width  = cell.getAbutmentBox().getWidth()
  cell_Xmin   = cell.getAbutmentBox().getXMin()
  cell_Ymin   = cell.getAbutmentBox().getYMin()
  cell_Xmax   = cell.getAbutmentBox().getXMax()
  cell_Ymax   = cell.getAbutmentBox().getYMax()
  
  # Verifier s'il y a assez de places pour les couronnes
  if ( cell_height - core_height ) < ( cell_width - core_width ) : smaller = cell_height - core_height
  else                                                           : smaller = cell_width  - core_width
  limit = DbU_lambda ( int ( DbU_getLambda ( smaller ) / 2 ) ) - pad_height
  
  if ( (2*n + 1) * DbU_lambda(RING_INTERVAL) + (2*n) * DbU_lambda(RING_WIDTH) ) >= limit :   # 2*n + 1 spaces + 2*n width
    raise ErrorMessage(2,"PowerRing : too many rings, not enough space")
  
  # la distance entre les couronnes et le core         
  marge = DbU_lambda ( int ( DbU_getLambda ( limit - (2*n + 1 ) * DbU_lambda(RING_INTERVAL) - (2*n) * DbU_lambda(RING_WIDTH) ) ) / 2 )
  
  east_power  = [] 
  west_power  = []
  north_power = []
  south_power = []
  
  searchVddVss ( cell, east_power, west_power, north_power, south_power )
  #affichePad ( cell )
  
  # Creation des listes pad_north_pin_y_vss et pad_north_pin_vdd
  pad_north_pin_x_vss = []
  pad_north_pin_x_vdd = []
  for pad_inst in pad_north :
    if isInternalGroundPad ( pad_inst ) or isInternalPowerPad ( pad_inst ) \
    or isExternalGroundPad ( pad_inst ) or isExternalPowerPad ( pad_inst ):
      transformation = pad_inst.getTransformation()
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_north_pin_x_vss.append ( [transformation.getX ( element.getX(), element.getY() ), element.getBoundingBox().getHalfWidth()] )

      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_north_pin_x_vdd.append ( [transformation.getX ( element.getX(), element.getY() ), element.getBoundingBox().getHalfWidth()] )


  # Creation des listes pad_south_pin_y_vss et pad_south_pin_vdd
  pad_south_pin_x_vss = []
  pad_south_pin_x_vdd = []
  for pad_inst in pad_south :
    if isInternalGroundPad ( pad_inst ) or isInternalPowerPad ( pad_inst ) \
    or isExternalGroundPad ( pad_inst ) or isExternalPowerPad ( pad_inst ):
      transformation = pad_inst.getTransformation()
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_south_pin_x_vss.append ( [transformation.getX ( element.getX(), element.getY() ), element.getBoundingBox().getHalfWidth()] )

      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_south_pin_x_vdd.append ( [transformation.getX ( element.getX(), element.getY() ), element.getBoundingBox().getHalfWidth()] )


  # Creation des listes pad_east_pin_y_vss et pad_east_pin_vdd
  pad_east_pin_y_vss = []
  pad_east_pin_y_vdd = []
  for pad_inst in pad_east :
    if isInternalGroundPad ( pad_inst ) or isInternalPowerPad ( pad_inst ) \
    or isExternalGroundPad ( pad_inst ) or isExternalPowerPad ( pad_inst ):
      transformation = pad_inst.getTransformation()
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_east_pin_y_vss.append ( [transformation.getY ( element.getX(), element.getY() ), element.getBoundingBox().getHalfHeight()] )

      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_east_pin_y_vdd.append ( [transformation.getY ( element.getX(), element.getY() ), element.getBoundingBox().getHalfHeight()] )
      
  # Creation des listes pad_west_pin_y_vss et pad_west_pin_vdd
  pad_west_pin_y_vss = []
  pad_west_pin_y_vdd = []
  for pad_inst in pad_west :
    if isInternalGroundPad ( pad_inst ) or isInternalPowerPad ( pad_inst ) \
    or isExternalGroundPad ( pad_inst ) or isExternalPowerPad ( pad_inst ):
      transformation = pad_inst.getTransformation()
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_west_pin_y_vss.append ( [transformation.getY ( element.getX(), element.getY() ), element.getBoundingBox().getHalfHeight()] )

      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        if re.search ( "METAL2", str ( element.getLayer() ) ) \
        and ( ( element.getY() - element.getBoundingBox().getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          pad_west_pin_y_vdd.append ( [transformation.getY ( element.getX(), element.getY() ), element.getBoundingBox().getHalfHeight()] )
      
  #####################################
  ##  PLACER LES COURONNES INTERNES  ##
  #####################################
  
  # les listes pour sauvegarder les segments des couronnes des quatres coins 
  vertical_west_vss    = [] 
  vertical_east_vss    = []
  horizontal_south_vss = []
  horizontal_north_vss = []
  
  vertical_west_vdd    = []
  vertical_east_vdd    = []
  horizontal_south_vdd = []
  horizontal_north_vdd = []
  
  init_Xmin = core_Xmin - marge - DbU_lambda(RING_WIDTH) / 2
  init_Xmin = init_Xmin - ( init_Xmin % DbU_lambda(PITCH) )
  init_Ymin = core_Ymin - marge - DbU_lambda(RING_WIDTH) / 2
  init_Ymin = init_Ymin - ( init_Ymin % DbU_lambda(PITCH) )
  init_Xmax = core_Xmax + marge + DbU_lambda(RING_WIDTH) / 2 
  init_Xmax = init_Xmax - ( init_Xmax % DbU_lambda(PITCH) )
  init_Ymax = core_Ymax + marge + DbU_lambda(RING_WIDTH) / 2 
  init_Ymax = init_Ymax - ( init_Ymax % DbU_lambda(PITCH) )

  contact_side = DbU_lambda(RING_WIDTH-1)
  
  
  for i in range ( 0, 2*n+1, 2 ) : 
    contact1 = Contact ( vss, via3, init_Xmin - decalage*i, init_Ymin - decalage*i, contact_side, contact_side )
    contact2 = Contact ( vss, via3, init_Xmin - decalage*i, init_Ymax + decalage*i, contact_side, contact_side )
    contact3 = Contact ( vss, via3, init_Xmax + decalage*i, init_Ymax + decalage*i, contact_side, contact_side )
    contact4 = Contact ( vss, via3, init_Xmax + decalage*i, init_Ymin - decalage*i, contact_side, contact_side )

    vertical_west_vss.append    ( Vertical   ( contact1, contact2, metal3, init_Xmin - decalage*i, DbU_lambda(RING_WIDTH) ) )          
    vertical_east_vss.append    ( Vertical   ( contact3, contact4, metal3, init_Xmax + decalage*i, DbU_lambda(RING_WIDTH) ) )          
    horizontal_south_vss.append ( Horizontal ( contact1, contact4, metal4, init_Ymin - decalage*i, DbU_lambda(RING_WIDTH) ) )          
    horizontal_north_vss.append ( Horizontal ( contact2, contact3, metal4, init_Ymax + decalage*i, DbU_lambda(RING_WIDTH) ) )
     
    if i != 2*n :   
     contact1 = Contact ( vdd, via3, init_Xmin - decalage* ( i + 1 ), init_Ymin - decalage*( i + 1 ) , contact_side, contact_side )
     contact2 = Contact ( vdd, via3, init_Xmin - decalage* ( i + 1 ), init_Ymax + decalage*( i + 1 ) , contact_side, contact_side )
     contact3 = Contact ( vdd, via3, init_Xmax + decalage* ( i + 1 ), init_Ymax + decalage*( i + 1 ) , contact_side, contact_side )
     contact4 = Contact ( vdd, via3, init_Xmax + decalage* ( i + 1 ), init_Ymin - decalage*( i + 1 ) , contact_side, contact_side )        
     vertical_west_vdd.append    ( Vertical   ( contact1, contact2, metal3, init_Xmin - decalage* ( i + 1 ), DbU_lambda(RING_WIDTH) ) )
     vertical_east_vdd.append    ( Vertical   ( contact3, contact4, metal3, init_Xmax + decalage* ( i + 1 ), DbU_lambda(RING_WIDTH) ) )
     horizontal_south_vdd.append ( Horizontal ( contact1, contact4, metal4, init_Ymin - decalage* ( i + 1 ), DbU_lambda(RING_WIDTH) ) )  
     horizontal_north_vdd.append ( Horizontal ( contact2, contact3, metal4, init_Ymax + decalage* ( i + 1 ), DbU_lambda(RING_WIDTH) ) )
  
  # MACRO pour les directions d'access des pins  
  UNDEFINED = 0
  NORTH     = 1
  SOUTH     = 2
  EAST      = 3
  WEST      = 4

  ##############################################################
  ##     Connecter les pins de vss aux couronnes de vss       ##
  ##############################################################
  core_transformation = core.getTransformation()        
                                 
  # parcourir les pins de vss
  for element in mips_core.getNet("vss").getPins():
    element_layer_name = str ( element.getLayer().getName() ) # string, nom du layer
    # avoir le x, y transforme
    _x = core_transformation.getX ( element.getX(), element.getY() )
    _y = core_transformation.getY ( element.getX(), element.getY() )

    # Creer un contact a la place du pin 
    if   re.search ( "METAL4", element_layer_name ) : old_contact = Contact ( vss, metal4, _x, _y , element.getHeight(), element.getHeight() ) 
    elif re.search ( "METAL1", element_layer_name ) : old_contact = Contact ( vss, metal1, _x, _y , element.getHeight(), element.getHeight() )
    elif re.search ( "METAL3", element_layer_name ) : old_contact = Contact ( vss, metal3, _x, _y , element.getHeight(), element.getHeight() )
    else : 
      raise ErrorMessage(2,"wrong layer of pin in the west of core : %s.\n" % element_layer_name)
  
    # Connection du cote de l'ouest   
    if element.getAccessDirection() == WEST : 
      # Parcourir tous les couronnes vss
      halfHeight = element.getHeight() / 2
      lastVerti  = n
      for [yPin,pinHalfHeight] in pad_west_pin_y_vss + pad_west_pin_y_vdd :
        if ( _y + halfHeight + DbU_lambda(3) >= yPin - pinHalfHeight and _y + halfHeight + DbU_lambda(3) <= yPin + pinHalfHeight ) \
        or ( _y - halfHeight - DbU_lambda(3) <= yPin + pinHalfHeight and _y - halfHeight - DbU_lambda(3) >= yPin - pinHalfHeight ):
          lastVerti = n/2 + (n+1)%2
          
      for i in range ( lastVerti + 1 ) : 
       contact_x =  init_Xmin - ( decalage*2 )*i # x du contact a creer
       contact_side = element.getHeight() - DbU_lambda(1.0) 

       if  re.search ( "METAL4", element_layer_name ) :
         contact     = Contact ( vss, via3 , contact_x , _y , contact_side , contact_side )
         horizontal  = Horizontal ( contact, old_contact , metal4 , _y , element.getHeight() )
         old_contact = contact
       else : 
         contact_via1 = Contact ( vss, via1 , contact_x , _y , contact_side , contact_side )
         contact_via2 = Contact ( vss, via2 , contact_x , _y , contact_side , contact_side )
         horizontal   = Horizontal ( contact_via1 , old_contact , metal1 , _y , element.getHeight() )       
         old_contact  = contact_via1
         
    # Connection du cote de l'est   
    if element.getAccessDirection() == EAST : 
      # Parcourir tous les couronnes vss
      halfHeight = element.getHeight() / 2
      lastVerti  = n
      for [yPin,pinHalfHeight] in pad_east_pin_y_vss + pad_east_pin_y_vdd :
        if ( _y + halfHeight + DbU_lambda(3) >= yPin - pinHalfHeight and _y + halfHeight + DbU_lambda(3) <= yPin + pinHalfHeight ) \
        or ( _y - halfHeight - DbU_lambda(3) <= yPin + pinHalfHeight and _y - halfHeight - DbU_lambda(3) >= yPin - pinHalfHeight ):
          lastVerti = n/2 + (n+1)%2

      for i in range ( lastVerti + 1 ) : 
        contact_x    =  init_Xmax + ( decalage*2 )*i # x du contact a creer         
        contact_side = element.getHeight() - DbU_lambda(1.0)
        
        if  re.search ( "METAL4", element_layer_name ) :
          contact     = Contact ( vss, via3 , contact_x , _y , contact_side, contact_side )
          horizontal  = Horizontal ( contact, old_contact , metal4 , _y , element.getHeight() )
          old_contact = contact
        else : 
          contact_via1 = Contact ( vss, via1 , contact_x , _y , contact_side , contact_side )
          contact_via2 = Contact ( vss, via2 , contact_x , _y , contact_side , contact_side )
          horizontal   = Horizontal ( contact_via1 , old_contact , metal1 , _y , element.getHeight() )
          old_contact  = contact_via1
  
    # Connection du cote du nord    
    if element.getAccessDirection() == NORTH  : 
      # Parcourir tous les couronnes vss
      halfWidth = element.getHeight() / 2
      lastVerti  = n
      for [xPin,pinHalfWidth] in pad_north_pin_x_vss + pad_north_pin_x_vdd :
        if ( _x + halfWidth + DbU_lambda(3) >= xPin - pinHalfWidth and _x + halfWidth + DbU_lambda(3) <= xPin + pinHalfWidth ) \
        or ( _x - halfWidth - DbU_lambda(3) <= xPin + pinHalfWidth and _x - halfWidth - DbU_lambda(3) >= xPin - pinHalfWidth ):
          lastVerti = n/2 + (n+1)%2

      for i in range ( lastVerti + 1 ) : 
        contact_y =  init_Ymax  +  ( decalage*2 )*i # y du contact a creer         
  
        if re.search ( "METAL3", element_layer_name ) :         
          contact     = Contact ( vss, via3, _x, contact_y, element.getHeight(), element.getHeight() )
          vertical    = Vertical ( contact, old_contact, metal3, _x, DbU_lambda(RING_WIDTH) )
          old_contact = contact 
        else :
          raise ErrorMessage(2,"wrong layer of pin in the west of core : %s.\n"% element_layer_name)
       
    # Connection du cote du sud    
    if element.getAccessDirection() == SOUTH : 
      # Parcourir tous les couronnes vss
      halfWidth = element.getHeight() / 2
      lastVerti  = n
      for [xPin,pinHalfWidth] in pad_south_pin_x_vss + pad_south_pin_x_vdd :
        if ( _x + halfWidth + DbU_lambda(3) >= xPin - pinHalfWidth and _x + halfWidth + DbU_lambda(3) <= xPin + pinHalfWidth ) \
        or ( _x - halfWidth - DbU_lambda(3) <= xPin + pinHalfWidth and _x - halfWidth - DbU_lambda(3) >= xPin - pinHalfWidth ):
          lastVerti = n/2 + (n+1)%2

      for i in range ( lastVerti+1 ) : 
        contact_y =  init_Ymin  -  ( decalage*2 )*i # x du contact a creer
  
        if re.search ( "METAL3", element_layer_name ) :         
          contact     = Contact ( vss, via3, _x, contact_y, element.getHeight(), element.getHeight() )
          vertical    = Vertical ( contact, old_contact, metal3, _x, DbU_lambda(RING_WIDTH) )
          old_contact = contact
        else :
          raise ErrorMessage(2,"wrong layer of pin in the west of core : %s."%element_layer_name)

    # End of while 
    
  #######################################################
  ### Connecter les pins de vdd aux couronnes de vdd  ###
  #######################################################
  core_transformation = core.getTransformation()        
  
  # parcourir les pins de vdd
  for element in mips_core.getNet("vdd").getPins():
    element_layer_name = str(element.getLayer().getName()) # string , nom du layer 
    # avoir x, y transforme
    _x = core_transformation.getX ( element.getX(), element.getY() )
    _y = core_transformation.getY ( element.getX(), element.getY() )
  
    # Creer un contact a la place du pin 
    if   re.search ( "METAL4", element_layer_name ) : old_contact = Contact ( vdd, metal4, _x, _y , element.getHeight(), element.getHeight() )
    elif re.search ( "METAL1", element_layer_name ) : old_contact = Contact ( vdd, metal1, _x, _y , element.getHeight(), element.getHeight() )
    elif re.search ( "METAL3", element_layer_name ) : old_contact = Contact ( vdd, metal3, _x, _y , element.getHeight(), element.getHeight() )  
    else : 
      raise ErrorMessage(2,"wrong layer of pin in the west of core : %s."%element_layer_name)
  
    # Connection du cote de l'ouest   
    if element.getAccessDirection() == WEST : 
      # Parcourir tous les couronnes vdd
      halfHeight = element.getHeight() / 2
      lastVerti  = n
      for [yPin,pinHalfHeight] in pad_west_pin_y_vss + pad_west_pin_y_vdd :
        if ( _y + halfHeight + DbU_lambda(3) >= yPin - pinHalfHeight and _y + halfHeight + DbU_lambda(3) <= yPin + pinHalfHeight ) \
        or ( _y - halfHeight - DbU_lambda(3) <= yPin + pinHalfHeight and _y - halfHeight - DbU_lambda(3) >= yPin - pinHalfHeight ):
          lastVerti = n/2 + (n+1)%2
      
      for i in range ( lastVerti ) : 
        contact_x    =  init_Xmin - decalage - ( decalage*2 )*i # x du contact a creer         
        contact_side = element.getHeight() - DbU_lambda(1.0)
  
        if re.search ( "METAL4", element_layer_name ) :         
          contact     = Contact ( vdd, via3, contact_x, _y, contact_side, contact_side )
          horizontal  = Horizontal ( contact, old_contact, metal4, _y, element.getHeight() )
          old_contact = contact 
        else : 
          contact_via1 = Contact ( vdd, via1, contact_x, _y, contact_side, contact_side )
          contact_via2 = Contact ( vdd, via2, contact_x, _y, contact_side, contact_side )
          horizontal   = Horizontal ( contact_via1, old_contact, metal1, _y, element.getHeight() )       
          old_contact  = contact_via1
  
    # Connection du cote de l'est   
    if element.getAccessDirection() == EAST : 
      # Parcourir tous les couronnes vdd
      halfHeight = element.getHeight() / 2
      lastVerti  = n
      for [yPin,pinHalfHeight] in pad_east_pin_y_vss + pad_east_pin_y_vdd :
        if ( _y + halfHeight + DbU_lambda(3) >= yPin - pinHalfHeight and _y + halfHeight + DbU_lambda(3) <= yPin + pinHalfHeight ) \
        or ( _y - halfHeight - DbU_lambda(3) <= yPin + pinHalfHeight and _y - halfHeight - DbU_lambda(3) >= yPin - pinHalfHeight ):
          lastVerti = n/2 + (n+1)%2
      
      for i in range ( lastVerti ) : 
       contact_x    =  init_Xmax + ( decalage*2 )*i + decalage # x du contact a creer         
       contact_side = element.getHeight() - DbU_lambda(1.0)
  
       if re.search ( "METAL4", element_layer_name ) :
         contact     = Contact ( vdd, via3 , contact_x , _y , contact_side , contact_side )
         horizontal  = Horizontal ( contact, old_contact , metal4 , _y , element.getHeight() )
         old_contact = contact 
       else : 
         contact_via1 = Contact ( vdd, via1, contact_x, _y, contact_side, contact_side )
         contact_via2 = Contact ( vdd, via2, contact_x, _y, contact_side, contact_side )
         horizontal   = Horizontal ( contact_via1, old_contact, metal1, _y, element.getHeight() )       
         old_contact  = contact_via1  
  
    # Connection du cote du nord    
    if element.getAccessDirection() == NORTH  : 
      # Parcourir tous les couronnes vdd
      halfWidth = element.getHeight() / 2
      lastVerti  = n
      for [xPin,pinHalfWidth] in pad_north_pin_x_vss + pad_north_pin_x_vdd :
        if ( _x + halfWidth + DbU_lambda(3) >= xPin - pinHalfWidth and _x + halfWidth + DbU_lambda(3) <= xPin + pinHalfWidth ) \
        or ( _x - halfWidth - DbU_lambda(3) <= xPin + pinHalfWidth and _x - halfWidth - DbU_lambda(3) >= xPin - pinHalfWidth ):
          lastVerti = n/2 + (n+1)%2

      for i in range ( lastVerti ) : 
        contact_y =  init_Ymax + decalage + ( decalage*2 )*i # x du contact a creer         
  
        if re.search ( "METAL3", element_layer_name ) :         
          contact     = Contact ( vdd, via3, _x, contact_y, element.getHeight(), element.getHeight() )
          vertical    = Vertical ( contact, old_contact, metal3, _x, DbU_lambda(RING_WIDTH) )
          old_contact = contact 
        else :
          raise ErrorMessage(2,"wrong layer of pin in the west of core : %s."%element_layer_name)

    # Connection du cote du sud    
    if element.getAccessDirection() == SOUTH  : 
      # Parcourir tous les couronnes vdd
      halfWidth = element.getHeight() / 2
      lastVerti  = n
      for [xPin,pinHalfWidth] in pad_south_pin_x_vss + pad_south_pin_x_vdd :
        if ( _x + halfWidth + DbU_lambda(3) >= xPin - pinHalfWidth and _x + halfWidth + DbU_lambda(3) <= xPin + pinHalfWidth ) \
        or ( _x - halfWidth - DbU_lambda(3) <= xPin + pinHalfWidth and _x - halfWidth - DbU_lambda(3) >= xPin - pinHalfWidth ):
          lastVerti = n/2 + (n+1)%2
         
      for i in range ( lastVerti ) : 
        contact_y =  init_Ymin - decalage - ( decalage*2 )*i # x du contact a creer         
  
        if re.search ( "METAL3", element_layer_name ) :         
          contact     = Contact ( vdd, via3, _x, contact_y, element.getHeight(), element.getHeight() )
          vertical    = Vertical ( contact, old_contact, metal3, _x, DbU_lambda(RING_WIDTH) )
          old_contact = contact     
        else :
          raise ErrorMessage(2,"wrong layer of pin in the west of core : %s."%element_layer_name)
  
    # End of while 
  
  # Attention au decalage de 1/2 de CMETAL 
  #LAMBDA = DbU_lambda ( 1 ) / 2
  LAMBDA = DbU_lambda ( 1 )
  
  ###############################################################
  #  Connection entre couronnes internes et couronnes externes  #
  ###############################################################
  for pad_inst in pad_east :
    if isInternalPowerPad ( pad_inst )  or isExternalPowerPad ( pad_inst )  \
    or isInternalGroundPad ( pad_inst ) or isExternalGroundPad ( pad_inst ) :

      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )
          
          contactPad = Contact ( vss, metal1, X, Y, height, height )
          Horizontal ( contactPad, vertical_east_vss[-1], metal1, Y, DbU_lambda ( RING_WIDTH ) )
          Contact    ( vss, via1, vertical_east_vss[-1].getX(), Y, height, height )
          Contact    ( vss, via2, vertical_east_vss[-1].getX(), Y, height, height )
      
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          contactPad = Contact ( vdd, metal1, X, Y, height, height )
          Horizontal ( contactPad, vertical_east_vdd[-1], metal1, Y, DbU_lambda ( RING_WIDTH ) )
          Contact    ( vdd, via1, vertical_east_vdd[-1].getX(), Y, height, height )
          Contact    ( vdd, via2, vertical_east_vdd[-1].getX(), Y, height, height )
      
  for pad_inst in pad_west :
    if isInternalPowerPad ( pad_inst )  or isExternalPowerPad ( pad_inst )  \
    or isInternalGroundPad ( pad_inst ) or isExternalGroundPad ( pad_inst ) :
        
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          contactPad = Contact ( vss, metal1, X, Y, height, height )
          Horizontal ( contactPad, vertical_west_vss[-1], metal1, Y, DbU_lambda ( RING_WIDTH ) )
          Contact    ( vss, via1, vertical_west_vss[-1].getX(), Y, height, height )
          Contact    ( vss, via2, vertical_west_vss[-1].getX(), Y, height, height )
      
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)

        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          contactPad = Contact ( vdd, metal1, X, Y, height, height )
          Horizontal ( contactPad, vertical_west_vdd[-1], metal1, Y, DbU_lambda ( RING_WIDTH ) )
          Contact    ( vdd, via1, vertical_west_vdd[-1].getX(), Y, height, height )
          Contact    ( vdd, via2, vertical_west_vdd[-1].getX(), Y, height, height )
      
  for pad_inst in pad_north : 
    if isInternalPowerPad ( pad_inst )  or isExternalPowerPad ( pad_inst )  \
    or isInternalGroundPad ( pad_inst ) or isExternalGroundPad ( pad_inst ) :
        
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :    
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          Contact    ( vss, via1, X, Y, height, height )
          Contact    ( vss, via2, X, Y, height, height )
    
          contactPad = Contact ( vss, metal1, X, Y, height, height )
          Vertical   ( contactPad, horizontal_north_vss[-1], metal1, X , DbU_lambda ( RING_WIDTH ) )
          Contact    ( vss, via1, X, horizontal_north_vss[-1].getY(), height, height )
          Contact    ( vss, via2, X, horizontal_north_vss[-1].getY(), height, height )
          Contact    ( vss, via3, X, horizontal_north_vss[-1].getY(), height, height )
      
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )
          
          Contact    ( vdd, via1, X, Y, height, height )
          Contact    ( vdd, via2, X, Y, height, height )
          
          contactPad = Contact ( vdd, metal1, X, Y, height, height )
          Vertical   ( contactPad, horizontal_north_vdd[-1], metal1, X, DbU_lambda ( RING_WIDTH ) )
          Contact    ( vdd, via1, X, horizontal_north_vdd[-1].getY(), height, height )
          Contact    ( vdd, via2, X, horizontal_north_vdd[-1].getY(), height, height )
          Contact    ( vdd, via3, X, horizontal_north_vdd[-1].getY(), height, height )
  
  for pad_inst in pad_south : 
    if isInternalPowerPad ( pad_inst )  or isExternalPowerPad ( pad_inst )  \
    or isInternalGroundPad ( pad_inst ) or isExternalGroundPad ( pad_inst ) :
        
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vssi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          Contact    ( vss, via1, X, Y, height, height )
          Contact    ( vss, via2, X, Y, height, height )
          
          contactPad = Contact ( vss, metal1, X, Y, height, height )
          Vertical ( contactPad, horizontal_south_vss[-1], metal1, X , DbU_lambda ( RING_WIDTH ) )
          Contact    ( vss, via1, X, horizontal_south_vss[-1].getY(), height, height )
          Contact    ( vss, via2, X, horizontal_south_vss[-1].getY(), height, height )
          Contact    ( vss, via3, X, horizontal_south_vss[-1].getY(), height, height )
      
      for element in NetExternalComponents.get ( pad_inst.getMasterCell().getNet("vddi") ):
        layer   = element.getLayer()
        height  = element.getBoundingBox().getHeight() - DbU_lambda(1.0)
    
        if re.search ( "METAL3", str ( layer ) ) \
        and ( ( element.getY() - ( height / 2 ) ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :
          X = pad_inst.getTransformation().getX ( element.getX(), element.getY() ) 
          Y = pad_inst.getTransformation().getY ( element.getX(), element.getY() )

          Contact    ( vdd, via1, X, Y, height, height )
          Contact    ( vdd, via2, X, Y, height, height )
          
          contactPad = Contact ( vdd, metal1, X, Y, height, height )
          Vertical ( contactPad, horizontal_south_vdd[-1], metal1, X , DbU_lambda ( RING_WIDTH ) )
          Contact    ( vdd, via1, X, horizontal_south_vdd[-1].getY(), height, height )
          Contact    ( vdd, via2, X, horizontal_south_vdd[-1].getY(), height, height )
          Contact    ( vdd, via3, X, horizontal_south_vdd[-1].getY(), height, height )
  

  #####################################                      ###########################
  ##  PLACER LES COURONNES EXTERNES  ##                      # 1                     2 #
  #####################################                      #                         #
                                                             #                         #
                                                             #                         #
  points_0 = []                                              #           CHIP          #
  points_1 = []                                              #                         #
  points_2 = []                                              #                         #
  points_3 = []                                              #                         #
                                                             #                         #
                                                             # 0                     3 #
                                                             ###########################
  
  instance    = cell.getInstance ( pad_north[0].getName() )
  trans       = instance.getTransformation()
    
  for net in instance.getMasterCell().getNets():
    if net.isSupply() or net.isClock() :
      for component in NetExternalComponents.get(net):
        plug = instance.getPlug ( net )
        NET  = getNetFromPlug( plug )
        if ( not NET ) : raise ErrorMessage(2,"Error plug : %s must be connected" % str(plug.getName()))
  
        layer     = getNonCLayer ( component.getLayer() )

        # On applique la transformation au component, et on recupere les nouvelles coordonnees
        Y = cell_Ymax + cell_Ymin - trans.getY ( component.getX(), component.getY() )
  
        # On recupere la hauteur du component pour router sur la meme hauteur
        RING_HEIGHT = component.getBoundingBox().getHeight()
        
        # On calcule les differents points aux angles pour chaque net
        X_point = cell_Xmin + Y
        Y_point = cell_Ymin + Y
        contact = Contact ( NET, layer, X_point, Y_point, RING_HEIGHT, RING_HEIGHT )
        points_0.append ( dict ( [ ['X',X_point], ['Y',Y_point], ['N',NET], ['L',layer], ['R',RING_HEIGHT], ['C',contact] ] ) )
  
        X_point = cell_Xmin + Y
        Y_point = cell_Ymin + ( cell_Ymax - Y )
        contact = Contact ( NET, layer, X_point, Y_point, RING_HEIGHT, RING_HEIGHT )
        points_1.append ( dict ( [ ['X',X_point], ['Y',Y_point], ['N',NET], ['L',layer], ['R',RING_HEIGHT], ['C',contact] ] ) )
        
        X_point = cell_Xmin + ( cell_Xmax - Y )
        Y_point = cell_Ymin + ( cell_Ymax - Y )
        contact = Contact ( NET, layer, X_point, Y_point, RING_HEIGHT, RING_HEIGHT )
        points_2.append ( dict ( [ ['X',X_point], ['Y',Y_point], ['N',NET], ['L',layer], ['R',RING_HEIGHT], ['C',contact] ] ) )
      
        X_point = cell_Xmin + ( cell_Xmax - Y )
        Y_point = cell_Ymin + Y
        contact = Contact ( NET, layer, X_point, Y_point, RING_HEIGHT, RING_HEIGHT )
        points_3.append ( dict ( [ ['X',X_point], ['Y',Y_point], ['N',NET], ['L',layer], ['R',RING_HEIGHT], ['C',contact] ] ) )
      # end of while
  # end of while

  #print "\n\n\n\npoints_0 : ", points_0 , "\n\npoints_1 : " ,points_1 , "\n\npoints_2 : " ,points_2 , "\n\npoints_3 : " , points_3 , "\n\n\n\n" 

  # Placer au cote du nord 
  for ins_pad in pad_north :
    Y_pad   = DbU_lambda ( int ( DbU_getLambda ( ins_pad.getAbutmentBox().getYCenter() ) ) )
    X_pad   = DbU_lambda ( int ( DbU_getLambda ( ins_pad.getAbutmentBox().getXCenter() ) ) )
  
    if ins_pad == pad_north[0] :
      for point in points_1 :
        contact = Contact ( point['N'], point['L'], X_pad, point['Y'], point['R'], point['R'] )
        Horizontal ( point['C'], contact, point['L'], point['Y'], point['R'] ) 
    else :
      for point in points_1 :
        Horizontal ( point['N'], point['L'], point['Y'], point['R'], old_X_pad, X_pad ) 
      
    if ins_pad == pad_north[-1] :
      for point in points_2 :
        contact = Contact ( point['N'], point['L'], X_pad, point['Y'], point['R'], point['R'] )
        Horizontal ( contact, point['C'], point['L'], point['Y'], point['R'] ) 
  
    old_X_pad = X_pad

  # Placer au cote de l'est 
  for ins_pad in pad_east :
    Y_pad   = ins_pad.getAbutmentBox().getYCenter()
    X_pad   = ins_pad.getAbutmentBox().getXCenter()
  
    if ins_pad == pad_east[0] :
      for point in points_3 :
        contact = Contact ( point['N'], point['L'], point['X'], Y_pad, point['R'], point['R'] )
        Vertical ( point['C'], contact, point['L'], point['X'], point['R'] )  
    else :
      for point in points_3 :
        Vertical ( point['N'], point['L'], point['X'], point['R'], old_Y_pad, Y_pad )  
  
    if ins_pad == pad_east[-1] :
      for point in points_2 :
        contact = Contact ( point['N'], point['L'], point['X'], Y_pad, point['R'], point['R'] )
        Vertical ( contact, point['C'], point['L'], point['X'], point['R'] )  
       
    old_Y_pad = Y_pad

  # Placer au cote du south 
  for ins_pad in pad_south :
    Y_pad   = ins_pad.getAbutmentBox().getYCenter()
    X_pad   = ins_pad.getAbutmentBox().getXCenter()
  
    if ins_pad == pad_south[0] :
      for point in points_0 :
        contact = Contact ( point['N'], point['L'], X_pad, point['Y'], point['R'], point['R'] )
        Horizontal ( point['C'], contact, point['L'], point['Y'], point['R'] ) 
    else :
      for point in points_0 :
        Horizontal ( point['N'], point['L'], point['Y'], point['R'], old_X_pad, X_pad ) 
      
    if ins_pad == pad_south[-1] :
      for point in points_3 :
        contact = Contact ( point['N'], point['L'], X_pad, point['Y'], point['R'], point['R'] )
        Horizontal ( contact, point['C'], point['L'], point['Y'], point['R'] ) 
  
    old_X_pad = X_pad
    
  # Placer au cote de l'ouest 
  for ins_pad in pad_west :
    Y_pad   = ins_pad.getAbutmentBox().getYCenter()
    X_pad   = ins_pad.getAbutmentBox().getXCenter()
  
    if ins_pad == pad_west[0] :
      for point in points_0 :
        contact = Contact ( point['N'], point['L'], point['X'], Y_pad, point['R'], point['R'] )
        Vertical ( point['C'], contact, point['L'], point['X'], point['R'] )  
    else :
      for point in points_0 :
        Vertical ( point['N'], point['L'], point['X'], point['R'], old_Y_pad, Y_pad )  
  
    if ins_pad == pad_west[-1] :
      for point in points_1 :
        contact = Contact ( point['N'], point['L'], point['X'], Y_pad, point['R'], point['R'] )
        Vertical ( contact, point['C'], point['L'], point['X'], point['R'] )  
       
    old_Y_pad = Y_pad

  UpdateSession.close()

#######################################
def create_inst ( model, name, cell ) :

  # Error : if the cell the instance has to be instanciated in does not exist
  if not cell :
    raise ErrorMessage(2,"Cannot create instance %s : the cell does not exist."%name)
    
  # Load model in the database
  modelmastercell = CRL.AllianceFramework.get().getCell ( model, CRL.Catalog.State.Views )
    
  # Error : if the model is not found in the libraries
  if not modelmastercell :
    raise ErrorMessage(2,"Cannot create instance %s : model %s does not exist in the database."%(name,model))
    
  inst = Instance ( cell, name, modelmastercell )
  
  # Connection
  plugGround = inst.getPlug ( iter(modelmastercell.getGroundNets()).next() )
  plugGround.setNet ( iter(cell.getGroundNets()).next() )
  plugPower = inst.getPlug ( iter(modelmastercell.getPowerNets()).next() )
  plugPower.setNet ( iter(cell.getPowerNets()).next() )

  return inst

##########################
def place ( inst, x, y, orientation ) :
    
  # Error : if the hurricane instance does not exist
  if not inst :
    raise ErrorMessage(2,"Layout : The instance of %s has not been created."%str(inst.getName()))
 
  # Error : if the instance is already placed
  if inst.getPlacementStatus() == PlacementStatusFIXED :
    raise ErrorMessage(2,"Placement : the instance %s is already placed."%str(inst.getName()))
  
  UpdateSession.open()
    
  ## A COMPLETER POUR FAIRE DES PLACE AVEC TOUTES LES SYMETRIES ##
  if orientation == OrientationMY :
    y += inst.getAbutmentBox().getHeight()
    
  inst.setTransformation  ( Transformation ( x, y, orientation ) )
  inst.setPlacementStatus ( PlacementStatusFIXED ) 

  UpdateSession.close()

#########################################
def getStandardInstancesMasque ( cell ) :
  '''This function creates the mask of the standard cells'''

  global FREE, standard_instances_masque, nb_lignes, nb_colonnes, reference
            
  # The two dimensions of the mask 
  nb_lignes   = cell.getAbutmentBox().getHeight() / DbU_lambda(SLICE) 
  nb_colonnes = cell.getAbutmentBox().getWidth()  / DbU_lambda(PITCH) 

  # Creation of a mask named standard_instances_masque 
  standard_instances_masque = []
  standard_instances_masque = [[FREE for j in range(nb_colonnes)] for i in range(nb_lignes)]
  
  # Cover of all the instances of this model and fill in of the mask 
  reference = getAllStandardInstances ( cell )

  return reference

######################################
def getAllStandardInstances ( cell ) : 
  '''This function is used to get all the standard instances of a cell and to flatten all the standard cells''' 
  
  global reference
  
  reference = []
  
  for element in cell.getInstances():
    # FOR EACH PLACED or FIXED instance of the cell, we call getStandardInstances method
    if element.getPlacementStatus() != PlacementStatusUNPLACED :
      reference = getStandardInstances ( cell, element, element.getTransformation(), cell.getName() )

  return reference

##########################################################
def getStandardInstances ( cell, inst, transformation, name_masterinst ) :
  '''This function covers all the instances of the model , 
  it is also used to get all the transformations of the placed instances 
  it fills the standard_instances masque and the standard_instances_list ''' 
  
  global POWER, OCCUPIED
  global standard_instances_masque, reference

  # Si l'instance est PLACED ou FIXED
  if inst.getPlacementStatus() != PlacementStatusUNPLACED :
    name = str ( inst.getMasterCell().getLibrary().getName() )
    
    if inst.isLeaf() :
      # new abutmentbox
      newbox = transformation.getBox ( inst.getMasterCell().getAbutmentBox() )
      
      Height = newbox.getHeight()
      Width  = newbox.getWidth()
      Xmin   = newbox.getXMin()
      Ymin   = newbox.getYMin()
  
      orientation = transformation.getOrientation()
      
      if reference == [] : reference[ 0:3 ] = [ Xmin, Ymin, orientation ]
      
      # on ajoute l'inst a la liste standard_instances_list
      standard_instances_list.append ( ( Ymin, orientation, inst, str(name_masterinst) ) ) # add a tuple 
               
      nb_slices = Height / DbU_lambda(SLICE)
      nb_pitchs = Width  / DbU_lambda(PITCH)
      
      # on remplit le masque
      for i in range ( nb_slices ) :
        for j in range ( nb_pitchs ) :
          # on marque les cellules en POWER ou OCCUPIED      FIXME : pourquoi ?
          if re.search ( "powmid_x0", str ( inst.getMasterCell().getName() ) ) : 
            standard_instances_masque [ Ymin / DbU_lambda(SLICE) + i ][ Xmin / DbU_lambda(PITCH) + j ] = POWER
          else :
            standard_instances_masque [ Ymin / DbU_lambda(SLICE) + i ][ Xmin / DbU_lambda(PITCH) + j ] = OCCUPIED 
  
    # Si la masterCell n'est pas terminale, on appelle la fonction recursivement sur ses instances
    else  :
      for element in inst.getMasterCell().getInstances():
        getStandardInstances ( cell, element, transformation.getTransformation(element.getTransformation()), inst.getName() ) 

  return reference

##########
def verifyPlacement ( cell ) :
  '''This function is used to check the placement of the standard cells''' 

  global standard_instances_list
  
  # YMin of reference 
  YMin = standard_instances_list[0][0]   
   
  # orientation code of reference 
  orientation = standard_instances_list[0][1]    

  # cover all the other  
  for i in range ( 1, len ( standard_instances_list ) ) :
     element             = standard_instances_list[i]           
     element_YMin        = element[0]
     element_orientation = element[1] 

     distance            = element_YMin - YMin

     # Error :  if the orientation is different from 0, 2, 4 ,6 
     #if not element_orientation in [0,2,4,6] :
     if element_orientation in [1,3,5,7] :
       err  = "Placement of cells : please check your file of layout with DRUC."
#       err += "Error Detail :" + "\n"
#       err += "    instance " + str(element[2].getName()) + " of model " + str(element[2].getMasterCell().getName()) + "\n"
#       err += "    in cell "  + str(element[2].getCell().getName()) + "\n"
#       err += "    incoherent with :" + "\n"
#       err += "    instance " + str(standard_instances_list[0][2].getName()) + "\n"
#       err += "    of model " + str(standard_instances_list[0][2].getMasterCell().getName()) + "\n"
#       err += "    in cell "  + str(standard_instances_list[0][2].getCell().getName()) + "\n"
#       err += "    reference ymin is " + str(YMin) + " orientation " + str(orientation) + "\n"
#       err += "    element ymin is", element[0], "orientation", element_orientation + "\n"
       raise ErrorMessage(2,err)
  
     if distance < 0 : distance = -distance

     nb_case = distance / DbU_lambda ( 50 )

     # odd number 
     if nb_case % 2 :
       if ( element_orientation - orientation ) in [4,-4] :  
         err  = "Placement of cells : please check your file of layout with DRUC\n"
#         err += "Error Detail :" + "\n"
#         err += "    instance " + str(element[2].getName()) + " of model " + str(element[2].getMasterCell().getName()) + "\n"
#         err += "    in cell "  + str(element[2].getCell().getName()) + "\n"
#         err += "    incoherent with :" + "\n"
#         err += "    instance " + str(standard_instances_list[0][2].getName()) + "\n"
#         err += "    of model " + str(standard_instances_list[0][2].getMasterCell().getName()) + "\n"
#         err += "    in cell "  + str(standard_instances_list[0][2].getCell().getName()) + "\n"
#         err += "    reference ymin is " + str(YMin) + " orientation " + orientation + "\n"
#         err += "    element ymin is " + element[0] + " orientation " + element_orientation + "\n"
         raise ErrorMessage(2,err)
           
     # even number
     else :
       if ( element_orientation - orientation ) in [2,-2,6,-6] : 
         err  = "Placement of cells : please check your file of layout with DRUC\n"
#         err += "Error Detail :"
#         err += "    instance " + str(element[2].getName()) + " of model " + str(element[2].getMasterCell().getName()) + "\n"
#         err += "    in cell "  + str(element[2].getCell().getName()) + "\n"
#         err += "    incoherent with :" + "\n"
#         err += "    instance " + str(standard_instances_list[0][2].getName()) + "\n"
#         err += "    of model " + str(standard_instances_list[0][2].getMasterCell().getName()) + "\n"
#         err += "    in cell "  + str(standard_instances_list[0][2].getCell().getName()) + "\n"
#         err += "    reference ymin is " + str(YMin) + " orientation " + orientation + "\n"
#         err += "    element ymin is " + element[0] + " orientation " + element_orientation + "\n"
         raise ErrorMessage(2,err)
         
###################################
def temporarySave ( cell = None ) :

  UpdateSession.open()

  framework = CRL.AllianceFramework.get()
  
  if cell == None :
    for cell in framework.getLibrary(0).getCells():
      if str ( cell.getName() ) != "__Scratch__" :
        framework.saveCell ( cell, CRL.Catalog.State.Physical )
  else :
    framework.saveCell ( cell, CRL.Catalog.State.Physical )
  
  UpdateSession.close()

###########################
def getVddVss ( cell, y ) :
  '''This function returns a string 'vdd' or 'vss' according to the variance between y and the reference cell. y is in slices''' 

  global reference
  
  if reference == [] : raise ErrorMessage(2,"getVddVss : Reference is not token.") 
  
  # in order to know if it is vdd or vss
  distance = y - int ( DbU_getLambda ( reference[1] ) ) / 50
  
  if distance < 0 : distance = -distance

  orientation = reference[2]    

  if orientation in [0,4] :
    if distance % 2 : return "vdd"
    else            : return "vss"
  elif orientation in [2,6] :
    if distance % 2 : return "vss"  
    else            : return "vdd"
  else :
    raise ErrorMessage(2,"get_vdd_vss : Illegal orientation of reference %s.\n"%orientation)

###########################
def getPadHeight ( cell ) :
  '''This function returns the pad height if there is at least one pad'''

  padFound = 0
  
  for instance in cell.getInstances():
    if isPad ( instance ):
      pad_height = instance.getMasterCell().getAbutmentBox().getHeight()
      padFound   = 1
      break
  
  if padFound : return pad_height
  else        : raise ErrorMessage(2,"getPadHeight : No pad found.")

###################
def isPad ( ins ) :
  '''This function returns 1 if the instance is a pad'''

  if re.search ( "p.*_px", str ( ins.getMasterCell().getName() ) ) \
  or re.search ( "p.*_sp", str ( ins.getMasterCell().getName() ) ):
    return True
  else:
    return False

################################
def isInternalPowerPad ( ins ) :
  '''This function returns 1 if pad is an internal power pad'''
  if re.search ( "pvddi", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

################################
def isExternalPowerPad ( ins ) :
  '''This function returns 1 if pad is an external power pad'''
  if re.search ( "pvdde", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

#################################
def isInternalGroundPad ( ins ) :
  '''This function returns 1 if pad is an internal ground pad'''
  if re.search ( "pvssi", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

#################################
def isExternalGroundPad ( ins ) :
  '''This function returns 1 if pad is an external ground pad'''
  if re.search ( "pvsse", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

################################
def isInternalClockPad ( ins ) :
  '''This function returns 1 if pad is an internal clock pad'''
  if re.search ( "pvssick_", str ( ins.getMasterCell().getName() ) ) \
  or re.search ( "pvddick_", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

################################
def isExternalClockPad ( ins ) :
  '''This function returns 1 if pad is an external clock pad'''
  if re.search ( "pvsseck_", str ( ins.getMasterCell().getName() ) ) \
  or re.search ( "pvddeck_", str ( ins.getMasterCell().getName() ) ) :
    return 1
  else:
    return 0

#########################
def affichePad ( cell ) :
  global pad_north, pad_south, pad_east, pad_west
  
  print "Pads in the north are :"
  for pad in pad_north : print cell.getInstance ( pad.getName() ).getMasterCell().getName() 

  print "Pads in the south are :"
  for pad in pad_south : print cell.getInstance ( pad.getName() ).getMasterCell().getName() 

  print "Pads in the east are :"
  for pad in pad_east  : print cell.getInstance ( pad.getName() ).getMasterCell().getName() 

  print "Pads in the west are :"
  for pad in pad_west  : print cell.getInstance ( pad.getName() ).getMasterCell().getName()

############
def searchVddVss ( cell, *args ) :
  '''This function searches plots providing vdd and vss, and returns four lists'''

  global pad_north, pad_south, pad_east, pad_west
  
  # MACRO pour les directions d'access des pins  
  UNDEFINED = 0
  NORTH     = 1
  SOUTH     = 2
  EAST      = 3
  WEST      = 4

  pad_list = [pad_east, pad_west, pad_north, pad_south]   

  for i in range ( len ( args ) ) :

    for ins in pad_list[i] : 
      model          = ins.getMasterCell()
      transformation = ins.getTransformation()

      if isInternalPowerPad ( ins )  or isExternalPowerPad ( ins )  \
      or isInternalGroundPad ( ins ) or isExternalGroundPad ( ins ) :
        # on connecte les pins vddi
        for element in model.getNet("vddi").getPins():
          layer = element.getLayer()
    
          X = transformation.getX ( element.getX(), element.getY() )
          Y = transformation.getY ( element.getX(), element.getY() )
    
          args[i].append ( dict ( [['N', "vdd"], ['L',layer], ['X',X], ['Y',Y], ['C',0]] ) )

        # on connecte les pins vssi
        for element in model.getNet("vssi").getPins():
          layer = element.getLayer()
    
          X = transformation.getX ( element.getX(), element.getY() ) 
          Y = transformation.getY ( element.getX(), element.getY() )
    
          args[i].append ( dict ( [['N', "vss"], ['L',layer], ['X',X], ['Y', Y],['C',0]] ) ) 

########################
def createGrid ( my_tuple ) :

  global ck_contact_list
  global _Xmin, _Ymin, _Xmax, _Ymax
    
  net, cell = my_tuple
    
  def FindNearestPosition ( pos, poslist ) :
    bestDistance = pos - poslist[0]
    bestPos      =  poslist[0]
    
    for x in poslist[1:] :
      distance = pos - x
      if distance <= 0 :
        distance *= -1
        if bestDistance > distance : return x
      else :
        if bestDistance > distance :
          bestPos      = x
          bestDistance = pos - bestPos
    return bestPos
    
  def CreateZ ( contact1, contact2 ) :
    centerX   = (contact1.getX() + contact2.getX() ) / 2
    centerX   = centerX - (centerX % DbU_lambda(5))
    zContact1 = Contact ( net, via5, centerX, contact1.getY(), DbU_lambda(11), DbU_lambda(11) )
    zContact2 = Contact ( net, via5, centerX, contact2.getY(), DbU_lambda(11), DbU_lambda(11) )
    
    Horizontal ( contact1,  zContact1, metal6, contact1.getY(),  DbU_lambda(12) )
    Vertical   ( zContact1, zContact2, metal5, zContact1.getX(), DbU_lambda(12) )
    Horizontal ( zContact2, contact2,  metal6, zContact2.getY(), DbU_lambda(12) )
    
  def CreateN ( contact1, contact2 ) :
    centerY   = ( contact1.getY() + contact2.getY() ) / 2
    centerY   = centerY - ( centerY % DbU_lambda(5) )
    nContact1 = Contact ( net, via5, contact1.getX(), centerY, DbU_lambda(11), DbU_lambda(11) )
    nContact2 = Contact ( net, via5, contact2.getX(), centerY, DbU_lambda(11), DbU_lambda(11) )
    
    Vertical   ( contact1,  nContact1, metal5, contact1.getX(),  DbU_lambda(12) )
    Horizontal ( nContact1, nContact2, metal6, nContact1.getY(), DbU_lambda(12) )
    Vertical   ( nContact2, contact2,  metal5, nContact2.getX(), DbU_lambda(12) )
    
  def FindPositionForContact ( position, contactlist1, contactlist2 ) :
    def PositionIsInTargetRange ( position, target ) :
      return position >= ( target - DbU_lambda(10) ) and position <= ( target + DbU_lambda(10) )
    
    if contactlist1 == [] and contactlist2 == [] : return position
    
    if contactlist1 != [] and contactlist2 != [] :
      nearest1 = FindNearestPosition ( position, contactlist1 )
      nearest2 = FindNearestPosition ( position, contactlist2 )
    
      if ( not PositionIsInTargetRange(position, nearest1) ) and ( not PositionIsInTargetRange(position, nearest2) ) : return position
                                                                                                                       
      if PositionIsInTargetRange ( position, nearest1 ) :
        if nearest2 > nearest1 : return position + DbU_lambda(12)
        else                   : return position - DbU_lambda(12)
    
      if PositionIsInTargetRange ( position, nearest2 ) :
        if nearest1 > nearest2 : return position + DbU_lambda(12)
        else                   : return position - DbU_lambda(12)
    
    if contactlist1 != [] : contactlist = contactlist1
    else                  : contactlist = contactlist2
    
    nearest = FindNearestPosition ( position, contactlist )
    
    if PositionIsInTargetRange ( position, nearest ) :
      if position > nearest : return position + DbU_lambda(12)
      else                  : return position - DbU_lambda(12)
    else :
      return position
      
  #_Xmin = None
  #_Ymin = None
  #_Xmax = None
  #_Ymax = None    
  coreBox = cell.getInstance('core').getAbutmentBox()
  #print coreBox
  _Xmin = coreBox.getXMin()
  _Ymin = coreBox.getYMin()
  _Xmax = coreBox.getXMax()
  _Ymax = coreBox.getYMax()
  ck_contact_list = []  

  getNetInstances ( cell, net, Transformation ( 0, 0, OrientationID ) )
   
  db   = getDataBase()
  via1 = db.getTechnology().getLayer ( "VIA12" )
  via2 = db.getTechnology().getLayer ( "VIA23" )
  via3 = db.getTechnology().getLayer ( "VIA34" )
  via4 = db.getTechnology().getLayer ( "VIA45" )
  via5 = db.getTechnology().getLayer ( "VIA56" )
  metal5 = db.getTechnology().getLayer ( "METAL5"  )
  metal6 = db.getTechnology().getLayer ( "METAL6"  )
  
  gridBoundingBox = Box()

  for contact in ck_contact_list : gridBoundingBox.merge ( contact[0], contact[1] )
    
  #the Bounding Box is inflated in function of the VIA_5 rule
  gridBoundingBox.inflate ( DbU_lambda(15) )
  
  #Create the Bounding Box grid
  NEContact    = Contact ( net, via5, gridBoundingBox.getXMin(), gridBoundingBox.getYMax() , DbU_lambda(11), DbU_lambda(11) )
  NWContact    = Contact ( net, via5, gridBoundingBox.getXMax(), gridBoundingBox.getYMax() , DbU_lambda(11), DbU_lambda(11) )
  SEContact    = Contact ( net, via5, gridBoundingBox.getXMin(), gridBoundingBox.getYMin() , DbU_lambda(11), DbU_lambda(11) )
  SWContact    = Contact ( net, via5, gridBoundingBox.getXMax(), gridBoundingBox.getYMin() , DbU_lambda(11), DbU_lambda(11) )
  northSegment = Segment ( NEContact, NWContact, metal6, DbU_lambda(12) )
  southSegment = Segment ( SEContact, SWContact, metal6, DbU_lambda(12) )
  eastSegment  = Segment ( NEContact, SEContact, metal5, DbU_lambda(12) )
  westSegment  = Segment ( NWContact, SWContact, metal5, DbU_lambda(12) )

  northContacts = []
  southContacts = []
  eastContacts  = []
  westContacts  = []
  
  #connect the pins to the grid
  plugList = []
  #Fill a list with the plugs ... we are going to modify the underlying collection ;)
  for plug in net.getPlugs():
    plugList.append ( plug )
  
  for plug in plugList:
    instance = plug.getInstance()
    if isPad ( instance ) : 
      # Connect this plug to the grid ...
      masterNet      = plug.getMasterNet()
      transformation = instance.getTransformation()
    
      for comp in NetExternalComponents.get(masterNet):
        x    = transformation.getX ( comp.getX(), comp.getY() ) 
        y    = transformation.getY ( comp.getX(), comp.getY() ) 
        #layer   = element.getLayer ()
        #if re.search ( "METAL2", str ( layer ) ) \
        #  and ( ( element.getY() - element.getHalfHeight() ) < pad_inst.getMasterCell().getAbutmentBox().getYMin() ) :

        if x >= gridBoundingBox.getXMin() and x <= gridBoundingBox.getXMax() :
          layer = metal5
          if y < gridBoundingBox.getYMin() :
            if x == gridBoundingBox.getXMin() :
              gridContact = NWContact
            elif x == gridBoundingBox.getXMax() :
              gridContact = NEContact
            else :
              gridContact = Contact(southSegment, via5, x, 0, DbU_lambda(11), DbU_lambda(11))
              southContacts.append(x)
              
          elif y > gridBoundingBox.getYMax() :
            if x == gridBoundingBox.getXMin() :
              gridContact = SWContact
            elif x == gridBoundingBox.getXMax() :
              gridBoundingBox = SEContact
            else :
              gridContact = Contact(northSegment, via5, x, 0, DbU_lambda(11), DbU_lambda(11))
              northContacts.append(x)
          else : 
            raise ErrorMessage(2,"RouteCK : bad pad placement.") 
            
        elif y >= gridBoundingBox.getYMin() and y <= gridBoundingBox.getYMax() :
          layer = metal6
          if x < gridBoundingBox.getXMin() :
            if y == gridBoundingBox.getYMin() :
              gridContact = SWContact
            elif y == gridBoundingBox.getYMax() :
              gridContact = NWContact
            else :
              gridContact = Contact(eastSegment, via5, 0, y, DbU_lambda(11), DbU_lambda(11))
              eastContacts.append(y)
          elif x > gridBoundingBox.getXMax() :
            if y == gridBoundingBox.getYMin() :
              gridContact = SEContact
            elif y == gridBoundingBox.getYMax() :
              gridContact = SWContact
            else :
              gridContact = Contact(westSegment, via5, 0, y, DbU_lambda(11), DbU_lambda(11))
              westContacts.append(y)
          else : 
            raise ErrorMessage(2,"RouteCK : bad pad placement.") 
            
        else :
          err  = "RouteCk: The pads ("+ str(instance.getName()) +") must be in direct regard of the clock grid\n"
          err += "coordinates are : (x1,y1) = (%s,%s) and (x2,y2) = (%s,%s)." \
              % ( DbU.getValueString(gridBoundingBox.getXMin())
                , DbU.getValueString(gridBoundingBox.getYMin())
                , DbU.getValueString(gridBoundingBox.getXMax())
                , DbU.getValueString(gridBoundingBox.getYMax()) )
          raise ErrorMessage(2,err)

        compContact = Contact ( net, via5, x, y, DbU_lambda(11), DbU_lambda(11) )
    
        Segment ( compContact, gridContact, layer , DbU_lambda(12) )
    
        break # just one component ...
        

  # create the internal grid 200x200
  nbVTracks     = gridBoundingBox.getWidth()  / DbU_lambda(200) 
  widthVTracks  = gridBoundingBox.getWidth()  / nbVTracks
  nbHTracks     = gridBoundingBox.getHeight() / DbU_lambda(200)
  heightHTracks = gridBoundingBox.getHeight() / nbHTracks

  xList = []
  yList = []
  for i in range ( 1, nbVTracks ) :
    x = gridBoundingBox.getXMin() + i * widthVTracks 
    x = x - (x % DbU_lambda(5))
    x = FindPositionForContact(x, northContacts, southContacts)
    
    contact1 = Contact ( southSegment, via5, x, 0, DbU_lambda(11), DbU_lambda(11))
    contact2 = Contact ( northSegment, via5, x, 0, DbU_lambda(11), DbU_lambda(11))
    
    Segment ( contact1, contact2, metal5, DbU_lambda(12) )
    
    xList.append ( x )
    
  for i in range ( 1, nbHTracks ) :
    y = gridBoundingBox.getYMin() + i * heightHTracks 
    y = y - ( y % DbU_lambda(5) )
    y = FindPositionForContact ( y, eastContacts, westContacts )
    
    contact1 = Contact ( westSegment, via5, 0, y, DbU_lambda(11), DbU_lambda(11) )
    contact2 = Contact ( eastSegment, via5, 0, y, DbU_lambda(11), DbU_lambda(11) )
    
    horizontal = Segment ( contact1, contact2, metal6, DbU_lambda(12) )
    
    yList.append ( y )

    for x in xList : Contact ( horizontal, via5, x, 0, DbU_lambda(11), DbU_lambda(11) )
          
  # Connection to the grid
  # Cette liste contient les contacts qui sont deja crees 
  ck_contact_list_created = []

  # Now connect all the internal contacts to the grid
  for contact in ck_contact_list :
    xContact = contact[0]
    yContact = contact[1]
    plugContact = Contact ( net, via1 , xContact, yContact, DbU_lambda(2), DbU_lambda(2) ) 
    #find the closest x,y on grid
    xList.insert ( 0, gridBoundingBox.getXMin() )
    yList.insert ( 0, gridBoundingBox.getYMin() )
    xList.append ( gridBoundingBox.getXMax() )
    yList.append ( gridBoundingBox.getYMax() )

    xTarget = FindNearestPosition ( xContact, xList )
    yTarget = FindNearestPosition ( yContact, yList )

    xDistance = abs ( xTarget - xContact )
    yDistance = abs ( yTarget - yContact )

    Contact(net, via2, xContact, yContact, DbU_lambda(2), DbU_lambda(2) ) 
    Contact(net, via3, xContact, yContact, DbU_lambda(2), DbU_lambda(2) ) 
    Contact(net, via4, xContact, yContact, DbU_lambda(2), DbU_lambda(2) )

    if xDistance != 0 or yDistance != 0 :
      if ( xDistance <= yDistance + DbU_lambda(10) ):  # test pour faire un horizontal
        if xDistance != 0 :
          if abs(xDistance) <= DbU_lambda(3) :
            gridContact = Contact ( net, metal5, xTarget, yContact, DbU_lambda(2), DbU_lambda(2) ) 
            layer = metal5
          else :
            Contact ( net, via5, xContact, yContact, DbU_lambda(2), DbU_lambda(2) )
            gridContact = Contact ( net, via5, xTarget, yContact, DbU_lambda(2), DbU_lambda(2) ) 
            layer = metal6
          Horizontal( gridContact, plugContact, layer, gridContact.getY(), DbU_lambda(2) )
        else :
          gridContact = Contact ( net, via5, xTarget, yContact, DbU_lambda(2), DbU_lambda(2) ) 
      else:
        if yDistance != 0 :
          if abs(yDistance) <= DbU_lambda(3) :
            layer = metal6
            gridContact = Contact ( net, metal6, xContact, yTarget, DbU_lambda(2), DbU_lambda(2) ) 
            Contact ( net, via5, xContact, yContact, DbU_lambda(2), DbU_lambda(2) ) 
          else :
            gridContact = Contact ( net, via5, xContact, yTarget, DbU_lambda(2), DbU_lambda(2) ) 
            layer = metal5
          Vertical ( gridContact, plugContact, layer, gridContact.getX(), DbU_lambda(2) )
        else :
          gridContact = Contact ( net, via5, xContact, yTarget, DbU_lambda(2), DbU_lambda(2) ) 

  del _Xmin
  del _Ymin 
  del _Xmax 
  del _Ymax   
  del ck_contact_list

############################################
def getNetInstances ( cell, net, transformation) :
  '''This function is used to cover instances with standard model, which are connected to "net"
  transformation is the transformation of the instance which model contains this net''' 

  global ck_contact_list_to_create, ck_contact_list
  global _Xmin, _Ymin, _Xmax, _Ymax
  
  for plug in net.getPlugs():   # parcourir les plugs de ce net 
    ins  = plug.getInstance()
    
    # Si c est une instance de type leaf 
    if ins.isLeaf() :
      if ins.getPlacementStatus() == PlacementStatusUNPLACED :
        raise ErrorMessage(2,"getNetInstances : instance %s is unplaced" % str(ins.getName()))
      else :
        if not isPad ( ins ) :
          # get transformation final de cette instance 
          ins_transformation = transformation.getTransformation ( ins.getTransformation() )

          nbSeg = 0
          for segment in plug.getMasterNet().getSegments():
            layer = segment.getLayer() 
            if NetExternalComponents.isExternal(segment) \
               and (  re.search ( "METAL1", layer.getName() ) \
                   or re.search ( "METAL2", layer.getName() ) ):
              # avoir x ,y de contact a placer
              _x = ins_transformation.getX ( segment.getSourceX(), segment.getSourceY() )
              _y = ins_transformation.getY ( segment.getSourceX(), segment.getSourceY() )
               
              #print ins, ":", segment, ",", nbSeg, ",", _x, ",", _y
              nbSeg += 1
              ck_contact_list_to_create.append (  (_x, _y) ) 
              
              ck_contact_list.append ( (_x, _y) )           
 
              newbox = ins_transformation.getBox ( ins.getMasterCell().getAbutmentBox() )              # get new box 
              Height = newbox.getHeight()
              Width  = newbox.getWidth()
              Xmin   = newbox.getXMin()
              Ymin   = newbox.getYMin()
              Xmax   = newbox.getXMax()
              Ymax   = newbox.getYMax()
              #print " placer contact in ", _x, " ",  _y  , " in the net ", plug.getMasterNet().getName() ,
              #print " of instance ", plug.getInstance().getName() , " in ", Xmin , " ", Ymin , 
              #print " of model ", plug.getInstance().getMasterCell().getName(), "\n"

              # Positionner la grille
              if ( Xmin < _Xmin ) or ( _Xmin == None ) : _Xmin = Xmin
              if ( Ymin < _Ymin ) or ( _Ymin == None ) : _Ymin = Ymin
              if ( Xmax > _Xmax ) or ( _Xmax == None ) : _Xmax = Xmax
              if ( Ymax > _Ymax ) or ( _Ymax == None ) : _Ymax = Ymax
              break

          if not nbSeg :
            raise ErrorMessage(2,"getNetInstances : net %s in model %s does not have a segment\n" % ( str ( plug.getMasterNet().getName()), str(ins.getMasterCell().getName()) ) )

          if ( not ck_contact_list ) : print "Error in function getNetInstances : no segment found"
              
    else :
      if ins.getPlacementStatus() == PlacementStatusUNPLACED :
        raise ErrorMessage(2,"getNetInstances : instance %s is unplaced" % str(ins.getName()))
      else :        
        getNetInstances ( cell, plug.getMasterNet(), transformation.getTransformation ( ins.getTransformation () ))
    
############################
def getNonCLayer ( layer ) :
  '''This function returns the nonC layer corresponding to the one given as argument'''
  
  metal1 = getDataBase ().getTechnology ().getLayer ( "METAL1" )
  metal2 = getDataBase ().getTechnology ().getLayer ( "METAL2" )
  metal3 = getDataBase ().getTechnology ().getLayer ( "METAL3" )
  metal4 = getDataBase ().getTechnology ().getLayer ( "METAL4" )
  metal5 = getDataBase ().getTechnology ().getLayer ( "METAL5" )
  metal6 = getDataBase ().getTechnology ().getLayer ( "METAL6" )

  if re.search ( "CMETAL1", str ( layer.getName() ) ) : return metal1
  if re.search ( "CMETAL2", str ( layer.getName() ) ) : return metal2
  if re.search ( "CMETAL3", str ( layer.getName() ) ) : return metal3
  if re.search ( "CMETAL4", str ( layer.getName() ) ) : return metal4
  if re.search ( "CMETAL5", str ( layer.getName() ) ) : return metal5
  if re.search ( "CMETAL6", str ( layer.getName() ) ) : return metal6
  
  return layer    

######################################################
def Segment ( component1, component2, layer, width ) :
  '''This function creates a segment linking component1 and component2'''
  
  if   component1.getX() == component2.getX() : return Vertical   ( component1, component2, layer, component1.getX(), width )
  elif component1.getY() == component2.getY() : return Horizontal ( component1, component2, layer, component1.getY(), width )
  else:
    raise ErrorMessage(2,"Segment : the components must be horizontaly or verticaly aligned.")

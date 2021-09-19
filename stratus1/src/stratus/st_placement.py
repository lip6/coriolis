
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
# |  Py Module   :       "./st_placement.py"                        |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from Hurricane   import *
from st_net      import *
from util_Place  import *

global nbCkBuf
nbCkBuf = 0


###########
def Place ( ins, sym, ref, plac = FIXED, cell = None ) :
  global MYPITCH, MYSLICE

  # Error if x and y found not permitted :
  if ref._x % MYPITCH :
    err = "\n[Stratus ERROR] Place : " + ins._name + " : coordinate x " \
          + DbU.getValueString(ref._x) + " is not a mutiple of PITCH " \
          + DbU.getValueString(MYPITCH) + ".\n"
    raise Exception ( err )

  if ref._y % MYSLICE :
    message = '\n[ERROR] Stratus.Model.Place(): For instance "'  \
            + ins._name + '", coordinate Y ' + DbU.getValueString(ref._y) + ' (' + str(ref._y) + ')' \
            + " is not a mutiple of slice " + DbU.getValueString(MYSLICE) + '.'
    raise Exception( message )
                                            
  # Error message : if ref is not a reference
  if str ( ref.__class__ ) != "<class 'st_ref.XY'>" :
    raise Exception ( "\n[Stratus ERROR] Place : wrong argument for placement, the coordinates must be put in a XY object.\n" )
    
  # Error message if plac is not one of the permitted values
  if ( plac != PLACED ) and ( plac != FIXED ) : raise Exception ( "\n[Stratus ERROR] Place : wrong argument for placement type.\n" )
    
 #placement ( ins, sym, getUnit(ref._x), getUnit(ref._y), plac, cell = cell )
  placement ( ins, sym, ref._x, ref._y, plac, cell = cell )
    
###############
def PlaceTop ( ins, symetry, offsetX = 0, offsetY = 0, plac = FIXED ) :
  global CELLS
  from st_model import CELLS

  global MYPITCH, MYSLICE
  
  cell = CELLS[-1]

  if offsetX % MYPITCH :
    err = "\n[Stratus ERROR] PlaceTop : " + ins._name + " : offsetX is not a mutiple of PITCH.\n"
    raise Exception ( err )
  
  if offsetY % MYSLICE :
    err = "\n[Stratus ERROR] PlaceTop : " + ins._name + " : offsetY is not a mutiple of SLICE.\n"
    raise Exception ( err )

  if ( plac != PLACED ) and ( plac != FIXED ) :
    err = "\n[Stratus ERROR] PlaceTop : " + ins._name + " : wrong argument for placement type.\n"
    raise Exception ( err )
  
  if cell._insref == None :
    err = "\n[Stratus ERROR] PlaceTop : " + ins._name + " : no previous instance.\n"
    raise Exception ( err )
    
  x = cell._insref._x
  y = cell._insref._y

  if droite ( cell._insref._sym ) :
    if vertical ( cell._insref._sym ) : x -= width  ( cell._insref )
    else                              : x -= height ( cell._insref )
    
  if bas ( cell._insref._sym ) :
    if vertical ( cell._insref._sym ) : y += height ( cell._insref )
    else                              : y += width  ( cell._insref )
   
  placement ( ins, symetry, x + offsetX, y + offsetY, plac )
    
#################
def PlaceRight ( ins, symetry, offsetX = 0, offsetY = 0, plac = FIXED ) :
  global CELLS
  from st_model import CELLS
  
  global MYPITCH, MYSLICE 
  
  cell = CELLS[-1]

  if offsetX % MYPITCH :
    message = '\n[ERROR] Stratus.Model.PlaceRight(): For instance "'  \
            + ins._name + '", offsetX ' + DbU.getValueString(offsetX) \
            + " is not a mutiple of pitch " + DbU.getValueString(MYPITCH) + '.'
    raise Exception( message )
  
  if offsetY % MYSLICE :
    message = '\n[ERROR] Stratus.Model.PlaceRight(): For instance "'  \
            + ins._name + '", offsetY ' + DbU.getValueString(offsetX) \
            + " is not a mutiple of slice " + DbU.getValueString(MYSLICE) + '.'
    raise Exception( message )
  
  if ( plac != PLACED ) and ( plac != FIXED ) :
    err = "\n[Stratus ERROR] PlaceRight : " + ins._name + " : wrong argument for placement type.\n"
    raise Exception ( err )
  
  if cell._insref == None :
    err = "\n[Stratus ERROR] PlaceRight : " + ins._name + " : no previous instance.\n"
    raise Exception ( err )
    
  x = cell._insref._x
  y = cell._insref._y

  if gauche ( cell._insref._sym ):
    if vertical ( cell._insref._sym ) : x += width  ( cell._insref )
    else                              : x += height ( cell._insref )
    
  if haut ( cell._insref._sym ) :
    if vertical ( cell._insref._sym ) : y -= height ( cell._insref )
    else                              : y -= width  ( cell._insref )
    
  placement ( ins, symetry, x + offsetX, y + offsetY, plac )

################
def PlaceLeft ( ins, symetry, offsetX = 0, offsetY = 0, plac = FIXED ) :
  global CELLS
  from st_model import CELLS
  
  global MYPITCH, MYSLICE
  
  cell = CELLS[-1]

  if offsetX % MYPITCH :
    err = "\n[Stratus ERROR] PlaceLeft : " + ins._name + " : offsetX is not a mutiple of PITCH.\n"
    raise Exception ( err )
  
  if offsetY % MYSLICE :
    err = "\n[Stratus ERROR] PlaceLeft : " + ins._name + " : offsetY is not a mutiple of SLICE.\n"
    raise Exception ( err )
  
  if ( plac != PLACED ) and ( plac != FIXED ) :
    err = "\n[Stratus ERROR] PlaceLeft : " + ins._name + " : wrong argument for placement type.\n"
    raise Exception ( err )
  
  if cell._insref == None :
    err = "\n[Stratus ERROR] PlaceLeft : " + ins._name + " : no previous instance.\n"
    raise Exception ( err )
    
  x = cell._insref._x
  y = cell._insref._y

  sym = transformation ( symetry )

  if droite ( cell._insref._sym ) :
    if vertical ( cell._insref._sym )  : x -= width  ( cell._insref )
    else                               : x -= height ( cell._insref )
  
  if haut ( cell._insref._sym ) :
    if  vertical ( cell._insref._sym ) : y -= height ( cell._insref )
    else                               : y -= width  ( cell._insref )
    
  placement ( ins, symetry, x - offsetX, y + offsetY, plac, fonction = "Left" )

##################
def PlaceBottom ( ins, symetry, offsetX = 0, offsetY = 0, plac = FIXED ) :
  global CELLS
  from st_model import CELLS
  
  global MYPITCH, MYSLICE
  
  cell = CELLS[-1]

  if offsetX % MYPITCH :
    err = "\n[Stratus ERROR] PlaceBottom : " + ins._name + " : offsetX is not a mutiple of PITCH.\n"
    raise Exception ( err )
  
  if offsetY % MYSLICE :
    err = "\n[Stratus ERROR] PlaceBottom : " + ins._name + " : offsetY is not a mutiple of SLICE.\n"
    raise Exception ( err )
  
  if ( plac != PLACED ) and ( plac != FIXED ) :
    err = "\n[Stratus ERROR] PlaceBottom : " + ins._name + " : wrong argument for placement type.\n"
    raise Exception ( err )
  
  if cell._insref == None :
    err = "\n[Stratus ERROR] PlaceBottom : " + ins._name + " : no previous instance.\n"
    raise Exception ( err )
    
  x = cell._insref._x
  y = cell._insref._y

  sym = transformation ( symetry )

  if droite ( cell._insref._sym ) :
    if vertical ( cell._insref._sym )  : x -= width  ( cell._insref )
    else                               : x -= height ( cell._insref )
  
  if haut ( cell._insref._sym ) :
    if vertical ( cell._insref._sym )  : y -= height ( cell._insref )
    else                               : y -= width  ( cell._insref )
 
  placement ( ins, symetry, x + offsetX, y - offsetY, plac, fonction = "Bottom" )

#################
def SetRefIns ( ins ) :
  global CELLS
  from st_model import CELLS

  from util_Place import UNPLACED
  
  cell = CELLS[-1]

  # Error : SetRefIns on a non existing instance
  if not ins : raise Exception ( "\n[Stratus ERROR] SetRefIns : the instance doesn't exist.\n" )

  # Error : SetRefIns on a non placed instance
  if ins._plac == UNPLACED :
    err = "\n[Stratus ERROR] SetRefIns : the instance " + ins._name + " is not placed.\n"
    raise Exception ( err )

  cell._insref = ins

############
def DefAb ( ref1, ref2 ) :
  global CELLS
  from st_model import CELLS

  global MYSLICE, MYPITCH
  
  cell = CELLS[-1]

  UpdateSession.open ()
    
  # Error message : if ref are not references
  if    ( str ( ref1.__class__ ) != "<class 'st_ref.XY'>" ) \
     or ( str ( ref2.__class__ ) != "<class 'st_ref.XY'>" ) :
    err = "\n[Stratus ERROR] DefAb : wrong argument, the coordinates must be put in a XY object.\n"
    raise Exception ( err )

  x1 = ref1._x
  y1 = ref1._y
  x2 = ref2._x
  y2 = ref2._y

  if not cell._hur_cell :
    err = "\n[Stratus ERROR] Layout : The hurricane cell has not been created.\n" \
        + "One has to use HurricanePlug before creating the layout.\n"
    raise Exception ( err )

  if x1 % MYPITCH or y1 % MYSLICE or x2 % MYPITCH or y2 % MYSLICE :
    err = "\nError in DefAb : Coordinates of an abutment Box in y must be multiple of the slice.\n" \
        + "                 : Coordinates of an abutment Box in x must be multiple of the pitch.\n"
    raise Exception ( err )
    
  boite = cell._hur_cell.getAbutmentBox ()

  if not ( boite.isEmpty() ) :
    err = "\n[Stratus ERROR] DefAb : an abutment box already exists for cell : " \
          + str ( cell._name ) \
          + ". Maybe you should use ResizeAb function.\n"
    raise Exception ( err )

  cell._hur_cell.setAbutmentBox ( Box ( x1, y1, x2, y2 ) )
    
  UpdateSession.close ()
      
############
def ResizeAb ( dx1, dy1, dx2, dy2 ) :
  global CELLS
  from st_model import CELLS

 #print( "ResizeAb()", DbU.getValueString(dx1) \
 #                   , DbU.getValueString(dy1) \
 #                   , DbU.getValueString(dx2) \
 #                   , DbU.getValueString(dy2) )

  global MYSLICE, MYPITCH
  
  cell = CELLS[-1]

  hurCell = cell._hur_cell
  ab      = hurCell.getAbutmentBox()

  UpdateSession.open ()

  if dx1 % MYPITCH or dy1 % MYSLICE or dx2 % MYPITCH or dy2 % MYSLICE :
    err = "\n[Stratus ERROR] ResizeAb : Coordinates of an abutment Box in y must be multiple of the slice.\n" \
        + "                           : Coordinates of an abutment Box in x must be multiple of the pitch.\n"
    raise Exception ( err )

  old_xmin =  ab.getXMin()
  old_xmax =  ab.getXMax()
  old_ymin =  ab.getYMin()
  old_ymax =  ab.getYMax()

  new_xmin = old_xmin - dx1
  new_ymin = old_ymin - dy1
  new_xmax = old_xmax + dx2
  new_ymax = old_ymax + dy2
    
  if new_xmin >= new_xmax :
    err = "\n[Stratus ERROR] ResizeAb : one of the values of dx1 or dx2  is incompatible with the size of the abutment box.\n"
    raise Exception ( err )
    
  if new_ymin >= new_ymax :
    err = "\n[Stratus ERROR] ResizeAb : one of the values of dy1 or dy2  is incompatible with the size of the abutment box.\n"
    raise Exception ( err )
    
  hurCell.setAbutmentBox ( Box ( new_xmin, new_ymin, new_xmax, new_ymax ) )
  
  UpdateSession.close ()
    
############
def DefInstanceAb ( instance, x1, y1, x2, y2 ) :

  cell = instance._hur_instance.getMasterCell()

  boite = cell.getAbutmentBox()

  if not ( boite.isEmpty() ) :
    err =  "\n[Stratus ERROR] DefInstanceAb : an abutment box already exists. Maybe you should use ResizeAb function.\n"
    raise Exception ( err )

  if x1 % MYPITCH or y1 % MYSLICE or x2 % MYPITCH or y2 % MYSLICE :
    err = "\nError in DefAb : Coordinates of an abutment Box in y must be multiple of the slice.\n" \
        + "                 : Coordinates of an abutment Box in x must be multiple of the pitch.\n"
    raise Exception ( err )
    
  cell.setAbutmentBox ( Box ( x1, y1, x2, y2 ) )
    
  
####################################
##            placement           ##
####################################
def placement ( st_inst, sym, x, y, plac = FIXED, cell = None, fonction = None ) :
  global CELLS
  from st_model import CELLS
  
  if not cell : cell = CELLS[-1]

  # Error : placement of a non existing instance
  if not st_inst : raise Exception ( "\n[Stratus ERROR] Placement : the instance doesn't exist.\n" )

  # Error : st_inst is not an instance
  if str ( st_inst.__class__ ) != "<class 'st_instance.Inst'>" :
    err = "\n\n[Stratus ERROR] Placement : the first argument " + st_inst + " is not an instance.\n"
    raise Exception ( err )  

  # Hurricane instance
  hur_inst = st_inst._hur_instance

  # Error : if the hurricane instance does not exist
  if not hur_inst :
    err = "\n[Stratus ERROR] Layout : The hurricane instance of " + st_inst._name + " has not been created.\n" \
        + "One has to use HurricanePlug before creating the layout.\n"
    raise Exception ( err )
 
  # Error : if the instance is already placed
  if hur_inst.getPlacementStatus() == Instance.PlacementStatus.FIXED :
    err = "\n[Stratus ERROR] Placement : the instance " + st_inst._name + " is already placed.\n"
    raise Exception ( err )      
  
  ##### Legalization of the symetry #####
  st_inst._sym = transformation ( sym )
    
  ##### For PlaceBottom and PlaceLeft : need the size of the instance being placed #####
  if fonction == "Left" :
    if     vertical ( st_inst._sym ) : x -= width  ( st_inst )
    elif horizontal ( st_inst._sym ) : x -= height ( st_inst )
  elif fonction == "Bottom" :
    if     vertical ( st_inst._sym ) : y -= height ( st_inst )
    elif horizontal ( st_inst._sym ) : y -= width  ( st_inst )
  
  ##### Initialisation for relative placement #####
  cell._insref = st_inst

  ##### Placement #####
  if st_inst._sym == Transformation.Orientation.ID :
    st_inst._x = x
    st_inst._y = y
    
  elif st_inst._sym == Transformation.Orientation.MX :
    abtemp     = ab ( st_inst, cell )
  
    st_inst._x = x + abtemp.getWidth ()
    st_inst._y = y
    
  elif st_inst._sym == Transformation.Orientation.MY :
    abtemp     = ab ( st_inst, cell )
  
    st_inst._x = x
    st_inst._y = y + abtemp.getHeight ()
    
  elif st_inst._sym == Transformation.Orientation.R2 :
    abtemp     = ab ( st_inst, cell )
  
    st_inst._x = x + abtemp.getWidth  ()
    st_inst._y = y + abtemp.getHeight ()
    
  elif st_inst._sym == Transformation.Orientation.R1 :
    abtemp     = ab ( st_inst, cell )
  
    st_inst._x = x + abtemp.getHeight ()
    st_inst._y = y
    
  elif st_inst._sym == Transformation.Orientation.R3 :
    abtemp     = ab ( st_inst, cell )
  
    st_inst._x = x
    st_inst._y = y + abtemp.getWidth ()
    
  elif st_inst._sym == Transformation.Orientation.YR :
    st_inst._x = x
    st_inst._y = y
    
  elif st_inst._sym == Transformation.Orientation.XR :
    abtemp     = ab ( st_inst, cell )

    st_inst._x = x + abtemp.getHeight ()
    st_inst._y = y + abtemp.getWidth  ()

  else :
    raise Exception ( "\n[Stratus ERROR] Placement : wrong transformation.\n" )

  # if the abutment box is not at 0 0  FIXME
  if st_inst._sym == Transformation.Orientation.MY :
    x = st_inst._x + hur_inst.getAbutmentBox().getXMin()
    y = st_inst._y + hur_inst.getAbutmentBox().getYMin()
  else :
    x = st_inst._x - hur_inst.getAbutmentBox().getXMin()
    y = st_inst._y - hur_inst.getAbutmentBox().getYMin()

  UpdateSession.open()
    
  hur_inst.setTransformation ( Transformation ( x, y, st_inst._sym ) )

  if   plac == PLACED :
    cell._hur_cell.setAbutmentBox ( cell._hur_cell.getAbutmentBox ().merge ( hur_inst.getAbutmentBox () ) )
    hur_inst.setPlacementStatus   ( Instance.PlacementStatus.PLACED )
  elif plac == FIXED :
    cell._hur_cell.setAbutmentBox ( cell._hur_cell.getAbutmentBox ().merge ( hur_inst.getAbutmentBox () ) )
    hur_inst.setPlacementStatus   ( Instance.PlacementStatus.FIXED )
  elif plac == UNPLACED :
    hur_inst.setPlacementStatus   ( Instance.PlacementStatus.UNPLACED )
  else :
    raise Exception ( "\n[Stratus ERROR] Placement : wrong argument for type of placement.\n" )

  st_inst._plac = plac  

  UpdateSession.close()


## Two names for the symetry ##
def transformation ( symetry ) :
  if   symetry == NOSYM or symetry == Transformation.Orientation.ID : transf = Transformation.Orientation.ID
  elif symetry == SYM_X or symetry == Transformation.Orientation.MX : transf = Transformation.Orientation.MX
  elif symetry == SYM_Y or symetry == Transformation.Orientation.MY : transf = Transformation.Orientation.MY
  elif symetry == SYMXY or symetry == Transformation.Orientation.R2 : transf = Transformation.Orientation.R2
  elif symetry == ROT_P or symetry == Transformation.Orientation.R1 : transf = Transformation.Orientation.R1
  elif symetry == ROT_M or symetry == Transformation.Orientation.R3 : transf = Transformation.Orientation.R3
  elif symetry == SY_RP or symetry == Transformation.Orientation.YR : transf = Transformation.Orientation.YR
  elif symetry == SY_RM or symetry == Transformation.Orientation.XR : transf = Transformation.Orientation.XR
  else :
    err = "\n[Stratus ERROR] Placement :Illegal transformation.\n"
    raise Exception ( err )
    
  return transf
  
## How to get the abutment box ##
def ab ( ins, cell = None ) :
  global CELLS
  from st_model import CELLS
  
  if not cell : cell = CELLS[-1]

  return cell._hur_cell.getInstance ( ins._name ).getMasterCell ().getAbutmentBox ()

## the height of the abutment box ##
def height ( ins ) :
  return ab ( ins ).getHeight ()
        
## the width of the abutment box ##
def width ( ins ) :
  return ab ( ins ).getWidth ()

## Test of symetry ##
def vertical ( sym ) :
  return sym == Transformation.Orientation.ID or sym == Transformation.Orientation.MX or sym == Transformation.Orientation.MY or sym == Transformation.Orientation.R2
  
def horizontal ( sym ) :
  return sym == Transformation.Orientation.R1 or sym == Transformation.Orientation.R3 or sym == Transformation.Orientation.YR or sym == Transformation.Orientation.XR    
  
def bas ( sym ) :
  return sym == Transformation.Orientation.ID or sym == Transformation.Orientation.MX or sym == Transformation.Orientation.R1 or sym == Transformation.Orientation.YR
  
def haut ( sym ) :
  return sym == Transformation.Orientation.R2 or sym == Transformation.Orientation.MY or sym == Transformation.Orientation.R3 or sym == Transformation.Orientation.XR 
  
def gauche ( sym ) :
  return sym == Transformation.Orientation.ID or sym == Transformation.Orientation.MY or sym == Transformation.Orientation.R3 or sym == Transformation.Orientation.YR   
  
def droite ( sym ) :
  return sym == Transformation.Orientation.R2 or sym == Transformation.Orientation.MX or sym == Transformation.Orientation.R1 or sym == Transformation.Orientation.XR


#################
## ClockBuffer ##
#################
class ClockBuffer :
  def __init__ ( self, netname ) :
    global CELLS
    from st_model import CELLS
    global nbCkBuf
    self.cell = CELLS[-1]
    
    self.net = self.cell._hur_cell.getNet ( netname )
    self.ck_b = Signal ( "ck_b%d"%nbCkBuf, 1 )

    modelMasterCell = CRL.AllianceFramework.get().getCell ( "buf_x2", CRL.Catalog.State.Views )
    if not modelMasterCell :
      err = "Stratus Error : ClockBuffer : Cannot find model cell : buf_x2 in database !\n"
      raise Exception ( err )

    inst = Instance.create ( self.cell._hur_cell, "ck_buffer%d"%nbCkBuf, modelMasterCell )
    nbCkBuf += 1
  
    ##### Connection #####
    plugI = inst.getPlug ( modelMasterCell.getNet ( "i" ) )
    plugI.setNet ( self.net )
    
    plugQ = inst.getPlug ( modelMasterCell.getNet ( "q" ) )
    plugQ.setNet ( self.ck_b._hur_net[0] )
    
    plugGround = inst.getPlug ( iter(modelMasterCell.getGroundNets()).__next__() )
    plugGround.setNet ( iter(self.cell._hur_cell.getGroundNets()).__next__() )

    plugPower = inst.getPlug ( iter(modelMasterCell.getPowerNets()).__next__() )
    plugPower.setNet ( iter(self.cell._hur_cell.getPowerNets()).__next__() )

  def AddFF ( self, netname ) :
    net = self.cell._hur_cell.getNet ( netname )
    if not net :
        err = "AddFF Error net : " + netname + " not found"
        raise Exception ( err )

    instDrive = None
    for plug in net.getPlugs():
      if plug.getMasterNet().getDirection() == Net.Direction.OUT :
        instDrive = plug.getInstance()
        break

    if instDrive == None :
      err = "AddFF Error no drive instance found for net " + netname + ".\n"
      raise Exception ( err )
    
    masterCell = instDrive.getMasterCell()
    ## ici il vaudrait mieux faire une recherche sur l'ensemble des plugs de l'instDrive et trouver celle accrochee au self.net ##
    netCk = masterCell.getNet ( "ck" )
    if instDrive.getPlug ( netCk ).getNet() != self.net :
      err = "Stratus Error : AddFF : driver Instance of net " + netname + " is not connected to signal " + str ( self.net.getName() ) + ".\n"
      raise Exception ( err )

    instDrive.getPlug ( netCk ).setNet ( self.ck_b._hur_net[0] )

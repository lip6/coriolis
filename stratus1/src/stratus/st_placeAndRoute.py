
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
# |  Author      :                      Sophie BELLOEIL             |
# |  E-mail      :              Sophie.Belloeil@lip6.fr             |
# | =============================================================== |
# |  Py Module   :       "./st_placeAndRoute.py"                    |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



import CRL
from placeandroute import *
from st_net        import *

import re, string

global nbCkBuf
nbCkBuf = 0


###############
## PlaceGlue ##
###############
def PlaceGlue ( cell = None, tool = "mistral", greedy = True, nparts = 0 ) :

  if not cell : 
    global CELLS
    from st_model import CELLS
  
    cell = CELLS[-1]
  
  if isinstance ( cell, Model ) : # FIXME : a changer, mettre toujours un model et faire utiliser aux etudiants de l'annee prochaine getModel
    pyPlaceGlue ( cell._hur_cell )

  if isinstance ( cell, Inst ) :
    pyPlaceGlue ( cell._st_masterCell._hur_cell )


##################
## PlaceCentric ##
##################
def PlaceCentric ( instance ) :
  global CELLS
  from st_model import CELLS

  cell = CELLS[-1]
    
  pyPlaceCentric ( cell._hur_cell, instance._hur_instance )

######################
## AlimVerticalRail ##
######################
def AlimVerticalRail ( coordonnee ) :
  global CELLS
  from st_model import CELLS

  cell = CELLS[-1]
  
  pyAlimVerticalRail ( cell._hur_cell, coordonnee )

########################
## AlimHorizontalRail ##
########################
def AlimHorizontalRail ( coordonnee ) :
  global CELLS
  from st_model import CELLS

  cell = CELLS[-1]
      
  pyAlimHorizontalRail ( cell._hur_cell, coordonnee )

####################
## AlimConnectors ##
####################
def AlimConnectors () :
  global CELLS
  from st_model import CELLS

  cell = CELLS[-1]
      
  pyAlimConnectors ( cell._hur_cell )

#############
## RouteCk ##
#############
def RouteCk ( net ) :
  global CELLS
  from st_model import CELLS

  cell = CELLS[-1]
    
  pyRouteCk ( cell._hur_cell, net._hur_net[0] )

#################
## GlobalRoute ##
#################
def GlobalRoute ( cell = None ):
  if not cell: 
    global CELLS
    from st_model import CELLS

    cell = CELLS[-1]

    if isinstance ( cell, Model ): # FIXME : a changer, mettre toujours un model et faire utiliser aux etudiants de l'annee prochaine getModel
       pyGlobalRoute ( cell._hur_cell )
    if isinstance ( cell, Inst ):
       pyGlobalRoute ( cell._st_masterCell._hur_cell )
  return

###################
## DetailRoute ##
###################
def DetailRoute ( cell = None ):
  if not cell: 
    global CELLS
    from st_model import CELLS

    cell = CELLS[-1]

    if isinstance ( cell, Model ) : # FIXME : a changer, mettre toujours un model et faire utiliser aux etudiants de l'annee prochaine getModel
       pyDetailRoute ( cell._hur_cell )
    if isinstance ( cell, Inst ) :
       pyDetailRoute ( cell._st_masterCell._hur_cell )
  return

##########################
## TimingStaticAnalysis ##
##########################
#def TimingStaticAnalysis ( cell = None ):
#  if not cell: 
#    global CELLS
#    from st_model import CELLS
#
#    cell = CELLS[-1]
#
#    if isinstance ( cell, Model ): # FIXME : a changer, mettre toujours un model et faire utiliser aux etudiants de l'annee prochaine getModel
#       pyTimingStaticAnalysis ( cell._hur_cell )
#    if isinstance ( cell, Inst ):
#       pyTimingStaticAnalysis ( cell._st_masterCell._hur_cell )
#  return


##############
## PadNorth ##
##############
def PadNorth ( *args ) :
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]
  
  hur_core = cell.getCore()

  hur_args = []
  for arg in args :
    if not arg :
      raise Exception ( "\n[Stratus ERROR] PadNorth : one instance doesn't exist.\n" )
    if str ( arg.__class__ ) != "<class 'st_instance.Inst'>" :
      raise Exception ( "\n[Stratus ERROR] PadNorth : one argument is not an instance.\n" )    

    hur_args.append ( arg._hur_instance )
 
  pyPadNorth ( cell._hur_cell, hur_core, hur_args )

##############
## PadSouth ##
##############
def PadSouth ( *args ) :
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]
  
  hur_core = cell.getCore()

  hur_args = []
  for arg in args :
    if not arg :
      raise Exception ( "\n[Stratus ERROR] PadSouth : one instance doesn't exist.\n" )
    if str ( arg.__class__ ) != "<class 'st_instance.Inst'>" :
      raise Exception ( "\n[Stratus ERROR] PadSouth : one argument is not an instance.\n" )   

    hur_args.append ( arg._hur_instance )
 
  pyPadSouth ( cell._hur_cell, hur_core, hur_args )

#############
## PasEast ##
#############
def PadEast ( *args ) :
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]
  
  hur_core = cell.getCore()

  hur_args = []
  for arg in args :
    if not arg :
      raise Exception ( "\n[Stratus ERROR] PadEast : one instance doesn't exist.\n" )
    if str ( arg.__class__ ) != "<class 'st_instance.Inst'>" :
      raise Exception ( "\n[Stratus ERROR] PadEast : one argument is not an instance.\n" )    

    hur_args.append ( arg._hur_instance )
 
  pyPadEast ( cell._hur_cell, hur_core, hur_args )

#############
## PadWest ##
#############
def PadWest ( *args ) :
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]
    
  hur_core = cell.getCore()

  hur_args = []
  for arg in args :
    if not arg :
      raise Exception ( "\n[Stratus ERROR] PadWest : one instance doesn't exist.\n" )
    if str ( arg.__class__ ) != "<class 'st_instance.Inst'>" :
      raise Exception ( "\n[Stratus ERROR] PadWest : one argument is not an instance.\n" ) 

    hur_args.append ( arg._hur_instance )
 
  pyPadWest ( cell._hur_cell, hur_core, hur_args )

###############
## PowerRing ##
###############
def PowerRing ( n ) : 
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]
  
  if n < 3 : raise Exception ( "\n[Stratus ERROR] : PowerRing : must have at least 3 pairs of vdd/vss rings\n" )
  
  hur_core = cell.getCore()
    
  pyPowerRing ( cell._hur_cell, hur_core, n )


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

    inst = Instance ( self.cell._hur_cell, "ck_buffer%d"%nbCkBuf, modelMasterCell )
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
      if plug.getMasterNet().getDirection() == DirectionOUT :
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

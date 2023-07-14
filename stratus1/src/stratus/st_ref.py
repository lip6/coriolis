
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
# |  Py Module   :       "./st_ref.py"                              |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from   Hurricane   import *
from   util_Place  import *
from   ref         import *
import types

###########
class XY :
  def __init__ ( self, x, y ) :
    self._x = x
    self._y = y

##################################
def GetRefXY ( pathname, refname ) :
  global FRAMEWORK, CELLS
  from st_model import FRAMEWORK, CELLS

  cell = CELLS[-1]
    
  # Check arguments type
  if not isinstance(pathname,str):
    err = "\n[Stratus ERROR] GetRefXY : The instance's path must be put with a string.\n"
    raise Exception ( err )
    
  if not isinstance(refname,str):
    err = "\n[Stratus ERROR] GetRefXY : The reference must be done with it's name : a string.\n"
    raise Exception ( err )

  ( x, y ) = pyGetXY ( cell._hur_cell, pathname, refname )    

  return XY ( x, y )
  
############################
def PlaceRef ( ref, name ) :
  global FRAMEWORK, CELLS
  from st_model import FRAMEWORK, CELLS

  cell = CELLS[-1]
  
  # Check arguments type
  if ( str ( ref.__class__ ) != "<class 'st_ref.XY'>" ) :
    err = "\n[Stratus ERROR] PlaceRef : Wrong argument, the coordinates of the reference must be put in a XY object.\n"
    raise Exception ( err )
    
  if not isinstance(name,str):
    err = "\n[Stratus ERROR] PlaceRef : Argument layer must be a string.\n"
    raise Exception ( err )
   
  pyPlaceRef ( cell._hur_cell, name, ref._x, ref._y )

###################################################
def PlaceContact ( net, layer, ref, width, height ) :
  global FRAMEWORK
  from st_model import FRAMEWORK

  # Check arguments type
  if not isinstance(layer,str):
    err = "\n[Stratus ERROR] PlaceContact : Argument layer must be a string.\n"
    raise Exception ( err )
  myLayer = getDataBase().getTechnology().getLayer ( layer )
#  if not ( myLayer ) :
#    err = "\n[Stratus ERROR] PlaceContact : Argument layer does not exist.\n"
#    raise err

  if ( str ( ref.__class__ ) != "<class 'st_ref.XY'>" ) :
    err = "\n[Stratus ERROR] PlaceContact : Wrong argument, the coordinates of the contact must be put in a XY object.\n"
    raise Exception ( err )

  if ( net._arity > 1 ) :
    err = "\n[Stratus ERROR] PlaceContact : Wrong argument, the net must be a 1 bit net.\n"
    raise Exception ( err )

  if net._real_net :
    indice = net._ind - net._real_net._ind
    net = net._real_net
  else :
    indice = 0

  pyPlaceContact ( net._hur_net[indice], myLayer, ref._x, ref._y, width,  height )

###################################################
def PlacePin ( net, layer, direction, ref, width, height ) :
  global FRAMEWORK
  from st_model    import FRAMEWORK

  # Check arguments type
  if not isinstance(layer,str):
    err = "\n[Stratus ERROR] PlacePin : Argument layer must be a string.\n"
    raise Exception ( err )
  # No CALU permitted for Pin
  calu = re.search ( "CALU", layer )
  if calu :
    err = "\n[Stratus ERROR] PlacePin : Illegal layer, CALU not allowed.\n"
    raise Exception ( err )
  myLayer = getDataBase().getTechnology().getLayer ( layer )
#  if not ( myLayer ) :
#    err = "\n[Stratus ERROR] PlacePin : Argument layer does not exist.\n"
#    raise err

  if   direction == UNDEFINED : direct = PinAccessDirectionUNDEFINED
  elif direction == NORTH     : direct = PinAccessDirectionNORTH
  elif direction == SOUTH     : direct = PinAccessDirectionSOUTH
  elif direction == EAST      : direct = PinAccessDirectionEAST
  elif direction == WEST      : direct = PinAccessDirectionWEST
  else :
    err = "\n[Stratus ERROR] PlacePin : Illegal pin access direction. The values are : UNDEFINED, NORTH, SOUTH, EAST, WEST.\n"
    raise Exception ( err )

  if ( str ( ref.__class__ ) != "<class 'st_ref.XY'>" ) :
    err = "\n[Stratus ERROR] PlacePin : Wrong argument, the coordinates of the pin must be put in a XY object.\n"
    raise Exception ( err )

  if ( net._arity > 1 ) :
    err = "\n[Stratus ERROR] PlacePin : Wrong argument, the net must be a 1 bit net.\n"
    raise Exception ( err )

  if net._real_net :
    indice = net._ind - net._real_net._ind
    net = net._real_net
  else :
    indice = 0

  if ( net._ext == False ) :
    err = "\n[Stratus ERROR] PlacePin : Wrong argument, the net must be external.\n"
    raise Exception ( err )

  pyPlacePin ( net._hur_net[indice], direct, PinPlacementStatusFIXED, myLayer, ref._x, ref._y, width, height )

#################################################################
def PlaceSegment ( net, layer, ref1, ref2, width ) :
  global FRAMEWORK
  from st_model import FRAMEWORK

  # Check arguments type
  if not isinstance(layer,str):
    err = "\n[Stratus ERROR] PlaceSegment : Argument layer must be a string.\n"
    raise Exception ( err )

  if    ( str ( ref1.__class__ ) != "<class 'st_ref.XY'>" ) \
     or ( str ( ref2.__class__ ) != "<class 'st_ref.XY'>" ) :
    err = "\n[Stratus ERROR] PlaceSegment : Wrong argument, the coordinates of the segment must be put in XY objects.\n"  
    raise Exception ( err )

  if ( ref1._x != ref2._x ) and ( ref1._y != ref2._y ) :
    err = "\n[Stratus ERROR] PlaceSegment : Segments are vertical or horizontal.\n"
    raise Exception ( err )

  if ( net._arity > 1 ) :
    err = "\n[Stratus ERROR] PlaceSegment : Wrong argument, the net must be a 1 bit net.\n"
    raise Exception ( err )

  if net._real_net :
    indice = net._ind - net._real_net._ind
    net = net._real_net
  else :
    indice = 0

  pyPlaceSegment ( net._hur_net[indice]
                 , getDataBase().getTechnology().getLayer ( layer )
                 , ref1._x, ref1._y
                 , ref2._x, ref2._y
                 , width
                 )

#################################################
def CopyUpSegment ( pathname, netname, newnet ) :
  global FRAMEWORK, CELLS
  from st_model import FRAMEWORK, CELLS

  cell = CELLS[-1]
    
  # Check arguments type
  if not isintance(pathname,str):
    err = "\n[Stratus ERROR] CopyUpSegment : The instance's path must be put with a string.\n"
    raise Exception ( err )
    
  if not isinstance(netname,str):
    err = "\n[Stratus ERROR] CopyUpSegment : The segment must be done with it's name : a string.\n"
    raise Exception ( err )

  if ( newnet._arity > 1 ) :
    err = "\n[Stratus ERROR] PlacePin : Wrong argument, the net must be a 1 bit net.\n"
    raise Exception ( err )

  if newnet._real_net :
    indice = newnet._ind - newnet._real_net._ind
    newnet = newnet._real_net
  else :
    indice = 0

  pyCopyUpSegment ( cell._hur_cell, pathname, netname, newnet._hur_net[indice] )    

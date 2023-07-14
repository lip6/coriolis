
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
# |  Py Module   :       "./st_getrealmodel.py"                     |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x

import Cfg
from st_parser import Parser, InitParser, WeightParser

import os

def InitBV () :
  myP = InitParser()

  mapping = Cfg.getParamString('stratus1.mappingName').asString()
  if not mapping:
    print( '[ERROR] \"stratus1.mappingName\" configuration variable has not been set.' )
  myP.Parse ( mapping )
  return

def GetRealModel ( model ) :
  myP = Parser()

  mapping = Cfg.getParamString('stratus1.mappingName').asString()
  myP.Parse ( mapping )

  return myP._realCell[model], myP._inOut[model]    

def GetWeightTime ( model, value=None ) :
  myP = WeightParser()
    
  mapping = Cfg.getParamString('stratus1.mappingName').asString()
  myP.Parse ( mapping )

  if value == None :  
    if type(myP._weightTime[model]) == dict:
      return myP._weightTime[model]['time']
    else :
      return myP._weightTime[model]
  else:
    return myP._weightTime[model][value]

def GetWeightArea ( model ) :
  myP = WeightParser()
    
  mapping = Cfg.getParamString('stratus1.mappingName').asString()
  myP.Parse ( mapping )
    
  return myP._weightArea[model]

def GetWeightPower ( model ) :
  myP = WeightParser()
    
  if 'STRATUS_MAPPING_NAME' in os.environ :
    myP.Parse ( os.environ['STRATUS_MAPPING_NAME'] )
  else :
    err = "\n[Stratus ERROR] Virtual library : No file found in order to parse.\nCheck STRATUS_MAPPING_NAME.\n"
    raise Exception ( err )        
    
  return myP._weightPower[model]

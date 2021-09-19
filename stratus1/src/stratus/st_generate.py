
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
# |  Py Module   :       "./st_generate.py"                         |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from Hurricane        import *
from CRL              import *

from st_model         import Model, MODELMAP
from st_getrealmodel  import GetRealModel, InitBV
    
import sys, re, types, os

def Generate ( model_name, inst_name, param = {} ) :
  global CELLS, FRAMEWORK, MODEL_MAP
  from st_model import CELLS, FRAMEWORK
  cell = CELLS[-1]

  if '_vbe' in cell.__dict__ : param['behavioral'] = cell._vbe

  # Error : if the model is not a string
  if not isinstance(model_name,str):
    err = "\n[Stratus ERROR] Generate : the model must be described in a string.\n"
    raise Exception ( err )
  if not isinstance(inst_name,str):
    err = "\n[Stratus ERROR] Generate : the model must be described in a string.\n"
    raise Exception ( err )

  # Error : spaces are forbidden
  if re.search ( " ", inst_name ) :
    err = "\n[Stratus ERROR] Generate : " + inst_name + " the name of the model \"" + model_name + "\" can not contain a space.\n"
    raise Exception ( err )
  # Warning : the name can not contain capitalized letters
  if re.search ( "[A-Z]", inst_name ) :
    print( "[Stratus Warning] Generate : Upper case letters are not supported, the name", inst_name, "is lowered." )
    inst_name = inst_name.lower()
  
  # Check if the model does already exist
  hur_masterCell = FRAMEWORK.getCell ( inst_name, Catalog.State.Views )

  st_masterCell = None
  for c in cell._underCells :
    if inst_name == c._name : st_masterCell = c
  
  # Creation of model not found neither in the database nor in the files
  if not ( hur_masterCell or st_masterCell ) :
    cell._model_map[inst_name] = [model_name, param]

    realModele = None
    inout      = None

    global BVg
    from st_parser import BVg
    if BVg == [] : InitBV()
    
    if model_name in BVg : realModele, inout = GetRealModel ( model_name.lower() )

    st_masterCell = cell.ModelCreation ( model_name, inst_name, param, cell._hierarchy+1, realModele, inout )

    if "Properties" in dir ( st_masterCell ) : return st_masterCell.Properties()
    else                                     : return st_masterCell

  else :
    if st_masterCell :
      # MAJ of the hierarchy in _underCells
      CELLS[0]._underCells[st_masterCell] = cell._hierarchy+1
      return st_masterCell

    else:
      return hur_masterCell

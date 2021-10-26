
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
# |    S t r a t u s   -  Netlists/Layouts Description              |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./util_uRom.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


from util_Gen import *


## Local constants ##
#####################
ERROR_INDEX  =  4096

CELLS_ROM2   =     5
CELLS_ROM4   =    17

SIZE_SNAME   =  1024

## Local Variable ##
####################
global LV_tsLeafCell

LV_tsLeafCell = ["dp_rom2_buf", "zero_x0", "buf_x4", "inv_x4", "one_x0", "dp_rom4_buf", "zero_x0", "a2_x4", "a2_x4", "buf_x4", "a2_x4", "buf_x4", "dp_rom4_xr2_x4", "o2_x4", "a2_x4", "dp_rom4_nxr2_x4", "buf_x4",  "o2_x4", "buf_x4", "o2_x4", "o2_x4", "one_x0"]


## Functions Definitions ##
###########################
  
#####################################################################################
def get_index_Cell ( aiSlice, LV_Function, LV_xl0, LV_xl1, LV_xl2 = 0, LV_xl3 = 0 ) :

  index = ERROR_INDEX

  if   LV_Function == DPGEN_ROM2 :
    index  = ( LV_xl1.getxlbit ( aiSlice ) << 1 ) 
    index +=   LV_xl0.getxlbit ( aiSlice )

  elif LV_Function == DPGEN_ROM4 :
    index  = ( LV_xl3.getxlbit ( aiSlice ) << 3 ) 
    index += ( LV_xl2.getxlbit ( aiSlice ) << 2 ) 
    index += ( LV_xl1.getxlbit ( aiSlice ) << 1 ) 
    index +=   LV_xl0.getxlbit ( aiSlice )

  return index

###################################################################
def get_model_Cell ( aiSlice, LV_Function, aiCell, LV_iLeafCell ) :
  global LV_tsLeafCell

  index = ERROR_INDEX

  if LV_Function in ( DPGEN_ROM4, DPGEN_ROM2 ) : index = LV_iLeafCell + 1 + aiCell

  if index >= ERROR_INDEX : return "bug_leaf"

  return LV_tsLeafCell[index]

######################################
def get_model_Buffer ( LV_Function, LV_iLeafCell ) :
  global LV_tsLeafCell

  index = ERROR_INDEX

  if LV_Function in ( DPGEN_ROM4, DPGEN_ROM2 ) : index = LV_iLeafCell

  if index == ERROR_INDEX : return "bug_buff"

  return LV_tsLeafCell[index]

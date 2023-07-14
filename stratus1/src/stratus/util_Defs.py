
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
# |  Py Module   :       "./util_Defs.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


from Hurricane    import DbU
from st_placement import Place, NOSYM, SYM_Y, FIXED
from st_ref       import XY
from util_Gen     import F_MSB_FIRST

Y_SLICE        = DbU.fromLambda(50.0)

# for DpgenAdder
F_ADDER_CIN    = 0x00000001
F_ADDER_SIGN   = 0x00000002
F_ADDER_OVER   = 0x00000004



################
## slice_to_y ##
################
def slice_to_y ( s ) :
  return s * Y_SLICE


##################
## slice_to_sym ##
##################
def slice_to_sym ( s ) :
  if s % 2 : return SYM_Y
  else     : return NOSYM


##################
## slice_to_bit ##
##################
def slice_to_bit ( s, N, m ) :
  if   m : m = N - 1 - s
  else   : m = s

  return m


####################
## dpgen_port_bit ##
####################
def dpgen_port_bit ( n, flags, s ) :
  return slice_to_bit ( s
                      , n
                      , flags & F_MSB_FIRST
                      )



#################
## dpgen_place ##
#################
def dpgen_place ( Ins, x, y, plac = FIXED ) :
  sym = NOSYM
  if ( y // 50 ) % 2 : sym = SYM_Y

  Place ( Ins, sym, XY ( x, y ), plac )


###################
## VHDL_BUS_WIDE ##
###################
def VHDL_BUS_WIDE ( msb, lsb, n, flags ) :

  if   flags & F_MSB_FIRST : temp = "to"
  else                     : temp = "downto"
  
  bus = "(%ld %s %ld)" % ( dpgen_port_bit ( n, flags, msb )
                         , temp
                         , dpgen_port_bit ( n, flags, lsb )
                         )

  return bus

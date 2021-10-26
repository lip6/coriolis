
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
# |  Py Module   :       "./util_Misc.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



###########
## ilog2 ##
###########
def ilog2 ( x ) :
  i = 0
  while x :
    i = i + 1
    x = x >> 1
  return i  

###########
## iexp2 ##
###########
def iexp2 ( x ) :
  return ( 1 << x )

###############
## get_depth ##
###############
def get_depth ( x, depth ) :
  mDepth = ilog2 ( x )

  for i in range ( depth , mDepth + 1, 1 ) :
    if x & ( 1 << i ) : return i

  return 0

##########
## log2 ##
##########
def log2 ( x ) :
  import math
  
  if x <= 1 : return 0
  
#  l = int(math.ceil(math.log(x-0.4999)/math.log(2)))
#  if l >= 48 : raise "ERROR in log2."
#  return l

  try:
    l = int(math.ceil(math.log(x-0.4999)/math.log(2)))
    if l>=48: raise OverflowError
    # x is small enough : float rounding is accurate enough
    return l
  except OverflowError:
    # x is too large for direct float handling
    # with this method, no limit is known
    return log2((x+0xffffffff)//0x100000000)+32

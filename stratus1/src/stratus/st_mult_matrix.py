
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
# |  Py Module   :       "./mult_matrix.py"                         |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

###########################################################################################################################
def Matrix ( matrice, position, sig_comp, size_A, size_B ) :
  size_row = size_B + 1
  
  for k in range ( size_B ) :
    size_line = (2*(size_B - 1)) - 2*k
    
    if k == 0 :
      for kk in range ( size_line ) :
        matrice[k]  = matrice[k]  + [matrice[k][size_A-1]] 
        position[k] = position[k] + [1]
        
    elif k == size_B-1 :
      position[k] = [0] + position[k]
      position[k] = [1] + position[k]
      matrice[k]  = [0] + matrice[k][:]
      matrice[k]  = [sig_comp[k-1]] + matrice[k][:]
      
    else :
      position[k] = [0] + position[k]
      matrice[k]  = [0] + matrice[k]
      matrice[k]  = [sig_comp[k-1]]+ matrice[k][:]
      position[k] = [1] + position[k]
      
      for kk in range ( size_line ) :
        matrice[k]= matrice[k] + [matrice[k][len(matrice[k])-1]]
        position[k] = position[k] + [1]
            
  position.append([])
  matrice.append([])
    
  for ii in range ( 2, size_row ) :
    for jj in range ( ii - 1 ) :
      position[ii] = [0] + [0]+ position[ii]
      matrice[ii]  = [0] + [0]+ matrice[ii][:]
      
  position[size_B] = position[size_B] + [1]
  matrice[size_B]  = matrice[size_B][:] + [sig_comp[k]]
  
  for jj in range ( size_A-1 ) :
    position[ii] = position[ii] + [0]
    matrice[ii]  = matrice[ii][:] + [0]
    
  return [matrice,position]

def Matrix_Inv ( matrice, size_row ) :
  matrice_inv = []
  
  for i in range ( size_row ) :
    matrice_inv.append ([])
    for j in range ( len ( matrice[0] ) ) :
      matrice_inv[i] = matrice_inv[i] + [matrice[i][len(matrice[i])-j-1]]
                                    
  return(matrice_inv)

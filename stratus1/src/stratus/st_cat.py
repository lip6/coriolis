
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
# |  Py Module   :       "./st_cat.py"                              |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from Hurricane import *

from st_net    import *

import types

def Cat ( *nets ) :
  global CELLS
  from st_model import CELLS
  
  cell = CELLS[-1]

  nb = len ( cell._TAB_NETS_CAT )

  cell._TAB_NETS_CAT += [Signal ( "net_cat_%d" % nb )]

  netToCat = cell._TAB_NETS_CAT[nb]

  #if isinstance( nets[0], list ): nets = nets[0]
    
  # Creation of the inversed tab
  netstries = []
  for net in nets :
     if isinstance(net,list):
        for netin in net : netstries.insert(0, netin)
     else:
        netstries.insert ( 0, net )

    
  # Construction of the tab _to_cat if needed
  if not ( len ( netToCat._to_cat ) ) :
    for net in netstries :                
      for l in range ( net._arity ) : netToCat._to_cat.append ( 0 )

  # Initialisation of _to_cat
  for net in netstries :
    if ( "_arity" not in net.__dict__ ) or ( "_real_net" not in net.__dict__ ) or ( "_ind" not in net.__dict__ ) :
      err = "\n[Stratus ERROR] Cat : Problem of parameter.\n"
      raise Exception ( err )
    if net._real_net : realNet = net._real_net
    else             : realNet = net
    
    nbit1 = net._ind
    if ( '_reverse' in net.__dict__ ) and ( net._reverse ) :
      nbit2 = net._ind - net._arity
      pas   = -1
    else :
      nbit2 = net._ind + net._arity
      pas   =  1

    for l in range ( nbit1, nbit2, pas ) :
      netToCat._to_cat[netToCat._arity] = [realNet, l]
      netToCat._arity += 1

      # Initialisation of _hur_net if in hurricane mode
      if netToCat._st_cell._hur_cell : netToCat._hur_net += [realNet._hur_net[l]]

  return netToCat

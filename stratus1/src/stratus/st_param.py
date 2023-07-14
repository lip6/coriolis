
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
# |  Py Module   :       "./st_param.py"                            |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


import getopt, sys, re, types

#######################
def Param ( *tokens ) :
    
  try :
    argum = ""
    for i in range ( len ( tokens ) ) : argum += "%s:"
    opts = getopt.getopt ( sys.argv[1:], argum % tokens )[0]

  except getopt.GetoptError : 
    raise Exception ( "\n[Stratus ERROR] Param.\n" )

  if opts == [] :
    raise Exception ( "\n[Stratus ERROR] Param : there is no parameter.\n" )

  n = []
  for option, argument in opts :
    for i in range ( len ( tokens ) ) :
      if option in ( "-%s" % tokens[i] ) :
        if   re.search (                    "^[0-9]+$", argument ) : n.insert ( i,   int ( argument ) )
        elif re.search ( "^[0-9]+\.?[0-9]*e?-?[0-9]+$", argument ) : n.insert ( i, float ( argument ) )
        else                                                       : n.insert ( i,         argument   ) # By default, type is str
  
  if len ( n ) == 1 : return n[0]
  
  return n

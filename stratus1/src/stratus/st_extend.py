
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
# |  Py Module   :       "./st_extend.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


from stratus import *

class Extend ( Model ) :
    
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    self.nbit0 = param['nbit0']
    self.nbit1 = param['nbit1']

    self.type  = param['type']

  def Interface ( self ) :
    self.i = SignalIn  ( "i", self.nbit0 )
    self.o = SignalOut ( "o", self.nbit1 )

    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self ) :
    if self.nbit0 > 1 : self.o[self.nbit0-1:0] <= self.i.Buffer()
    else              : self.o[0]              <= self.i.Buffer()

    if   self.type == 'zero' :
      if (self.nbit1-1) == self.nbit0 : self.o[self.nbit0]              <= Zero ( 1 )
      else                            : self.o[self.nbit1-1:self.nbit0] <= Zero ( self.nbit1 - self.nbit0 )
    elif self.type == 'one' :
      if (self.nbit1-1) == self.nbit0 : self.o[self.nbit0]              <= One  ( 1 )
      else                            : self.o[self.nbit1-1:self.nbit0] <= One  ( self.nbit1 - self.nbit0 )
    elif self.type == 'signed' :
      for i in range ( self.nbit0, self.nbit1 ) : self.o[i] <= self.i[self.nbit0-1].Buffer()
    else :
      err = "\n[Stratus ERROR] Extend : type must be \'zero\', \'one\' or \'signed\'.\n"
      raise Exception ( err )

  def GetParam ( cls ):

     return {'nbit0' : 'integer', 'nbit1' : 'integer', 'type' : ["zero", "one", "signed"]}

  GetParam = classmethod ( GetParam )



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
# |  Author      :                        Sophie BELLOEIL           |
# |  E-mail      :           Sophie.Belloeil@asim.lip6.fr           |
# | =============================================================== |
# |  Py Module   :       "./st_mult_blocs.py"                       |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class booth_decoder ( Model ) :
  def Interface ( self ) :
    self.Bminus = SignalIn  ( "bminus", 1 )
    self.B      = SignalIn  (      "b", 1 )
    self.Bplus  = SignalIn  (  "bplus", 1 )
    self.NUL    = SignalOut (    "nul", 1 )
    self.DECA   = SignalOut (   "deca", 1 )
    self.COMP   = SignalOut (   "comp", 1 )
    
    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self ) :
    inv_B      = Signal (      "inv_b" )
    inv_Bplus  = Signal (  "inv_bplus" )
    inv_Bminus = Signal ( "inv_bminus" )

    inv_B      <= ~self.B
    inv_Bplus  <= ~self.Bplus
    inv_Bminus <= ~self.Bminus
      
    self.NUL  <= ( self.Bplus & self.B & self.Bminus ) | ( inv_B & inv_Bplus & inv_Bminus )
    self.COMP <= self.Bplus & ( inv_B | inv_Bminus  )
    self.DECA <= ( inv_B & inv_Bminus & self.Bplus ) | ( inv_Bplus & self.B & self.Bminus )

  def GetModelName ( cls, param ) :
    return "booth_decoder"
  GetModelName = classmethod ( GetModelName )

class booth_mux ( Model ) :
  def Interface ( self ) :
  	self.A    = SignalIn  (    "a", 1 )
  	self.B    = SignalIn  (    "b", 1 )
  	self.NUL  = SignalIn  (  "nul", 1 )
  	self.COMP = SignalIn  ( "comp", 1 )
  	self.DECA = SignalIn  ( "deca", 1 )
  	self.PP   = SignalOut (   "pp", 1 )
    
  	self.vdd  = VddIn ( "vdd" )
  	self.vss  = VssIn ( "vss" )
  
  def Netlist ( self ) :
    self.PP <= ( ( ( self.A & self.DECA ) | ( ~self.DECA & self.B ) ) ^ ( self.COMP ) ) & ( ~self.NUL )

  def GetModelName ( cls, param ) :
    return "booth_mux"
  GetModelName = classmethod ( GetModelName )

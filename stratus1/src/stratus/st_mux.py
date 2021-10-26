
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
# |  Py Module   :       "./st_mux.py"                              |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class Smux ( Model ) :

  def Interface ( self ) :

    nbit_cmd = self._param['nbit_cmd']
    nbit     = self._param['nbit']

    if nbit_cmd < 1 : raise "\n[Stratus ERROR] mux : the number of bits of the command must be greater than 0.\n"
    if nbit     < 1 : raise "\n[Stratus ERROR] mux : the number of bits of the inputs must be greater than 0.\n"
    
    self._cmd = SignalIn  ( "cmd", nbit_cmd )
   
    self._in_tab = []
    for i in range ( int(pow ( 2, nbit_cmd )) ) : self._in_tab += [SignalIn ( "i%d" % i, nbit )]
    
    self._q = SignalOut ( "q", nbit )
    
    self._vdd = VddIn  ( "vdd" )
    self._vss = VssIn  ( "vss" )
    
  def Netlist ( self ) :

    nbit_cmd = self._param['nbit_cmd']
    nbit     = self._param['nbit']

    # If the command is a bus
    #########################
    if nbit_cmd > 1 :
      temp = {}
      # Input nets are renamed
      for i in range ( int(pow ( 2, nbit_cmd )) ) : temp[i] = self._in_tab[i]
      # Temporary nets
      for i in range ( int(pow ( 2, nbit_cmd )), int(pow ( 2, nbit_cmd + 1 ) - 1) ) : temp[i] = Signal ( "temp_%d" % i, nbit )

      bit_cmd    = 0
      bit_entree = 0
      bit_sortie = pow ( 2, nbit_cmd )

      for i in range ( nbit_cmd - 1, -1, -1 ) :
        for j in range ( int(pow ( 2, i )) ) :
          Generate ( "Mx2", "mux_%d" % nbit, param = { 'nbit' : nbit } )
          Inst ( "mux_%d" % nbit
               , map = { 'i0'  : temp[2*j+bit_entree]
                       , 'i1'  : temp[2*j+1+bit_entree]
                       , 'cmd' : self._cmd[bit_cmd]
                       , 'q'   : temp[j+bit_sortie]
                       , 'vdd' : self._vdd
                       , 'vss' : self._vss
                       }
               )
        
        bit_cmd    += 1
        bit_entree += pow ( 2, i ) * 2
        bit_sortie += pow ( 2, i )

      self._q <= temp[pow ( 2, nbit_cmd + 1 ) - 2]

    # If the command is a 1 bit net
    ###############################
    else :
      Generate ( "Mx2", "mux_%d" % nbit, param = { 'nbit' : nbit } )
      Inst ( "mux_%d" % nbit
           , map = { 'i0'  : self._in_tab[0]
                   , 'i1'  : self._in_tab[1]
                   , 'cmd' : self._cmd
                   , 'q'   : self._q
                   , 'vdd' : self._vdd
                   , 'vss' : self._vss
                   }
           )

  def GetModelName ( cls, param ) :
  
    modelName = "smux"
    
    modelName += "_"
    modelName += str(param['nbit'])
    
    modelName += "_"
    modelName += str(param['nbit_cmd'])

    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetParam ( cls ):

    return {'nbit' : 'integer', 'nbit_cmd' : 'integer'}

  GetParam = classmethod ( GetParam )


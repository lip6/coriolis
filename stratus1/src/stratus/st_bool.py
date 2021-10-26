
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
# |  Py Module   :       "./st_bool.py"                             |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

from st_parser import I, I0, I2, I3, A, CK, CMD, CMD0, CIN, Q, NQ, S

class Bool ( Model ) :
    
  def __init__ ( self, name, param, inout ) :
    Model.__init__ ( self, name, param )
      
    self.m     = self._param['model']
    self.realM = self._param['realModel']

    self.dictInOut = inout
    
    if 'nbit' not in param : self.nbit = 1
    else                   : self.nbit = self._param['nbit']
    
    if self.nbit < 1 :
      err = "\n[Stratus ERROR] " + self.m + " : the number of bits must be greater than 0.\n"
      raise Exception ( err )

  def Interface ( self ) :    
    if self.m in I :
      self._i  = SignalIn ( "i", self.nbit )
    elif self.m in I0 :
      self._i0 = SignalIn ("i0", self.nbit )
      self._i1 = SignalIn ("i1", self.nbit )
    elif self.m in A :
      self._a  = SignalIn ( "a", self.nbit )
      self._b  = SignalIn ( "b", self.nbit )

    if self.m in I2 :
      self._i2 = SignalIn ("i2", self.nbit )
    if self.m in I3 :
      self._i3 = SignalIn ("i3", self.nbit )
      
    if self.m in CMD :
      self._cmd = SignalIn ( "cmd", 1 )
    elif self.m in CMD0 :
      self._cmd0 = SignalIn ( "cmd0", 1 )
      self._cmd1 = SignalIn ( "cmd1", 1 )
      
    if self.m in CK :
      self._ck  = CkIn ( "ck" )
      
    if self.m in CIN :
      self._cin = SignalIn ( "cin", self.nbit )
    
    if self.m in Q :
      self._q  = SignalOut   (    "q", self.nbit )
    elif self.m in NQ :
      self._nq = SignalOut   (   "nq", self.nbit )
    elif self.m in S :
      self._sout = SignalOut ( "sout", self.nbit )
      self._cout = SignalOut ( "cout", self.nbit )
    
    self._vdd = VddIn  ( "vdd" )
    self._vss = VssIn  ( "vss" )

  def Netlist ( self ) :
    for i in range ( self.nbit ) :
      boolMap = { self.dictInOut['vdd'] : self._vdd
                , self.dictInOut['vss'] : self._vss
                }

      if self.m in I :
        boolMap[self.dictInOut['i']]    = self._i[i]  
      elif self.m in I0 :
        boolMap[self.dictInOut['i0']]   = self._i0[i]
        boolMap[self.dictInOut['i1']]   = self._i1[i]
      elif self.m in A :
        # sxlib fulladder:
        import types
        if isinstance( self.dictInOut['a'], list ):
          for realpin in self.dictInOut['a'] : boolMap[realpin] = self._a[i]
        else :
          boolMap[self.dictInOut['a']]    = self._a[i]
        if isinstance( self.dictInOut['b'], list ):
          for realpin in self.dictInOut['b'] : boolMap[realpin] = self._b[i]
        else :
          boolMap[self.dictInOut['b']]    = self._b[i]

      if self.m in I2 :
        boolMap[self.dictInOut['i2']]   = self._i2[i]
      if self.m in I3 :
        boolMap[self.dictInOut['i3']]   = self._i3[i]

      if self.m in CIN :
        if isinstance( self.dictInOut['cin'], list ):
          for realpin in self.dictInOut['cin'] : boolMap[realpin] = self._cin[i]
        else :
          boolMap[self.dictInOut['cin']]    = self._cin[i]
      if self.m in CMD :
        boolMap[self.dictInOut['cmd']]  = self._cmd
      elif self.m in CMD0 :
        boolMap[self.dictInOut['cmd0']]  = self._cmd0
        boolMap[self.dictInOut['cmd1']]  = self._cmd1
      if self.m in CK :
        boolMap[self.dictInOut['ck']]   = self._ck

      if self.m in Q :
        boolMap[self.dictInOut['q']]    = self._q[i]
      elif self.m in NQ :
        boolMap[self.dictInOut['nq']]   = self._nq[i]
      elif self.m in S :
        boolMap[self.dictInOut['sout']] = self._sout[i]
        boolMap[self.dictInOut['cout']] = self._cout[i]

      Inst ( "%s" % self.realM
           , map = boolMap
           )       

  def GetParam ( cls ):
    global BV
    from st_parser import BV
    if BV == [] : InitBV()

    return {'nbit' : 'integer', 'model' : BV}

  GetParam = classmethod ( GetParam )

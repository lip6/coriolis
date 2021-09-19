
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
# |  Py Module   :       "./util_Const.py"                          |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



import sys, types

XLONG_SIZE =  8


##########
## btol ##
##########
def btol ( aDigit ) :
  global LV_const

  digit = 0

  if   aDigit == '0' : digit = 0
  elif aDigit == '1' : digit = 1
  else :
    err = "\n[Stratus ERROR] : Invalid digit \'%c\' in boolean string \"%s\"\n" % ( aDigit, LV_const )
    raise Exception ( err )

  return digit


##########
## otol ##
##########
def otol ( aDigit ) :
  global LV_const

  digit = -1

  if aDigit >= '0' and aDigit <= '7' : digit = int ( aDigit )

  if digit < 0 :
    err = "\n[Stratus ERROR] : Invalid digit \'%c\' in octal string \"%s\".\n" % ( aDigit, LV_const )
    raise Exception ( err )

  return digit


##########
## xtol ##
##########
def xtol ( aDigit ) :
  global LV_const

  digit = -1

  if   aDigit >= '0' and aDigit <= '9' : digit = int ( aDigit )
  elif aDigit == 'a'  or aDigit == 'A' : digit = 10
  elif aDigit == 'b'  or aDigit == 'B' : digit = 11
  elif aDigit == 'c'  or aDigit == 'C' : digit = 12
  elif aDigit == 'd'  or aDigit == 'D' : digit = 13
  elif aDigit == 'e'  or aDigit == 'E' : digit = 14
  elif aDigit == 'f'  or aDigit == 'F' : digit = 15

  if digit < 0 :
    err = "\n[Stratus ERROR] : Invalid digit \'%c\' in hexadecimal string \"%s\".\n" % ( aDigit, LV_const )
    raise Exception ( err )

  return digit


##########
## ltox ##
##########
def ltox ( aDigit ) :
  digit = '\0'

  if   aDigit >=  0 and aDigit <=  9 : digit = str ( aDigit )
  elif aDigit == 10                  : digit = 'a'
  elif aDigit == 11                  : digit = 'b'
  elif aDigit == 12                  : digit = 'c'
  elif aDigit == 13                  : digit = 'd'
  elif aDigit == 14                  : digit = 'e'
  elif aDigit == 15                  : digit = 'f'

  if digit == '\0' :
    err = "\n[Stratus ERROR] : Invalid number %d in hexadecimal conversion.\n" % aDigit
    raise Exception ( err )

  return digit


###########
## newxl ##
###########
class newxl :

  def __init__ ( self ) :
    self._size = 0
    self._l    = []

    for i in range ( XLONG_SIZE ) : self._l.append ( 0 )


  ###########
  ## atoxl ##
  ###########
  def atoxl ( self, asConst ) :
    global LV_const
  
    LV_const = asConst

    if not isinstance(asConst,str): raise Exception ( "\n[Stratus ERROR] : the constant must be described in a string.\n" )
  
    base   = 1
    offset = 2
  
    if asConst[0] == '0' :
      if   asConst[1] == 'b' or asConst[1] == 'B' : base = 1
      elif asConst[1] == 'x' or asConst[1] == 'X' : base = 4
      elif asConst[1] == 'o' or asConst[1] == 'O' : base = 3
      else : 
        err = "\n[Stratus ERROR] : one has to specify the basis of the constant : 0b or 0x or 0o.\n"
        raise Exception ( err )
    else :
      err = "\n[Stratus ERROR] : one has to specify the basis of the constant : 0b or 0x or 0o.\n"
      raise Exception ( err )
  
    length = len ( asConst )
  
    self._size = (length - offset) * base
  
    xl_size = self._size // 32
    if self._size % 32 : xl_size += 1
  
    for i in range ( xl_size ) :
      iRight = length - 1 - ( 32 // base ) *   i
      iLeft  = length     - ( 32 // base ) * ( i + 1 )
  
      if iLeft < offset : iLeft = offset
  
      power = 1
      
      while iRight >= iLeft :
        if base == 1 :
#          self._l[i] += btol ( asConst[iRight] ) << ( power - 1 )
          temp1 = power - 1
          temp2 = btol ( asConst[iRight] )
          while temp1 :
            temp2 = temp2 * 2
            temp1 = temp1 - 1

          self._l[i] += temp2
    
        elif base == 3 :
#          self._l[i] += otol ( asConst[iRight] ) << ( power - 1 )
          temp1 = power - 1
          temp2 = otol ( asConst[iRight] )
          while temp1 :
            temp2 = temp2 * 2
            temp1 = temp1 - 1

          self._l[i] += temp2
    
        elif base == 4 :
#          self._l[i] += xtol ( asConst[iRight] ) << ( power - 1 )
          temp1 = power - 1
          temp2 = xtol ( asConst[iRight] )
          while temp1 :
            temp2 = temp2 * 2
            temp1 = temp1 - 1

          self._l[i] += temp2
  
        power  += base

        iRight -= 1


  ##############
  ## getxlbit ##
  ##############
  def getxlbit ( self, aBit ) :
    i    = aBit // 32
    
#    mask = ( 1 << ( aBit % 32 ) )
    mask = 1
    temp = aBit % 32
    while temp :
      mask = mask * 2
      temp = temp - 1
 

    if i >= XLONG_SIZE :
      err = "\n[Stratus ERROR] : Requested bit", aBit, "is out of range\n"
      raise Exception ( err )
  
    if   self._l[i] & mask : return 1
    else                   : return 0

    
  ###############
  ## getxlhexa ##
  ###############
  def getxlhexa ( self, aBit ) :
    i     = aBit // 32
    shift = aBit % 32

    if i >= XLONG_SIZE :
      err = "\n[Stratus ERROR] : Requested hexa %ld is out of range.\n" % aBit
      raise Exception ( err )

    return ( ( self._l[i] ) >> shift ) & 15

  
  ##############
  ## xltovhdl ##
  ##############
  def xltovhdl ( self ) :
    pS = ''
    
    i  = self._size

    if i % 4 :
      pS =  'B\"'

      while i % 4 :
        if self.getxlbit ( i - 1 ) : pS += '1'
        else                       : pS += '0'

        i -= 1
            
    if i >= 4 :
      if pS != '' : pS += '\" & '

      pS += 'X\"'
      
      i -= 4
      for j in range ( i, -1, -4 ) :
        pS += ltox ( self.getxlhexa ( j ) )

    pS += '\"'

    return pS

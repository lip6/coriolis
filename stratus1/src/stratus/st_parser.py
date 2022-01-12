
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
# |  Py Module   :       "./st_parser.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x

import xml.parsers.expat
import re, types, os.path

BV   = []
BVg  = []

I    = []
I0   = []
I2   = []
I3   = []
A    = []

CK   = []
CMD  = []
CMD0 = []
CIN  = []

Q    = []
NQ   = []
S    = []

##############
class Parser :

  def __init__ ( self ) :
    self._p = xml.parsers.expat.ParserCreate ()
    
    self._p.StartElementHandler  = self.start_element
    self._p.EndElementHandler    = self.end_element
    self._p.CharacterDataHandler = self.char_data

    self._realCell = {}
    self._inOut    = {}
    
  # Handler functions

  #########################################
  def start_element ( self, name, attrs ) :
    # Print which the technology is
#    if   name == 'technology' : print( "  - Stratus virtual technology targets:", attrs['name'] )

    # Modification of attributes
    if name == 'model' :
      virtName = str(attrs['name'])
    
      self._realCell[virtName] = str(attrs['realcell'])
      
      inOutTemp = {}
      for key in attrs :
        if key not in ( 'name', 'realcell' ) :
          virtualPort = str(key)
          realPort    = str(attrs[virtualPort])
          if ',' in realPort :
            import re
            tabPort = re.split ( '[,]', realPort )
            inOutTemp[virtualPort] = tabPort
          else :
            inOutTemp[virtualPort] = realPort

      self._inOut[str(attrs['name'])] = inOutTemp
      
  ################################
  def end_element ( self, name ) : pass
     
  ############################## 
  def char_data ( self, data ) : pass  # print( repr(data) )
 
  # Parsing a file
  ################
  def Parse ( self, nameFile ) :
    self._p.ParseFile ( open ( nameFile, "rb" ) )

##################
class InitParser :

  def __init__ ( self ) :
    self._p = xml.parsers.expat.ParserCreate ()
    
    self._p.StartElementHandler  = self.start_element
    self._p.EndElementHandler    = self.end_element
    self._p.CharacterDataHandler = self.char_data

    self._realCell = {}
    self._inOut    = {}

  # Handler functions

  #########################################
  def start_element ( self, name, attrs ) :
      global BV, I, I0, I2, I3, A, CK, CMD, CMD0, CIN, Q, NQ, S
      
      # Modification of attributes
      if name == 'model' :
        virtName = str(attrs['name'])
        BV.append ( virtName )
      
        for key in attrs :
          if   key == 'i'    : I.append    ( virtName )
          elif key == 'i0'   : I0.append   ( virtName )
          elif key == 'i2'   : I2.append   ( virtName )
          elif key == 'i3'   : I3.append   ( virtName )
          elif key == 'a'    : A.append    ( virtName )
          elif key == 'ck'   : CK.append   ( virtName )
          elif key == 'cmd'  : CMD.append  ( virtName )
          elif key == 'cmd0' : CMD0.append ( virtName )
          elif key == 'cin'  : CIN.append  ( virtName )
          elif key == 'q'    : Q.append    ( virtName )
          elif key == 'nq'   : NQ.append   ( virtName )
          elif key == 'sout' : S.append    ( virtName )

  ################################
  def end_element ( self, name ) : pass
     
  ############################## 
  def char_data ( self, data ) : pass  # print( repr(data) )
 
  # Parsing a file
  ################
  def Parse ( self, nameFile ) :
    if not os.path.isfile(nameFile):
      print( '[ERROR] Parser.Parse(): stratus1.mappingName (\"%s\") file not found.' % nameFile )
    self._p.ParseFile ( open ( nameFile, "rb" ) )

    # Given the tab of the name of the cells, contruction of a tab giving the name of the generators (first letter uppered)
    for name in BV :
      chaine = re.search ( "([a-z])(.+)", name )
      name_g = chaine.group(1).upper() + chaine.group(2)
      BVg.append ( name_g )

##############
class WeightParser :

  def __init__ ( self ) :
    self._p = xml.parsers.expat.ParserCreate ()
    
    self._p.StartElementHandler  = self.start_element
    self._p.EndElementHandler    = self.end_element
    self._p.CharacterDataHandler = self.char_data

    self._weightTime  = {}
    self._weightArea  = {}
    self._weightPower = {}
    
  # Handler functions

  #########################################
  def start_element ( self, name, attrs ) :
      if name == 'model' :
        virtName = str(attrs['name'])
        
        if 'time' in attrs :
          temp = float(attrs['time'])
        else :
#          print( 'Warning: no weight done in the file, weight put to 1.' )
          temp = 1
      
        #self._weightTime[virtName][0] = temp

        # multi output cells as halfadder and fulladder
        if 'time2' in attrs :
          self._weightTime[virtName] = {}
          self._weightTime[virtName]['time'] = temp
          temp = float(attrs['time2'])
          self._weightTime[virtName]['time2'] = temp
        else :
          self._weightTime[virtName] = temp
      

        if 'area' in attrs :
          temp = float(attrs['area'])
        else :
#          print( 'Warning: no weight done in the file, weight put to 1.' )
          temp = 1
      
        self._weightArea[virtName] = temp
        
        if 'power' in attrs :
          temp = float(attrs['power'])
        else :
#          print( 'Warning: no weight done in the file, weight put to 1.' )
          temp = 1
      
        self._weightPower[virtName] = temp
      
  ################################
  def end_element ( self, name ) : pass
     
  ############################## 
  def char_data ( self, data ) : pass  # print( repr(data) )
 
  # Parsing a file
  ################
  def Parse ( self, nameFile ) :
    self._p.ParseFile ( open ( nameFile, "r" ) )

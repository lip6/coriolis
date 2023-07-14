
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
# |  Py Module   :       "./st_func_gen_wallace.py"                 |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

from math    import *

import string

################################################################
def genWallace ( cell, _tabIn, matrixIn ) :
  FA               = 0
  HA               = 1
  tabIn            = transpose ( _tabIn )
  mylistTransposed = transpose ( matrixIn[:] ) 
  nbNumbers        = len ( _tabIn )
  
  tabtmp           = wallace ( mylistTransposed )
  tabFAHA          = tabtmp[0]
  
  count            = 0
  choix            = 0
  
  tabInput = []
  for i in range ( len ( tabIn ) ) :
    tabInput.append ( [] )
    
    for j in range ( len ( tabIn[i] ) ) :
      if tabIn[i][j] :
        choix = 1
        count = count + 1
        tabInput[i].append ( Signal ( "tabinput_%d_%d" % ( i, j ) , 1 ) )
        
        tabInput[i][len(tabInput[i])-1] = tabIn[i][j]
  
  if choix == 0 : tabInput = tabIn
  
  if nbNumbers > 2 and len ( tabFAHA ) > 0 :
    for i in range ( len ( tabInput ) ) :
      for k in range ( tabInput[i].count ( 0 ) ) : tabInput[i].pop ( tabInput[i].index ( 0 ) )
    
    for i in range ( len ( tabFAHA[0] ) - len( tabInput ) ) : tabInput = [[]] + tabInput
    
    colOut          = []
    reportedCarries = [] 
    
    for i in range ( len ( tabFAHA ) ) :
      for jinv in range ( len ( tabFAHA[i] ) ) :
        j  = len(tabFAHA[i]) - 1 - jinv 
        jj = j

        t             = tabFAHA[i][j]
        reportCarries = []
        col           = []
        col           = tabInput[jj] + reportedCarries
        colOut        = []
        
        for ii in range ( t[FA] ) :
          listFA = getInputs ( col, 3 )
          
          colOut.append        ( Signal ( "colout_%d_%d_%d" % ( i, jinv, ii ), 1 ) )
          reportCarries.append ( Signal ( "report_%d_%d_%d" % ( i, jinv, ii ), 1 ) )
         
          Generate ( 'Fulladder', "full_adder", param = {'nbit' : 1} )
          Inst ( 'full_adder'
               , map = { 'a'    : col[listFA[0]]
                       , 'b'    : col[listFA[1]]
                       , 'cin'  : col[listFA[2]]
                       , 'sout' : colOut[ii] #[0]
                       , 'cout' : reportCarries[ii][0]
                       , 'vdd'  : cell._st_vdds[0]
                       , 'vss'  : cell._st_vsss[0]
                       }
               )
          
          col.pop ( listFA[2] )
          col.pop ( listFA[1] )
          col.pop ( listFA[0] )
        
        sizeReportCarries = len ( reportCarries )
        sizeColOut        = len ( colOut )
        
        for ii in range ( t[HA] ) :
          listHA = getInputs ( col, 2 )
    
          colOut.append        ( Signal ( "colout_%d_%d_%d" % ( i, jinv, ii+t[FA]), 1 ) )
          reportCarries.append ( Signal ( "report_%d_%d_%d" % ( i, jinv, ii+t[FA]), 1 ) )
          
          Generate ( 'Halfadder', 'half_adder', param = {'nbit ': 1} )
          Inst ( 'half_adder'
               , map = { 'a'    : col[listHA[0]]
                       , 'b'    : col[listHA[1]]
                       , 'sout' : colOut[sizeColOut + ii] #[0]
                       , 'cout' : reportCarries[sizeReportCarries + ii][0]
                       , 'vdd'  : cell._st_vdds[0]
                       , 'vss'  : cell._st_vsss[0]
                       }
               )
          
          col.pop(listHA[1])
          col.pop(listHA[0])
        
        for ii in range ( len ( colOut ) ) : col = col + [colOut[ii][0]]
        
        for ii in range ( len ( reportCarries ) ) :
          tmp               = reportCarries[ii][0]
          reportCarries[ii] = tmp
        
        tabInput[jj] = col[:]
        
        reportedCarries = reportCarries[:]
  
  for i in range ( len ( tabInput ) ) :
    for k in range ( tabInput[i].count ( 0 ) ): tabInput[i].pop ( tabInput[i].index ( 0 ) )
  
  sizeTabInput = len ( tabInput )
    
  output0 = Signal ( "output0", sizeTabInput ) 
  output1 = Signal ( "output1", sizeTabInput )

  for j in range ( sizeTabInput ) :
    jj = sizeTabInput -1 -j
    
    if len ( tabInput[j] ) == 0 :
      output0[jj] <= Zero(1)
      output1[jj] <= Zero(1)
    
    elif len ( tabInput[j] ) == 1 :
      output0[jj] <= tabInput[j][0].Buffer()
      output1[jj] <= Zero(1)
        
    elif len ( tabInput[j] ) > 1 :
      output0[jj] <= tabInput[j][0].Buffer()
      output1[jj] <= tabInput[j][1].Buffer()
      
  return [sizeTabInput,output0,output1]

################################################################
def getInputs ( lst, nb ) :
  lResult  = []
  sizeList = len(lst)
  i        = 0
  
  while nb > 0 and i < sizeList:
    if lst[i] :
      lResult = lResult + [i]
      nb      = nb -1
    i = i+1
    
  return lResult  

################################################################
def suite_dadda ( iValue ) :
  Un   = 2
  Un_1 = 2
  
  while Un < iValue :
    aux  = (3 * Un_1) // 2
    Un_1 = Un
    Un   = aux
  
  return Un_1  

################################################################
def transpose ( lValue ) :
  ltmp = []
  
  nbCol  = len(lValue[0])
  nbLine = len(lValue)
  
  for j in range(nbCol):
    ltmp2 = []
    for i in range ( nbLine ) : ltmp2.append ( lValue[i][j] )
    
    ltmp.append ( 0 )
    ltmp[j] = ltmp2
  
  return ltmp

################################################################
def find_first_zero ( lValue ) :
  i_ = 0
  
  for i_ in range (len ( lValue ) ) :
    if lValue[i_] == 0 : return i_
  
  return -1
  
################################################################
def wallace_1level ( lMat, lAdders ) :

  lMatrice = lMat[:]  

  for j in range(len(lMatrice)):
    lAdders.append([0])
  
  hmax = max_list(lMatrice)
  umax = suite_dadda(hmax)
  
  carries = []
  
  for j in range(len(lMatrice)):
    i=len(lMatrice)-1-j 
    
    nb_FA = 0
    nb_HA = 0
    report_carries = []
    
    delta = size_list(lMatrice[i]) - umax
    nbToDel = delta + len(carries)
    
    if(nbToDel > 0):
      nb_FA = nbToDel//2
      nb_HA = nbToDel%2
    else:
      nb_FA = 0
      nb_HA = 0
    
    for k in range(nb_FA + nb_HA) : report_carries = report_carries + [1]
    
    lAdders[i] = [nb_FA, nb_HA]
    
    nb = size_list(lMatrice[i])
    
    for k in range(len(carries)):
      ii = find_first_zero(lMatrice[i])  
      
      if(ii == -1):
        (lMatrice[i]).append(1)
      else:
        lMatrice[i].remove(0)
        lMatrice[i].insert(ii,1)
    
    for k in range(nbToDel ):
      (lMatrice[i]).remove(1)
      (lMatrice[i]).append(0)
    
    carries = report_carries
  
  if(len(carries) > 0 ):
    for i in range( len(lMatrice[0]) - len(carries) ) : carries = carries + [0]
    
    lMatrice = [carries] + lMatrice
  
  return lMatrice

#------------------------------------------------------------------------
def wallace ( lValue ) :
  ltmp=lValue[:]
  
  nbNumbers = len(lValue[0])
  
  listAdders = []
  
  counter = 0
  
  while max_list(ltmp)>2:
    listAdders.append([0])
    listAdders[counter] = []
    
    ltmp = wallace_1level(ltmp,listAdders[counter])
    counter = counter +1
  
  if(nbNumbers>2):
    mmax = 0;
    for i in range(len(listAdders)):
      if len(listAdders[i]) > mmax : mmax = len(listAdders[i])
    
    for i in range(len(listAdders)) :
      while (mmax - len(listAdders[i])) > 0 : listAdders[i] = [[0,0]] + listAdders[i]
  
  if(nbNumbers>2 and len(listAdders) > 0) : return [listAdders,ltmp]
  else : return [[],lValue]

#------------------------------------------------------------------------
def size_list ( lValue ) :
  count = 0
  
  for i in range ( len ( lValue ) ) :
  	if ( lValue[i] == 1 ) : count = count + 1
                            
  return count

#------------------------------------------------------------------------
def max_list ( lValue ) :
  nbmax = 0
  
  for j in range ( len ( lValue ) ) :
  	nb = size_list ( lValue[j] )
    
  	if nb > nbmax : nbmax = nb
  
  return nbmax

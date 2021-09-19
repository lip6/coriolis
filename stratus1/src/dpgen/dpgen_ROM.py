
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
# |  Py Module   :       "./dpgen_ROM.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

# Le nombre de mots peut prendre cinq valeurs : 64, 128, 256, 512, 1024
# Le nombre de bits est un multiple de 4 et doit etre inferieur ou egal a 64
# On peut avoir une sortie tri-state en option (pour construire des ROMs
# de capacite superieure a 1024 mots.
# La largeur est egale a ( nmot / 64 * 50) + 150 (+ 20 si option tristate)
# La hauteur est egale a ( nbit + 8) * 50 pour 64 ou 128 mots 
#                        ( nbit + 9) * 50 pour 256, 512 ou 1024 mots 
#
# Les valeurs a encoder dans la ROM sont recuperees dans le tableau data[2048]
# qui contient des "mots" de 32 bits : une valeur est stockee dans deux cases
# consecutives de ce tableau, ce qui permet d'avoir des mots de 64 bits.
# Les poids faibles de la valeur i sont a l'adresse data[2*i]
# Les poids forts de la valeur i sont a l'adresse data[2*i+1]
#
# Les lignes de bit horizontales en ALU2 sont hors grilles et pre-routees
# la ROM respecte le gabarit SXLIB pour ce qui concerne les caissons et les
# alimentations en ALU1 sur les faces est et ouest, mais il n'y a pas 
# de continuite des alimentations en ALU1 dans la zone data.
# Pour la partie "donnees", on utilise les cellules suivantes :
# - rom_data_invss
# - rom_data_insel
# - rom_data_midvss
# - rom_data_midsel
# - rom_data_outvss
# - rom_data_outsel
# - rom_data_outvss_ts
# - rom_data_outsel_ts
#
# Pour le decodeur, on utilise les cellules suivantes :
# - rom_dec_adbuf
# - rom_dec_nop
# - rom_dec_prech
# - rom_dec_col2 
# - rom_dec_col3 
# - rom_dec_col4 
# - rom_dec_colbuf 
# - rom_dec_line01
# - rom_dec_line23
# - rom_dec_line45
# - rom_dec_line67
# - rom_dec_selmux01
# - rom_dec_selmux23
# - rom_dec_selmux45
# - rom_dec_selmux67
# - rom_dec_selmux67_128
# - rom_dec_selmux01_ts
# - rom_dec_selmux23_ts
# - rom_dec_selmux45_ts
# - rom_dec_selmux67_ts
# - rom_dec_selmux67_128_ts

class TopRom ( Model ) :

  def Interface ( self ) :
    self.nbit  = self._param['nbit']
    self.nword = self._param['nword']
    
    if   self.nword ==   64 : adrange =  6
    elif self.nword ==  128 : adrange =  7
    elif self.nword ==  256 : adrange =  8
    elif self.nword ==  512 : adrange =  9
    elif self.nword == 1024 : adrange = 10
    else :
      print( "Error : DPGEN_ROM" )
      print( "The word number (nword := %d) ", nword, "can only takes values : 64, 128, 256, 512 or 1024.\n" )
      sys.exit ( 3 )
    
    if self.nbit % 4 != 0 or self.nbit > 64 or self.nbit < 4 :
      raise( "\n[Stratus ERROR] DPGEN_ROM : The bus width (nbit := %d) ", nbit, "must be multiple of 4 and no larger than 64.\n" )
    
    self.ck     = SignalIn  (     "ck", 1 )
    self.selrom = SignalIn  ( "selrom", 1 )
    
    self.ad     = SignalIn  (     "ad",   adrange )
    self.data   = SignalOut (   "data", self.nbit )

    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self, type ) :
      
    ### Acquisition du tableau des valeurs a encoder ###
#  ICI IL FAUT APPELER LE PARSER
#    for(i=0 ; i<1024 ; i++) {
#    data[2*i] = i;
#    data[2*i+1] = 1024 - i;
#    }
#   dpgen_ROM_code ( LV_name, self.nbit, self.nword, 0, 0, data )

    # Signaux internes
    nck    = Signal (    "nck", 1 )
    
    prech  = Signal (  "prech", 1 )
    nprech = Signal ( "nprech", 1 )
    
    mux0   = Signal (   "mux0", 1 )
    mux1   = Signal (   "mux1", 1 )
    mux2   = Signal (   "mux2", 1 )
    mux3   = Signal (   "mux3", 1 )
    mux4   = Signal (   "mux4", 1 )
    mux5   = Signal (   "mux5", 1 )
    mux6   = Signal (   "mux6", 1 )
    mux7   = Signal (   "mux7", 1 )
    
    sel0   = Signal (   "sel0", 1 )
    sel1   = Signal (   "sel1", 1 )
    sel2   = Signal (   "sel2", 1 )
    sel3   = Signal (   "sel3", 1 )
    sel4   = Signal (   "sel4", 1 )
    sel5   = Signal (   "sel5", 1 )
    sel6   = Signal (   "sel6", 1 )
    sel7   = Signal (   "sel7", 1 )
    
    bit0  = Signal (  "bit0", self.nbit )  
    bit1  = Signal (  "bit1", self.nbit )  
    bit2  = Signal (  "bit2", self.nbit )  
    bit3  = Signal (  "bit3", self.nbit )  
    bit4  = Signal (  "bit4", self.nbit )  
    bit5  = Signal (  "bit5", self.nbit )  
    bit6  = Signal (  "bit6", self.nbit )  
    bit7  = Signal (  "bit7", self.nbit )  
    line0 = Signal ( "line0", self.nbit )  
    line1 = Signal ( "line1", self.nbit )  
    line2 = Signal ( "line2", self.nbit )  
    line3 = Signal ( "line3", self.nbit )  
    line4 = Signal ( "line4", self.nbit )  
    line5 = Signal ( "line5", self.nbit )  
    line6 = Signal ( "line6", self.nbit )
    line7 = Signal ( "line7", self.nbit )
    
    mux0  = Signal (  "mux0", 1 )
    mux1  = Signal (  "mux1", 1 )
    mux2  = Signal (  "mux2", 1 )
    mux3  = Signal (  "mux3", 1 )
    mux4  = Signal (  "mux4", 1 )
    mux5  = Signal (  "mux5", 1 )
    mux6  = Signal (  "mux6", 1 )
    mux7  = Signal (  "mux7", 1 )

    a0    = Signal (    "a0", 1 )
    na0   = Signal (   "na0", 1 )
    a1    = Signal (    "a1", 1 )
    na1   = Signal (   "na1", 1 )
    a2    = Signal (    "a2", 1 )
    na2   = Signal (   "na2", 1 )
    a3    = Signal (    "a3", 1 )
    na3   = Signal (   "na3", 1 )
    a4    = Signal (    "a4", 1 )
    na4   = Signal (   "na4", 1 )
    a5    = Signal (    "a5", 1 )
    na5   = Signal (   "na5", 1 )
    a6x   = Signal (   "a6x", 1 )
    na6x  = Signal (  "na6x", 1 )
    a7x   = Signal (   "a7x", 1 )
    na7x  = Signal (  "na7x", 1 )
    a8x   = Signal (   "a8x", 1 )
    na8x  = Signal (  "na8x", 1 )
    a9x   = Signal (   "a9x", 1 )
    na9x  = Signal (  "na9x", 1 )
    
    col   = Signal (   "col", self.nbit )

    In  = {}
    Bit = {}
    Out = {}
    for i in range ( self.nbit ) :
      valeur = i % 4
      if   valeur == 0 :	
        cellin  = "rom_data_invss"
        cellmid = "rom_data_midvss"
        if type == 0 : cellout = "rom_data_outvss"
        else         : cellout = "rom_data_outvss_ts"
      elif valeur == 1 :
        cellin  = "rom_data_insel"
        cellmid = "rom_data_midsel"
        if type == 0 : cellout = "rom_data_outsel"
        else         : cellout = "rom_data_outsel_ts"
      elif valeur == 2:	
        cellin  = "rom_data_insel"
        cellmid = "rom_data_midsel"
        if type == 0 : cellout = "rom_data_outsel"
        else         : cellout = "rom_data_outsel_ts"
      elif valeur == 3 :	
        cellin  = "rom_data_invss"
        cellmid = "rom_data_midvss"
        if type == 0 : cellout = "rom_data_outvss"
        else         : cellout = "rom_data_outvss_ts"
      
      In[i] = Inst ( cellin
                   , "in%d" % i
                   , map = { 'prech' : prech
                           , 'bit0'  : bit0[i]
                           , 'bit1'  : bit1[i]
                           , 'bit2'  : bit2[i]
                           , 'bit3'  : bit3[i]
                           , 'bit4'  : bit4[i]
                           , 'bit5'  : bit5[i]
                           , 'bit6'  : bit6[i]
                           , 'bit7'  : bit7[i]
                           , 'vdd'   : self.vdd
                           , 'vss'   : self.vss
                           }
                   )
      
      BitBis = {}
      Bit[i] = BitBis
      for j in range ( 0, self.nword, 64 ) :
        Bit[i][j] = Inst ( cellmid
                         , "bit%d_%d" % ( i, j )
                         , map = { 'sela' : line0[j]
                                 , 'selb' : line2[j]
                                 , 'selc' : line4[j]
                                 , 'seld' : line6[j]
                                 , 'bit0' : bit0 [i]
                                 , 'bit1' : bit1 [i]
                                 , 'bit2' : bit2 [i]
                                 , 'bit3' : bit3 [i]
                                 , 'bit4' : bit4 [i]
                                 , 'bit5' : bit5 [i]
                                 , 'bit6' : bit6 [i]
                                 , 'bit7' : bit7 [i]
                                 , 'vdd'  : self.vdd
                                 , 'vss'  : self.vss
                                 }
                         )
        Bit[i][j+32] = Inst ( cellmid
                            , "bit%d_%d" % ( i, j+32 )
                            , map = { 'sela' : line1[j]
                                    , 'selb' : line3[j]
                                    , 'selc' : line5[j]
                                    , 'seld' : line7[j]
                                    , 'bit0' : bit0 [i]
                                    , 'bit1' : bit1 [i]
                                    , 'bit2' : bit2 [i]
                                    , 'bit3' : bit3 [i]
                                    , 'bit4' : bit4 [i]
                                    , 'bit5' : bit5 [i]
                                    , 'bit6' : bit6 [i]
                                    , 'bit7' : bit7 [i]
                                    , 'vdd'  : self.vdd
                                    , 'vss'  : self.vss
                                    }
                            )
 
      thisMap = { 'nprech' : nprech
                , 'mux0'   : mux0
                , 'mux1'   : mux1
                , 'mux2'   : mux2
                , 'mux3'   : mux3
                , 'mux4'   : mux4
                , 'mux5'   : mux5
                , 'mux6'   : mux6
                , 'mux7'   : mux7
                , 'bit0'   : bit0[i]
                , 'bit1'   : bit1[i]
                , 'bit2'   : bit2[i]
                , 'bit3'   : bit3[i]
                , 'bit4'   : bit4[i]
                , 'bit5'   : bit5[i]
                , 'bit6'   : bit6[i]
                , 'bit7'   : bit7[i]
                , 'q'      : data[i]
                , 'vdd'    : self.vdd
                , 'vss'    : self.vss
                }
      if type == 1 :
        thisMap['enx']  = enx
        thisMap['nenx'] = nenx
      Out[i] = Inst ( cellout
                    , "out%d" % i
                    , map = thisMap
                    )
   
    ## Cellules decodeur ##
    # Lignes 0 et 1                          
    Prech = Inst ( "rom_dec_prech"
                 , "prech"
                 , map = { 'nck'    : nck
                         , 'prech'  : prech
                         , 'nprech' : nprech
                         , 'vdd'    : self.vdd
                         , 'vss'    : self.vss
                         }
                 )
   
    And01 = {} 
    for j in range ( 0, self.nword, 64 ) :
      thisMap = { 'nck0'  : nck
    	        , 'nck1'  : nck
    	        , 'sel0'  : sel0
    	        , 'sel1'  : sel1
    	        , 'line0' : line0[j]
    	        , 'line1' : line1[j]
                , 'vdd'   : self.vdd
                , 'vss'   : self.vss
                }
      if self.nword != 64 : thisMap['col'] = col[j]
      else                : thisMap['col'] = One ( 1 )
      And01[j] = Inst ( "rom_dec_line01"
                      , "and01_%d" % j
                      , map = thisMap
                      )
        
    if type == 0 : model = "rom_dec_selmux01"
    else         : model = "rom_dec_selmux01_rs"
    thisMap = { 'a0'     : a0
              , 'na0'    : na0
              , 'a1'     : a1
              , 'na1'    : na1
              , 'a2'     : a2
              , 'na2'    : na2
              , 'a3'     : a3
              , 'na3'    : na3
              , 'a4'     : a4
              , 'na4'    : na4
              , 'a5'     : a5
              , 'na5'    : na5
              , 'ck'     : self.ck
              , 'selrom' : self.selrom
              , 'nck'    : nck
              , 'mux0'   : mux0
              , 'sel0'   : sel0
              , 'mux1'   : mux1
              , 'sel1'   : sel1
              , 'vdd'    : self.vdd
              , 'vss'    : self.vss
              }
    if type == 1 :
      thisMap['enx']  = enx
      thisMap['nenx'] = nenx
    Selmux01 = Inst ( model
                    , "selmux01"
                    , map = thisMap
                    )

    # Lignes 2 et 3
    Ad4 = Inst ( "rom_dec_adbuf"
               , "ad4"
               , map = { 'ad'   : self.ad[4]
                       , 'adx'  : a4
                       , 'nadx' : na4
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
    Ad5 = Inst ( "rom_dec_adbuf"
               , "ad5"
               , map = { 'ad'   : self.ad[5]
                       , 'adx'  : a5
                       , 'nadx' : na5
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
   
    And23 = {} 
    for j in range ( 0, self.nword, 64 ) :
      thisMap = { 'nck2'  : nck
    	        , 'nck3'  : nck
    	        , 'sel2'  : sel2
    	        , 'sel3'  : sel3
    	        , 'line2' : line2[j]
    	        , 'line3' : line3[j]
                , 'vdd'   : self.vdd
                , 'vss'   : self.vss
                }
      if self.nword != 64 : thisMap['col'] = col[j]
      else                : thisMap['col'] = One ( 1 )
      And23[j] = Inst ( "rom_dec_line23"
                      , "and23_%d" % j
                      , map = thisMap
                      )
        
    if type == 0 : model = "rom_dec_selmux23"
    else         : model = "rom_dec_selmux23_rs"
    thisMap = { 'a0'     : a0
              , 'na0'    : na0
              , 'a1'     : a1
              , 'na1'    : na1
              , 'a2'     : a2
              , 'na2'    : na2
              , 'a3'     : a3
              , 'na3'    : na3
              , 'a4'     : a4
              , 'na4'    : na4
              , 'a5'     : a5
              , 'na5'    : na5
              , 'ck'     : self.ck
              , 'selrom' : self.selrom
              , 'nck'    : nck
              , 'mux2'   : mux2
              , 'sel2'   : sel2
              , 'mux3'   : mux3
              , 'sel3'   : sel3
              , 'vdd'    : self.vdd
              , 'vss'    : self.vss
              }
    if type == 1 :
      thisMap['enx']  = enx
      thisMap['nenx'] = nenx
    Selmux23 = Inst ( "rom_dec_selmux23"
                    , "selmux23"
                    , map = thisMap
                    )

    # Lignes 4 et 5
    Ad2 = Inst ( "rom_dec_adbuf"
               , "ad2"
               , map = { 'ad'   : self.ad[2]
                       , 'adx'  : a2
                       , 'nadx' : na2
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
    Ad3 = Inst ( "rom_dec_adbuf"
               , "ad3"
               , map = { 'ad'   : self.ad[3]
                       , 'adx'  : a3
                       , 'nadx' : na3
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
  
    And45 = {}
    for j in range ( 0, self.nword, 64 ) :
      thisMap = { 'nck4'  : nck
    	          , 'nck5'  : nck
    	          , 'sel4'  : sel4
    	          , 'sel5'  : sel5
    	          , 'line4' : line4[j]
    	          , 'line5' : line5[j]
                , 'vdd'   : self.vdd
                , 'vss'   : self.vss
                }
      if self.nword != 64 : thisMap['col'] = col[j]
      else                : thisMap['col'] = One ( 1 )
      And45[j] = Inst ( "rom_dec_line45"
                      , "and45_%d" % j
                      , map = thisMap
                      )
      
    if type == 0 : model = "rom_dec_selmux45"
    else         : model = "rom_dec_selmux45_rs"
    thisMap = { 'a0'     : a0
  	          , 'na0'    : na0
  	          , 'a1'     : a1
  	          , 'na1'    : na1
  	          , 'a2'     : a2
  	          , 'na2'    : na2
  	          , 'a3'     : a3
  	          , 'na3'    : na3
  	          , 'a4'     : a4
  	          , 'na4'    : na4
  	          , 'a5'     : a5
  	          , 'na5'    : na5
  	          , 'ck'     : self.ck
  	          , 'selrom' : self.selrom
  	          , 'nck'    : nck
  	          , 'mux4'   : mux4
  	          , 'sel5'   : sel4
  	          , 'mux5'   : mux5
  	          , 'sek5'   : sel5
              , 'vdd'    : self.vdd
              , 'vss'    : self.vss
              }
    if type == 1 :
      thisMap['enx']  = enx
      thisMap['nenx'] = nenx
    Selmux45 = Inst ( model
                    , "selmux23"
                    , map = thisMap
                    )    	
    
    # Lignes 6 et 7
    Ad0 = Inst ( "rom_dec_adbuf"
               , "ad0"
               , map = { 'ad'   : self.ad[0]
                       , 'adx'  : a0
                       , 'nadx' : na0
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
    Ad1 = Inst ( "rom_dec_adbuf"
               , "ad1"
               , map = { 'ad'   : self.ad[1]
                       , 'adx'  : a1
                       , 'nadx' : na1
                       , 'vdd'  : self.vdd
                       , 'vss'  : self.vss
                       }
               )
   
    And67 = {} 
    for j in range ( 0, self.nword, 64 ) :
      thisMap = { 'nck6'  : nck
    	          , 'nck7'  : nck
    	          , 'sel6'  : sel6
    	          , 'sel7'  : sel7
    	          , 'line6' : line6[j]
    	          , 'line7' : line7[j]
                , 'vdd'   : self.vdd
                , 'vss'   : self.vss
                }  
    	if self.nword != 64 : thisMap['col'] = col[j]
    	else                : thisMap['col'] = One ( 1 )
    	And67[j] = Inst ( "rom_dec_line67"
                        , "and67_%d" % j
                        , map = thisMap
                        )
  
    if self.nword != 128 : # pas de sortie tristate
      if type == 0 : model = "rom_dec_selmux67"
      else         : model = "rom_dec_selmux67_ts"
      thisMap = { 'a0'     : a0
    	        , 'na0'    : na0
    	        , 'a1'     : a1
    	        , 'na1'    : na1
    	        , 'a2'     : a2
    	        , 'na2'    : na2
    	        , 'a3'     : a3
    	        , 'na3'    : na3
    	        , 'a4'     : a4
    	        , 'na4'    : na4
    	        , 'a5'     : a5
    	        , 'na5'    : na5
    	        , 'ck'     : self.ck
    	        , 'selrom' : self.selrom
    	        , 'nck'    : nck
    	        , 'mux6'   : mux6
    	        , 'sel6'   : sel6
    	        , 'mux7'   : mux7
    	        , 'sel7'   : sel7
                , 'vdd'    : self.vdd
                , 'vss'    : self.vss
                }
      if type == 1 :
        thisMap['enx']  = enx
        thisMap['nenx'] = nenx
      Selmux67 = Inst ( model
                      , "selmux67"
    	              , map = thisMap
                      )
    else :   # pas de sortie tristate - 128 mots
      if type == 0 : model = "rom_dec_selmux128"
      else         : model = "rom_dec_selmux128_ts"
      thisMap = { 'a0'     : a0
    	        , 'na0'    : na0
    	        , 'a1'     : a1
    	        , 'na1'    : na1
    	        , 'a2'     : a2
    	        , 'na2'    : na2
    	        , 'a3'     : a3
    	        , 'na3'    : na3
    	        , 'a4'     : a4
    	        , 'na4'    : na4
    	        , 'a5'     : a5
    	        , 'na5'    : na5
    	        , 'a6'     : self.ad[6]
    	        , 'selrom' : self.selrom
    	        , 'a6x'    : col[64]
    	        , 'na6x'   : col[0]
    	        , 'mux6'   : mux6
    	        , 'sel6'   : sel6
    	        , 'mux7'   : mux7
    	        , 'sel7'   : sel7
                , 'vdd'    : self.vdd
                , 'vss'    : self.vss
                }
      if type == 1 :
        thisMap['enx']  = enx
        thisMap['nenx'] = nenx
      Selmux67 = Inst ( model
                        , "selmux67"
    	                , map = thisMap
                      )
  
    # Ligne 9 dans le cas 1024 mots
    if self.nword == 1024 :
      Nop = Inst ( "rom_dec_nop"
                 , "nop"
                 , map = { 'vdd' : self.vdd
                         , 'vss' : self.vss
                         }
                 )
     
      Col = {} 
      for j in range ( 0, self.nword, 64 ) :
        thisMap = { 'q'   : col[j]
                  , 'vdd' : self.vdd
                  , 'vss' : self.vss
                  }
        if ( j >> 6 ) % 2 == 0 : thisMap['i0'] = na6x
        else                   : thisMap['i0'] = a6x
        if ( j >> 7 ) % 2 == 0 : thisMap['i1'] = na7x
        else                   : thisMap['i1'] = a7x
        if ( j >> 8 ) % 2 == 0 : thisMap['i2'] = na8x
        else                   : thisMap['i2'] = a8x
        if ( j >> 9 ) % 2 == 0 : thisMap['i3'] = na9x
        else                   : thisMap['i3'] = a9x
        Col[j] = Inst ( "rom_dec_col4"
                      , "col_%d" % j
                      , map = thisMap
                      )
  
      ColBuf6 = Inst ( "rom_dec_colbuf"
                     , "colbuf6"
                     , map = { 'a'   : self.ad[6]
                             , 'ax'  : a6x
                             , 'nax' : na6x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                     )
      ColBuf7 = Inst ( "rom_dec_colbuf"
                     , "colbuf7"
                     , map = { 'a'   : self.ad[7]
                             , 'ax'  : a7x
                             , 'nax' : na7x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
      ColBuf8 = Inst ( "rom_dec_colbuf"
                     , "colbuf8"
                     , map = { 'a'   : self.ad[8]
                             , 'ax'  : a8x
                             , 'nax' : na8x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
      ColBuf9 = Inst ( "rom_dec_colbuf"
                     , "colbuf9"
                     , map = { 'a'   : self.ad[9]
                             , 'ax'  : a9x
                             , 'nax' : na9x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
    
    # Ligne 9 dans le cas 512 mots
    if self.nword == 512 :
      Nop = Inst ( "rom_dec_nop"
                 , "nop"
                 , map = { 'vdd' : self.vdd
                         , 'vss' : self.vss
                         }
                 )
      
      for j in range ( 0, self.nword, 64 ) :
        thisMap = { 'q'   : col[j]
                  , 'vdd' : self.vdd
                  , 'vss' : self.vss
                  }
        if ( j >> 6 ) % 2 == 0 : thisMap['i0'] = na6x
        else                   : thisMap['i0'] = a6x
        if ( j >> 7 ) % 2 == 0 : thisMap['i1'] = na7x
        else                   : thisMap['i1'] = a7x
        if ( j >> 8 ) % 2 == 0 : thisMap['i2'] = na8x
        else                   : thisMap['i2'] = a8x
        Col[j] = Inst ( "rom_dec_col3"
                      , "col_%d" % j
                      , map = thisMap
                      )
  
      Colbuf6 = Inst ( "rom_dec_colbuf"
                     , "colbuf6"
                     , map = { 'a'   : ad[6]
                             , 'ax'  : a6x
                             , 'nax' : na6x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
      Colbuf7 = Inst ( "rom_dec_colbuf"
                     , "colbuf7"
                     , map = { 'a'   : ad[7]
                             , 'ax'  : a7x
                             , 'nax' : na7x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
      Colbuf8 = Inst ( "rom_dec_colbuf"
                     , "colbuf8"
                     , map = { 'a'   : ad[8]
                             , 'ax'  : a8x
                             , 'nax' : na8x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
    
    # Ligne 9 dans le cas 256 mots
    if self.nword == 256 :
      Nop = Inst ( "rom_dec_nop"
                 , "nop"
                 , map = { 'vdd' : self.vdd
                         , 'vss' : self.vss
                         }
                 )
  
      Col = {}
      for j in range ( 0, self.nword, 64 ) :
        thisMap = { 'q'   : col[j]
                  , 'vdd' : self.vdd
                  , 'vss' : self.vss
                  }
        if ( j >> 6 ) % 2 == 0 : thisMap['i0'] = na6x
        else                   : thisMap['i0'] = a6x
        if ( j >> 7 ) % 2 == 0 : thisMap['i1'] = na7x
        else                   : thisMap['i1'] = a7x
        Col[j] = Inst ( "rom_dec_col2"
                      , "col_%d" % j
                      , map = thisMap
                      )
  
      Colbuf6 = Inst ( "rom_dec_colbuf"
                     , "colbuf6"
                     , map = { 'a'   : ad[6]
                             , 'ax'  : a6x
                             , 'nax' : na6x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )
      ColBuf7 = Inst ( "rom_dec_colbuf"
                     , "colbuf7"
                     , map = { 'a'   : ad[7]
                             , 'ax'  : a7x
                             , 'nax' : na7x
                             , 'vdd' : self.vdd
                             , 'vss' : self.vss
                             }
                     )

  def Layout ( self ) :
    ## Placement des lignes du plan memoire ##
    for i in range ( self.nbit ) :
      val = i % 4
      if   val == 0 : sym = NOSYM
	  elif val == 1 : sym = SYM_Y
	  elif val == 2 : sym = NOSYM
	  elif val == 3 : sym = SYM_Y
      
      Place ( In(i], sym, 0, i*HCELL )
              
    for j in range ( 0, self.nword, 64 ) :
      PlaceRight ( Bit[i][j],    sym )
	  PlaceRight ( Bit[i][j+32], sym )

    PlaceRight ( Out[i], sym )

    ## Placement des cellules decodeur ##
    
    # Lignes 0 et 1
    Place ( Prech, NOSYM, 0, nbit*HCELL )
    for j in range ( 0, self.nword, 64 ) : PlaceRight ( And01[j], NOSYM )
    PlaceRight ( Selmux01, NOSYM )

    # Lignes 2 et 3
    Place ( Ad4, SYM_Y, 0, (nbit+3)*HCELL )
    Place ( Ad5, NOSYM, 0, (nbit+2)*HCELL )
    for j in range ( 0, self.nword, 64 ) : PlaceRight ( And23[j], NOSYM )
    PlaceRight ( Selmux23, NOSYM )
    
    # Lignes 4 et 5
    Place ( Ad2, SYM_Y, 0, (nbit+5)*HCELL )
    Place ( Ad3, NOSYM, 0, (nbit+4)*HCELL )
    for j in range ( 0, self.nword, 64 ) : PlaceRight ( And45[j], NOSYM )
    Place ( Selmux45, NOSYM )
        
    # Lignes 6 et 7
    Place ( Ad0, SYM_Y, 0, (nbit+7)*HCELL )
    Place ( Ad1, NOSYM, 0, (nbit+6)*HCELL )
    for j in range ( 0, self.nword, 64 ) : PlaceRight ( And67[j], NOSYM )
	PlaceRight ( Selmux67, NOSYM )

    # Ligne 9 dans le cas 1024 mots
    if self.nword == 1024 :
      Place ( Nop, NOSYM, 0, (nbit+8)*HCELL )
      for j in range ( 0, self.nword, 64 ) : PlaceRight ( Col, NOSYM )
      PlaceRight ( ColBuf6, NOSYM )
      PlaceRight ( ColBuf7, NOSYM )
      PlaceRight ( ColBuf8, NOSYM )
      PlaceRight ( ColBuf9, NOSYM )
    # Ligne 9 dans le cas 512 mots
    elif self.nword == 512 :
      Place ( Nop, NOSYM, 0, (nbit+8)*HCELL )
      for j in range ( 0, self.nword, 64 ) : PlaceRight ( Col[j], NOSYM )
      PlaceRight ( ColBuf6, NOSYM )
      PlaceRight ( ColBuf7, NOSYM )
      PlaceRight ( ColBuf8, NOSYM )
    # Ligne 9 dans le cas 256 mots
    elif self.nword == 256 :
      Place ( Nop, NOSYM, 0, (nbit+8)*HCELL )
      for j in range ( 0, self.nword, 64 ) : PlaceRight ( Col[j], NOSYM )
      PlaceRight ( ColBuf6, NOSYM )
      PlaceRight ( ColBuf7, NOSYM )
      
  def ROM_VHDL ( self ) :
    global LV_flags
    global LV_ModelName

    pass
#    fileName = LV_ModelName + ".vbe"
#    
#    file = open ( fileName, "w+" )
#    
#    if   self.nword ==   64 : adrange =  6
#    elif self.nword ==  128 : adrange =  7
#    elif self.nword ==  256 : adrange =  8
#    elif self.nword ==  512 : adrange =  9
#    elif self.nword == 1024 : adrange = 10
#    
#    sBusWide0 = VHDL_BUS_WIDE ( self.nbit - 1, 0, self.nbit, LV_flags )
#    sBusWide1 = VHDL_BUS_WIDE ( adrange - 1, 0, self.nbit, LV_flags )
#
#    ## Entity description ##
#    file.write ( "\nENTITY %s IS\n  PORT (\n" % LV_ModelName )
#    
#    file.write ( "%14s : in    BIT;\n" % "ck"  )
#    file.write ( "%14s : in    BIT;\n" % "selrom" )
#    file.write ( "%14s : in    BIT_VECTOR %s;\n" % (   "ad", sBusWide1 ) )
#    file.write ( "%14s : out   BIT_VECTOR %s;\n" % ( "data", sBusWide0 ) )
#
#    ## Power supplies terminals ##  
#    file.write ( "%14s : in    BIT;\n" % "vdd" )
#    file.write ( "%14s : in    BIT\n"  % "vss" )
#    
#    ## End of entity description ##
#    file.write ( "  );\nEND %s;\n\n\n" %  LV_ModelName )    
#
#    ## Architecture description ##
#    file.write ( "ARCHITECTURE VBE OF %s IS\n\n" % LV_ModelName )
#
#    ## Signals ##  
#    
#
#
#    ## Behavior ##  
#    file.write ( "\nBEGIN\n\n" )
#
#
#
#
#    
#    ## Assert ##
#    file.write ( "  ASSERT (vdd = '1')\n" )
#    file.write ( "    REPORT \"Power supply is missing on vdd of Model %s.\"\n" % LV_modelName )
#    file.write ( "    SEVERITY WARNING;\n\n" )
#
#
#    file.write ( "  ASSERT (vss = '0')\n" )
#    file.write ( "    REPORT \"Power supply is missing on vss of Model %s.\"\n" % LV_modelName )
#    file.write ( "    SEVERITY WARNING;\n\n" )
#
#    ## End of Architectural description ##
#    file.write ( "END VBE;\n" )
#
#    file.close ()

####################
##### DpgenROM #####
####################
class DpgenROM ( TopRom ) :
    
  def Interface ( self ) : TopRom.Interface ( self )
  def Netlist ( self ) :   TopRom.Netlist   ( self, 0 )
  def Layout ( self ) :    topRom.Layout    ( self )
  def Vbe    ( self ) :    topRom.Vbe       ( self )
 

######################
##### DpgenROMTS #####
######################
class DpgenROMTS ( TopRom ) :
    
  def Interface ( self ) : TopRom.Interface ( self )
  def Netlist ( self ) :   TopRom.Netlist   ( self, 1 )
  def Layout ( self ) :    topRom.Layout    ( self )
  def Vbe    ( self ) :    topRom.Vbe       ( self )

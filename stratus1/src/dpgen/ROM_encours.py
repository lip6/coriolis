
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

class dpgen_ROM ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self, nbit, nword ) :
    
    if   nword ==   64 : adrange =  6
    elif nword ==  128 : adrange =  7
    elif nword ==  256 : adrange =  8
    elif nword ==  512 : adrange =  9
    elif nword == 1024 : adrange = 10
    else :
      print( "Error : DPGEN_ROM" )
      print( "The word number (nword := %d) ", nword, "can only takes values : 64, 128, 256, 512 or 1024.\n" )
      sys.exit ( 3 )
    
    if nbit % 4 != 0 or nbit > 64 or nbit < 4 :
      print( "Error : DPGEN_ROM\n" )
      print( "The bus width (nbit := %d) ", nbit, "must be multiple of 4 and no larger than 64.\n" )
      sys.exit ( 3 )
    
    ck     = LogicIn  (     "ck",       1 )
    selrom = LogicIn  ( "selrom",       1 )
    
    ad     = LogicIn  (     "ad", adrange )
    data   = LogicOut (   "data",    nbit )

    vdd    = VddIn    ( "vdd" )
    vss    = VssIn    ( "vss" )


  ## Architecture ##
  ##################
  def architecture ( self, nbit, nword ) :

#  //           Acquisition du tableau des valeurs a encoder
# 
#  /* ICI IL FAUT APPELER LE PARSER DE FICHIER .vbe  */
#  /*
#    for(i=0 ; i<1024 ; i++) {
#    data[2*i] = i;
#    data[2*i+1] = 1024 - i;
#    }
#  */
#  
#   dpgen_ROM_code ( LV_name, nbit, nword, 0, 0, data ) # !!!

    for i in range ( nbit ) :
      if   i % 4 == 0 :	
        cellin  = "rom_data_invss"
        cellmid = "rom_data_midvss"
        
        cellout = "rom_data_outvss"
        
      elif i % 4 == 1 :
        cellin  = "rom_data_insel"
        cellmid = "rom_data_midsel"
  
        cellout = "rom_data_outsel"
        
      elif i % 4 == 2:	
        cellin  = "rom_data_insel"
        cellmid = "rom_data_midsel"
        
        cellout = "rom_data_outsel"
  
      elif i % 4 == 3 :	
        cellin  = "rom_data_invss"
        cellmid = "rom_data_midvss"
        
        cellout = "rom_data_outvss"
    
    instanciate ( cellin
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
                        }
                )
    
    for j in range ( 0, nword, 64 ) :
      instanciate ( cellmid
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
                          }
                  )
      
      instanciate ( cellmid
                  , "bit%d_%d" % ( i, j + 32 )
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
                          }
                  )

    instanciate ( cellout
                , "out%d" % i
    	        , map = { 'nprech' : nprech
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
                        }
                )
 
  ## Placement des cellules decodeur ##
  
  # Lignes 0 et 1                          
  instanciate ( "rom_dec_prech"
              , "prech"
              , map = { 'nck'    : nck
                      , 'prech'  : prech
                      , 'nprech' : nprech
                      }
              )
  
  for j in range ( 0, nword, 64 ) :
    thisMap = { 'nck0'  : nck
  	          , 'nck1'  : nck
  	          , 'sel0'  : sel0
  	          , 'sel1'  : sel1
  	          , 'line0' : line0[j]
  	          , 'line1' : line1[j]
              }
    
  	if nword != 64 : thisMap['col'] = col[j]
  	else           : thisMap['col'] = vdd
    
  	instanciate ( "rom_dec_line01"
                , "and01_%d" % j
                , map = thisMap
                )
  	
  instanciate ( "rom_dec_selmux01"
              , "selmux01"
              , map = { 'a0'     : a0
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
                      , 'ck'     : ck
                      , 'selrom' : selrom
                      , 'nck'    : nck
                      , 'mux0'   : mux0
                      , 'sel0'   : sel0
                      , 'mux1'   : mux1
                      , 'sel1'   : sel1
                      }
              )
  
  # Lignes 2 et 3
  instanciate ( "rom_dec_adbuf"
              , "ad4"
              , map = { 'ad'   : ad[4]
                      , 'adx'  : a4
                      , 'nadx' : na4
                      }
              )
  
  instanciate ( "rom_dec_adbuf"
              , "ad5"
              , map = { 'ad'   : ad[5]
                      , 'adx'  : a5
                      , 'nadx' : na5
                      }
              )
  
  for j in range ( 0, nword, 64 ) :
    thisMap = { 'nck2'  : nck
  	          , 'nck3'  : nck
  	          , 'sel2'  : sel2
  	          , 'sel3'  : sel3
  	          , 'line2' : line2[j]
  	          , 'line3' : line3[j]
              }
      
  	if nword != 64 : thisMap['col'] = col[j]
  	else           : thisMap['col'] = vdd
    
  	instanciate ( "rom_dec_line23"
                , "and23_%d" % j
                , map = thisMap
                )
  	
  LV_name.PLACE_RIGHT ( "rom_dec_selmux23"
                      , "selmux23"
                      , map = { 'a0'     : a0
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
                              , 'ck'     : ck
                              , 'selrom' : selrom
                              , 'nck'    : nck
                              , 'mux2'   : mux2
                              , 'sel2'   : sel2
                              , 'mux3'   : mux3
                              , 'sel3'   : sel3
                              }
                      )
  
  # Lignes 4 et 5
  instanciate ( "rom_dec_adbuf"
              , "ad2"
              , map = { 'ad'   : ad[2]
                      , 'adx'  : a2
                      , 'nadx' : na2
                      }
              )
  
  nstanciate ( "rom_dec_adbuf"
             , "ad3"
             , map = { 'ad'   : ad[3]
                     , 'adx'  : a3
                     , 'nadx' : na3
                     }
             )

  for j in range ( 0, nword, 64 ) :
    thisMap = { 'nck4'  : nck
  	          , 'nck5'  : nck
  	          , 'sel4'  : sel4
  	          , 'sel5'  : sel5
  	          , 'line4' : line4[j]
  	          , 'line5' : line5[j]
              }

  	if nword != 64 : thisMap['col'] = col[j]
  	else           : thisMap['col'] = vdd
    
  	instanciate ( "rom_dec_line45"
                , "and45_%d" % j
                , map = thisMap
                )
  	
  instanciate ( "rom_dec_selmux45"
              , "selmux45"
	          , map = { 'a0'     : a0
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
	                  , 'ck'     : ck
	                  , 'selrom' : selrom
	                  , 'nck'    : nck
	                  , 'mux4'   : mux4
	                  , 'sel5'   : sel4
	                  , 'mux5'   : mux5
	                  , 'sek5'   : sel5
                      }
              )
  
  # Lignes 6 et 7
  instanciate ( "rom_dec_adbuf"
              , "ad0"
              , map = { 'ad'   : ad[0]
                      , 'adx'  : a0
                      , 'nadx' : na0
                      }
              )
  
  instanciate ( "rom_dec_adbuf"
              , "ad1"
              , map = { "ad   => ad[1]"
                      , "adx  => a1"
                      , "nadx => na1"
                      }
              )
  
  for j in range ( 0, nword, 64 ) :
    thisMap = { 'nck6'  : nck
  	          , 'nck7'  : nck
  	          , 'sel6'  : sel6
  	          , 'sel7'  : sel7
  	          , 'line6' : line6[j]
  	          , 'line7' : line7[j]
              }
      
  	if nword != 64 : thisMap['col'] = col[j]
  	else           : thisMap['col'] = vdd
    
  	instanciate ( "rom_dec_line67"
                , "and67_%d" % j
                , map = thisMap
                )

  if nword != 128 : # pas de sortie tristate
  	instanciate ( "rom_dec_selmux67"
                , "selmux67"
  	            , map = { 'a0'     : a0
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
  	                    , 'ck'     : ck
  	                    , 'selrom' : selrom
  	                    , 'nck'    : nck
  	                    , 'mux6'   : mux6
  	                    , 'sel6'   : sel6
  	                    , 'mux7'   : mux7
  	                    , 'sel7'   : sel7
                        }
                )

  else :   # pas de sortie tristate - 128 mots
  	instanciate ( "rom_dec_selmux67_128"
                , "selmux67"
  	            , map = { 'a0'     : a0
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
  	                    , 'a6'     : ad[6]
  	                    , 'selrom' : selrom
  	                    , 'a6x'    : col_64
  	                    , 'na6x'   : col_0
  	                    , 'mux6'   : mux6
  	                    , 'sel6'   : sel6
  	                    , 'mux7'   : mux7
  	                    , 'sel7'   : sel7
                        }
                )

  # Ligne 9 dans le cas 1024 mots
  if nword == 1024 :
    instanciate ( "rom_dec_nop"
                , "nop"
                )
    
    for j in range ( 0, nword, 64 ) :
        thisMap = { 'q' : col[j] }
        
        if ( j >> 6 ) % 2 == 0 : thisMap['i0'] = na6x
    	else                   : thisMap['i0'] = a6x
        
        if ( j >> 7 ) % 2 == 0 : thisMap['i1'] = na7x
    	else                   : thisMap['i1'] = a7x
        
        if ( j >> 8 ) % 2 == 0 : thisMap['i2'] = na8x
    	else                   : thisMap['i2'] = a8x
        
        if ( j >> 9 ) % 2 == 0 : thisMap['i3'] = na9x
    	else                   : thisMap['i3'] = a9x
        
    	instanciate ( "rom_dec_col4"
                    , "col_%d" % j
                    , map = thisMap
                    )

    instanciate ( "rom_dec_colbuf"
                , "colbuf6"
                , map = { " => ad[6]"
                        , " => a6x"
                        , " => na6x"
                )

    instanciate ( "rom_dec_colbuf"
                , "colbuf7"
                , NOSYM
                , FIXED
                , "a   => ad[7]"
                , "ax  => a7x"
                , "nax => na7x"
                , vdd
                , vss
                )
    
    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf8"
                        , NOSYM
                        , FIXED
                        , "a   => ad[8]"
                        , "ax  => a8x"
                        , "nax => na8x"
                        , vdd
                        , vss
                        )
    
    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf9"
                        , NOSYM
                        , FIXED
                        , "a   => ad[9]"
                        , "ax  => a9x"
                        , "nax => na9x"
                        , vdd
                        , vss
                        )
  
  # Ligne 9 dans le cas 512 mots
  if nword == 512 :
    LV_name.PLACE ( "rom_dec_nop"
                  , "nop"
                  , NOSYM
                  , 0
                  , ( nbit + 8 ) * HCELL
                  , FIXED
                  , vdd
                  , vss
                  )
    
    for j in range ( 0, nword, 64 ) :
        if ( j >> 6 ) % 2 == 0 : x6 = "i0 => na6x"
    	else                   : x6 = "i0 => a6x"
            
        if ( j >> 7 ) % 2 == 0 : x7 = "i1 => na7x"
    	else                   : x7 = "i1 => a7x"
            
        if ( j >> 8 ) % 2 == 0 : x8 = "i2 => na8x"
    	else                   : x8 = "i2 => a8x"
            
    	LV_name.PLACE_RIGHT ( "rom_dec_col3"
                            , "col_%d"      % j
                            , NOSYM
                            , FIXED
    	                    , x6
    	                    , x7
    	                    , x8
    	                    , "q => col_%d" % j
    	                    , vdd
    	                    , vss
                            )

    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf6"
                        , NOSYM
                        , FIXED
                        , "a   => ad[6]"
                        , "ax  => a6x"
                        , "nax => na6x"
                        , vdd
                        , vss
                        )

    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf7"
                        , NOSYM
                        , FIXED
                        , "a   => ad[7]"
                        , "ax  => a7x"
                        , "nax => na7x"
                        , vdd
                        , vss
                        )
    
    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf8"
                        , NOSYM
                        , FIXED
                        , "a   => ad[8]"
                        , "ax  => a8x"
                        , "nax => na8x"
                        , vdd
                        , vss
                        )
  
  # Ligne 9 dans le cas 256 mots
  if nword == 256 :
    LV_name.PLACE ( "rom_dec_nop"
                  , "nop"
                  , NOSYM
                  , 0
                  , ( nbit + 8 ) * HCELL
                  , FIXED
                  , vdd
                  , vss
                  )
        
    for j in range ( 0, nword, 64 ) :
      if ( j >> 6 ) % 2 == 0 : x6 = "i0 => na6x"
      else                   : x6 = "i0 => a6x"
          
      if ( j >> 7 ) % 2 == 0 : x7 = "i1 => na7x"
      else                   : x7 = "i1 => a7x"
          
      LV_name.PLACE_RIGHT ( "rom_dec_col2"
                          , "col_%d"      % j
                          , NOSYM
                          , FIXED
                          , x6
                          , x7
                          , "q => col_%d" % j
                          , vdd
                          , vss
                          )

    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf6"
                        , NOSYM
                        , FIXED
                        , "a   => ad[6]"
                        , "ax  => a6x"
                        , "nax => na6x"
                        , vdd
                        , vss
                        )
    
    LV_name.PLACE_RIGHT ( "rom_dec_colbuf"
                        , "colbuf7"
                        , NOSYM
                       , FIXED
                        , "a   => ad[7]"
                        , "ax  => a7x"
                        , "nax => na7x"
                        , vdd
                        , vss
                        )
  
#  ## Routage des rappels d'alimentation et des signaux hors grille ##
#  y1 = nbit * HCELL
#  
#  if nword > 128 : y2 = ( nbit + 9 ) * HCELL
#  else             : y2 = ( nbit + 8 ) * HCELL
#  
#  if type == 0     : x2 = ( nword // 64 * 50 ) + 150
#  else             : x2 = ( nword // 64 * 50 ) + 170
#  
#  # alimentations verticales ALU3
#  LV_name.PHSEG ( CALU3,12, "vdd", 10, 0, 10, y2)
#  LV_name.PHSEG ( CALU3, 2, "vss", 20, 0, 20, y2)
#  LV_name.PHSEG ( CALU3, 2, "vss", 30, 0, 30, y2)
#  
#  for j in range ( 0, nword, 64 ) :
#    LV_name.PHSEG ( CALU3, 2, "vss", 55 + ( 50 * j // 64 ), 0, 55 + ( 50 * j // 64 ), y2 )
#    LV_name.PHSEG ( CALU3, 2, "vss", 80 + ( 50 * j // 64 ), 0, 80 + ( 50 * j // 64 ), y2 )
#  
#  if type == 0 :
#    LV_name.PHSEG ( CALU3, 2, "vss", 125 + ( nword // 64 * 50 ), 0, 125 + ( nword // 64 * 50 ), y1 )
#    LV_name.PHSEG ( CALU3, 2, "vdd", 135 + ( nword // 64 * 50 ), 0, 135 + ( nword // 64 * 50 ), y1 )
#    LV_name.PHSEG ( CALU3, 2, "vdd", 145 + ( nword // 64 * 50 ), 0, 145 + ( nword // 64 * 50 ), y1 )
#
#  else :
#    LV_name.PHSEG ( CALU3, 2, "vss", 135 + ( nword // 64 * 50 ), 0, 135 + ( nword // 64 * 50 ), y1 )
#    LV_name.PHSEG ( CALU3, 2, "vdd", 145 + ( nword // 64 * 50 ), 0, 145 + ( nword // 64 * 50 ), y1 )
#    LV_name.PHSEG ( CALU3, 2, "vss", 155 + ( nword // 64 * 50 ), 0, 155 + ( nword // 64 * 50 ), y2 )
#    LV_name.PHSEG ( CALU3, 2, "vdd", 165 + ( nword // 64 * 50 ), 0, 165 + ( nword // 64 * 50 ), y2 )
#  
#  # alimemtations horizontales ALU2
#  for i in range ( 0, nbit, 4 ) :
#    LV_name.PHSEG ( CALU2, 2, "vss", 0, i * HCELL, x2, i * HCELL )
#
#  for l in range ( 0, 8, 2 ) :
#    LV_name.PHSEG ( CALU2, 2, "vss", 0, ( nbit + l     ) * HCELL, x2, ( nbit + l     ) * HCELL )
#    LV_name.PHSEG ( CALU2, 2, "vdd", 0, ( nbit + l + 1 ) * HCELL, x2, ( nbit + l + 1 ) * HCELL )
#
#  LV_name.PHSEG ( CALU2, 2, "vss", 0, ( nbit + 8 ) * HCELL, x2, ( nbit + 8 ) * HCELL )
#  
#  if nword > 128 : LV_name.PHSEG ( CALU2, 2, "vdd", 0, ( nbit + 9 ) * HCELL, x2, ( nbit + 9 ) * HCELL )
#  
#  # alimentations horizontales ALU1
#  for i in range ( 0, nbit, 2 ) :
#    LV_name.PHSEG ( CALU1, 6, "vss", 0     , HCELL * i +  3,  5, HCELL * i +  3 )
#    LV_name.PHSEG ( CALU1, 6, "vss", x2 - 5, HCELL * i +  3, x2, HCELL * i +  3 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", 0     , HCELL * i + 47,  5, HCELL * i + 47 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", x2 - 5, HCELL * i + 47, x2, HCELL * i + 47 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", 0     , HCELL * i + 53,  5, HCELL * i + 53 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", x2 - 5, HCELL * i + 53, x2, HCELL * i + 53 )
#    LV_name.PHSEG ( CALU1, 6, "vss", 0     , HCELL * i + 97,  5, HCELL * i + 97 )
#    LV_name.PHSEG ( CALU1, 6, "vss", x2 - 5, HCELL * i + 97, x2, HCELL * i + 97 )
#
#  for l in range ( 0, 8, 2 ) :
#    LV_name.PHSEG ( CALU1, 6, "vss", 0, y1 + HCELL * l +  3, x2, y1 + HCELL * l +  3 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", 0, y1 + HCELL * l + 47, x2, y1 + HCELL * l + 47 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", 0, y1 + HCELL * l + 53, x2, y1 + HCELL * l + 53 )
#    LV_name.PHSEG ( CALU1, 6, "vss", 0, y1 + HCELL * l + 97, x2, y1 + HCELL * l + 97 )
#
#  if nword > 128 :
#    LV_name.PHSEG ( CALU1, 6, "vss", 0, y1 + 403, x2, y1 + 403 )
#    LV_name.PHSEG ( CALU1, 6, "vdd", 0, y1 + 447, x2, y1 + 447 )
#  
#  # routage des lignes de bit horizontales hors grille
#  for i in range ( nbit ) :
#    for l inr ange ( 8 ) :
#      x1 = GET_REF_X ( "in%d"  % i), "bit%d" % ,l ) )
#      y1 = GET_REF_Y ( "in%d"  % i), "bit%d" % ,l ) )
#      x2 = GET_REF_X ( "out%d" % i), "bit%d" % ,l ) )
#      y2 = GET_REF_Y ( "out%d" % i), "bit%d" % ,l ) )
#      
#      LV_name.PHSEG ( ALU2, 2, "bit%d_%d" % ( l, i ), x1, y1, x2, y2 )
#  
#  # cablage a 1 de l'entree col dans le cas 64 mots
#  if nword == 64 :
#    x1 = GET_REF_X ( "and01_0", "refcol" )
#    y1 = GET_REF_Y ( "and01_0", "refcol" )
#    PHBIGVIA ( CONT_VIA2, x1, y1, 0, 0, "vdd" )
#    
#    x1 = GET_REF_X ( "and23_0", "refcol" )
#    y1 = GET_REF_Y ( "and23_0", "refcol" )
#    PHBIGVIA ( CONT_VIA2, x1, y1, 0, 0, "vdd" )
#    
#    x1 = GET_REF_X ( "and45_0", "refcol" )
#    y1 = GET_REF_Y ( "and45_0", "refcol" )
#    PHBIGVIA ( CONT_VIA2, x1, y1, 0, 0, "vdd" )
#    
#    x1 = GET_REF_X ( "and67_0", "refcol" )
#    y1 = GET_REF_Y ( "and67_0", "refcol" )
#    PHBIGVIA ( CONT_VIA2, x1, y1, 0, 0, "vdd" )
#   
#    # Codage Plan Memoire (codage diffusion)                
# a modifier pour creer ces segments dans une figure separee
#  for j in range ( 0, nword ; j+=32) : # boucle sur les cellules 
#    for k in range ( 4 ) :               # boucle sur les colonnes 
#      for l in range ( 8 ) :             # boucle sur le multiplexeur de sortie
#        for i in range ( nbit ) :        # boucle sur les bits du mot
#          x1 = GET_REF_X ( "bit%d_%d" % ( i, j ), "ref%d%d" % ( k, l ) )
#          y1 = GET_REF_Y ( "bit%d_%d" % ( i, j ), "ref%d%d" % ( k, l ) )
#          
#          if i < 32 :
#            value = data[2*(j + 8*k + l)]
#                
#            if (value>>i ) % 2 == 0 : LV_name.PHSEG ( NTRANS, 1, "", x1, y1 - 3, x1, y1 + 3 )
#            else                    : LV_name.PHSEG ( POLY  , 1, "", x1, y1 - 3, x1, y1 + 3 )
#
#          else :
#            value = data[2*(j + 8*k + l)+1]
#                
#            if ( value>>(i-32) ) % 2 == 0 : LV_name.PHSEG ( NTRANS, 1, "", x1, y1 - 3, x1, y1 + 3 )
#            else                          : LV_name.PHSEG ( POLY  , 1, "", x1, y1 - 3, x1, y1 + 3 )
  



########## ROM_VHDL ##########
##############################
def ROM_VHDL () :
  global nbit
  global LV_flags
  global LV_ModelName
  global nword

  print( "ROM behavior not yet implemented" )

#  fileName = LV_ModelName + ".vbe"
#  
#  file = open ( fileName, "w+" )
#  
#  if   nword ==   64 : adrange =  6
#  elif nword ==  128 : adrange =  7
#  elif nword ==  256 : adrange =  8
#  elif nword ==  512 : adrange =  9
#  elif nword == 1024 : adrange = 10
#  
#  sBusWide0 = VHDL_BUS_WIDE (    nbit - 1, 0, nbit, LV_flags )
#  sBusWide1 = VHDL_BUS_WIDE ( adrange - 1, 0, nbit, LV_flags )
#
#  ## Entity description ##
#  file.write ( "\nENTITY %s IS\n  PORT (\n" % LV_ModelName )
#  
#  file.write ( "%14s : in    BIT;\n" % "ck"  )
#  file.write ( "%14s : in    BIT;\n" % "selrom" )
#  file.write ( "%14s : in    BIT_VECTOR %s;\n" % (   "ad", sBusWide1 ) )
#  file.write ( "%14s : out   BIT_VECTOR %s;\n" % ( "data", sBusWide0 ) )
#
#  ## Power supplies terminals ##  
#  file.write ( "%14s : in    BIT;\n" % "vdd" )
#  file.write ( "%14s : in    BIT\n"  % "vss" )
#  
#  ## End of entity description ##
#  file.write ( "  );\nEND %s;\n\n\n" %  LV_ModelName )    
#
#  ## Architecture description ##
#  file.write ( "ARCHITECTURE VBE OF %s IS\n\n" % LV_ModelName )
#
#  ## Signals ##  
#  
#
#
#  ## Behavior ##  
#  file.write ( "\nBEGIN\n\n" )
#
#
#
#
#  
#  ## Assert ##
#  file.write ( "  ASSERT (vdd = '1')\n" )
#  file.write ( "    REPORT \"Power supply is missing on vdd of Model %s.\"\n" % LV_modelName )
#  file.write ( "    SEVERITY WARNING;\n\n" )
#
#
#  file.write ( "  ASSERT (vss = '0')\n" )
#  file.write ( "    REPORT \"Power supply is missing on vss of Model %s.\"\n" % LV_modelName )
#  file.write ( "    SEVERITY WARNING;\n\n" )
#
#  ## End of Architectural description ##
#  file.write ( "END VBE;\n" )
#
#  file.close ()

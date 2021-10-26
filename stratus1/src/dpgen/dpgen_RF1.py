
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
# |  Py Module   :       "./dpgen_RF1.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

# Banc de registre double acces : une lecture, une ecriture
# Le programme genere une net-list et un placement optimise.
# On peut generer trois types d'operateurs:
#	- banc de registres sans decodeur
#	- banc de registres avec decodeur
#	- banc de registre de LV_type fifo
#
# La largeur est egale a ( nmot * 25 ) + 100
# La hauteur est egale a ( nbit + 2  ) * 50 sans decodeur
#                        ( nbit + 6  ) * 50 avec decodeur
#                        ( nbit + 8  ) * 50 pour la fifo 
#
# Pour la partie "donnees", on utilise les LV_names suivantes :
#   colonne d'entree 
# - rf_inmux_mem          
# - rf_inmux_buf_2        
# - rf_inmux_buf_4        
#   colonne centrale
# - rf_mid_mem          
# - rf_mid_mem_r0          
# - rf_mid_buf_2        
# - rf_mid_buf_4        
#   colonne de sortie
# - rf_out_mem          
# - rf_out_buf_2        
# - rf_out_buf_4        
#
# Pour le controleur fifo, on utilise les LV_names suivantes :
# - rf_fifo_buf
# - rf_fifo_clock
# - rf_fifo_empty
# - rf_fifo_full
# - rf_fifo_inc
# - rf_fifo_nop
# - rf_fifo_ok
# - rf_fifo_orand4
# - rf_fifo_orand5
# - rf_fifo_ptreset
# - rf_fifo_ptset
#
# Pour le decodeur, on utilise les LV_names suivantes :
# - rf_dec_bufad0
# - rf_dec_bufad1
# - rf_dec_bufad2
# - rf_dec_nbuf
# - rf_dec_nor3
# - rf_dec_nao3
# - rf_dec_nand2
# - rf_dec_nand3
# - rf_dec_nand4


#----------------------------------------------------------------
#	DEBUT PROGRAMME  PRINCIPAL			   
#----------------------------------------------------------------
# aFunction : le type  :
#       DPGEN_RF1      : sans decodeur
#       DPGEN_RF1R0    : sans decodeur, avec R0 colle a 0
#       DPGEN_RF1D     : avec decodeur
#       DPGEN_RF1DR0   : avec decodeur, avec R0 colle a 0
#       DPGEN_FIFO     : une fifo
#
# Liste des parametres :
#          ModelName   :  nom de modele.
#          flags       :  indexation des connecteurs.
#          n           :  nombre de bits (largeur du bus).
#          nmot        :  nombre de mots. 
# - le nombre de mots doit etre pair et =< 32
# - le nombre de bits doit etre pair, non nul et =< 64
#----------------------------------------------------------------



###################
##### top_RF1 #####
###################
class  top_rf1 ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self, nbit, nword, type ) :
    global nbitu, hbuf, adrange

    nbitu = 0

    if nword % 2 == 1 or nword > 32 or nword < 4 :
      print( "Error in DpgenRf1 : the word number must be even larger than 4 and no larger than 32\n" )
      sys.exit ( 3 )
    
    if nword == 4 and ( type == 2 or type == 3 ) :
      print( "Error in DpgenRf1 : the word number must be larger than 4\n" )
      sys.exit ( 3 )
    
    if nbit % 2 == 1 or nbit > 64 or nbit < 2 :
      print( "Error in DpgenRf1 : the bit number must be even and no larger than 64\n" )
      sys.exit ( 3 )

    adrange = 2
    if nword > 4  : adrange = 3
    if nword > 8  : adrange = 4
    if nword > 16 : adrange = 5
    
    if type < 2 :   # c'est un banc de registres sans decodeur  
      self._ck      = SignalIn    (       "ck",       1 )
      self._sel     = SignalIn    (      "sel",       1 )

      self._selr    = SignalIn    (     "selr",   nword )
      self._selw    = SignalIn    (     "selw",   nword )
      self._datain0 = SignalIn    (  "datain0",    nbit )
      self._datain1 = SignalIn    (  "datain1",    nbit )
      self._dataout = SignalOut   (  "dataout",    nbit )
  
    elif type < 4 : # c'est un banc de registres avec decodeur
      self._ck      = SignalIn    (       "ck",       1 )
      self._sel     = SignalIn    (      "sel",       1 )
      self._wen     = SignalIn    (      "wen",       1 )
      self._ren     = SignalIn    (      "ren",       1 )
                     
      self._adr     = SignalIn    (      "adr", adrange )
      self._adw     = SignalIn    (      "adw", adrange )
      self._datain0 = SignalIn    (  "datain0",    nbit )
      self._datain1 = SignalIn    (  "datain1",    nbit )
      self._dataout = SignalOut   (  "dataout",    nbit )

      self._selr    = Signal     (     "selr",   nword )
      self._selw    = Signal     (     "selw",   nword )
    
    else :		     # c'est une fifo
      self._ck      = SignalIn    (       "ck",       1 )
      self._reset   = SignalIn    (    "reset",       1 )
      self._r       = SignalIn    (        "r",       1 )
      self._w       = SignalIn    (        "w",       1 )
      self._rok     = SignalInOut (      "rok",       1 )
      self._wok     = SignalInOut (      "wok",       1 )
      self._sel     = SignalIn    (      "sel",       1 )
      
      self._datain0 = SignalIn    (  "datain0",    nbit )
      self._datain1 = SignalIn    (  "datain1",    nbit )
      self._dataout = SignalOut   (  "dataout",    nbit )

      self._selr    = Signal     (     "selr",   nword )
      self._selw    = Signal     (     "selw",   nword )
      self._ckok    = Signal     (     "ckok",       1 )

    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )


  ## Netlist  ##
  ##############
  def Netlist ( self, nbit, nword, type ) :
    
    hbuf     = 2   # hauteur des buffers (2 ou 4 slices)  : non utilise
  
    # choix des LV_names buffer
    if hbuf == 2 :	# buffers sur 2 slices
    	cell_in_buf  = "rf_inmux_buf_2"
    	cell_mid_buf = "rf_mid_buf_2"
    	cell_out_buf = "rf_out_buf_2"
      
    elif hbuf == 4 :	# buffers sur 4 slices
      cell_in_buf  = "rf_inmux_buf_4"
      cell_mid_buf = "rf_mid_buf_4"
      cell_out_buf = "rf_out_buf_4"


    sel0      = Signal (   "sel0",    1 )
    sel1      = Signal (   "sel1",    1 )
    xck       = Signal (    "xck",    1 )
    nck       = Signal (    "nck",    1 )

    rbus      = Signal (  "rbus",  nbit )
    dinx      = Signal (  "dinx",  nbit )

    write = []
    read  = []
    for i in range ( nword ) : 
      write += [Signal ( "write%d" % i, 1 )]
      read  += [Signal (  "read%d" % i, 1 )]

    if type != 4 :
      nandr   = []
      for i in range ( nword >> 1 ) :
        nandr   += [Signal (   "nandr%d" % i, 1 )]

    cell_in_mem  = "rf_inmux_mem"
    cell_out_mem = "rf_out_mem"  
    
    self.In  = {}
    self.Bit = {}
    self.Out = {}
     
    for i in range ( nbit ) :
      self.In[nbit-1-i] = Inst ( cell_in_mem
                               , "in%d" % ( nbit - 1 - i )
                               , map = { 'datain0' : self._datain0[nbit - 1 - i]
                                       , 'datain1' : self._datain1[nbit - 1 - i]
                                       , 'sel0'    : sel0
                                       , 'sel1'    : sel1
                                       , 'dinx'    : dinx[nbit - 1 - i]
                                       , 'vdd'     : self._vdd
                                       , 'vss'     : self._vss
                                       }
                               )
      
      self.BitBis = {}
      
      for j in range ( 0, nword, 2 ) :
          
        if j == 0 and type % 2 == 1 : cell_mid_mem = "rf_mid_mem_r0"
        else                        : cell_mid_mem = "rf_mid_mem"
        
        self.BitBis[j] = Inst ( cell_mid_mem
                              , "bit%d_%d" % ( nbit - 1 - i, j )
                              , map = { 'dinx'  : dinx[nbit - 1 - i]
                                      , 'write' : write[j]
                                      , 'read'  : read[j]
                                      , 'rbus'  : rbus[nbit - 1 - i]
                                      , 'vdd'   : self._vdd
                                      , 'vss'   : self._vss
                                      }
                              )
        
        cell_mid_mem = "rf_mid_mem"
        
        self.BitBis[j+1] = Inst ( cell_mid_mem
                                , "bit%d_%d" % ( nbit - 1 - i, j + 1 )
                                , map = { 'dinx'  : dinx[nbit - 1 - i]
                                        , 'write' : write[j + 1]
                                        , 'read'  : read[j + 1]
                                        , 'rbus'  : rbus[nbit - 1 - i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )

      self.Bit[nbit-1-i] = self.BitBis
        
      self.Out[nbit-1-i] = Inst ( cell_out_mem
                                , "out%d" % ( nbit - 1 - i )
                                , map = { 'rbus'    : rbus[nbit - 1 - i]
                                        , 'xcks'    : xck
                                        , 'dataout' : self._dataout[nbit - 1 - i]
                                        , 'vdd'     : self._vdd
                                        , 'vss'     : self._vss
                                        }
                                )
   
                  
    ## Placement de la ligne contenant les buffers ##
    if type < 4 :
      self.InBuf = Inst ( cell_in_buf
                        , "inbuf"
                        , map = { 'ck'   : self._ck
                                , 'sel'  : self._sel
                                , 'nck'  : nck
                                , 'sel0' : sel0
                                , 'sel1' : sel1
                                , 'vdd'  : self._vdd
                                , 'vss'  : self._vss
                                }
                        )
    else :
      self.InBuf = Inst ( cell_in_buf
                        , "inbuf"
                        , map = { 'ck'   : self._ckok
                                , 'sel'  : self._sel
                                , 'nck'  : nck
                                , 'sel0' : sel0
                                , 'sel1' : sel1
                                , 'vdd'  : self._vdd
                                , 'vss'  : self._vss
                                }
                        )
     
    self.Buf = {}
      
    for j in range ( 0, nword, 2 ) :	
      self.Buf[j] = Inst ( cell_mid_buf
                         , "buf%d" % j
                         , map = { 'selr'  : self._selr[j]
                                 , 'selw'  : self._selw[j]
                                 , 'nck'   : nck
                                 , 'read'  : read[j]
                                 , 'write' : write[j]
                                 , 'vdd'   : self._vdd
                                 , 'vss'   : self._vss
                                 }
                         )
      self.Buf[j+1] = Inst ( cell_mid_buf
                           , "buf%d" % ( j + 1 )
                           , map = { 'selr'  : self._selr[j + 1]
                                   , 'selw'  : self._selw[j + 1]
                                   , 'nck'   : nck
                                   , 'read'  : read[j + 1]
                                   , 'write' : write[j + 1]
                                   , 'vdd'   : self._vdd
                                   , 'vss'   : self._vss
                                   }
                           )
  
    self.OutBuf = Inst ( cell_out_buf
                       , "outbuf"
                       , map = { 'nck'   : nck
                               , 'xcks'  : xck
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                               }
                       )
    
    
    ## Placement des lignes de bit au dessus des buffers ##
    cell_in_mem  = "rf_inmux_mem"
    cell_out_mem = "rf_out_mem"

    for i in range  ( nbitu ) :
      self.In[nbitu-1-i] = Inst ( cell_in_mem
                                , "in%d" % ( nbitu - 1 - i )
                                , map = { 'datain0' : datain0[nbitu - 1 - i]
                                        , 'datain1' : datain1[nbitu - 1 - i]
                                        , 'sel0'    : sel0
                                        , 'sel1'    : sel1
                                        , 'dinx'    : dinx[nbitu - 1 - i]
                                        , 'vdd'     : self._vdd
                                        , 'vss'     : self._vss
                                        }
                                )

      self.BitBis = {}
      
      for j in range ( 0, nword, 2 ) :
        if j == 0 and type % 2 == 1 : cell_mid_mem = "rf_mid_mem_r0"
        else                        : cell_mid_mem = "rf_mid_mem"
        
        self.BitBis[j] = Inst ( cell_mid_mem
                              , "bit%d_%d" % ( nbitu - 1 - i, j )
                              , map = { 'dinx'  : dinx[nbitu - 1 - i]
                                      , 'write' : write[j]
                                      , 'read'  : read[j]
                                      , 'rbus'  : rbus[nbitu - 1 - i]
                                      , 'vdd'   : self._vdd
                                      , 'vss'   : self._vss
                                      }
                              )
        
        cell_mid_mem = "rf_mid_mem"
        
        self.BitBis[j+1] = Inst ( cell_mid_mem
                                , "bit%d_%d" % ( nbitu - 1 - i, j + 1 )
                                , map = { 'dinx'  : dinx[nbitu - 1 - i]
                                        , 'write' : write[j + 1]
                                        , 'read'  : read[j + 1]
                                        , 'rbus'  : rbus[nbitu - 1 - i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )

      self.Bit[nbitu-1-i] = self.BitBis
        
      self.Out[nbitu-1-i] = Inst ( cell_out_mem
                                 , "out%d" % ( nbitu - 1 - i )
                                 , sym1
                                 , map = { 'rbus'    : rbus[nbitu - 1 - i]
                                         , 'xcks'    : xck
                                         , 'dataout' : dataout[nbitu - 1 - i]
                                         , 'vdd'     : self._vdd
                                         , 'vss'     : self._vss
                                         }
                                 )
  
      
    ## Placement des cellules du decodeur ##
    if type == 2 or type == 3 :
      nwen      = Signal (   "nwen",    1 )
      nren      = Signal (   "nren",    1 )

      nadw0x    = Signal ( "nadw0x",    1 )
      adw0x     = Signal (  "adw0x",    1 )

      adr0x     = Signal (   "adr0x",   1 )
      nadr0x    = Signal (  "nadr0x",   1 )
    
      nandw   = []
      for i in range ( nword >> 1 ) :
        nandw   += [Signal (   "nandw%d" % i, 1 )]


      half = nword >> 1
      
      # Premiere ligne decodeur ecriture
      self.Adw0 = Inst ( "rf_dec_bufad0"
                       , "adw0"
                       , map = { 'i'   : self._adw[0]
                               , 'nq'  : nadw0x
                               , 'q'   : adw0x
                               , 'vdd' : self._vdd
                               , 'vss' : self._vss
                               }
                       )
      
      self.SelW = {}
      
      for j in range ( half ) :
        self.SelW[2*j] = Inst ( "rf_dec_nor3"
                              , "selw%d" % ( 2 * j )
                              , map = { 'i0'  : nandw[j]
                                      , 'i1'  : adw0x
                                      , 'i2'  : nwen
                                      , 'nq'  : self._selw[2 * j]
                                      , 'vdd' : self._vdd
                                      , 'vss' : self._vss
                                      }
                              )
        self.SelW[2*j+1] = Inst ( "rf_dec_nor3"
                                , "selw%d" % ( 2 * j + 1 )
                                , map = { 'i0'  : nandw[j]
                                        , 'i1'  : nadw0x
                                        , 'i2'  : nwen
                                        , 'nq'  : self._selw[2 * j + 1]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
      self.Wen = Inst ( "rf_dec_nbuf"
                      , "wen"
                      , map = { 'i'   : self._wen
                              , 'nq'  : nwen
                              , 'vdd' : self._vdd
                              , 'vss' : self._vss
                              }
                       )
      
      # Deuxieme ligne decodeur ecriture
      if adrange == 5 :    # adresse sur 5 bits
      
        nadw1x    = Signal ( "nadw1x",    1 )
        adw1x     = Signal (  "adw1x",    1 )
        nadw2x    = Signal ( "nadw2x",    1 )
        adw2x     = Signal (  "adw2x",    1 )
        adw3x     = Signal (  "adw3x",    1 )
        nadw3x    = Signal ( "nadw3x",    1 )
        adw4x     = Signal (  "adw4x",    1 )
        nadw4x    = Signal ( "nadw4x",    1 )
        
        adr1x     = Signal (   "adr1x",   1 )
        nadr1x    = Signal (  "nadr1x",   1 )
        adr2x     = Signal (   "adr2x",   1 )
        nadr2x    = Signal (  "nadr2x",   1 )
        adr3x     = Signal (   "adr3x",   1 )
        nadr3x    = Signal (  "nadr3x",   1 )
        adr4x     = Signal (   "adr4x",   1 )
        nadr4x    = Signal (  "nadr4x",   1 )

        self.Adw12 = Inst ( "rf_dec_bufad2"
                          , "adw12"
                          , map = { 'i0'  : self._adw[1]
                                  , 'i1'  : self._adw[2]
                                  , 'nq0' : nadw1x
                                  , 'q0'  : adw1x
                                  , 'nq1' : nadw2x
                                  , 'q1'  : adw2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )

        self.Nand4W = {}
        
        for j in range ( half ) :
          thisMap = { 'nq' : nandw[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadw1x
          else                   : thisMap['i0'] = adw1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadw2x
          else                   : thisMap['i1'] = adw2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadw3x
          else                   : thisMap['i2'] = adw3x
          if ( j >> 3 ) % 2 == 0 : thisMap['i3'] = nadw4x
          else                   : thisMap['i3'] = adw4x
          
          self.Nand4W[j] = Inst ( "rf_dec_nand4"
                                , "nand4w%d" % j
                                , map = thisMap
                                )
          
        self.Adw34 = Inst ( "rf_dec_bufad2"
                          , "adw34"
                          , map = { 'i0'  : self._adw[3]
                                  , 'i1'  : self._adw[4]
                                  , 'nq0' : nadw3x
                                  , 'q0'  : adw3x
                                  , 'nq1' : nadw4x
                                  , 'q1'  : adw4x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )
    
      elif adrange == 4 :    # adresse sur 4 bits
      
        nadw1x    = Signal ( "nadw1x",    1 )
        adw1x     = Signal (  "adw1x",    1 )
        nadw2x    = Signal ( "nadw2x",    1 )
        adw2x     = Signal (  "adw2x",    1 )
        adw3x     = Signal (  "adw3x",    1 )
        nadw3x    = Signal ( "nadw3x",    1 )
        
        adr1x     = Signal (   "adr1x",   1 )
        nadr1x    = Signal (  "nadr1x",   1 )
        adr2x     = Signal (   "adr2x",   1 )
        nadr2x    = Signal (  "nadr2x",   1 )
        adr3x     = Signal (   "adr3x",   1 )
        nadr3x    = Signal (  "nadr3x",   1 )

        self.Adw12 = Inst ( "rf_dec_bufad2"
                          , "adw12"
                          , map = { 'i0'  : self._adw[1]
                                  , 'i1'  : self._adw[2]
                                  , 'nq0' : nadw1x
                                  , 'q0'  : adw1x
                                  , 'nq1' : nadw2x
                                  , 'q1'  : adw2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )

        self.Nand3W = {}
      
        for j in range ( half ) :
          thisMap = { 'nq' : nandw[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadw1x
          else                   : thisMap['i0'] = adw1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadw2x
          else                   : thisMap['i1'] = adw2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadw3x
          else                   : thisMap['i2'] = adw3x
          
          self.Nand3W[j] = Inst ( "rf_dec_nand3"
                                , "nand3w%d" % j
                                , map = thisMap
                                )
          
        self.Adw3 = Inst ( "rf_dec_bufad1"
                         , "adw3"
                         , map = { 'i'   : self._adw[3]
                                 , 'nq'  : nadw3x
                                 , 'q'   : adw3x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
    
      elif adrange == 3 :    # adresse sur 3 bits
      
        nadw1x    = Signal ( "nadw1x",    1 )
        adw1x     = Signal (  "adw1x",    1 )
        nadw2x    = Signal ( "nadw2x",    1 )
        adw2x     = Signal (  "adw2x",    1 )
        
        adr1x     = Signal (   "adr1x",   1 )
        nadr1x    = Signal (  "nadr1x",   1 )
        adr2x     = Signal (   "adr2x",   1 )
        nadr2x    = Signal (  "nadr2x",   1 )

        self. Adw1 = Inst ( "rf_dec_bufad1"
                          , "adw1"
                          , map = { 'i'   : self._adw[1]
                                  , 'nq'  : nadw1x
                                  , 'q'   : adw1x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )

        self.Nand2W = {}
        
        for j in range ( half ) :
          thisMap = { 'nq' : nandw[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadw1x
          else                   : thisMap['i0'] = adw1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadw2x
          else                   : thisMap['i1'] = adw2x
          
          self.Nand2W[j] = Inst ( "rf_dec_nand2"
                                , "nand2w%d" % j
                                , map = thisMap
                                )
        
        self.Adw2 = Inst ( "rf_dec_bufad1"
                         , "adw2"
                         , map = { 'i'   : self._adw[2]
                                 , 'nq'  : nadw2x
                                 , 'q'   : adw2x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
    
      # Premiere ligne decodeur lecture
      self.Adr0 = Inst ( "rf_dec_bufad0"
                       , "adr0"
                       , map = { 'i'   : self._adr[0]
                               , 'nq'  : nadr0x
                               , 'q'   : adr0x
                               , 'vdd' : self._vdd
                               , 'vss' : self._vss
                               }
                       )

      self.SelR = {}
      
      for j in range ( half ) :
        if j == 0 :
      	  self.SelR[2*j] = Inst ( "rf_dec_nao3"
                                , "selr%d" % ( 2 * j )
                                , map = { 'i0'  : nandr[j]
                                        , 'i1'  : adr0x
                                        , 'i2'  : self._ren
                                        , 'nq'  : self._selr[2 * j]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
        else :
      	  self.SelR[2*j] = Inst ( 'rf_dec_nor3'
                                , 'selr%d' % ( 2 * j )
                                , map = { 'i0'  : nandr[j]
                                        , 'i1'  : adr0x
                                        , 'i2'  : nren
                                        , 'nq'  : self._selr[2 * j]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
                      
        self.SelR[2*j+1] = Inst ( "rf_dec_nor3"
                                , 'selr%d' % ( 2 * j + 1 )
                                , map = { 'i0'  : nandr[j]
                                        , 'i1'  : nadr0x
                                        , 'i2'  : nren
                                        , 'nq'  : self._selr[2 * j + 1]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
    
      self.Ren = Inst ( 'rf_dec_nbuf'
                      , 'ren'
                      , map = { 'i'   : self._ren
                              , 'nq'  : nren
                              , 'vdd' : self._vdd
                              , 'vss' : self._vss
                              }
                      )
      
      # Deuxieme ligne decodeur lecture
      if adrange == 5 :    # adresse sur 5 bits
        self.Adr12 = Inst ( 'rf_dec_bufad2'
                          , 'adr12'
                          , map = { 'i0'  : self._adr[1]
                                  , 'i1'  : self._adr[2]
                                  , 'nq0' : nadr1x
                                  , 'q0'  : adr1x
                                  , 'nq1' : nadr2x
                                  , 'q1'  : adr2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )

        self.Nand4R = {}
        
        for j in range ( half ) :
          thisMap = { 'nq':  nandr[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadr1x
          else                   : thisMap['i0'] = adr1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadr2x
          else                   : thisMap['i1'] = adr2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadr3x
          else                   : thisMap['i2'] = adr3x
          if ( j >> 3 ) % 2 == 0 : thisMap['i3'] = nadr4x
          else                   : thisMap['i3'] = adr4x
          
          self.Nand4R[j] = Inst ( 'rf_dec_nand4'
                                , 'nand4r%d' % j
                                , map = thisMap
                                )
    
        self.Adr34 = Inst ( 'rf_dec_bufad2'
                          , 'adr34'
                          , map = { 'i0'  : self._adr[3]
                                  , 'i1'  : self._adr[4]
                                  , 'nq0' : nadr3x
                                  , 'q0'  : adr3x
                                  , 'nq1' : nadr4x
                                  , 'q1'  : adr4x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )
      
      if adrange == 4 :    # adresse sur 4 bits
        self.Adr12 = Inst ( 'rf_dec_bufad2'
                          , 'adr12'
                          , map = { 'i0'  : self._adr[1]
                                  , 'i1'  : self._adr[2]
                                  , 'nq0' : nadr1x
                                  , 'q0'  : adr1x
                                  , 'nq1' : nadr2x
                                  , 'q1'  : adr2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )

        self.Nand3R = {}
        
        for j in range ( half ) :
          thisMap = { 'nq' : nandr[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadr1x
          else                   : thisMap['i0'] = adr1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadr2x
          else                   : thisMap['i1'] = adr2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadr3x
          else                   : thisMap['i2'] = adr3x
          
          self.Nand3R[j] = Inst ( 'rf_dec_nand3'
                                , 'nand3r%d' % j
                                , map = thisMap
                                )
    
        self.Adr3 = Inst ( 'rf_dec_bufad1'
                         , 'adr3'
                         , map = { 'i'   : self._adr[3]
                                 , 'nq'  : nadr3x
                                 , 'q'   : adr3x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
      
      if adrange == 3 :    # adresse sur 3 bits
        self.Adr1 = Inst ( 'rf_dec_bufad1'
                         , 'adr1'
                         , map = { 'i'   : self._adr[1]
                                 , 'nq'  : nadr1x
                                 , 'q'   : adr1x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )

        self.Nand2R = {}
            
        for j in range ( half ) :
          thisMap = { 'nq' : nandr[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j       % 2 == 0  : thisMap['i0'] = nadr1x
          else                   : thisMap['i0'] = adr1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadr2x
          else                   : thisMap['i1'] = adr2x
          
          self.Nand2R[j] = Inst ( 'rf_dec_nand2'
                                , 'nand2r%d' % j
                                , map = thisMap
                                )
    
        self.Adr2 = Inst ( 'rf_dec_bufad1'
                         , 'adr2'
                         , map = { 'i'   : self._adr[2]
                                 , 'nq'  : nadr2x
                                 , 'q'   : adr2x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
        
# end if type == 2 or type == 3
 
    ## Placement des cellules du controleur FIFO  ##
    if type == 4 :
      ckm       = Signal (    "ckm",    1 )
      cks       = Signal (    "cks",    1 )
      nreset    = Signal ( "nreset",    1 )
      nwval     = Signal (  "nwval",    1 )
      pwnop     = Signal (  "pwnop",    1 )
      pwinc     = Signal (  "pwinc",    1 )
      prnop     = Signal (  "prnop",    1 )
      princ     = Signal (  "princ",    1 )
      xreset    = Signal ( "xreset",    1 )
      xckm      = Signal (   "xckm",    1 )
      xcks      = Signal (   "xcks",    1 )
      nr        = Signal (     "nr",    1 )
      nw        = Signal (     "nw",    1 )
      full      = Signal (      "full", 1 )
      fullnext  = Signal (  "fullnext", 1 )
      empty     = Signal (     "empty", 1 )
      emptynext = Signal ( "emptynext", 1 )
      nrval     = Signal (  "nrval",    1 )
      
      orfull  = []
      orempty = []
      for i in range ( nword >> 1 ) :
        orfull  += [Signal (  "orfull%d" % i, 1 )]
        orempty += [Signal ( "orempty%d" % i, 1 )]

      
      half = nword >> 1
      
      # Premiere ligne
      self.Inspwnop = Inst ( 'rf_fifo_nop'
                           , 'inspwnop'
                           , map = { 'ckm'    : ckm
                                   , 'nreset' : nreset
                                   , 'rw'     : self._w
                                   , 'rwok'   : self._wok
                                   , 'nval'   : nwval
                                   , 'nop'    : pwnop
                                   , 'vdd'    : self._vdd
                                   , 'vss'    : self._vss
                                   }
                           )
      
      self.Pw = {}
      
      self.Pw[0] = Inst ( 'rf_fifo_ptset'
                        , 'pw0'
                        , map = { 'nop'    : pwnop
                                , 'inc'    : pwinc
                                , 'cks'    : xcks
                                , 'nreset' : nreset
                                , 'ptm1'   : self._selw[nword - 1]
                                , 'pt'     : self._selw[0]
                                , 'vdd'    : self._vdd
                                , 'vss'    : self._vss
                                }
                        )
      
      for j in range ( 1, half, 1 ) :
        self.Pw[j] = Inst ( 'rf_fifo_ptreset'
                          , 'pw%d' % j
                          , map = { 'nop'   : pwnop
                                  , 'inc'   : pwinc
                                  , 'cks'   : xcks
                                  , 'reset' : xreset
                                  , 'ptm1'  : self._selw[j - 1]
                                  , 'pt'    : self._selw[j]
                                  , 'vdd'   : self._vdd
                                  , 'vss'   : self._vss
                                  }
                          )
  
      self.FifoBuf = Inst ( 'rf_fifo_buf'
                          , 'fifo_buf'
                          , map = { 'cks'    : cks
                                  , 'ckm'    : ckm
                                  , 'r'      : self._r
                                  , 'w'      : self._w
                                  , 'reset'  : self._reset
                                  , 'xcks'   : xcks
                                  , 'xckm'   : xckm
                                  , 'nr'     : nr
                                  , 'nw'     : nw
                                  , 'xreset' : xreset
                                  , 'nreset' : nreset
                                  , 'vdd'    : self._vdd
                                  , 'vss'    : self._vss
                                  }
                          )
      
      # Deuxieme ligne
      self.Inspwinc = Inst ( 'rf_fifo_inc'
                           , 'inspwinc'
                           , map = { 'ckm'    : ckm
                                   , 'nreset' : nreset
                                   , 'nval'   : nwval
                                   , 'inc'    : pwinc
                                   , 'vdd'    : self._vdd
                                   , 'vss'    : self._vss
                                   }
                           )
      
      for j in range ( half ) :
        self.Pw[nword-1-j] = Inst ( 'rf_fifo_ptreset'
                                  , 'pw%d' % ( nword - 1 - j )
                                  , map = { 'nop'   : pwnop
                                          , 'inc'   : pwinc
                                          , 'cks'   : xcks
                                          , 'reset' : xreset
                                          , 'ptm1'  : self._selw[nword - 2 - j]
                                          , 'pt'    : self._selw[nword - 1 - j]
                                          , 'vdd'   : self._vdd
                                          , 'vss'   : self._vss
                                          }
                                  )
      
      # Troisieme ligne
      self.InsWok = Inst ( 'rf_fifo_ok'
                         , 'inswok'
                         , map = { 'rw'      : self._w
                                 , 'ripple'  : orfull[0]
                                 , 'nrw'     : nr
                                 , 'prev'    : full
                                 , 'nextval' : fullnext
                                 , 'ok'      : self._wok
                                 , 'vdd'     : self._vdd
                                 , 'vss'     : self._vss
                                 }
                         )

      self.InsOrFull = {}
      
      for j in range ( half - 1 ) :
        self.InsOrFull[j] = Inst ( 'rf_fifo_orand5'
                                 , 'insorfull%d' % j
                                 , map = { 'a0'        : self._selw[j]
                                         , 'b0'        : self._selr[j + 1]
                                         , 'a1'        : self._selw[nword - 1 - j]
                                         , 'b1'        : self._selr[( nword - j ) % nword]
                                         , 'ripplein'  : orfull[j + 1]
                                         , 'rippleout' : orfull[j]
                                         , 'vdd'       : self._vdd
                                         , 'vss'       : self._vss
                                         }
                                 )
  
      self.InsOrFull[half-1] = Inst ( 'rf_fifo_orand4'
                                    , 'insorfull%d' % ( half - 1 )
      	                            , map = { 'a0'        : self._selw[half - 1]
      	                                    , 'b0'        : self._selr[half]
                                            , 'a1'        : self._selw[half]
      	                                    , 'b1'        : self._selr[half + 1]
      	                                    , 'rippleout' : orfull[half - 1]
                                            , 'vdd'       : self._vdd
                                            , 'vss'       : self._vss
                                            }
                                    )
      
      self.InsEmpty = Inst ( 'rf_fifo_empty'
                           , 'insempty'
                           , map = { 'ckm'       : xckm
                                   , 'nreset'    : nreset
                                   , 'emptynext' : emptynext
                                   , 'cks'       : xcks
                                   , 'empty'     : empty
                                   , 'vdd'       : self._vdd
                                   , 'vss'       : self._vss
                                   }
                           )
      
      # Quatrieme ligne
      self.InsFull = Inst ( 'rf_fifo_full'
                          , 'insfull'
                          , map = { 'ckm'      : xckm
                                  , 'reset'    : xreset
                                  , 'fullnext' : fullnext
                                  , 'cks'      : xcks
                                  , 'full'     : full
                                  , 'vdd'      : self._vdd
                                  , 'vss'      : self._vss
                                  }
                          )
      
      self.InsOrEmpty = {}
      
      self.InsOrEmpty[0] = Inst ( 'rf_fifo_orand4'
                                , 'insorempty0'
      	                        , map = { 'a0'        : self._selr[0]
      	                                , 'b0'        : self._selw[1]
      	                                , 'a1'        : self._selr[nword - 1]
      	                                , 'b1'        : self._selw[0]
                                        , 'rippleout' : orempty[0]
                                        , 'vdd'       : self._vdd
                                        , 'vss'       : self._vss
                                        }
                                 )
      
      for j in range ( 1, half ) :
        self.InsOrEmpty[j] = Inst ( 'rf_fifo_orand5'
                                  , 'insorempty%d' % j
                                  , map = { 'a0'        : self._selr[j]
                                          , 'b0'        : self._selw[j + 1]
                                          , 'a1'        : self._selr[nword - 1 - j]
                                          , 'b1'        : self._selw[nword - j]
                                          , 'ripplein'  : orempty[j - 1]
                                          , 'rippleout' : orempty[j]
                                          , 'vdd'       : self._vdd
                                          , 'vss'       : self._vss
                                          }
                                  )
  
      self.InsRok = Inst ( 'rf_fifo_ok'
                         , 'insrok'
                         , map = { 'rw'      : self._r
                                 , 'ripple'  : orempty[half - 1]
      	                         , 'nrw'     : nw
      	                         , 'prev'    : empty
      	                         , 'nextval' : emptynext
      	                         , 'ok'      : self._rok
                                 , 'vdd'     : self._vdd
                                 , 'vss'     : self._vss
                                 }
                         )
      
      # Cinquieme ligne
      self.InsClock = Inst ( 'rf_fifo_clock'
                           , 'insclock'
                           , map = { 'ck'   : self._ck
                                   , 'wok'  : self._wok
                                   , 'cks'  : cks
                                   , 'ckm'  : ckm
                                   , 'ckok' : self._ckok
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
                                   }
                           )
      
      self.Pr = {}
      
      self.Pr[0] = Inst ( 'rf_fifo_ptset'
                        , 'pr0'
      	                , map = { 'nop'    : prnop
      	                        , 'inc'    : princ
      	                        , 'cks'    : xcks
      	                        , 'nreset' : nreset
      	                        , 'ptm1'   : self._selr[nword - 1]
      	                        , 'pt'     : self._selr[0]
                                , 'vdd'    : self._vdd
                                , 'vss'    : self._vss
                                }
                        )
      
      for j in range ( 1, half ) :
        self.Pr[j] = Inst ( 'rf_fifo_ptreset'
                          , 'pr%d' % j
                          , map = { 'nop'   : prnop
                                  , 'inc'   : princ
                                  , 'cks'   : xcks
                                  , 'reset' : xreset
                                  , 'ptm1'  : self._selr[j - 1]
                                  , 'pt'    : self._selr[j]
                                  , 'vdd'   : self._vdd
                                  , 'vss'   : self._vss
                                  }
                          )
  
      self.InsPrNop = Inst ( 'rf_fifo_nop'
                           , 'insprnop'
      	                   , map = { 'ckm'    : ckm
      	                           , 'nreset' : nreset
      	                           , 'rw'     : self._r
      	                           , 'rwok'   : self._rok
      	                           , 'nval'   : nrval
      	                           , 'nop'    : prnop
                                   , 'vdd'    : self._vdd
                                   , 'vss'    : self._vss
                                   }
                           )
      	
      # Sixieme ligne
      self.Pr[nword-1] = Inst ( 'rf_fifo_ptreset'
                              , 'pr%d' % ( nword - 1 )
                              , map = { 'nop'   : prnop
                                      , 'inc'   : princ
                                      , 'cks'   : xcks
                                      , 'reset' : xreset
                                      , 'ptm1'  : self._selr[nword - 2]
                                      , 'pt'    : self._selr[nword - 1]
                                      , 'vdd'   : self._vdd
                                      , 'vss'   : self._vss
                                      }
                              )
      
      for j in range ( 1, half ) :
        self.Pr[nword-1-j] = Inst ( 'rf_fifo_ptreset'
                                  , 'pr%d' % ( nword - 1 - j )
                                  , map = { 'nop'   : prnop
                                          , 'inc'   : princ
                                          , 'cks'   : xcks
                                          , 'reset' : xreset
                                          , 'ptm1'  : self._selr[nword - 2 - j]
                                          , 'pt'    : self._selr[nword - 1 - j]
                                          , 'vdd'   : self._vdd
                                          , 'vss'   : self._vss
                                          }
                                  )
        
      self.InsPrinc= Inst ( 'rf_fifo_inc'
                          , 'insprinc'
      	                  , map = { 'ckm'    : ckm
      	                          , 'nreset' : nreset
      	                          , 'nval'   : nrval
      	                          , 'inc'    : princ
                                  , 'vdd'    : self._vdd
                                  , 'vss'    : self._vss
                                  }
                          )
# end if type == 4

    
  ## Layout ##
  ############
  def Layout ( self, nbit, nword, type ) :
    
    hbuf = 2   # hauteur des buffers (2 ou 4 slices)  : non utilise

   #HCELL = 50
    HCELL = DbU.toLambda( self.In[0]._hur_masterCell.getAbutmentBox().getHeight() )
      
    ## Placement des lignes de bit en dessous des buffers ##
    bottom       = 0
        
    sym1 = NOSYM
    sym2 = SYM_X
    
    for i in range ( nbit ) :
      if i % 2 :  # lignes paires	
        sym1 = NOSYM
        sym2 = SYM_X
      else :      #  lignes impaires
        sym1 = SYM_Y
        sym2 = SYMXY  
  
      Place ( self.In[nbit - 1 - i], sym1, XY ( 0, bottom + ( nbit - 1 - i ) * HCELL ) )
      
      for j in range ( 0, nword, 2 ) :
        PlaceRight ( self.Bit[nbit - 1 - i][j],     sym1 )
        PlaceRight ( self.Bit[nbit - 1 - i][j + 1], sym2 )       
  
      PlaceRight ( self.Out[nbit - 1 - i] , sym1 )
    
  ## Placement de la ligne contenant les buffers ##
    bottom = nbit * HCELL
    
    Place ( self.InBuf, NOSYM, XY ( 0, bottom ) )
    
    for j in range ( 0, nword, 2 ) :	
      PlaceRight ( self.Buf[j],     NOSYM )
      PlaceRight ( self.Buf[j + 1], SYM_X )
  
    PlaceRight ( self.OutBuf, NOSYM )
    
    ## Placement des lignes de bit au dessus des buffers ##
    bottom = ( nbit + hbuf ) * HCELL
        
    for i in range  ( nbitu ) :
      if i % 2  : # lignes impaires
        sym1 = SYM_Y
        sym2 = SYMXY
      else      : # lignes paires
        sym1 = NOSYM
        sym2 = SYM_X
     	
      Place ( self.In[nbitu - 1 - i], sym1, XY ( 0, bottom + i * HCELL ) )
      
      for j in range ( 0, nword, 2 ) :
        PlaceRight ( self.Bit[nbitu - 1 - i][j],     sym1 )
        PlaceRight ( self.Bit[nbitu - 1 - i][j + 1], sym2 )
  
      PlaceRight ( self.Out[nbitu - 1 - i], sym1 )
  
    ## Placement des cellules du decodeur ##
    if type == 2 or type == 3 :  
      half = nword >> 1
      
      # Premiere ligne decodeur ecriture
      bottom = ( nbit + nbitu + hbuf + 2 ) * HCELL
      
      Place ( self.Adw0, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half ) :
        PlaceRight ( self.SelW[2 * j],     SYM_X )
        PlaceRight ( self.SelW[2 * j + 1], NOSYM )
        
      PlaceRight ( self.Wen, NOSYM )
      
      # Deuxieme ligne decodeur ecriture
      bottom = ( nbit + nbitu + hbuf + 3 ) * HCELL
      
      if adrange == 5 :    # adresse sur 5 bits
        Place ( self.Adw12, SYM_Y, XY ( 0, bottom ) )
        
        for j in range ( half ) : PlaceRight ( self.Nand4W[j], SYM_Y )
          
        PlaceRight ( self.Adw34, SYMXY )
    
      if adrange == 4 :    # adresse sur 4 bits
        Place ( self.Adw12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand3W[j], SYM_Y )
        PlaceRight ( self.Adw3, SYMXY )
    
      if adrange == 3 :    # adresse sur 3 bits
        Place ( self.Adw1, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand2W[j], SYM_Y )
        PlaceRight ( self.Adw2, SYMXY )
    
      # Premiere ligne decodeur lecture
      bottom = ( nbit + nbitu + hbuf ) * HCELL
      
      Place ( self.Adr0, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half ) :
        if j == 0 : PlaceRight ( self.SelR[2 * j], NOSYM )
        else      : PlaceRight ( self.SelR[2 * j], NOSYM )
        PlaceRight ( self.SelR[2 * j + 1], SYM_X )
    
      PlaceRight ( self.Ren, NOSYM )
      
      # Deuxieme ligne decodeur lecture
      bottom = ( nbit + nbitu + hbuf + 1 ) * HCELL
      
      if adrange == 5 :    # adresse sur 5 bits
        Place ( self.Adr12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand4R[j], SYM_Y )
        PlaceRight ( self.Adr34, SYMXY )
      
      if adrange == 4 :    # adresse sur 4 bits
        Place ( self.Adr12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand3R[j], SYM_Y )
        PlaceRight ( self.Adr3, SYMXY )
      
      if adrange == 3 :    # adresse sur 3 bits
        Place ( self.Adr1, SYM_Y, XY ( 0, bottom ) )    
        for j in range ( half ) : PlaceRight ( self.Nand2R[j], SYM_Y )
        PlaceRight ( self.Adr2, SYMXY )
        
    ## Placement des cellules du controleur FIFO  ##
    if type == 4 :
      half = nword >> 1
      
      # Premiere ligne
      bottom = ( nbit + nbitu + hbuf ) * HCELL
      Place      ( self.Inspwnop, SYM_X, XY ( 0, bottom ) )
      PlaceRight ( self.Pw[0], NOSYM )
      
      for j in range ( 1, half, 1 ) : PlaceRight ( self.Pw[j], NOSYM )
          
      PlaceRight ( self.FifoBuf, SYM_X )
      
      # Deuxieme ligne
      bottom = ( nbit + nbitu + hbuf + 1 ) * HCELL
      
      Place ( self.Inspwinc, SYMXY, XY ( 0, bottom ) )
      
      for j in range ( half ) : PlaceRight ( self.Pw[nword - 1 - j], SYMXY )
      
      # Troisieme ligne
      bottom = ( nbit  + nbitu + hbuf + 2 ) * HCELL
      
      Place ( self.InsWok, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half - 1 ) : PlaceRight ( self.InsOrFull[j], NOSYM )
  
      PlaceRight ( self.InsOrFull[half - 1], NOSYM )
      PlaceRight ( self.InsEmpty, SYM_X )
      
      # Quatrieme ligne
      bottom = ( nbit + nbitu + hbuf + 3 ) * HCELL
      
      Place      ( self.InsFull, SYM_Y, XY ( 0, bottom ) )
      PlaceRight ( self.InsOrEmpty[0], SYMXY )
      
      for j in range ( 1, half ) : PlaceRight ( self.InsOrEmpty[j], SYMXY )
  
      PlaceRight ( self.InsRok, SYMXY )
      
      # Cinquieme ligne
      bottom = ( nbit + nbitu + hbuf + 4 ) * HCELL
      
      Place      ( self.InsClock, NOSYM, XY ( 0, bottom ) )
      PlaceRight ( self.Pr[0], NOSYM )
      
      for j in range ( 1, half ) : PlaceRight ( self.Pr[j], NOSYM )
  
      PlaceRight ( self.InsPrNop, NOSYM )
      	
      # Sixieme ligne
      bottom = ( nbit + nbitu + hbuf + 5 ) * HCELL
      
      Place ( self.Pr[nword - 1] , SYMXY, XY ( HCELL, bottom ) )
      
      for j in range ( 1, half ) : PlaceRight ( self.Pr[nword - 1 - j], SYM_Y )
  
      PlaceRight ( self.InsPrinc, SYM_Y )

    
  ## VBE ##
  #########
  def Vbe ( self ) :
    print( "rf1 behavior not implemented" )



####################
##### DpgenRf1 #####
####################
class DpgenRf1 ( top_rf1 ) :
    
  def Interface ( self ) :
    top_rf1.Interface ( self, self._param['nbit'], self._param['nword'], 0 )

  def Netlist ( self ) :
    top_rf1.Netlist   ( self, self._param['nbit'], self._param['nword'], 0 )

  def Layout ( self ) :
    top_rf1.Layout    ( self, self._param['nbit'], self._param['nword'], 0 )    
 

######################
##### DpgenRf1r0 #####
######################
class DpgenRf1r0 ( top_rf1 ) :
    
  def Interface ( self ) :
    top_rf1.Interface ( self, self._param['nbit'], self._param['nword'], 1 )

  def Netlist ( self ) :
    top_rf1.Netlist   ( self, self._param['nbit'], self._param['nword'], 1 )

  def Layout ( self ) :
    top_rf1.Layout    ( self, self._param['nbit'], self._param['nword'], 1 ) 

    
#####################
##### DpgenRf1d #####
#####################
class DpgenRf1d ( top_rf1 ) :
    
  def Interface ( self ) :
    top_rf1.Interface ( self, self._param['nbit'], self._param['nword'], 2 )

  def Netlist ( self ) :
    top_rf1.Netlist   ( self, self._param['nbit'], self._param['nword'], 2 )

  def Layout ( self ) :
    top_rf1.Layout    ( self, self._param['nbit'], self._param['nword'], 2 ) 
 

#######################
##### DpgenRf1dr0 #####
#######################
class DpgenRf1dr0 ( top_rf1 ) :
    
  def Interface ( self ) :
    top_rf1.Interface ( self, self._param['nbit'], self._param['nword'], 3 )

  def Netlist ( self ) :
    top_rf1.Netlist   ( self, self._param['nbit'], self._param['nword'], 3 )

  def Layout ( self ) :
    top_rf1.Layout    ( self, self._param['nbit'], self._param['nword'], 3 )
 

#####################
##### DpgenFifo #####
#####################
class DpgenFifo ( top_rf1 ) :
    
  def Interface ( self ) :
    top_rf1.Interface ( self, self._param['nbit'], self._param['nword'], 4 )

  def Netlist ( self ) :
    top_rf1.Netlist   ( self, self._param['nbit'], self._param['nword'], 4 )

  def Layout ( self ) :
    top_rf1.Layout    ( self, self._param['nbit'], self._param['nword'], 4 )

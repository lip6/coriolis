
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
# |  Py Module   :       "./dpgen_RF2.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

# Banc de registre triple acces : deux lectures, une ecriture
# Le programme genere une net-list et un placement optimise.
#
# La largeur est egale a (nmot * 35) + 150
# La hauteur est egale a (nbit + 8) * 50 
#
# Pour la partie "donnees", on utilise les cellules suivantes :
#   colonne d'entree 
# - rf2_inmux_mem          
# - rf2_inmux_buf        
#   colonne centrale
# - rf2_mid_mem          
# - rf2_mid_mem_r0          
# - rf2_mid_buf      
#   colonne de sortie
# - rf2_out_mem          
# - rf2_out_buf        
#
# Pour le decodeur, on utilise les cellules suivantes :
# - rf2_dec_bufad0
# - rf2_dec_bufad1_l
# - rf2_dec_bufad2_l
# - rf2_dec_bufad1_r
# - rf2_dec_bufad2_r
# - rf2_dec_nbuf
# - rf2_dec_nor3
# - rf2_dec_nao3
# - rf2_dec_nand2
# - rf2_dec_nand3
# - rf2_dec_nand4


#----------------------------------------------------------------
#	DEBUT PROGRAMME  PRINCIPAL			   
#----------------------------------------------------------------
# aFunction : le type  :
#       DPGEN_RF2            (sans decodeur)
#       DPGEN_RF2R0          (sans decodeur, avec R0 colle a 0)
#       DPGEN_RF2D           (avec decodeur)
#       DPGEN_RF2DR0         (avec decodeur, avec R0 colle a 0)
#
# aAL : liste des parametres :
#          Model_name  :  nom de modele.
#          flags       :  indexation des connecteurs.
#          nbit        :  nombre de bits (largeur du bus).
#          nmot        :  nombre de mots. 
# - le nombre de mots doit etre pair, 6 =< nmot =< 32
# - le nombre de bits doit etre pair, 2 =< nbit =< 64
#----------------------------------------------------------------



#################################
##### Classe mere : top_rf2 #####
#################################
class top_rf2 ( Model ) :
    
  ## Interface ##
  ###############
  def Interface ( self, nbit, nword, type  ) :
    global adrange
    
    if nword % 2 == 1 or nword > 32 or nword < 4 :
      print( "Error in DpgenRf2 : the word number must be even, larger than 4 and no larger than 32\n" )
      sys.exit ( 1 )
    
    if nword == 4 and ( type == 2 or type == 3 ) :
      print( "Error in DpgenRf2 : the word number must be larger than 4\n" )
      sys.exit ( 1 )
    
    if nbit % 2 == 1 or nbit > 64 or nbit < 2 :
      print( "Error in DpgenRf2 : the bit number must be even and no larger than 64\n" )
      sys.exit ( 1 )
    
    adrange = 2
    if nword > 4  : adrange = 3
    if nword > 8  : adrange = 4
    if nword > 16 : adrange = 5
    
    if type < 2 :  # c'est un banc de registres sans decodeur
      self._ck       = SignalIn  (       "ck",     1 )
      self._sel      = SignalIn  (      "sel",     1 )
    
      self._sela     = SignalIn  (     "sela", nword )
      self._selb     = SignalIn  (     "selb", nword )
      self._selw     = SignalIn  (     "selw", nword )
      self._datain0  = SignalIn  (  "datain0", nbit  )
      self._datain1  = SignalIn  (  "datain1", nbit  )
  
      self._dataouta = SignalOut ( "dataouta", nbit  )
      self._dataoutb = SignalOut ( "dataoutb", nbit  )
    
    else :   # c'est un banc de registres avec decodeur  
      self._ck       = SignalIn  (       "ck",       1 )
      self._sel      = SignalIn  (      "sel",       1 )
      self._wen      = SignalIn  (      "wen",       1 )
      self._rena     = SignalIn  (     "rena",       1 )
      self._renb     = SignalIn  (     "renb",       1 )
    
      self._adra     = SignalIn  (     "adra", adrange )
      self._adrb     = SignalIn  (     "adrb", adrange )
      self._adw      = SignalIn  (     "adw" , adrange )
   
      self._datain0  = SignalIn  (  "datain0",    nbit )
      self._datain1  = SignalIn  (  "datain1",    nbit )
  
      self._dataouta = SignalOut ( "dataouta",    nbit )
      self._dataoutb = SignalOut ( "dataoutb",    nbit )

      self._selw     = Signal    (     "selw",   nword )
    
    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )
     

  ## Netlist ##
  #############
  def Netlist ( self, nbit, nword, type ) :
    global adrange

    # Signaux
    sel0    = Signal (   "sel0",    1 )
    sel1    = Signal (   "sel1",    1 )
    xcks    = Signal (   "xcks",    1 )
    nck     = Signal (    "nck",    1 )
    nwen    = Signal (   "nwen",    1 )
    nrena   = Signal (  "nrena",    1 )
    nrenb   = Signal (  "nrenb",    1 )
            
    adw0x   = Signal (  "adw0x",    1 )
    nadw0x  = Signal ( "nadw0x",    1 )
    adw1x   = Signal (  "adw1x",    1 )
    nadw1x  = Signal ( "nadw1x",    1 )
    adw2x   = Signal (  "adw2x",    1 )
    nadw2x  = Signal ( "nadw2x",    1 )
    adw3x   = Signal (  "adw3x",    1 )
    nadw3x  = Signal ( "nadw3x",    1 )
    adw4x   = Signal (  "adw4x",    1 )
    nadw4x  = Signal ( "nadw4x",    1 )

    adra0x  = Signal (  "adra0x",   1 )
    nadra0x = Signal ( "nadra0x",   1 )
    adra1x  = Signal (  "adra1x",   1 )
    nadra1x = Signal ( "nadra1x",   1 )
    adra2x  = Signal (  "adra2x",   1 )
    nadra2x = Signal ( "nadra2x",   1 )
    adra3x  = Signal (  "adra3x",   1 )
    nadra3x = Signal ( "nadra3x",   1 )
    adra4x  = Signal (  "adra4x",   1 )
    nadra4x = Signal ( "nadra4x",   1 )

    adrb0x  = Signal (  "adrb0x",   1 )
    nadrb0x = Signal ( "nadrb0x",   1 )
    adrb1x  = Signal (  "adrb1x",   1 )
    nadrb1x = Signal ( "nadrb1x",   1 )
    adrb2x  = Signal (  "adrb2x",   1 )
    nadrb2x = Signal ( "nadrb2x",   1 )
    adrb3x  = Signal (  "adrb3x",   1 )
    nadrb3x = Signal ( "nadrb3x",   1 )
    adrb4x  = Signal (  "adrb4x",   1 )
    nadrb4x = Signal ( "nadrb4x",   1 )

    dinx    = Signal (  "dinx",  nbit )
    busa    = Signal (  "busa",  nbit )
    busb    = Signal (  "busb",  nbit )
    
    if type >= 2 :    
      self._sela   = Signal ( "sela", nword )
      self._selb   = Signal ( "selb", nword )

    write = []
    reada = []
    readb = []
    for i in range ( nword ) : 
      write += [Signal ( "write%d" % i, 1 )]
      reada += [Signal ( "reada%d" % i, 1 )]
      readb += [Signal ( "readb%d" % i, 1 )]

    nandw  = []
    nandra = []
    nandrb = []
    for i in range ( nword >> 1 ) :
      nandw  += [Signal ( "nandw%d"  % i, 1 )]
      nandra += [Signal ( "nandra%d" % i, 1 )]
      nandrb += [Signal ( "nandrb%d" % i, 1 )]
      
      
    # placement des lignes de bit en dessous des buffers
    self.In  = {}
    self.Bit = {}
    self.Out = {}
      
    for i in range ( nbit ) :
      self.In[nbit-1-i] = Inst ( 'rf2_inmux_mem'
                               , 'in%d' % ( nbit - 1 - i )
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
        if j == 0 and ( type == 1 or type == 3 ) :
          self.BitBis[j] = Inst ( 'rf2_mid_mem_r0'
                                , 'bit%d_%d' % ( nbit - 1 - i , j )
      	                        , map = { 'dinx'  : dinx[nbit - 1 - i]
      	                                , 'write' : write[j]
      	                                , 'reada' : reada[j]
      	                                , 'readb' : readb[j]
      	                                , 'busa'  : busa[nbit - 1 - i]
      	                                , 'busb'  : busb[nbit - 1 - i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )
        else :
          self.BitBis[j] = Inst ( 'rf2_mid_mem'
                                , 'bit%d_%d' % ( nbit - 1 - i, j )
      	                        , map = { 'dinx'  : dinx[nbit - 1 - i]
      	                                , 'write' : write[j]
      	                                , 'reada' : reada[j]
      	                                , 'readb' : readb[j]
      	                                , 'busa'  : busa[nbit - 1 - i]
      	                                , 'busb'  : busb[nbit - 1 - i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )
          
        self.BitBis[j+1] = Inst ( 'rf2_mid_mem'
                                , 'bit%d_%d' % ( nbit - 1 - i, j + 1 )
      	                        , map = { 'dinx'  : dinx[nbit - 1 - i]
      	                                , 'write' : write[j + 1]
      	                                , 'reada' : reada[j + 1]
      	                                , 'readb' : readb[j + 1]
      	                                , 'busa'  : busa[nbit - 1 - i]
      	                                , 'busb'  : busb[nbit - 1 - i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )

      self.Bit[nbit-1-i] = self.BitBis
  
      self.Out[nbit-1-i] = Inst ( 'rf2_out_mem'
                                , 'out%d' % ( nbit - 1 - i )
      	                        , map = { 'busa'     : busa[nbit - 1 - i]
      	                                , 'busb'     : busb[nbit - 1 - i]
      	                                , 'xcks'     : xcks
      	                                , 'dataouta' : self._dataouta[nbit - 1 - i]
      	                                , 'dataoutb' : self._dataoutb[nbit - 1 - i]
                                        , 'vdd'      : self._vdd
                                        , 'vss'      : self._vss
                                        }
                                )
    
    # placement de la ligne contenant les buffers
    self.InBuf = Inst ( 'rf2_inmux_buf'
                      , 'inbuf'
    	              , map = { 'sel'  : self._sel
    	                      , 'sel0' : sel0
    	                      , 'sel1' : sel1
                              , 'vdd'  : self._vdd
                              , 'vss'  : self._vss
                              }
                      )

    self.Buf = {}
   
    for j in range ( 0, nword, 2 ) :	
      self.Buf[j] = Inst ( 'rf2_mid_buf'
                         , 'buf%d' % j
                         , map = { 'selra' : self._sela[j]
                                 , 'selrb' : self._selb[j]
                                 , 'selw'  : self._selw[j]
                                 , 'nck'   : nck
                                 , 'reada' : reada[j]
                                 , 'readb' : readb[j]
                                 , 'write' : write[j]
                                 , 'vdd'   : self._vdd
                                 , 'vss'   : self._vss
                                 }
                         )
  
      self.Buf[j+1] = Inst ( 'rf2_mid_buf'
                           , 'buf%d' % ( j + 1 )
                           , map = { 'selra' : self._sela[j + 1]
                                   , 'selrb' : self._selb[j + 1]
                                   , 'selw'  : self._selw[j + 1]
                                   , 'nck'   : nck
                                   , 'reada' : reada[j + 1]
                                   , 'readb' : readb[j + 1]
                                   , 'write' : write[j + 1]
                                   , 'vdd'   : self._vdd
                                   , 'vss'   : self._vss
                                   }
                           )
    	
    self.OutBuf = Inst ( 'rf2_out_buf'
                       , 'outbuf'
    	               , map = { 'ck'   : self._ck
    	                       , 'nck'  : nck
    	                       , 'xcks' : xcks
                               , 'vdd'  : self._vdd
                               , 'vss'  : self._vss
    	                       }
                       )
    
    # placement des cellules du decodeur ecriture 
    if type == 2 or type == 3 :
      half = nword >> 1
      
      # Premiere ligne decodeur ecriture
      self.Adw0 = Inst ( 'rf2_dec_bufad0'
                       , 'adw0'
      	               , map = { 'i'   : self._adw[0]
      	                       , 'nq'  : nadw0x
      	                       , 'q'   : adw0x
                               , 'vdd' : self._vdd
                               , 'vss' : self._vss
                               }
                       )

      self.SelW = {}
      
      for j in range ( half ) :
        self.SelW[2*j] = Inst ( 'rf2_dec_nor3'
                              , 'selw%d' % ( 2 * j )
                              , map = { 'i0'  : nandw[j]
                                      , 'i1'  : adw0x
                                      , 'i2'  : nwen
                                      , 'nq'  : self._selw[2 * j]
                                      , 'vdd' : self._vdd
                                      , 'vss' : self._vss
                                      }
                              )
        
        self.SelW[2*j+1] = Inst ( 'rf2_dec_nor3'
                                , 'selw%d' % ( 2 * j + 1 )
                                , map = { 'i0'  : nandw[j]
                                        , 'i1'  : nadw0x
                                        , 'i2'  : nwen
                                        , 'nq'  : self._selw[2 * j + 1]
                                        , 'vdd' : self._vdd
                                        , 'vss' : self._vss
                                        }
                                )
  
      self.Wen = Inst ( 'rf2_dec_nbuf'
                      , 'wen'
      	              , map = { 'i'   : self._wen
      	                      , 'nq'  : nwen
                              , 'vdd' : self._vdd
                              , 'vss' : self._vss
                              }
                      )
      
      # Deuxieme ligne decodeur ecriture
      if adrange == 5 :    # adresse sur 5 bits  
        self.Adw12 = Inst ( 'rf2_dec_bufad2_l'
                          , 'adw12'
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
          thisMap = { 'nq'  : nandw[j]
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
          
       	  self.Nand4W[j] = Inst ( 'rf2_dec_nand4'
                                , 'nand4w%d' % j
                                , map = thisMap
                                )
  
        self.Adw34 = Inst ( 'rf2_dec_bufad2_l'
                          , 'adw34'
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
      
      if adrange == 4 :    # adresse sur 4 bits  
        self.Adw12 = Inst ( 'rf2_dec_bufad2_l'
                          , 'adw12'
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
                    , 'vdd'     : self._vdd
                    , 'vss'     : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadw1x
          else                   : thisMap['i0'] = adw1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadw2x
          else                   : thisMap['i1'] = adw2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadw3x
          else                   : thisMap['i2'] = adw3x
          
          self.Nand3W[j] = Inst ( 'rf2_dec_nand3'
                                , 'nand3w%d' % j
                                , map = thisMap
                                )
  
        self.Adw3 = Inst ( 'rf2_dec_bufad1_r'
                         , 'adw3'
        	             , map = { 'i'   : self._adw[3]
        	                     , 'nq'  : nadw3x
        	                     , 'q'   : adw3x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
      
      if adrange == 3 :    # adresse sur 3 bits  
        self.Adw1 = Inst ( 'rf2_dec_bufad1_l'
                         , 'adw1'
        	             , map = { 'i'   : self._adw[1]
        	                     , 'nq'  : nadw1x
        	                     , 'q'   : adw1x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
        	                     }
                         )

        self.Nand2W = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandw[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadw1x
          else                   : thisMap['i0'] = adw1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadw2x
          else                   : thisMap['i1'] = adw2x
          
          self.Nand2W[j] = Inst ( 'rf2_dec_nand2'
                                , 'nand2w%d' % j
                                , map = thisMap
                                )
  
        self.Adw2 = Inst ( 'rf2_dec_bufad1_r'
                         , 'adw2'
        	             , map = { 'i'   : self._adw[2]
        	                     , 'nq'  : nadw2x
        	                     , 'q'   : adw2x
                                 , 'vdd' : self._vdd
                                 , 'vss' : self._vss
                                 }
                         )
    
    # placement des cellules du decodeur lecture A 
    if  type == 2 or type == 3 :  
      half = nword >> 1
      
      # Premiere ligne decodeur lecture A
      self.Adra0 = Inst ( 'rf2_dec_bufad0'
                        , 'adra0'
      	                , map = { 'i'   : self._adra[0]
      	                        , 'nq'  : nadra0x
      	                        , 'q'   : adra0x
                                , 'vdd' : self._vdd
                                , 'vss' : self._vss
      	                        }
                        )

      self.SelRa = {}
      
      for j in range ( half ) :
        if j == 0 :
          self.SelRa[2*j] = Inst ( 'rf2_dec_nao3'
                                 , 'sela%d' % ( 2 * j )
                                 , map = { 'i0'  : nandra[j]
                                         , 'i1'  : adra0x
                                         , 'i2'  : self._rena
                                         , 'nq'  : self._sela[2 * j]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        else :
          self.SelRa[2*j] = Inst ( 'rf2_dec_nor3'
                                 , 'sela%d' % ( 2 * j )
                                 , map = { 'i0'  : nandra[j]
                                         , 'i1'  : adra0x
                                         , 'i2'  : nrena
                                         , 'nq'  : self._sela[2 * j]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
      
        self.SelRa[2*j+1] = Inst ( 'rf2_dec_nor3'
                                 , 'sela%d' % ( 2 * j + 1 )
      	                         , map = { 'i0'  : nandra[j]
      	                                 , 'i1'  : nadra0x
      	                                 , 'i2'  : nrena
      	                                 , 'nq'  : self._sela[2 * j + 1]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      self.RenA = Inst ( 'rf2_dec_nbuf'
                       , 'rena'
      	               , map = { 'i'   : self._rena
      	                       , 'nq'  : nrena
                               , 'vdd' : self._vdd
                               , 'vss' : self._vss
      	                       }
                       )
      
      # Deuxieme ligne decodeur lecture A
      if adrange == 5 :    # adresse sur 5 bits  
        self.Adra12 = Inst ( 'rf2_dec_bufad2_l'
                           , 'adra12'
        	               , map = { 'i0'  : self._adra[1]
        	                       , 'i1'  : self._adra[2]
        	                       , 'nq0' : nadra1x
        	                       , 'q0'  : adra1x
        	                       , 'nq1' : nadra2x
        	                       , 'q1'  : adra2x
                                   , 'vdd' : self._vdd
                                   , 'vss' : self._vss
                                   }
                           )

        self.Nand4Ra = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandra[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadra1x
          else                   : thisMap['i0'] = adra1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadra2x
          else                   : thisMap['i1'] = adra2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadra3x
          else                   : thisMap['i2'] = adra3x
          if ( j >> 3 ) % 2 == 0 : thisMap['i3'] = nadra4x
          else                   : thisMap['i3'] = adra4x
          
       	  self.Nand4Ra[j] = Inst ( 'rf2_dec_nand4'
                                 , 'nand4ra%d' % j
                                 , map = thisMap
                                 )
  
        self.Adra34 = Inst ( 'rf2_dec_bufad2_r'
                           , 'adra34'
        	               , map = { 'i0'  : self._adra[3]
        	                       , 'i1'  : self._adra[4]
        	                       , 'nq0' : nadra3x
        	                       , 'q0'  : adra3x
        	                       , 'nq1' : nadra4x
        	                       , 'q1'  : adra4x
                                   , 'vdd' : self._vdd
                                   , 'vss' : self._vss
        	                       }
                           )
      
      if adrange == 4 :    # adresse sur 4 bits  
        self.Adra12 = Inst ( 'rf2_dec_bufad2_l'
                           , 'adra12'
        	               , map = { 'i0'  : self._adra[1]
        	                       , 'i1'  : self._adra[2]
        	                       , 'nq0' : nadra1x
        	                       , 'q0'  : adra1x
        	                       , 'nq1' : nadra2x
        	                       , 'q1'  : adra2x
                                   , 'vdd' : self._vdd
                                   , 'vss' : self._vss
        	                       }
                           )

        self.Nand3Ra = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandra[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadra1x
          else                   : thisMap['i0'] = adra1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadra2x
          else                   : thisMap['i1'] = adra2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadra3x
          else                   : thisMap['i2'] = adra3x
          
       	  self.Nand3Ra[j] = Inst ( 'rf2_dec_nand3'
                                 , 'nand3ra%d' % j
                                 , map = thisMap
                                 )
  
        self.Adra3 = Inst ( 'rf2_dec_bufad1_r'
                          , 'adra3'
        	              , map = { 'i'   : self._adra[3]
        	                      , 'nq'  : nadra3x
        	                      , 'q'   : adra3x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )
        
      if adrange == 3 :    # adresse sur 3 bits  
        self.Adra1 = Inst ( 'rf2_dec_bufad1_l'
                          , 'adra1'
        	              , map = { 'i'   : self._adra[1]
        	                      , 'nq'  : nadra1x
        	                      , 'q'   : adra1x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )

        self.Nand2Ra = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandra[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadra1x
          else                   : thisMap['i0'] = adra1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadra2x
          else                   : thisMap['i1'] = adra2x
          
          self.Nand2Ra[j] = Inst ( 'rf2_dec_nand2'
                                 , 'nand2ra%d' % j
        	                     , map = thisMap
                                 )
  
        self.Adra2 = Inst ( 'rf2_dec_bufad1_r'
                          , 'adra2'
        	              , map = { 'i'   : self._adra[2]
        	                      , 'nq'  : nadra2x
        	                      , 'q'   : adra2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )
    
    # placement des cellules du decodeur lecture B
    if  type == 2 or type == 3 :
      half = nword >> 1
      
      # Premiere ligne decodeur lecture A
      self.Adrb0 = Inst ( 'rf2_dec_bufad0'
                        , 'adrb0'
      	                , map = { 'i'   : self._adrb[0]
      	                        , 'nq'  : nadrb0x
      	                        , 'q'   : adrb0x
                                , 'vdd' : self._vdd
                                , 'vss' : self._vss
      	                        }
                        )

      self.SelRb = {}
      
      for j in range ( half ) :
        if j == 0 :
          self.SelRb[2*j] = Inst ( 'rf2_dec_nao3'
                                 , 'selb%d' % ( 2 * j )
                                 , map = { 'i0'  : nandrb[j]
                                         , 'i1'  : adrb0x
                                         , 'i2'  : self._renb
                                         , 'nq'  : self._selb[2 * j]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        else :
          self.SelRb[2*j] = Inst ( 'rf2_dec_nor3'
                                 , 'selb%d' % ( 2 * j )
                                 , map = { 'i0'  : nandrb[j]
      	                                 , 'i1'  : adrb0x
      	                                 , 'i2'  : nrenb
      	                                 , 'nq'  : self._selb[2 * j]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
      	                                 }
                                 )
        
        self.SelRb[2*j+1] = Inst ( 'rf2_dec_nor3'
                                 , 'selb%d' % ( 2 * j + 1 )
      	                         , map = { 'i0'  : nandrb[j]
      	                                 , 'i1'  : nadrb0x
      	                                 , 'i2'  : nrenb
      	                                 , 'nq'  : self._selb[2 * j + 1]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
      	                                 }
                                 )
  
      self.RenB = Inst ( 'rf2_dec_nbuf'
                       , 'renb'
      	               , map = { 'i'   : self._renb
      	                       , 'nq'  : nrenb
                               , 'vdd' : self._vdd
                               , 'vss' : self._vss
      	                       }
                       )
      
      # Deuxieme ligne decodeur lecture A
      if adrange == 5 :    # adresse sur 5 bits  
        self.Adrb12 = Inst ( 'rf2_dec_bufad2_l'
                           , 'adrb12'
        	               , map = { 'i0'   : self._adrb[1]
        	                       , 'i1'   : self._adrb[2]
        	                       , 'nq0'  : nadrb1x
        	                       , 'q0'   : adrb1x
        	                       , 'nq1'  : nadrb2x
        	                       , 'q1'   : adrb2x
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
        	                       }
                           )

        self.Nand4Rb = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandrb[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }
          if   j        % 2 == 0 : thisMap['i0'] = nadrb1x
          else                   : thisMap['i0'] = adrb1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadrb2x
          else                   : thisMap['i1'] = adrb2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadrb3x
          else                   : thisMap['i2'] = adrb3x
          if ( j >> 3 ) % 2 == 0 : thisMap['i3'] = nadrb4x
          else                   : thisMap['i3'] = adrb4x
          
          self.Nand4Rb[j] = Inst ( 'rf2_dec_nand4'
                                 , 'nand4rb%d' % j
                                 , map = thisMap
                                 )
  
        self.Adrb34 = Inst ( 'rf2_dec_bufad2_r'
                           ,'adrb34'
        	               , map = { 'i0'  : self._adrb[3]
        	                       , 'i1'  : self._adrb[4]
        	                       , 'nq0' : nadrb3x
        	                       , 'q0'  : adrb3x
        	                       , 'nq1' : nadrb4x
        	                       , 'q1'  : adrb4x
                                   , 'vdd' : self._vdd
                                   , 'vss' : self._vss
        	                       }
                           )
      
      if adrange == 4 :    # adresse sur 4 bits  
        self.Adrb12 = Inst ( 'rf2_dec_bufad2_l'
                           , 'adrb12'
        	               , map = { 'i0'   : self._adrb[1]
        	                       , 'i1'   : self._adrb[2]
        	                       , 'nq0'  : nadrb1x
        	                       , 'q0'   : adrb1x
        	                       , 'nq1'  : nadrb2x
        	                       , 'q1'   : adrb2x
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
        	                       }
                           )

        self.Nand3Rb = {}
        
        for j in range ( half ) :
          thisMap = { 'nq'  : nandrb[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadrb1x
          else                   : thisMap['i0'] = adrb1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadrb2x
          else                   : thisMap['i1'] = adrb2x
          if ( j >> 2 ) % 2 == 0 : thisMap['i2'] = nadrb3x
          else                   : thisMap['i2'] = adrb3x
          
          self.Nand3Rb[j] = Inst ( 'rf2_dec_nand3'
                                 , 'nand3rb%d' % j
                                 , map = thisMap
                                 )
  
        self.Adrb3 = Inst ( 'rf2_dec_bufad1_r'
                          , 'adrb3'
        	              , map = { 'i'   : self._adrb[3]
        	                      , 'nq'  : nadrb3x
        	                      , 'q'   : adrb3x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )
      
      if adrange == 3 :    # adresse sur 3 bits  
        self.Adrb1 = Inst ( 'rf2_dec_bufad1_l'
                          , 'adrb1'
        	              , map = { 'i'   : self._adrb[1]
        	                      , 'nq'  : nadrb1x
        	                      , 'q'   : adrb1x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )

        self.Nand2Rb = {}
            
        for j in range ( half ) :
          thisMap = { 'nq'  : nandrb[j]
                    , 'vdd' : self._vdd
                    , 'vss' : self._vss
                    }

          if   j        % 2 == 0 : thisMap['i0'] = nadrb1x
          else                   : thisMap['i0'] = adrb1x
          if ( j >> 1 ) % 2 == 0 : thisMap['i1'] = nadrb2x
          else                   : thisMap['i1'] = adrb2x
          
          self.Nand2Rb[j] = Inst ( 'rf2_dec_nand2'
                                 , 'nand2rb%d' % j
                                 , map = thisMap
                                 )
  
        self.Adrb2 = Inst ( 'rf2_dec_bufad1_r'
                          , 'adrb2'
        	              , map = { 'i'   : self._adrb[2]
        	                      , 'nq'  : nadrb2x
        	                      , 'q'   : adrb2x
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
        	                      }
                          )
  

  ## Layout ##
  ############
  def Layout ( self, nbit, nword, type ) :
    global adrange

   #HCELL = 50
    HCELL = self.In[0]._hur_masterCell.getAbutmentBox().getHeight()

    # placement des lignes de bit en dessous des buffers  
    bottom = 0
    
    sym1 = NOSYM
    sym2 = SYM_X
    for i in range ( nbit ) :
      if i % 2 : # lignes impaires
    	  sym1 = SYM_Y
    	  sym2 = SYMXY
      else     : # lignes paires
    	  sym1 = NOSYM
    	  sym2 = SYM_X
  
      Place ( self.In[i], sym1, XY ( 0, bottom + i * HCELL ) )
      
      for j in range ( 0, nword, 2 ) :
        if j == 0 and ( type == 1 or type == 3 ) : PlaceRight ( self.Bit[i][j], sym1 )
        else                                     : PlaceRight ( self.Bit[i][j], sym1 )
          
        PlaceRight ( self.Bit[i][j + 1], sym2 )
  
      PlaceRight ( self.Out[i], sym1 )
    
    # s_placement de la ligne contenant les buffers
    bottom = nbit * HCELL
    
    Place ( self.InBuf, NOSYM, XY ( 0, bottom ) )
    
    for j in range ( 0, nword, 2 ) :	
      PlaceRight ( self.Buf[j],     NOSYM )
      PlaceRight ( self.Buf[j + 1], SYM_X )
    	
    PlaceRight ( self.OutBuf, NOSYM )
    
    # s_placement des cellules du decodeur ecriture 
    if type == 2 or type == 3 :
      half = nword >> 1
      
      # Premiere ligne decodeur ecriture
      bottom = ( nbit + 2 ) * HCELL
      
      Place ( self.Adw0, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half ) :
        PlaceRight ( self.SelW[2 * j],     SYM_X )
        PlaceRight ( self.SelW[2 * j + 1], NOSYM )
  
      PlaceRight ( self.Wen, NOSYM )
      
      # Deuxieme ligne decodeur ecriture
      bottom = ( nbit + 3 ) * HCELL
      
      if adrange == 5 :    # adresse sur 5 bits  
        Place ( self.Adw12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand4W[j], SYM_Y, FIXED )
        PlaceRight ( self.Adw34, SYMXY )
      
      if adrange == 4 :    # adresse sur 4 bits  
        Place ( self.Adw12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand3W[j], SYM_Y )
        PlaceRight ( self.Adw3, SYMXY )
      
      if adrange == 3 :    # adresse sur 3 bits  
        Place ( self.Adw1, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand2W[j], SYM_Y )
        PlaceRight ( self.Adw2, SYMXY )
    
    # s_placement des cellules du decodeur lecture A 
    if  type == 2 or type == 3 :  
      half = nword >> 1
      
      # Premiere ligne decodeur lecture A
      bottom = ( nbit + 4 ) * HCELL
      
      Place ( self.Adra0, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half ) :
        PlaceRight ( self.SelRa[2 * j],     NOSYM )
        PlaceRight ( self.SelRa[2 * j + 1], SYM_X )
  
      PlaceRight ( self.RenA, NOSYM )
      
      # Deuxieme ligne decodeur lecture A
      bottom = ( nbit + 5 ) * HCELL
      
      if adrange == 5 :    # adresse sur 5 bits  
        Place ( self.Adra12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand4Ra[j], SYM_Y )
        PlaceRight ( self.Adra34, SYMXY )
      
      if adrange == 4 :    # adresse sur 4 bits  
        Place ( self.Adra12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand3Ra[j], SYM_Y )
        PlaceRight ( self.Adra3, SYMXY )
      
      if adrange == 3 :    # adresse sur 3 bits  
        Place ( self.Adra1, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand2Ra[j], SYM_Y )
        PlaceRight ( self.Adra2, SYMXY )
    
    # s_placement des cellules du decodeur lecture B
    if  type == 2 or type == 3 :
      half = nword >> 1
      
      # Premiere ligne decodeur lecture A
      bottom = ( nbit + 6 ) * HCELL
      
      Place ( self.Adrb0, NOSYM, XY ( 0, bottom ) )
      
      for j in range ( half ) :
        PlaceRight ( self.SelRb[2 * j],     NOSYM )
        PlaceRight ( self.SelRb[2 * j + 1], SYM_X )
  
      PlaceRight ( self.RenB, NOSYM )
      
      # Deuxieme ligne decodeur lecture A
      bottom = ( nbit + 7 ) * HCELL
      
      if adrange == 5 :    # adresse sur 5 bits  
        Place ( self.Adrb12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand4Rb[j], SYM_Y )
        PlaceRight ( self.Adrb34, SYMXY )
      
      if adrange == 4 :    # adresse sur 4 bits  
        Place ( self.Adrb12, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand3Rb[j], SYM_Y )
        PlaceRight ( self.Adrb3, SYMXY )
      
      if adrange == 3 :    # adresse sur 3 bits  
        Place ( self.Adrb1, SYM_Y, XY ( 0, bottom ) )
        for j in range ( half ) : PlaceRight ( self.Nand2Rb[j], SYM_Y )
        PlaceRight ( self.Adrb2, SYMXY )
       

        
  ## VBE ##
  #########
  def Vbe ( self ) :
    print( 'RF2 behavior not implemented' )





#####################
##### DPGEN_RF2 #####
#####################
class DpgenRf2 ( top_rf2 ) :
    
  def Interface ( self ) :
    top_rf2.Interface ( self, self._param['nbit'], self._param['nword'], 0 )

  def Netlist ( self ) :
    top_rf2.Netlist   ( self, self._param['nbit'], self._param['nword'], 0 )

  def Layout ( self ) :
    top_rf2.Layout    ( self, self._param['nbit'], self._param['nword'], 0 )    
 

#######################
##### DPGEN_RF2R0 #####
#######################
class DpgenRf2r0 ( top_rf2 ) :
    
  def Interface ( self ) :
    top_rf2.Interface ( self, self._param['nbit'], self._param['nword'], 1 )

  def Netlist ( self ) :
    top_rf2.Netlist   ( self, self._param['nbit'], self._param['nword'], 1 )

  def Layout ( self ) :
    top_rf2.Layout    ( self, self._param['nbit'], self._param['nword'], 1 )  

    
######################
##### DPGEN_RF2D #####
######################
class DpgenRf2d ( top_rf2 ) :
    
  def Interface ( self ) :
    top_rf2.Interface ( self, self._param['nbit'], self._param['nword'], 2 )

  def Netlist ( self ) :
    top_rf2.Netlist   ( self, self._param['nbit'], self._param['nword'], 2 )

  def Layout ( self ) :
    top_rf2.Layout    ( self, self._param['nbit'], self._param['nword'], 2 )  
 

########################
##### DPGEN_RF2DR0 #####
########################
class DpgenRf2dr0 ( top_rf2 ) :
    
  def Interface ( self ) :
    top_rf2.Interface ( self, self._param['nbit'], self._param['nword'], 3 )

  def Netlist ( self ) :
    top_rf2.Netlist   ( self, self._param['nbit'], self._param['nword'], 3 )

  def Layout ( self ) :
    top_rf2.Layout    ( self, self._param['nbit'], self._param['nword'], 3 ) 

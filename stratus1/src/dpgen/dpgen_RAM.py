
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
# |  Py Module   :       "./dpgen_RAM.py"                           |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from Hurricane import DbU
from stratus import *

class DpgenRam ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :
    global adrange

    self._nbit = self._param['nbit']
    self._nmot = self._param['nword']
    
    if self._nbit % 2 == 1 or self._nbit > 64 or self._nbit < 2 :
      print( "Error in DpgenRam : the bit number must be even and no larger than 64\n" )
      sys.exit ( 3 )

    if self._nmot % 8 != 0 or self._nmot > 256 or self._nmot < 32 :
      print( "Error in instanciaton of DpgenRam" )
      print( "The word number (nword = %d) " % self._nmot, "must be multiple of 8, at least 32 and no larger than 256.\n" )
      sys.exit ( 3 )

    if   self._nmot > 128 : adrange = 8
    elif self._nmot >  64 : adrange = 7
    elif self._nmot >  32 : adrange = 6
    else                  : adrange = 5

    # Connecteurs externes
    self._ck      = SignalIn  (      "ck",          1 )
    self._w       = SignalIn  (       "w",          1 )
    self._selram  = SignalIn  (  "selram",          1 )

    self._ad      = SignalIn  (      "ad",    adrange )
    self._datain  = SignalIn  (  "datain", self._nbit )
    
    self._dataout = TriState ( "dataout", self._nbit )
    
    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )

    
  ## Netlist ##
  #############
  def Netlist ( self ) :

    # Signaux
    prech    = Signal (    "prech", 1 )
    prechbis = Signal ( "prechbis", 1 )
    
    ad0x     = Signal (     "ad0x", 1 )
    nad0x    = Signal (    "nad0x", 1 )
    
    nsense   = Signal (   "nsense", 1 )
    sensex   = Signal (   "sensex", 1 )
    nsensex  = Signal (  "nsensex", 1 )
    
    selramx  = Signal (  "selramx", 1 )
    
    nwrite   = Signal (   "nwrite", 1 )
    writex   = Signal (   "writex", 1 )
    
    ndec00   = Signal (   "ndec00", 1 )
    ndec01   = Signal (   "ndec01", 1 )
    ndec10   = Signal (   "ndec10", 1 )
    ndec11   = Signal (   "ndec11", 1 )
    
    nck      = Signal (      "nck", 1 )
    nckx     = Signal (     "nckx", 1 )
    nckxbis  = Signal (  "nckxbis", 1 )
   
    ad3x     = Signal (     "ad3x", 1 )
    nad3x    = Signal (    "nad3x", 1 )
    ad4x     = Signal (     "ad4x", 1 )
    nad4x    = Signal (    "nad4x", 1 )
    if adrange > 5 : 
      ad5x     = Signal (     "ad5x", 1 )
      nad5x    = Signal (    "nad5x", 1 )
    if adrange > 6 : 
      ad6x     = Signal (     "ad6x", 1 )
      nad6x    = Signal (    "nad6x", 1 )
    if adrange > 7 : 
      ad7x     = Signal (     "ad7x", 1 )
      nad7x    = Signal (    "nad7x", 1 )

    bus0   = []
    nbus0  = []
    bus1   = []
    nbus1  = []
    
    sel    = []
    nsel   = []
    selx   = []
    
    ndeca  = []
    ndecb  = []
    for i in range ( self._nbit ) :
      bus0  += [Signal (  "bus0_%d" % i, 1 )]
      nbus0 += [Signal ( "nbus0_%d" % i, 1 )]
      bus1  += [Signal (  "bus1_%d" % i, 1 )]
      nbus1 += [Signal ( "nbus1_%d" % i, 1 )]

    for i in range ( self._nmot ) :      
      sel   += [Signal (    "sel%d" % i, 1 )]
      nsel  += [Signal (   "nsel%d" % i, 1 )]
      selx  += [Signal (   "selx%d" % i, 1 )]
      
      ndeca += [Signal (  "ndeca%d" % i, 1 )]
      ndecb += [Signal (  "ndecb%d" % i, 1 )]


    # Lignes de bit
    self.Prech = {}
    self.Bit   = {}
    self.Sense = {}
    
    bottom = 0
    for i in range ( self._nbit ) :
      self.Prech[i] = Inst ( "ram_prech_data"
                           , "prech%d" % i
                           , map = { 'prech' : prech
                                   , 'bit0'  : bus0[i]
                                   , 'nbit0' : nbus0[i]
                                   , 'bit1'  : bus1[i]
                                   , 'nbit1' : nbus1[i]
                                   , 'vdd'   : self._vdd
                                   , 'vss'   : self._vss
                                   }
                           )

      self.BitBis = {}
      for j in range ( 0, self._nmot, 4 ) :
        
        self.BitBis[j] = Inst ( "ram_mem_data"
                              , "bit_%d_%d" % ( i, j )
                              , map = { 'selxi' : selx[j]
                                      , 'bit0'  : bus0[i]
                                      , 'nbit0' : nbus0[i]
                                      , 'bit1'  : bus1[i]
                                      , 'nbit1' : nbus1[i]
                                      , 'vdd'   : self._vdd
                                      , 'vss'   : self._vss
                                      }
                              )
        
        self.BitBis[j+2] = Inst ( "ram_mem_data"
                                , "bit_%d_%d" % ( i, j + 2 )
                                , map = { 'selxi' : selx[j+2]
                                        , 'bit0'  : bus0[i]
                                        , 'nbit0' : nbus0[i]
                                        , 'bit1'  : bus1[i]
                                        , 'nbit1' : nbus1[i]
                                        , 'vdd'   : self._vdd
                                        , 'vss'   : self._vss
                                        }
                                )

      self.Bit[i] = self.BitBis
      
      self.Sense[i] = Inst ( "ram_sense_data"
                           , "sense%d" % i
                           , map = { 'bit0'    : bus0[i]
                                   , 'nbit0'   : nbus0[i]
                                   , 'bit1'    : bus1[i]
                                   , 'nbit1'   : nbus1[i]
                                   , 'ad0x'    : ad0x
                                   , 'nad0x'   : nad0x
                                   , 'sensex'  : sensex
                                   , 'nsensex' : nsensex
                                   , 'prechx'  : prechbis
                                   , 'writex'  : writex
                                   , 'din'     : self._datain[i]
                                   , 'dout'    : self._dataout[i]
                                   , 'vdd'     : self._vdd
                                   , 'vss'     : self._vss
                                   }
                           )

    # Premiere ligne de la zone des buffers
    self.PrechBuf0 = Inst ( "ram_prech_buf0"
                          , "prechbuf0"
                          , map = { 'i'   : nckx
                                  , 'nq'  : prech
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )
   
    self.Buf0 = {} 
    for j in range ( 0, self._nmot, 4 ) :
      self.Buf0[j] = Inst ( "ram_mem_buf0"
                          , "buf0_%d" % j
                          , map = { 'i'   : nsel[j]
                                  , 'nq'  : selx[j]
                                  , 'vdd' : self._vdd
                                  , 'vss' : self._vss
                                  }
                          )
      
      self.Buf0[j+2] = Inst ( "ram_mem_buf0"
                            , "buf0_%d" % ( j + 2 )
                            , map = { 'i'   : nsel[j+2]
                                    , 'nq'  : selx[j+2]
                                    , 'vdd' : self._vdd
                                    , 'vss' : self._vss
                                    }
                            )

    self.SenseBuf0 = Inst ( "ram_sense_buf0"
                          , "sensebuf0"
                          , map = { 'ad0'     : self._ad[0]
                                  , 'nsense'  : nsense
                                  , 'nckx'    : nckxbis
                                  , 'nwrite'  : nwrite
                                  , 'ad0x'    : ad0x
                                  , 'nad0x'   : nad0x
                                  , 'sensex'  : sensex
                                  , 'nsensex' : nsensex
                                  , 'prech'   : prechbis
                                  , 'writex'  : writex
                                  , 'vdd'     : self._vdd
                                  , 'vss'     : self._vss
                                  }
                          )

    # Deuxieme ligne de la zone des buffers
    self.PrechBuf1 = Inst ( "ram_prech_buf1"
                          , "prechbuf1"
                          , map = { 'nck'  : nck
                                  , 'nckx' : nckx
                                  , 'vdd'  : self._vdd
                                  , 'vss'  : self._vss
                                  }
                          )
   
    self.Buf1 = {} 
    for j in range ( 0, self._nmot, 4 ) :
      self.Buf1[j] = Inst ( "ram_mem_buf1"
                          , "buf1_%d" % j
                          , map = { 'seli'    : sel[j]
                                  , 'nck'     : nck
                                  , 'selramx' : selramx
                                  , 'nseli'   : nsel[j]
                                  , 'vdd'     : self._vdd
                                  , 'vss'     : self._vss
                                  }
                          )
      
      self.Buf1[j+2] = Inst ( "ram_mem_buf1"
                            , "buf1_%d" % ( j + 2 )
                            , map = { 'seli'    : sel[j+2]
                                    , 'nck'     : nck
                                    , 'selramx' : selramx
                                    , 'nseli'   : nsel[j+2]
                                    , 'vdd'     : self._vdd
                                    , 'vss'     : self._vss
                                    }
                            )

    self.SenseBuf1 = Inst ( "ram_sense_buf1"
                          , "sensebuf1"
                          , map = { 'ck'      : self._ck
                                  , 'selram'  : self._selram
                                  , 'w'       : self._w
                                  , 'nck'     : nck
                                  , 'selramx' : selramx
                                  , 'nsense'  : nsense
                                  , 'nwrite'  : nwrite
                                  , 'nckx'    : nckxbis
                                  , 'vdd'     : self._vdd
                                  , 'vss'     : self._vss
                                  }
                          )

    # Premiere ligne de la zone du decodeur
    self.PrechDecB = Inst ( "ram_prech_dec0"
                          ,"prechdecb"
                          , map = { 'vdd'   : self._vdd
                                  , 'vss'   : self._vss                
                                  }
                          )
   
    self.Dec = {} 
    for j in range ( 0, self._nmot, 8 ) :
      self.Dec[j] = Inst ( "ram_mem_deci"
                         , "dec_%d" % j
                         , map = { 'i0'   : ndeca[j]
                                 , 'i1'   : ndecb[j]
                                 , 'i2'   : ndec00
                                 , 'seli' : sel[j]
                                 , 'vdd'  : self._vdd
                                 , 'vss'  : self._vss
                                 }
                         )
      
      self.Dec[j+2] = Inst ( "ram_mem_deci"
                           , "dec_%d" % ( j + 2 )
                           , map = { 'i0'   : ndeca[j]
                                   , 'i1'   : ndecb[j]
                                   , 'i2'   : ndec01
                                   , 'seli' : sel[j+2]
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
                                   }
                           )
      
      self.Dec[j+4] = Inst ( "ram_mem_deci"
                           , "dec_%d" % ( j + 4 )
                           , map = { 'i0'   : ndeca[j]
                                   , 'i1'   : ndecb[j]
                                   , 'i2'   : ndec10
                                   , 'seli' : sel[j+4]
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
                                   }
                           )
      
      self.Dec[j+6] = Inst ( "ram_mem_deci"
                           , "dec_%d" % ( j + 6 )
                           , map = { 'i0'   : ndeca[j]
                                   , 'i1'   : ndecb[j]
                                   , 'i2'   : ndec11
                                   , 'seli' : sel[j+6]
                                   , 'vdd'  : self._vdd
                                   , 'vss'  : self._vss
                                   }
                           )

    self.SenseDecAd12 = Inst ( "ram_sense_decad12"
                             , "sensedecad12"
                             , map = { 'ad1'    : self._ad[1]
                                     , 'ad2'    : self._ad[2]
                                     , 'ndec00' : ndec00
                                     , 'ndec01' : ndec01
                                     , 'ndec10' : ndec10
                                     , 'ndec11' : ndec11
                                       , 'vdd'  : self._vdd
                                       , 'vss'  : self._vss
                                     }
                             )

    # Deuxieme ligne de la zone du decodeur
    if adrange == 8 :   # adresse sur 8 bits  : on decode 5 bits
      self.PrechDecU = Inst ( "ram_prech_dec0"
                            , "prechdecu"
                            , map = { 'vdd'   : self._vdd
                                    , 'vss'   : self._vss
                                    }
                            )
     
      self.MemDec = {} 
      for j in range ( 0, self._nmot, 8 ) :        
        thisMap = { 'ndeca' : ndeca[j]
                  , 'ndecb' : ndecb[j]
                  , 'vdd'   : self._vdd
                  , 'vss'   : self._vss
                  }
          
        if ( j >> 3 ) % 2 == 0 : thisMap['i0'] = nad3x
        else                   : thisMap['i0'] = ad3x
            
        if ( j >> 4 ) % 2 == 0 : thisMap['i1'] = nad4x
        else                   : thisMap['i1'] = ad4x
            
        if ( j >> 5 ) % 2 == 0 : thisMap['i2'] = nad5x
        else                   : thisMap['i2'] = ad5x
            
        if ( j >> 6 ) % 2 == 0 : thisMap['i3'] = nad6x
        else                   : thisMap['i3'] = ad6x
            
        if ( j >> 7 ) % 2 == 0 : thisMap['i4'] = nad7x
        else                   : thisMap['i4'] = ad7x

        self.MemDec[j] = Inst ( "ram_mem_dec5"
                              , "memdec%d" % j
                              , map = thisMap
                              )

      self.SenseDecAd = Inst ( "ram_sense_decad5"
                             , "sensedecad"
                             , map = { 'ad3'   : self._ad[3]
                                     , 'ad4'   : self._ad[4]
                                     , 'ad5'   : self._ad[5]
                                     , 'ad6'   : self._ad[6]
                                     , 'ad7'   : self._ad[7]
                                     , 'ad3x'  : ad3x
                                     , 'nad3x' : nad3x
                                     , 'ad4x'  : ad4x
                                     , 'nad4x' : nad4x
                                     , 'ad5x'  : ad5x
                                     , 'nad5x' : nad5x
                                     , 'ad6x'  : ad6x
                                     , 'nad6x' : nad6x
                                     , 'ad7x'  : ad7x
                                     , 'nad7x' : nad7x
                                     , 'vdd'   : self._vdd
                                     , 'vss'   : self._vss
                                     }
                             )

    if adrange == 7 :    # adresse sur 7 bits : on decode 4 bits
      self.PrechDecU = Inst ( "ram_prech_dec0"
                            , "prechdecu"
                            , map = { 'vdd'   : self._vdd
                                    , 'vss'   : self._vss
                                    }
                            )
     
      self.MemDec = {} 
      for j in range ( 0, self._nmot, 8 ) :
        thisMap = { 'ndeca' : ndeca[j]
                  , 'ndecb' : ndecb[j]
                  , 'vdd'   : self._vdd
                  , 'vss'   : self._vss
                  }
        
        if ( j >> 3 ) % 2 == 0 : thisMap['i0'] = nad3x
        else                   : thisMap['i0'] = ad3x
            
        if ( j >> 4 ) % 2 == 0 : thisMap['i1'] = nad4x
        else                   : thisMap['i1'] = ad4x
        
        if ( j >> 5 ) % 2 == 0 : thisMap['i2'] = nad5x
        else                   : thisMap['i2'] = ad5x
        
        if ( j >> 6 ) % 2 == 0 : thisMap['i3'] = nad6x
        else                   : thisMap['i3'] = ad6x
        
        self.MemDec[j] = Inst ( "ram_mem_dec4"
                              , "memdec%d" % j
                              , map = thisMap
                              )

      self.SenseDecAd = Inst ( "ram_sense_decad4"
                             , "sensedecad"
                             , map = { 'ad3'   : self._ad[3]
                                     , 'ad4'   : self._ad[4]
                                     , 'ad5'   : self._ad[5]
                                     , 'ad6'   : self._ad[6]
                                     , 'ad3x'  : ad3x
                                     , 'nad3x' : nad3x
                                     , 'ad4x'  : ad4x
                                     , 'nad4x' : nad4x
                                     , 'ad5x'  : ad5x
                                     , 'nad5x' : nad5x
                                     , 'ad6x'  : ad6x
                                     , 'nad6x' : nad6x
                                     , 'vdd'   : self._vdd
                                     , 'vss'   : self._vss
                                     }
                             )

    if adrange == 6 :    # adresse sur 6 bits : on decode 3 bits
      self.PrechDecU = Inst ( "ram_prech_dec0"
                            , "prechdecu"
                            , map = { 'vdd'   : self._vdd
                                    , 'vss'   : self._vss
                                    }
                            )
     
      self.MemDec = {} 
      for j in range ( 0, self._nmot, 8 ) :
        thisMap = { 'ndeca' : ndeca[j]
                  , 'ndecb' : ndecb[j]
                  , 'vdd'   : self._vdd
                  , 'vss'   : self._vss
                  }
         
        if ( j >> 3 ) % 2 == 0 : thisMap['i0'] = nad3x
        else                   : thisMap['i0'] = ad3x
            
        if ( j >> 4 ) % 2 == 0 : thisMap['i1'] = nad4x
        else                   : thisMap['i1'] = ad4x
            
        if ( j >> 5 ) % 2 == 0 : thisMap['i2'] = nad5x
        else                   : thisMap['i2'] = ad5x
   
        self.MemDec[j] = Inst ( "ram_mem_dec3"
                              , "memdec%d" % j
                              , map = thisMap
                              )

      self.SenseDecAd = Inst ( "ram_sense_decad3"
                             , "sensedecad"
                             , map = { 'ad3'   : self._ad[3]
                                     , 'ad4'   : self._ad[4]
                                     , 'ad5'   : self._ad[5]
                                     , 'ad3x'  : ad3x
                                     , 'nad3x' : nad3x
                                     , 'ad4x'  : ad4x
                                     , 'nad4x' : nad4x
                                     , 'ad5x'  : ad5x
                                     , 'nad5x' : nad5x
                                     , 'vdd'   : self._vdd
                                     , 'vss'   : self._vss
                                     }
                             )

    if adrange == 5 :    # adresse sur 5 bits : on decode 2 bits
      self.PrechDecU = Inst ( "ram_prech_dec0"
                            , "prechdecu"
                            , map = { 'vdd'   : self._vdd
                                    , 'vss'   : self._vss
                                    }
                            )
     
      self.MemDec = {} 
      for j in range ( 0, self._nmot, 8 ) :
        thisMap = { 'ndeca' : ndeca[j]
                  , 'ndecb' : ndecb[j]
                  , 'vdd'   : self._vdd
                  , 'vss'   : self._vss
                  }
        
        if ( j >> 3 ) % 2 == 0 : thisMap['i0'] = nad3x
        else                   : thisMap['i0'] = ad3x
            
        if ( j >> 4 ) % 2 == 0 : thisMap['i1'] = nad4x
        else                   : thisMap['i1'] = ad4x

        self.MemDec[j] = Inst ( "ram_mem_dec2"
                              , "memdec%d" % j
                              , map = thisMap
                              )

      self.SenseDecAd = Inst ( "ram_sense_decad2"
                             , "sensedecad"
                             , map = { 'ad3'   : self._ad[3]
                                     , 'ad4'   : self._ad[4]
                                     , 'ad3x'  : ad3x
                                     , 'nad3x' : nad3x
                                     , 'ad4x'  : ad4x
                                     , 'nad4x' : nad4x
                                     , 'vdd'   : self._vdd
                                     , 'vss'   : self._vss
                                     }
                             )
       
    
  ########## Layout ##########
  ############################
  def Layout ( self ) :

    #HCELL = 50
    HCELL = DbU.toLambda( self.Prech[0]._hur_masterCell.getAbutmentBox().getHeight() )
  
    # Lignes de bit
    bottom = 0
    
    for i in range ( self._nbit ) :
      if i % 2 : # lignes impaires
        sym1 = SYM_Y
        sym2 = SYMXY
      else :     # lignes paires
        sym1 = NOSYM
        sym2 = SYM_X
        
      Place ( self.Prech[i], sym1, XY ( 0, bottom + i * HCELL ) )
  
      for j in range ( 0, self._nmot, 4 ) :
        PlaceRight ( self.Bit[i][j],   sym2 )
        PlaceRight ( self.Bit[i][j+2], sym1 )
  
      PlaceRight ( self.Sense[i], sym1 )
  
    # Premiere ligne de la zone des buffers
    bottom = self._nbit * HCELL
    
    Place ( self.PrechBuf0, NOSYM, XY ( 0, bottom ) )
    
    for j in range ( 0, self._nmot, 4 ) :
      PlaceRight ( self.Buf0[j],   SYM_X )
      PlaceRight ( self.Buf0[j+2], NOSYM )
  
    PlaceRight ( self.SenseBuf0, NOSYM )
   
    # Deuxieme ligne de la zone des buffers
    bottom = ( self._nbit + 1 ) * HCELL
  
    Place ( self.PrechBuf1, SYM_Y, XY ( 0, bottom ) )
    
    for j in range ( 0, self._nmot, 4 ) :
  	  PlaceRight ( self.Buf1[j],   SYMXY )
  	  PlaceRight ( self.Buf1[j+2], SYM_Y )
  
    PlaceRight ( self.SenseBuf1, SYM_Y )
  
    # Premiere ligne de la zone du decodeur
    bottom = ( self._nbit + 2 ) * HCELL
    
    Place ( self.PrechDecB, NOSYM, XY ( 0, bottom ) )
    
    for j in range ( 0, self._nmot, 8 ) :
  	  PlaceRight ( self.Dec[j],   SYM_X )
  	  PlaceRight ( self.Dec[j+2], NOSYM )
  	  PlaceRight ( self.Dec[j+4], SYM_X )
  	  PlaceRight ( self.Dec[j+6], NOSYM )
  
    PlaceRight ( self.SenseDecAd12, NOSYM )
  
    # Deuxieme ligne de la zone du decodeur
    bottom = ( self._nbit + 3 ) * HCELL
    
    if adrange == 8 :   # adresse sur 8 bits  : on decode 5 bits
      Place ( self.PrechDecU, SYM_Y, XY ( 0, bottom ) )
      for j in range ( 0, self._nmot, 8 ) : PlaceRight ( self.MemDec[j], SYM_Y )
      PlaceRight ( self.SenseDecAd, SYM_Y )
  
    if adrange == 7 :    # adresse sur 7 bits : on decode 4 bits
      Place ( self.PrechDecU, SYM_Y, XY ( 0, bottom ) )
      for j in range ( 0, self._nmot, 8 ) : PlaceRight ( self.MemDec[j], SYM_Y )
      PlaceRight ( self.SenseDecAd, SYM_Y )

    if adrange == 6 :    # adresse sur 6 bits : on decode 3 bits
      Place ( self.PrechDecU, SYM_Y, XY ( 0, bottom ) )
      for j in range ( 0, self._nmot, 8 ) : PlaceRight ( self.MemDec[j], SYM_Y )
      PlaceRight ( self.SenseDecAd, SYM_Y )
  
    if adrange == 5 :    # adresse sur 5 bits : on decode 2 bits
      Place ( self.PrechDecU, SYM_Y, XY ( 0, bottom ) )
      for j in range ( 0, self._nmot, 8 ) : PlaceRight ( self.MemDec[j], SYM_Y )
      PlaceRight ( self.SenseDecAd, SYM_Y )


  ## VBE ##
  #########
  def Vbe () :
    print( "RAM behavior not yet implemented" )

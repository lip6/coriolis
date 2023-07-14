
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
# |  Py Module   :       "./dpgen_ADSB2F.py"                        |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class DpgenAdsb2f ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self ) :
    global add_sub, c30, c31, i0, i1, q, vdd, vss

    self._nbit = self._param['nbit']

    add_sub = SignalIn  ( "add_sub",    1 )
    c30     = SignalOut (     "c30",    1 )
    c31     = SignalOut (     "c31",    1 )
    
    i0      = SignalIn  (      "i0", self._nbit )
    i1      = SignalIn  (      "i1", self._nbit )
    q       = SignalOut (       "q", self._nbit )

    vdd     = VddIn    ( "vdd" )
    vss     = VssIn    ( "vss" )
    
  ## Netlist ##
  #############
  def Netlist ( self ) :

    # Liste des signaux internes
    xorb = Signal ( "xorb", self._nbit )
    c    = Signal (    "c", self._nbit )
    
    pi   = []
    gi   = []
    for i in range ( self._nbit ) :
      pi_bis = []
      gi_bis = []
      for j in range ( self._nbit ) :
        pi_bis += [Signal("pi_%d_%d" % (i, j), 1 )]
        gi_bis += [Signal("gi_%d_%d" % (i, j), 1 )]
      pi += [pi_bis]
      gi += [gi_bis]

    c30.Alias ( c[self._nbit-2] )
    c31.Alias ( c[self._nbit-1] )

    # Ligne des XOR
    self.Xorb = {}
    for i in range ( self._nbit ) :
      self.Xorb[i] =  Inst ( "xr2_x1"
                           , "xorb%d" % i
                           , map = { 'i0'  : add_sub
                                   , 'i1'  : i1[i]
                                   , 'q'   : xorb[i]
                                   , 'vdd' : vdd
                                   , 'vss' : vss
                                   }
                           )

    # Ligne des PIGI
    self.Pigi = {}
    for i in range ( 1, self._nbit ) :
      self.Pigi[i] =  Inst ( "halfadder_x2"
                           , "pigi%d" % i
                           , map = { 'a'    : i0[i]
                                   , 'b'    : xorb[i]
                                   , 'cout' : gi[0][i]
                                   , 'sout' : pi[0][i]
                                   , 'vdd' : vdd
                                   , 'vss' : vss
                                   }
                           )
  
    # Ligne des PG
    self.PgI1 = {}
    self.PgI2 = {}
    
    for n in range ( 1, ilog2(self._nbit) + 1 ) :
        
      self.I1Bis = {}
      self.I2Bis = {}
      
      for ipg in range ( iexp2(n) + iexp2(n-1), self._nbit, iexp2(n) ) :
        inpg = ipg

        for npg in range ( iexp2(n-1) ) :
          if inpg >= self._nbit : break
          
          self.I1Bis[inpg] = Inst ( "a2_x2"
                                  , "pg_i1_%d_%d" % ( n, inpg )
                                  , map = { 'i0'  : pi[n-1][ipg-1]
                                          , 'i1'  : pi[ilog2(npg)][inpg ]
                                          , 'q'   : pi[n][inpg ]
                                          , 'vdd' : vdd
                                          , 'vss' : vss
                                          }
                                  )
          
          self.I2Bis[inpg] = Inst ( "oa22_x2"
                                  , "pg_i2_%d_%d" % ( n, inpg )
                                  , map = { 'i0' : pi[ilog2(npg)][inpg ]
                                          , 'i1' : gi[n-1       ][ipg-1]
                                          , 'i2' : gi[ilog2(npg)][inpg ]
                                          , 'q'  : gi[n         ][inpg ]
                                          , 'vdd' : vdd
                                          , 'vss' : vss
                                          }
                                  )
          
          inpg += 1

      self.PgI1[n] = self.I1Bis
      self.PgI2[n] = self.I2Bis
          
    # Calcul des retenues
    p = 2
    self.R = {}
    for n in range ( 1, ilog2 ( self._nbit ) + 1 ) :
      for isr in range ( iexp2 ( n - 1 ) ) :
        if iexp2 ( n - 1 ) + isr >= self._nbit : break
     
        self.R[p] = Inst ( "oa22_x2"
                         , "r%d" % p
                         , map = { 'i0' : pi[ilog2(isr)][iexp2(n-1) + isr]
                                 , 'i1' : c[iexp2(n-1) - 1]
                                 , 'i2' : gi[ilog2(isr)][iexp2(n-1) + isr]
                                 , 'q'  : c[iexp2(n-1) + isr]
                                 , 'vdd' : vdd
                                 , 'vss' : vss
                                 }
                         )
        p += 1
    
        
    # Calcul des sommes
    self.S = {}

    for i in range ( 1, self._nbit ) :
      self.S[i] = Inst ( "xr2_x1"
                       , "s%d"  % i
                       , map = { 'i0' : pi[0][i]
                               , 'i1' : c[i-1]
                               , 'q'  : q[i]
                               , 'vdd' : vdd
                               , 'vss' : vss
                               }
                       )
    
    # Calcul somme et retenue du LSB
    self.S[0] =  Inst ( "fulladder_x2"
                      , "s0"
                      , map = { 'a1'   : i0[0]
                              , 'a2'   : i0[0]
                              , 'a3'   : i0[0]
                              , 'a4'   : i0[0]
                              , 'b1'   : xorb[0]
                              , 'b2'   : xorb[0]
                              , 'b3'   : xorb[0]
                              , 'b4'   : xorb[0]
                              , 'cin1' : add_sub
                              , 'cin2' : add_sub
                              , 'cin3' : add_sub
                              , 'cout' : c[0]
                              , 'sout' : q[0]
                              , 'vdd' : vdd
                              , 'vss' : vss
                              }
                      )


  ## Layout ##
  ############
  def Layout ( self ) :

    twostage = -1
  
    # Ligne des XOR
    Place ( self.Xorb[0], NOSYM, XY ( 0, 0 ) )
    
    for i in range ( 1, self._nbit ) :
      if i % 2 == 0 : sym = NOSYM
      else          : sym = SYM_Y
      PlaceTop ( self.Xorb[i], sym )
       
    # Ligne des PIGI
    if self._nbit > 1 : Place ( self.Pigi[1], SYM_Y, XY ( DbU.fromLambda(45), DbU.fromLambda(50) ) )
    
    for i in range ( 2, self._nbit ) :
      if i % 2 == 0 : sym = NOSYM
      else          : sym = SYM_Y
      PlaceTop ( self.Pigi[i], sym )
        
    # Ligne des PG
    for n in range ( 1, ilog2(self._nbit) + 1, 2 ) :
      twostage += 1
      pi        = self._nbit - 1
      
      for i in range ( self._nbit - 1, 0, -1 ) :
        if i >= iexp2(n) + iexp2(n-1) and i & ( 1 << (n-1) ) :
        
          if pi % 2 == 0 : sym = NOSYM
          else           : sym = SYM_Y
          
          Place ( self.PgI1[n][i], sym, XY ( DbU.fromLambda(125 + 55 * twostage), DbU.fromLambda(pi * 50) ) )
          Place ( self.PgI2[n][i], sym, XY ( DbU.fromLambda(150 + 55 * twostage), DbU.fromLambda(pi * 50) ) )
            
          pi -= 1
        
        if i >= iexp2(n+1) + iexp2(n) and i & ( 1 << n ) :
        
          if pi % 2 == 0 : sym = NOSYM
          else           : sym = SYM_Y
          Place ( self.PgI1[n+1][i], sym, XY ( DbU.fromLambda(125 + 55 * twostage), DbU.fromLambda(pi * 50) ) )
          Place ( self.PgI2[n+1][i], sym, XY ( DbU.fromLambda(150 + 55 * twostage), DbU.fromLambda(pi * 50) ) )
            
          pi -= 1
    
    # nbit n'est pas une puissance entiere de 2
    if self._nbit % ( iexp2 ( ilog2 ( self._nbit ) - 1 ) ) : twostage += 1
      
    # Ligne retenue
    for n in range ( 2, self._nbit + 1 ) :
      if ( n - 1 ) % 2 == 0 : sym = NOSYM
      else                  : sym = SYM_Y
      Place ( self.R[n], sym, XY ( DbU.fromLambda(125 + 55 * twostage), DbU.fromLambda( (n-1) * 50 ) ) )
    
        
    # Calcul des sommes
    if self._nbit > 1 : Place ( self.S[1], NOSYM, XY ( DbU.fromLambda(155 + 55 * twostage), 0 ) )
    
    for i in range ( 2, self._nbit ) :
      if ( i - 1 ) % 2 == 0 : sym = NOSYM
      else                  : sym = SYM_Y
      PlaceTop ( self.S[i], sym )
    
  
    # Calcul somme et retenue du LSB
    Place ( self.S[0], NOSYM, XY ( DbU.fromLambda(45), 0 ) )
  
  
  ########## VHDL ##########
  ##########################
  def Vbe ( self ) :

    name = self._name

    LV_flags = self._param['flags']
  
    fileName = name + ".vbe"
    
    file = open ( fileName, "w+" )
  
    sBusWide0 = VHDL_BUS_WIDE ( self._nbit - 1, 0, self._nbit, LV_flags )
    sBusWide1 = VHDL_BUS_WIDE ( self._nbit    , 1, self._nbit, LV_flags )
    sBusWide2 = VHDL_BUS_WIDE ( self._nbit    , 0, self._nbit, LV_flags )
  
    ## Entity description ##
    file.write ( "\nENTITY %s IS\n  PORT (\n" % name )
  
    file.write ( "%14s : in    BIT;\n"           % "add_sub" )
    file.write ( "%14s : out   BIT;\n"           % "c31" )
    file.write ( "%14s : out   BIT;\n"           % "c30" )
    file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide0 ) )
    file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i1", sBusWide0 ) )
    file.write ( "%14s : out   BIT_VECTOR %s;\n" % (  "q", sBusWide0 ) )
  
    ## Power supplies terminals ##  
    file.write ( "%14s : in    BIT;\n" % "vdd" )
    file.write ( "%14s : in    BIT\n"  % "vss" )
    
    ## End of entity description ##
    file.write ( "  );\nEND %s;\n\n\n" %  name )
  
    ## Architecture description ##
    file.write ( "ARCHITECTURE VBE OF %s IS\n\n" % name )
  
    ## Signals ##  
    file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n" % ( "carry", sBusWide2 ) )
    file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n" % ( "xor_b", sBusWide0 ) )
  
    ## Behavior ##  
    file.write ( "\nBEGIN\n\n" )
  
    for i in range ( self._nbit - 1, -1, -1 ) :
      file.write ( "  xor_b(%ld) <= i1(%ld) xor add_sub;\n" % ( i, i ) )
      
    file.write ( "\n" )
  
    file.write ( "  carry(0) <= add_sub;\n\n" )
  
    file.write ( "  q <=     i0\n" )
    file.write ( "       xor xor_b\n" )
    file.write ( "       xor carry%s;\n\n"   % sBusWide0 )
  
    file.write ( "  c31 <=  carry(%ld);\n"   %   self._nbit       )
    file.write ( "  c30 <=  carry(%ld);\n\n" % ( self._nbit - 1 ) )
  
    file.write ( "  carry%s <=      (   i0 and xor_b)\n"        % sBusWide1 )
    file.write ( "               or (   i0 and carry%s)\n"      % sBusWide0 )
    file.write ( "               or (xor_b and carry%s);\n\n\n" % sBusWide0 )
  
    ## Assert ##
    file.write ( "  ASSERT (vdd = '1')\n" )
    file.write ( "    REPORT \"Power supply is missing on vdd of Model %s.\"\n" % name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    file.write ( "  ASSERT (vss = '0')\n" )
    file.write ( "    REPORT \"Power supply is missing on vss of Model %s.\"\n" % name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    ## End of Architectural description ##
    file.write ( "END VBE;\n" )
  
    file.close ()

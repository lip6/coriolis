
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
# |  Py Module   :       "./dpgen_Regular.py"                       |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *


# Local constants
MAX_TDRIVE      =     8
ERROR_INDEX     =  4096
SIZE_SNAME      =  1024


# Local variables  (prefix "LV_")
LV_ttDrive = []
LV_ttDrive.append ( [1, 0, 0, 0, 0, 0, 0, 0] )
LV_ttDrive.append ( [1, 2, 4, 8, 0, 0, 0, 0] )
LV_ttDrive.append ( [2, 4, 8, 0, 0, 0, 0, 0] )
LV_ttDrive.append ( [1, 4, 0, 0, 0, 0, 0, 0] )
LV_ttDrive.append ( [2, 4, 0, 0, 0, 0, 0, 0] )
LV_ttDrive.append ( [4, 8, 0, 0, 0, 0, 0, 0] )

LV_tsLeafCell = []
LV_tsLeafCell.append ( "inv_x1"             )
LV_tsLeafCell.append ( "inv_x2"             )
LV_tsLeafCell.append ( "inv_x4"             )
LV_tsLeafCell.append ( "inv_x8"             )
LV_tsLeafCell.append ( "buf_x2"             )
LV_tsLeafCell.append ( "buf_x4"             )
LV_tsLeafCell.append ( "buf_x8"             )
LV_tsLeafCell.append ( "na2_x1"             )
LV_tsLeafCell.append ( "na2_x4"             )
LV_tsLeafCell.append ( "na3_x1"             )
LV_tsLeafCell.append ( "na3_x4"             )
LV_tsLeafCell.append ( "na4_x1"             )
LV_tsLeafCell.append ( "na4_x4"             )
LV_tsLeafCell.append ( "a2_x2"              )
LV_tsLeafCell.append ( "a2_x4"              )
LV_tsLeafCell.append ( "a3_x2"              )
LV_tsLeafCell.append ( "a3_x4"              )
LV_tsLeafCell.append ( "a4_x2"              )
LV_tsLeafCell.append ( "a4_x4"              )
LV_tsLeafCell.append ( "no2_x1"             )
LV_tsLeafCell.append ( "no2_x4"             )
LV_tsLeafCell.append ( "no3_x1"             )
LV_tsLeafCell.append ( "no3_x4"             )
LV_tsLeafCell.append ( "no4_x1"             )
LV_tsLeafCell.append ( "no4_x4"             )
LV_tsLeafCell.append ( "o2_x2"              )
LV_tsLeafCell.append ( "o2_x4"              )
LV_tsLeafCell.append ( "o3_x2"              )
LV_tsLeafCell.append ( "o3_x4"              )
LV_tsLeafCell.append ( "o4_x2"              )
LV_tsLeafCell.append ( "o4_x4"              )
LV_tsLeafCell.append ( "nxr2_x1"            )
LV_tsLeafCell.append ( "nxr2_x4"            )
LV_tsLeafCell.append ( "xr2_x1"             )
LV_tsLeafCell.append ( "xr2_x4"             )
LV_tsLeafCell.append ( "dp_nmux_x1_buf"     )
LV_tsLeafCell.append ( "dp_nmux_x1"         )
LV_tsLeafCell.append ( "dp_mux_x2_buf"      )
LV_tsLeafCell.append ( "dp_mux_x2"          )
LV_tsLeafCell.append ( "dp_mux_x4_buf"      )
LV_tsLeafCell.append ( "dp_mux_x4"          )
LV_tsLeafCell.append ( "dp_nts_x2_buf"      )
LV_tsLeafCell.append ( "dp_nts_x2"          )
LV_tsLeafCell.append ( "dp_ts_x4_buf"       )
LV_tsLeafCell.append ( "dp_ts_x4"           )
LV_tsLeafCell.append ( "dp_ts_x8_buf"       )
LV_tsLeafCell.append ( "dp_ts_x8"           )
LV_tsLeafCell.append ( "inv_x8"             )
LV_tsLeafCell.append ( "na2_x4"             )
LV_tsLeafCell.append ( "inv_x4"             )
LV_tsLeafCell.append ( "buf_x8"             )
LV_tsLeafCell.append ( "inv_x4"             )
LV_tsLeafCell.append ( "no2_x4"             )
LV_tsLeafCell.append ( "buf_x8"             )
LV_tsLeafCell.append ( "inv_x4"             )
LV_tsLeafCell.append ( "nxr2_x4"            )
LV_tsLeafCell.append ( "zero_x0"            )
LV_tsLeafCell.append ( "one_x0"             )
LV_tsLeafCell.append ( "dp_dff_x4_buf"      )
LV_tsLeafCell.append ( "dp_dff_x4"          )
LV_tsLeafCell.append ( "dp_dff_scan_x4_buf" )
LV_tsLeafCell.append ( "dp_dff_scan_x4"     )
LV_tsLeafCell.append ( "dp_sff_x4_buf"      )
LV_tsLeafCell.append ( "dp_sff_x4"          )
LV_tsLeafCell.append ( "dp_sff_scan_x4_buf" )
LV_tsLeafCell.append ( "dp_sff_scan_x4"     )

###########################
## Functions Definitions ##
###########################

###################
def set_tDrive ( LV_Function ) :
  global LV_tDrive
  global LV_ttDrive

  
  if   LV_Function == DPGEN_INV       : LV_tDrive = LV_ttDrive[1]
  elif LV_Function == DPGEN_BUFF      : LV_tDrive = LV_ttDrive[2]
  elif LV_Function == DPGEN_NAND2     : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_NAND3     : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_NAND4     : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_NOR2      : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_NOR3      : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_NOR4      : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_XNOR2     : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_XOR2      : LV_tDrive = LV_ttDrive[3]
  elif LV_Function == DPGEN_AND2      : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_AND3      : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_AND4      : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_OR2       : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_OR3       : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_OR4       : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_MUX2      : LV_tDrive = LV_ttDrive[4]
  elif LV_Function == DPGEN_BUSE      : LV_tDrive = LV_ttDrive[5]
  elif LV_Function == DPGEN_NMUX2     : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_NBUSE     : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_NAND2MASK : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_NOR2MASK  : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_XNOR2MASK : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_CONST     : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_DFF       : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_DFFT      : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_SFF       : LV_tDrive = LV_ttDrive[0]
  elif LV_Function == DPGEN_SFFT      : LV_tDrive = LV_ttDrive[0]
  else                                : LV_tDrive = LV_ttDrive[0] 
  
######################
def set_iLeafCell ( LV_Function ) :
  global LV_iLeafCell

  LV_iLeafCell = 0

  if   LV_Function == DPGEN_SFFT      : LV_iLeafCell += 64
  elif LV_Function == DPGEN_SFF       : LV_iLeafCell += 62
  elif LV_Function == DPGEN_DFFT      : LV_iLeafCell += 60
  elif LV_Function == DPGEN_DFF       : LV_iLeafCell += 58
  elif LV_Function == DPGEN_CONST     : LV_iLeafCell += 56
  elif LV_Function == DPGEN_XNOR2MASK : LV_iLeafCell += 53
  elif LV_Function == DPGEN_NOR2MASK  : LV_iLeafCell += 50
  elif LV_Function == DPGEN_NAND2MASK : LV_iLeafCell += 47
  elif LV_Function == DPGEN_BUSE      : LV_iLeafCell += 43
  elif LV_Function == DPGEN_NBUSE     : LV_iLeafCell += 41
  elif LV_Function == DPGEN_MUX2      : LV_iLeafCell += 37
  elif LV_Function == DPGEN_NMUX2     : LV_iLeafCell += 35
  elif LV_Function == DPGEN_XOR2      : LV_iLeafCell += 33
  elif LV_Function == DPGEN_XNOR2     : LV_iLeafCell += 31
  elif LV_Function == DPGEN_OR4       : LV_iLeafCell += 29
  elif LV_Function == DPGEN_OR3       : LV_iLeafCell += 27
  elif LV_Function == DPGEN_OR2       : LV_iLeafCell += 25
  elif LV_Function == DPGEN_NOR4      : LV_iLeafCell += 23
  elif LV_Function == DPGEN_NOR3      : LV_iLeafCell += 21
  elif LV_Function == DPGEN_NOR2      : LV_iLeafCell += 19
  elif LV_Function == DPGEN_AND4      : LV_iLeafCell += 17
  elif LV_Function == DPGEN_AND3      : LV_iLeafCell += 15
  elif LV_Function == DPGEN_AND2      : LV_iLeafCell += 13
  elif LV_Function == DPGEN_NAND4     : LV_iLeafCell += 11
  elif LV_Function == DPGEN_NAND3     : LV_iLeafCell += 9
  elif LV_Function == DPGEN_NAND2     : LV_iLeafCell += 7
  elif LV_Function == DPGEN_BUFF      : LV_iLeafCell += 4

##################################
def init_Generator ( aFunction ) :
  global LV_Function

  # Set the function once and for all.
  LV_Function = aFunction

  set_tDrive    ( LV_Function )
  set_iLeafCell ( LV_Function )

###################
def has_Buffer ( LV_Function ) :

  if LV_Function in ( DPGEN_INV, DPGEN_BUFF, DPGEN_NAND2, DPGEN_NAND3, DPGEN_NAND4, DPGEN_AND2, DPGEN_AND3, DPGEN_AND4, DPGEN_NOR2, DPGEN_NOR3, DPGEN_NOR4, DPGEN_OR2, DPGEN_OR3, DPGEN_OR4, DPGEN_XNOR2, DPGEN_XOR2, DPGEN_CONST ) :
    return 0
    
  elif LV_Function in ( DPGEN_NMUX2, DPGEN_NBUSE, DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT, DPGEN_BUSE, DPGEN_MUX2, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
    return 1

  return 0

################################
def get_Model_Cell ( aiSlice ) :
  global LV_Function
  global LV_iLeafCell
  global LV_tsLeafCell
  global LV_macroDrive
  global LV_xl

  index = ERROR_INDEX

  if LV_Function in ( DPGEN_INV, DPGEN_BUFF, DPGEN_NAND2, DPGEN_NAND3, DPGEN_NAND4, DPGEN_AND2, DPGEN_AND3, DPGEN_AND4, DPGEN_NOR2, DPGEN_NOR3, DPGEN_NOR4, DPGEN_OR2, DPGEN_OR3, DPGEN_OR4, DPGEN_XNOR2, DPGEN_XOR2 ) :
    index = LV_iLeafCell + LV_macroDrive
    
  elif LV_Function in ( DPGEN_NMUX2, DPGEN_NBUSE, DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) :
    index = LV_iLeafCell + 1
    
  elif LV_Function in ( DPGEN_MUX2, DPGEN_BUSE ) :
    index = LV_iLeafCell +  ( LV_macroDrive * 2 ) + 1 # LV_macroDrive << 1
    
  elif LV_Function in ( DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
    index = LV_iLeafCell + 1 + LV_xl.getxlbit ( aiSlice )
    
  elif LV_Function == DPGEN_CONST :
    index = LV_iLeafCell + LV_xl.getxlbit ( aiSlice )

  if ( index == ERROR_INDEX ) : return "bug_leaf"

  return LV_tsLeafCell[index]

#########################
def get_Model_Buffer () :
  global LV_Function
  global LV_iLeafCell
  global LV_tsLeafCell
  global LV_macroDrive

  index = ERROR_INDEX

  if LV_Function in ( DPGEN_NMUX2, DPGEN_NBUSE, DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
    index = LV_iLeafCell

  elif LV_Function in ( DPGEN_BUSE, DPGEN_MUX2 ) :
    index = LV_iLeafCell + ( LV_macroDrive * 2 ) # LV_macroDrive << 1 

  if index == ERROR_INDEX :  return "bug_buff"

  return LV_tsLeafCell[index]


##################
## DpgenRegular ##
##################
class DpgenRegular ( Model ) :

  ## Interface ##
  ###############
  def Interface ( self, LV_N, LV_Function ) :

    # Control terminals
    if LV_Function in ( DPGEN_NMUX2, DPGEN_MUX2, DPGEN_NBUSE, DPGEN_BUSE, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
      self._cmd = SignalIn ( "cmd", 1 )
   
    if LV_Function in ( DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) :
      self._wen = SignalIn ( "wen", 1 )
      self._ck  = SignalIn (  "ck", 1 )
    
    if LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
      self._scan = SignalIn ( "scan", 1 )
      self._scin = SignalIn ( "scin", 1 )
     
    # Input data buses terminals
    if LV_Function in ( DPGEN_OR4, DPGEN_NOR4, DPGEN_AND4, DPGEN_NAND4 ) :
      self._i3 = SignalIn ( "i3", LV_N )
      self._i2 = SignalIn ( "i2", LV_N )
      self._i1 = SignalIn ( "i1", LV_N )
      self._i0 = SignalIn ( "i0", LV_N )
      
    elif LV_Function in ( DPGEN_OR3, DPGEN_NOR3, DPGEN_AND3, DPGEN_NAND3 ) :
      self._i2 = SignalIn ( "i2", LV_N )
      self._i1 = SignalIn ( "i1", LV_N )
      self._i0 = SignalIn ( "i0", LV_N )
      
    elif LV_Function in ( DPGEN_MUX2, DPGEN_NMUX2, DPGEN_XOR2, DPGEN_XNOR2, DPGEN_OR2, DPGEN_NOR2, DPGEN_AND2, DPGEN_NAND2 ) :
      self._i1 = SignalIn ( "i1", LV_N )
      self._i0 = SignalIn ( "i0", LV_N )

    elif LV_Function in ( DPGEN_DFFT, DPGEN_DFF, DPGEN_SFFT, DPGEN_SFF, DPGEN_XNOR2MASK, DPGEN_NOR2MASK, DPGEN_NAND2MASK, DPGEN_BUSE, DPGEN_NBUSE, DPGEN_BUFF, DPGEN_INV ) :
      self._i0 = SignalIn ( "i0", LV_N )

    # Output data buses terminals
    if LV_Function in ( DPGEN_CONST, DPGEN_MUX2, DPGEN_DFFT, DPGEN_DFF, DPGEN_SFFT, DPGEN_SFF, DPGEN_XOR2, DPGEN_OR4, DPGEN_OR3, DPGEN_OR2, DPGEN_AND4, DPGEN_AND3, DPGEN_AND2, DPGEN_BUFF ) :
      self._q = SignalOut ( "q", LV_N )

    elif LV_Function == DPGEN_BUSE:
      self._q = TriState ( "q", LV_N )

    elif LV_Function in ( DPGEN_INV, DPGEN_NAND2, DPGEN_NAND3, DPGEN_NAND4, DPGEN_NOR2, DPGEN_NOR3, DPGEN_NOR4, DPGEN_XNOR2, DPGEN_NMUX2, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
      self._nq = SignalOut ( "nq", LV_N )

    elif LV_Function == DPGEN_NBUSE:
      self._nq = TriState ( "nq", LV_N )

    # Power terminals
    self._vdd = VddIn ( "vdd" )
    self._vss = VssIn ( "vss" )


  ## Netlist ##
  ############# 
  def Netlist ( self, LV_N, LV_Function, *val ) :
    global LV_drive, LV_macroDrive, LV_xl
     
    # Initializes for the requested generator
    init_Generator ( LV_Function )
  
    LV_drive      = 1
    LV_macroDrive = 0
  
    # Parses the optionals arguments
    if LV_Function in ( DPGEN_INV, DPGEN_BUFF, DPGEN_NAND2, DPGEN_NAND3, DPGEN_NAND4, DPGEN_AND2, DPGEN_AND3, DPGEN_AND4, DPGEN_NOR2, DPGEN_NOR3, DPGEN_NOR4, DPGEN_OR2, DPGEN_OR3, DPGEN_OR4, DPGEN_XNOR2, DPGEN_XOR2, DPGEN_MUX2, DPGEN_BUSE ) :
      LV_drive = val[0]
      
    elif LV_Function in ( DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK, DPGEN_CONST ) :
      LV_xl = newxl ()
      LV_xl.atoxl ( val[0] )
      temp1 = LV_xl.xltovhdl()
      LV_xl._size = LV_N
      temp2 = LV_xl.xltovhdl()
  
      if temp1 != temp2 :
        print( "[Stratus Warning] : Due to its wrong size, the constant", temp1, "has been modified to :", temp2, ". Check if it's the value one wanted." )
  
    # Check the drive argument
    for iDrive in range ( MAX_TDRIVE ) :
      if ( LV_tDrive[iDrive] and ( LV_tDrive[iDrive] == LV_drive ) ) :
        LV_macroDrive = iDrive
        break
  
      if iDrive == MAX_TDRIVE - 1 : iDrive += 1
  
    if iDrive >= MAX_TDRIVE :
      raise "\n[Stratus ERROR] Dpgen : Invalid drive value %ld.\n" % LV_drive

    # Internal signals
    if LV_Function in ( DPGEN_NBUSE, DPGEN_BUSE )  :
      enx    = Signal (  "enx", 1 )
      nenx   = Signal ( "nenx", 1 )
    elif LV_Function in ( DPGEN_DFF, DPGEN_SFF ) :
      ckx    = Signal (   "ckx", 1 )
      nckx   = Signal (  "nckx", 1 )
      wenx   = Signal (  "wenx", 1 )
      nwenx  = Signal ( "nwenx", 1 )          
    elif LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
      ckx    = Signal (    "ckx", 1 )
      nckx   = Signal (   "nckx", 1 )
      wenx   = Signal (   "wenx", 1 )
      nwenx  = Signal (  "nwenx", 1 )
      scanx  = Signal (  "scanx", 1 )
      nscanx = Signal ( "nscanx", 1 )
      scoutx = Signal ( "scoutx", 1 )
    elif LV_Function in ( DPGEN_MUX2, DPGEN_NMUX2 ) :
      sel0   = Signal (  "sel0", 1 )
      sel1   = Signal (  "sel1", 1 )
    elif LV_Function == DPGEN_NAND2MASK :
      ncmd   = Signal (  "ncmd", 1 )
    elif LV_Function in ( DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
      for iSlice in range ( LV_N ) :
        if LV_xl.getxlbit ( iSlice ) :
          bcmd = Signal ( "bcmd", 1 )
          break

          
    # Loop for all the data slices
    self.Cell = {}
          
    for iSlice in range ( LV_N ) :
      sModel = get_Model_Cell ( iSlice )
  
      if LV_Function == DPGEN_INV :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : self._i0[iSlice]
                                         , 'nq'  : self._nq[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_BUFF :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : self._i0[iSlice]
                                         , 'q'   :  self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_NAND2, DPGEN_NOR2, DPGEN_XNOR2 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         , 'nq'  : self._nq[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_AND2, DPGEN_OR2, DPGEN_XOR2 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         ,  'q'  :  self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
        
      elif LV_Function in ( DPGEN_NAND3, DPGEN_NOR3 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         , 'i2'  : self._i2[iSlice]
                                         , 'nq'  : self._nq[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_AND3, DPGEN_OR3 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         , 'i2'  : self._i2[iSlice]
                                         , 'q'   :  self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_NAND4, DPGEN_NOR4 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         , 'i2'  : self._i2[iSlice]
                                         , 'i3'  : self._i3[iSlice]
                                         , 'nq'  : self._nq[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_AND4, DPGEN_OR4 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i0'  : self._i0[iSlice]
                                         , 'i1'  : self._i1[iSlice]
                                         , 'i2'  : self._i2[iSlice]
                                         , 'i3'  : self._i3[iSlice]
                                         , 'q'   :  self._q[iSlice]
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_NMUX2 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'sel0' : sel0
                                         , 'sel1' : sel1
                                         , 'i1'   : self._i1[iSlice]
                                         , 'i0'   : self._i0[iSlice]
                                         , 'nq'   : self._nq[iSlice]
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_MUX2 :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'sel0' : sel0
                                         , 'sel1' : sel1
                                         , 'i1'   : self._i1[iSlice]
                                         , 'i0'   : self._i0[iSlice]
                                         , 'q'    :  self._q[iSlice]
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_NBUSE :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'enx'  : enx
                                         , 'nenx' : nenx
                                         , 'i'    : self._i0[iSlice]
                                         , 'nq'   : self._nq[iSlice]
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_BUSE :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'enx'  : enx
                                         , 'nenx' : nenx
                                         , 'i'    : self._i0[iSlice]
                                         , 'q'    :  self._q[iSlice]
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function == DPGEN_NAND2MASK :
        if LV_xl.getxlbit ( iSlice ) :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'i'   : self._i0[iSlice]
                                           , 'nq'  : self._nq[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
        else :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'i1'  : ncmd
                                           , 'i0'  : self._i0[iSlice]
                                           , 'nq'  : self._nq[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
  
      elif LV_Function in ( DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
        if LV_xl.getxlbit ( iSlice ) :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'i1'  : bcmd
                                           , 'i0'  : self._i0[iSlice]
                                           , 'nq'  : self._nq[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
        else :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'i'   : self._i0[iSlice]
                                           , 'nq'  : self._nq[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
  
      elif LV_Function == DPGEN_CONST :
        if LV_xl.getxlbit ( iSlice ) :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'q'   : self._q[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
        else :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'nq'  : self._q[iSlice]
                                           , 'vdd' : self._vdd
                                           , 'vss' : self._vss
                                           }
                                   )
  
      elif ( LV_Function == DPGEN_DFF ) or ( LV_Function == DPGEN_SFF ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'ckx'   : ckx
                                         , 'nckx'  : nckx
                                         , 'wenx'  : wenx
                                         , 'nwenx' : nwenx
                                         , 'i'     : self._i0[iSlice]
                                         , 'q'     :  self._q[iSlice]
                                         , 'vdd'   : self._vdd
                                         , 'vss'   : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
        if iSlice == 0 :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'ckx'    : ckx
                                           , 'nckx'   : nckx
                                           , 'wenx'   : wenx
                                           , 'nwenx'  : nwenx
                                           , 'scanx'  : scanx
                                           , 'nscanx' : nscanx
                                           , 'scin'   : scoutx
                                           , 'i'      : self._i0[iSlice]
                                           , 'q'      : self._q[iSlice]
                                           , 'vdd'    : self._vdd
                                           , 'vss'    : self._vss
                                           }
                                   )
        else :
          self.Cell[iSlice] = Inst ( sModel
                                   , "cell_%ld" % iSlice
                                   , map = { 'ckx'    : ckx
                                           , 'nckx'   : nckx
                                           , 'wenx'   : wenx
                                           , 'nwenx'  : nwenx
                                           , 'scanx'  : scanx
                                           , 'nscanx' : nscanx
                                           , 'scin'   : self._q[iSlice - 1]
                                           , 'i'      : self._i0[iSlice]
                                           , 'q'      : self._q[iSlice]
                                           , 'vdd'    : self._vdd
                                           , 'vss'    : self._vss
                                           }
                                   )
  
    iSlice += 1 # attention : difference entre les boucles en c et les boucles en python
    
    if has_Buffer ( LV_Function ) :
      if iSlice % 2 : iSlice += 1
  
      sModel    = get_Model_Buffer ()
  
      if LV_Function == DPGEN_NAND2MASK :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : self._cmd
                                         , 'nq'  : ncmd
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'i'   : self._cmd
                                         , 'q'   : bcmd
                                         , 'vdd' : self._vdd
                                         , 'vss' : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_NMUX2, DPGEN_MUX2 ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'sel'  : self._cmd
                                         , 'sel0' : sel0
                                         , 'sel1' : sel1
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_NBUSE, DPGEN_BUSE ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'en'   : self._cmd
                                         , 'nenx' : nenx
                                         , 'enx'  : enx
                                         , 'vdd'  : self._vdd
                                         , 'vss'  : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_DFF, DPGEN_SFF ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'ck'    : self._ck
                                         , 'wen'   : self._wen
                                         , 'ckx'   : ckx
                                         , 'nckx'  : nckx
                                         , 'wenx'  : wenx
                                         , 'nwenx' : nwenx
                                         , 'vdd'   : self._vdd
                                         , 'vss'   : self._vss
                                         }
                                 )
  
      elif LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
        self.Cell[iSlice] = Inst ( sModel
                                 , "cell_%ld" % iSlice
                                 , map = { 'ck'     : self._ck
                                         , 'wen'    : self._wen
                                         , 'scan'   : self._scan
                                         , 'ckx'    : ckx
                                         , 'nckx'   : nckx
                                         , 'wenx'   : wenx
                                         , 'nwenx'  : nwenx
                                         , 'scanx'  : scanx
                                         , 'nscanx' : nscanx
                                         , 'scin'   : self._scin
                                         , 'scout'  : scoutx
                                         , 'vdd'    : self._vdd
                                         , 'vss'    : self._vss
                                         }
                                 )

      
  ## Layout ##
  ############
  def Layout ( self, LV_N, LV_Function ) :
  
    # Loop for all the data slices
    for iSlice in range ( LV_N ) :
      Place ( self.Cell[iSlice]
            , slice_to_sym ( iSlice )
            , XY ( 0, slice_to_y ( iSlice ) )
            )
  
    iSlice += 1 # attention : difference entre les boucles en c et les boucles en python
  
    # Add the buffer cell
    if has_Buffer ( LV_Function ) :
      if iSlice % 2 : iSlice += 1
      
      Place ( self.Cell[iSlice]
            , slice_to_sym ( iSlice )
            , XY ( 0, slice_to_y ( iSlice ) )
            )


  ## VBE ##
  #########
  def Vbe ( self, LV_N, LV_flags, LV_Function ) :
  
    global file
  
    fileName = self._name + ".vbe"
    
    file = open ( fileName, "w+" )
    
    sBusWide = VHDL_BUS_WIDE ( LV_N - 1, 0, LV_N, LV_flags )
    
    ## Entity Description ##
    file.write ( "\nENTITY %s IS\n  PORT (\n" % self._name )
  
    ## Controls Terminals ##
    if LV_Function in ( DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK, DPGEN_BUSE, DPGEN_NBUSE, DPGEN_MUX2, DPGEN_NMUX2 ) :
      file.write ( "%14s : in    BIT;\n" % "cmd")
  
    if LV_Function in ( DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) :
      file.write ( "%14s : in    BIT;\n" % "wen" )
      file.write ( "%14s : in    BIT;\n" % "ck"  )
  
    if LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
      file.write ( "%14s : in    BIT;\n" % "scan")
      file.write ( "%14s : in    BIT;\n" % "scin")
      
    ## Input data buses terminals ##
    if LV_Function in ( DPGEN_NAND4, DPGEN_AND4, DPGEN_NOR4, DPGEN_OR4 ) :
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i3", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i2", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i1", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide ) )
  
    elif LV_Function in (DPGEN_NAND3, DPGEN_OR3, DPGEN_AND3, DPGEN_NOR3 ) :
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i2", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i1", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide ) )
  
    elif LV_Function in ( DPGEN_NAND2, DPGEN_AND2, DPGEN_NOR2, DPGEN_OR2, DPGEN_XNOR2, DPGEN_XOR2, DPGEN_NMUX2, DPGEN_MUX2 ) :
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i1", sBusWide ) )
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide ) )
  
    elif LV_Function in ( DPGEN_INV, DPGEN_BUFF, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK, DPGEN_NBUSE, DPGEN_BUSE, DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) :
      file.write ( "%14s : in    BIT_VECTOR %s;\n" % ( "i0", sBusWide ) )
      
    ## Output data buses terminals ##
    if LV_Function in ( DPGEN_NAND4, DPGEN_NOR4, DPGEN_NAND3, DPGEN_NOR3, DPGEN_NAND2, DPGEN_NOR2, DPGEN_XNOR2, DPGEN_NMUX2, DPGEN_INV, DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) :
      file.write ( "%14s :   out BIT_VECTOR %s;\n" % ( "nq", sBusWide ) )
        
    elif LV_Function == DPGEN_NBUSE :
      file.write ( "%14s :   out MUX_VECTOR %s BUS;\n" % ( "nq", sBusWide ) )
  
    elif LV_Function in ( DPGEN_AND2, DPGEN_AND3, DPGEN_AND4, DPGEN_OR2, DPGEN_OR3, DPGEN_OR4, DPGEN_XOR2, DPGEN_MUX2, DPGEN_BUFF, DPGEN_CONST, DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) :
      file.write ( "%14s :   out BIT_VECTOR %s;\n" % ( "q", sBusWide ) )
  
    elif LV_Function == DPGEN_BUSE :
      file.write ( "%14s :   out MUX_VECTOR %s BUS;\n" % ( "q", sBusWide ) )
      
    ## Power supplies terminals ##
    file.write ( "%14s : in    BIT;\n" % "vdd")
    file.write ( "%14s : in    BIT\n"  % "vss")
    
    ## End of entity description ##
    file.write ( "  );\nEND %s;\n\n\n" %  self._name )
    
    ## Architecture description ##
    file.write (  "ARCHITECTURE VBE OF %s IS\n\n" %  self._name )
  
    ## Signals ##
    if LV_Function in ( DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK, DPGEN_NMUX2 ) :
      file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n" % ( "q", sBusWide ) )
  
    elif LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
      file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n"          % ( "d_test", sBusWide ) )
      file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n"          % ( "d_wen",  sBusWide ) )
      file.write ( "  SIGNAL %20s : REG_VECTOR %s REGISTER;\n" % ( "ff",     sBusWide ) )
  
    elif LV_Function in ( DPGEN_DFF, DPGEN_SFF ) :
      file.write ( "  SIGNAL %20s : BIT_VECTOR %s;\n"          % ( "d_wen",  sBusWide ) )
      file.write ( "  SIGNAL %20s : REG_VECTOR %s REGISTER;\n" % ( "ff",     sBusWide ) )
  
    ## Behavior ##  
    file.write ( "\nBEGIN\n\n" )  
  
    if LV_Function in ( DPGEN_NAND4, DPGEN_AND4, DPGEN_NOR4, DPGEN_OR4, DPGEN_NAND3, DPGEN_OR3, DPGEN_AND3, DPGEN_NOR3, DPGEN_NAND2, DPGEN_AND2, DPGEN_NOR2, DPGEN_OR2, DPGEN_XNOR2, DPGEN_XOR2, DPGEN_INV, DPGEN_BUFF ) : self.VHDL_boolean ( LV_Function )
  
    elif LV_Function in ( DPGEN_NAND2MASK, DPGEN_NOR2MASK, DPGEN_XNOR2MASK ) : self.VHDL_mask ( LV_Function )
  
    elif LV_Function in ( DPGEN_NMUX2, DPGEN_MUX2 ) : self.VHDL_mux ( LV_Function )
  
    elif LV_Function in ( DPGEN_BUSE, DPGEN_NBUSE ) : self.VHDL_buse ( LV_Function )
      
    elif LV_Function == DPGEN_CONST : self.VHDL_const ()
      
    elif LV_Function in ( DPGEN_DFF, DPGEN_DFFT, DPGEN_SFF, DPGEN_SFFT ) : self.VHDL_ff ( LV_Function, LV_N, LV_flags )
   
    ## Assert ##
    file.write ( "  ASSERT (vdd = '1')\n" )
    file.write ( "    REPORT \"Power supply is missing on vdd of Model %s.\"\n" % self._name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    file.write ( "  ASSERT (vss = '0')\n" )
    file.write ( "    REPORT \"Power supply is missing on vss of Model %s.\"\n" % self._name )
    file.write ( "    SEVERITY WARNING;\n\n" )
  
    ## End of behavioral view ##
    file.write ( "END VBE;\n" )
  
    file.close ()
  
  
  ## VHDL_boolean ##
  ##################  
  def  VHDL_boolean ( self, LV_Function ) :
    global file
  
    if   LV_Function == DPGEN_INV   : file.write ( "  nq <= not i0;\n\n" )
    elif LV_Function == DPGEN_BUFF  : file.write ( "   q <= i0;\n\n" )
    elif LV_Function == DPGEN_NAND2 : file.write ( "  nq <= not (i0 and i1);\n\n" )
    elif LV_Function == DPGEN_AND2  : file.write ( "   q <= (i0 and i1);\n\n" )
    elif LV_Function == DPGEN_NOR2  : file.write ( "  nq <= not (i0 or i1);\n\n" )
    elif LV_Function == DPGEN_OR2   : file.write ( "   q <= (i0 or i1);\n\n" )
    elif LV_Function == DPGEN_XNOR2 : file.write ( "  nq <= not (i0 xor i1);\n\n" )
    elif LV_Function == DPGEN_XOR2  : file.write ( "   q <= (i0 xor i1);\n\n" )
    elif LV_Function == DPGEN_NOR3  : file.write ( "  nq <= not (i0 or i1 or i2);\n\n" )
    elif LV_Function == DPGEN_OR3   : file.write ( "   q <= (i0 or i1 or i2);\n\n" )
    elif LV_Function == DPGEN_NAND3 : file.write ( "  nq <= not (i0 and i1 and i2);\n\n" )
    elif LV_Function == DPGEN_AND3  : file.write ( "   q <= (i0 and i1 and i2);\n\n" )
    elif LV_Function == DPGEN_NAND4 : file.write ( "  nq <= not (i0 and i1 and i2 and i3);\n\n" )
    elif LV_Function == DPGEN_AND4  : file.write ( "   q <= (i0 and i1 and i2 and i3);\n\n" )
    elif LV_Function == DPGEN_NOR4  : file.write ( "  nq <= not (i0 or i1 or i2 or i3);\n\n" )
    elif LV_Function == DPGEN_OR4   : file.write ( "   q <= (i0 or i1 or i2 or i3);\n\n" )
  
  ## VHDL_mask ##
  ###############
  def  VHDL_mask ( self, LV_Function ) :
    global LV_xl
  
    global file
  
    file.write ( "  WITH  cmd  SELECT\n" )
    file.write ( "    q <=  i0  WHEN \'0\',\n" )
  
    if   LV_Function == DPGEN_NAND2MASK : 
      file.write ( "          (i0 and %s)  WHEN \'1\';\n" % LV_xl.xltovhdl ( ) )
  
    elif LV_Function == DPGEN_NOR2MASK :
      file.write ( "          (i0 or %s) WHEN \'1\';\n"   % LV_xl.xltovhdl ( ) )
  
    elif LV_Function == DPGEN_XNOR2MASK :
      file.write ( "          (i0 xor %s) WHEN \'1\';\n"  % LV_xl.xltovhdl ( ) )
  
    file.write ( "\n  nq <= not q;\n\n" )
  
  ## VHDL_const ##
  ################
  def VHDL_const ( self ) :
    global LV_xl
  
    global file
  
    file.write ( "  q <= %s;\n\n" % LV_xl.xltovhdl ( ) )
  
  ## VHDL_mux ##
  ##############
  def VHDL_mux ( self, LV_Function ) :
  
    global file
    
    if LV_Function in ( DPGEN_NMUX2, DPGEN_MUX2 ) :
      file.write ( "  WITH  cmd  SELECT\n" )
      file.write ( "    q <= i0  WHEN  \'0\',\n" )
      file.write ( "         i1  WHEN  \'1\';\n" )
  
    if   LV_Function == DPGEN_NMUX2 :
      file.write ( "\n  nq <= not q;\n" )
  
    file.write ( "\n" )
  
  ## VHDL_buse ##
  ###############
  def VHDL_buse ( self, LV_Function ) :
  
    global file
    
    file.write ( "  label_buse:BLOCK(cmd = '1')\n" )
    file.write ( "  BEGIN\n" )
  
    if LV_Function == DPGEN_NBUSE :
      file.write ( "    nq <= GUARDED not i0;\n" )
  
    elif LV_Function == DPGEN_BUSE :
      file.write ( "    q <= GUARDED i0;\n" )
  
    file.write ( "  END BLOCK label_buse;\n\n" )
  
  ## VHDL_ff () ##
  ################
  def VHDL_ff ( self, LV_Function, LV_N, LV_flags ) :
  
    global file
  
    sBusWide = VHDL_BUS_WIDE ( LV_N - 2, 0, LV_N, LV_flags )
  
    dwrite = "d_wen"
  
    if LV_Function in ( DPGEN_DFFT, DPGEN_SFFT ) :
      file.write ( "  WITH  scan  SELECT\n" )
      file.write ( "    d_test <= d_wen  WHEN  \'0\',\n" )
      file.write ( "              q %s & scin  WHEN  \'1\';\n\n" % sBusWide )
  
      dwrite = "d_test";
  
      file.write ( "  WITH  wen  SELECT\n" )
      file.write ( "    d_wen <=  q  WHEN  \'0\',\n" )
      file.write ( "             i0  WHEN  \'1\';\n\n" )
  
      
    elif LV_Function in ( DPGEN_DFF, DPGEN_SFF ) :
      file.write ( "  WITH  wen  SELECT\n" )
      file.write ( "    d_wen <=  q  WHEN  \'0\',\n" )
      file.write ( "             i0  WHEN  \'1\';\n\n" )
  
    
    file.write ( "  label_ff:BLOCK ((ck and not (ck'STABLE)) = \'1\')\n" )
    file.write ( "  BEGIN\n" )
    file.write ( "    ff <= GUARDED %s;\n" % dwrite )
    file.write ( "  END BLOCK label_ff;\n\n" )
  
    file.write ( "  q <= ff;\n\n" )


##########################################################################

## AND2 ##
class DpgenAnd2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_AND2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_AND2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_AND2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_AND2 )

## AND3 ##
class DpgenAnd3 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_AND3 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_AND3, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_AND3 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_AND3 )

## AND4 ##
class DpgenAnd4 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_AND4 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_AND4, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_AND4) 

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_AND4 )

## BUFF ##
class DpgenBuff ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_BUFF )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_BUFF, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_BUFF )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self,self._param['nbit'], self._param['flags'], DPGEN_BUFF )

## BUSE ##
class DpgenBuse ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 4
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_BUSE )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_BUSE, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_BUSE )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_BUSE )

## CONST ##
class DpgenConst ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_CONST )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_CONST, self._param['const'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_CONST )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_CONST )

## DFF ##
class DpgenDff ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_DFF )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_DFF, 0 )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_DFF )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_DFF )

## DFFT ##
class DpgenDfft ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_DFFT )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_DFFT, 0 )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_DFFT )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_DFFT )

## INV ##
class DpgenInv ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_INV )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_INV, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_INV )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_INV )

## MUX2 ##
class DpgenMux2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_MUX2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_MUX2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_MUX2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_MUX2 )   

## NAND2 ##
class DpgenNand2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NAND2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NAND2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NAND2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NAND2 )

## NAND2MASK ##
class DpgenNand2mask ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NAND2MASK )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NAND2MASK, self._param['const'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NAND2MASK )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NAND2MASK )

## NAND3 ##
class DpgenNand3 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NAND3 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NAND3, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NAND3 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NAND3 )

## NAND4 ##
class DpgenNand4 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NAND4 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NAND4, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NAND4 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NAND4 )

## NBUSE ##
class DpgenNbuse ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NBUSE )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NBUSE )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NBUSE )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NBUSE )

## NMUX2 ##
class DpgenNmux2 ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NMUX2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NMUX2 )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NMUX2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NMUX2 )   

## NOR2 ##
class DpgenNor2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NOR2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NOR2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NOR2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NOR2 )

## NOR2MASK ##
class DpgenNor2mask ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NOR2MASK )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NOR2MASK, self._param['const'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NOR2MASK )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NOR2MASK )

## NOR3 ##
class DpgenNor3 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NOR3 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NOR3, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NOR3 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NOR3 )

## NOR4 ##
class DpgenNor4 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_NOR4 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_NOR4, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_NOR4 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_NOR4 )
    
## OR2 ##
class DpgenOr2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_OR2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_OR2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_OR2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_OR2 )

## OR3 ##
class DpgenOr3 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_OR3 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_OR3, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_OR3 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_OR3 )

## OR4 ##
class DpgenOr4 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 2
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_OR4 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_OR4, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_OR4 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_OR4 )
    
## SFF ##
class DpgenSff ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_SFF )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_SFF, 0 )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_SFF )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_SFF )

## SFFT ##
class DpgenSfft ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_SFFT )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_SFFT, 0 )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_SFFT )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_SFFT )

## XNOR2 ##
class DpgenXnor2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_XNOR2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_XNOR2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_XNOR2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_XNOR2 )

## XNOR2MASK ##
class DpgenXnor2mask ( DpgenRegular ) :
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_XNOR2MASK )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_XNOR2MASK, self._param['const'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_XNOR2MASK )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_XNOR2MASK )

## XOR2 ##
class DpgenXor2 ( DpgenRegular ) :
  def __init__ ( self, name, param ) :
    Model.__init__ ( self, name, param )
    if 'drive' not in param : self._param['drive'] = 1
    
  def Interface ( self ) :
    DpgenRegular.Interface    ( self, self._param['nbit'], DPGEN_XOR2 )

  def Netlist ( self ) :
    DpgenRegular.Netlist      ( self, self._param['nbit'], DPGEN_XOR2, self._param['drive'] )

  def Layout ( self ) :
    DpgenRegular.Layout       ( self, self._param['nbit'], DPGEN_XOR2 )

  def Vbe ( self ) :
    DpgenRegular.Vbe          ( self, self._param['nbit'], self._param['flags'], DPGEN_XOR2 )

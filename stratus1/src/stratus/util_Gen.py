
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
# |  Py Module   :       "./util_Gen.py"                            |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



F_LSB_FIRST = 0x00000000    # flags macro
F_MSB_FIRST = 0x00000001
F_BEHAV     = 0x00000004


DPGEN_INV        =  1
DPGEN_BUFF       =  2
DPGEN_NAND2      =  3
DPGEN_NAND3      =  4
DPGEN_NAND4      =  5
DPGEN_AND2       =  6
DPGEN_AND3       =  7
DPGEN_AND4       =  8
DPGEN_NOR2       =  9
DPGEN_NOR3       = 10
DPGEN_NOR4       = 11
DPGEN_OR2        = 12
DPGEN_OR3        = 13
DPGEN_OR4        = 14
DPGEN_XNOR2      = 15
DPGEN_XOR2       = 16
DPGEN_NMUX2      = 17
DPGEN_MUX2       = 18
DPGEN_NBUSE      = 19
DPGEN_BUSE       = 20
DPGEN_NAND2MASK  = 21
DPGEN_NOR2MASK   = 22
DPGEN_XNOR2MASK  = 23
DPGEN_ADSB2F     = 24
DPGEN_SHIFT      = 25
DPGEN_NUL        = 26
DPGEN_CONST      = 27
DPGEN_RF1        = 28
DPGEN_RF1R0      = 29
DPGEN_RF1D       = 30
DPGEN_RF1DR0     = 31
DPGEN_FIFO       = 32
DPGEN_RF2        = 33
DPGEN_RF2R0      = 34
DPGEN_RF2D       = 35
DPGEN_RF2DR0     = 36
DPGEN_RAM        = 37
DPGEN_ROM        = 38
DPGEN_ROMTS      = 39
DPGEN_DFF        = 40
DPGEN_DFFT       = 41
DPGEN_SFF        = 42
DPGEN_SFFT       = 43
DPGEN_ROM2       = 44
DPGEN_ROM4       = 45
DPGEN_SHROT      = 46
DPGEN_UNKNOWN    = 47


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
# |  Author      : Roselyne AVOT-CHOTIN - Sophie BELLOEIL           |
# |  E-mail      :           Sophie.Belloeil@asim.lip6.fr           |
# | =============================================================== |
# |  Py Module   :       "./st_shifter.py"                          |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *


def getConfig ( type ) :
   def getValue ( pos ) :
      if (type & 2**pos) != 0 : return True
      else                    : return False
      
   config = { 'sticky' : None
            , 'type' : {'log' : None, 'arith' : None, 'rot' : None, 'con' : None}
            , 'dir' :  {'left' : None, 'right' : None, 'con' : None}
            }

   config['sticky']        = getValue(5)
   config['type']['log']   = getValue(4)
   config['type']['arith'] = getValue(3)
   config['type']['rot']   = getValue(2)
   config['dir']['left']   = getValue(1)
   config['dir']['right']  = getValue(0)
   
   config['dir']['con']    = config['dir']['left'] and config['dir']['right']
   config['type']['con']   = ((config['type']['log'] and config['type']['arith'])
         or (config['type']['log'] and config['type']['rot'])
         or (config['type']['arith'] and config['type']['rot']))
   
   if not (config['dir']['left'] or config['dir']['right']):
      raise Exception ( 'any of left or right directions are set' )
   if not (config['type']['log'] or config['type']['arith'] or config['type']['rot']) :
      raise Exception ( 'any of log or arith or rot types are set' )

   return config
   

class Shifter ( Model ) :
    
  def __init__ ( self, name, param ) :
       
    Model.__init__ ( self, name, param )
    
    self.nbit = param['nbit']
    
    if self.nbit < 2: raise Exception ( "\n[Stratus ERROR] Shift : input arity should be greater than 1.\n" )
    
    self.config = getConfig(param['type'])
    
    self.cmd_width = log2 ( self.nbit )

  def Interface ( self ) :
       
    self.cmd = SignalIn  ( "cmd", self.cmd_width )
    self.i   = SignalIn  (   "i", self.nbit )
    self.s   = SignalOut (   "s", self.nbit )
    
    if self.config['sticky'] :      self.sticky = SignalOut ( "sticky", 1 )
    if self.config['type']['con'] : self.type   = SignalIn  (    "typ", 3 )
    if self.config['dir']['con'] :  self.dir    = SignalIn  (    "dir", 1 )
    
    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )

  def Netlist ( self ) :
      
    nbStages = self.cmd_width
    
    MuxOutput = {}
    inj_lsb   = {}
    inj_msb   = {}
    mux_cmd   = {}
    
    one = Signal ( "one", 1 )
    one <= One ( 1 )
    
    zero = Signal ( "zero", 1 )
    zero <= Zero ( 1 )
    
    # Normalizing the input signals names
    MuxOutput[0] = self.i
    
    # signal giving the shift direction
    if not self.config['dir']['con'] :
       if self.config['dir']['left'] : dir = zero.Buffer()
       else                          : dir = one.Buffer()
    else :
       dir = self.dir
    
    # signals giving the type of shift
    if self.config['type']['con'] :
       log   = self.type[2].Buffer()
       arith = self.type[1].Buffer()
       rot   = self.type[0].Buffer()
    else :
       if self.config['type']['log'] : log = one.Buffer()
       else                          : log = zero.Buffer()
    
       if self.config['type']['arith'] : arith = one.Buffer()
       else                            : arith = zero.Buffer()
    
       if self.config['type']['rot'] : rot = one.Buffer()
       else                          : rot = zero.Buffer()
    
    # Building the control signals
    c0 = Signal ( "c0", nbStages )
    c1 = Signal ( "c1", nbStages )
    
    for stage in range ( nbStages ) :
       c1[stage] <= ~self.cmd[stage]
       c0[stage] <= c1[stage] | dir
    
    # Connecting the mux matrix
    for stage in range ( nbStages ) :
       valDec = 2**stage
       
       inj_lsb[stage]     = Signal ( "inj_lsb%d" %     stage, valDec )
       inj_msb[stage]     = Signal ( "inj_msb%d" %     stage, valDec )
       MuxOutput[stage+1] = Signal ( "mxout%d"   % (stage+1), self.nbit )
       
       if ( self.nbit-valDec-1 ) == 0 : MuxInput0 = MuxOutput[stage][0]
       else                           : MuxInput0 = MuxOutput[stage][self.nbit-valDec-1:0]
       
       for i in range ( valDec ) :
          inj_lsb[stage][i] <= (((MuxOutput[stage][i] & dir) | (MuxOutput[stage][self.nbit-valDec+i] & ~dir)) & rot) | (dir & ~rot)
    
       MuxInput0 = Cat ( MuxInput0, inj_lsb[stage] )
       MuxInput1 = MuxOutput[stage]
       
       # mettre zero, signe ou rot
       for i in range ( valDec ) :
          inj_msb[stage][i] <= (inj_lsb[stage][i] & rot) | (self.i[self.nbit-1] & arith) | (zero & log)
    
       if (self.nbit-1) == valDec : MuxInput2 = MuxOutput[stage][self.nbit-1]
       else                       : MuxInput2 = MuxOutput[stage][self.nbit-1:valDec]
       
       MuxInput2      = Cat ( inj_msb[stage], MuxInput2 )
       mux_cmd[stage] = Cat ( c0[stage], c1[stage] )
    
       MuxOutput[stage + 1] <= mux_cmd[stage].Mux ( { '0' : MuxInput0
                                                    , '1' : MuxInput0
                                                    , '2' : MuxInput2
                                                    , '3' : MuxInput1
                                                    }
                                                  )
    
    # Normalizing the output signal names
    self.s <= MuxOutput[nbStages]
    
  def GetModelName ( cls, param ) :
  
    modelName = "shifter"
    
    modelName += "_"
    modelName += str(param['nbit'])

    if   param['type'] == 0x12 : modelName += "_ll"
    elif param['type'] == 0xa  : modelName += "_al"
    elif param['type'] == 0x6  : modelName += "_cl"
    elif param['type'] == 0x11 : modelName += "_lr"
    elif param['type'] == 0x9  : modelName += "_ar"
    elif param['type'] == 0x5  : modelName += "_cr"

    return modelName
    
  GetModelName = classmethod ( GetModelName )

  def GetParam ( cls ):

    return {'nbit' : 'integer', 'type' : 'integer'}

  GetParam = classmethod ( GetParam )

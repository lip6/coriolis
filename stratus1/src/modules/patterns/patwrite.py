
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
# |  Author      :                      Ludovic NOURY               |
# |                              Roselyne CHOTIN-AVOT               |
# |  E-mail      :              Ludovic.Noury@lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./__init__.py"                            |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x

# $Id: patwrite.py,v 1.6 2009/02/10 09:38:18 roselyne Exp $
# Author: LIP6/ASIM ludovic.noury@lip6.fr
# Contributor: Roselyne.Chotin-Avot@lip6.fr
__all__ =['PatWrite']

from stratus import *
from utils   import *
from math    import ceil

## Create new patterns
#
# This class is used to create patterns for Stratus models. Currently
# it only supports Alliance ".pat" pattern format.
#
# Patterns time stamps are in the "absolute date" format, "relative
# date" isn't allowed. Legal time unit are ps, ns, us and ms. Default
# is ps.
#
# @remark we suppose output pattern files won't be read by a
# human. So to simplify the implementation we don't provide
# hexadecimal or octal representation for signal values. All the
# signals values are written in binary.
#
# @todo add another constructor with time interval as parameter.
# @todo add checks.
# @todo add another constructor and variable to select time unit.
# @todo add automatic vdd/vss declaration and affect. (?)
# @todo modify declar() to be able to declar "name",arity,type
#
# @version $Id: patwrite.py,v 1.6 2009/02/10 09:38:18 roselyne Exp $
class PatWrite:

    # Public attributes doxydoc ----------------------------
    ## @var connectors_list
    # store input/output connectors order

    ## @var connectors_current_values
    # dictionary, key=connector_name

    ## @var self.connectors_format
    # dictionary, key=connector_name, value=connector format

    ## @var current_time
    # auto-incremented after addpat()

    ## @var time_interval
    # time interval

    ## @var time_unit
    # time unit, valid values are ps, ns, us and ms.

    # Private attributes -----------------------------------
    ## @var _f
    # file object linked to the output file

    ## @var _finished
    # true if the pattern file has been written and closed. Used by
    # the destructor.

    ## @var _cell
    # Reference to the input Stratus cell. Used to be able to use
    # design net by theirs string name. Value 0 indicates that the
    # reference is unknown.
    
    # ------------------------------------------------------
    ## The constructor
    # @param filename New pattern output filename (example: \c adder.pat).
    # @param cell Optional, reference to Stratus cell.
    def __init__(self, filename, cell=None):
        # Instance attributes declaration
        self._f = open(filename, 'w')
        self._finished = False
        self.connectors_list = []           
        self.connectors_current_values = {}
        self.connectors_format = {}
        self.current_time = 0               
        self.time_interval = 10
        self.time_unit = 'ns'
        self._cell = cell


    # ------------------------------------------------------
    ## The destructor
    #
    # Call pattern_end if it hasn't been called yet.
    def __del__(self):
        if self._finished == False:
            self.pattern_end()

    # ------------------------------------------------------
    ## Get direction string from a net
    #
    # @param net Stratus net
    #
    # @remark: if InOut we default to out
    # @todo add inout mode, but we can't select automagically in or out !
    # @todo add code for all modes (ask sophie)
    def _get_direction(self, net):
        return {'st_net.SignalIn'   : 'in',
                'st_net.SignalOut'  : 'out',
                'st_net.Signal'     : 'out',
                'st_net.VddIn'      : 'in',
                'st_net.VssIn'      : 'in',                
                'st_net.CkIn'       : 'in',
                'st_net.Signal'     : 'signal',
                'st_net.TriState'   : 'out',
                'st_net.SignalInOut': 'out'
                }[str(net.__class__)]
    
         #'st_net.SignalInOut': in ou out selon le voeux de util'
         #'st_net.SignalUnknown': je sais pas quoi faire
         #'st_net.TriState' :  je sais pas quoi faire
         # VddInFromHur, VssInFromHur, Signal_virtuel


    # ------------------------------------------------------
    # Predicate, true of the connector is an output
    # @remark: considers that SignalInOut is an output
    #
    # @param self object reference
    # @param connector Stratus connector
    def _is_output(self, connector):
        return ((str(connector.__class__ ) == "st_net.SignalOut") or
                (str(connector.__class__ ) == "st_net.SignalInOut"))

    # ------------------------------------------------------
    ## Adds a connector from a Stratus model to the pattern interface.
    #
    # Writes the corresponding connector declaration in the pattern
    # file with name, arity and direction automatically extracted from
    # the connector properties.
    #
    # Also adds the connector to \e connectors_list. It is used when
    # add patterns to respect the connectors order.
    #
    # Supported Stratus connectors are:
    # - SignalIn,
    # - SignalOut (only supported if used as an output),
    # - VddIn,
    # - VssIn,
    # - CkIn,
    # - SignalInOut,
    # - TriState (always an output),
    # - Signals.
    #
    # @param connector Can either be a reference to a stratus net or a
    # string containing the name of the stratus net.
    #
    # @param format Optional format for the connectors values into the
    # pattern file, accepted values are :
    # - 'B': binary (default),
    # - 'X': hexadecimal,
    # - 'O': octal.
    #
    # @remark if the connector arity is greater than 1 it will always
    # be written in the form <code>N downto 0</code>.
    #
    # @remark if the connector arity is 1 the format is set 'B' even if
    # the format parameter is set to another value.
    #
    # @remark To be able to use a string as connector parameter the
    # constructor must have been given a reference to the net.
    #
    # @warning string connector don't seem to work for vdd/vss
    #
    # @todo add exception when string command and self._cell unset
    # @todo add exception when string command signal not found
    def declar(self, connector, format='B'):

        # 1. Check connector type:
        # - if stratus net, then nothing to do
        # - if string name, then search for a net with this name
        from types import StringType
        if type(connector) is StringType:
            for net in self._cell._st_sigs:
                if (net._name == connector):
                    connector_ref = net
        else:
            connector_ref = connector
        
        # 2.write declaration to output file
        # 2.a write connector direction
        self._f.write(self._get_direction(connector_ref).rjust(5)
                      + " " + connector_ref._name)
        # 2.b write: "N downto 0"  if necessary
        if(connector_ref._arity != 1):
            self._f.write(" (" + str(connector_ref._arity-1) + " downto 0)")

        # 2.c write format
        if ((format == 'B') or (connector_ref._arity == 1)):
            self._f.write(" B;;\n")
        elif format == 'X':
            self._f.write(" X;;\n")
        elif format == 'O':
            self._f.write(" O;;\n")
        
        # 3. add connector to list (we need an ordered connectors lists)
        self.connectors_list.append(connector_ref._name)
        self.connectors_format[connector_ref._name] = format
        
        # 4. if output connector or signal set value to any
        if ((self._get_direction(connector_ref) == 'out') or
            (self._get_direction(connector_ref) == 'signal')):
            self.affect_any(connector_ref)

    # ------------------------------------------------------
    ## Subcomponents internal signals declaration
    #
    # Imported vst signals aren't in the database. Hence we can't
    # easily import data. So the designer has to set all the parameter
    # with this command to allow pattern check of subcomponents
    # internal signals values.
    #
    # @param self object reference
    # @param signame string with the full signal name eg:
    # <i>instance0_modpmfs8_fsm.chip_ep</i>
    # @param msb signal msb, default value 0
    # @param lsb signal lsb, default value 0
    # @param format optional, default value B, repport
    # declar_interface documentation
    def declar_subsig(self, signame, msb=0, lsb=0, format='B'):
        self.connectors_list.append(signame)
        self.connectors_format[signame] = format
        
        if msb == lsb :
            self._f.write('signal %s %s;;\n' % \
                              (signame, format))
        else:
            self._f.write('signal %s (%d downto %d) %s;;\n' % \
                              (signame, msb, lsb, format))

        # create output value
        any_str = ''
        arity = msb-lsb+1
        if format == 'B':
            any_str += arity*'*'
        elif format == 'O':
            any_str += int(ceil(arity/3.))*'*'
        elif format == 'X':
            any_str += int(ceil(arity/4.))*'*'

        # Update connectors values dictionary
        self.connectors_current_values[signame] = any_str

        

    # ------------------------------------------------------
    ## Adds all the connectors from a Stratus model to the pattern 
    # interface.
    #
    # Write the corresponding connector declaration in the pattern
    # file with name, arity and direction directly taken from the
    # connector proprieties.
    #
    # Also add the connector to the connectors_list. The connector
    # list is used to store the connectors order. It's used later when
    # we'll write the patterns.
    #
    # @remark Declarare in the following order: clock, input, output, inout
    # vdd, vss. The fixed connector order is a constraint for Synopsys
    # testbench generation.
    # 
    # @remark The only supported connectors are instance of SignalIn,
    # SignalOut, SignalInOut, VddIn, VssIn and CkIn.
    #
    # @remark If the connector arity is greater than 1 it will always
    # be written in the form <code>N downto 0</code>.
    #
    # @param self The object pointer
    # @param cell The tested Stratus model reference. Optional if a
    # reference to the tested Stratus model was given during
    # instanciation (patterns.patwrite.PatWrite.__init__ cell parameter).
    # @param format Optional format for the connectors values into the
    # pattern file, accepted values are :
    # - 'B': binary (default),
    # - 'X': hexadecimal,
    # - 'O': octal.
    #
    # @remark if the conector arity is 1 the format is 'B'
    # @todo use get_interface simplify
    def declar_interface(self, cell=None, format='B'):
        #
        if (cell==None):
            if self._cell == None:
                raise Exception("PatWrite.declar_interface() "
                    "cell reference no given during PatWrite instanciation,"
                        " thus the cell parameter is mandatory.")
            else:
                cell = self._cell

        # 1. Clocks
        for port in cell._st_cks:
            if isinstance(port, CkIn):
                self.declar(port, format)

        # 2. Inputs
        for port in cell._st_ports:
            if isinstance(port, SignalIn):
                self.declar(port, format)

        # 3. Outputs
        for port in cell._st_ports:
            if isinstance(port, SignalOut):
                self.declar(port, format)

        # 4. InOuts
        for port in cell._st_ports:
            if isinstance(port, SignalInOut):
                self.declar(port, format)

        # 5. VDDs
        for port in cell._st_vdds:
            if isinstance(port, VddIn):
                self.declar(port, format)

        # 6. VSSs
        for port in cell._st_vsss:
            if isinstance(port, VssIn):                
                self.declar(port, format)

        # 7. TriState
        for port in cell._st_ports:
            if isinstance(port, TriState):
                self.declar(port, format)

    ## Return interface connectors dictionnary where key is the conector's name
    #  and value the connector's format ('B' or 'X' or 'O')
    #
    # @param name is the tested Stratus model reference name
    # @return connectors dictionnary
    def get_interface_format(self, name):
        file = open(name+'.pat', 'r')
        dict = {}
        for line in file :
            # stop at end of the declaration
            if re.compile(r"^begin$", re.I).search(line):
                break;
            # skip blank line
            if re.compile(r"^ *$", re.I).search(line):
                continue
            # get name and format
            res = re.search("[in|out]+ *(\w*).*([X|B|O]) *;*",line).group(1,2)
            dict[res[0]] = res[1]
        file.close()
        return dict


    ## Return interface connectors list
    #
    # Construct an ordered connectors list corresponding to
    # declare_interface() order.
    #
    # @param cell tested Stratus model reference. Optional if a
    # reference to the tested Stratus model was given during
    # instanciation (patterns.patwrite.PatWrite.__init__ cell parameter).
    # @return connectors list
    def get_interface(self, cell=None):
        l = []

        if (cell==None):
            if self._cell == None:
                raise Exception("PatWrite.get_interface() "
                    "cell reference no given during PatWrite instanciation,"
                        " thus the cell parameter is mandatory.")
            else:
                cell = self._cell

        # 1. Clocks
        for port in cell._st_cks:
            if isinstance(port, CkIn):
                port._format = self.connectors_format[port._name]
                l.append(port)

        # 2. Inputs
        for port in cell._st_ports:
            if isinstance(port, SignalIn):
                port._format = self.connectors_format[port._name]
                l.append(port)

        # 3. Outputs
        for port in cell._st_ports:
            if isinstance(port, SignalOut):
                port._format = self.connectors_format[port._name]
                l.append(port)

        # 4. InOuts
        for port in cell._st_ports:
            if isinstance(port, SignalInOut):
                port._format = self.connectors_format[port._name]
                l.append(port)

        # 5. VDDs
        for port in cell._st_vdds:
            if isinstance(port, VddIn):
                port._format = self.connectors_format[port._name]
                l.append(port)

        # 6. VSSs
        for port in cell._st_vsss:
            if isinstance(port, VssIn):
                port._format = self.connectors_format[port._name]
                l.append(port)

        return l


    # ------------------------------------------------------
    ## Affect a string value to a connector
    #
    # Update the connectors_current_values dictionary with value.
    #
    # @param connector Stratus connector
    # @param value String to affect to connector
    def affect(self, connector, value):
        self.connectors_current_values[connector._name] = value

    # ------------------------------------------------------
    ## Affect an integer (CA2) value to a connector
    #
    # Convert the 2's complement value to the corresponding binary
    # value. The binary size is taken from the connector arity. Then,
    # update the connectors_current_values dictionary with the
    # computed binary value.
    #
    # If the connector is an output, the binary value is preceded by
    # "?".
    #
    # @param connector Stratus connector.
    # @param value 2's complement value to affect to the connector.
    # @todo add code for octal format and execption ...
    def affect_int(self, connector, value):
        # Test if the connector is a connector
        if str(connector.__class__ ) not in ( "st_net.SignalIn", "st_net.SignalOut", "st_net.SignalInOut", "st_net.SignalUnknown", "st_net.TriState", "st_net.VddIn", "st_net.VssIn", "st_net.CkIn" ) :
            raise Exception("PatWrite.affect_int() the first argument must be an external net.")
        
        # 1.Test if output connector
        if self._is_output(connector):
            tmp = "?"
        else:
            tmp =""

        # 2. Get connector format
        format = self.connectors_format[connector._name]

        # 3. Create output string
        if format == 'B':
            tmp += int2str(value, connector._arity)
        elif format == 'O':
            pass
        elif format == 'X':
            tmp += int2strhex(value, connector._arity)

        # 4. Update connector value
        self.connectors_current_values[connector._name] = tmp



    # ------------------------------------------------------
    ## Affect a fixed point value to a connector.
    #
    # Convert the floating point input value to the corresponding
    # fixed point value with word_length=connector.arity() and
    # integer_word_length=iwl.
    #
    # If the connector is an output, the binary value is preceded by
    # "?".
    #
    # @param connector Stratus connector.
    # @param value floating point value to convert and asign to connector.
    # @param iwl integer word length
    def affect_fix(self, connector, value, iwl):
        # 1.Test if output connector
        if self._is_output(connector):
            tmp = "?"
        else:
            tmp =""

        # 2.Finish to compute binary string and update connectors
        # values dictionary.
        self.connectors_current_values[connector._name] = \
          tmp+float2strfix(value, connector._arity, iwl)

    # ------------------------------------------------------
    ## Disable comparison between this connector value and the one
    # calculated during simulation.
    #
    # @param connector Stratus connector.
    #
    # @remark We systematicaly write the optional "?" mark before the
    # "*" symbols. This is to allow the Synopsys module generated
    # testbench to correctly detect output values. With this all the
    # output values are begin with "?".
    #
    # @todo add exception if unknown type
    def affect_any(self, connector):
        # search connector format
        format = self.connectors_format[connector._name]

        # create output value
        any_str = '?'
        if format == 'B':
            any_str += connector._arity*'*'
        elif format == 'O':
            any_str += int(ceil(connector._arity/3.))*'*'
        elif format == 'X':
            any_str += int(ceil(connector._arity/4.))*'*'

        
        # Update connectors values dictionary
        self.connectors_current_values[connector._name] = any_str



    # ------------------------------------------------------
    ## Adds a pattern
    #
    # Add the pattern with @e current_time time and
    # @e connectors_current_values values. Then increment
    # @e current_time by @e time_interval.
    def addpat(self):
        # write time and comment
        self._f.write("< " + str(self.current_time).rjust(10) +
                      " " + self.time_unit + ">" + " : ")
        # write connectors values
        for connector in self.connectors_list:
            if connector not in self.connectors_current_values :
                raise Exception("PatWrite.addpat() : connector %s is missing." % connector)
            self._f.write(self.connectors_current_values[connector] + " ")
        # end pattern
        self._f.write(";\n")
        
        # increment time for next pattern
        self.current_time += self.time_interval

    # ------------------------------------------------------
    ## Mark the end of the interface declaration and the beginning of
    # the test vectors.
    def pattern_begin(self):
        self._f.write("\nbegin\n\n")

    # ------------------------------------------------------
    ## Mark the end of the test vectors and of the patterns file.
    def pattern_end(self):
        self._f.write("\nend;\n")
        self._f.close()
        self._finished = True

    
    # ------------------------------------------------------
    ## Automatically create the pattern values for arithmetic function
    # of 2 inputs 
    #
    # @param cell is the tested Stratus model
    # @param val_mask is the value of the MaskNumber
    # @param function is the arithmetic function to test defined as a python lambda
    # function of (a,b) operands (ex: function = lambda a,b : a+b)
    # @param mode is the mode for the patterns :
    # - 'zd' : zero delay simulation
    # - 'delay' : the current pattern affects the inputs and the next the output
    # - 'dense delay' : the current pattern affects the inputs and the output of the last pattern
    #
    # @remark this function can be used only if the cell's ports are :
    # - i0 corresponding to the 1st operand signal
    # - i1 corresponding to the 2nd operand signal
    # - o corresponding to the output signal
    # - cin corresponding to the input carry signal (optionnal)
    # - cout corresponding to the output carry signal (optionnal)
    def pat_function(self, cell, val_mask, function, mode = 'zd'):
        res = res_cout = None
        if 'cin' not in cell._param : cin = False
        else :                        cin = param['cin']
        if 'cout' not in cell._param : cout = False
        else :                         cout = param['cout']

        # extends the sign of a given value from width1 to width2
        def extend(val,width):
           if val >= 2**(width-1):
              mask = ~(2**width-1)
              return (val | mask)
           else:
              return val
        # cout calculation
        def cout_cal(valA,valB,valCin):
           mask = 2**cell._nbit - 1
           val = (valA & mask) + (valB & mask) + valCin
           if val >= 2**cell._nbit:
              return 1
           else:
              return 0
           
        # Affectation
        self.affect_int(cell._st_vdds[0], 1)
        self.affect_int(cell._st_vsss[0], 0)
        # use of a MaskNumber value of val_mask for the 2 input
        valuesA = MaskNumber(cell.i0._arity,val_mask)
        for valA in valuesA:
            valuesB = MaskNumber(cell.i1._arity,val_mask)
            for valB in valuesB:
                self.affect_int(cell.i0,valA)
                self.affect_int(cell.i1,valB)
                # take account of sign extension
                if cell._signed :
                  valAe = extend(valA,cell.i0._arity)
                  valBe = extend(valB,cell.i1._arity)
                else:
                  valAe = valA
                  valBe = valB
                if cin:
                   for valCin in (0,1):
                       self.affect_int(cell.cin,valCin)
                       # the output is affected with the parameter function
                       res_old = res
                       res = function(valAe,valBe)+valCin
                       res_cout_old = res_cout
                       res_cout = cout_cal(valAe,valBe,valCin)
                       if mode == 'zd' :
                           self.affect_int(cell.o,res)
                           if cout:
                              self.affect_int(cell.cout,res_cout)
                       elif mode == 'delay' :
                           self.affect_any(cell.o)
                           if cout:
                              self.affect_any(cell.cout)
                           self.addpat()
                           self.affect_int(cell.o,res)
                           if cout:
                              self.affect_int(cell.cout,res_cout)
                       elif mode == 'dense delay' :
                           if res_old == None :
                              self.affect_any(cell.o)
                           else:
                              self.affect_int(cell.o,res_old)
                           if cout:
                              if res_cout_old == None :
                                 self.affect_any(cell.cout)
                              else:
                                 self.affect_int(cell.cout,res_cout_old)
                       self.addpat()
                else:
                    # the output is affected with the parameter function
                    res = function(valAe,valBe)
                    self.affect_int(cell.o,res)
                    if cout:
                        self.affect_int(cell.cout,cout_cal(valA,valB,0))
                    self.addpat()


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

# ------------------------------------------------------
# $Id: utils.py,v 1.3 2009/11/18 15:35:02 cobell Exp $
# Author: LIP6/ASIM ludovic.noury@lip6.fr
# Contributor: Roselyne.Chotin-Avot@lip6.fr
# ------------------------------------------------------
__all__ = ['strfix2float', 'str2int', 'str2uint', 'int2str', 'int2strhex',
           'float2strfix', 'runpat', 'LoadSig', 'MaskNumber', 'runsynop',
           'cout_cal']

import math
import re
from math import ceil
from os import system
from stratus import *

# ------------------------------------------------------
## @defgroup patterns_utils patterns.utils
# Module: conversion functions
#
# @todo add parameters checks everywhere ... 
#
# @version $Id: utils.py,v 1.3 2009/11/18 15:35:02 cobell Exp $

# ------------------------------------------------------
## Convert string to int list (with LSB at res[0])
#
# @param val input string
# @return corresponding list
#
# @ingroup patterns_utils
def _str2list(val):
    binlist = list(val)
    binlist.reverse()
    binlist = map(int, binlist)
    return binlist

# ------------------------------------------------------
## Convert the binary representation of a CA2 signed fixed point
# number to the corresponding floating point number.
#
# @param val input string
# @param iwl integer word length
# @return corresponding floating point value
# @todo make a function of "1" as we also use it for PatRead ?
# @remark iwl includes sign bit.
#
# @ingroup patterns_utils
def strfix2float(val, iwl):
    # 1. Convert string to int list (with LSB at binlist[0])
    binlist = _str2list(val)
    
    # 2. Get word length & fractional word length
    wl = len(binlist)
    fwl = wl-iwl
    
    # 3. Compute
    res = -(2**(iwl-1)) * binlist[wl-1]
    for i in range(wl-2, -1, -1):
        res += binlist[i]*(2**(i-fwl))

    # 4. Return
    return res

# ------------------------------------------------------
## Convert the binary representation of a CA2 signed fixed point
# number to the corresponding int number.
#
# @remark Warning ! works only on signed numbers !!
#
# @param val input string
# @return corresponding int value
#
# @ingroup patterns_utils
def str2int(val):
    # 1. Convert string to int list (with LSB at binlist[0])
    binlist = _str2list(val)

    # 2. Get word length
    wl = len(binlist)

    # 3. Compute
    res = -pow(2,wl-1) * binlist[wl-1]
    for i in range(wl-1):
        res += binlist[i]*(2**i)

    # 4. Return
    return res

# ------------------------------------------------------
## Convert the binary representation of a CA2 unsigned fixed point
# number to the corresponding int number.
#
# @param val input string
# @return corresponding int value
#
# @ingroup patterns_utils
def str2uint(val):
    # 1. Convert string to int list (with LSB at binlist[0])
    binlist = _str2list(val)

    # 2. Get word length
    wl = len(binlist)

    # 3. Compute
    res = 0
    for i in range(wl):
        res += binlist[i]*(2**i)

    # 4. Return
    return res


# ------------------------------------------------------
## Convert an int value to the corresponding 2's complement binary
# string value on wl bits.
#
# We suppose that the machine representation of "value" is in 2's
# complement. Hence we just decode it with binary operations and
# put the result into a string.
#
# @param value int value to convert
# @param wl size of the binary output string : word length
#
# @ingroup patterns_utils
def int2str(value, wl):
    res = ""
    for pos in range(wl):
        res = str( (value & 1<<pos) >>pos) + res
    return res

# ------------------------------------------------------
## Convert an int on wl bit to the corresponding hexadecimal
# representation string.
#
# @param value int value to convert
# @param wl input value word size
#
# @ingroup patterns_utils
def int2strhex(value, wl):
    mask = 2**wl-1
    format = '%%0%dx' %int(ceil(wl/4.))
    return format %(value & mask)


# ------------------------------------------------------
## Convert a float value to the corresponding signed fixed point
# string on wl bit and with an integer word size of iwl bits.
#
# @param value int value to convert
# @param wl word length
# @param iwl interger word length (including sign bit)
#
# @ingroup patterns_utils
def float2strfix(value, wl, iwl):
    # Put into an int corresponding fixed point number
    fixed_value = int(value*2**(wl-iwl))

    # Convert the number to string
    return int2str(fixed_value, wl)

# ------------------------------------------------------
## Pattern simulation with asimut
#
# Run netlist simulation with Asimut using the following parameters:
# - options (see option parameter),
# - netlist filename: \e name.vst,
# - pattern filename: \e name.pat,
# - results filename: \e name_res.pat .
#
# @param name netlist name,
# @param options asimut command line options (optional, default empty),
# @param stdout asimut stout (optional, default False),
# @param stderr asimut stderr (optional, default True).
# @return returns asimut exit value.
# @todo Add exception for simulation error.
#
# @ingroup patterns_utils
def runpat(name_vst, name_pat, options='', stdout=False, stderr=True):
    cmd_str = ('asimut %s %s %s %s') % (options, name_vst, name_pat, name_pat+'_res')

    if not stdout:
        cmd_str += ' > /dev/null'
    if not stderr:
        cmd_str += ' 2> /dev/null'

    result = system(cmd_str)
    if not result : print( "Simulation OK" )

    return result

## Return interface connectors dictionnary where key is the conector's name
#  and value the connector's format ('B' or 'X' or 'O')
#
# @param name is the tested Stratus model reference name
# @return connectors dictionnary
def get_interface_format(name):
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
# @param cell tested Stratus model reference
# @return connectors list
#
# @ingroup patterns_utils
def get_interface(cell):
    l = []

    dict = get_interface_format(cell._name)

    if (cell==None):
         raise Exception("get_interface() "
             "cell reference no given thus the cell parameter is mandatory.")

    # 1. Clocks
    for port in cell._st_cks:
        if isinstance(port, CkIn):
            port._format = dict[port._name]
            l.append(port)

    # 2. Inputs
    for port in cell._st_ports:
        if isinstance(port, SignalIn):
            port._format = dict[port._name]
            l.append(port)

    # 3. Outputs
    for port in cell._st_ports:
        if isinstance(port, SignalOut):
            port._format = dict[port._name]
            l.append(port)

    # 4. VDDs
    for port in cell._st_vdds:
        if isinstance(port, VddIn):
            port._format = dict[port._name]
            l.append(port)

    # 5. VSSs
    for port in cell._st_vsss:
        if isinstance(port, VssIn):
            port._format = dict[port._name]
            l.append(port)

    return l

# ------------------------------------------------------
## Pattern simulation with synopsys
#
# @param cell tested Stratus model reference
# @param vasy if vhdl is obtained from Alliance vst netlist (optional, default False)
# @param time_interval between pattern (optional, default 20)
# @param time_unit for simulation, valid values are ps, ns, us and ms (optional, default 'ns')
# @param debug informations during compilation and simulation (optional, default False)
# @param downto to have vector in MSB to LSB format (optional, default True)
# @param logic to have a time_interval delay between inputs and outputs affectations for logical simulation (optional, default False)
#
# @ingroup patterns_utils
def runsynop(cell, vasy = False, time_interval = 20, time_unit = 'ns', debug = False, downto = True, logic = False):
   from synopsys import Synopsys
   # Convert to VHDL
   syn = Synopsys(cell)
   if vasy :
      syn.convert_to_vhdl()

   # Create testbench
   syn.create_testbench(get_interface(cell), time_interval, time_unit, debug, downto, logic)

   # Create testbench and block instance
   syn.create_run(get_interface(cell), debug, downto)

   # Compile
   syn.compile(debug=True)

   # Run simulation
   syn.run()

   del syn

# ------------------------------------------------------
## Create a samples iterator from an input file
#
# Return a iterator from the specified input signal. The iterator
# output format can be :
# - LoadSig.FIX the corresponding fixed point number converted back
#   to float,
# - LoadSig.STRING the binary representation of the fixed point number
#   into a string,
# - LoadSig.FLOAT the original floating point number without any
#   conversion.
# - LoadSig.INT the rounded int number
#
# @remark the input file has one floating point number per line,
# comments begin with #
#
# @remark Python don't support multiple constructors without hack, so
# we prefer not to use them.
# (see: http://mail.python.org/pipermail/python-list/2005-May/280116.html)
#
# @ingroup patterns_utils
class LoadSig:
    # Class attributes -------------------------------------
    ## Used a constructor parameter, select fixed point output
    FIX = 0
    ## Used a constructor parameter, select binary string output
    STRING = 1
    ## Used a constructor parameter, select unconverted float output
    FLOAT = 2
    ## Used a constructor parameter, select rounded int output
    INT = 3

    # Private attributes -----------------------------------
    # @var _f    file object
    # @var _it   input file object iterator
    # @var _wl   word length
    # @var _iwl  input word length
    # @var _mode output format (FIX|STRING|FLOAT|INT)

    # ------------------------------------------------------
    ## Constructor
    #
    # @param filename  input filename,
    # @param connector stratus connector,
    # @param iwl       integer word length,
    # @param mode      set iterator output format (LoadSig.FIX |
    # LoadSig.STRING | LoadSig.FLOAT | LoadSig.INT).
    def __init__(self, filename, connector, iwl, mode):
        self._f = open(filename, 'r')
        self._it = iter(self._f)
        self._wl = connector._arity
        self._iwl = iwl
        self._mode = mode

    # ------------------------------------------------------
    # Activate Iterator 
    def __iter__(self):
        return self

    # ------------------------------------------------------
    # Subfonction used by next(), convert a line to the selected output
    # format.
    def _make_output(self,line):
        r = float2strfix(float(line), self._wl, self._iwl)

        if self._mode == self.STRING:
            return r
        elif self._mode == self.FIX:
            return strfix2float(r , self._iwl)
        elif self._mode == self.FLOAT:
            return float(line)
        elif self._mode == self.INT:
            return int(round(float(line)))
        
    # ------------------------------------------------------
    ## Return next sample
    def next(self):
        line = self._it.next()
        while re.compile("^#").search(line):
            line = self._it.next()
        return self._make_output(line)

# -------------------------------------------------------------------------
## Create an iterator for a pattern input signal values.
#
# The iterator will return at each call the next input signal value in
# the form of the corresponding unsigned int. The output value
# sequence depends on the input signal word length (wl):
# - wl<MAX_IT : exhaustive generation of all possible signal values,
# - wl>=MAX_IT: use Pattern Mask Generator (PMG) to reduce the number
#   of values.
#
# The PMG splits the number into 3 parts:
# - a middle fixed part where all bits have the same value (0 or 1),
# - 2 varying parts (called masks, one for msb and one for lsb) with
#   the same fixed width.
#
# At each call, the iterator :
# -# increments the masks to obtain all the possible values,
# -# add these 2 masks around the middle part to obtain the final
#   value,
# -# returns the final value.
#
# @todo Add constructor parameters check (raise exception if
# width<=mask_width).
# @todo Check if there is no errors during << witch could be corrected
# by using long int.
#
# @ingroup patterns_utils
class MaskNumber:
    # Class attributes -----------------------------------------------
    ## Maximal signal width for exhaustive generation
    MAX_IT = 9

    # Private instance attributes ------------------------------------
    ## @var _mask_value
    # Varying part value.

    ## @var _middle
    # Central part value.

    ## @var _currrent
    # Current value of the number.

    ## @var _mask_width
    # Mask width.

    ## @var _width
    # Width of the signal to affect.

    ## @var _exhaustiv
    # If False force using PMG even if wl<MAX_IT

    ## @var _mask_max_value
    # Maximal mask value.

    ## @var _mask_lsb
    # Lsb mask.
	
    ## @var _mask_msb
    # Msb mask.

    ## @var _dec_val
    # The value to decal the mask in the msb

    # ----------------------------------------------------------------
    ## Constructor
    #
    # @param self The object pointer.
    # @param width The width of the signal to affect.
    # @param mask_width The width of a varying part.
    # @param exhaustiv If False force using PMG even if wl<MAX_IT.
    #
    # @todo enhance __init__ exception 
    def __init__(self, width, mask_width, exhaustiv=True):
        self._mask_value = -1
        self._middle = 0
        self._current = -1
        self._mask_width = mask_width
        self._width = width
        self._exhaustiv = exhaustiv

        # Exhaustive test ---------------------------------------
        if (width < self.MAX_IT) and (exhaustiv):
            self._mask_max_value = 2**width - 1
        # With mask ---------------------------------------------
        else:
            if (mask_width*2) >= width:
                raise Exception('MaskNumber.__init__: '
                    'mask_width(%d)*2 >= width(%d)' % (mask_width, width))
            self._mask_max_value = 2**(2*mask_width) - 1
                        
        self._mask_lsb = 2**mask_width-1
        self._mask_msb = (2**mask_width-1) << mask_width 
        self._dec_val = self._width - 2 * self._mask_width

    # ----------------------------------------------------------------
    # Activate Iterator
    def __iter__(self):
        return self

    # ----------------------------------------------------------------
    def _next_exhaustive(self):
        if self._mask_value == self._mask_max_value:
            # stop generation at last value
            raise StopIteration
        else:
            # increment each value by 1
            self._mask_value += 1
            self._current = self._mask_value
            return self._mask_value

    # ----------------------------------------------------------------
    def _next_pmg(self):
        if self._mask_value == self._mask_max_value:
            if self._middle != 0:
                # Stop generation when all the mask value are obtained
                # for both value (0 and 1) of the central part.
                raise StopIteration
            else:
                # When all the mask value are obtained for the 0 central value
                # we pass to the 1 central value.
                self._mask_value = -1
                self._middle = 2**self._width - 1
                self._middle = (self._middle ^
                    ((self._mask_msb * 2**self._dec_val) + self._mask_lsb))
        value = 0
        # 2.a Adds 1 to the mask value
        self._mask_value = self._mask_value + 1
        # 2.b Constructs msb and lsb parts of the mask value
        lsb = self._mask_value & self._mask_lsb
        msb = (self._mask_value & self._mask_msb) * 2**self._dec_val
        # 2.c Place msb and lsb around central part
        value = msb + self._middle + lsb
        # 2.d Return the obtained value
        self._current = value
        return value
        
    # ----------------------------------------------------------------
    ## Return next sample
    #
    # @param self The object pointer.
    # @return next sample.
    def next(self):
        # Signal width inferior to MAX_IT => exhaustive pattern generation.
        # else we use PMG.
        if (self._width < self.MAX_IT) and (self._exhaustiv):
            return self._next_exhaustive()
        else:
            return self._next_pmg()

    # ----------------------------------------------------------------
    ## Reset the generator to the first value
    #
    # @param self The object pointer.
    def reset(self):
        self._mask_value = -1
        self._middle = 0
        self._current = -1

    # ----------------------------------------------------------------
    ## Return current sample
    #
    # @param self The object pointer.
    # @return current sample.
    def current(self):
        return self._current

# ------------------------------------------------------
## Extend a given signed number
# @param val is the number to extend
# @param width is the width in bits of the number
# @return the extended value
def extend(val,width):
   # if the number is negative extend the sign ('1')
   if val >= 2**(width-1):
      mask = ~(2**width-1)
      return (val | mask)
   # else return the number
   else:
      return val

# ------------------------------------------------------
## Calculation of the cout's value in case of addition
# @param width is the width in bits of the number
# @param vali0 is the value of the first operand
# @param vali1 is the value of the second operand
# @param valCin is the value of the input carry
# @return the corresponding value of cout
def cout_cal(width,valA,valB,valCin):
   mask = 2**width - 1
   val = (valA & mask) + (valB & mask) + valCin
   if val >= 2**width:
      return 1
   else:
      return 0


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

# ----------------------------------------------------------------------------
# $Id: synopsys.py,v 1.6 2008/11/26 15:19:11 roselyne Exp $
# Author: LIP6/ASIM ludovic.noury@lip6.fr
# Contributor: Roselyne.Chotin-Avot@lip6.fr
# ----------------------------------------------------------------------------
import os
import re
from tempfile import TemporaryFile

from stratus import *


# ----------------------------------------------------------------------------
## Everything to simulate with Synopsys
#
# - export design to VHDL.
# - create testbench and run blocks
# - Synopsys compilaltion
# - Synopsys simulation
#
# See complete example into the example section
#
# @remark ".pat" connectors order is important, the call to patWrite
# instance to get it.
#
# @remark Currently all values must be in binary in the ".pat".
#
# @remark This is a first draft. This class should be edited a lot and
# maybe merged (or dervived from) with patwrite
#
# @todo signal errors to output pat like asimut does
# @todo separate debug to (verbose and debug)
# @bug description bloc copy to res has some missing caracters. Hack
# adds missing ';' ...
class Synopsys:

  # @var self._cell cell

  # -------------------------------------------------------------
  ## Constructor
  # @var cell reference to Stratus design cell
  def __init__(self, cell):
    self._cell = cell
    self._name = cell._name

  # -------------------------------------------------------------
  ## Convert the VST netlist to VHDL
  #
  # Uses VASY to convert from VST to VHDL, then remove VDD/VSS from
  # resulting VHD files. Optionaly rename fd1qll cells input clock
  # from 'ck' to 'cp' (st option).
  #
  # raise Exception if error
  # @param debug debug mode (default=False)
  # @param st must be set to True if we use ST CORE9GP standard cells
  # @bug as the function is generic and don't depend on synopsys it 
  # should be defined somewhere else. (But it depends on VASY)
  def convert_to_vhdl(self, debug=False, st=False):
    # Convert to VHDL
    if debug:
      print('VST to VHDL conversion ---------------'
            '--------------------------------------\n'
            ' - VASY (%s) ...'% self._name)
    if os.system('vasy -o -V -S -I vst -H %s %s 1> %s_vasy.log' %
                 (self._name, self._name, self._name)):
      raise Exception('VASY error (see "%s_vasy.log")'% self._name)
    vhdl = re.compile(r'\.vhd$')

    # Remove VDD/VSS and change CK to ST name
    if debug and st:
      print(' - remove vdd/vss connectors declarations and change registers\n'
            '   input connector name (ST mode) ...')
    elif debug:
      print(' - remove vdd/vss connectors declarations ...')
      
    for filename in os.listdir('./'):
      if vhdl.search(filename):
        self._remove_power(filename, False)   # false for no debug
        if st:
          self._change_ck2st(filename, False) # false for no debug
  
    if debug:
      print( ' - VHDL conversion done.' )

  # -------------------------------------------------------------
  ## Remove power supply connectors from a VHDL file.
  #
  # Overwrite input file. 
  #
  # @param filename VHDL input file name.
  # @param debug debug mode (default False).
  # @bug if instances names have vdd/vss in !!!
  def _remove_power(self, filename, debug=False):
    # STEP1: remove vdd/vss -------------------------------------
    input = open(filename, 'r')
    temp  = TemporaryFile('w+')
    alim = re.compile('.*(vdd|vss)+.*', re.IGNORECASE)
    removed = 0

    for line in input:
      if not alim.search(line):            
        temp.write(line)
      else:
        removed += 1

    if debug:
      print( ' - removed %d power supply declarations'%removed )

    input.close()

    # STEP2: remove ";\n)" and ",\n)" syntax errors -------------
    temp.seek(0)

    out = open(filename, 'w')

    comma = re.compile('.*\w+.*(;|,)')
    close = re.compile('^\s*\);')
    corrige = re.compile('(;|,)')
    corrected = 0

    previous = temp.next()
    for current in temp:
      if close.search(current) and comma.search(previous):
        out.write(corrige.sub('', previous))
        corrected += 1
      else:
        out.write(previous)        
      previous = current
    out.write(previous)

    if debug:
      print( ' - corrected %s syntax errors'%corrected )

    # END -------------------------------------------------------
    temp.close()
    input.close()
    out.close()

  # -------------------------------------------------------------
  ## Correct clocks names for ST CORE9GP fd1qll cells
  #
  # Change 'ck' connector name to 'cp'. Overwrite input file
  #
  # @param filename VHDL input filename.
  # @param debug Debug mode (default=False)
  def _change_ck2st(self, filename, debug=False):
    # STEP1: change CKs -----------------------------------------
    input = open(filename, 'r')
    temp  = TemporaryFile('w+')

    alim = re.compile('.*(vdd|vss)+.*', re.IGNORECASE)
    changed = 0

    reg_declare = re.compile('COMPONENT fd1qll')
    reg_inst    = re.compile(': fd1qll')
    reg_aff     = re.compile('ck =>')

    for line in input:
      if reg_declare.search(line):
        temp.write(line)
        temp.write(input.next())
        temp.write('  cp	: IN STD_LOGIC;\n')
        input.next()
        changed += 1
      elif reg_inst.search(line):
        temp.write(line)
        temp.write(input.next())
        line = input.next()
        temp.write(reg_aff.sub('cp =>', line))
        changed += 1
      else:
        temp.write(line)

    if debug:
      print( ' - changed %d ck'%changed )

    # STEP2: write changes to input file ------------------------
    temp.seek(0)
    input.close()
    input = open(filename, 'w')
    for line in temp:
      input.write(line)

    # END -------------------------------------------------------
    input.close()
    temp.close()

  # -------------------------------------------------------------
  def create_testbench(self, interface, delay, unit, debug=False, downto=False, logic=False):
    name = self._cell._name + '_tb'
    outfilename = self._cell._name + '_res.pat'
    infilename = self._cell._name + '.pat'
    tb = open('%s_tb.vhd'%self._cell._name, 'w')

    if debug:
      print('Testbench module ---------------------'
            '--------------------------------------\n'
            ' - module name    : %s.vhd\n'
            ' - patterns input : %s\n'
            ' - patterns output: %s' 
            % (name, infilename, outfilename))

    # Libraries ---------------------------------------------------
    tb.write('library ieee;\n'
             'use ieee.std_logic_1164.all;\n'
             'use ieee.std_logic_textio.all;\n'
             'use std.textio.all;\n\n')

    # Entity ------------------------------------------------------
    # Declar input, outputs and clocks. No support inout yet.
    tb.write('entity %s IS\n\n'
             '  port (\n' % name)
    for signal in self._cell._st_ports:
      # SignalIn
      if isinstance(signal, SignalIn):
        tb.write('    %s : out std_logic' % signal._name)
      # SignalOut
      elif isinstance(signal, SignalOut):
        tb.write('    %s : in std_logic' % signal._name)
      # SignalInOut (unsupported)
      elif isinstance(signal, SignalInOut):
        raise Exception('unsupported connector (SignalInOut: %s)' % signal._name)
      # Skip to next if not a IO
      else:
        continue
      # Bit vectors
      if signal._arity != 1:
        if downto :
          tb.write('_vector(%d downto 0)'% (signal._arity-1))
        else:
          tb.write('_vector(0 to %d)'% (signal._arity-1))
      tb.write(';\n')

    # CkIn
    for signal in self._cell._st_cks:
      tb.write('    %s : out std_logic;\n' % signal._name)

    # Remove last ';'
    tb.seek(-2,1)
    tb.write(');\n\n'
             'end %s;\n\n' % name)

    # Architecture ------------------------------------------------
    tb.write('architecture rtl of %s is\n\n' % name)

    # Procedures
    tb.write('  -- Skip whitespaces\n'
             '  procedure skip_spaces(l :inout line) is\n'
             '    variable c : character;\n'
             '  begin\n'
             '    loop\n'
             "      if l(1) = ' ' then\n"
             '        read(l, c);\n'
             '      else\n'
             '        exit;\n'
             '      end if;\n'
             '    end loop;\n'
             '  end skip_spaces;\n\n')

    tb.write('  -- Skip asterisks\n'
             '  procedure skip_asterisks(l :inout line) is\n'
             '    variable c : character;\n'
             '  begin\n'
             '    loop\n'
             "      if l(1) = '*' then\n"
             '        read(l, c);\n'
             '      else\n'
             '        exit;\n'
             '      end if;\n'
             '    end loop;\n'
             '  end skip_asterisks;\n\n')

    tb.write('  -- Check for end of declaration block\n'
             '  procedure is_begin(l : inout LINE; declaration_block : inout boolean) is\n'
             '  begin\n'
             "    if l(1)='b' and l(2)='e' and l(3)='g' and l(4)='i' and l(5)='n' then\n"
             '      declaration_block := false;\n'
             '    end if;\n'
             '  end is_begin;\n\n')


    tb.write('  -- change end_pat to true if end detected\n'
             '  procedure is_end (l : inout line; end_pat : inout boolean) is\n'
             '  begin\n'
             "    if l(1)='e' and l(2)='n' and l(3)='d' and l(4)=';' then\n"
             '      end_pat := true;\n'
             '   end if;\n'
             '  end is_end;\n\n')



    tb.write('begin -- rtl\n\n')

    # Variables
    tb.write('  process\n'
             '    -- VARIABLES\n'
             '    variable l                 : line; -- input line\n'
             '    variable lout              : line; -- output line\n'
             '    variable ldebug            : line; -- termial output line\n'
             '    variable empty_line        : line;\n'
             '    variable declaration_block : boolean := true;\n'
             '    variable end_of_patterns   : boolean := false;\n'
             '    variable char              : character;\n'
             '    file TTY_OUTPUT: TEXT open WRITE_MODE is "STD_OUTPUT";\n'
             )

    tb.write('    -- INPUT PATTERNS VARIABLES\n')
    in_list = []
    for c in interface:
      if isinstance(c, SignalIn) or isinstance(c, CkIn):
        in_list.append(c)
        if (c._format == 'X') and (c._arity % 4 != 0) and (c._arity != 1):
           tb.write('    variable v_%s_ext: std_logic' % c._name)
           if downto :
              tb.write('_vector(%d downto 0)'% (c._arity-1 + (4 - c._arity % 4)))
           else:
               tb.write('_vector(0 to %d)'% (c._arity-1) + (4 - c._arity % 4))
           tb.write(';\n')
        tb.write('    variable v_%s: std_logic' % c._name)
        if c._arity != 1:
          if downto :
            tb.write('_vector(%d downto 0)'% (c._arity-1))
          else:
            tb.write('_vector(0 to %d)'% (c._arity-1))
        tb.write(';\n')

    tb.write('    -- OUTPUT PATTERNS VARIABLES\n')
    out_list = []
    for c in interface:
      if isinstance(c, SignalOut):
        out_list.append(c)
        if (c._format == 'X') and (c._arity % 4 != 0) and (c._arity != 1):
           tb.write('    variable vpat_%s_ext: std_logic' % c._name)
           if downto :
               tb.write('_vector(%d downto 0)'% (c._arity-1 + (4 - c._arity % 4)))
           else:
               tb.write('_vector(0 to %d)'% (c._arity-1) + (4 - c._arity % 4))
           tb.write(';\n')
        tb.write('    variable vpat_%s: std_logic' % c._name)
        if c._arity != 1:
          if downto:
            tb.write('_vector(%d downto 0)'% (c._arity-1))
          else:
            tb.write('_vector(0 to %d)'% (c._arity-1))
        tb.write(';\n')
        if (c._format == 'X') and (c._arity % 4 != 0):
           tb.write('    variable v_%s_ext: std_logic' % c._name)
           if c._arity != 1:
             if downto :
               tb.write('_vector(%d downto 0)'% (c._arity-1 + (4 - c._arity % 4)))
             else:
               tb.write('_vector(0 to %d)'% (c._arity-1) + (4 - c._arity % 4))
           tb.write(';\n')
        tb.write('    variable v_%s   : std_logic' % c._name)
        if c._arity != 1:
          if downto:
            tb.write('_vector(%d downto 0)'% (c._arity-1))
          else:
            tb.write('_vector(0 to %d)'% (c._arity-1))
        tb.write(';\n')


    tb.write('    -- FILES\n'
             '    file outfile        : TEXT is OUT "%s"; -- input patterns\n'
             '    file infile         : TEXT is IN  "%s"; -- output patterns\n' %
             (outfilename, infilename))
    tb.write('  begin -- process\n')
    tb.write('    write(empty_line, string\'(""));')
    # Write header to pat
    tb.write('    -- Write Header\n'
             '    write(l, string\'("-- Synopsys VHDL simulation results"));\n'
             '    writeline(outfile, empty_line);\n\n')
    # MAIN LOOP
    tb.write('    -- main loop\n'
             '    while not endfile(infile) loop\n'
             '      readline(infile, l);\n')
    # Comments and empty lines
    tb.write('      ---------------------------------------------------------\n'
             '      -- Comments and empty lines\n'
             '      ---------------------------------------------------------\n'
             '      -- skip empty lines\n'
             '      if L\'length = 0 then\n'
             '        writeline(outfile, empty_line);\n'
             '        next;\n'
             '      end if;\n\n'

             "      -- Skip '--' comments\n"
             "      if (l(1) = '-') and (l(2) = '-') then\n"
             '        next;\n'
             '      end if;\n\n'

             "      -- Copy '#' comments to output file\n"
             "      if l(1) = '#' then\n"
             '        writeline(outfile, l);\n'
             '        next;\n'
             '      end if;\n')
    # Declaration block
    tb.write('      ---------------------------------------------------------\n'
             '      -- Declaration block\n'
             '      ---------------------------------------------------------\n'
             '      skip_spaces(l);\n'
             '      if declaration_block then\n'
             '        -- Check for end of declaration block and copy to output\n'
             '        is_begin(l, declaration_block);\n'
             '        -- Ugly hack : add mysteriously missing ending ";;"\n'
             '        if declaration_block then\n'
             '          write(l, string\'(";;"));\n'
             '        end if;\n'
             '        writeline(outfile, l);\n'             
             )

    # Description block
    # - time
    tb.write('      ---------------------------------------------------------\n'
             '      -- Description block\n'           
             '      ---------------------------------------------------------\n'
             "      elsif l(1) = '<' then -- lines begin with <\n"
             "        -- we don't copy time, instead we use simulation time\n"
             '        read(l, char);\n'
             "        while char /= '>' loop\n"
             '          read(l,char);\n'
             '        end loop;\n'
             '        write(lout, string\'("< "));\n'
             '        write(lout, now);\n'
             '        write(lout, string\'(" >"));\n\n'

             '        -- Copy label to outfile\n'
             '        read(l, char);\n'
             "        while char /= ':' loop\n"
             '          write(lout, char);\n'
             '          read(l,char);\n'          
             '        end loop;\n'
             '        write(lout, char);\n'
             "        write(lout, character'(' '));\n\n")
    # - read pattern values and affect them netlist
    tb.write('        -- read inputs values from connectors and write them to output\n')
    for c in in_list:
      n = c._name
      hex=''
      ext=''
      if ((c._format == 'X') and (c._arity > 1)):
         hex = 'h'
         if (c._arity % 4) != 0:
            ext = '_ext'
      #else:
      #   hex = ''
      tb.write('        -- %s\n'
               '        skip_spaces(l);\n'
               '        %sread(l, v_%s%s);\n'
               '        %swrite(lout, v_%s%s);\n'
               "        write(lout, ' ');\n" % (n, hex, n, ext, hex, n, ext))
      tb.write('        %s <= ' % n)

      if c._arity == 1:
        tb.write('v_%s;' % n)
      else:
        for b in range(c._arity-1, -1, -1):
          tb.write('v_%s%s(%d)&' % (n, ext, b))
        tb.seek(-1,1)
        tb.write('; -- to respect order in .pat\n')


    # - get output connectors values and write them to outfile
    # - delay beetween input and output
    if logic :
      tb.write('\n        -- delay\n'
               '        wait for %d %s;\n\n' % (delay, unit))
    tb.write('\n        --  get output connectors values and write them to outfile\n')
    for c in out_list:
      n = c._name
      hex=''
      ext=''
      if ((c._format == 'X') and (c._arity > 1)):
         hex = 'h'
         if (c._arity % 4) != 0:
            ext = '_ext'
      #else:
      #   hex = ''
      tb.write('        -- %s\n'
               '        v_%s := ' % (n, n))
      if c._arity == 1:
        tb.write('%s;' % n)
      else:
        for b in range(c._arity-1, -1, -1):
          tb.write('%s(%d)&' % (n, b))
        tb.seek(-1,1)
        tb.write(';  -- to respect order in .pat\n')

      tb.write('        write(lout, string\'("?"));\n'
               '        %swrite(lout, v_%s%s);\n'
               "        write(lout, character'(' '));\n"
               '        skip_spaces(l);\n'
               "        read(l, char); -- skip '?'\n"
               "        if l(1) /= '*' then\n"
               '          %sread(l, vpat_%s%s);\n' % (hex, n, ext, hex, n, ext))
      if (c._format == 'X') and ((c._arity % 4) != 0) and (c._arity > 1) :
         tb.write('          vpat_%s := vpat_%s_ext(vpat_%s\'range);\n' %(n, n, n))
      tb.write('          if vpat_%s /= v_%s then\n'
               '            write(ldebug, string\'("- at time="));\n'
               '            write(ldebug, now);\n'
               '            write(ldebug, string\'(" error: %s="));\n'
               '            %swrite(ldebug, v_%s%s);\n'
               '            write(ldebug, string\'(" (expected "));\n'
               '            %swrite(ldebug, vpat_%s%s);\n'
               '            write(ldebug, string\'(")"));\n'
               '            writeline(TTY_OUTPUT, ldebug);\n'
               '          end if;\n'
               '        end if;\n'
               '        skip_asterisks(l);\n'
               % (n, n , n, hex, n, ext, hex, n, ext))

    # - write vdd vss
    tb.write('\n        --  write vdd/vss\n'
             '        write(lout, string\'("1 0;"));\n')
    # - save line
    tb.write('\n        -- save line\n'
             '        writeline(outfile, lout);\n\n')

    # - delay beetween 2 patterns
    if not logic:
      tb.write('\n        -- delay\n'
               '        wait for %d %s;\n\n' % (delay, unit))

    # End of pattern
    tb.write('      ---------------------------------------------------------\n'
             '      -- End\n'
             '      ---------------------------------------------------------\n'
             '      else\n'
             '        is_end(l, end_of_patterns);\n'
             '        if end_of_patterns then\n'
             '          writeline(outfile, l);\n'
             '          assert false report "End of simulation."\n'
             '            severity note;\n'
             '          wait;\n'
             '        end if;\n'
             '      end if;\n'
             '    end loop;\n'
             '    ASSERT FALSE REPORT "End of simulation without end signal !"\n'
             '      severity ERROR;\n'
             '    wait;\n'
             '  end process;\n'
             '\nend rtl;\n')  
    tb.close()


    if debug:
      print(' - testbench successfully created')



  # ----------------------------------------------------------------
  ## Remove STCells empty vhd files
  #
  # To allow the use of ST macroblocks we declare them a ".vst".
  # As a consequence, VASY outputs .vhd files corresponding to these
  # blocks, which belonging to the library musn't be here a ".vhd".
  def remove_stcells_vhds(self, debug=False):
    stmacro = 'b2cr_65.vhd dphs9gp_72x12m4d4_h.vhd schmitc_65.vhd'

    if debug:
      print('Remove macroblocks ".vhd"-------------'
            '--------------------------------------\n'
            ' - block list: %s' % stmacro)

    os.system('rm -f %s'%stmacro)

    if debug:
      print(' - macroblocks vhds removed')


  # ----------------------------------------------------------------
  ## Create run block
  #
  # run block instanciate both design and testbench
  def create_run(self, interface, debug=False, downto=False):
    name = self._cell._name + '_run'
    run = open('%s.vhd' % name, 'w')

    if debug:
      print('Run module ---------------------------'
            '--------------------------------------\n'
            ' - module name    : %s.vhd' % name)

    # Libraries ---------------------------------------------------
    run.write('library ieee;\n'
              'use ieee.std_logic_1164.all;\n'
              'use ieee.std_logic_textio.all;\n'
              'use std.textio.all;\n\n')
    # Entity ------------------------------------------------------
    run.write('entity %s is\n'
              'end %s;\n\n' % (name, name))

    # Architecture ------------------------------------------------
    run.write('architecture rtl of %s is\n\n' % name)
    # - signals
    run.write('  -- SIGNALS\n')

    for c in interface:
      if (isinstance(c, SignalIn) or isinstance(c, SignalOut) or
          isinstance(c, CkIn)):
        run.write('  signal %s: std_logic' % c._name)
        if c._arity != 1:
          if downto:
            run.write('_vector(%d downto 0)'% (c._arity-1))
          else:
            run.write('_vector(0 to %d)'% (c._arity-1))
        run.write(';\n')

    # - components
    run.write('\n  -- COMPONENTS\n')

    run.write('  component %s\n'
              '    Port(\n' % self._cell._name)
    for c in interface:
      if isinstance(c, SignalIn) or isinstance(c, CkIn):
        run.write('      %s: in  std_logic' % c._name)
      elif isinstance(c, SignalOut):
        run.write('      %s: out std_logic' % c._name)
      else:
        continue # skip vdd/vss
      if c._arity != 1:
        if downto:
          run.write('_vector(%d downto 0)'% (c._arity-1))
        else:
          run.write('_vector(0 to %d)'% (c._arity-1))
      run.write(';\n')
    # Remove last ';'
    run.seek(-2,1)
    run.write(');\n'
              '  end component;\n\n')

    run.write('  component %s_tb\n'
              '    Port(\n' % self._cell._name)
    for c in interface:
      if isinstance(c, SignalIn) or isinstance(c, CkIn):
        run.write('      %s: out std_logic' % c._name)
      elif isinstance(c, SignalOut):
        run.write('      %s: in  std_logic' % c._name)
      else:
        continue # skip vdd/vss
      if c._arity != 1:
        if downto :
          run.write('_vector(%d downto 0)'% (c._arity-1))
        else:
          run.write('_vector(0 to %d)'% (c._arity-1))
      run.write(';\n')
    # Remove last ';'
    run.seek(-2,1)
    run.write(');\n'
              '  end component;\n\n')

    # - main
    run.write('begin\n\n'
              '  CHIP: %s\n'
              '    port map (' % self._cell._name)
    for c in interface:
      if (isinstance(c, SignalIn) or isinstance(c, SignalOut) or
          isinstance(c, CkIn)):
        run.write(c._name+', ')
    # remove last ', '
    run.seek(-2,1)
    run.write(');\n\n')

    run.write('  STIM: %s_tb\n'
              '    port map (' % self._cell._name)
    for c in interface:
      if (isinstance(c, SignalIn) or isinstance(c, SignalOut) or
          isinstance(c, CkIn)):
        run.write(c._name+', ')
    # remove last ', '
    run.seek(-2,1)
    run.write(');\n\n')


    run.write('end rtl;')
    run.close()

    if debug:
      print(' - run module successfully created')



  # ---------------------------------------------------------------
  ## Compile for Synopsys VHDL simulation
  def compile(self, debug=False):
    if debug:
      print('Compile design Synopsys simulator ----'
            '--------------------------------------\n'
            ' - vhdlan ...')      
    os.system('mkdir WORK')
    vhdl = re.compile(r'\.vhd$')
    for filename in os.listdir('./'):
      if vhdl.search(filename):
        #if debug:
        #  print('vhdlan: %s' % filename)
        if (os.system('vhdlan -nc -debug %s' % filename) != 0):
          raise Exception('vhdlan error: vhdlan failure on "%s" analysis or '
                          'Synopsys environment not set.' % filename)
    if debug:
      print(' - scs %s (ignore "sys_errlist" and "sys_nerr" warnings) ...'
            % self._name)
    os.system('scs -nc -debug %s_run' % self._name)

    if debug:
      print(' - simulator successfully created')

  # ---------------------------------------------------------------
  ## Run Synopsys simulation
  def run(self, debug=False):
    if debug:
      print('Executing simulation -----------------'
            '--------------------------------------')
    os.system("echo 'run; quit' | ./scsim -nc")
    if debug:
      print(' - end of simulation')
  

  # -------------------------------------------------------------------------
  ## If called after Inst.Save() and Inst.Pattern() will build a Synopsys
  # simulator
  #
  # 1 (optional) convert the netlist from vst to vhd through VASY
  #   (with "self.convert_to_vhdl")
  # 2 create vhdl module sending the patterns
  #   (with "self.create_testbench")
  # 3 create main vhdl which connect : the chip and the patterns block
  #   (with "self.create_run method")
  # 4 run the simulator compilation (withe "self.compile")
  # @remark Don't forget that you may need a "synopsys_sim.setup" file
  # to configure Synopsys for the target library.
  def build_simulator(self, debug=False, convert_vst=True, st=True):
      if convert_vst:
          self.convert_to_vhdl(st=st, debug=debug)

      if debug:
          print( ' - generate test bench and build simulator (Synopsys) ...' )
      try:
          self._cell.pat
      except AttributeError:
          # On plante car ne fait pas sens sans pattern
          raise Exception("build_simulator: paterns are mandatory")
      self.create_testbench(self._cell.pat.get_interface(),
                            self._cell.pat.time_interval,
                            self._cell.pat.time_unit)        
      self.create_run(self._cell.pat.get_interface())
      self.compile(debug=debug)

  # -------------------------------------------------------------------------
  ## Convert the netlist vst to  hierachical verilog
  #
  #
  # We consider that this method is called after module.Save(),
  # i.e. that the vst as been exported.
  #
  # 1. (optional) use VASY to convert from VST to VHDL 
  # 2. use Synopsys to convert from VHDL to VERILOG
  #
  # @remark this is very usefull to export stratus designs to Cadence
  # SOC Encounter.
  #
  # @remark Creates a file named 'convert_to_verilog.cmd' (name can be
  # changed with command_file parameter)
  #
  # @remark Create a directory named 'BG' (name can be changed with
  # the library_dir parameter)
  #
  # @remark Don't forget that you may need a ".synopsys_dc.setup" file
  # to configure Synopsys for the target library.
  #
  # @warning delete modulename_tb.vhd and modulename_run.vhd
  def convert_vhdl2verilog(self, debug=False,
                           command_file='convert_to_verilog.cmd',
                           library_dir='BG',
                           convert_vst=True,
                           uniquify=True,
                           st=False):
    # Debug messages
    if debug:
      print(' - converting vhdl to verilog (uses Synopsys, uniquify=%s) ...'
            %uniquify)
      
    # Optionnal vst2vhd conversion
    if convert_vst:
      self.convert_to_vhdl(st=st, debug=debug)

    # We don't want the test bench components into the output verilog
    os.system("rm %s_run.vhd %s_tb.vhd 2> /dev/null"%
              (self._name,self._name))

    # Write Synopsys command file -----------------------------------
    # First part
    command_file = 'convert_to_verilog.cmd'
    output = open(command_file, 'w')
    output.write('sh mkdir %s\n'
                 'define_design_lib %s -path ./%s\n'
                 'analyze -library %s -format vhdl {\n'%
                 (library_dir,library_dir,library_dir,library_dir))
    output.close()

    # Add the vhd files names to the conversion script
    # (as we use a shell command we need to close the command_file and
    # reopen it after)
    if (os.system("ls *.vhd >> %s" % command_file) != 0):
      raise Exception("error : no vhd files found (missing conversion ?)")

    # End
    output = open(command_file, 'a+')
    output.write('}\n'
                 'elaborate %s -architecture RTL -library %s\n'
                 %(self._name, library_dir))
    if uniquify:
      output.write('uniquify\n')
    output.write('write -hierarchy -format verilog -output %s.v\n'
                 'exit\n'%self._name)
    output.close()

    # Call synopsys to make the conversion --------------------------
    os.system('dc_shell -tcl_mode < %s' % command_file)

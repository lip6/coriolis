from stratus import *

## Create new stimuli
#
# This class is used to create stimuli for Stratus models. 
class Stimuli:

   # Public attributes doxydoc ----------------------------
   ## @var connectors
   # dictionary, key=connector_name, value=current value

   ## @var _cell
   # Reference to the input Stratus cell. Used to be able to use
   # design net by theirs string name. 
   
   # ------------------------------------------------------
   ## The constructor
   # @param cell Optional, reference to Stratus cell.
   def __init__(self, cell=None):
      # Instance attributes declaration
      self._connectors = {}        
      self._cell = cell
      cell._stim = self
      for port in cell._st_ports:
         self._connectors[port._name] = [port,'-']
      for port in cell._st_cks:
         self._connectors[port._name] = [port,'-']
      for port in cell._st_vdds:
          self._connectors[port._name] = [port,1]
      for port in cell._st_vsss:
          self._connectors[port._name] = [port,0]


   # ------------------------------------------------------
   ## Affect a value to a connector
   #
   # Update the connectors dictionary with value.
   #
   # @param connector Stratus connector
   # @param value to affect to connector
   def affect(self, connector, value):
      self._connectors[connector._name][1] = value

   # ------------------------------------------------------
   ## Adds a pattern
   #
   # Return the stimuli with @e connectors values. 
   def add(self):
      return self._connectors

   def create_stimuli_text_file(self):
      import st_config
      if st_config.format == 'vhd' :
         fic = open(self._cell._name+'.txt', 'w')
         for stim in self._cell.Stimuli():
            for val in stim.values():
               if not(isinstance(val[0],VddIn) or isinstance(val[0], VssIn)):
                  fic.write(str(val[1]) + " ")
            fic.write('\n')
         fic.close()
      elif st_config.format == 'vst' :
         pass

   def create_pat_file(self):
      from patwrite import PatWrite
      pat = PatWrite(self._cell._name+'.pat',self._cell)
      # pat file interface
      for port in self._connectors.values():
         pat.declar(port[0])
      pat.pattern_begin()

      # stimuli
      for stim in self._cell.Stimuli():
         # write time and comment
         pat._f.write("< " + str(pat.current_time).rjust(10) +
                      " " + pat.time_unit + ">" + " : ")
         for val in stim.values():
            strOut = ""
            if isinstance(val[0], SignalOut) or isinstance(val[0], SignalInOut): strOut = "?" 
            if val[1] == '-': strVal = '*'
            else : strVal = val[1]
            pat._f.write("%s%s " %(strOut,strVal))
         pat._f.write(";\n")

         # increment time for next pattern
         pat.current_time += pat.time_interval
            
      # end
      pat.pattern_end()
      del pat

   def create_testbench(self, delay, unit, debug=False, downto=False, logic=False):
     name = self._cell._name + '_tb'
     outfilename = self._cell._name + '_res.txt'
     infilename = self._cell._name + '.txt'
     var = ""
     varext = ""
     tb = open('%s_tb.vhd'%self._cell._name, 'w')

     if debug:
      print('Testbench module ---------------------'
           '--------------------------------------\n'
           ' - module name    : %s.vhd\n'
           ' - input file     : %s\n'
           ' - output file    : %s' 
           % (name, infilename, outfilename))

     # Libraries ---------------------------------------------------
     tb.write('library ieee;\n'
            'use ieee.std_logic_1164.all;\n'
            'use ieee.std_logic_textio.all;\n'
            'use std.textio.all;\n\n')

     # Entity ------------------------------------------------------
     # Declar input, outputs and clocks.
     tb.write('entity %s IS\n\n'
            '  port (\n' % name)

     for signal in self._connectors.values():
        vector = ""
        if isinstance(signal[0],VddIn) or isinstance(signal[0],VssIn):
           del self._connectors[signal[0]._name]
           continue
        # Bit vectors
        if signal[0]._arity != 1:
           if downto :
              vector = '_vector(%d downto 0)'% (signal[0]._arity-1)
           else:
              vector = '_vector(0 to %d)'% (signal[0]._arity-1)
        var += '    variable v_%s: std_logic%s;\n' % (signal[0]._name, vector)

        # CkIn
        if isinstance(signal[0], CkIn):
           tb.write('    %s : out std_logic;\n' % signal[0]._name)
        # SignalIn
        if isinstance(signal[0], SignalIn):
           tb.write('    %s : out std_logic%s;\n' % (signal[0]._name, vector))
        # SignalOut
        elif isinstance(signal[0], SignalOut):
           tb.write('    %s : in std_logic%s;\n' % (signal[0]._name, vector))
           var += '    variable vstim_%s: std_logic%s;\n' % (signal[0]._name, vector)
        # SignalInOut
        elif isinstance(signal[0], SignalInOut):
           tb.write('    %s : in std_logic%s;\n' % (signal[0]._name, vector))
           var += '    variable vstim_%s: std_logic%s;\n' % (signal[0]._name, vector)
        # Skip to next if not a IO
        else:
           continue

     # Remove last ';'
     tb.seek(-2,1)
     tb.write(');\n\n'
            'end %s;\n\n' % name)
     # Architecture ------------------------------------------------
     tb.write('architecture rtl of %s is\n\n' % name)
     tb.write('begin -- rtl\n\n')

     # Variables
     tb.write('  process\n'
            '    -- VARIABLES\n'
            '    variable l                 : line; -- input line\n'
            '    variable lout              : line; -- output line\n'
            '    variable ldebug            : line; -- termial output line\n'
            '    variable empty_line        : line;\n'
            '    variable char              : character;\n'
            '    file TTY_OUTPUT: TEXT open WRITE_MODE is "STD_OUTPUT";\n'
            )

     tb.write('    -- STIMULI VARIABLES\n')
     tb.write(var);
     if varext :
        tb.write(varext)

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
            '      readline(infile, l);')
     # Description block
     # - read pattern values and affect them netlist
     for n, c in self._connectors.iteritems():
        if c[0]._direct == 'IN':
          tb.write('      -- %s\n'
                  '      read(l, v_%s);\n'
                  '      write(lout, v_%s);\n'
                  "      write(lout, ' ');\n" % (n, n, n))
          tb.write('      %s <= ' % n)

          if c[0]._arity == 1:
             tb.write('v_%s;' % n)
          else:
             for b in range(c[0]._arity-1, -1, -1):
                tb.write('v_%s(%d)&' % (n, b))
             tb.seek(-1,1)
             tb.write('; -- to respect order in .pat\n')
          
        else :
           tb.write('      -- %s\n'
                   '      wait for 1 ps;\n'
                   '      v_%s := ' % (n, n))
           if c[0]._arity == 1:
              tb.write('%s;' % n)
           else:
              for b in range(c[0]._arity-1, -1, -1):
                 tb.write('%s(%d)&' % (n, b))
              tb.seek(-1,1)
              tb.write(';  -- to respect order in .pat\n')

           tb.write('      write(lout, v_%s);\n'
                   "      write(lout, character'(' '));\n"
                   '        read(l, vstim_%s);\n' % (n, n))
           tb.write('      if (vstim_%s /= v_%s) and (vstim_%s /= \'-\') then\n'
                   '        write(ldebug, string\'("- at time="));\n'
                   '        write(ldebug, now);\n'
                   '        write(ldebug, string\'(" error: %s="));\n'
                   '        write(ldebug, v_%s);\n'
                   '        write(ldebug, string\'(" (expected "));\n'
                   '        write(ldebug, vstim_%s);\n'
                   '        write(ldebug, string\'(")"));\n'
                   '        writeline(TTY_OUTPUT, ldebug);\n'
                   '      end if;'
                   % (n, n , n, n, n, n))

     # - save line
     tb.write('\n      -- save line\n'
            '      writeline(outfile, lout);\n\n')

     # - delay beetween 2 patterns
     if not logic:
        tb.write('\n      -- delay\n'
              '      wait for %d %s;\n\n' % (delay, unit))
     # End of pattern
     tb.write('    ---------------------------------------------------------\n'
            '    -- End\n'
            '    ---------------------------------------------------------\n'
            '    end loop;\n'
            '    ASSERT FALSE REPORT "End of simulation !"\n'
            '      severity ERROR;\n'
            '    wait;\n'
            '  end process;\n'
            '\nend rtl;\n')  
     tb.close()

     if debug:
        print(' - testbench successfully created')


   # ----------------------------------------------------------------
   ## Create run block
   #
   # run block instanciate both design and testbench
   def create_run(self, debug=False, downto=False):
      name = self._cell._name + '_run'
      run = open('%s.vhd' % name, 'w')
      compports = ""
      tbports = ""
      portmap = ""

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

      for c in self._cell._st_ports:
         # portmap
         portmap += '         %s => %s,\n' %(c._name, c._name)
         # vector
         vector = ""
         if c._arity != 1:
            if downto:
               vector = '_vector(%d downto 0)'% (c._arity-1)
            else:
               vector = '_vector(0 to %d)'% (c._arity-1)

         if isinstance(c, SignalIn) :
            compports += '      %s: in  std_logic%s;\n' % (c._name, vector)
            tbports += '      %s: out  std_logic%s;\n' % (c._name, vector)
         if isinstance(c, SignalOut) :
            compports += '      %s: out  std_logic%s;\n' % (c._name, vector)
            tbports += '      %s: in  std_logic%s;\n' % (c._name, vector)
         if isinstance(c, SignalInOut) :
            compports += '      %s: inout  std_logic%s;\n' % (c._name, vector)
            tbports += '      %s: in  std_logic%s;\n' % (c._name, vector)

         run.write('  signal %s: std_logic%s;\n' % (c._name, vector))

      for c in self._cell._st_cks:
         compports += '      %s: in  std_logic;\n' % c._name
         tbports += '      %s: out  std_logic;\n' % c._name
         portmap += '         %s => %s,\n' %(c._name, c._name)
         run.write('  signal %s: std_logic%s;\n' % (c._name, vector))

      # - components
      run.write('\n  -- COMPONENTS\n')

      run.write('  component %s\n'
              '    Port(\n' % self._cell._name)
      run.write(compports)
      # Remove last ';'
      run.seek(-2,1)
      run.write(');\n'
              '  end component;\n\n')

      run.write('  component %s_tb\n'
              '    Port(\n' % self._cell._name)
      run.write(tbports)
      # Remove last ';'
      run.seek(-2,1)
      run.write(');\n'
              '  end component;\n\n')

      # - main
      run.write('begin\n\n'
              '  CHIP: %s\n'
              '    port map (\n' % self._cell._name)
      run.write(portmap)
      # remove last ', '
      run.seek(-2,1)
      run.write(');\n\n')

      run.write('  STIM: %s_tb\n'
              '    port map (\n' % self._cell._name)
      run.write(portmap)
      # remove last ', '
      run.seek(-2,1)
      run.write(');\n\n')

      run.write('end rtl;')
      run.close()

      if debug:
         print(' - run module successfully created')




Description
===========

The patterns module of *Stratus* is a set of *Python* classes and
methods that allows a procedural description of input pattern file for
the logic simulator. The *Stratus* ``Pattern`` method produces a pattern
description file as output. The file generated by ``Pattern`` method is
in pat format, so IT IS STRONGLY RECOMMENDED TO SEE pat(5) manual BEFORE
TO USE IT.

Syntax
======

From a user point of view, ``Pattern`` method is a pattern description
language using all standard *Python* facilities. Here follows the
description of the ``Pattern`` method.
A pat format file can be divided in two parts : declaration and
description part.
The declaration part is the list of inputs, outputs, internal signals
and registers. Inputs are to be forced to a certain value and all the
others are to be observed during simulation.
The description part is a set of patterns, where each pattern defines
the value of inputs and outputs. The pattern number represents actually
the absolute time for the simulator.
Similarly, a ``Pattern`` method can be divided in two parts :
declaration and description part. Methods related to the declaration
must be called before any function related to the description part.

Declaration part
----------------

The first thing you should do in this part is to instantiate the class
``Patwrite`` to have access to all patterns declaration and description
methods. The constructor of this class take as parameters the name of
pattern output file and the *Stratus* cell that is described (see
``PatWrite`` [patwrite]).
Then, this part allows you to declare the inputs, the outputs, and
internal observing points (see ``declar``\ [declar] and
``declar_interface`` [declar:sub:`i`\ nterface]).

Description part
----------------

After all signals are declared, you can begin the description part (see
``pattern_begin`` [pattern:sub:`b`\ egin]). In this part you have to
define input values which are to be applied to the inputs of the circuit
or output values which are to be compare with the values produced during
the simulation. (see ``affect`` [affect], ``affect_any``
[affect:sub:`a`\ ny], ``affect_int`` [affect:sub:`i`\ nt] and
``affect_fix`` [affect:sub:`f`\ ix]). ``Pattern`` method describes the
stimulus by event : only signal transitions are described. After each
event there is a new input in the pattern file (see ``addpat``
[addpat]). Last thing you should do in this part is to generate the
output file (see ``pattern_end`` [pattern:sub:`e`\ nd]).

Methods
=======

PatWrite
--------

This class is used to create patterns for *Stratus* models. Currently it
only supports Alliance “.pat” pattern format. Patterns time stamps are
in the “absolute date” format, “relative date” isn’t allowed. Legal time
unit are ps (default), ns, us and ms. The constructor takes as
parameters the pattern output filename and an optional reference to
Stratus cell.

declar
------

Adds a connector from a Stratus model to the pattern interface. Writes
the corresponding connector declaration in the pattern file with name,
arity and direction automatically extracted from the connector
properties.
Supported Stratus connectors are:

-  SignalIn,

-  SignalOut (only supported if used as an output),

-  VddIn,

-  VssIn,

-  CkIn,

-  SignalInOut,

-  TriState (always an output),

-  Signals.

Parameters
~~~~~~~~~~

-  connector : can either be a reference to a stratus net or a string
   containing the name of the stratus net.

-  format : optional format for the connectors values into the pattern
   file, accepted values are :

   -  ’B’: binary (default),

   -  ’X’: hexadecimal,

   -  ’O’: octal.

declar\_interface
-----------------

Adds all the connectors from a Stratus model to the pattern interface.
Write the corresponding connector declaration in the pattern file with
name, arity and direction directly taken from the connector proprieties.

Parameters
~~~~~~~~~~

-  cell : the tested Stratus model reference. Optional if a reference to
   the tested Stratus model was given during instanciation[patwrite].

-  format : optional format for the connectors values into the pattern
   file, accepted values are :

   -  ’B’: binary (default),

   -  ’X’: hexadecimal,

   -  ’O’: octal.

declar
------

Affect a string value to a connector.

Parameters
~~~~~~~~~~

-  connector : *Stratus* connector

-  value : string to affect to connector

affect\_int
-----------

Affect an integer (CA2) value to a connector. Convert the 2’s complement
value to the corresponding binary value. The binary size is taken from
the connector arity. If the connector is an output, the binary value is
preceded by “?”.

Parameters
~~~~~~~~~~

-  connector : *Stratus* connector.

-  value : 2’s complement value to affect to the connector.

affect\_fix
-----------

Affect a fixed point value to a connector. Convert the floating point
input value to the corresponding fixed point value with
word\_length=connector.arity() and integer\_word\_length=iwl. If the
connector is an output, the binary value is preceded by “?”.

Parameters
~~~~~~~~~~

-  connector : *Stratus* connector.

-  value : floating point value to convert and asign to connector.

-  iwl : integer word length

affect\_any
-----------

Disable comparison between this connector value and the one calculated
during simulation.

Parameters
~~~~~~~~~~

-  connector : *Stratus* connector.

addpat
------

Adds a pattern in the pattern file.

pattern\_begin
--------------

Mark the end of the interface declaration and the beginning of the test
vectors.

pattern\_end
------------

Mark the end of the test vectors and of the patterns file.

Example
=======

``Pattern`` method for an addaccu

::

    def Pattern(self):
        # initialisation
        pat = PatWrite(self._name+'.pat',self)

        # declaration of ports
        pat.declar(self.ck, 'B')
        pat.declar(self.load, 'B')
        pat.declar(self.input, 'X')
        pat.declar(self.output, 'X')
        pat.declar(self.vdd, 'B')
        pat.declar(self.vss, 'B')
        
        # use of pat.declar_interface(self) has the same effect

        # description beginning
        pat.pattern_begin()

        # affect vdd and vss values
        pat.affect_int(self.vdd,1)
        pat.affect_int(self.vss,0)

        # first pattern : load an initial value
        pat.affect_int(self.input,5)
        pat.affect_int(self.load,1)
        pat.affect_int(self.ck,0)
        # add the pattern in the pattern file
        pat.addpat()
        # compute next event
        pat.affect_int(self.ck,1)
        pat.addpat()

        # compute 22 cycle of accumulation
        pat.affect_int(self.load,0)
        for i in range(1,22):
           pat.affect_int(self.ck,0)
           pat.addpat()
           pat.affect_int(self.ck,1)
           pat.affect_int(self.output,i+5)
           pat.addpat()

        # end of the description
        pat.pattern_end()


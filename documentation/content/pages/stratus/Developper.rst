

Class Model
===========

Synopsys
--------

::

    class myClass ( Model ) :
        ...
        
    exemple = myClass ( name, param )

Description
-----------

Every cell made is a class herited from class ``Model``.
Some methods have to be created, like ``Interface``, ``Netlist`` ...
Some methods are inherited from the class ``Model``.

Parameters
----------

-  ``name`` : The name of the cell (which is the name of the files which
   will be created)

-  ``param`` : A dictionnary which gives all the parameters useful in
   order to create the cell

Attributes
----------

-  ``_name`` : Name of the cell

-  ``_st_insts`` : List of all the instances of the cell

-  ``_st_ports`` : List of all the external nets of the cell (except for
   alimentations and clock)

-  ``_st_sigs`` : List of all the internal nets of the cell

-  ``_st_vdds``, ``_st_vsss`` : Two tabs of the nets which are
   instancied as ``VddIn`` and ``VssIn``

-  ``_st_cks`` : List of all the nets which are instancied as ``CkIn``

-  ``_st_merge`` : List of all the internal nets which have to be merged

-  ``_param`` : The map given as argument at the creation of the cell

-  ``_underCells`` : List of all the instances which are cells that have
   to be created

-  ``_and``, ``_or``, ``_xor``, ``_not``, ``_buff``, ``_mux``, ``_reg``,
   ``_shift``, ``_comp``, ``_add``, ``_mult``, ``_div`` : tells which
   generator to use when using overloard

-  ``_NB_INST`` : The number of instances of the cell (useful in order
   to automatically give a name to the instances)

-  ``_TAB_NETS_OUT`` and ``_TAB_NETS_CAT`` : Lists of all the nets
   automatically created

-  ``_insref`` : The reference instance (for placement)

And, in connection with Hurricane :

-  ``_hur_cell`` : The hurricane cell (None by default)

-  ``_db`` : The database

-  ``_lib0`` : ``self._db.Get_CATA_LIB ( 0 )``

-  ``_nb_alims_verticales``, ``_nb_pins``, ``_nb_vdd_pins``,
   ``_nb_vss_pins``, ``standard_instances_list``, ``pad_north``,
   ``pad_south``, ``pad_east``, ``pad_west`` : all place and route
   stuffs ...

Methods
-------

Methods of class ``Model`` are listed below :

-  ``HurricanePlug`` : Creates the Hurricane cell thanks to the
   stratus cell.
   Before calling this method, only the stratus cell is created, after
   this method, both cells are created. This method has to be called
   before View and Save, and before Layout.

-  ``View`` : Opens/Refreshes the editor in order to see the created
   layout
   
-  ``Quit`` : Finishes a cell without saving
   
-  ``Save`` : Saves the created cell
   If several cells have been created, they are all going to be saved
   in separated files

Some of those methods have to be defined in order to create a new cell :

-  ``Interface`` : Description of the external ports of the cell

-  ``Netlist`` : Description of the netlist of the cell

-  ``Layout`` : Description of the layout of the cell

-  ``Vbe`` : Description of the behavior of the cell

-  ``Pattern`` : Description of the patterns in order to test the cell

Nets
====

Synopsys
--------

::

    netInput = LogicIn ( name, arity )

Description
-----------

Instanciation of net. Differents kind of nets are listed below :

-  ``LogicIn`` : Creation of an input port

-  ``LogicOut`` : Creation of an output port

-  ``LogicInOut`` : Creation of an inout port

-  ``LogicUnknown`` : Creation of an input/output port which direction
   is not defined

-  ``TriState`` : Creation of a tristate port

-  ``CkIn`` : Creation of a clock port

-  ``VddIn`` : Creation of the vdd alimentation

-  ``VssIn`` : Creation of the vss alimentation

-  ``Signal`` : Creation of an internal net

Parameters
----------

-  ``name`` : Name of the net (mandatory argument)

-  ``arity`` : Arity of the net (mandatory argument)

-  ``indice`` : For buses only : the LSB bit (optional argument : set
   to 0 by default)

Only ``CkIn, ``\ ``VddIn`` and ``VssIn`` do not have the same parameters: there is only the ``name`` parameter (they are 1 bit nets).

Attributes
----------

-  ``_name`` : Name of the net

-  ``_arity`` : Arity of the net (by default set to 0)

-  ``_ind`` : LSB of the net

-  ``_ext`` : Tells if the net is external or not (True/False)

-  ``_direct`` : If the net is external, tells the direction (“IN”,
   “OUT”, “INOUT”, “TRISTATE”, “UNKNOWN”)

-  ``_h_type`` : If the net is an alimentation or a clock, tells the
   type (“POWER”, “GROUND”, “CLOCK”)

-  ``_type`` : The arithmetic type of the net ( “nr” )

-  ``_st_cell`` : The stratus cell which the net is instanciated in

-  ``_real_net`` : If the net is a part of a net (Sig) it is the real
   net corresponding

-  ``_alias`` : [] by default. When the net has an alias, it’s a tab.
   Each element of the tab correspond to a bit of the net (from the LSB
   to the MSB), it’a a dictionnary : the only key is the net which this
   net is an alias from, the value is the bit of the net

-  ``_to_merge`` : [] by default. The same as \_alias

-  | ``_to_cat`` : [] by default. The same as \_alias

And, in connection with Hurricane :

-  ``_hur_net`` : A tab with all the hurricane nets corresponding to the
   stratus net ; From the LSB to the MSB (for example, with a 1 bit net,
   one gets the hurricane net by doing : ``net._hur_net[0]`` ).

Methods
-------

-  ``Buffer`` : Instanciation of a Buffer

-  ``Shift`` : Instanciation of a shifter

-  ``Mux`` : Instanciation of a multiplexor

-  ``Reg`` : Instanciation of a register

-  ``Eq/Ne`` : Instanciation of comparison generator

-  ``Extend`` : A net is extended

-  ``Alias`` : A net is an alias of another net

-  | ``Delete`` : Deletion of the Hurricane nets

And the overloards :

-  \_\_init\_\_ : Initialisation of nets

-  \_\_le\_\_ : initialisation of a net thanks to <= notation

-  \_\_getitem\_\_, \_\_geslice\_\_ : Creation of “Sig” nets : which are
   part of nets (use of ``[]`` and ``[:]``)

-  \_\_and\_\_, \_\_or\_\_, \_\_xor\_\_, \_\_invert\_\_ : boolean
   operation with &, \|, ^ ,  

-  \_\_add\_\_, \_\_mul\_\_, \_\_div\_\_ : arithmetic operators with +,
   \* and /

Instances
=========

Synopsys
--------

::

    Inst ( model
         , name
         , param = myParam
         , map = myMap
         )

Description
-----------

Instantiation of an instance. The type of the instance is given by the
``model`` parameter. The connexions are made thanks to the ``map``
parameters.

Parameters
----------

-  ``model`` : Name of the mastercell of the instance to create
   (mandatory argument)

-  ``name`` : Name of the instance (optional)
   When this argument is not defined, the instance has a name created
   by default. This argument is usefull when one wants to create a
   layout as well. Indeed, the placement of the instances is much easier
   when the conceptor has chosen himself the name f the instances.

-  ``param`` : Dictionnary for parameters of the mastercell (optional :
   only for mastercells which require it)

-  ``map`` : Dictionnary for connexions in order to make the netlist

Attributes
----------

-  ``_name`` : Name of the instance (the name given as parameter if
   there’s one, a name created otherwise)

-  ``_model`` : Name of the model given as argument

-  ``_real_model`` : Name of the model created thanks to ``_model`` and
   all the parameters

-  ``_map`` : Dictionnary ``map`` given at the instanciation

-  ``_param`` : Dictionnary ``param`` given at the instanciation

-  ``_st_cell`` : The stratus cell which the instance is instanciated in

-  ``_st_masterCell`` : The stratus master cell of the instance

For placement :

-  ``_plac`` : tells if the instance is placed or not (UNPLACED by
   default)

-  ``_x``, ``_y`` : the coordinates of the instance (only for placed
   instances)

-  ``_sym`` : the symetry of the instance (only for placed instances)

And, in connection with Hurricane :

-  ``_hur_instance`` : The hurricane instance (None by default)

-  ``_hur_masterCell`` : The Hurricane master cell of the instance (None
   by default)

Methods
-------

-  Delete : Deletion of the Hurricane instance



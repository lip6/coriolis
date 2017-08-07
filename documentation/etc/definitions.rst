.. -*- Mode: rst; explicit-buffer-name: "definition.rst<documentation/etc>" -*-


.. role:: raw-html(raw)
   :format: html

.. role:: raw-latex(raw)
   :format: latex

.. role:: ul
.. role:: cb
.. role:: sc
.. role:: fboxtt

.. HTML/LaTeX backends mixed macros.
.. |br|                             replace:: :raw-latex:`\linebreak` :raw-html:`<br/>`
.. |medskip|                        replace:: :raw-latex:`\medskip`   :raw-html:`<br class="medskip"/>`
.. |newpage|                        replace:: :raw-latex:`\newpage`
.. |linebreak|                      replace:: :raw-latex:`\smallskip`
.. |noindent|                       replace:: :raw-latex:`\noindent`  :raw-html:`<p class="noindent"></p>`
.. |dotfill|                        replace:: :raw-html:`&nbsp;&nbsp;`
.. |bcenter|                        replace:: :raw-latex:`\begin{center}`
.. |ecenter|                        replace:: :raw-latex:`\end{center}`
.. |pagestylefancy|                 replace:: :raw-latex:`\thispagestyle{fancy}`


.. Acronyms & names.
.. |GNU|                            replace:: :sc:`gnu`
.. |LGPL|                           replace:: :sc:`lgpl`
.. |GPL|                            replace:: :sc:`gpl`
.. |UPMC|                           replace:: :sc:`upmc`
.. |Bull|                           replace:: :sc:`Bull`
.. |Cadence|                        replace:: :sc:`Cadence`
.. |Si2|                            replace:: :sc:`Si2`
.. |LEFDEF|                         replace:: :sc:`lefdef`
.. |Flute|                          replace:: :sc:`Flute`
.. |MacOS|                          replace:: :sc:`MacOS`
.. |RHEL6|                          replace:: :sc:`rhel6`
.. |RHEL7|                          replace:: :sc:`rhel7`
.. |SL6|                            replace:: :sc:`Scientific Linux 6`
.. |SL7|                            replace:: :sc:`Scientific Linux 7`
.. |Scientific Linux|               replace:: :sc:`Scientific Linux`
.. |RedHat|                         replace:: :sc:`RedHat`
.. |Fedora|                         replace:: :sc:`Fedora`
.. |FC13|                           replace:: :sc:`fc13`
.. |Debian|                         replace:: :sc:`Debian`
.. |Ubuntu|                         replace:: :sc:`Ubuntu`

.. |Alexandre|                      replace:: :sc:`Alexandre`
.. |Belloeil|                       replace:: :sc:`Belloeil`
.. |Chaput|                         replace:: :sc:`Chaput`
.. |Chu|                            replace:: :sc:`Chu`
.. |Clement|                        replace:: :sc:`Clement`
.. |Dupuis|                         replace:: :sc:`Dupuis`
.. |Escassut|                       replace:: :sc:`Escassut`
.. |Gouvine|                        replace:: :sc:`Gouvine`
.. |Masson|                         replace:: :sc:`Masson`
.. |Sroka|                          replace:: :sc:`Sroka`
.. |Yifei|                          replace:: :sc:`Yifei`

.. |ANSI|                           replace:: :sc:`ansi`
.. |MIPS|                           replace:: :sc:`mips`
.. |Am2901|                         replace:: :sc:`Am2901`
.. |Hurricane|                      replace:: :sc:`Hurricane`
.. |HurricaneAMS|                   replace:: :sc:`HurricaneAMS`
.. |Alliance|                       replace:: :sc:`Alliance`
.. |Yosys|                          replace:: :sc:`Yosys`
.. |GenLib|                         replace:: :sc:`GenLib`
.. |Nero|                           replace:: :sc:`Nero`
.. |Druc|                           replace:: :cb:`Druc`
.. |Coloquinte|                     replace:: :sc:`Coloquinte`
.. |Coriolis|                       replace:: :sc:`Coriolis`
.. |Coriolis1|                      replace:: :sc:`Coriolis 1`
.. |Coriolis2|                      replace:: :sc:`Coriolis 2`
.. |VLSISAPD|                       replace:: :sc:`vlsisapd`
.. |CRLcore|                        replace:: :sc:`CRLcore`
.. |Cyclop|                         replace:: :sc:`Cyclop`
.. |Nimbus|                         replace:: :sc:`Nimbus`
.. |hMetis|                         replace:: :sc:`hMetis`
.. |Mauka|                          replace:: :sc:`Mauka`
.. |Etesian|                        replace:: :sc:`Etesian`
.. |Knik|                           replace:: :sc:`Knik`
.. |Katabatic|                      replace:: :sc:`Katabatic`
.. |Kite|                           replace:: :sc:`Kite`
.. |Stratus|                        replace:: :sc:`Stratus`
.. |Stratus1|                       replace:: :sc:`Stratus1`
.. |Stratus2|                       replace:: :sc:`Stratus2`
.. |Unicorn|                        replace:: :sc:`Unicorn`
.. |ccb|                            replace:: :cb:`ccb`
.. |cgt|                            replace:: :cb:`cgt`
.. |Chams|                          replace:: :sc:`Chams`
.. |OpenChams|                      replace:: :sc:`OpenChams`
.. |Pharos|                         replace:: :cb:`Pharos`
.. |API|                            replace:: :sc:`api`
.. |STL|                            replace:: :sc:`stl`
.. |XML|                            replace:: :sc:`xml`
.. |pdf|                            replace:: :sc:`pdf`
.. |UTF-8|                          replace:: :sc:`utf-8`
.. |Python|                         replace:: :sc:`Python`
.. |Linux|                          replace:: :sc:`Linux`
.. |MacPorts|                       replace:: :sc:`MacPorts`
.. |devtoolset2|                    replace:: :cb:`devtoolset2`
.. |boost|                          replace:: :cb:`boost`
.. |Qt|                             replace:: :sc:`qt`
.. |tty|                            replace:: :cb:`tty`
.. |svn|                            replace:: :cb:`svn`
.. |git|                            replace:: :cb:`git`
.. |rpm|                            replace:: :cb:`rpm`
.. |gdb|                            replace:: :cb:`gdb`
.. |cmake|                          replace:: :cb:`cmake`
.. |struct|                         replace:: :cb:`struct`

.. |KeyUp|                          replace:: :fboxtt:`Up`
.. |KeyDown|                        replace:: :fboxtt:`Down`
.. |KeyLeft|                        replace:: :fboxtt:`Left`
.. |KeyRight|                       replace:: :fboxtt:`Right`
.. |KeyF|                           replace:: :fboxtt:`f`
.. |KeyL|                           replace:: :fboxtt:`l`
.. |KeyG|                           replace:: :fboxtt:`g`
.. |KeyZ|                           replace:: :fboxtt:`z`
.. |KeyM|                           replace:: :fboxtt:`m`
.. |KeyI|                           replace:: :fboxtt:`i`
.. |KeyK|                           replace:: :fboxtt:`k`
.. |KeyP|                           replace:: :fboxtt:`p`
.. |KeyO|                           replace:: :fboxtt:`o`
.. |KeyW|                           replace:: :fboxtt:`w`
.. |KeyQ|                           replace:: :fboxtt:`q`
.. |KeyCapK|                        replace:: :fboxtt:`K`
.. |KeyCapS|                        replace:: :fboxtt:`S`
.. |Plus|                           replace:: :fboxtt:`+`
.. |KeyESC|                         replace:: :fboxtt:`ESC`
.. |CTRL|                           replace:: :fboxtt:`CTRL`
.. |CTRL_L|                         replace:: :fboxtt:`CTRL+L`
.. |CTRL_I|                         replace:: :fboxtt:`CTRL+I`
.. |CTRL_P|                         replace:: :fboxtt:`CTRL+P`
.. |CTRL_O|                         replace:: :fboxtt:`CTRL+O`
.. |CTRL_W|                         replace:: :fboxtt:`CTRL+W`
.. |CTRL_Q|                         replace:: :fboxtt:`CTRL+Q`
.. |CTRL_Down|                      replace:: :fboxtt:`CTRL+Down`
.. |CTRL_Up|                        replace:: :fboxtt:`CTRL+Up`
.. |CTRL_Left|                      replace:: :fboxtt:`CTRL+Left`
.. |CTRL_Right|                     replace:: :fboxtt:`CTRL+Right`

.. URLs
.. _Doxygen:                        http://www.stack.nl/~dimitri/doxygen/
.. _LaTeX2HTML:                     http://www.latex2html.org/
.. _FGR:                            http://vlsicad.eecs.umich.edu/BK/FGR/
.. _Box Router:                     http://www.cerc.utexas.edu/~thyeros/boxrouter/boxrouter.htm
.. _hMETIS:                         http://glaros.dtc.umn.edu/gkhome/views/metis
.. _Knik Thesis:                    http://www-soc.lip6.fr/en/users/damiendupuis/PhD/
.. _RapidJSON:                      http://miloyip.github.io/rapidjson/
.. _Python/C API Reference Manual:  https://docs.python.org/2/c-api/index.html

.. Standard CAO/VLSI Concepts.
.. |netlist|                        replace:: *netlist*
.. |netlists|                       replace:: *netlists*
.. |layout|                         replace:: *layout*
.. |layouts|                        replace:: *layouts*
.. |CMOS|                           replace:: :sc:`cmos`
.. |VHDL|                           replace:: :sc:`vhdl`
.. |NWELL|                          replace:: :sc:`nwell`
.. |POWER|                          replace:: :sc:`power`
.. |GROUND|                         replace:: :sc:`ground`

.. MBK Concepts
.. |MBK|                            replace:: :sc:`mbk`
.. |LOFIG|                          replace:: :cb:`Lofig`
.. |PHFIG|                          replace:: :cb:`Phfig`
.. |SxLib|                          replace:: :sc:`SxLib`
.. |RDS|                            replace:: :sc:`rds`

.. Hurricane Concepts.
.. |hypernet|                       replace:: *hypernet*
.. |hypernets|                      replace:: *hypernets*
.. |Cell|                           replace:: *Cell*
.. |Rings|                          replace:: *Rings*
.. |QuadTrees|                      replace:: *QuadTrees*
.. |Collections|                    replace:: *Collections*
.. |ap|                             replace:: :cb:`ap`
.. |vst|                            replace:: :cb:`vst`
.. |kgr|                            replace:: :cb:`kgr`
.. |dot_conf|                       replace:: :cb:`.conf`

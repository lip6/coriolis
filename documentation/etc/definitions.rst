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
.. |lambda|                         replace:: :raw-latex:`$\lambda$` :raw-html:`&#955`


.. Acronyms & names.
.. |NDA|                            replace:: :sc:`nda`
.. |OS|                             replace:: :sc:`os`
.. |FOSS|                           replace:: :sc:`foss`
.. |GNU|                            replace:: :sc:`gnu`
.. |LGPL|                           replace:: :sc:`lgpl`
.. |GPL|                            replace:: :sc:`gpl`
.. |UPMC|                           replace:: :sc:`upmc`
.. |SorbonneUniversite|             replace:: :sc:`Sorbonne Université`
.. |SU|                             replace:: :sc:`su`
.. |CIAN|                           replace:: :sc:`cian`
.. |LIP6|                           replace:: :sc:`lip6`
.. |Bull|                           replace:: :sc:`Bull`
.. |Cadence|                        replace:: :sc:`Cadence`
.. |Si2|                            replace:: :sc:`Si2`
.. |LEFDEF|                         replace:: :sc:`lefdef`
.. |LEF|                            replace:: :sc:`lef`
.. |DEF|                            replace:: :sc:`def`
.. |Flute|                          replace:: :sc:`Flute`
.. |MacOS|                          replace:: :sc:`MacOS`
.. |Darwin|                         replace:: :sc:`Darwin`
.. |RHEL|                           replace:: :sc:`rhel`
.. |RHEL6|                          replace:: :sc:`rhel6`
.. |RHEL7|                          replace:: :sc:`rhel7`
.. |SL6|                            replace:: :sc:`Scientific Linux 6`
.. |SL7|                            replace:: :sc:`Scientific Linux 7`
.. |Scientific Linux|               replace:: :sc:`Scientific Linux`
.. |RedHat|                         replace:: :sc:`RedHat`
.. |Fedora|                         replace:: :sc:`Fedora`
.. |FC13|                           replace:: :sc:`fc13`
.. |EPEL|                           replace:: :sc:`epel`
.. |Debian|                         replace:: :sc:`Debian`
.. |Ubuntu|                         replace:: :sc:`Ubuntu`
.. |MOSIS|                          replace:: :sc:`mosis`
.. |Blif|                           replace:: :sc:`blif`
.. |TSMC|                           replace:: :sc:`tsmc`
.. |AMS|                            replace:: :sc:`ams`

.. |Alexandre|                      replace:: :sc:`Alexandre`
.. |Belloeil|                       replace:: :sc:`Belloeil`
.. |Chaput|                         replace:: :sc:`Chaput`
.. |Chotin|                         replace:: :sc:`Chotin`
.. |Chu|                            replace:: :sc:`Chu`
.. |Clement|                        replace:: :sc:`Clement`
.. |Dupuis|                         replace:: :sc:`Dupuis`
.. |Escassut|                       replace:: :sc:`Escassut`
.. |Gouvine|                        replace:: :sc:`Gouvine`
.. |Masson|                         replace:: :sc:`Masson`
.. |Sroka|                          replace:: :sc:`Sroka`
.. |Yifei|                          replace:: :sc:`Yifei`
.. |Mead|                           replace:: :sc:`Mead`
.. |Conway|                         replace:: :sc:`Conway`

.. |ASIC|                           replace:: :sc:`asic`
.. |EDA|                            replace:: :sc:`eda`
.. |RTL|                            replace:: :sc:`rtl`
.. |HSL|                            replace:: :sc:`hsl`
.. |IEEE|                           replace:: :sc:`ieee`
.. |ANSI|                           replace:: :sc:`ansi`
.. |MIPS|                           replace:: :sc:`mips`
.. |Am2901|                         replace:: :sc:`Am2901`
.. |CAIRO|                          replace:: :sc:`cairo`
.. |CAIRO+|                         replace:: :sc:`cairo+`
.. |Hurricane|                      replace:: :sc:`Hurricane`
.. |CRL|                            replace:: :sc:`crl`
.. |Alliance|                       replace:: :sc:`Alliance`
.. |StaCS|                          replace:: :sc:`StaCS`
.. |Yosys|                          replace:: :sc:`Yosys`
.. |GenLib|                         replace:: :sc:`GenLib`
.. |Nero|                           replace:: :sc:`Nero`
.. |Druc|                           replace:: :cb:`Druc`
.. |Coloquinte|                     replace:: :sc:`Coloquinte`
.. |Coriolis|                       replace:: :sc:`Coriolis`
.. |Coriolis1|                      replace:: :sc:`Coriolis 1`
.. |Coriolis2|                      replace:: :sc:`Coriolis 2`
.. |alliance-check-toolkit|         replace:: ``alliance-check-toolkit``
.. |VLSISAPD|                       replace:: :sc:`vlsisapd`
.. |CRLcore|                        replace:: :sc:`CRLcore`
.. |Cyclop|                         replace:: :sc:`Cyclop`
.. |Nimbus|                         replace:: :sc:`Nimbus`
.. |hMetis|                         replace:: :sc:`hMetis`
.. |Mauka|                          replace:: :sc:`Mauka`
.. |Etesian|                        replace:: :sc:`Etesian`
.. |Knik|                           replace:: :sc:`Knik`
.. |Anabatic|                       replace:: :sc:`Anabatic`
.. |Katabatic|                      replace:: :sc:`Katabatic`
.. |Kite|                           replace:: :sc:`Kite`
.. |Katana|                         replace:: :sc:`Katana`
.. |Stratus|                        replace:: :sc:`Stratus`
.. |Stratus1|                       replace:: :sc:`Stratus1`
.. |Stratus2|                       replace:: :sc:`Stratus2`
.. |Unicorn|                        replace:: :sc:`Unicorn`
.. |ccb|                            replace:: :cb:`ccb`
.. |cgt|                            replace:: :cb:`cgt`
.. |Chams|                          replace:: :sc:`Chams`
.. |OpenChams|                      replace:: :sc:`OpenChams`
.. |API|                            replace:: :sc:`api`
.. |STL|                            replace:: :sc:`stl`
.. |XML|                            replace:: :sc:`xml`
.. |pdf|                            replace:: :sc:`pdf`
.. |UTF-8|                          replace:: :sc:`utf-8`
.. |Python|                         replace:: :sc:`Python`
.. |Linux|                          replace:: :sc:`Linux`
.. |MacPorts|                       replace:: :sc:`MacPorts`
.. |devtoolset|                     replace:: :cb:`devtoolset`
.. |boost|                          replace:: :cb:`boost`
.. |Qt|                             replace:: :sc:`qt`
.. |Qwt|                            replace:: :sc:`qwt`
.. |PyQt|                           replace:: :sc:`PyQt`
.. |PyQt4|                          replace:: :sc:`PyQt4`
.. |PyQt5|                          replace:: :sc:`PyQt5`
.. |tty|                            replace:: :cb:`tty`
.. |svn|                            replace:: :cb:`svn`
.. |git|                            replace:: :cb:`git`
.. |rpm|                            replace:: :cb:`rpm`
.. |gdb|                            replace:: :cb:`gdb`
.. |valgrind|                       replace:: :cb:`valgrind`
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
.. _Apache License, Version 2.0:    http://www.apache.org/licences/LICENSE-2.0
.. _FreePDK45:                      https://www.eda.ncsu.edu/wiki/FreePDK45:Contents
.. _Yosys:                          http://www.clifford.at/yosys/
.. _GHDL:                           https://github.com/ghdl/ghdl
.. _Si2:                            http://www.si2.org/

.. _GPL:                            http://www.gnu.org/copyleft/gpl.txt
.. _LGPL:                           https://www.gnu.org/licenses/lgpl-3.0.html
.. _Apache License, Version 2.0:    http://www.apache.org/licences/LICENSE-2.0
.. _Tokai University:               http://www.u-tokai.ac.jp/international/
.. _Shimizu Lab:                    http://labo.nshimizu.com/
.. _Smartlabs:                      http://www.smartlabsinc.com/
.. _Don Bosco University:           http://www.udb.edu.sv/
.. _MyCMP:                          https://www.mycmp.fr/

.. Standard CAO/VLSI Concepts.
.. |netlist|                        replace:: *netlist*
.. |netlists|                       replace:: *netlists*
.. |layout|                         replace:: *layout*
.. |layouts|                        replace:: *layouts*
.. |VLSI|                           replace:: :sc:`vlsi`
.. |GDS|                            replace:: :sc:`gds`
.. |CMOS|                           replace:: :sc:`cmos`
.. |VHDL|                           replace:: :sc:`vhdl`
.. |NWELL|                          replace:: :sc:`nwell`
.. |POWER|                          replace:: :sc:`power`
.. |GROUND|                         replace:: :sc:`ground`

.. Alliance & MBK Concepts
.. |MBK|                            replace:: :sc:`mbk`
.. |LOFIG|                          replace:: :cb:`Lofig`
.. |PHFIG|                          replace:: :cb:`Phfig`
.. |SxLib|                          replace:: :sc:`SxLib`
.. |RDS|                            replace:: :sc:`rds`
.. |s2r|                            replace:: :cb:`s2r`

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

#!/usr/bin/python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |  S t r a t u s   -  Netlists/Layouts Description                |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./stratus.py"                             |
# +-----------------------------------------------------------------+


try:
  import sys
  import Cfg
  import CRL

 # Triggers the default configuration files loading.
  CRL.AllianceFramework.get()

  Cfg.Configuration.pushDefaultPriority(Cfg.Parameter.Priority.ApplicationBuiltin)
  Cfg.getParamString('stratus1.format'   ).setString('vst')
  Cfg.getParamString('stratus1.simulator').setString('asimut')
  Cfg.Configuration.popDefaultPriority()

  print '  o  Stratus Configuration:'
  print '     - Netlist format: <%s>.' % Cfg.getParamString('stratus1.format').asString()
  print '     - Simulator: <%s>.'      % Cfg.getParamString('stratus1.simulator').asString()
  
  from st_model         import *
  from st_net           import *
  from st_instance      import *
  from st_placement     import *
  from st_placeAndRoute import *
  from st_ref           import *
  from st_generate      import *
  from st_const         import *
  from st_cat           import *
  from st_param         import *
  from st_getrealmodel  import GetWeightTime, GetWeightArea, GetWeightPower
  
  from util_Const       import *
  from util_Defs        import *
  from util_Misc        import *
  from util_Gen         import *
  from util_Shift       import *
  from util_uRom        import *
  from util             import *
  
  from patterns         import *
except ImportError, e:
  module = str(e).split()[-1]

  print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
  print '        Please check the integrity of the <coriolis> package.'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)

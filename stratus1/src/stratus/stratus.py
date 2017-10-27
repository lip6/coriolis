#!/usr/bin/python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2008-2016, All Rights Reserved
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
  import traceback
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
  
  from Hurricane        import UpdateSession
  from st_model         import *
  from st_net           import *
  from st_instance      import *
  from st_placement     import *
 #from st_placeAndRoute import *
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


DoNetlist = 0x0001
DoLayout  = 0x0002
DoPattern = 0x0004
DoStop    = 0x0008


def buildModel ( moduleName, flags, className=None, modelName=None, parameters={} ):
    try:
     #print moduleName
      if not className: className = moduleName
      if not modelName: modelName = moduleName.lower()

      module = __import__( moduleName, globals(), locals(), className )
      if not module.__dict__.has_key(className):
          print '[ERROR] Stratus module <%s> do not contains a design named <%s>.' % (moduleName,className)
          sys.exit(1)

      UpdateSession.open()
      print '     - Generating Stratus Model <%s> (generator:<%s>).' % (modelName, className)
      model = module.__dict__[className](modelName,parameters)
      model.Interface()

      saveFlags = 0
      if flags & DoNetlist: model.Netlist(); saveFlags |= LOGICAL 
      if flags & DoLayout:  model.Layout (); saveFlags |= PHYSICAL
      if flags & DoPattern: model.Pattern()

      stopLevel=0
      if flags & DoStop: stopLevel = 1
      model.View(stopLevel, 'Model %s' % modelName)
      model.Save(saveFlags)
      UpdateSession.close()

    except ImportError, e:
      module = str(e).split()[-1]

      print '[ERROR] The <%s> Stratus design cannot be loaded.' % module
      print '        Please check your design hierarchy.'
      print e
      sys.exit(1)
    except Exception, e:
      print '[ERROR] A strange exception occurred while loading the Stratus'
      print '        design <%s>. Please check that module for error:\n' % moduleName
      traceback.print_tb(sys.exc_info()[2])
      print '        %s' % e
      sys.exit(2)

    framework = CRL.AllianceFramework.get()
    return framework.getCell( modelName, CRL.Catalog.State.Views )

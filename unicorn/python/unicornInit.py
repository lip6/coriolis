#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          U n i c o r n  -  M a i n   G U I                      |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./init/unicornInit.py"                    |
# +-----------------------------------------------------------------+


try:
  import traceback
  import sys
  import os.path
  import helpers
  from   helpers.io import ErrorMessage
  from   helpers.io import WarningMessage
  import plugins
  import Viewer
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


#helpers.staticInitialization( quiet=True )


def unicornConfigure ( **kw ):
    editor = None
    if kw.has_key('editor'):
      editor = kw['editor']
    else:
      print ErrorMessage( 3, 'unicornConfigure.py: Must be run from a CellView derived class.' )
      return

    if editor.hasMenu( 'plugins' ):
      print WarningMessage( 'The <plugins> menu has already been created.' )
      return

   #editor.addMenu( 'plugins', 'Plu&gins', Viewer.CellViewer.TopMenu )

    for moduleName in sys.modules:
      if moduleName.startswith('plugins.'):
        try:
          module = sys.modules[ moduleName ]
          if not module:
           #print WarningMessage( 'Plugin "%s" not found in Python sys/modules[].' \
           #                       % moduleName )
            continue
          
          if not module.__dict__.has_key('unicornHook'):
            elements = module.__file__.split( os.sep )
            if elements[-2] == 'plugins':
              print WarningMessage( 'Plugin "%s" do not provides the unicornHook() method, skipped.' \
                                     % moduleName )
            continue
          
          module.__dict__['unicornHook']( **kw )
        except ErrorMessage, e:
          print e
          helpers.showStackTrace( e.trace )
        except Exception, e:
          helpers.showPythonTrace( __file__, e )

    return

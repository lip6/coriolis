#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          U n i c o r n  -  M a i n   G U I                      |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./init/initHook.py"                       |
# +-----------------------------------------------------------------+


try:
    import traceback
    import sys
    import os.path
    from   ..helpers    import staticInitialization, setTraceLevel
    from   ..helpers.io import ErrorMessage, WarningMessage, showStackTrace, \
                               showPythonTrace, vprint
    from   ..           import plugins
    from   ..           import Viewer
except ImportError as e:
    print( e )
    serror = str(e)
    if serror.startswith('No module named'):
        module = serror.split()[-1]
        print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(module) )
        print( '        Please check the integrity of the <coriolis> package.' )
    if str(e).find('cannot open shared object file'):
        library = serror.split(':')[0]
        print( '[ERROR] The "{}" shared library cannot be loaded.'.format(library) )
        print( '        Under RHEL 6, you must be under devtoolset-2.' )
        print( '        (scl enable devtoolset-2 bash)' )
    sys.exit(1)
#except Exception as e:
#    print( '[ERROR] A strange exception occurred while loading the basic Coriolis/Python' )
#    print( '        modules. Something may be wrong at Python/C API level.\n' )
#    print( '        {}'.format(e) )
#    sys.exit(2)


#staticInitialization( quiet=True )


def unicornConfigure ( **kw ):
    """
    Look through all the currently loaded modules and try to find, then call
    the ``unicornHook()`` function.

    .. note:: The modules plugins should have been loaded by the static
              initialization of the ``cumulus`` tool.
    """
    editor = None
    if 'editor' in kw:
        editor = kw['editor']
    else:
        print( ErrorMessage( 3, 'unicorn.initHook.py: Must be run from a CellView derived class.' ))
        return
    if editor.hasMenu( 'plugins' ):
        print( WarningMessage( 'The <plugins> menu has already been created.' ))
        return
   #editor.addMenu( 'plugins', 'Plu&gins', Viewer.CellViewer.TopMenu )
    vprint( 1, '  o  Unicorn loading plugins into menus.' )
    for moduleName in sys.modules:
        try:
            module = sys.modules[ moduleName ]
            if not module:
               #print( WarningMessage( 'Plugin "{}" not found in Python sys/modules[].' \
               #                       .format(moduleName) ))
                continue
            if not 'unicornHook' in module.__dict__:
                if module.__name__.endswith( '.__pycache__' ): continue
                if not '__file__' in module.__dict__: continue
                if module.__file__ is None: continue
                elements = module.__file__.split( os.sep )
                if elements[-1] == 'plugins':
                    print( WarningMessage( 'Plugin "{}" do not provides the unicornHook() method, skipped.' \
                                           .format(moduleName) ))
                continue
            vprint( 1, '     - "{}".'.format( module.__name__ ))
            module.__dict__['unicornHook']( **kw )
        except ErrorMessage as e:
            showStackTrace( e.trace )
        except Exception as e:
            showPythonTrace( __file__, e )
    return

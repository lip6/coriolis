#!/usr/bin/env python

try:
  import sys
  import os.path
  from   helpers   import ErrorMessage
  from   helpers   import WarningMessage
  import Viewer
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


def unicornConfigure ( **kw ):
    editor = None
    if kw.has_key('editor'):
        editor = kw['editor']
    else:
        print ErrorMessage( 3, 'unicornConfigure.py: Must be run from a CellView derived class.' )
        return

    cumulusDir = None
    for path in sys.path:
      if path.endswith('/cumulus'):
        cumulusDir = path
    if not cumulusDir:
        print ErrorMessage( 3, 'unicornConfigure.py: Cannot find <cumulus> in PYTHONPATH.' )
        return

    pluginsDir = os.path.join( cumulusDir, 'plugins' )
    if not os.path.isdir(pluginsDir):
        print ErrorMessage( 3, 'unicornConfigure.py: Cannot find <cumulus/plugins> directory:' \
                             , '<%s>' % pluginsDir )
        return
    sys.path.append( pluginsDir )
      
    if editor.hasMenu( 'plugins' ):
        print WarningMessage( 'The <plugins> menu has already been created.' )
        return

    editor.addMenu( 'plugins', 'Plu&gins', Viewer.CellViewer.TopMenu )

    for pluginFile in os.listdir( pluginsDir ):
      if not pluginFile.endswith('.py'): continue

      moduleName = os.path.basename(pluginFile)[:-3]
      module     = __import__( moduleName, globals(), locals(), moduleName )

      if not module.__dict__.has_key('unicornHook'):
        print WarningMessage( 'Module <%s> do not provides the unicornHook() method, skipped.' \
                              % moduleName )
        continue

      module.__dict__['unicornHook']( editor )

    return

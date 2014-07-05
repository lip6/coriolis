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


def kiteHook ( **kw ):
    kite = None
    if kw.has_key('kite'):
        kite = kw['kite']
    else:
        print ErrorMessage( 3, 'kiteHook(): Must be run from a KiteEngine.' )
        return

    userInit = os.path.join( os.getcwd(), '.coriolis2.kite.py' )
    if (os.path.exists(userInit)):
      execfile( userInit )
    return

#!/usr/bin/env python

try:
  import sys
  import os.path
  import helpers.io
  from   helpers.io import ErrorMessage
  from   helpers.io import WarningMessage
  import Viewer
except Exception, e:
  helpers.io.catch( e )
  sys.exit( 1 )


def boraHook ( **kw ):
    bora = None
    if kw.has_key('bora'):
      bora = kw['bora']
    else:
      print ErrorMessage( 3, 'boraHook(): Must be run from a BoraEngine.' )
      return

    try:
      userInit = os.path.join( os.getcwd(), 'coriolis2/bora.py' )
      if (os.path.exists(userInit)):
        execfile( userInit )
    except Exception, e:
      helpers.io.catch( e )
    return

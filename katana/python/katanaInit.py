#!/usr/bin/env python

try:
    import sys
    import os.path
    import helpers.io
    from   helpers.io import ErrorMessage
    from   helpers.io import WarningMessage
    import Viewer
except Exception as e:
    helpers.io.catch( e )
    sys.exit( 1 )


def katanaHook ( **kw ):
    katana = None
    if 'katana' in kw:
        katana = kw['katana']
    else:
        print( ErrorMessage( 3, 'katanaHook(): Must be run from a KatanaEngine.' ))
        return
    try:
        userInit = os.path.join( os.getcwd(), 'coriolis2/katana.py' )
        if (os.path.exists(userInit)):
            exec( open(userInit).read() )
    except Exception as e:
        helpers.io.catch( e )
    return

#!/usr/bin/env python

try:
    import sys
    import os.path
    from   coriolis.helpers.io import ErrorMessage, WarningMessage, catch
    from   coriolis            import Viewer
except Exception as e:
    catch( e )
    sys.exit( 1 )


def katanaHook ( **kw ):
    katana = None
    if 'katana' in kw:
        katana = kw['katana']
    else:
        print( ErrorMessage( 3, 'katana.initHook(): Must be run from a KatanaEngine.' ))
        return
    try:
        userInit = os.path.join( os.getcwd(), 'coriolis2/katana.py' )
        if (os.path.exists(userInit)):
            exec( open(userInit).read() )
    except Exception as e:
        catch( e )
    return

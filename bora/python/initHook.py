
try:
    import sys
    import os.path
    from   coriolis.helpers.io import ErrorMessage, WarningMessage, catch
    import coriolis.Viewer
except Exception as e:
    catch( e )
    sys.exit( 1 )


def boraHook ( **kw ):
    bora = None
    if 'bora' in kw:
        bora = kw['bora']
    else:
        print( ErrorMessage( 3, 'bora.initHook(): Must be run from a BoraEngine.' ))
        return
    try:
        userInit = os.path.join( os.getcwd(), 'coriolis2/bora.py' )
        if (os.path.exists(userInit)):
            exec( open(userInit).read() )
    except Exception as e:
        catch( e )
    return

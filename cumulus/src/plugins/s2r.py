
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/s2r.py"                         |
# +-----------------------------------------------------------------+


try:
    import os
    import sys
    import traceback
    import subprocess
    import Viewer
    import helpers
    from   helpers.io import ErrorMessage
    from   helpers.io import WarningMessage
    from   Hurricane  import DataBase
    from   Hurricane  import Library
    from   CRL        import Gds
    import plugins
except Exception as e:
    helpers.io.catch( e )
    sys.exit(2)

  
# --------------------------------------------------------------------
# S2R class

class S2R ( object ):

    def __init__ ( self ):
        self.s2r  = None
        pathes = os.environ[ "PATH" ]
        for path in pathes.split(':'):
            binary = os.path.join( path, 's2r' )
            if os.path.exists(binary):
                self.s2r = binary
                break
        if not self.s2r:
            print( ErrorMessage( 1, 'S2R.__init__(): No s2r binary found in PATH, please setup Alliance environement.' ))
        return

    def convert ( self, cell ):
        if not self.s2r: return
        os.environ[ 'RDS_IN'  ] = 'gds'
        os.environ[ 'RDS_OUT' ] = 'gds'
        process = subprocess.Popen( [ self.s2r, cell.getName() ]
                                  , stderr=subprocess.STDOUT
                                  , stdout=subprocess.PIPE
                                  , shell =False
                                  )
        for line in process.stdout.readlines():
            print( 's2r | {}'.format(line[:-1] ))
        gdsFile = os.path.join( os.environ['MBK_WORK_LIB'], cell.getName()+'.gds' )
        rootLibrary = DataBase.getDB().getRootLibrary()
        gdsLibrary  = rootLibrary.getLibrary( 'gds' )
        if not gdsLibrary:
            gdsLibrary = Library.create( rootLibrary, 'GDS' )
        Gds.load( gdsLibrary, gdsFile )
        return gdsLibrary.getCell( cell.getName() )


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'tools.s2r'
                         , 'Symbolic to real (Alliance s2r)'
                         , 'Convert symbolic layout into GDSII using Alliance s2r'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    rvalue = True
    try:
       #helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        s2r = S2R()
        gdsCell = s2r.convert( cell )
        print( gdsCell )
        if editor: editor.setCell( gdsCell )
    except Exception as e:
        helpers.io.catch( e )
        if     'editor' in locals() and editor \
           and 'cell'   in locals() and cell:
            editor.fit()
        rvalue = False
    sys.stdout.flush()
    sys.stderr.flush()
    return rvalue

#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/S2R.py"                         |
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
  import chip.Chip
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
          print ErrorMessage( 1, 'S2R.__init__(): No s2r binary found in PATH, please setup Alliance environement.' )
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
          print 's2r | %s' % line[:-1]

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


def ScriptMain ( **kw ):
  rvalue = True
  try:
   #helpers.setTraceLevel( 550 )

    cell, editor = plugins.kwParseMain( **kw )
    s2r = S2R()
    gdsCell = s2r.convert( cell )
    print gdsCell
    if editor: editor.setCell( gdsCell )

  except Exception, e:
    helpers.io.catch( e )
    if     locals().has_key('editor') and editor \
       and locals().has_key('cell'  ) and cell: editor.fit()
    rvalue = False

  sys.stdout.flush()
  sys.stderr.flush()
      
  return rvalue

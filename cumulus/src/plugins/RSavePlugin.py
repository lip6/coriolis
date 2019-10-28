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
# |  Python      :       "./plugins/RSavePlugin.py"                 |
# +-----------------------------------------------------------------+


try:
  import sys
  import traceback
  import os.path
  import Cfg
  import CRL
  import helpers
  from   helpers.io import ErrorMessage
  from   helpers.io import WarningMessage
  import plugins
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


# Write back layout to disk if everything has gone fine.
# Must write all the sub-blocks of the core but *not* the
# standard cell (mainly the feed-through).
#
# If the model has been uniquified, in the case of a merging
# of abutment box for placement, the netlist view must also
# be saved.

def rsave ( cell, views=CRL.Catalog.State.Physical, depth=0 ):
  if cell.isTerminal(): return

  framework = CRL.AllianceFramework.get()
  if depth == 0: print '  o  Recursive Save-Cell.'

  sviews = ''
  if views & CRL.Catalog.State.Logical:
    sviews += 'netlist'
    if views & CRL.Catalog.State.VstUseConcat:
      if sviews: sviews += ','
      sviews += 'VST uses &'
  if views & CRL.Catalog.State.Physical:
    if sviews: sviews += ','
    sviews += 'layout'

  print '     %s+ %s (%s).' % ( ' '*(depth*2), cell.getName(), sviews )
  if cell.isUniquified(): views |= CRL.Catalog.State.Logical
  framework.saveCell( cell, views )

  for instance in cell.getInstances():
    masterCell = instance.getMasterCell()
    if not masterCell.isTerminal():
      rsave( masterCell, views, depth+1 )
  return


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'tools.rsave'
                         , 'Recursive Save (layout)'
                         , 'Recursively save layout of the top cell and it\'s instances'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  try:
   #helpers.setTraceLevel( 550 )

    cell, editor = plugins.kwParseMain( **kw )

    views = CRL.Catalog.State.Physical
    if kw.has_key('views'): views = kw['views']

    if not cell:
      print WarningMessage( 'No Cell loaded in the editor (yet), nothing done.' )
      return 0

    rsave( cell, views )
    CRL.destroyAllVHDL()

  except Exception, e:
    helpers.io.catch( e )

  sys.stdout.flush()
  sys.stderr.flush()
      
  return 0

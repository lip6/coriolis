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
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/rsave.py"                       |
# +-----------------------------------------------------------------+


from   __future__ import print_function
import sys
import traceback
import os.path

try:
    import Cfg
    import CRL
    import helpers
    from   helpers.io import ErrorMessage
    from   helpers.io import WarningMessage
    import plugins
except Exception, e:
    helpers.io.catch( e )
    sys.exit(2)


def rsave ( cell, views=CRL.Catalog.State.Physical, depth=0 ):
    """
    Write back layout to disk if everything has gone fine.
    Must write all the sub-blocks of the core but *not* the
    standard cell (mainly the feed-through).
   
    If the model has been uniquified, in the case of a merging
    of abutment box for placement, the netlist view must also
    be saved.
    """
    framework = CRL.AllianceFramework.get()
    if depth == 0: print( '  o  Recursive Save-Cell.' )
    sviews = ''
    if views & CRL.Catalog.State.Logical:
        sviews += 'netlist'
        if views & CRL.Catalog.State.VstUseConcat:
            if sviews: sviews += ','
            sviews += ' uses &'
        if views & CRL.Catalog.State.VstNoLowerCase:
            if sviews: sviews += ', no lowercase'
        if views & CRL.Catalog.State.VstUniquifyUpperCase:
            if sviews: sviews += ', uniquify uppercase'
        if views & CRL.Catalog.State.VstNoLinkage:
            if sviews: sviews += ', no linkage'
            sviews += ''
    if views & CRL.Catalog.State.Physical:
        if sviews: sviews += ','
        sviews += 'layout'
    print( '     {}+ {} ({}).'.format(' '*(depth*2), cell.getName(), sviews) )
    if cell.isUniquified():             views |= CRL.Catalog.State.Logical
    if cell.getName().endswith('_cts'): views |= CRL.Catalog.State.Logical
    if cell.getName().endswith('_r'  ): views |= CRL.Catalog.State.Logical
    framework.saveCell( cell, views )
    for instance in cell.getInstances():
        #print( '     {}| {}.'.format(' '*(depth*2), instance) )
        masterCell = instance.getMasterCell()
        if not masterCell.isTerminalNetlist():
            rsave( masterCell, views, depth+1 )
        #else:
        #    print( '     {}| Master cell is terminal netlist {}.'.format(' '*(depth*2), instance.getMasterCell()) )
    return


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    """Hook up rsave plugin into Unicorn/CGT menu tree."""
    plugins.kwUnicornHook( 'tools.rsave'
                         , 'Recursive Save (layout)'
                         , 'Recursively save layout of the top cell and it\'s instances'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    """Called when run as a stand alone script through Unicorn/CGT."""
    try:
       #helpers.setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        views        = CRL.Catalog.State.Physical
        if kw.has_key('views'): views = kw['views']
        if not cell:
            print( WarningMessage( 'No Cell loaded in the editor (yet), nothing done.' ) )
            return 0
        rsave( cell, views )
        CRL.destroyAllVHDL()
    except Exception, e:
      helpers.io.catch( e )
    sys.stdout.flush()
    sys.stderr.flush()
    return 0

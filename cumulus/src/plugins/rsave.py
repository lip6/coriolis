
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2023, All Rights Reserved
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


import sys
import traceback
import os.path

try:
    from ..           import Cfg
    from ..           import CRL
    from ..helpers    import setTraceLevel
    from ..helpers.io import ErrorMessage, WarningMessage, catch
    from ..            import plugins
except Exception as e:
    catch( e )
    sys.exit(2)


saveds = set()


def rsave ( cell, views=CRL.Catalog.State.Physical, depth=0, enableSpice=False ):
    """
    Write back layout to disk if everything has gone fine.
    Must write all the sub-blocks of the core but *not* the
    standard cell (mainly the feed-through).
   
    If the model has been uniquified, in the case of a merging
    of abutment box for placement, the netlist view must also
    be saved.
    """
    global saveds

    if depth == 0: saveds.clear()
    if cell in saveds: return
    saveds.add( cell )
    
    framework = CRL.AllianceFramework.get()
    if depth == 0: print( '  o  Recursive Save-Cell.' )
    if cell.isUniquified():             views |= CRL.Catalog.State.Logical
    if cell.getName().endswith('_cts'): views |= CRL.Catalog.State.Logical
    if cell.getName().endswith('_r'  ): views |= CRL.Catalog.State.Logical
    sviews = ''
    if views & CRL.Catalog.State.Logical:
        sviews += 'netlist'
        if views & CRL.Catalog.State.VstUseConcat:
            if sviews: sviews += ', '
            sviews += 'uses &'
        if views & CRL.Catalog.State.VstNoLowerCase:
            if sviews: sviews += ', '
            sviews += 'no lowercase'
        if views & CRL.Catalog.State.VstUniquifyUpperCase:
            if sviews: sviews += ', '
            sviews += 'uniquify uppercase'
        if views & CRL.Catalog.State.VstNoLinkage:
            if sviews: sviews += ', '
            sviews += 'no linkage'
        if enableSpice:
            if sviews: sviews += ', '
            sviews += 'SPICE'
    if views & CRL.Catalog.State.Physical:
        if sviews: sviews += ','
        sviews += 'layout'
    print( '     {}+ {} ({}).'.format(' '*(depth*2), cell.getName(), sviews) )
    framework.saveCell( cell, views )
    spiceFlags = CRL.Spice.TopCell if depth == 0 else 0
    CRL.Spice.save( cell, spiceFlags )
    for instance in cell.getInstances():
        #print( '     {}| {}.'.format(' '*(depth*2), instance) )
        masterCell = instance.getMasterCell()
        if not masterCell.isTerminalNetlist():
            rsave( masterCell, views, depth+1, enableSpice )
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
       #setTraceLevel( 550 )
        cell, editor = plugins.kwParseMain( **kw )
        views        = CRL.Catalog.State.Physical
        if 'views' in kw: views = kw['views']
        if not cell:
            print( WarningMessage( 'No Cell loaded in the editor (yet), nothing done.' ) )
            return 0
        rsave( cell, views )
        CRL.destroyAllVHDL()
    except Exception as e:
      catch( e )
    sys.stdout.flush()
    sys.stderr.flush()
    return 0

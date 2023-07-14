#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2017-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |        T o o l E n g i n e   T u t o r i a l                    |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./runDemo.py"                             |
# +-----------------------------------------------------------------+


try:
    import sys
    import os.path
    from   coriolis.Hurricane       import DataBase, Breakpoint, DbU, Box, Contact, \
                                           Vertical, Horizontal, Net, Cell
    from   coriolis                 import Cfg
    from   coriolis                 import Viewer
    from   coriolis                 import CRL
    from   coriolis.helpers         import trace, l, u, n
    from   coriolis.helpers.io      import showPythonTrace, ErrorMessage, WarningMessage
    from   coriolis.helpers.overlay import UpdateSession
    import coriolis.cumulus.plugins as plugins
except ImportError as e:
    serror = str(e)
    if serror.startswith('No module named'):
        module = serror.split()[-1]
        print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(module) )
        print( '        Please check the integrity of the "coriolis" package.' )
    if serror.find('cannot open shared object file'):
        library = serror.split(':')[0]
        print( '[ERROR] The "{}" shared library cannot be loaded.'.format(library) )
    sys.exit(1)
except Exception as e:
    print( '[ERROR] A strange exception occurred while loading the basic Coriolis/Python' )
    print( '        modules. Something may be wrong at Python/C API level.\n' )
    print( '        {}'.format(e) )
    sys.exit(2)



def runDemo ( cell, editor ):
    print( 'runDemo() Python plugin function has been called.' )
    if cell:
        print( WarningMessage( 'A Cell "{}" is already loaded in the Viewer, cowardly exiting.' \
                               .format(cell.getName()) ))
        return
    library = CRL.AllianceFramework.get().getLibrary( 0 ) 
    cell    = CRL.AllianceFramework.get().getCell( 'demo_cell', CRL.Catalog.State.Views )
    if cell:
       print( WarningMessage( '"demo_cell" has already been created, do not run twice.' ))
       return
    with UpdateSession():
        cell = Cell.create( library, 'demo_cell' )
        cell.setAbutmentBox( Box( l( 0.0), l( 0.0), l(15.0), l(50.0) ) )

    if editor: editor.setCell( cell )
    Breakpoint.stop( 1, 'Abutment box has been drawn.' )
    with UpdateSession():
        technology = DataBase.getDB().getTechnology()
        nwell      = technology.getLayer( 'NWELL' )
        ntrans     = technology.getLayer( 'NTRANS' )
        ptrans     = technology.getLayer( 'PTRANS' )
        diffN      = technology.getLayer( 'NDIF' )
        diffP      = technology.getLayer( 'PDIF' )
        contDiffN  = technology.getLayer( 'CONT_DIF_N' )
        contDiffP  = technology.getLayer( 'CONT_DIF_P' )
        contPoly   = technology.getLayer( 'CONT_POLY' )
        poly       = technology.getLayer( 'POLY' )
        metal1     = technology.getLayer( 'METAL1' )
        
        nwellNet = Net.create( cell, 'nwell' )
        Horizontal.create( nwellNet, nwell, l(39.0), l(24.0), l(0.0), l(15.0) )
        vss = Net.create( cell, 'vss' )
        vdd = Net.create( cell, 'vdd' )
        Horizontal.create( vss, metal1   , l(3.0), l(6.0), l(0.0), l(15.0) )
        Vertical.create  ( vss, diffN    , l(3.5), l(4.0), l(4.0), l(12.0) )
        Contact.create   ( vss, contDiffN, l(4.0), l(5.0) )
        Horizontal.create( vdd, metal1   , l(47.0), l( 6.0), l( 0.0), l(15.0) )
        Vertical.create  ( vdd, diffP    , l( 3.5), l( 4.0), l(28.0), l(46.0) )
        Contact.create   ( vdd, contDiffP, l( 4.0), l(45.0) )
    Breakpoint.stop( 1, 'Power nets have been drawn.' )

    with UpdateSession():
        nq = Net.create( cell, 'nq' )
        Vertical.create  ( nq, diffN    , l(10.0), l( 3.0), l( 8.0), l(12.0) )
        Vertical.create  ( nq, diffP    , l(10.0), l( 3.0), l(28.0), l(37.0) )
        Contact.create   ( nq, contDiffN, l(10.0), l(10.0) )
        Contact.create   ( nq, contDiffP, l(10.0), l(30.0) )
        Contact.create   ( nq, contDiffP, l(10.0), l(35.0) )
        Vertical.create  ( nq, metal1   , l(10.0), l( 2.0), l(10.0), l(40.0) )
    Breakpoint.stop( 1, 'Output has been drawn.' )

    with UpdateSession():
        i = Net.create( cell, 'i' )
        Vertical.create  ( i, ntrans    , l( 7.0), l( 1.0), l( 6.0), l(14.0) )
        Vertical.create  ( i, poly      , l( 7.0), l( 1.0), l(14.0), l(26.0) )
        Vertical.create  ( i, ptrans    , l( 7.0), l( 1.0), l(26.0), l(39.0) )
        Horizontal.create( i, poly      , l(20.0), l( 3.0), l( 4.0), l( 7.0) )
        Contact.create   ( i, contPoly  , l( 5.0), l(20.0) )
        Vertical.create  ( i, metal1    , l( 5.0), l( 2.0), l(10.0), l(40.0) )
    Breakpoint.stop( 1, 'Input has been drawn.' )


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    """ 
    The mandatory function to make the plugin appears in the menus.
    """

    editor = kw['editor']
    editor.addMenu( 'tutorials'        , 'Tutorials', Viewer.CellViewer.TopMenu )
   #editor.addMenu( 'tutorials.plugins', 'Plugins'  , Viewer.CellViewer.NoFlags )
    plugins.kwUnicornHook( 'tutorials.runDemo'
                         , 'Run Demo (Python flavor)'
                         , 'Launch runDemo() in the Tutorial Python plugin.'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    """ 
    The mandatory function from which a plugin will be called by Coriolis CGT/Unicorn.
    """

    try:
        helpers.staticInitialization( quiet=True )
       #helpers.setTraceLevel( 550 )
        Breakpoint.setStopLevel( 1 )
        print( '  o  Breakpoint level: {}.'.format(Breakpoint.getStopLevel()) )
        cell, editor = plugins.kwParseMain( **kw )
        runDemo( cell, editor )
    except Exception as e:
        helpers.io.catch( e )
    return 0

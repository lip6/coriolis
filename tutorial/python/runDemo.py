#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2017-2021, All Rights Reserved
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
    import Cfg
    import Viewer
    import CRL
    import helpers
    from   helpers.io import ErrorMessage
    from   helpers.io import WarningMessage
    from   helpers    import showPythonTrace
    import plugins
    from   Hurricane  import DataBase
    from   Hurricane  import Breakpoint
    from   Hurricane  import UpdateSession
    from   Hurricane  import DbU
    from   Hurricane  import Box
    from   Hurricane  import Contact
    from   Hurricane  import Vertical
    from   Hurricane  import Horizontal
    from   Hurricane  import Net
    from   Hurricane  import Cell
except ImportError as e:
    serror = str(e)
    if serror.startswith('No module named'):
        module = serror.split()[-1]
        print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(module) )
        print( '        Please check the integrity of the "coriolis" package.' )
    if serror.find('cannot open shared object file'):
        library = serror.split(':')[0]
        print( '[ERROR] The "{}" shared library cannot be loaded.'.format(library) )
        print( '        Under RHEL 6, you must be under devtoolset-2.' )
        print( '        (scl enable devtoolset-2 bash)' )
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
    UpdateSession.open()
    cell = Cell.create( library, 'demo_cell' )
    cell.setAbutmentBox( Box( DbU.fromLambda( 0.0), DbU.fromLambda( 0.0)
                            , DbU.fromLambda(15.0), DbU.fromLambda(50.0) ) )
    UpdateSession.close()

    if editor: editor.setCell( cell )
    Breakpoint.stop( 1, 'Abutment box has been drawn.' )
    UpdateSession.open()

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
    Horizontal.create( nwellNet, nwell, DbU.fromLambda(39.0), DbU.fromLambda(24.0), DbU.fromLambda(0.0), DbU.fromLambda(15.0) )
    vss = Net.create( cell, 'vss' )
    vdd = Net.create( cell, 'vdd' )
    Horizontal.create( vss, metal1   , DbU.fromLambda(3.0), DbU.fromLambda(6.0), DbU.fromLambda(0.0), DbU.fromLambda(15.0) )
    Vertical.create  ( vss, diffN    , DbU.fromLambda(3.5), DbU.fromLambda(4.0), DbU.fromLambda(4.0), DbU.fromLambda(12.0) )
    Contact.create   ( vss, contDiffN, DbU.fromLambda(4.0), DbU.fromLambda(5.0) )
    Horizontal.create( vdd, metal1   , DbU.fromLambda(47.0), DbU.fromLambda( 6.0), DbU.fromLambda( 0.0), DbU.fromLambda(15.0) )
    Vertical.create  ( vdd, diffP    , DbU.fromLambda( 3.5), DbU.fromLambda( 4.0), DbU.fromLambda(28.0), DbU.fromLambda(46.0) )
    Contact.create   ( vdd, contDiffP, DbU.fromLambda( 4.0), DbU.fromLambda(45.0) )
    UpdateSession.close()
    Breakpoint.stop( 1, 'Power nets have been drawn.' )

    UpdateSession.open()
    nq = Net.create( cell, 'nq' )
    Vertical.create  ( nq, diffN    , DbU.fromLambda(10.0), DbU.fromLambda( 3.0), DbU.fromLambda( 8.0), DbU.fromLambda(12.0) )
    Vertical.create  ( nq, diffP    , DbU.fromLambda(10.0), DbU.fromLambda( 3.0), DbU.fromLambda(28.0), DbU.fromLambda(37.0) )
    Contact.create   ( nq, contDiffN, DbU.fromLambda(10.0), DbU.fromLambda(10.0) )
    Contact.create   ( nq, contDiffP, DbU.fromLambda(10.0), DbU.fromLambda(30.0) )
    Contact.create   ( nq, contDiffP, DbU.fromLambda(10.0), DbU.fromLambda(35.0) )
    Vertical.create  ( nq, metal1   , DbU.fromLambda(10.0), DbU.fromLambda( 2.0), DbU.fromLambda(10.0), DbU.fromLambda(40.0) )
    UpdateSession.close()
    Breakpoint.stop( 1, 'Output has been drawn.' )

    UpdateSession.open()
    i = Net.create( cell, 'i' )
    Vertical.create  ( i, ntrans    , DbU.fromLambda( 7.0), DbU.fromLambda( 1.0), DbU.fromLambda( 6.0), DbU.fromLambda(14.0) )
    Vertical.create  ( i, poly      , DbU.fromLambda( 7.0), DbU.fromLambda( 1.0), DbU.fromLambda(14.0), DbU.fromLambda(26.0) )
    Vertical.create  ( i, ptrans    , DbU.fromLambda( 7.0), DbU.fromLambda( 1.0), DbU.fromLambda(26.0), DbU.fromLambda(39.0) )
    Horizontal.create( i, poly      , DbU.fromLambda(20.0), DbU.fromLambda( 3.0), DbU.fromLambda( 4.0), DbU.fromLambda( 7.0) )
    Contact.create   ( i, contPoly  , DbU.fromLambda( 5.0), DbU.fromLambda(20.0) )
    Vertical.create  ( i, metal1    , DbU.fromLambda( 5.0), DbU.fromLambda( 2.0), DbU.fromLambda(10.0), DbU.fromLambda(40.0) )
    
    UpdateSession.close()
    Breakpoint.stop( 1, 'Input has been drawn.' )
    return


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
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

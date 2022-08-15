#!/usr/bin/python

import sys
from   Hurricane import DataBase, NetExternalComponents, Net, \
                        DbU, Point, Box, Horizontal, Vertical, Contact, RoutingPad, \
                        Breakpoint
from   CRL       import AllianceFramework, Catalog
from   helpers   import l
from   helpers.overlay import UpdateSession


def doBreak ( level, message ):
    """Put a breakpoint into the script."""
    Breakpoint.stop( level, message )


def buildInvertor ( editor ):
    """Build step by step an invertor standard cell."""
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'invertor' )
        cell.setTerminalNetlist( True )
        cell.setAbutmentBox( Box( l(0.0), l(0.0), l(15.0), l(50.0) ) )
    if editor:
        editor.setCell( cell )
        editor.fit()
    
    technology = DataBase.getDB().getTechnology()
    metal1     = technology.getLayer( "METAL1"     ) 
    poly       = technology.getLayer( "POLY"       )
    ptrans     = technology.getLayer( "PTRANS"     )
    ntrans     = technology.getLayer( "NTRANS"     )
    pdif       = technology.getLayer( "PDIF"       )
    ndif       = technology.getLayer( "NDIF"       )
    contdifn   = technology.getLayer( "CONT_DIF_N" )
    contdifp   = technology.getLayer( "CONT_DIF_P" )
    nwell      = technology.getLayer( "NWELL"      )
    contpoly   = technology.getLayer( "CONT_POLY"  )
    ntie       = technology.getLayer( "NTIE"       )

    with UpdateSession():
        net = Net.create( cell, "nwell" )
        Vertical.create( net, nwell, l(7.5), l(15.0), l(27.0), l(51.0) )
        
        vdd = Net.create( cell, "vdd" )
        vdd.setExternal( True )
        vdd.setGlobal  ( True )
        h = Horizontal.create(vdd, metal1, l(47.0), l(6.0), l(0.0), l(15.0) )
        NetExternalComponents.setExternal( h )
        Contact.create ( vdd, contdifn, l(10.0), l(47.0), l( 1.0), l( 1.0) )
        Contact.create ( vdd, contdifp, l( 4.0), l(45.0), l( 1.0), l( 1.0) )
        Vertical.create( vdd, pdif    , l( 3.5), l( 4.0), l(28.0), l(46.0) )
        Vertical.create( vdd, ntie    , l(10.0), l( 3.0), l(43.0), l(48.0) )
    doBreak( 1, 'Done building vdd.' )
    
    with UpdateSession():
        vss = Net.create( cell, "vss" )
        vss.setExternal( True )
        vss.setGlobal  ( True )
        h = Horizontal.create(vss, metal1, l(3.0), l(6.0), l(0.0), l(15.0))
        NetExternalComponents.setExternal( h )
        Vertical.create( vss, ndif    , l(3.5), l(4.0), l(4.0), l(12.0) )
        Contact.create ( vss, contdifn, l(4.0), l(5.0), l(1.0), l( 1.0) )
    doBreak( 1, 'Done building vss.' )
    
    with UpdateSession():
        i = Net.create( cell, "i" )
        i.setExternal( True )
        v = Vertical.create ( i, metal1, l(5.0), l(2.0), l(10.0), l(40.0) )
        NetExternalComponents.setExternal( v )
        Vertical.create  ( i, ptrans  , l( 7.0), l( 1.0), l(26.0), l(39.0) )
        Vertical.create  ( i, ntrans  , l( 7.0), l( 1.0), l( 6.0), l(14.0) )
        Vertical.create  ( i, poly    , l( 7.0), l( 1.0), l(14.0), l(26.0) )
        Horizontal.create( i, poly    , l(20.0), l( 3.0), l( 4.0), l( 7.0) )
        Contact.create   ( i, contpoly, l( 5.0), l(20.0), l( 1.0), l( 1.0) )
    doBreak( 1, 'Done building i.' )
    
    with UpdateSession():
        nq = Net.create ( cell, "nq" )
        nq.setExternal( True )
        v = Vertical.create( nq, metal1, l(10.0), l(2.0), l(10.0), l(40.0) )
        NetExternalComponents.setExternal( v )
        Vertical.create( nq, pdif    , l(10.0), l( 3.0), l(28.0), l(37.0) )
        Vertical.create( nq, ndif    , l(10.0), l( 3.0), l( 8.0), l(12.0) )
        Contact.create ( nq, contdifp, l(10.0), l(35.0), l( 1.0), l( 1.0) )
        Contact.create ( nq, contdifp, l(10.0), l(30.5), l( 1.0), l( 1.0) )
        Contact.create ( nq, contdifn, l(10.0), l(10.0), l( 1.0), l( 1.0) )
    doBreak( 1, 'Done building q.' )

    AllianceFramework.get().saveCell( cell, Catalog.State.Views )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    buildInvertor( editor )
    return True 

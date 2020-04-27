#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *


def toDbU ( l ): return DbU.fromLambda(l)


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def buildInvertor ( editor ):
    UpdateSession.open()

    cell = AllianceFramework.get().createCell( 'invertor' )
    cell.setTerminal( True )

    cell.setAbutmentBox( Box( toDbU(0.0), toDbU(0.0), toDbU(15.0), toDbU(50.0) ) )

    if editor:
      UpdateSession.close()
      editor.setCell( cell )
      editor.fit()
      UpdateSession.open()
    
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

    net = Net.create( cell, "nwell" )
    Vertical.create( net, nwell, toDbU(7.5), toDbU(15.0), toDbU(27.0), toDbU(51.0) )
    
    vdd = Net.create( cell, "vdd" )
    vdd.setExternal( True )
    vdd.setGlobal  ( True )
    h = Horizontal.create(vdd, metal1, toDbU(47.0), toDbU(6.0), toDbU(0.0), toDbU(15.0) )
    NetExternalComponents.setExternal( h )
    Contact.create ( vdd, contdifn, toDbU(10.0), toDbU(47.0), toDbU( 1.0), toDbU( 1.0) )
    Contact.create ( vdd, contdifp, toDbU( 4.0), toDbU(45.0), toDbU( 1.0), toDbU( 1.0) )
    Vertical.create( vdd, pdif    , toDbU( 3.5), toDbU( 4.0), toDbU(28.0), toDbU(46.0) )
    Vertical.create( vdd, ntie    , toDbU(10.0), toDbU( 3.0), toDbU(43.0), toDbU(48.0) )
    doBreak( 1, 'Done building vdd.' )
    
    vss = Net.create( cell, "vss" )
    vss.setExternal( True )
    vss.setGlobal  ( True )
    h = Horizontal.create(vss, metal1, toDbU(3.0), toDbU(6.0), toDbU(0.0), toDbU(15.0))
    NetExternalComponents.setExternal( h )
    Vertical.create( vss, ndif    , toDbU(3.5), toDbU(4.0), toDbU(4.0), toDbU(12.0) )
    Contact.create ( vss, contdifn, toDbU(4.0), toDbU(5.0), toDbU(1.0), toDbU( 1.0) )
    doBreak( 1, 'Done building vss.' )
    
    i = Net.create( cell, "i" )
    i.setExternal( True )
    v = Vertical.create ( i, metal1, toDbU(5.0), toDbU(2.0), toDbU(10.0), toDbU(40.0) )
    NetExternalComponents.setExternal( v )
    Vertical.create  ( i, ptrans  , toDbU( 7.0), toDbU( 1.0), toDbU(26.0), toDbU(39.0) )
    Vertical.create  ( i, ntrans  , toDbU( 7.0), toDbU( 1.0), toDbU( 6.0), toDbU(14.0) )
    Vertical.create  ( i, poly    , toDbU( 7.0), toDbU( 1.0), toDbU(14.0), toDbU(26.0) )
    Horizontal.create( i, poly    , toDbU(20.0), toDbU( 3.0), toDbU( 4.0), toDbU( 7.0) )
    Contact.create   ( i, contpoly, toDbU( 5.0), toDbU(20.0), toDbU( 1.0), toDbU( 1.0) )
    doBreak( 1, 'Done building i.' )
    
    nq = Net.create ( cell, "nq" )
    nq.setExternal( True )
    v = Vertical.create( nq, metal1, toDbU(10.0), toDbU(2.0), toDbU(10.0), toDbU(40.0) )
    NetExternalComponents.setExternal( v )
    Vertical.create( nq, pdif    , toDbU(10.0), toDbU( 3.0), toDbU(28.0), toDbU(37.0) )
    Vertical.create( nq, ndif    , toDbU(10.0), toDbU( 3.0), toDbU( 8.0), toDbU(12.0) )
    Contact.create ( nq, contdifp, toDbU(10.0), toDbU(35.0), toDbU( 1.0), toDbU( 1.0) )
    Contact.create ( nq, contdifp, toDbU(10.0), toDbU(30.5), toDbU( 1.0), toDbU( 1.0) )
    Contact.create ( nq, contdifn, toDbU(10.0), toDbU(10.0), toDbU( 1.0), toDbU( 1.0) )
    doBreak( 1, 'Done building q.' )

    UpdateSession.close()

    AllianceFramework.get().saveCell( cell, Catalog.State.Views )
    return


def scriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    buildInvertor( editor )
    return True 

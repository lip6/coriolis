#!/usr/bin/python

import sys
from   Hurricane import DataBase, DbU, Point, Box, Net, Vertical, Horizontal, Diagonal
from   CRL       import AllianceFramework, Catalog, Gds
from   helpers   import l
from   helpers.overlay import UpdateSession


def buildDiagonals ( editor ):
    """Check the Diagonal segments of Hurricane."""
    DbU.setPolygonStep( l(1.0) )
    
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'diagonal' )
        cell.setTerminalNetlist( True )

    cell.setAbutmentBox( Box( l(-5.0), l(-5.0), l(65.0), l(75.0) ) )

    if editor:
        editor.setCell( cell )
        editor.fit()
    
    technology = DataBase.getDB().getTechnology()
    metal1     = technology.getLayer( "metal1"     ) 
    metal2     = technology.getLayer( "metal2"     ) 
    metal4     = technology.getLayer( "metal4"     ) 
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
        net = Net.create( cell, 'my_net' )
        net.setExternal( True )
        
        Diagonal.create( net, metal2
                       , Point( l(20.0), l(10.0) )
                       , Point( l(10.0), l(20.0) )
                       , l(4.0)
                       )
        Vertical.create( net, metal2, l(10.0), l(4.0), l(20.0), l(30.0) )
        Diagonal.create( net, metal2
                       , Point( l(10.0), l(30.0) )
                       , Point( l(20.0), l(40.0) )
                       , l(4.0)
                       )
        Horizontal.create( net, metal2, l(40.0), l(4.0), l(20.0), l(30.0) )
        Diagonal.create( net, metal2
                       , Point( l(30.0), l(40.0) )
                       , Point( l(40.0), l(30.0) )
                       , l(4.0)
                       )
        Vertical.create( net, metal2, l(40.0), l(4.0), l(30.0), l(20.0) )
        Diagonal.create( net, metal2
                       , Point( l(40.0), l(20.0) )
                       , Point( l(30.0), l(10.0) )
                       , l(4.0)
                       )
        Horizontal.create( net, metal2, l(10.0), l(4.0), l(30.0), l(20.0) )

    Gds.save( cell )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    buildDiagonals( editor )
    return True 

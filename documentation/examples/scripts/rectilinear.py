#!/usr/bin/python

import sys
from   Hurricane import DataBase, Net, \
                        DbU, Point, Box, Rectilinear
from   CRL       import AllianceFramework, Catalog, Gds
from   helpers   import l, u
from   helpers.overlay import UpdateSession


def buildRectilinear ( editor ):
    """Check Hurricane.Rectilinear class."""
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'Rectilinear' )
        cell.setTerminalNetlist( True )
        cell.setAbutmentBox( Box( l(-5.0), l(-5.0), l(65.0), l(75.0) ) )
       #cell.setAbutmentBox( Box( l(-5.0), l(-5.0), l(21.0), l(35.0) ) )
    if editor:
        editor.setCell( cell )
        editor.fit()
    
    technology = DataBase.getDB().getTechnology()
    metal1     = technology.getLayer( "METAL1"     ) 
    metal2     = technology.getLayer( "METAL2"     ) 
    metal3     = technology.getLayer( "METAL3"     ) 
    metal4     = technology.getLayer( "METAL4"     ) 
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
    
        points = [ Point( l(  0.0), l(  0.0) )
                 , Point( l(  0.0), l( 10.0) )
                 , Point( l( 20.0), l( 30.0) )
                 , Point( l( 30.0), l( 30.0) )
                 , Point( l( 30.0), l( 20.0) )
                 , Point( l( 10.0), l(  0.0) ) ]
        r = Rectilinear.create( net, metal2, points )
    
    
       #print( 'Normalized and manhattanized contour:' )
       #i = 0
       #for point in p.getMContour():
       #  print( '| %d '%i, point, '[%fum %fum]' % ( u(point.getX()), u(point.getY()) ))
       #  i += 1

    Gds.save( cell )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    buildRectilinear( editor )
    return True 

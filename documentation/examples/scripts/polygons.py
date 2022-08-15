#!/usr/bin/python

import sys
from   Hurricane import DataBase, NetExternalComponents, Net, \
                        DbU, Point, Box, Polygon
from   CRL       import AllianceFramework, Catalog, Gds
from   helpers   import l, u
from   helpers.overlay import UpdateSession


def buildPolygons ( editor ):
    """Draw a set of Polygons checking all possible combination."""
    #DbU.setPolygonStep( l(0.1) )
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'polygons' )
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
        #points = [ Point( l(  0.0), l(  0.0) )
        #         , Point( l( 10.0), l(  0.0) )
        #         , Point( l(  7.0), l(  8.0) )
        #         , Point( l(  3.0), l(  8.0) ) ]
        #p = Polygon.create( net, metal4, points )
    
        # Counter-clockwise, slope > 1.
        points = [ Point( l(  3.0), l(  0.0) )
                 , Point( l( 13.0), l(  0.0) )
                 , Point( l( 16.0), l( 10.0) )
                 , Point( l( 16.0), l( 20.0) )
                 , Point( l( 13.0), l( 30.0) )
                 , Point( l(  3.0), l( 30.0) )
                 , Point( l(  0.0), l( 20.0) )
                 , Point( l(  0.0), l( 10.0) ) ]
        p = Polygon.create( net, metal2, points )
        #p.translate( l(40.0), l(0.0) )
    
        # clockwise, slope > 1.
        points = [ Point( l(  0.0), l( 10.0) )
                 , Point( l(  0.0), l( 20.0) )
                 , Point( l(  3.0), l( 30.0) )
                 , Point( l( 13.0), l( 30.0) )
                 , Point( l( 16.0), l( 20.0) )
                 , Point( l( 16.0), l( 10.0) )
                 , Point( l( 13.0), l(  0.0) )
                 , Point( l(  3.0), l(  0.0) ) ]
        p = Polygon.create( net, metal2, points )
        p.translate( l(0.0), l(40.0) )
    
        # Counter-clockwise, slope < 1.
        points = [ Point( l( 10.0), l(  0.0) )
                 , Point( l( 20.0), l(  0.0) )
                 , Point( l( 30.0), l(  3.0) )
                 , Point( l( 30.0), l( 13.0) )
                 , Point( l( 20.0), l( 16.0) )
                 , Point( l( 10.0), l( 16.0) )
                 , Point( l(  0.0), l( 13.0) )
                 , Point( l(  0.0), l(  3.0) ) ]
        p = Polygon.create( net, metal2, points )
        p.translate( l(30.0), l(0.0) )
    
        # clockwise.
        points = [ Point( l(  0.0), l(  3.0) ) 
                 , Point( l(  0.0), l( 13.0) )
                 , Point( l( 10.0), l( 16.0) )
                 , Point( l( 20.0), l( 16.0) )
                 , Point( l( 30.0), l( 13.0) )
                 , Point( l( 30.0), l(  3.0) )
                 , Point( l( 20.0), l(  0.0) )
                 , Point( l( 10.0), l(  0.0) ) ]
        p = Polygon.create( net, metal2, points )
        p.translate( l(30.0), l(40.0) )
    
        # Big parallelogram.
        points = [ Point( l(  0.0), l(  0.0) ) 
                 , Point( l( 20.0), l( 20.0) )
                 , Point( l( 60.0), l( 20.0) )
                 , Point( l( 40.0), l(  0.0) ) ]
        p = Polygon.create( net, metal3, points )
        p.translate( l(70.0), l(0.0) )
        
        points = [ Point( l(  0.0), l( 20.0) ) 
                 , Point( l( 40.0), l( 20.0) )
                 , Point( l( 60.0), l(  0.0) )
                 , Point( l( 20.0), l(  0.0) ) ]
        p = Polygon.create( net, metal3, points )
        p.translate( l(140.0), l(0.0) )
        
        points = [ Point( l(  0.0), l(  0.0) ) 
                 , Point( l(  0.0), l( 10.0) )
                 , Point( l( 20.0), l( 30.0) )
                 , Point( l( 60.0), l( 30.0) )
                 , Point( l( 60.0), l( 20.0) )
                 , Point( l( 40.0), l(  0.0) ) ]
        p = Polygon.create( net, metal3, points )
        p.translate( l(70.0), l(40.0) )
        
        points = [ Point( l(  0.0), l( 20.0) ) 
                 , Point( l(  0.0), l( 30.0) )
                 , Point( l( 40.0), l( 30.0) )
                 , Point( l( 60.0), l( 10.0) )
                 , Point( l( 60.0), l(  0.0) )
                 , Point( l( 20.0), l(  0.0) ) ]
        p = Polygon.create( net, metal3, points )
        p.translate( l(140.0), l(40.0) )
    
        print( 'Normalized and manhattanized contour:' )
        i = 0
        for point in p.getMContour():
          print( '| %d '%i, point, '[%fum %fum]' % ( u(point.getX()), u(point.getY()) ))
          i += 1
    
        print( 'Sub-polygons (for GDSII generation)' )
        subpolygons = p.getSubPolygons()
        for i in range(len(subpolygons)):
          print( '+ Sub-Polygon %d:' % i )
          for j in range(len(subpolygons[i])):
            print( '  [%3d]' % j, subpolygons[i][j] \
                 , '[%fum %fum]' % ( u(subpolygons[i][j].getX()), u(subpolygons[i][j].getY()) ))

    Gds.save( cell )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    buildPolygons( editor )
    return True 

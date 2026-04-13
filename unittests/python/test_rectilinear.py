#!/usr/bin/python

import sys
from   coriolis.Hurricane       import DataBase, Net, \
                                       DbU, Point, Box, Pad, Rectilinear
from   coriolis                 import Cfg
from   coriolis.CRL             import AllianceFramework, Catalog, Gds
from   coriolis.helpers         import l, u
from   coriolis.helpers.overlay import CfgCache, UpdateSession


def testRectilinear ( editor ):
    """Check Hurricane.Rectilinear class."""
    with CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
        cfg.misc.minTraceLevel       = 17000
        cfg.misc.maxTraceLevel       = 18000
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'Rectilinear' )
        cell.setTerminalNetlist( True )
        cell.setAbutmentBox( Box( l(-5.0), l(-5.0), l(704.0), l(200.0) ) )
       #cell.setAbutmentBox( Box( l(-5.0), l(-5.0), l(21.0), l(35.0) ) )
    if editor:
        editor.setCell( cell )
        editor.fit()
    
    technology = DataBase.getDB().getTechnology()
    metal1     = technology.getLayer( "METAL1"     ) 
    metal2     = technology.getLayer( "METAL2"     ) 
    metal3     = technology.getLayer( "METAL3"     ) 
    metal4     = technology.getLayer( "METAL4"     ) 
    metal5     = technology.getLayer( "METAL5"     ) 
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
                 , Point( l( 10.0), l(  0.0) )  
                 , Point( l(  0.0), l(  0.0) ) ]
        r = Rectilinear.create( net, metal2, points )
    
       #print( 'Normalized and manhattanized contour:' )
       #i = 0
       #for point in p.getMContour():
       #  print( '| %d '%i, point, '[%fum %fum]' % ( u(point.getX()), u(point.getY()) ))
       #  i += 1
    
       #points = [ Point( l(  0.0), l( 40.0) )    # 0
       #         , Point( l( 30.0), l( 40.0) )    # 1
       #         , Point( l( 30.0), l( 60.0) )    # 2
       #         , Point( l( 50.0), l( 60.0) )    # 3
       #         , Point( l( 50.0), l( 80.0) )    # 4
       #         , Point( l( 90.0), l( 80.0) )    # 5
       #         , Point( l( 90.0), l( 50.0) )    # 6
       #         , Point( l( 60.0), l( 50.0) )    # 7
       #         , Point( l( 60.0), l( 30.0) )    # 8
       #         , Point( l( 70.0), l( 30.0) )    # 9
       #         , Point( l( 70.0), l( 20.0) )    # 10
       #         , Point( l( 90.0), l( 20.0) )    # 11
       #         , Point( l( 90.0), l(  0.0) )    # 12
       #         , Point( l( 20.0), l(  0.0) )    # 13
       #         , Point( l( 20.0), l( 20.0) )    # 14
       #         , Point( l(  0.0), l( 20.0) )    # 15
       #         , Point( l(  0.0), l( 40.0) ) ]  # 16
       # Super-test rectilinear.
        points = [ Point( l(  0.0), l(  0.0) )    # 0
                 , Point( l(  0.0), l( 20.0) )    # 1
                 , Point( l( 10.0), l( 20.0) )    # 2
                 , Point( l( 10.0), l( 30.0) )    # 3
                 , Point( l( 20.0), l( 30.0) )    # 4
                 , Point( l( 20.0), l( 40.0) )    # 5
                 , Point( l( 40.0), l( 40.0) )    # 6
                 , Point( l( 40.0), l( 80.0) )    # 7
                 , Point( l( 20.0), l( 80.0) )    # 8
                 , Point( l( 20.0), l( 70.0) )    # 9
        
                 , Point( l( 10.0), l( 70.0) )    # 10
                 , Point( l( 10.0), l( 60.0) )    # 11
                 , Point( l(  0.0), l( 60.0) )    # 12
                 , Point( l(  0.0), l(120.0) )    # 13
                 , Point( l( 10.0), l(120.0) )    # 14
                 , Point( l( 10.0), l(110.0) )    # 15
                 , Point( l( 20.0), l(110.0) )    # 16
                 , Point( l( 20.0), l(100.0) )    # 17
                 , Point( l( 40.0), l(100.0) )    # 18
                 , Point( l( 40.0), l(140.0) )    # 19
        
                 , Point( l( 20.0), l(140.0) )    # 20
                 , Point( l( 20.0), l(150.0) )    # 21
                 , Point( l( 10.0), l(150.0) )    # 22
                 , Point( l( 10.0), l(160.0) )    # 23
                 , Point( l(  0.0), l(160.0) )    # 24
                 , Point( l(  0.0), l(180.0) )    # 25
                 , Point( l( 40.0), l(180.0) )    # 26
                 , Point( l( 40.0), l(170.0) )    # 27
                 , Point( l( 50.0), l(170.0) )    # 28
                 , Point( l( 50.0), l(160.0) )    # 29
        
                 , Point( l(150.0), l(160.0) )    # 30
                 , Point( l(150.0), l(150.0) )    # 31
                 , Point( l(130.0), l(150.0) )    # 32
                 , Point( l(130.0), l(140.0) )    # 33
                 , Point( l(120.0), l(140.0) )    # 34
                 , Point( l(120.0), l(130.0) )    # 35
                 , Point( l(110.0), l(130.0) )    # 36
                 , Point( l(110.0), l(110.0) )    # 37
                 , Point( l(120.0), l(110.0) )    # 38
                 , Point( l(120.0), l(100.0) )    # 39
        
                 , Point( l(130.0), l(100.0) )    # 40
                 , Point( l(130.0), l( 90.0) )    # 41
                 , Point( l(150.0), l( 90.0) )    # 42
                 , Point( l(150.0), l( 80.0) )    # 43
                 , Point( l(120.0), l( 80.0) )    # 44
                 , Point( l(120.0), l( 70.0) )    # 45
                 , Point( l(110.0), l( 70.0) )    # 46
                 , Point( l(110.0), l( 50.0) )    # 47
                 , Point( l(120.0), l( 50.0) )    # 48
                 , Point( l(120.0), l( 40.0) )    # 49
        
                 , Point( l(130.0), l( 40.0) )    # 50
                 , Point( l(130.0), l( 30.0) )    # 51
                 , Point( l(150.0), l( 30.0) )    # 52
                 , Point( l(150.0), l( 20.0) )    # 53
                 , Point( l( 50.0), l( 20.0) )    # 54
                 , Point( l( 50.0), l( 10.0) )    # 55
                 , Point( l( 40.0), l( 10.0) )    # 56
                 , Point( l( 40.0), l(  0.0) )    # 57
                 , Point( l(  0.0), l(  0.0) ) ]  # 57
       #points = [ Point( l(  0.0), l(  0.0) )    # 0
       #         , Point( l(  0.0), l( 80.0) )    # 1
       #         , Point( l( 40.0), l( 80.0) )    # 2
       #         , Point( l( 40.0), l( 60.0) )    # 3
       #         , Point( l( 20.0), l( 60.0) )    # 4
       #         , Point( l( 20.0), l(  0.0) )    # 5
       #         , Point( l(  0.0), l(  0.0) ) ]  # 6
        r = Rectilinear.create( net, metal2, points )

        boxes = []
        r.getAsRectangles( boxes )
       #print( 'boxes={}'.format( boxes ))
        for box in boxes:
            box.translate( l(180.0), l(0.0) )
            Pad.create( net, metal3, box )
        boxes = []
        r.getAsBiggestRectangles( boxes, l(15.0), l(15.0) )
       #print( 'boxes={}'.format( boxes ))
        for box in boxes:
            box.translate( l(360.0), l(0.0) )
            Pad.create( net, metal4, box )
        boxes = []
        r.getAsRectangles( boxes, Rectilinear.HSliced )
       #print( 'boxes={}'.format( boxes ))
        for box in boxes:
            box.translate( l(540.0), l(0.0) )
            Pad.create( net, metal5, box )

    Gds.save( cell )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    testRectilinear( editor )
    return True 

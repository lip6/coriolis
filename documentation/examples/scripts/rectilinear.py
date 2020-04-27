#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *


def toDbU    ( l ): return DbU.fromLambda(l)
def toMicron ( u ): return DbU.toPhysical( u, DbU.UnitPowerMicro )


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def buildRectilinear ( editor ):
    UpdateSession.open()

    cell = AllianceFramework.get().createCell( 'Rectilinear' )
    cell.setTerminal( True )

    cell.setAbutmentBox( Box( toDbU(-5.0), toDbU(-5.0), toDbU(65.0), toDbU(75.0) ) )
   #cell.setAbutmentBox( Box( toDbU(-5.0), toDbU(-5.0), toDbU(21.0), toDbU(35.0) ) )

    if editor:
      UpdateSession.close()
      editor.setCell( cell )
      editor.fit()
      UpdateSession.open()
    
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


    net = Net.create( cell, 'my_net' )
    net.setExternal( True )

    points = [ Point( toDbU(  0.0), toDbU(  0.0) )
             , Point( toDbU(  0.0), toDbU( 10.0) )
             , Point( toDbU( 20.0), toDbU( 30.0) )
             , Point( toDbU( 30.0), toDbU( 30.0) )
             , Point( toDbU( 30.0), toDbU( 20.0) )
             , Point( toDbU( 10.0), toDbU(  0.0) ) ]
    r = Rectilinear.create( net, metal2, points )


   #print 'Normalized and manhattanized contour:'
   #i = 0
   #for point in p.getMContour():
   #  print '| %d '%i, point \
   #      , '[%fum %fum]' % ( toMicron(point.getX()) \
   #                        , toMicron(point.getY()) )
   #  i += 1

    UpdateSession.close()

    Gds.save( cell )
    return


def scriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    buildRectilinear( editor )
    return True 

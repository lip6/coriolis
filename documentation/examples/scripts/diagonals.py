#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *


def toDbU ( l ): return DbU.fromLambda(l)


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def buildDiagonals ( editor ):
    DbU.setPolygonStep( toDbU(1.0) )
    
    UpdateSession.open()

    cell = AllianceFramework.get().createCell( 'diagonal' )
    cell.setTerminal( True )

    cell.setAbutmentBox( Box( toDbU(-5.0), toDbU(-5.0), toDbU(65.0), toDbU(75.0) ) )

    if editor:
      UpdateSession.close()
      editor.setCell( cell )
      editor.fit()
      UpdateSession.open()
    
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


    net = Net.create( cell, 'my_net' )
    net.setExternal( True )

    Diagonal.create( net, metal2
                   , Point( toDbU(20.0), toDbU(10.0) )
                   , Point( toDbU(10.0), toDbU(20.0) )
                   , toDbU(4.0)
                   )
    Vertical.create( net, metal2, toDbU(10.0), toDbU(4.0), toDbU(20.0), toDbU(30.0) )
    Diagonal.create( net, metal2
                   , Point( toDbU(10.0), toDbU(30.0) )
                   , Point( toDbU(20.0), toDbU(40.0) )
                   , toDbU(4.0)
                   )
    Horizontal.create( net, metal2, toDbU(40.0), toDbU(4.0), toDbU(20.0), toDbU(30.0) )
    Diagonal.create( net, metal2
                   , Point( toDbU(30.0), toDbU(40.0) )
                   , Point( toDbU(40.0), toDbU(30.0) )
                   , toDbU(4.0)
                   )
    Vertical.create( net, metal2, toDbU(40.0), toDbU(4.0), toDbU(30.0), toDbU(20.0) )
    Diagonal.create( net, metal2
                   , Point( toDbU(40.0), toDbU(20.0) )
                   , Point( toDbU(30.0), toDbU(10.0) )
                   , toDbU(4.0)
                   )
    Horizontal.create( net, metal2, toDbU(10.0), toDbU(4.0), toDbU(30.0), toDbU(20.0) )

    UpdateSession.close()

    Gds.save( cell )
    return


def ScriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    buildDiagonals( editor )
    return True 

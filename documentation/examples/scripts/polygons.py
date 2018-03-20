#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *


def toDbU ( l ): return DbU.fromLambda(l)


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def buildPolygons ( editor ):
    UpdateSession.open()

    cell = AllianceFramework.get().createCell( 'polygons' )
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


    net = Net.create( cell, 'my_net' )
    net.setExternal( True )
    t = Triangle.create( net, metal1, Point( toDbU(   0.0), toDbU(  0.0) )
                                    , Point( toDbU( 100.0), toDbU(  0.0) )
                                    , Point( toDbU(  50.0), toDbU( 50.0) ) )

    UpdateSession.close()

   #AllianceFramework.get().saveCell( cell, Catalog.State.Views )
   # No saving as we don't have 
    return


def ScriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    buildPolygons( editor )
    return True 

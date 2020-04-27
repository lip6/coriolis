#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *


def toDbU ( l ): return DbU.fromLambda(l)


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def drawLogo ( editor ):
    UpdateSession.open()

    cell = AllianceFramework.get().createCell( 'coriolis_logo' )
    cell.setTerminal( True )

    cell.setAbutmentBox( Box( toDbU(-16.0), toDbU(-16.0), toDbU(76.0), toDbU(76.0) ) )

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

    net = Net.create( cell, 'logo' )
    net.setExternal( True )

    Vertical  .create( net, metal1, toDbU(10.0), toDbU(12.0), toDbU( 4.0), toDbU(56.0) )
    Horizontal.create( net, metal1, toDbU(50.0), toDbU(12.0), toDbU( 4.0), toDbU(56.0) )
    Vertical  .create( net, metal1, toDbU(50.0), toDbU(12.0), toDbU(56.0), toDbU( 4.0) )
    Horizontal.create( net, metal1, toDbU(10.0), toDbU(12.0), toDbU(56.0), toDbU( 4.0) )

    Vertical  .create( net, metal1, toDbU(-6.0), toDbU(12.0), toDbU(22.0), toDbU(56.0) )
    Horizontal.create( net, metal1, toDbU(66.0), toDbU(12.0), toDbU(22.0), toDbU(56.0) )
    Vertical  .create( net, metal1, toDbU(66.0), toDbU(12.0), toDbU(36.0), toDbU( 4.0) )
    Horizontal.create( net, metal1, toDbU(-6.0), toDbU(12.0), toDbU(36.0), toDbU( 4.0) )

    Horizontal.create( net,  nwell, toDbU(30.0), toDbU(92.0), toDbU(-16.0), toDbU(76.0) )

    UpdateSession.close()

    AllianceFramework.get().saveCell( cell, Catalog.State.Views )
    return


def scriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    drawLogo( editor )
    return True 

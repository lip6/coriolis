#!/usr/bin/python

import sys
from   Hurricane import DataBase, Box, Net, Vertical, Horizontal
from   CRL       import AllianceFramework, Catalog
from   helpers   import l
from   helpers.overlay import UpdateSession


def drawLogo ( editor ):
    """Draw a small logo of Coriolis in GDS."""
    with UpdateSession():
        cell = AllianceFramework.get().createCell( 'coriolis_logo' )
        cell.setTerminalNetlist( True )
        cell.setAbutmentBox( Box( l(-16.0), l(-16.0), l(76.0), l(76.0) ) )
        
    if editor:
        editor.setCell( cell )
        editor.fit()
        
    with UpdateSession():
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
        
        Vertical  .create( net, metal1, l(10.0), l(12.0), l( 4.0), l(56.0) )
        Horizontal.create( net, metal1, l(50.0), l(12.0), l( 4.0), l(56.0) )
        Vertical  .create( net, metal1, l(50.0), l(12.0), l(56.0), l( 4.0) )
        Horizontal.create( net, metal1, l(10.0), l(12.0), l(56.0), l( 4.0) )
        
        Vertical  .create( net, metal1, l(-6.0), l(12.0), l(22.0), l(56.0) )
        Horizontal.create( net, metal1, l(66.0), l(12.0), l(22.0), l(56.0) )
        Vertical  .create( net, metal1, l(66.0), l(12.0), l(36.0), l( 4.0) )
        Horizontal.create( net, metal1, l(-6.0), l(12.0), l(36.0), l( 4.0) )
        
        Horizontal.create( net,  nwell, l(30.0), l(92.0), l(-16.0), l(76.0) )
        
    AllianceFramework.get().saveCell( cell, Catalog.State.Views )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    drawLogo( editor )
    return True 

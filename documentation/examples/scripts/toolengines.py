#!/usr/bin/python

import sys
from   Hurricane import *
from   CRL       import *
import Etesian
import Anabatic
import Katana

# Everybody needs it.
af = AllianceFramework.get()


def toDbU ( l ): return DbU.fromLambda(l)


def doBreak ( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


def buildFulladder ( editor ):

   # Get the Framework and all the master cells.
    xr2_x2 = af.getCell( 'xr2_x1', Catalog.State.Views )
    a2_x2  = af.getCell( 'a2_x2' , Catalog.State.Views )
    o2_x2  = af.getCell( 'o2_x2' , Catalog.State.Views )

    UpdateSession.open()

    fulladder = af.createCell( 'fulladder' )
    
   # Create Instances.
    a2_1  = Instance.create( fulladder,  'a2_1',  a2_x2 )
    a2_2  = Instance.create( fulladder,  'a2_2',  a2_x2 ) 
    xr2_1 = Instance.create( fulladder, 'xr2_1', xr2_x2 )
    xr2_2 = Instance.create( fulladder, 'xr2_2', xr2_x2 )
    o2_1  = Instance.create( fulladder,  'o2_1',  o2_x2 )
    
   # Create Nets.
    vss = Net.create( fulladder, "vss" )
    vss.setExternal( True )
    vss.setGlobal  ( True )

    vdd = Net.create( fulladder, "vdd" )
    vdd.setExternal( True )
    vdd.setGlobal  ( True )

    cin = Net.create( fulladder, "cin" )
    cin.setExternal( True )
    xr2_2.getPlug( xr2_x2.getNet('i0') ).setNet( cin )
    a2_2 .getPlug(  a2_x2.getNet('i0') ).setNet( cin )
    
    a = Net.create( fulladder, 'a' )
    a.setExternal( True )
    xr2_1.getPlug( xr2_x2.getNet('i0') ).setNet( a )
    a2_1 .getPlug(  a2_x2.getNet('i0') ).setNet( a )
    
    b = Net.create( fulladder, 'b' )
    b.setExternal( True )
    xr2_1.getPlug( xr2_x2.getNet('i1') ).setNet( b )
    a2_1 .getPlug(  a2_x2.getNet('i1') ).setNet( b )
    
    sout_1 = Net.create( fulladder, 'sout_1' )
    xr2_1.getPlug( xr2_x2.getNet('q' ) ).setNet( sout_1 )
    xr2_2.getPlug( xr2_x2.getNet('i1') ).setNet( sout_1 )
    a2_2 .getPlug(  a2_x2.getNet('i1') ).setNet( sout_1 )
    
    carry_1 = Net.create( fulladder, 'carry_1' )
    a2_1.getPlug( a2_x2.getNet('q' ) ).setNet( carry_1 )
    o2_1.getPlug( o2_x2.getNet('i1') ).setNet( carry_1 )

    carry_2 = Net.create( fulladder, 'carry_2' )
    a2_2.getPlug( a2_x2.getNet('q' ) ).setNet( carry_2 )
    o2_1.getPlug( o2_x2.getNet('i0') ).setNet( carry_2 )

    sout = Net.create( fulladder, 'sout' )
    sout.setExternal( True )
    xr2_2.getPlug( xr2_x2.getNet('q') ).setNet( sout )
    
    cout = Net.create( fulladder, 'cout' )
    cout.setExternal( True )
    o2_1.getPlug( o2_x2.getNet('q') ).setNet( cout )

    UpdateSession.close()

    af.saveCell( fulladder, Catalog.State.Views )
    return fulladder


def placeAndRoute ( editor, cell ):
   # Run the placer.
    etesian = Etesian.EtesianEngine.create(cell)
    etesian.place()

    if editor:
      editor.setCell( cell )
      editor.fit()

    Breakpoint.stop( 1, 'After placement' )

   # Run the router.
    katana = Katana.KatanaEngine.create(cell)
    katana.digitalInit      ()
    katana.runGlobalRouter  ()
    katana.loadGlobalRouting( Anabatic.EngineLoadGrByNet )
    katana.layerAssign      ( Anabatic.EngineNoNetLayerAssign )
    katana.runNegociate     ( Katana.Flags.NoFlags )

    af.saveCell( cell, Catalog.State.Views )
    return


def scriptMain ( **kw ):
    editor = None
    if kw.has_key('editor') and kw['editor']:
      editor = kw['editor']

    fulladder = buildFulladder( editor )
    placeAndRoute( editor, fulladder )
    return True 

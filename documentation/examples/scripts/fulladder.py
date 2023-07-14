#!/usr/bin/python

import sys
from   Hurricane import DataBase, Instance, Box, Net, Horizontal, Vertical, Contact, \
                        RoutingPad, Transformation, Occurrence, \
                        Breakpoint
from   CRL       import AllianceFramework, Catalog, Gds
from   helpers   import l
from   helpers.overlay import UpdateSession


def doBreak ( level, message ):
    Breakpoint.stop( level, message )


def buildFulladder ( editor ):
    """
    Build a full adder, place it, add filler cells and create routing
    for the "a" net.
    """
    # Get the Framework and all the master cells.
    af     = AllianceFramework.get()
    xr2_x2 = af.getCell( 'xr2_x1', Catalog.State.Views )
    a2_x2  = af.getCell( 'a2_x2' , Catalog.State.Views )
    o2_x2  = af.getCell( 'o2_x2' , Catalog.State.Views )

    with UpdateSession():
        fulladder = af.createCell( 'fulladder' )
        fulladder.setAbutmentBox( Box( l(0.0), l(0.0), l(90.0), l(100.0) ) )
    if editor:
        editor.setCell( fulladder )
        editor.fit()
    
    with UpdateSession():
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

    with UpdateSession():
        # Instances placement.
        a2_1.setTransformation( Transformation( l(0.0)
                                              , l(0.0)
                                              , Transformation.Orientation.ID ) )
        a2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Placed a2_1' )
    
    with UpdateSession():
        xr2_1.setTransformation( Transformation( l(  0.0)
                                               , l(100.0)
                                               , Transformation.Orientation.MY ) )
        xr2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Placed xr2_1' )
    
    with UpdateSession():
        a2_2.setTransformation( Transformation( l(25.0)
                                              , l( 0.0)
                                              , Transformation.Orientation.ID ) )
        a2_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Placed a2_2' )
    
    with UpdateSession():
        xr2_2.setTransformation( Transformation( l( 45.0)
                                               , l(100.0)
                                               , Transformation.Orientation.MY ) )
        xr2_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Placed xr2_2' )
    
    with UpdateSession():
        o2_1.setTransformation( Transformation( l(65.0)
                                              , l( 0.0)
                                              , Transformation.Orientation.ID ) )
        o2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Placed o2_1' )

    with UpdateSession():
        # Add filler cells.
        tie_x0    = af.getCell(    'tie_x0', Catalog.State.Views )
        rowend_x0 = af.getCell( 'rowend_x0', Catalog.State.Views )
        filler_1  = Instance.create( fulladder, 'filler_1',    tie_x0 )
        filler_2  = Instance.create( fulladder, 'filler_2', rowend_x0 )
        
        filler_1.setTransformation( Transformation( l(50.0)
                                                  , l( 0.0)
                                                  , Transformation.Orientation.ID ) )
        filler_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
        
        filler_2.setTransformation( Transformation( l(60.0)
                                                  , l( 0.0)
                                                  , Transformation.Orientation.ID ) )
        filler_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
    #doBreak( 1, 'Filler cell placeds' )

    # Getting the layers.
    technology = DataBase.getDB().getTechnology()
    metal2     = technology.getLayer( "METAL2" ) 
    metal3     = technology.getLayer( "METAL3" ) 
    via12      = technology.getLayer( "VIA12"  ) 
    via23      = technology.getLayer( "VIA23"  ) 
    
    with UpdateSession():
        # Build wiring for a.
        # Create RoutingPads first.
        rp1 = RoutingPad.create( a
                               , Occurrence( xr2_1.getPlug( xr2_x2.getNet("i0")) )
                               , RoutingPad.BiggestArea )
        rp2 = RoutingPad.create( a
                               , Occurrence(  a2_1.getPlug(  a2_x2.getNet("i0")) )
                               , RoutingPad.BiggestArea )
        
        # Then regular wiring.
        contact1 = Contact.create( rp1, via12, l( 0.0), l(-15.0) )
        contact2 = Contact.create( rp2, via12, l( 0.0), l( 10.0) )
        turn     = Contact.create( a  , via23, l(10.0), l( 35.0) )
        Horizontal.create( contact2, turn    , metal2, l(35.0), l(2.0) )
        Vertical  .create( turn    , contact1, metal3, l(10.0), l(2.0) )

    af.saveCell( fulladder, Catalog.State.Views )
    Gds.save( fulladder )


def scriptMain ( **kw ):
    """The mandatory function to be called by Coriolis CGT/Unicorn."""
    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']
    buildFulladder( editor )
    return True 

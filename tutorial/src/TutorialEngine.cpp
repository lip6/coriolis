// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TutorialEngine.cpp"                     |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "crlcore/AllianceFramework.h"
#include "tutorial/TutorialEngine.h"


namespace Tutorial {

  using namespace std;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::Component;
  using Hurricane::Contact;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Plug;
  using Hurricane::Instance;
  using Hurricane::Transformation;
  using Hurricane::Occurrence;


// -------------------------------------------------------------------
// Class  :  "Tutorial::TutorialEngine".

  Name  TutorialEngine::_toolName = "Tutorial";


  const Name& TutorialEngine::staticGetName ()
  { return _toolName; }


  TutorialEngine* TutorialEngine::get ( const Cell* cell )
  { return static_cast<TutorialEngine*>(ToolEngine::get(cell,staticGetName())); }


  TutorialEngine::TutorialEngine ( Cell* cell )
    : Super           (cell)
    , _viewer         (NULL)
  { }


  void  TutorialEngine::_postCreate ()
  {
    Super::_postCreate ();
  }


  TutorialEngine* TutorialEngine::create ( Cell* cell )
  {
    TutorialEngine* tutorial = new TutorialEngine ( cell );

    tutorial->_postCreate();

    return tutorial;
  }


  void  TutorialEngine::_preDestroy ()
  { }


  TutorialEngine::~TutorialEngine ()
  { }


  const Name& TutorialEngine::getName () const
  { return _toolName; }


  Cell* TutorialEngine::runDemoPart1 ()
  {
    cerr << "TutorialEngine::runDemoPart1() has been called." << endl;

    CRL::AllianceFramework* framework = CRL::AllianceFramework::get();

    UpdateSession::open();
    
    Cell* demo = framework->createCell( "demo_cell_2" );
    demo->setAbutmentBox( Box( DbU::fromLambda(  0.0)
                             , DbU::fromLambda(  0.0)
                             , DbU::fromLambda(135.0)
                             , DbU::fromLambda(110.0)
                             ) );

  // Instanciation & placement of "i1_xr2".
    Cell* model = framework->getCell( "xr2_x1", CRL::Catalog::State::Views );
    Instance* i1_xr2 = Instance::create( demo, "i1_xr2", model );
    i1_xr2->setTransformation( Transformation( DbU::fromLambda( 0.0 )
                                             , DbU::fromLambda( 0.0 )
                                             , Transformation::Orientation::ID
                                             ) );
    i1_xr2->setPlacementStatus( Instance::PlacementStatus::PLACED );

  // Instanciation & placement of "i2_xr2".
    model = framework->getCell( "xr2_x1", CRL::Catalog::State::Views );
    Instance* i2_xr2 = Instance::create( demo, "i2_xr2", model );
    i2_xr2->setTransformation( Transformation( DbU::fromLambda( 55.0 )
                                             , DbU::fromLambda(  0.0 )
                                             , Transformation::Orientation::ID
                                             ) );
    i2_xr2->setPlacementStatus( Instance::PlacementStatus::PLACED );

  // Instanciation & placement of "i3_a2".
    model = framework->getCell( "a2_x2", CRL::Catalog::State::Views );
    Instance* i3_a2 = Instance::create( demo, "i3_a2", model );
    i3_a2->setTransformation( Transformation( DbU::fromLambda(   0.0 )
                                            , DbU::fromLambda( 100.0 )
                                            , Transformation::Orientation::MY
                                            ) );
    i3_a2->setPlacementStatus( Instance::PlacementStatus::PLACED );

  // Instanciation & placement of "i4_a2".
    model = framework->getCell( "a2_x2", CRL::Catalog::State::Views );
    Instance* i4_a2 = Instance::create( demo, "i4_a2", model );
    i4_a2->setTransformation( Transformation( DbU::fromLambda(  55.0 )
                                            , DbU::fromLambda( 100.0 )
                                            , Transformation::Orientation::MY
                                            ) );
    i4_a2->setPlacementStatus( Instance::PlacementStatus::PLACED );

  // Instanciation & placement of "i1_xr2".
    model = framework->getCell( "o2_x2", CRL::Catalog::State::Views );
    Instance* i5_o2 = Instance::create( demo, "i5_o2", model );
    i5_o2->setTransformation( Transformation( DbU::fromLambda( 110.0 )
                                            , DbU::fromLambda(   0.0 )
                                            , Transformation::Orientation::ID
                                            ) );
    i5_o2->setPlacementStatus( Instance::PlacementStatus::PLACED );

    UpdateSession::close();

    return demo;
  }


  void  TutorialEngine::runDemoPart2 ()
  {
    cerr << "TutorialEngine::runDemoPart2() has been called." << endl;

    Cell* cell = getCell();

    UpdateSession::open();

    Net* net = Net::create( cell, "demo" );
    Instance* i1_xr2 = cell->getInstance( "i1_xr2" );
    Plug* plug = i1_xr2->getPlug( i1_xr2->getMasterCell()->getNet("q") );
    plug->setNet( net );

    RoutingPad* rp1 = RoutingPad::create( net, Occurrence(plug) );
    rp1->setOnBestComponent( RoutingPad::BiggestArea );

    Instance* i2_xr2 = cell->getInstance( "i2_xr2" );
    plug = i2_xr2->getPlug( i2_xr2->getMasterCell()->getNet("i0") );
    plug->setNet( net );

    RoutingPad* rp2 = RoutingPad::create( net, Occurrence(plug) );
    rp2->setOnBestComponent( RoutingPad::BiggestArea );

    Instance* i4_a2 = cell->getInstance( "i4_a2" );
    plug = i4_a2->getPlug( i4_a2->getMasterCell()->getNet("i0") );
    plug->setNet( net );

    RoutingPad* rp3 = RoutingPad::create( net, Occurrence(plug) );
    rp3->setOnBestComponent( RoutingPad::BiggestArea );

    UpdateSession::close();
    Breakpoint::stop( 1, "RoutingPad createds" );
    UpdateSession::open();

    Technology* technology = DataBase::getDB()->getTechnology();
    Layer* metal2 = technology->getLayer( "METAL2" );
    Layer* metal3 = technology->getLayer( "METAL3" );
    Layer* via1   = technology->getLayer( "VIA12" );
    Layer* via2   = technology->getLayer( "VIA23" );

    Contact* contact1 = Contact::create( rp1, via1, DbU::fromLambda( 0.0), DbU::fromLambda( 0.0) );
    Contact* contact2 = Contact::create( rp2, via1, DbU::fromLambda( 0.0), DbU::fromLambda(-5.0) );
    Contact* contact3 = Contact::create( rp3, via1, DbU::fromLambda( 0.0), DbU::fromLambda( 0.0) );
    Contact* contact4 = Contact::create( net, via2, DbU::fromLambda(50.0), DbU::fromLambda(20.0) );
    Contact* contact5 = Contact::create( net, via2, DbU::fromLambda(50.0), DbU::fromLambda(75.0) );

    Horizontal::create( contact1, contact4, metal2, DbU::fromLambda(20.0), DbU::fromLambda(2.0) );
    Horizontal::create( contact4, contact2, metal2, DbU::fromLambda(20.0), DbU::fromLambda(2.0) );
    Horizontal::create( contact5, contact3, metal2, DbU::fromLambda(75.0), DbU::fromLambda(2.0) );
    Vertical::create  ( contact4, contact5, metal3, DbU::fromLambda(50.0), DbU::fromLambda(2.0) );

    UpdateSession::close();

    cout << "All components of " << net << endl;
    for ( Component* component : net->getComponents() ) {
      cout << "| " << component << endl;
    }
    Breakpoint::stop( 1, "Contacts and wires createds" );
    UpdateSession::open();

    contact5->destroy();
    
    UpdateSession::close();
  }
  

  string  TutorialEngine::_getTypeName () const
  { return "Tutorial::TutorialEngine"; }


  string  TutorialEngine::_getString () const
  {
    ostringstream  os;
    os << "<" << "TutorialEngine " << _cell->getName () << ">";
    return os.str();
  }


  Record* TutorialEngine::_getRecord () const
  {
    Record* record = Super::_getRecord ();
                                     
    if (record) {
    // Add new records here.
    }
    return record;
  }


} // Tutorial namespace.

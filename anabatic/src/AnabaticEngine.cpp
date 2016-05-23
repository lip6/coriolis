// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AnabaticEngine.cpp"                          |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::ostringstream;
  using Hurricane::Error;
  using Hurricane::Cell;
  using Hurricane::UpdateSession;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AnabaticEngine".

  Name  AnabaticEngine::_toolName = "Anabatic";


  AnabaticEngine* AnabaticEngine::get ( const Cell* cell )
  { return static_cast<AnabaticEngine*>(ToolEngine::get(cell,staticGetName())); }


  const Name& AnabaticEngine::staticGetName ()
  { return _toolName; }


  const Name& AnabaticEngine::getName () const
  { return _toolName; }


  AnabaticEngine::AnabaticEngine ( Cell* cell )
    : Super(cell)
    , _configuration (new ConfigurationConcrete())
    , _matrix        ()
    , _southWestGCell(NULL)
  {
    _matrix.setCell( cell, _configuration->getSliceHeight() );
  }


  void  AnabaticEngine::_postCreate ()
  {
    Super::_postCreate();

  //cdebug.setMinLevel(110);
  //cdebug.setMaxLevel(120);

    UpdateSession::open();
    _southWestGCell = GCell::create( this );

    UpdateSession::close();
  }


  AnabaticEngine* AnabaticEngine::create ( Cell* cell )
  {
    if (not cell) throw Error( "AnabaticEngine::create(): NULL cell argument." );
    if (cell->getAbutmentBox().isEmpty())
      throw Error( "AnabaticEngine::create(): %s has no abutment box." , getString(cell).c_str() );
    
    AnabaticEngine* engine = new AnabaticEngine ( cell );
    engine->_postCreate();
    return engine;
  }


  AnabaticEngine::~AnabaticEngine ()
  {
    delete _configuration;
  }


  void  AnabaticEngine::_preDestroy ()
  {
  // To be done: destroy the whole set of GCells.
  // Must be stored in some way before destruction (in a set<> ?).

    Super::_preDestroy();
  }


  Configuration* AnabaticEngine::getConfiguration ()
  { return _configuration; }


  void  AnabaticEngine::_runTest ()
  {
    cerr << "AnabaticEngine::_runTest() called." << endl;
  }


  string  AnabaticEngine::_getTypeName () const
  { return getString(_toolName); }


  string  AnabaticEngine::_getString () const
  {
    ostringstream os;
    os << "<" << _toolName << " " << _cell->getName() << ">";
    return os.str();
  }


  Record* AnabaticEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_configuration" ,  _configuration ) );
    record->add( getSlot("_southWestGCell",  _southWestGCell) );
    record->add( getSlot("_matrix"        , &_matrix        ) );
    return record;
  }


}  // Anabatic namespace.

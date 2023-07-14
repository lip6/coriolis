// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universié 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./FoehnEngine.cpp"                             |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DataBase.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/Measures.h"
#include "crlcore/Histogram.h"
#include "foehn/DagProperty.h"
#include "foehn/FoehnEngine.h"


namespace Foehn {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using Hurricane::Plug;
  using CRL::addMeasure;
  using CRL::getMeasure;
  using CRL::Histogram;


// -------------------------------------------------------------------
// Class  :  "Foehn::FoehnEngine".

  Name  FoehnEngine::_toolName = "Foehn";


  FoehnEngine* FoehnEngine::get ( const Cell* cell )
  { return static_cast<FoehnEngine*>(ToolEngine::get(cell,staticGetName())); }


  const Name& FoehnEngine::staticGetName ()
  { return _toolName; }


  const Name& FoehnEngine::getName () const
  { return _toolName; }


  FoehnEngine::FoehnEngine ( Cell* cell )
    : Super         (cell)
    , _viewer       (nullptr)
    , _configuration()
  { }


  void  FoehnEngine::_postCreate ()
  {
    Super::_postCreate();
  }


  FoehnEngine* FoehnEngine::create ( Cell* cell )
  {
    if (not cell) throw Error( "FoehnEngine::create(): NULL cell argument." );
    
    FoehnEngine* engine = new FoehnEngine ( cell );
    engine->_postCreate();
    return engine;
  }


  FoehnEngine::~FoehnEngine ()
  { }


  void  FoehnEngine::_preDestroy ()
  {
    clear();
    Super::_preDestroy();
  }


  Dag* FoehnEngine::newDag ( string label )
  {
    Dag* dag = new Dag ( this, label );
    _dags.push_back( dag );
    return dag;
  }


  Dag* FoehnEngine::getDag ( string label ) const
  {
    for ( Dag* dag : _dags ) {
      if (dag->getLabel() == label) return dag;
    }
    return NULL;
  }


  void  FoehnEngine::clear ()
  {
    for ( Dag* dag : _dags ) delete dag;
    _dags.clear();
  }


  string  FoehnEngine::_getTypeName () const
  { return getString(_toolName); }


  string  FoehnEngine::_getString () const
  {
    ostringstream os;
    os << "<" << _toolName << " " << _cell->getName() << ">";
    return os.str();
  }


  Record* FoehnEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_toolName"     , &_toolName      ));
    record->add( getSlot( "_configuration", &_configuration ));
    record->add( getSlot( "_dags"         , &_dags          ));
    return record;
  }


}  // Foehn namespace.

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./TramontanaEngine.cpp"                       |
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
#include "hurricane/Breakpoint.h"
#include "hurricane/Timer.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/viewer/Script.h"
#include "crlcore/Measures.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::vector;
  using std::make_pair;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TramontanaEngine".

  Name  TramontanaEngine::_toolName = "Tramontana";


  const Name& TramontanaEngine::staticGetName ()
  { return _toolName; }


  TramontanaEngine* TramontanaEngine::get ( const Cell* cell )
  { return static_cast<TramontanaEngine*>(ToolEngine::get(cell,staticGetName())); }


  TramontanaEngine::TramontanaEngine ( Cell* cell )
    : Super           (cell)
    , _viewer         (NULL)
  { }


  void  TramontanaEngine::_postCreate ()
  {
    Super::_postCreate();
  }


  TramontanaEngine* TramontanaEngine::create ( Cell* cell )
  {
    TramontanaEngine* tramontana = new TramontanaEngine ( cell );

    tramontana->_postCreate();

    return tramontana;
  }


  void  TramontanaEngine::_preDestroy ()
  {
    cdebug_log(160,1) << "TramontanaEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << _cell->getName() << ">" << endl;
    Super::_preDestroy();

    cdebug_tabw(160,-1);
  }


  TramontanaEngine::~TramontanaEngine ()
  { }


  const Name& TramontanaEngine::getName () const
  { return _toolName; }


  void  TramontanaEngine::extract ()
  {
    cerr << "TramontanaEngine::extract() called on " << getCell() << endl;
  }


  string  TramontanaEngine::_getTypeName () const
  { return "Tramontana::TramontanaEngine"; }


  string  TramontanaEngine::_getString () const
  {
    ostringstream  os;
    os << "<TramontanaEngine " << _cell->getName () << ">";
    return os.str();
  }


  Record* TramontanaEngine::_getRecord () const
  {
    Record* record = Super::_getRecord ();
                                     
    if (record) {
    //record->add( getSlot( "_blocks"       , &_blocks ) );
    }
    return record;
  }


} // Tramontana namespace.

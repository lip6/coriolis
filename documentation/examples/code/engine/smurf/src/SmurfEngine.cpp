// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   S m u r f  -  A  D e m o   T o o l E n g i n e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SmurfEngine.cpp"                        |
// +-----------------------------------------------------------------+


#include <sstream>
#include <fstream>
#include <iomanip>
#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/Measures.h"
#include "smurf/SmurfEngine.h"


namespace Smurf {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::vector;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Cell;
  using CRL::ToolEngine;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;


  const char* missingSmurf =
    "%s :\n\n"
    "    Cell %s do not have any SmurfEngine (or not yet created).\n";


// -------------------------------------------------------------------
// Class  :  "Smurf::SmurfEngine".

  Name  SmurfEngine::_toolName = "Smurf";


  const Name& SmurfEngine::staticGetName ()
  { return _toolName; }


  SmurfEngine* SmurfEngine::get ( const Cell* cell )
  { return static_cast<SmurfEngine*>(ToolEngine::get(cell,staticGetName())); }


  SmurfEngine::SmurfEngine ( Cell* cell )
    : ToolEngine(cell)
  { }


  void  SmurfEngine::_postCreate ()
  { }


  SmurfEngine* SmurfEngine::create ( Cell* cell )
  {
    SmurfEngine* smurf = new SmurfEngine ( cell );

    smurf->_postCreate();
    return smurf;
  }


  void  SmurfEngine::_preDestroy ()
  {
    ltrace(90) << "SmurfEngine::_preDestroy()" << endl;
    ltracein(90);

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << getCell()->getName() << ">" << endl;

    ltraceout(90);
  }


  SmurfEngine::~SmurfEngine ()
  {
    delete _configuration;
  }


  const Name& SmurfEngine::getName () const
  { return _toolName; }


  Configuration* SmurfEngine::getConfiguration ()
  {
    return _configuration;
  }


  void  SmurfEngine::place ( unsigned int slowMotion )
  {
  }


  string  SmurfEngine::_getTypeName () const
  { return "Smurf::SmurfEngine"; }


  string  SmurfEngine::_getString () const
  {
    ostringstream  os;
    os << "<" << "SmurfEngine " << getCell()->getName () << ">";
    return os.str();
  }


  Record* SmurfEngine::_getRecord () const
  {
    Record* record = ToolEngine::_getRecord ();
                                     
    if (record) {
    //record->add( getSlot( "_routingPlanes", &_routingPlanes ) );
      record->add( getSlot( "_configuration",  _configuration ) );
    }
    return record;
  }


} // Smurf namespace.

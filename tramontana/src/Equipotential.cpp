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
// |  C++ Module   :  "./Equipotential.cpp"                          |
// +-----------------------------------------------------------------+


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
#include "crlcore/Utilities.h"
#include "tramontana/Equipotential.h"
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
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Tramontana::Equipotential".


  Equipotential::Equipotential ( Cell* owner )
    : _owner      (owner)
    , _boundingBox()
    , _components ()
    , _childs     ()
  { }


  void  Equipotential::_postCreate ()
  {
    Super::_postCreate();
    TramontanaEngine* tramontana = TramontanaEngine::get( _owner );
    tramontana->add( this );
  }


  Equipotential* Equipotential::create ( Cell* owner )
  {
    Equipotential* equi = new Equipotential ( owner );
    equi->_postCreate();
    return equi;
  }


  void  Equipotential::_preDestroy ()
  {
    Super::_preDestroy();
  }


  Equipotential::~Equipotential ()
  { }


  Cell* Equipotential::getCell () const
  { return _owner; }


  Box  Equipotential::getBoundingBox () const
  { return _boundingBox; }


  void  Equipotential::add ( Component* component )
  {
    _components.insert( component );
  }

  
  void  Equipotential::add ( Occurrence child )
  {
    if (child.getPath().isEmpty())
      add( dynamic_cast<Component*>( child.getEntity() ));
    else
      _childs.push_back( child );
  }

  
  void  Equipotential::merge ( Equipotential* other )
  {
    if (this == other) {
      cerr << Warning( "Equipotential::merge(): Attempt to merge itself (ignored).\n"
                       "        (on: %s)"
                     , getString(this).c_str()
                     ) << endl;
      return;
    }
    
    for ( Component* component : other->getComponents() ) {
      add( component );
    }
    for ( Occurrence child : other->getChilds() ) {
      add( child );
    }
    other->clear();
  }

  
  void  Equipotential::clear ()
  {
    _components.clear();
    _childs    .clear();
  }


  string  Equipotential::_getTypeName () const
  { return "Tramontana::Equipotential"; }


  string  Equipotential::_getString () const
  {
    ostringstream  os;
    os << "<Equipotential id:" << getId() << " " << _owner->getName() << ">";
    return os.str();
  }


  Record* Equipotential::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record) {
      record->add( getSlot( "_owner"      , &_owner       ) );
      record->add( getSlot( "_boundingBox", &_boundingBox ) );
      record->add( getSlot( "_components" , &_components  ) );
      record->add( getSlot( "_childs"     , &_childs      ) );
    }
    return record;
  }


} // Tramontana namespace.

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SeabreezeEngine.cpp"                         |
// +-----------------------------------------------------------------+


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
#include "seabreeze/SeabreezeEngine.h"
#include "seabreeze/Elmore.h"

namespace Seabreeze {

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


//---------------------------------------------------------
// Class : "Seabreeze::SeabreezeEngine".

  Name SeabreezeEngine::_toolName = "Seabreeze";


  const Name& SeabreezeEngine::staticGetName ()
  { return _toolName; }


  SeabreezeEngine* SeabreezeEngine::create ( Cell* cell )
  {
    SeabreezeEngine* seabreeze = new SeabreezeEngine ( cell );
    seabreeze->_postCreate();
    return seabreeze;
  }


  SeabreezeEngine* SeabreezeEngine::get ( const Cell* cell )
  { return static_cast<SeabreezeEngine*>(ToolEngine::get(cell, staticGetName())); }


  const Name& SeabreezeEngine::getName () const
  { return _toolName; };
  

  string  SeabreezeEngine::_getTypeName () const
  { return getString(_toolName); }


  string  SeabreezeEngine::_getString () const
  {
    ostringstream os;
    os << "<" << _toolName << " " << _cell->getName() << ">";
    return os.str();
  }


  Record* SeabreezeEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_configuration",  _configuration) );
    return record;
  }


  void  SeabreezeEngine::buildElmore ( Net* net )
  {
    DebugSession::open( net, 190, 200 );
    cdebug_log(199,1) << "SeabreezeEngine::buildElmore()" << endl;
    cdebug_log(199,0) << "Run on " << net << endl;

    RoutingPad* driver = nullptr;
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      Plug* p = static_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
      if (p->getMasterNet()->getDirection() & Net::Direction::DirOut) {
        driver = rp;
        break;
      }
    }

    Elmore* elmore = ElmoreExtension::create( net );
    elmore->contFromNet( net );

    cdebug_log(199,0) << "Found " << elmore->getContacts().size() << " RoutingPads:" << endl;
    for ( Contact* contact : elmore->getContacts() ) {
      cdebug_log(199,0) << "| " << contact << endl;
    }

    elmore->buildTree( driver );
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      Plug* plug = static_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
      if (plug->getMasterNet()->getDirection() & Net::Direction::DirOut) {
        continue;
      }
      cdebug_log(199,0) << "| Elmore's delay: " << elmore->delayElmore(rp) << " " << rp << endl;    

      Contact* ct = nullptr;
      for ( Component* comp : rp->getSlaveComponents() ) {
        Contact* cont = dynamic_cast<Contact*>(comp);
        if (cont) {
          ct = cont;
          break;
        }
      } 
      cerr << "| Elmore's delay: " << elmore->delayElmore(rp) << " " << ct << endl;     

    }
    cdebug_tabw(199,-1);
    DebugSession::close();
  }


  SeabreezeEngine::SeabreezeEngine ( Cell* cell )
    : Super         (cell)
    , _configuration(new Configuration())
    , _viewer       (NULL)
  {}


  SeabreezeEngine::~SeabreezeEngine ()
  {
    delete _configuration;
  }


  void SeabreezeEngine::_postCreate()
  {
    Super::_postCreate ();
  }


  void SeabreezeEngine::_preDestroy ()
  {}

}  // Seabreeze namespace.

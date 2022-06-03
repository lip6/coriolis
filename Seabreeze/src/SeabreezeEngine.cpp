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
#include "Seabreeze/SeabreezeEngine.h"

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

  Record* SeabreezeEngine::_getRecord () const
  {
    Record* record= Super::_getRecord ();

    if ( record ) {
    // Add new records here
    }
    return record;
  }

  string SeabreezeEngine::_getString () const
  {
    ostringstream os;
    os << "<" << "SeabreezeEngine " << _cell->getName() << ">";
    return os.str();
  }

  string SeabreezeEngine::_getTypeName () const
  { return "Seabreeze::SeabreezeEngine"; }

  void SeabreezeEngine::runTool ( Net* net )
  {
    cerr << "SeabreezeEngine::runTool() has been called." << endl;
    
    RoutingPad* driver=  nullptr;

    for ( RoutingPad* rp : net->getRoutingPads() ) {
      Plug* p = static_cast<Plug*>(rp->getPlugOccurrence().getEntity());
      if ( p->getMasterNet()->getDirection() & Net::Direction::DirOut ) {
        driver = rp;
        break;
      }
    }

    Elmore* elm = ElmoreProperty::create(net)->getElmore();
    elm->buildTree(driver);
  }

  SeabreezeEngine::SeabreezeEngine ( Cell* cell )
    : Super   (cell)
    , _viewer (NULL)
  {}

  SeabreezeEngine::~SeabreezeEngine ()
  {}

  void SeabreezeEngine::_postCreate()
  {
    Super::_postCreate ();
  }

  void SeabreezeEngine::_preDestroy ()
  {}
  
}

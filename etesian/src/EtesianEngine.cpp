// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./EtesianEngine.cpp"                      |
// +-----------------------------------------------------------------+


#include <sstream>
#include <fstream>
#include <iomanip>
#include <iomanip>
#include "coloquinte/circuit_graph.hxx"
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
#include "hurricane/Occurrence.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/Measures.h"
#include "crlcore/AllianceFramework.h"
#include "etesian/EtesianEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;


  string  extractInstanceName ( const RoutingPad* rp )
  {
    ostringstream name;

    Occurrence occurrence = rp->getOccurrence();

    name << getString(occurrence.getOwnerCell()->getName()) << ':';

    if (not rp->getOccurrence().getPath().getHeadPath().isEmpty())
      name << getString(rp->getOccurrence().getPath().getHeadPath().getName()) << ":";

    name << "I." << getString(rp->getOccurrence().getPath().getTailInstance()->getName());

    return name.str();
  }


  string  extractPinName ( const RoutingPad* rp )
  {
    ostringstream name;

    Occurrence occurrence = rp->getOccurrence();

    name << getString(occurrence.getOwnerCell()->getName()) << ':';

    if (not rp->getOccurrence().getPath().isEmpty())
      name << getString(rp->getOccurrence().getPath().getName()) << ":";

    name << "T." << getString(rp->_getEntityAsComponent()->getNet()->getName());

    return name.str();
  }


  string  extractTerminalName ( const RoutingPad* rp )
  {
    ostringstream name;

    Occurrence occurrence = rp->getOccurrence();

    name << getString(occurrence.getOwnerCell()->getName()) << ':';

    if (not rp->getOccurrence().getPath().isEmpty())
      name << getString(rp->getOccurrence().getPath().getName()) << ":";

    name << "T." << getString(rp->_getEntityAsComponent()->getNet()->getName());

    return name.str();
  }


  Coloquinte::cell::pin::pin_dir  extractDirection ( const RoutingPad* rp )
  {
    switch ( rp->_getEntityAsComponent()->getNet()->getDirection() ) {
      case Net::Direction::IN:       return Coloquinte::cell::pin::I;
      default:
      case Net::Direction::OUT:
      case Net::Direction::TRISTATE: return Coloquinte::cell::pin::O;
      case Net::Direction::INOUT:    return Coloquinte::cell::pin::B;
    }

    return Coloquinte::cell::pin::O;
  }


  Point  extractRpOffset ( const RoutingPad* rp )
  {
    Cell*      masterCell = rp->getOccurrence().getMasterCell();
    Component* component  = rp->_getEntityAsComponent();

    Point offset = masterCell->getAbutmentBox().getCenter();
    if (component) {
      offset.setX( component->getCenter().getX() - offset.getX() );
      offset.setY( component->getCenter().getY() - offset.getY() );
    }

    return offset;
  }


} // Anonymous namespace.


namespace Etesian {

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
  using std::pair;
  using std::make_pair;
  using std::unordered_map;
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
  using Hurricane::Instance;
  using Hurricane::RoutingPad;
  using Hurricane::Net;
  using Hurricane::Occurrence;
  using CRL::ToolEngine;
  using CRL::AllianceFramework;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;


  const char* missingEtesian =
    "%s :\n\n"
    "    Cell %s do not have any EtesianEngine (or not yet created).\n";


// -------------------------------------------------------------------
// Class  :  "Etesian::EtesianEngine".

  Name  EtesianEngine::_toolName = "Etesian";


  const Name& EtesianEngine::staticGetName ()
  { return _toolName; }


  EtesianEngine* EtesianEngine::get ( const Cell* cell )
  { return static_cast<EtesianEngine*>(ToolEngine::get(cell,staticGetName())); }


  EtesianEngine::EtesianEngine ( Cell* cell )
    : ToolEngine (cell)
    , _circuit   (NULL)
    , _cellsToIds()
  { }


  void  EtesianEngine::_postCreate ()
  { }


  EtesianEngine* EtesianEngine::create ( Cell* cell )
  {
    EtesianEngine* etesian = new EtesianEngine ( cell );

    etesian->_postCreate();
    return etesian;
  }


  void  EtesianEngine::_preDestroy ()
  {
    ltrace(90) << "EtesianEngine::_preDestroy()" << endl;
    ltracein(90);

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << getCell()->getName() << ">" << endl;

    ltraceout(90);
  }


  EtesianEngine::~EtesianEngine ()
  {
    if (_circuit) delete _circuit;
    delete _configuration;
  }


  const Name& EtesianEngine::getName () const
  { return _toolName; }


  Configuration* EtesianEngine::getConfiguration ()
  { return _configuration; }


  void  EtesianEngine::place ( unsigned int slowMotion )
  {
    AllianceFramework* af = AllianceFramework::get();

    getCell()->flattenNets( true );

  // Coloquinte circuit description data-structures.
    _circuit = new Coloquinte::circuit();
    _circuit->cells    .resize( getCell()->getLeafInstanceOccurrences().getSize() );
    _circuit->hypernets.resize( getCell()->getNets().getSize() );

    Coloquinte::cell_id  cellId = 0;
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = (*ioccurrence).getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      cerr << instanceName << " " << (int)instance->getPlacementStatus().getCode() << endl;

      Coloquinte::circuit_coordinate  cellSize ( DbU::toLambda( masterCell->getAbutmentBox().getWidth () )
                                               , DbU::toLambda( masterCell->getAbutmentBox().getHeight() ));
      _cellsToIds.insert( make_pair(instanceName,cellId) );

      _circuit->cells[cellId].name    = instanceName;
      _circuit->cells[cellId].sizes   = cellSize;
      _circuit->cells[cellId].area    = cellSize.cast<Coloquinte::cell_area>().prod();
      _circuit->cells[cellId].movable = not instance->isFixed() and instance->isTerminal();
    //_circuit->cells[cellId].movable = (instance->getPlacementStatus() == Instance::PlacementStatus::UNPLACED);

      cellId++;
    }

    unsigned int netId = 0;
    forEach ( Net*, inet, getCell()->getNets() )
    {
      const char* excludedType = NULL;
      if ((*inet)->getType() == Net::Type::POWER ) excludedType = "POWER";
      if ((*inet)->getType() == Net::Type::GROUND) excludedType = "GROUND";
      if ((*inet)->getType() == Net::Type::CLOCK ) excludedType = "CLOCK";
      if (excludedType) {
        cparanoid << Warning( "%s is not a routable net (%s,excluded)."
                            , getString(*inet).c_str(), excludedType ) << endl;
        continue;
      }
      if (af->isBLOCKAGE((*inet)->getName())) continue;

      cerr << (*inet)->getName() << endl;
      forEach ( RoutingPad*, irp, (*inet)->getRoutingPads() ) {
        cerr << "    " << (*irp)->getOccurrence().getCompactString() << endl;
        string insName = extractInstanceName( *irp );
        Point  offset  = extractRpOffset    ( *irp );

        cerr << "    Master Cell: " << (*irp)->getOccurrence().getMasterCell() << endl;
        cerr << "    Rebuilt instance name: " << insName << " " << offset << endl;

        auto  iid = _cellsToIds.find( insName );
        if (iid == _cellsToIds.end() ) {
          cerr << Error( "Unable to lookup instance <%s>.", insName.c_str() ) << endl;
        } else {
          Coloquinte::cell_id           cellId    = (*iid).second;
          Coloquinte::hypernet::pin_id  netPinId  ( cellId, _circuit->cells    [cellId].pins.size() );
          Coloquinte::cell::pin_id      cellPinId ( netId , _circuit->hypernets[netId ].pins.size() );
          _circuit->hypernets[netId].pins.push_back( netPinId );
     
          Coloquinte::cell::pin  cellPin;
        //cellPin.name     = extractTerminalName( *irp );
          cellPin.d        = extractDirection   ( *irp );
          cellPin.offs.x() = DbU::toLambda( offset.getX() );
          cellPin.offs.y() = DbU::toLambda( offset.getY() );
          cellPin.ind  = cellPinId;
          _circuit->cells[cellId].pins.push_back( cellPin );
        }
      }

      netId++;
    }

    _circuit->position_overlays.resize(1);
	_circuit->position_overlays[0].x_pos = Coloquinte::circuit_vector( _cellsToIds.size() );
	_circuit->position_overlays[0].y_pos = Coloquinte::circuit_vector( _cellsToIds.size() );

    for ( auto ipair : _cellsToIds ) {
      Coloquinte::circuit_coordinate position = Coloquinte::circuit_coordinate::Zero();
      position += _circuit->cells[ipair.second].get_sizes() / 2;
      _circuit->position_overlays[0].x_pos[ipair.second] = position.x();
      _circuit->position_overlays[0].y_pos[ipair.second] = position.y();
    }

    _circuit->bounds = Coloquinte::circuit_box
      ( Coloquinte::circuit_coordinate::Zero()
      , Coloquinte::circuit_coordinate( { DbU::toLambda(getCell()->getAbutmentBox().getWidth ())
                                        , DbU::toLambda(getCell()->getAbutmentBox().getHeight()) } ));

	float strength, strength_incr;

	cout << "Initial placement at " << time(NULL) << endl;
	for(int j = 0; j < 3; j++){
		_circuit->position_overlays[0]
          = Coloquinte::solve_quadratic_model( *_circuit
                                             ,  _circuit->position_overlays[0]
                                             ,  _circuit->position_overlays[0]
                                             );
		cout << "At " << time(NULL) << ", lower bound is "
             << B2B_wirelength(*_circuit, _circuit->position_overlays[0]) << endl;
	}
	
	strength      = 0.000001;
    strength_incr = 0.000002;

	_circuit->position_overlays.resize(2);
	for(int j = 0; j < 200; j++, strength = strength * 1.02 + strength_incr){
      _circuit->position_overlays[1]
        = Coloquinte::legalize( *_circuit
                              , 1.0
                              , _circuit->position_overlays[0]
                              , false
                              );
      cout << "At " << time(NULL) << " and iteration " << j
           << ", upper bound is " << /*B2B_wirelength(*_circuit, _circuit->position_overlays[1]) <<*/ endl;

      _circuit->position_overlays[0]
        = Coloquinte::solve_quadratic_model( *_circuit
                                           ,  _circuit->position_overlays[0]
                                           ,  _circuit->position_overlays[1]
                                           , strength
                                           );
      cout << "At " << time(NULL) << " and iteration " << j
           << ", lower bound is " << /*B2B_wirelength(*_circuit, _circuit->position_overlays[0]) <<*/ endl;
	}

    _updatePlacement();
  }


  void  EtesianEngine::_updatePlacement ()
  {
    UpdateSession::open();

    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Point     instancePosition;
      Instance* instance        = static_cast<Instance*>((*ioccurrence).getEntity());
    //Cell*     masterCell      = instance->getMasterCell();
      string    instanceName    = (*ioccurrence).getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      auto  iid = _cellsToIds.find( instanceName );
      if (iid == _cellsToIds.end() ) {
        cerr << Error( "Unable to lookup instance <%s>.", instanceName.c_str() ) << endl;
      } else {
        instancePosition.setX( DbU::fromLambda(_circuit->position_overlays[0].x_pos[(*iid).second]) );
        instancePosition.setY( DbU::fromLambda(_circuit->position_overlays[0].y_pos[(*iid).second]) );

        cerr << "Setting <" << instanceName << " @" << instancePosition << endl;

      // This is temporary as it's not trans-hierarchic: we ignore the posutions
      // of all the intermediary instances.
        instance->setTransformation( instancePosition );
      }
    }

    UpdateSession::close();
  }


  string  EtesianEngine::_getTypeName () const
  { return "Etesian::EtesianEngine"; }


  string  EtesianEngine::_getString () const
  {
    ostringstream  os;
    os << "<" << "EtesianEngine " << getCell()->getName () << ">";
    return os.str();
  }


  Record* EtesianEngine::_getRecord () const
  {
    Record* record = ToolEngine::_getRecord ();
                                     
    if (record) {
    //record->add( getSlot( "_routingPlanes", &_routingPlanes ) );
      record->add( getSlot( "_configuration",  _configuration ) );
    }
    return record;
  }


} // Etesian namespace.

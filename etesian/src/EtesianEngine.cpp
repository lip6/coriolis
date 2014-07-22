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
#if HAVE_COLOQUINTE
#include "coloquinte/circuit_graph.hxx"
#endif
#include "vlsisapd/utilities/Dots.h"
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
#include "hurricane/viewer/CellWidget.h"
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


#if HAVE_COLOQUINTE
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
#endif


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
  using Utilities::Dots;
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
  using Hurricane::CellWidget;
  using CRL::ToolEngine;
  using CRL::AllianceFramework;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;
  using CRL::CatalogExtension;


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
    : ToolEngine    (cell)
    , _configuration(NULL)
    , _flags        (0)
    , _circuit      (NULL)
    , _cellsToIds   ()
    , _cellWidget   (NULL)
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
#if HAVE_COLOQUINTE
    if (_circuit) delete _circuit;
#endif
    delete _configuration;
  }


  const Name& EtesianEngine::getName () const
  { return _toolName; }


  Configuration* EtesianEngine::getConfiguration ()
  { return _configuration; }


  void  EtesianEngine::resetPlacement ()
  {
  //cerr << "EtesianEngine::resetPlacement()" << endl;

    if (_flags & NoPlacement) return;
    _flags |= FlatDesign;

    Dots  dots ( cmess2, "     ", 80, 1000 );

    cmess1 << "  o  Erasing previous placement of <" << getCell()->getName() << ">" << endl;

    UpdateSession::open();
    vector<Occurrence>  feedOccurrences;
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      dots.dot();

      if ((_flags & FlatDesign) and not (*ioccurrence).getPath().getTailPath().isEmpty())
        _flags &= ~FlatDesign;

      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = (*ioccurrence).getCompactString();

      if (CatalogExtension::isFeed(masterCell)) {
        feedOccurrences.push_back( *ioccurrence );
      }
    }

    for ( auto ioccurrence : feedOccurrences ) {
      cerr << "  Destroy: " << ioccurrence.getCompactString() << endl;
      Instance* instance     = static_cast<Instance*>(ioccurrence.getEntity());
      instance->destroy();
    }
    UpdateSession::close();

    dots.finish( Dots::Reset );

    if (_cellWidget) _cellWidget->refresh();

    _flags |= NoPlacement;
  }


  void  EtesianEngine::place ( unsigned int slowMotion )
  {
#if HAVE_COLOQUINTE
    cmess1 << "  o  Converting <" << getCell()->getName() << "> into Coloquinte." << endl;

    resetPlacement();

    Dots               dots ( cmess2, "       ", 80, 1000 );
    AllianceFramework* af   = AllianceFramework::get();

    cmess1 << "     - Building RoutingPads (transhierarchical) ..." << endl;
    getCell()->flattenNets( Cell::BuildRings );

  // Coloquinte circuit description data-structures.
    size_t  instancesNb = getCell()->getLeafInstanceOccurrences().getSize();
    vector<Transformation>  idsToTransf ( instancesNb );

    _circuit = new Coloquinte::circuit();
    _circuit->cells    .resize( instancesNb );
    _circuit->hypernets.resize( getCell()->getNets().getSize() );

    cmess1 << "     - Converting Instances (Bookshelf nodes)" << endl;
    cout.flush();

    Coloquinte::cell_id  cellId = 0;
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = (*ioccurrence).getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      if (CatalogExtension::isFeed(masterCell)) continue;

      Coloquinte::circuit_coordinate  cellSize ( masterCell->getAbutmentBox().getWidth () / DbU::fromLambda(5.0)
                                               , masterCell->getAbutmentBox().getHeight() / DbU::fromLambda(5.0) );
      _cellsToIds.insert( make_pair(instanceName,cellId) );
      Transformation instanceTransf = instance->getTransformation();
      (*ioccurrence).getPath().getTransformation().applyOn( instanceTransf );
      idsToTransf[cellId] = instanceTransf;

      dots.dot();
    //cerr << instanceName << " " << (int)instance->getPlacementStatus().getCode()
    //     << " area:" << cellSize.cast<Coloquinte::cell_area>().prod() << endl;

      _circuit->cells[cellId].name    = instanceName;
      _circuit->cells[cellId].sizes   = cellSize;
      _circuit->cells[cellId].area    = cellSize.cast<Coloquinte::cell_area>().prod();
      _circuit->cells[cellId].movable = not instance->isFixed() and instance->isTerminal();
    //if (not _circuit->cells[cellId].movable)
    //  cerr << "FIXED (movable=false):" << instance << endl;
    //_circuit->cells[cellId].movable = (instance->getPlacementStatus() == Instance::PlacementStatus::UNPLACED);

      cellId++;
    }
    dots.finish( Dots::Reset|Dots::FirstDot );

    cmess1 << "     - Converting Nets (Bookshelf nets)" << endl;

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

      dots.dot();
    //cerr << (*inet)->getName() << endl;

      forEach ( RoutingPad*, irp, (*inet)->getRoutingPads() ) {
      //cerr << "    " << (*irp)->getOccurrence().getCompactString() << endl;
        string insName = extractInstanceName( *irp );
        Point  offset  = extractRpOffset    ( *irp );

      //cerr << "    Master Cell: " << (*irp)->getOccurrence().getMasterCell() << endl;
      //cerr << "    Rebuilt instance name: " << insName << " " << offset << endl;

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
          cellPin.offs.x() = offset.getX() / DbU::fromLambda(5.0);
          cellPin.offs.y() = offset.getY() / DbU::fromLambda(5.0);
          cellPin.ind  = cellPinId;
          _circuit->cells[cellId].pins.push_back( cellPin );
        }
      }

      netId++;
    }
    dots.finish( Dots::Reset );

    _circuit->position_overlays.resize(1);
	_circuit->position_overlays[0].x_pos = Coloquinte::circuit_vector( _cellsToIds.size() );
	_circuit->position_overlays[0].y_pos = Coloquinte::circuit_vector( _cellsToIds.size() );

    for ( auto ipair : _cellsToIds ) {
      Coloquinte::circuit_coordinate position ( idsToTransf[ipair.second].getTx() / DbU::fromLambda(5.0)
                                              , idsToTransf[ipair.second].getTy() / DbU::fromLambda(5.0) );
      // if (not _circuit->cells[ipair.second].movable) {
      //   cerr << _circuit->cells[ipair.second].name << endl;
      //   cerr << "  " << idsToTransf[ipair.second] << endl;
      //   cerr << "  Fixed cell BEFORE @" << position.x() << "x" << position.y() << endl;
      // }
    //position += _circuit->cells[ipair.second].get_sizes() / 2;
      _circuit->position_overlays[0].x_pos[ipair.second] = position.x();
      _circuit->position_overlays[0].y_pos[ipair.second] = position.y();

      // if (not _circuit->cells[ipair.second].movable) {
      //   cerr << "  Fixed cell @" << position.x() << "x" << position.y() << endl;
      // }
    }

  // Temporarily force the circuit size.
    // getCell()->setAbutmentBox( Box( DbU::fromLambda(0.0)
    //                               , DbU::fromLambda(0.0)
    //                               , DbU::fromLambda(5.0)*12000
    //                               , DbU::fromLambda(5.0)*12000
    //                               ) );
	// _circuit->bounds = Coloquinte::circuit_box( Coloquinte::circuit_coordinate::Zero()
    //                                           , Coloquinte::circuit_coordinate({12000, 12000}) );
    _circuit->bounds = Coloquinte::circuit_box
      ( Coloquinte::circuit_coordinate( { getCell()->getAbutmentBox().getXMin() / DbU::fromLambda(5.0)
                                        , getCell()->getAbutmentBox().getYMin() / DbU::fromLambda(5.0) } )
      , Coloquinte::circuit_coordinate( { getCell()->getAbutmentBox().getXMax() / DbU::fromLambda(5.0)
                                        , getCell()->getAbutmentBox().getYMax() / DbU::fromLambda(5.0) } ));

    _circuit->selfcheck();

    cmess1 << "  o  Running Coloquinte." << endl;
    cmess1 << "     - Computing initial placement..." << endl;
    cmess2 << setfill('0') << right;

    time_t  startTime = time(NULL);
    time_t  timeDelta;
    Coloquinte::circuit_scalar upperBound;
    Coloquinte::circuit_scalar lowerBound;

	for ( int j = 0; j < 3; j++ ) {
		_circuit->position_overlays[0]
          = Coloquinte::solve_quadratic_model( *_circuit
                                             ,  _circuit->position_overlays[0]
                                             ,  _circuit->position_overlays[0]
                                             );

        timeDelta  = time(NULL) - startTime;
        lowerBound = B2B_wirelength( *_circuit, _circuit->position_overlays[0] );
        cmess2 << "       Iteration "  << setw( 4) <<  (j+1)
               <<    "  Elapsed time:" << setw( 5) << timeDelta << "s"
               <<    "  Lower bound:"  << setw(10)
               << lowerBound << endl;

        _updatePlacement( 0 );
	}
	
	float strength      = 0.000001;
    float strength_incr = 0.000002;

    cmess1 << "     - Optimizing placement..." << endl;

	_circuit->position_overlays.resize(2);
	for ( int j = 0; j < 200; j++, strength = strength * 1.02 + strength_incr) {
      _circuit->position_overlays[1]
        = Coloquinte::legalize( *_circuit
                              , 1.0
                              , _circuit->position_overlays[0]
                              , 1
                              , false
                              );
      
      timeDelta  = time(NULL) - startTime;
      upperBound = B2B_wirelength( *_circuit, _circuit->position_overlays[1] );
      cmess2 << "       Iteration "  << setw( 4) << (j+1)
             <<    "  Elapsed time:" << setw( 5) << timeDelta << "s"
             <<    "  UPPER bound:"  << setw(10)
             << upperBound << endl;

      _circuit->position_overlays[0]
        = Coloquinte::solve_quadratic_model( *_circuit
                                           ,  _circuit->position_overlays[0]
                                           ,  _circuit->position_overlays[1]
                                           , strength
                                           );
      timeDelta  = time(NULL) - startTime;
      lowerBound = B2B_wirelength( *_circuit, _circuit->position_overlays[0] );
      cmess2 << "                     "
             <<    "  Elapsed time:" << setw( 5) << timeDelta << "s"
             <<    "  Lower bound:"  << setw(10)
             << lowerBound << endl;
      cmess2 << "                       Spreading ratio: "
             << ((((double)upperBound-(double)lowerBound)*100) / (double)lowerBound) << "%" << endl;

      _updatePlacement( 0 );
	}

    _updatePlacement( 1 );
    _flags &= ~NoPlacement;
#else
    cerr << Warning("Coloquinte library wasn't found, Etesian is disabled.") << endl;
#endif
  }


  void  EtesianEngine::_updatePlacement ( unsigned int placementId )
  {
#if HAVE_COLOQUINTE
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
        instancePosition.setX( _circuit->position_overlays[placementId].x_pos[(*iid).second] * DbU::fromLambda(5.0) );
        instancePosition.setY( _circuit->position_overlays[placementId].y_pos[(*iid).second] * DbU::fromLambda(5.0) );

      //cerr << "Setting <" << instanceName << " @" << instancePosition << endl;

      // This is temporary as it's not trans-hierarchic: we ignore the posutions
      // of all the intermediary instances.
        instance->setTransformation( instancePosition );
        instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
      }
    }

    UpdateSession::close();

    if (_cellWidget) _cellWidget->refresh();
#endif
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

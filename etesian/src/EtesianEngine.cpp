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
#include "coloquinte/circuit.hxx"
#include "coloquinte/legalizer.hxx"
#endif
#include "vlsisapd/configuration/Configuration.h"
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
#include "etesian/FeedCells.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using coloquinte::int_t;
  using coloquinte::float_t;
  using coloquinte::point;



#if HAVE_COLOQUINTE
  //inline bool  isNan( const float_t& f ) { return (f != f); }


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


#if 0
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
    // TODO: verify that it doesn't assume that the orientation is North
    Box        masterBox  = masterCell->getAbutmentBox();

    Point offset;
    if (component) {
      offset.setX( component->getCenter().getX() - masterBox.getXMin() );
      offset.setY( component->getCenter().getY() - masterBox.getYMin() );
    }
    else { // Why?
      offset = masterBox.getCenter();
    }

    return offset;
  }


  Transformation  toTransformation ( point<int_t>  position
                                   , point<bool>  orientation
                                   , Cell*           model
                                   , DbU::Unit       pitch
                                   )
  {
    DbU::Unit tx         = position.x_ * pitch;
    DbU::Unit ty         = position.y_ * pitch;
    //Point     center     = model->getAbutmentBox().getCenter();
    Box       cellBox    = model->getAbutmentBox();
    Transformation::Orientation orient = Transformation::Orientation::ID;

    // TODO offsets
    if ( orientation.x_ and orientation.y_ ) {
      //tx += - center.getX();
      //ty += - center.getY();
    } else if ( not orientation.x_ and orientation.y_) {
      //tx    +=   center.getX();
      tx    +=   cellBox.getWidth();
      //ty    += - center.getY();
      orient = Transformation::Orientation::MX;
    } else if ( orientation.x_ and not orientation.y_) {
      //tx    += - center.getX();
      //ty    +=   center.getY();
      ty    +=   cellBox.getHeight();
      orient = Transformation::Orientation::MY;
    } else if ( not orientation.x_ and not orientation.y_) {
      //tx    += center.getX();
      //ty    += center.getY();
      tx    +=   cellBox.getWidth();
      ty    +=   cellBox.getHeight();
      orient = Transformation::Orientation::R2;
    }

    return Transformation( tx, ty, orient );
  }
#endif


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
  using CRL::Catalog;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;
  using CRL::CatalogExtension;
  using coloquinte::index_t;
  using coloquinte::capacity_t;
  using coloquinte::int_t;
  using coloquinte::float_t;
  using coloquinte::point;
  using coloquinte::box;
  using coloquinte::Movability;
  using coloquinte::temporary_cell;
  using coloquinte::temporary_net;
  using coloquinte::temporary_pin;
  using coloquinte::netlist;
  using coloquinte::placement_t;

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
    , _configuration(new ConfigurationConcrete())
    , _flags        (0)
    , _timer        ()
    , _surface      ()
    , _circuit      ()
    , _placementLB  ()
    , _placementUB  ()
    , _cellsToIds   ()
    , _idsToInsts   ()
    , _cellWidget   (NULL)
    , _feedCells    (this)
  {
  }


  void  EtesianEngine::_postCreate ()
  {
  // Ugly: Name based detection of ISPD benchmarks.
    if (getString(getCell()->getName()).substr(0,7) == "bigblue") {
      cmess1 << "  o  ISPD benchmark <" << getCell()->getName()
             << ">, no feed cells will be added." << endl;
    } else {
    // Ugly: Direct uses of Alliance Framework.
    // Must change toward something in the settings.
      _feedCells.useFeed( AllianceFramework::get()->getCell("tie_x0"   ,Catalog::State::Views) );
      _feedCells.useFeed( AllianceFramework::get()->getCell("rowend_x0",Catalog::State::Views) );
    }
  }


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
    delete _configuration;
  }


  const Name& EtesianEngine::getName () const
  { return _toolName; }


  const Configuration* EtesianEngine::getConfiguration () const
  { return _configuration; }


  Configuration* EtesianEngine::getConfiguration ()
  { return _configuration; }


  void  EtesianEngine::startMeasures ()
  {
    _timer.resetIncrease();
    _timer.start();
  }


  void  EtesianEngine::stopMeasures ()
  { _timer.stop(); }


  void  EtesianEngine::printMeasures ( string tag ) const
  {
    ostringstream result;

    result <<  Timer::getStringTime(_timer.getCombTime()) << ", " 
           << Timer::getStringMemory(_timer.getIncrease());
    cmess1 << ::Dots::asString( "     - Done in", result.str() ) << endl;

    result.str("");
    result << _timer.getCombTime()
           << "s, +" << (_timer.getIncrease()>>10) <<  "Kb/"
           << (_timer.getMemorySize()>>10) << "Kb";
    cmess2 << ::Dots::asString( "     - Raw measurements", result.str() ) << endl;
  }


  void  EtesianEngine::setDefaultAb ()
  {
    double spaceMargin = Cfg::getParamPercentage("nimbus.spaceMargin", 10.0)->asDouble();
    double aspectRatio = Cfg::getParamPercentage("nimbus.aspectRatio",100.0)->asDouble();
    size_t instanceNb  = 0;
    double cellLength  = 0;

    vector<Occurrence>  feedOccurrences;
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = (*ioccurrence).getCompactString();

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning( "Found a feedcell %s in an unplaced design, removing."
                       , instanceName.c_str()
                       ) << endl;
        feedOccurrences.push_back( *ioccurrence );
        continue;
      }

      cellLength += DbU::toLambda( masterCell->getAbutmentBox().getWidth() );
      instanceNb += 1;
    }

    double gcellLength = cellLength*(1.0+spaceMargin) / DbU::toLambda( getSliceHeight() );
    double rows        = sqrt( gcellLength/aspectRatio );
    if (floor(rows) != rows) rows = floor(rows)+1.0;
    else                     rows = floor(rows);

    double columns     = gcellLength / rows;
    if (floor(columns) != columns) columns = floor(columns)+1.0;
    else                           columns = floor(columns);

    cmess1 << "  o  Creating abutment box (margin:" << (spaceMargin*100.0)
           << "% aspect ratio:" << (aspectRatio*100.0)
           << "% g-length:" << (cellLength/DbU::toLambda(getSliceHeight()))
           << ")" << endl;
    cmess1 << "     - GCell grid: [" << (int)columns << "x" << (int)rows << "]" << endl;

    UpdateSession::open();
    for ( auto ioccurrence : feedOccurrences ) {
      static_cast<Instance*>(ioccurrence.getEntity())->destroy();
    }

    getCell()->setAbutmentBox( Box( DbU::fromLambda(0)
                                  , DbU::fromLambda(0)
                                  , columns*getSliceHeight()
                                  , rows   *getSliceHeight()
                                  ) );
    UpdateSession::close();
    if (_cellWidget) _cellWidget->fitToContents();
  }


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


  void  EtesianEngine::toColoquinte ()
  {
#if HAVE_COLOQUINTE
    cmess1 << "  o  Converting <" << getCell()->getName() << "> into Coloquinte." << endl;

    resetPlacement();

    Dots               dots  ( cmess2, "       ", 80, 1000 );
    AllianceFramework* af    = AllianceFramework::get();
    DbU::Unit          pitch = getPitch();

    cmess1 << "     - Building RoutingPads (transhierarchical) ..." << endl;
    getCell()->flattenNets( Cell::BuildRings|Cell::NoClockFlatten );

  // Coloquinte circuit description data-structures.
    size_t                  instancesNb = getCell()->getLeafInstanceOccurrences().getSize();
    vector<Transformation>  idsToTransf ( instancesNb );
    vector<temporary_cell>  instances   ( instancesNb );
    vector< point<int_t> >  positions   ( instancesNb );
    vector< point<bool> >   orientations( instancesNb, point<bool>(true, true) );

    cmess1 << "     - Converting " << instancesNb << " instances" << endl;
    cout.flush();

    Box topAb = getCell()->getAbutmentBox();

    UpdateSession::open();
    forEach ( Occurrence, ioccurrence, getCell()->getNonLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();

    // Have to check here if the model is fully placed or not.
      masterCell->setAbutmentBox( topAb );
      instance->setTransformation( Transformation() ); // (0,0,ID).
      instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
    }
    UpdateSession::close();

    index_t instanceId = 0;
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>((*ioccurrence).getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = (*ioccurrence).getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning("Feed instance found and skipped.") << endl;
        continue;
      }

      Box instanceAb = masterCell->getAbutmentBox();

      Transformation instanceTransf = instance->getTransformation();
      (*ioccurrence).getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      // Upper rounded
      int_t xsize = (instanceAb.getWidth () + pitch -1) / pitch;
      int_t ysize = (instanceAb.getHeight() + pitch -1) / pitch;
      // Lower rounded
      int_t xpos  = instanceAb.getXMin() / pitch;
      int_t ypos  = instanceAb.getYMin() / pitch;

      instances[instanceId].size       = point<int_t>( xsize, ysize );
      instances[instanceId].list_index = instanceId;
      instances[instanceId].area       = static_cast<capacity_t>(xsize) * static_cast<capacity_t>(ysize);
      positions[instanceId] = point<int_t>( xpos, ypos );

      if ( not instance->isFixed() and instance->isTerminal() ) {
        instances[instanceId].attributes = coloquinte::XMovable
                                          |coloquinte::YMovable
                                          |coloquinte::XFlippable
                                          |coloquinte::YFlippable;
      } else {
        instances[instanceId].attributes = 0;
      }

      _cellsToIds.insert( make_pair(instanceName,instanceId) );
      _idsToInsts.push_back( instance );
      ++instanceId;
      dots.dot();
    }
    dots.finish( Dots::Reset|Dots::FirstDot );

    size_t netsNb = getCell()->getNets().getSize();
    cmess1 << "     - Converting " << netsNb << " nets" << endl;

    vector<temporary_net>  nets ( netsNb );
    vector<temporary_pin>  pins;

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

      nets[netId] = temporary_net( netId, 1000 );

      forEach ( RoutingPad*, irp, (*inet)->getRoutingPads() ) {
        string insName = extractInstanceName( *irp );
        Point  offset  = extractRpOffset    ( *irp );

        int_t xpin    = offset.getX() / pitch;
        int_t ypin    = offset.getY() / pitch;

        auto  iid = _cellsToIds.find( insName );
        if (iid == _cellsToIds.end() ) {
          cerr << Error( "Unable to lookup instance <%s>.", insName.c_str() ) << endl;
        } else {
          pins.push_back( temporary_pin( point<int_t>(xpin,ypin), (*iid).second, netId ) );
        }
      }

      netId++;
    }
    dots.finish( Dots::Reset );

    _surface = box<int_t>( (int_t)(getCell()->getAbutmentBox().getXMin() / pitch)
                         , (int_t)(getCell()->getAbutmentBox().getXMax() / pitch)
                         , (int_t)(getCell()->getAbutmentBox().getYMin() / pitch)
                         , (int_t)(getCell()->getAbutmentBox().getYMax() / pitch)
                         );
    _circuit = netlist( instances, nets, pins );
    _circuit.selfcheck();
    _placementLB.positions_    = positions;
    _placementLB.orientations_ = orientations;
    _placementUB = _placementLB;
  //cerr << "Coloquinte cell height: " << _circuit.get_cell(0).size.y_ << endl;

#endif  // HAVE_COLOQUINTE
  }

  void  EtesianEngine::place ( unsigned int flags )
  {
#if HAVE_COLOQUINTE
    using namespace coloquinte::gp;
    using namespace coloquinte::dp;

    if (flags & SlowMotion) getConfiguration()->  setFlags( SlowMotion );
    else                    getConfiguration()->unsetFlags( SlowMotion );

    if (getCell()->getAbutmentBox().isEmpty()) setDefaultAb();

    findYSpin();
    toColoquinte();

    cmess1 << "  o  Running Coloquinte." << endl;
    cmess1 << "     - Computing initial placement..." << endl;
    cmess2 << setfill('0') << right;


    double         sliceHeight = getSliceHeight() / getPitch();
    time_t         startTime   = time(NULL);
    time_t         timeDelta;
    ostringstream  label;

    cmess2 << "  o  Initial wirelength " << get_HPWL_wirelength(_circuit, _placementLB) << "." <<  endl;
    startMeasures();

    auto first_legalizer = get_rough_legalizer( _circuit, _placementLB, _surface);
    first_legalizer.selfcheck();

    cmess1 << "  o  Simple legalization." << endl;
    get_rough_legalization( _circuit, _placementUB, first_legalizer);

    timeDelta  = time(NULL) - startTime;
    cmess2 << "     - Elapsed time:" << timeDelta
           << " HPWL:" << get_HPWL_wirelength( _circuit, _placementUB )
           << "\n     "
           << "- Linear Disrupt.:" << get_mean_linear_disruption   ( _circuit, _placementLB, _placementLB )
           <<   " Quad. Disrupt.:" << get_mean_quadratic_disruption( _circuit, _placementLB, _placementLB )
           << endl;
    _placementLB = _placementUB;
    _placementLB.selfcheck();

    _updatePlacement( _placementUB );

    // cerr << _idsToInsts[1266]
    //      << " x:" << _placementLB.positions_[1095].x_
    //      << " y:" << _placementLB.positions_[1095].y_
    //      << endl;

    // Breakpoint::get()->stop( 0, "After " );

  // Early topology-independent solution + negligible pulling forces to avoid dumb solutions
    cmess1 << "  o  Star (*) Optimization." << endl;
    auto solv = get_star_linear_system( _circuit, _placementLB, 1.0, 0, 10000)
              + get_pulling_forces( _circuit, _placementUB, 1000000.0);
    solve_linear_system( _circuit, _placementLB, solv, 200 );
    _progressReport2( startTime, "     [--]" );
    
    float_t pulling_force = 0.01;

    cmess2 << "  o  Simple legalization." << endl;
    for ( int i=0; i<50; ++i, pulling_force += 0.03 ) {
    // Create a legalizer and bipartition it until we have sufficient precision
    // (~2 to 10 standard cell widths).
      auto legalizer = get_rough_legalizer( _circuit, _placementLB, _surface );
        for ( int quad_part=0 ; _circuit.cell_cnt() > 10 * (1 << (quad_part*2)) ; ++quad_part ) { // Until there is about 10 standard cells per region
            legalizer.x_bipartition();
            legalizer.y_bipartition();
            legalizer.redo_line_partitions();
            legalizer.redo_diagonal_bipartitions();
            legalizer.redo_line_partitions();
            legalizer.redo_diagonal_bipartitions();
            legalizer.selfcheck();
        }
      // Keep the orientation between LB and UB
        _placementUB = _placementLB;

        get_rough_legalization( _circuit, _placementUB, legalizer );
        label.str("");
        label  << "     [" << setw(2) << setfill('0') << i << "] Bipart.";
        _progressReport1( startTime, label.str() );
        _updatePlacement( _placementUB );

        if (i >= 30) {
          auto legalizer = legalize( _circuit, _placementUB, _surface, sliceHeight );
          coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
          _progressReport1( startTime, "          Legal. " );
          _updatePlacement( _placementUB );
        }

      // Get the system to optimize (tolerance, maximum and minimum pin counts)
      // and the pulling forces (threshold distance)
        auto solv = get_HPWLF_linear_system  ( _circuit, _placementLB, 0.01, 2, 100000 ) 
                  + get_linear_pulling_forces( _circuit, _placementUB, _placementLB, pulling_force, 40.0 );
        solve_linear_system( _circuit, _placementLB, solv, 400 ); // number of iterations
        _progressReport2( startTime, "          Linear." );
        _updatePlacement( _placementLB );

     // Optimize orientation sometimes
        if (i%5 == 0) {
            optimize_exact_orientations( _circuit, _placementLB );
            _progressReport2( startTime, "          Orient." );
            _updatePlacement( _placementLB );
        }
    }

    cmess1 << "  o  Detailed Placement." << endl;
    index_t legalizeIterations = 3;
    for ( index_t i=0; i<legalizeIterations; ++i ){
      ostringstream label;
      label.str("");
      label  << "     [" << setw(2) << setfill('0') << i << "]";

      optimize_exact_orientations( _circuit, _placementUB );
      _progressReport1( startTime, label.str()+" Oriented ......." );
      _updatePlacement( _placementUB );

      auto legalizer = legalize( _circuit, _placementUB, _surface, sliceHeight );
      coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
      _progressReport1( startTime, "          Legalized ......" );
      _updatePlacement( _placementUB );

      swaps_global_HPWL( _circuit, legalizer, 3, 4 );
      coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
      _progressReport1( startTime, "          Global Swaps ..." );
      _updatePlacement( _placementUB );

      OSRP_convex_HPWL( _circuit, legalizer );
      coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
      _progressReport1( startTime, "          Row Optimization" );
      _updatePlacement( _placementUB );

      swaps_row_HPWL( _circuit, legalizer, 4 );
      coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
      _progressReport1( startTime, "          Local Swaps ...." );

      if (i == legalizeIterations-1) {
        row_compatible_orientation( _circuit, legalizer, (_yspinSlice0 == 0) );
        coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
        _progressReport1( startTime, "          Final Legalize ." );
      }

      _updatePlacement( _placementUB, (i==legalizeIterations-1) ? ForceUpdate : 0 );
    }

    cmess1 << "  o  Adding feed cells." << endl;
    addFeeds();

    cmess1 << "  o  Placement finished." << endl;
    stopMeasures();
    printMeasures( "total" );
    cmess1 << ::Dots::asString
      ("     - HPWL", DbU::getValueString(get_HPWL_wirelength(_circuit,_placementUB )*getPitch()) ) << endl;
    cmess1 << ::Dots::asString
      ("     - RMST", DbU::getValueString(get_RSMT_wirelength(_circuit,_placementUB )*getPitch()) ) << endl;

    _flags &= ~NoPlacement;
#else
    cerr << Warning("Coloquinte library wasn't found, Etesian is disabled.") << endl;
#endif
  }


  void  EtesianEngine::_progressReport1 ( time_t startTime, string label ) const
  {
#if HAVE_COLOQUINTE
    size_t w      = label.size();
    string indent ( w, ' ' );
    if (not w) {
      label  = string( 5, ' ' );
      indent = label;
    }

    ostringstream elapsed;
    elapsed << "  dTime:" << setw(5) << (time(NULL) - startTime) << "s ";

    cmess2 << label << elapsed.str()
           << " HPWL:" << coloquinte::gp::get_HPWL_wirelength      ( _circuit, _placementUB )
           << " RMST:" << coloquinte::gp::get_RSMT_wirelength      ( _circuit, _placementUB )
           << "\n" << indent
           <<  "  Linear Disrupt.:" << coloquinte::gp::get_mean_linear_disruption   ( _circuit, _placementLB, _placementUB )
           <<   " Quad Disrupt.:"   << coloquinte::gp::get_mean_quadratic_disruption( _circuit, _placementLB, _placementUB )
           << endl;
#endif
  }


  void  EtesianEngine::_progressReport2 ( time_t startTime, string label ) const
  {
#if HAVE_COLOQUINTE
    size_t w      = label.size();
    string indent ( w, ' ' );
    if (not w) {
      label  = string( 5, ' ' );
      indent = label;
    }

    ostringstream elapsed;
    elapsed << "  dTime:" << setw(5) << (time(NULL) - startTime) << "s ";

    cmess2 << label << elapsed.str()
           << " HPWL:" << coloquinte::gp::get_HPWL_wirelength( _circuit, _placementLB )
           << " RMST:" << coloquinte::gp::get_RSMT_wirelength( _circuit, _placementLB )
           << endl;
#endif
  }


  void  EtesianEngine::_updatePlacement ( const coloquinte::placement_t& placement, unsigned int flags )
  {
#if HAVE_COLOQUINTE
    if ((not isSlowMotion()) and not (flags & ForceUpdate)) return;

    UpdateSession::open();

    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
      Point     instancePosition;
      Instance* instance        = static_cast<Instance*>((*ioccurrence).getEntity());
      string    instanceName    = (*ioccurrence).getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      auto  iid = _cellsToIds.find( instanceName );
      if (iid == _cellsToIds.end() ) {
        cerr << Error( "Unable to lookup instance <%s>.", instanceName.c_str() ) << endl;
      } else {
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
          continue;

        point<int_t>   position = placement.positions_[(*iid).second];
        Transformation trans    = toTransformation( position
                                                  , placement.orientations_[(*iid).second]
                                                  , instance->getMasterCell()
                                                  , getPitch()
                                                  );
      //cerr << "Setting <" << instanceName << " @" << instancePosition << endl;

      // This is temporary as it's not trans-hierarchic: we ignore the posutions
      // of all the intermediary instances.
        instance->setTransformation( trans );
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
      record->add( getSlot( "_configuration",  _configuration ) );
    }
    return record;
  }


} // Etesian namespace.

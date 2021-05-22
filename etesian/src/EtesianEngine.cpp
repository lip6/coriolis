// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
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
#include "coloquinte/circuit.hxx"
#include "coloquinte/circuit_helper.hxx"
#include "coloquinte/legalizer.hxx"
#include "vlsisapd/configuration/Configuration.h"
#include "vlsisapd/utilities/Dots.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DataBase.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Pin.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/Utilities.h"
#include "crlcore/Measures.h"
#include "crlcore/AllianceFramework.h"
#include "etesian/EtesianEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using coloquinte::int_t;
  using coloquinte::float_t;
  using coloquinte::point;
  using Etesian::EtesianEngine;

// Options for both placers
  unsigned const SteinerModel        = 0x0001;

// Options for the global placer
  unsigned const ForceUniformDensity = 0x0010;
  unsigned const UpdateLB            = 0x0020;
  unsigned const UpdateUB            = 0x0040;

// Options for the detailed placer
  unsigned const UpdateDetailed      = 0x0100;
//unsigned const NonConvexOpt        = 0x0200;


  Instance* extractInstance ( const RoutingPad* rp )
  {
    return rp->getOccurrence().getPath().getTailInstance();
  }


  string  extractInstanceName ( const RoutingPad* rp )
  {
    ostringstream name;

    Occurrence occurrence = rp->getOccurrence();

    name << getString(occurrence.getOwnerCell()->getName()) << ':';

    if (not rp->getOccurrence().getPath().getHeadPath().isEmpty())
      name << getString(rp->getOccurrence().getPath().getHeadPath().getName()) << ":";

    if (rp->getOccurrence().getPath().getTailInstance())
      name << "I." << getString(rp->getOccurrence().getPath().getTailInstance()->getName());
    else
      name << "C";

    return name.str();
  }


#if THIS_IS_DISABLED
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
                                   , point<bool>   orientation
                                   , Cell*         model
                                   , DbU::Unit     hpitch
                                   , DbU::Unit     vpitch
                                   )
  {
    DbU::Unit tx = position.x * vpitch;
    DbU::Unit ty = position.y * hpitch;

    Box       cellBox    = model->getAbutmentBox();
    Transformation::Orientation orient = Transformation::Orientation::ID;

    if ( not orientation.x and orientation.y) {
      tx    +=   cellBox.getWidth();
      orient = Transformation::Orientation::MX;
    } else if ( orientation.x and not orientation.y) {
      ty    +=   cellBox.getHeight();
      orient = Transformation::Orientation::MY;
    } else if ( not orientation.x and not orientation.y) {
      tx    +=   cellBox.getWidth();
      ty    +=   cellBox.getHeight();
      orient = Transformation::Orientation::R2;
    }

    return Transformation( tx, ty, orient );
  }


  uint32_t  getOutputSide ( Cell* cell )
  {
    static map<Cell*,uint32_t> cache;

    auto icache = cache.find( cell );
    if (icache != cache.end()) return (*icache).second;
    
    Net* output = NULL;
    for ( Net* net : cell->getNets() ) {
      if (net->isSupply()) continue;
      if (net->getDirection() & Net::Direction::DirOut) {
        output = net;
        break;
      }
    }
    if (not output) return EtesianEngine::RightSide;

    Box       ab    = cell->getAbutmentBox();
    DbU::Unit left  = ab.getWidth();
    DbU::Unit right = ab.getWidth();
    for ( Component* comp : NetExternalComponents::get(output) ) {
      Box       bb       = comp->getBoundingBox();
      DbU::Unit distance = bb.getXMin() - ab.getXMin();
      left = std::min( left, distance );

      distance = ab.getXMax() - bb.getXMax();
      right = std::min( right, distance );
    }

    uint32_t flags = (left < right) ? EtesianEngine::LeftSide : EtesianEngine::RightSide;
    cache[ cell ] = flags;
    return flags;
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
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Box;
  using Hurricane::DataBase;
  using Hurricane::Layer;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Pin;
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
    : Super         (cell)
    , _configuration(new Configuration())
    , _block        (NULL)
    , _ySpinSet     (false)
    , _flatDesign   (false)
    , _placeArea    (cell->getAbutmentBox())
    , _surface      (NULL)
    , _circuit      (NULL)
    , _placementLB  (NULL)
    , _placementUB  (NULL)
    , _densityLimits(NULL)
    , _netsToIds    ()
    , _instsToIds   ()
    , _idsToInsts   ()
    , _idsToNets    ()
    , _viewer       (NULL)
    , _diodeCell    (NULL)
    , _feedCells    (this)
    , _bufferCells  (this)
    , _bloatCells   (this)
    , _area         (NULL)
    , _yspinSlice0  (0)
    , _sliceHeight  (0)
    , _fixedAbHeight(0)
    , _fixedAbWidth (0)
    , _diodeCount   (0)
    , _bufferCount  (0)
  { }


  void  EtesianEngine::_postCreate ()
  {
    Super::_postCreate();

  // Ugly: Name based detection of ISPD benchmarks.
  // Maybe base it on whether the imported library is in Bookshelf format or not?
    if (getString(getCell()->getName()).substr(0,7) == "bigblue") {
      cmess2 << "  o  ISPD benchmark <" << getCell()->getName()
             << ">, no feed cells will be added." << endl;
    } else {
      _diodeCell = DataBase::getDB()->getCell( getConfiguration()->getDiodeName() );;
      if (not _diodeCell) {
        cerr << Warning( "EtesianEngine::_postCreate() Unable to find \"%s\" diode cell."
                       , getConfiguration()->getDiodeName().c_str()
                       ) << endl;
      }

      _bloatCells.select( getConfiguration()->getBloat() );

      string bufferName = getConfiguration()->getSpareBufferName();;
      Cell*  buffer = DataBase::getDB()->getCell( bufferName );
      if (buffer) {
        _bufferCells.useBuffer( buffer, 8, 20 );
      } else {
        cerr << Warning( "EtesianEngine::_postCreate() Unable to find \"%s\" spare buffer cell."
                       , bufferName.c_str()
                       ) << endl;
      }
      
      string feedNames = getConfiguration()->getFeedNames();
      char   separator = ',';

      while ( not feedNames.empty() ) {
        size_t cut = feedNames.find( separator );
        string feedName;
        if (cut != string::npos) {
          feedName  = feedNames.substr( 0, cut );
          feedNames = feedNames.substr( cut+1 );
        } else {
          feedName  = feedNames;
          feedNames.clear();
        }

        Cell*  feed = DataBase::getDB()->getCell( feedName );
        if (not feed)
          feed = AllianceFramework::get()->getCell( feedName, Catalog::State::Views|Catalog::State::Foreign );

        if (feed)
          _feedCells.useFeed( feed );
        else
          cerr << Warning( "EtesianEngine::_postCreate() Unable to find \"%s\" feed cell."
                         , feedName.c_str()
                         ) << endl;
      }
    }
    _sliceHeight = getCellGauge()->getSliceHeight();

    loadLeafCellLayouts();
  }


  EtesianEngine* EtesianEngine::create ( Cell* cell )
  {
    if (not cell) throw Error( "EtesianEngine::create(): NULL cell argument." );

    EtesianEngine* etesian = new EtesianEngine ( cell );

    etesian->_postCreate();
    return etesian;
  }


  void  EtesianEngine::_preDestroy ()
  {
    cdebug_log(120,1) << "EtesianEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << getCell()->getName() << ">" << endl;

    Super::_preDestroy();

    cdebug.log(120,-1);
  }


  EtesianEngine::~EtesianEngine ()
  {
    clearColoquinte();
    delete _area;
    delete _configuration;
  }


  void  EtesianEngine::clearColoquinte ()
  {
    cmess1 << "  o  Clearing Coloquinte data-structures on <" << getCell()->getName() << ">" << endl;

    delete _surface;
    delete _circuit;
    delete _placementLB;
    delete _placementUB;
    delete _densityLimits;

    NetsToIds emptyNetsToIds;
    _netsToIds.swap( emptyNetsToIds );

    InstancesToIds emptyInstsToIds;
    _instsToIds.swap( emptyInstsToIds );

    vector<InstanceInfos> emptyIdsToInsts;
    _idsToInsts.swap( emptyIdsToInsts );

    vector<NetInfos> emptyIdsToNets;
    _idsToNets.swap( emptyIdsToNets );

    _surface       = NULL;
    _circuit       = NULL;
    _placementLB   = NULL;
    _placementUB   = NULL;
    _densityLimits = NULL;
    _diodeCount    = 0;
  }


  const Name& EtesianEngine::getName () const
  { return _toolName; }


  const Configuration* EtesianEngine::getConfiguration () const
  { return _configuration; }


  Configuration* EtesianEngine::getConfiguration ()
  { return _configuration; }


  void  EtesianEngine::setPlaceArea ( const Box& placeArea )
  {
    Box topAb = getBlockCell()->getAbutmentBox();
    if (not topAb.contains(placeArea)) {
      cerr << Warning( "EtesianEngine::setPlaceArea(): placedArea is not *fully* inside topAb, trucating.\n"
                       "          * placedArea=%s\n"
                       "          * topAb=%s\n"
                     , getString(placeArea).c_str()
                     , getString(topAb).c_str()
                     ) << endl;
      _placeArea = topAb.getIntersection( placeArea );
    }

    DbU::Unit sliceHeight = getSliceHeight();
    _placeArea = Box( DbU::toCeil ( placeArea.getXMin(), sliceHeight )
                    , DbU::toCeil ( placeArea.getYMin(), sliceHeight )
                    , DbU::toFloor( placeArea.getXMax(), sliceHeight )
                    , DbU::toFloor( placeArea.getYMax(), sliceHeight )
                    );
    size_t bottomSlice = (_placeArea.getYMin() - topAb.getYMin()) / sliceHeight;
    if (bottomSlice % 2)
      throw Error( "EtesianEngine::setPlaceArea(): placedArea bottom must be on an even slice relative to topAb.\n"
                   "          * placedArea=%s\n"
                   "          * topAb=%s\n"
                   "          * bottomSlice=%u\n"
                 , getString(placeArea).c_str()
                 , getString(topAb).c_str()
                 , bottomSlice
                 );
  }


  void  EtesianEngine::setDefaultAb ()
  {
    _bloatCells.resetDxSpace();
    double    spaceMargin = getSpaceMargin();
    double    aspectRatio = getAspectRatio();
    size_t    instanceNb  = 0;
    DbU::Unit cellLength  = 0;

    cmess2 << "  o  Looking through the hierarchy." << endl;

    vector<Occurrence>  feedOccurrences;
    for( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = occurrence.getCompactString();

      if (CatalogExtension::isFeed(masterCell)) {
        cerr << Warning( "Found a feedcell %s in an unplaced design, removing."
                       , instanceName.c_str()
                       ) << endl;
        feedOccurrences.push_back( occurrence );
        continue;
      }

      if (masterCell->getAbutmentBox().getHeight() != getSliceHeight()) {
        cmess2 << "     - Using as block: " << occurrence.getCompactString() << "." << endl;

        cerr << Error( "EtesianEngine::setDefaultAb(): Block instances are not managed, \"%s\"."
                     , getString(instance->getName()).c_str() ) << endl;
      }
      cellLength += _bloatCells.getAb( occurrence ).getWidth();
      instanceNb += 1;
    }

    if (cellLength == 0) {
      throw Error( "EtesianEngine::setDefaultAb(): Null surface area computed for \"%s\" (are physical views loaded?)."
                 , getString(getCell()->getName()).c_str()
                 );
    } 

    if (cellLength < 0)
      throw Error( "EtesianEngine::setDefaultAb(): Negative surface area computed for \"%s\" (bad bloat profile?)."
                 , getString(getCell()->getName()).c_str()
                 );

    DbU::Unit bloatLength = _bloatCells.getDxSpace();
    double    bloatMargin = ( (double)cellLength / (double)(cellLength - bloatLength) ) - 1.0;
    
    DbU::Unit gcellLength = (double)cellLength*(1.0+spaceMargin) / (double)getSliceHeight();

    if (gcellLength == 0) {
      throw Error( "EtesianEngine::setDefaultAb(): Null g-length for \"%s\" (are you using the right gauge?)."
                 , getString(getCell()->getName()).c_str()
                 );
    } 

    double rows    = 0.0;
    double columns = 0.0;
    if (not getFixedAbWidth()) {
      if (getFixedAbHeight()) rows = getFixedAbHeight() / getSliceHeight();
      else                    rows = std::ceil( sqrt( (double)gcellLength/aspectRatio ) );
      columns = std::ceil( (double)gcellLength / rows );
    } else {
      columns = getFixedAbWidth() / getSliceHeight();
      rows    = std::ceil( (double)gcellLength / columns );
    }

    UpdateSession::open();
    for ( auto occurrence : feedOccurrences ) {
      static_cast<Instance*>(occurrence.getEntity())->destroy();
    }

    DbU::Unit abWidth = columns*getSliceHeight();
    DbU::Unit adjust  = abWidth % getSliceStep();
    if (adjust) abWidth += getSliceStep() - adjust;

    getCell()->setAbutmentBox( Box( 0
                                  , 0
                                  , abWidth
                                  , rows*getSliceHeight()
                                  ) );
    _placeArea = getCell()->getAbutmentBox();
    UpdateSession::close();
    if (_viewer) _viewer->getCellWidget()->fitToContents();

    cmess1 << "  o  Creating abutment box (margin:" << (spaceMargin*100.0)
           << "% aspect ratio:" << (aspectRatio*100.0)
           << "% g-length:" << DbU::getValueString(cellLength)
           << ")" << endl;
    if (getFixedAbHeight())
       cmess1 << "     - Fixed AB height: " << DbU::getValueString(getFixedAbHeight()) << endl;
    if (getFixedAbWidth())
       cmess1 << "     - Fixed AB width: " << DbU::getValueString(getFixedAbWidth()) << endl;
    cmess1 << "     - Bloat space margin: "
           << setprecision(4) << (bloatMargin*100.0) << "%." << endl;
    cmess1 << "     - " <<  getCell()->getAbutmentBox() << endl;
    cmess1 << "     - GCell grid: [" << (int)columns << "x" << (int)rows << "]" << endl;
  }


  void  EtesianEngine::resetPlacement ()
  {
    cmess1 << "  o  Erasing previous placement of <" << getCell()->getName() << ">" << endl;

    _flatDesign = true;

    Dots  dots ( cmess2, "     ", 80, 1000 );
    if (not cmess2.enabled()) dots.disable();

    Box topAb = getBlockCell()->getAbutmentBox();
    Transformation topTransformation;
    if (getBlockInstance()) topTransformation = getBlockInstance()->getTransformation();
    topTransformation.applyOn( topAb );

    UpdateSession::open();
    for ( Occurrence occurrence : getBlockCell()->getNonTerminalNetlistInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      if (not masterCell->getAbutmentBox().isEmpty()) {
        if (   (instance->getPlacementStatus() != Instance::PlacementStatus::PLACED)
           and (instance->getPlacementStatus() != Instance::PlacementStatus::FIXED ) ) {
          throw Error( "EtesianEngine::toColoquinte(): Non-leaf instance \"%s\" of \"%s\" has an abutment box but is *not* placed."
                     , getString(instance  ->getName()).c_str()
                     , getString(masterCell->getName()).c_str()
                     );
        } else {
          bool isFullyPlaced = true;
          for ( Instance* subInstance : masterCell->getInstances() ) {
            if (   (subInstance->getPlacementStatus() != Instance::PlacementStatus::PLACED)
               and (subInstance->getPlacementStatus() != Instance::PlacementStatus::FIXED ) ) {
              isFullyPlaced = false;
              break;
            }
          }
          if (isFullyPlaced) {
            masterCell->setTerminalNetlist( true );
          }
        }
      }

      if ( masterCell->getAbutmentBox().isEmpty()
         or (   (masterCell->getAbutmentBox().getHeight() == topAb.getHeight())
            and (masterCell->getAbutmentBox().getWidth () == topAb.getWidth ()) ) ) {
      // Have to check here if the model is fully placed or not.
      //masterCell->setAbutmentBox( topAb );
      //instance->setTransformation( Transformation() ); // (0,0,ID).
      //instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
        occurrence.makeInvalid();
        instance->slaveAbutmentBox();
      }
    }

    vector<Occurrence>  feedOccurrences;
    for( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      dots.dot();

      if ( _flatDesign and not occurrence.getPath().getTailPath().isEmpty())
        _flatDesign = true;

      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = occurrence.getCompactString();

      if (CatalogExtension::isFeed(masterCell))
        feedOccurrences.push_back( occurrence );
    }

    for ( auto occurrence : feedOccurrences ) {
      Instance* instance = static_cast<Instance*>(occurrence.getEntity());
      instance->destroy();
    }

    // if (not getBlockCell()->getAbutmentBox().isEmpty() )
    //   setFixedAbHeight( getBlockCell()->getAbutmentBox().getHeight() );
    getBlockCell()->setAbutmentBox( Box() );
    getBlockCell()->resetFlags( Cell::Flags::Placed );
    _placeArea.makeEmpty();
    UpdateSession::close();

    dots.finish( Dots::Reset );

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


  size_t  EtesianEngine::toColoquinte ()
  {
    AllianceFramework* af          = AllianceFramework::get();
    DbU::Unit          hpitch      = getSliceStep();
    DbU::Unit          vpitch      = getSliceStep();
    DbU::Unit          sliceHeight = getSliceHeight();
    bool               isFlexLib   = (getGauge()->getName() == "FlexLib");

    cmess1 << "  o  Converting \"" << getCell()->getName() << "\" into Coloquinte." << endl;
    cmess1 << ::Dots::asString("     - H-pitch"    , DbU::getValueString(hpitch)) << endl;
    cmess1 << ::Dots::asString("     - V-pitch"    , DbU::getValueString(vpitch)) << endl;
    if (isFlexLib)
      cmess1 << ::Dots::asString("     - Using patches for"    , "\"FlexLib\"") << endl;
    cmess2 << "     o  Looking through the hierarchy." << endl;

    for( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = occurrence.getCompactString();

      if (masterCell->getAbutmentBox().getHeight() != getSliceHeight()) {
        cmess2 << "        - Using as block: " << instanceName << "." << endl;

        if (instance->getPlacementStatus() != Instance::PlacementStatus::FIXED) {
          cerr << Error( "EtesianEngine::toColoquinte(): Block instance \"%s\" is *not* FIXED."
                       , getString(instance->getName()).c_str() ) << endl;
        }
      }
    }
    cmess2 << "        - Whole place area: " << getBlockCell()->getAbutmentBox() << "." << endl;
    cmess2 << "        - Sub-place Area: " << _placeArea << "." << endl;
    DbU::Unit totalLength = (_placeArea.getHeight()/sliceHeight) * _placeArea.getWidth();
    DbU::Unit usedLength  = 0;

    Dots  dots ( cmess2, "       ", 80, 1000 );
    if (not cmess2.enabled()) dots.disable();
    
    size_t  instancesNb = 0;
    size_t  fixedNb     = 0;
    size_t  registerNb  = 0;
    Box     topAb       = _placeArea;
    Transformation topTransformation;
    if (getBlockInstance()) {
      topTransformation = getBlockInstance()->getTransformation();
      topTransformation.applyOn( topAb );
      for ( Instance* instance : getCell()->getInstances() ) {
        if (instance == getBlockInstance()) continue;
        string masterName = getString( instance->getMasterCell()->getName() );
        if (masterName.substr(0,3) == "sff") ++registerNb;
        Box instanceAb = instance->getAbutmentBox();
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          if (topAb.intersect(instanceAb)) {
            ++instancesNb;
            ++fixedNb;
            totalLength -= (instanceAb.getHeight()/sliceHeight) * instanceAb.getWidth();
          }
        }
      }
    }

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) ) {
      ++instancesNb;
      Instance* instance   = static_cast<Instance*>(occurrence.getEntity());
      Box       instanceAb = instance->getAbutmentBox();
      string    masterName = getString( instance->getMasterCell()->getName() );
      if (masterName.substr(0,3) == "sff") ++registerNb;
      if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
        ++fixedNb;
        totalLength -= (instanceAb.getHeight()/sliceHeight) * instanceAb.getWidth();
      } else {
        usedLength += (instanceAb.getHeight()/sliceHeight) * instanceAb.getWidth();
      //cerr << DbU::getValueString(usedLength) << " " << instance << endl;
      }
    }
    if (instancesNb <= fixedNb) {
      cerr << Error( "EtesianEngine::toColoquinte(): \"%s\" has no instance to place, doing nothing."
                   , getString(getCell()->getName()).c_str()
                   ) << endl;
      return 0;
    }
  //cerr << "total length=" << DbU::getValueString(totalLength) << endl;
  //cerr << "used length=" << DbU::getValueString(usedLength) << endl;
    cmess1 << ::Dots::asPercentage( "     - Effective space margin"
                                  , (float)(totalLength-usedLength)/(float)totalLength ) << endl;

  // Coloquinte circuit description data-structures.
  // One dummy fixed instance at the end
    vector<Transformation>  idsToTransf ( instancesNb+1 );
    vector<temporary_cell>  instances   ( instancesNb+1 );
    vector< point<int_t> >  positions   ( instancesNb+1 );
    vector< point<bool> >   orientations( instancesNb+1, point<bool>(true, true) );

    cmess1 << ::Dots::asUInt( "     - Number of instances ", instancesNb ) << endl;
    if (instancesNb) {
      float ratio = ((float)registerNb / (float)instancesNb) * 100.0;
      ostringstream os1;
      os1 << registerNb << " (" << fixed << setprecision(2) << ratio << "%)";
      cmess1 << ::Dots::asString( "     - Registers (DFF) ", os1.str() ) << endl;
      ratio = ((float)_bufferCount / (float)instancesNb) * 100.0;
      ostringstream os2;
      os2 << _bufferCount << " (" << fixed << setprecision(2) << ratio << "%)";
      cmess1 << ::Dots::asString( "     - Buffers ", os2.str() ) << endl;
    }
    cout.flush();

    cmess1 << "     - Building RoutingPads (transhierarchical)" << endl;
  //getCell()->flattenNets( Cell::Flags::BuildRings|Cell::Flags::NoClockFlatten );
  //getCell()->flattenNets( getBlockInstance(), Cell::Flags::NoClockFlatten );
    getCell()->flattenNets( NULL, Cell::Flags::NoClockFlatten );

    bool    tooManyInstances = false;
    index_t instanceId       = 0;
    if (getBlockInstance()) {
      for ( Instance* instance : getCell()->getInstances() ) {
        if (instance == getBlockInstance()) continue;
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          Box overlapAb = topAb.getIntersection( instance->getAbutmentBox() );
          if (not overlapAb.isEmpty()) {
          // Upper rounded
            int_t xsize = (overlapAb.getWidth () + vpitch - 1) / vpitch;
            int_t ysize = (overlapAb.getHeight() + hpitch - 1) / hpitch;
          // Lower rounded
            int_t xpos  = overlapAb.getXMin() / vpitch;
            int_t ypos  = overlapAb.getYMin() / hpitch;

            instances[instanceId].size       = point<int_t>( xsize, ysize );
            instances[instanceId].list_index = instanceId;
            instances[instanceId].area       = static_cast<capacity_t>(xsize) * static_cast<capacity_t>(ysize);
            positions[instanceId]            = point<int_t>( xpos, ypos );
            instances[instanceId].attributes = 0;

            _instsToIds.insert( make_pair(instance,instanceId) );
            _idsToInsts.push_back( make_tuple(instance,vector<RoutingPad*>()) );
            // cerr << endl;
            // cerr << "FIXED " << instance << endl;
            // cerr << "  id=" << instanceId
            //      << " " << instance << " size:(" << xsize << " " << ysize
            //      << ") pos:(" << xpos << " " << ypos << ")" << endl;
            ++instanceId;
            dots.dot();
          }
        }
      }
    }

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      if (tooManyInstances or (instanceId == instancesNb)) {
        tooManyInstances = true;
        ++instanceId;
        continue;
      }

      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();
      string    instanceName = occurrence.getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      if (CatalogExtension::isFeed(masterCell)) {
        throw Error( "EtesianEngine::toColoquinte(): Feed instance \"%s\" found."
                   , instanceName.c_str() );
      }

      Box instanceAb = _bloatCells.getAb( occurrence );

      Transformation instanceTransf = instance->getTransformation();
      occurrence.getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      // Upper rounded
      int_t xsize = (instanceAb.getWidth () + vpitch - 1) / vpitch;
      int_t ysize = (instanceAb.getHeight() + hpitch - 1) / hpitch;
      // Lower rounded
      int_t xpos  = instanceAb.getXMin() / vpitch;
      int_t ypos  = instanceAb.getYMin() / hpitch;

      //if (xsize <  6) xsize += 2;

      // if ( (ysize != 1) and not instance->isFixed() ) {
      //   cerr << Error( "EtesianEngine::toColoquinte(): Instance \"%s\" of \"%s\" is a block (height: %d)." 
      //                , instanceName.c_str()
      //                , getString(masterCell->getName()).c_str()
      //                , ysize ) << endl;
      // }

      // cerr << instance << " size:(" << xsize << " " << ysize
      //     << ") pos:(" << xpos << " " << ypos << ")" << endl;

      string masterName = getString( instance->getMasterCell()->getName() );
      if (isFlexLib and not instance->isFixed() and (masterName == "buf_x8"))
         ++xsize;

      instances[instanceId].size       = point<int_t>( xsize, ysize );
      instances[instanceId].list_index = instanceId;
      instances[instanceId].area       = static_cast<capacity_t>(xsize) * static_cast<capacity_t>(ysize);
      positions[instanceId]            = point<int_t>( xpos, ypos );

      if ( not instance->isFixed() and instance->isTerminalNetlist() ) {
        instances[instanceId].attributes = coloquinte::XMovable
                                          |coloquinte::YMovable
                                          |coloquinte::XFlippable
                                          |coloquinte::YFlippable;
      } else {
        instances[instanceId].attributes = 0;
        // cerr << endl;
        // cerr << "FIXED " << instance << endl;
        // cerr << "  id=" << instanceId
        //      << " " << instance << " size:(" << xsize << " " << ysize
        //      << ") pos:(" << xpos << " " << ypos << ")" << endl;
      }

      _instsToIds.insert( make_pair(instance,instanceId) );
      _idsToInsts.push_back( make_tuple(instance,vector<RoutingPad*>()) );
      ++instanceId;
      dots.dot();
    }

    if (tooManyInstances) {
      throw Error( "EtesianEngine::toColoquinte(): Too many leaf instances, %d (expected: %d)\n"
                   "        maybe a virtual flattening problem."
                 , instanceId, instancesNb 
                 );
    }
    if (instanceId < instancesNb) {
      throw Error( "EtesianEngine::toColoquinte(): Too little leaf instances, %d (expected: %d)\n"
                   "        maybe a virtual flattening problem."
                 , instanceId, instancesNb
                 );
    }

    // Dummy fixed instance at the end
    instances[instanceId].size       = point<int_t>( 0, 0 );
    instances[instanceId].list_index = instanceId;
    instances[instanceId].area       = 0;
    positions[instanceId]            = point<int_t>( 0, 0 );
    instances[instanceId].attributes = 0;

    dots.finish( Dots::Reset|Dots::FirstDot );

    size_t netsNb = 0;
    for ( Net* net : getCell()->getNets() )
    {
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER ) excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND) excludedType = "GROUND";
      if (net->getType() == Net::Type::CLOCK ) excludedType = "CLOCK";
      if (excludedType) {
        cparanoid << Warning( "%s is not a routable net (%s,excluded)."
                            , getString(net).c_str(), excludedType ) << endl;
        continue;
      }
      if (af->isBLOCKAGE(net->getName())) continue;

      ++netsNb;
    }

    cmess1 << "     - Converting " << netsNb << " nets" << endl;

    vector<temporary_net>  nets ( netsNb );
    vector<temporary_pin>  pins;
    _idsToNets.resize( netsNb );

    unsigned int netId = 0;
    for ( Net* net : getCell()->getNets() )
    {
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER ) excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND) excludedType = "GROUND";
      if (net->getType() == Net::Type::CLOCK ) excludedType = "CLOCK";
      if (excludedType) continue;
      if (af->isBLOCKAGE(net->getName())) continue;

      dots.dot();

      _netsToIds.insert( make_pair(net,netId) );
      _idsToNets[netId] = make_tuple( net, _instsToIds.size(), 0 );
      nets[netId] = temporary_net( netId, 1 );

    //cerr << "+ " << net << endl;
      
      string topCellInstancePin = getString(getCell()->getName()) + ":C";

      for ( RoutingPad* rp : net->getRoutingPads() ) {
        Path path = rp->getOccurrence().getPath();
        Pin* pin  = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() ); 
        if (pin) {
          if (path.isEmpty()) {
            Point pt   = rp->getCenter();
            int_t xpin = pt.getX() / vpitch;
            int_t ypin = pt.getY() / hpitch;
          // Dummy last instance
            pins.push_back( temporary_pin( point<int_t>(xpin,ypin), instanceId, netId ) );
          }
          continue;
        }

        if (getBlockInstance()) {
        // For Gabriel Gouvine : if there are multiple blocks (i.e. we have a true
        // floorplan, there may be RoutingPad that are elsewhere. We should check
        // that the RP is placed or is inside a define area (the abutment box of
        // it's own block). No example yet of that case, though.
          if (path.getHeadInstance() != getBlockInstance()) {
            cerr << Warning( "EtesianEngine::toColoquinte(): Net %s has a RoutingPad that is not rooted at the placed instance.\n"
                             "          * Placed instance: %s\n"
                             "          * RoutingPad: %s"
                           , getString(net).c_str()
                           , getString(getBlockInstance()).c_str()
                           , getString(rp->getOccurrence()).c_str()
                           ) << endl;
          //cerr << "Outside RP: " << rp << endl;
            continue;
          }
        }

        Instance* instance = extractInstance    ( rp );
        string    insName  = extractInstanceName( rp );
        Point     offset   = extractRpOffset    ( rp );

        int_t xpin    = offset.getX() / vpitch;
        int_t ypin    = offset.getY() / hpitch;

        auto  iid = _instsToIds.find( instance );
        if (iid == _instsToIds.end()) {
          if (not instance) {
            cerr << Error( "Unable to lookup instance \"%s\".", insName.c_str() ) << endl;
          }
        } else {
          pins.push_back( temporary_pin( point<int_t>(xpin,ypin), (*iid).second, netId ) );
          Net*  rpNet = NULL;
          Plug* plug  = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
          if (plug) {
            rpNet = plug->getMasterNet();
            if (rpNet->getDirection() & Net::Direction::DirOut) {
              std::get<1>( _idsToNets[netId] ) = (*iid).second;
            }
          }
        }
      }

      netId++;
    }
    dots.finish( Dots::Reset );

    _densityLimits = new coloquinte::density_restrictions ();
    _surface = new box<int_t>( (int_t)(topAb.getXMin() / vpitch)
                             , (int_t)(topAb.getXMax() / vpitch)
                             , (int_t)(topAb.getYMin() / hpitch)
                             , (int_t)(topAb.getYMax() / hpitch)
                             );
    _circuit = new netlist( instances, nets, pins );
    _circuit->selfcheck();
    _placementLB = new coloquinte::placement_t ();
    _placementLB->positions_    = positions;
    _placementLB->orientations_ = orientations;
    _placementUB = new coloquinte::placement_t ( *_placementLB );

    return instancesNb-fixedNb;
  }


  void  EtesianEngine::adjustSliceHeight ()
  {
    /*
     * Modify the slice height if it doesn't match the one given by the Alliance Framework.
     * Useful for Bookshelf benchmarks
     */

    bool isSliceHeightSet = false;
    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell* masterCell = instance->getMasterCell();

      if (   (instance->getPlacementStatus() != Instance::PlacementStatus::PLACED)
         and (instance->getPlacementStatus() != Instance::PlacementStatus::FIXED ))
      {
        DbU::Unit cellHeight = masterCell->getAbutmentBox().getHeight();
        bool sliceHeightChange = cellHeight != getSliceHeight();
        if (isSliceHeightSet)
        {
          if (sliceHeightChange) throw Error( "EtesianEngine::toColoquinte(): Cannot manage unplaced block, instance \"%s\" of \"%s\": slice height was set to %d but cell height is %d."
                 , getString(instance  ->getName()).c_str()
                 , getString(masterCell->getName()).c_str()
                 , getSliceHeight()
                 , cellHeight
                 );
        }
        else
        {
          if (sliceHeightChange) cerr << Warning("Adjusting slice height from %d to %d fit a placeable cell.", getSliceHeight(), cellHeight) << endl;
          _sliceHeight = cellHeight;
        }
        isSliceHeightSet = true;
      }
    }
  }


  void  EtesianEngine::preplace ()
  {
    using namespace coloquinte::gp;

    // Perform a very quick legalization pass
    cmess2 << "  o  Simple legalization." << endl;
    auto first_legalizer = region_distribution::uniform_density_distribution(*_surface, *_circuit, *_placementLB);
    first_legalizer.selfcheck();
    get_rough_legalization( *_circuit, *_placementUB, first_legalizer);

    *_placementLB = *_placementUB;

    // Early topology-independent solution with a star model + negligible pulling forces to avoid dumb solutions
    // Spreads well to help subsequent optimization passes
    cmess2 << "  o  Star (*) Optimization." << endl;
    auto solv = get_star_linear_system( *_circuit, *_placementLB, 1.0, 0, 10) // Limit the number of pins: don't want big awful nets with high weight
              + get_pulling_forces( *_circuit, *_placementUB, 1000000.0);
    solve_linear_system( *_circuit, *_placementLB, solv, 200 );
    _progressReport2("     [---]" );
  }


  void  EtesianEngine::roughLegalize ( float minDisruption, unsigned options )
  {
    using namespace coloquinte::gp;
    // Create a legalizer and bipartition it until we have sufficient precision
    auto legalizer = (options & ForceUniformDensity) != 0 ?
        region_distribution::uniform_density_distribution (*_surface, *_circuit, *_placementLB, *_densityLimits)
      : region_distribution::full_density_distribution    (*_surface, *_circuit, *_placementLB, *_densityLimits);
    while(legalizer.region_dimensions().x > 2*legalizer.region_dimensions().y)
      legalizer.x_bipartition();
    while(2*legalizer.region_dimensions().x < legalizer.region_dimensions().y)
      legalizer.y_bipartition();
    while( std::max(legalizer.region_dimensions().x, legalizer.region_dimensions().y)*4 > minDisruption ) {
      legalizer.x_bipartition();
      legalizer.y_bipartition();
      legalizer.redo_line_partitions();
      legalizer.redo_diagonal_bipartitions();
      legalizer.redo_line_partitions();
      legalizer.redo_diagonal_bipartitions();
      legalizer.selfcheck();
    }
    // Keep the orientation between LB and UB
    *_placementUB = *_placementLB;
    // Update UB
    get_rough_legalization( *_circuit, *_placementUB, legalizer );
  }


  void  EtesianEngine::globalPlace ( float initPenalty
                                   , float minDisruption
                                   , float targetImprovement
                                   , float minInc
                                   , float maxInc
                                   , unsigned options )
  {
    using namespace coloquinte::gp;

    bool    antennaDone     = false;
    float_t penaltyIncrease = minInc;
    float_t linearDisruption  = get_mean_linear_disruption(*_circuit, *_placementLB, *_placementUB);
    float_t pullingForce = initPenalty;
    float_t upperWL = static_cast<float_t>(get_HPWL_wirelength(*_circuit, *_placementUB)),
            lowerWL = static_cast<float_t>(get_HPWL_wirelength(*_circuit, *_placementLB));
    float_t prevOptRatio = lowerWL / upperWL;

    index_t i=0;
    do{
      roughLegalize(minDisruption, options);
      if(options & UpdateUB)
        _updatePlacement( _placementUB );

      ostringstream label;
      label.str("");
      label  << "     [" << setw(3) << setfill('0') << i << setfill(' ') << "] "
             << setw(7) << fixed << setprecision(1) << linearDisruption << "% Bipart.";
      _progressReport1(label.str() );

      upperWL = static_cast<float_t>(get_HPWL_wirelength(*_circuit, *_placementUB));
    //float_t legRatio = lowerWL / upperWL;

      // Get the system to optimize (tolerance, maximum and minimum pin counts)
      // and the pulling forces (threshold distance)
      auto opt_problem = (options & SteinerModel) ?
        get_RSMT_linear_system  ( *_circuit, *_placementLB, minDisruption, 2, 100000 ) 
      : get_HPWLF_linear_system ( *_circuit, *_placementLB, minDisruption, 2, 100000 ); 
      auto solv = opt_problem
                + get_linear_pulling_forces( *_circuit
                                           , *_placementUB
                                           , *_placementLB
                                           , pullingForce
                                           , 2.0f * linearDisruption);
      solve_linear_system( *_circuit, *_placementLB, solv, 200 ); // 200 iterations
      _progressReport2("                    Linear." );

      if(options & UpdateLB)
        _updatePlacement( _placementUB );

      // Optimize orientation sometimes
      if (i%5 == 0) {
          optimize_exact_orientations( *_circuit, *_placementLB );
          _progressReport2("                    Orient." );
      }

      lowerWL = static_cast<float_t>(get_HPWL_wirelength(*_circuit, *_placementLB));
      float_t optRatio = lowerWL / upperWL;

     /*
      * Schedule the penalty during global placement to achieve uniform improvement
      *
      * Currently, the metric considered is the ratio optimized HPWL/legalized HPWL
      * Other ones, like the disruption itself, may be considered
      */
      penaltyIncrease = std::min(maxInc, std::max(minInc,
              penaltyIncrease * std::sqrt( targetImprovement / (optRatio - prevOptRatio) )
          ) );
      cparanoid << "                    L/U ratio: " << 100*optRatio << "% (previous: " << 100*prevOptRatio << "%)\n"
                << "                    Pulling force: " <<  pullingForce << " Increase: " << penaltyIncrease << endl;

      pullingForce += penaltyIncrease;
      prevOptRatio = optRatio;

      linearDisruption  = get_mean_linear_disruption(*_circuit, *_placementLB, *_placementUB);
      ++i;

      if ((linearDisruption < getAntennaInsertThreshold()*100.0) and not antennaDone) {
      //antennaProtect();
        antennaDone = true;
      }
      // First way to exit the loop: UB and LB difference is <10%
      // Second way to exit the loop: the legalization is close enough to the previous result
    } while (linearDisruption > minDisruption and prevOptRatio <= 0.9);
    _updatePlacement( _placementUB );
  }


  void  EtesianEngine::detailedPlace ( int iterations, int effort, unsigned options )
  {
    using namespace coloquinte::gp;
    using namespace coloquinte::dp;

    int_t sliceHeight = getSliceHeight() / getSliceStep();
    roughLegalize(sliceHeight, options);
    // TODO: for uniform density distribution, add some margin to the cell sizes so we don't disrupt it during detailed placement

    for ( int i=0; i<iterations; ++i ){
        ostringstream label;
        label.str("");
        label  << "     [" << setw(3) << setfill('0') << i << setfill(' ') << "]";

        optimize_x_orientations( *_circuit, *_placementUB ); // Don't disrupt VDD/VSS connections in a row
        _progressReport1(label.str() + " Oriented ......." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        auto legalizer = legalize( *_circuit, *_placementUB, *_surface, sliceHeight );
        coloquinte::dp::get_result( *_circuit, legalizer, *_placementUB );
        _progressReport1("           Legalized ......" );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        row_compatible_orientation( *_circuit, legalizer, true );
        swaps_global_HPWL( *_circuit, legalizer, 3, 4 );
        coloquinte::dp::get_result( *_circuit, legalizer, *_placementUB );
        _progressReport1("           Global Swaps ..." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if(options & SteinerModel)
          OSRP_noncvx_RSMT( *_circuit, legalizer );
        else
          OSRP_convex_HPWL( *_circuit, legalizer );
        coloquinte::dp::get_result( *_circuit, legalizer, *_placementUB );
        _progressReport1("           Row Optimization" );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if(options & SteinerModel)
          swaps_row_noncvx_RSMT( *_circuit, legalizer, effort+2 );
        else
          swaps_row_convex_HPWL( *_circuit, legalizer, effort+2 );
        coloquinte::dp::get_result( *_circuit, legalizer, *_placementUB );
        _progressReport1("           Local Swaps ...." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if (i == iterations-1) {
          //swaps_row_convex_RSMT( *_circuit, legalizer, 4 );
          row_compatible_orientation( *_circuit, legalizer, true );
          coloquinte::dp::get_result( *_circuit, legalizer, *_placementUB );
          verify_placement_legality( *_circuit, *_placementUB, *_surface );
          _progressReport1("           Final Legalize ." );
        }
    }
    *_placementLB = *_placementUB; // In case we run other passes
    _updatePlacement( _placementUB, FinalStage );
  }


  void  EtesianEngine::antennaProtect ()
  {
    DbU::Unit maxWL = getAntennaGateMaxWL();
    if (not maxWL) return;
    
    cmess1 << "  o  Inserting antenna effect protection." << endl;
    uint32_t count      = 0;
    int_t    diodeWidth = _diodeCell->getAbutmentBox().getWidth() / getSliceStep();
    cdebug_log(122,0) << "diodeWidth=" << diodeWidth << "p" << endl;

    for ( coloquinte::index_t inet=0 ; inet < _circuit->net_cnt() ; ++inet ) {
      DbU::Unit rsmt = toDbU( coloquinte::get_RSMT_length( *_circuit, *_placementUB, inet ) );
      Net*      net  = std::get<0>( _idsToNets[inet] );

      if ((rsmt > maxWL) or net->isExternal()) {
        cdebug_log(122,0) << "| Net [" << inet << "] \"" << net->getName() << "\" may have antenna effect, "
                          << DbU::getValueString(rsmt)
                          << endl;
        std::get<2>( _idsToNets[inet] ) |= NeedsDiode;

        for ( RoutingPad* rp : net->getRoutingPads() ) {
          Segment* segment = dynamic_cast<Segment*>( rp->getOccurrence().getEntity() );
          if (not segment) continue;
          if (segment->getNet()->getDirection() & Net::Direction::DirOut) continue;

          Instance* instance = extractInstance( rp );
          if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
            continue;

          auto iinst = _instsToIds.find( instance );
          if (iinst == _instsToIds.end()) continue;
          
          std::get<1>( _idsToInsts[ (*iinst).second ] ).push_back( rp );
          coloquinte::point<int_t> cell_size = _circuit->get_cell_size( (*iinst).second );
          cell_size.x += 2*diodeWidth;
          _circuit->set_cell_size( (*iinst).second, cell_size );
          ++count;
        }
      }
    }
    cmess1 << ::Dots::asInt( "     - Inserted diodes", count ) << endl;
  }


  void  EtesianEngine::loadLeafCellLayouts ()
  {
    AllianceFramework* af = AllianceFramework::get();
    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) ) {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      af->getCell( getString(masterCell->getName()), Catalog::State::Physical );
    }
  }


  void  EtesianEngine::place ()
  {
    if (getBlockCell()->isPlaced()) {
      cerr << Warning( "EtesianEngine::place(): The cell \"%s\" is already placed (aborting)"
                     , getString(getBlockCell()->getName()).c_str()
                     ) << std::endl;
      return;
    }
    getBlockCell()->uniquify();

    getConfiguration()->print( getCell() );
    adjustSliceHeight();
    if ( (getCell() == getBlockCell()) and getCell()->getAbutmentBox().isEmpty() ) {
      resetPlacement();
      setDefaultAb();
    }

  //findYSpin();
    if (not toColoquinte()) return;

    Effort        placementEffort = getPlaceEffort();
    GraphicUpdate placementUpdate = getUpdateConf();
    Density       densityConf     = getSpreadingConf();
    double        sliceHeight     = getSliceHeight() / getSliceStep();

    cmess1 << "  o  Running Coloquinte." << endl;
    cmess2 << "     - Computing initial placement..." << endl;
    cmess2 << right;
    startMeasures();

    preplace();

    float_t minPenaltyIncrease, maxPenaltyIncrease, targetImprovement;
    int detailedIterations, detailedEffort;
    unsigned globalOptions=0, detailedOptions=0;

    if (placementUpdate == UpdateAll) {
      globalOptions   |= (UpdateUB | UpdateLB);
      detailedOptions |=  UpdateDetailed;
    }
    else if (placementUpdate == LowerBound) {
      globalOptions |= UpdateLB;
    }

    if (densityConf == ForceUniform)
      globalOptions |= ForceUniformDensity;

    if (placementEffort == Fast) {
      minPenaltyIncrease = 0.005f;
      maxPenaltyIncrease = 0.08f;
      targetImprovement  = 0.05f; // 5/100 per iteration
      detailedIterations = 1;
      detailedEffort     = 0;
    } else if (placementEffort == Standard) {
      minPenaltyIncrease = 0.001f;
      maxPenaltyIncrease = 0.04f;
      targetImprovement  = 0.02f; // 2/100 per iteration
      detailedIterations = 2;
      detailedEffort     = 1;
    } else if (placementEffort == High) {
      minPenaltyIncrease = 0.0005f;
      maxPenaltyIncrease = 0.02f;
      targetImprovement  = 0.01f; // 1/100 per iteration
      detailedIterations = 4;
      detailedEffort     = 2;
    } else {
      minPenaltyIncrease = 0.0002f;
      maxPenaltyIncrease = 0.01f;
      targetImprovement  = 0.005f; // 5/1000 per iteration
      detailedIterations = 7;
      detailedEffort     = 3;
    }

    cmess1 << "  o  Global placement." << endl;
    globalPlace(minPenaltyIncrease, sliceHeight, targetImprovement, minPenaltyIncrease, maxPenaltyIncrease, globalOptions);

    cmess1 << "  o  Detailed Placement." << endl;
    detailedPlace(detailedIterations, detailedEffort, detailedOptions);

  //toHurricane();
  //addFeeds();

    cmess1 << "  o  Placement finished." << endl;
    stopMeasures();
    printMeasures();
    cmess1 << ::Dots::asString
      ( "     - HPWL", DbU::getValueString( (DbU::Unit)coloquinte::gp::get_HPWL_wirelength(*_circuit,*_placementUB )*getSliceStep() ) ) << endl;
    cmess1 << ::Dots::asString
      ( "     - RMST", DbU::getValueString( (DbU::Unit)coloquinte::gp::get_RSMT_wirelength(*_circuit,*_placementUB )*getSliceStep() ) ) << endl;

    UpdateSession::open();
    for ( Net* net : getCell()->getNets() ) {
      for ( RoutingPad* rp : net->getComponents().getSubSet<RoutingPad*>() ) {
        rp->invalidate();
      }
    }

    getCell()->setFlags( Cell::Flags::Placed );
    for ( Occurrence occurrence : getCell()->getNonTerminalNetlistInstanceOccurrences(getBlockInstance()) ) {
      Instance* instance = static_cast<Instance*>(occurrence.getEntity());
      if (instance->getPlacementStatus() == Instance::PlacementStatus::UNPLACED)
        instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
    }
    UpdateSession::close();
  }


  void  EtesianEngine::_progressReport1 ( string label ) const
  {
    size_t w      = label.size();
    string indent ( w, ' ' );
    if (not w) {
      label  = string( 5, ' ' );
      indent = label;
    }

    cmess2 << label
           << " HPWL=" << setw(14) << DbU::getValueString(toDbU(coloquinte::gp::get_HPWL_wirelength( *_circuit, *_placementUB )))
           << " RMST=" << setw(14) << DbU::getValueString(toDbU(coloquinte::gp::get_RSMT_wirelength( *_circuit, *_placementUB )))
           << endl;
    cparanoid << indent
           <<   " L-Dsrpt=" << setw(8) << coloquinte::gp::get_mean_linear_disruption   ( *_circuit, *_placementLB, *_placementUB )
           <<   " Q-Dsrpt=" << setw(8) << coloquinte::gp::get_mean_quadratic_disruption( *_circuit, *_placementLB, *_placementUB )
           << endl;
  }


  void  EtesianEngine::_progressReport2 ( string label ) const
  {
    size_t w      = label.size();
    string indent ( w, ' ' );
    if (not w) {
      label  = string( 5, ' ' );
      indent = label;
    }

    cmess2 << label
           << " HPWL=" << setw(14) << DbU::getValueString(toDbU(coloquinte::gp::get_HPWL_wirelength( *_circuit, *_placementLB )))
           << " RMST=" << setw(14) << DbU::getValueString(toDbU(coloquinte::gp::get_RSMT_wirelength( *_circuit, *_placementLB )))
           << endl;
  }


  void  EtesianEngine::_updatePlacement ( const coloquinte::placement_t* placement, uint32_t flags )
  {
    UpdateSession::open();

    Transformation topTransformation;
    if (getBlockInstance()) topTransformation = getBlockInstance()->getTransformation();
    topTransformation.invert();

    DbU::Unit diodeWidth = (_diodeCell) ? _diodeCell->getAbutmentBox().getWidth() : 0;
    vector< tuple<RoutingPad*,Transformation> > diodeInsts;

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      DbU::Unit hpitch          = getSliceStep();
      DbU::Unit vpitch          = getSliceStep();
      Point     instancePosition;
      Instance* instance        = static_cast<Instance*>(occurrence.getEntity());
      string    instanceName    = occurrence.getCompactString();
    // Remove the enclosing brackets...
      instanceName.erase( 0, 1 );
      instanceName.erase( instanceName.size()-1 );

      auto iid = _instsToIds.find( instance );
      if (iid == _instsToIds.end() ) {
        cerr << Error( "Unable to lookup instance <%s>.", instanceName.c_str() ) << endl;
      } else {
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
          continue;

      //uint32_t       outputSide = getOutputSide( instance->getMasterCell() );
        point<int_t>   position   = placement->positions_[(*iid).second];
        Transformation cellTrans  = toTransformation( position
                                                    , placement->orientations_[(*iid).second]
                                                    , instance->getMasterCell()
                                                    , hpitch
                                                    , vpitch
                                                    );
        topTransformation.applyOn( cellTrans );
      //if (flags & FinalStage)
      //  cerr << "Raw position of <" << instanceName << " @" << cellTrans << endl;

        const vector<RoutingPad*>& rps = std::get<1>( _idsToInsts[(*iid).second] );
        if ((flags & FinalStage) and not rps.empty()) {
          DbU::Unit sign       = 1;
          DbU::Unit cellWidth  = instance->getMasterCell()->getAbutmentBox().getWidth();
          cdebug_log(122,0) << "cellWidth="  << DbU::getValueString(cellWidth) << endl;
          cdebug_log(122,0) << "diodeWidth=" << DbU::getValueString(diodeWidth) << endl;
          if (  (cellTrans.getOrientation() == Transformation::Orientation::R2)
             or (cellTrans.getOrientation() == Transformation::Orientation::MX)) {
            sign = -1;
          }
          for ( size_t i=0 ; i<rps.size() ; ++i ) {
            cdebug_log(122,0) << "diode position [" << i << "] " << DbU::getValueString((DbU::Unit)(cellTrans.getTx() + cellWidth + i*diodeWidth)) << endl;
            diodeInsts.push_back
              ( make_tuple( rps[i]
                          , Transformation( cellTrans.getTx() + sign * (cellWidth + i*diodeWidth)
                                          , cellTrans.getTy()
                                          , cellTrans.getOrientation() ))
              );
          }
        }

      // This is temporary as it's not trans-hierarchic: we ignore the positions
      // of all the intermediary instances.
        instance->setTransformation( cellTrans );
        instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
      }
    }

    if (_diodeCell) {
      Net* diodeOutput = NULL;
      for ( Net* net : _diodeCell->getNets() ) {
        if (net->isSupply() or not net->isExternal()) continue;
        diodeOutput = net;
        break;
      }

      for ( auto diodeInfos : diodeInsts ) {
        RoutingPad* rp        = std::get<0>( diodeInfos );
        Net*        topNet    = rp->getNet();
        Instance*   instance  = extractInstance( rp );
        Cell*       ownerCell = instance->getCell();
        Instance*   diode     = _createDiode( ownerCell );
        diode->setTransformation ( std::get<1>( diodeInfos ));
        diode->setPlacementStatus( Instance::PlacementStatus::PLACED );

        cdebug_log(122,0) << "Driver net=" << topNet << endl;
        cdebug_log(122,0) << "  " << instance << " @" << instance->getTransformation() << endl;

        Plug* sinkPlug = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
        if (sinkPlug) {
          cdebug_log(122,0) << "  Bind diode input:" << endl;
          Plug* diodePlug  = diode->getPlug( diodeOutput );
          diodePlug->setNet( sinkPlug->getNet() );
          
          cdebug_log(122,0) << "    " << diode    << " @" << diode   ->getTransformation() << endl;
          cdebug_log(122,0) << "    topNet->getCell():" << topNet->getCell() << endl;
          cdebug_log(122,0) << "    " << rp->getOccurrence().getPath() << endl;
          Path path = rp->getOccurrence().getPath().getHeadPath();
          RoutingPad::create( topNet, Occurrence(diodePlug,path), RoutingPad::BiggestArea );
        }
      }
    }

    UpdateSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


  Instance* EtesianEngine::_createDiode ( Cell* owner )
  {
    if (not _diodeCell) return NULL;
    return Instance::create( owner, getUniqueDiodeName(), _diodeCell );
  }


  string  EtesianEngine::getUniqueDiodeName ()
  {
    ostringstream os;
    os << "diode_" << _getNewDiodeId();
    return os.str();
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
    Record* record = Super::_getRecord ();
                                     
    if (record) {
      record->add( getSlot( "_configuration",  _configuration ) );
      record->add( getSlot( "_area"         ,  _area ) );
      record->add( getSlot( "_diodeCount"   ,  _diodeCount ) );
    }
    return record;
  }


} // Etesian namespace.

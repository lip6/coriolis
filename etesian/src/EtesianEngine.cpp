// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2014-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./EtesianEngine.cpp"                      |
// +-----------------------------------------------------------------+



#include <sstream>
#include <fstream>
#include <iomanip>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/utilities/Dots.h"
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
#include "crlcore/Histogram.h"
#include "crlcore/AllianceFramework.h"
#include "etesian/EtesianEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using Etesian::EtesianEngine;
  using coloquinte::CellOrientation;

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
    Component* component  = rp->_getEntityAs<Component>();

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


  Transformation  toTransformation ( coloquinte::Point position
                                   , coloquinte::CellOrientation orientation
                                   , Cell*         model
                                   , DbU::Unit     hpitch
                                   , DbU::Unit     vpitch
                                   )
  {
    DbU::Unit tx = position.x * vpitch;
    DbU::Unit ty = position.y * hpitch;

    Box       cellBox    = model->getAbutmentBox();
    Transformation::Orientation orient = Transformation::Orientation::ID;

    switch (orientation) {
      case CellOrientation::N:
        orient = Transformation::Orientation::ID;
        break;
      case CellOrientation::R180:
        ty    +=   cellBox.getHeight();
        tx    +=   cellBox.getWidth();
        orient = Transformation::Orientation::R2;
        break;
      case CellOrientation::MY:
        orient = Transformation::Orientation::MY;
        ty    +=   cellBox.getHeight();
        break;
      case CellOrientation::MX:
        orient = Transformation::Orientation::MX;
        tx    +=   cellBox.getWidth();
        break;
      /*
      case CellOrientation::R90:
        orient = Transformation::Orientation::R1;
        // TODO
        break;
      case CellOrientation::R270:
        orient = Transformation::Orientation::R3;
        // TODO
        break;
      case CellOrientation::MX90:
        orient = Transformation::Orientation::XR;
        // TODO
        break;
      case CellOrientation::MY90:
        orient = Transformation::Orientation::YR;
        // TODO
        break;
      */
      default:
        throw Error("Unsupported orientation " + toString(orientation));
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
    , _trackAvoids  ()
    , _surface      (NULL)
    , _circuit      (NULL)
    , _placementLB  (NULL)
    , _placementUB  (NULL)
    , _instsToIds   ()
    , _idsToInsts   ()
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
    , _excludedNets ()
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

      if (feedNames.empty()) {
        cerr << Warning( "EtesianEngine::_postCreate() No feed cells configured.\n"
                         "          (please configure \"cfg.etesian.feedNames\")"
                       ) << endl;
      }

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

        Cell* feed = DataBase::getDB()->getCell( feedName );
        if (not feed)
          feed = AllianceFramework::get()->getCell( feedName, Catalog::State::Views|Catalog::State::Foreign );

        if (feed)
          _feedCells.useFeed( feed );
        else
          cerr << Warning( "EtesianEngine::_postCreate() Unable to find \"%s\" feed cell."
                         , feedName.c_str()
                         ) << endl;
      }

      string tieName = getConfiguration()->getTieName();
      Cell*  tie     = DataBase::getDB()->getCell( tieName );
      if (not tie)
        tie = AllianceFramework::get()->getCell( tieName, Catalog::State::Views|Catalog::State::Foreign );
      if (tie)
        _feedCells.useTie( tie );
      else
        cerr << Warning( "EtesianEngine::_postCreate() Unable to find \"%s\" tie cell."
                       , tieName.c_str()
                       ) << endl;
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

    InstancesToIds emptyInstsToIds;
    _instsToIds.swap( emptyInstsToIds );

    vector<InstanceInfos> emptyIdsToInsts;
    _idsToInsts.swap( emptyIdsToInsts );

    _surface       = NULL;
    _circuit       = NULL;
    _placementLB   = NULL;
    _placementUB   = NULL;
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
        // TODO: block instances and multi-row cells are manageable in Coloquinte now
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
  //if (_viewer) _viewer->getCellWidget()->fitToContents();

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
    clearColoquinte();
    AllianceFramework* af          = AllianceFramework::get();
    DbU::Unit          hpitch      = getSliceStep();
    DbU::Unit          vpitch      = getSliceStep();
    DbU::Unit          sliceHeight = getSliceHeight();
    bool               isFlexLib   = (getGauge()->getName() == "FlexLib");

    CRL::Histogram  stdCellSizes ( 0.0, 1.0, 2 );
    stdCellSizes.setTitle ( "Width"  , 0 );
    stdCellSizes.setColor ( "green"  , 0 );
    stdCellSizes.setIndent( "       ", 0 );
    stdCellSizes.setTitle ( "Bloat"  , 1 );
    stdCellSizes.setColor ( "red"    , 1 );
    stdCellSizes.setIndent( "       ", 1 );

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
    DbU::Unit totalLength    = (_placeArea.getHeight()/sliceHeight) * _placeArea.getWidth();
    DbU::Unit usedLength     = 0;
    DbU::Unit registerLength = 0;

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
        Box    instanceAb = instance->getAbutmentBox();
        if (af->isRegister(masterName)) {
          ++registerNb;
          registerLength += instanceAb.getWidth();
        }
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          if (topAb.intersect(instanceAb)) {
            ++instancesNb;
            ++fixedNb;
            totalLength -= (instanceAb.getHeight()/sliceHeight) * instanceAb.getWidth();
          }
        }
        if (instance->getPlacementStatus() == Instance::PlacementStatus::PLACED) {
          cerr << "PLACED " << instance << endl;
        }
      }
    }

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) ) {
      ++instancesNb;
      Instance* instance   = static_cast<Instance*>(occurrence.getEntity());
      Box       instanceAb = instance->getAbutmentBox();
      string    masterName = getString( instance->getMasterCell()->getName() );
      DbU::Unit length = (instanceAb.getHeight()/sliceHeight) * instanceAb.getWidth();
      if (af->isRegister(masterName)) {
        ++registerNb;
        registerLength += length;
      }
      if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
        ++fixedNb;
        totalLength -= length;
      } else if (instance->getPlacementStatus() == Instance::PlacementStatus::PLACED) {
        cerr << "PLACED " << instance << endl;
      } else {
        usedLength += length;
      }
    }
    if (instancesNb <= fixedNb) {
      cerr << Error( "EtesianEngine::toColoquinte(): \"%s\" has no instance to place, doing nothing."
                   , getString(getCell()->getName()).c_str()
                   ) << endl;
      return 0;
    }

    cmess1 << ::Dots::asPercentage( "     - Effective space margin"
                                  , (float)(totalLength-usedLength)/(float)totalLength ) << endl;

    cmess1 << ::Dots::asUInt( "     - Number of instances ", instancesNb ) << endl;
    if (instancesNb) {
      float ratio = ((float)registerNb / (float)instancesNb) * 100.0;
      ostringstream os1;
      os1 << registerNb << " (" << fixed << setprecision(2) << ratio << "%)";
      cmess1 << ::Dots::asString    ( "     - Registers (DFF) ", os1.str() ) << endl;
      cmess1 << ::Dots::asPercentage( "     - Registers (DFF) area "
                                  , (float)(registerLength)/(float)totalLength ) << endl;
      ratio = ((float)_bufferCount / (float)instancesNb) * 100.0;
      ostringstream os2;
      os2 << _bufferCount << " (" << fixed << setprecision(2) << ratio << "%)";
      cmess1 << ::Dots::asString( "     - Buffers ", os2.str() ) << endl;
    }
    cout.flush();

  // Coloquinte circuit description data-structures.
  // One dummy fixed instance at the end

    _circuit = new coloquinte::Circuit( instancesNb+1 );
    vector<int> cellX( instancesNb+1 );
    vector<int> cellY( instancesNb+1 );
    vector<coloquinte::CellOrientation> orient( instancesNb+1 );
    vector<int> cellWidth( instancesNb+1 );
    vector<int> cellHeight( instancesNb+1 );
    vector<bool> cellIsFixed( instancesNb+1 );
    vector<bool> cellIsObstruction( instancesNb+1 );
    vector<coloquinte::CellRowPolarity> cellRowPolarity( instancesNb+1, coloquinte::CellRowPolarity::SAME );

    cmess1 << "     - Building RoutingPads (transhierarchical)" << endl;
  //getCell()->flattenNets( Cell::Flags::BuildRings|Cell::Flags::NoClockFlatten );
  //getCell()->flattenNets( getBlockInstance(), Cell::Flags::NoClockFlatten );
    getCell()->flattenNets( NULL, _excludedNets, Cell::Flags::NoClockFlatten );

    int instanceId       = 0;
    if (getBlockInstance()) {
      for ( Instance* instance : getCell()->getInstances() ) {
        if (instance == getBlockInstance()) continue;
        if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED) {
          Box overlapAb = topAb.getIntersection( instance->getAbutmentBox() );
          if (not overlapAb.isEmpty()) {
          // Upper rounded
            int xsize = (overlapAb.getWidth () + hpitch - 1) / hpitch;
            int ysize = (overlapAb.getHeight() + vpitch - 1) / vpitch;
          // Lower rounded
            int xpos  = overlapAb.getXMin() / hpitch;
            int ypos  = overlapAb.getYMin() / vpitch;

            cellX[instanceId] = xpos;
            cellY[instanceId] = ypos;
            cellWidth[instanceId] = xsize;
            cellHeight[instanceId] = ysize;
            cellIsFixed[instanceId] = true;
            cellIsObstruction[instanceId] = true;

            _instsToIds.insert( make_pair(instance,instanceId) );
            _idsToInsts.push_back( make_tuple(instance,vector<RoutingPad*>()) );
            ++instanceId;
            dots.dot();
          }
        }
      }
    }

    double spaceMargin = getSpaceMargin();
    double densityVariation = getDensityVariation();
    double bloatFactor = 1.0 +  std::max(spaceMargin - densityVariation, 0.0);
    if (bloatFactor != 1.0) {
      cmess1 << "     - Cells inflated by " << bloatFactor << endl;
    }

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      if (instanceId >= instancesNb) {
        // This will be an error
        ++instanceId;
        continue;
      }
      _checkNotAFeed(occurrence);

      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      stdCellSizes.addSample( (float)(masterCell->getAbutmentBox().getWidth() / hpitch), 0 );
      Box instanceAb = _bloatCells.getAb( occurrence );
      stdCellSizes.addSample( (float)(instanceAb.getWidth() / hpitch), 1 );

      Transformation instanceTransf = instance->getTransformation();
      occurrence.getPath().getTransformation().applyOn( instanceTransf );
      instanceTransf.applyOn( instanceAb );

      // Upper rounded
      int xsize = (instanceAb.getWidth () + hpitch - 1) / hpitch;
      int ysize = (instanceAb.getHeight() + vpitch - 1) / vpitch;
      // Lower rounded
      int xpos  = instanceAb.getXMin() / hpitch;
      int ypos  = instanceAb.getYMin() / vpitch;

      string masterName = getString( instance->getMasterCell()->getName() );
      if (isFlexLib and not instance->isFixed() and (masterName == "buf_x8"))
         ++xsize;

      // Take bloat into account to compute the size
      xsize *= bloatFactor;

      cellX[instanceId] = xpos;
      cellY[instanceId] = ypos;
      cellWidth[instanceId] = xsize;
      cellHeight[instanceId] = ysize;

      if ( not instance->isFixed() and instance->isTerminalNetlist() ) {
        cellIsFixed[instanceId] = false;
        cellIsObstruction[instanceId] = false;
      } else {
        cellIsFixed[instanceId] = true;
        cellIsObstruction[instanceId] = true;
      }

      _instsToIds.insert( make_pair(instance,instanceId) );
      _idsToInsts.push_back( make_tuple(instance,vector<RoutingPad*>()) );
      ++instanceId;
      dots.dot();
    }

    if (instanceId != instancesNb) {
      throw Error( "EtesianEngine::toColoquinte(): %d  leaf instances, but expected %d\n"
                   "        maybe a virtual flattening problem."
                 , instanceId, instancesNb 
                 );
    }

    // Dummy fixed instance at the end
    cellX[instanceId] = 0;
    cellY[instanceId] = 0;
    cellWidth[instanceId] = 0;
    cellHeight[instanceId] = 0;
    cellIsFixed[instanceId] = true;
    cellIsObstruction[instanceId] = true;

    dots.finish( Dots::Reset|Dots::FirstDot );

    size_t netsNb = 0;
    for ( Net* net : getCell()->getNets() )
    {
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER )   excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND)   excludedType = "GROUND";
      if (net->getType() == Net::Type::CLOCK )   excludedType = "CLOCK";
      if (isExcluded(getString(net->getName()))) excludedType = "USER_EXCLUDED";
      if (excludedType) {
        cparanoid << Warning( "%s is not a routable net (%s,excluded)."
                            , getString(net).c_str(), excludedType ) << endl;
        continue;
      }
      if (af->isBLOCKAGE(net->getName())) continue;

      ++netsNb;
    }
    _circuit->setCellX(cellX);
    _circuit->setCellY(cellY);
    _circuit->setCellOrientation(orient);
    _circuit->setCellWidth(cellWidth);
    _circuit->setCellHeight(cellHeight);
    _circuit->setCellIsFixed(cellIsFixed);
    _circuit->setCellIsObstruction(cellIsObstruction);
    _circuit->setCellRowPolarity(cellRowPolarity);

    cmess1 << "     - Converting " << netsNb << " nets" << endl;

    for ( Net* net : getCell()->getNets() )
    {
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER )   excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND)   excludedType = "GROUND";
      if (net->getType() == Net::Type::CLOCK )   excludedType = "CLOCK";
      if (isExcluded(getString(net->getName()))) excludedType = "USER_EXCLUDED";
      if (excludedType) continue;
      if (af->isBLOCKAGE(net->getName())) continue;

      dots.dot();
      
      string topCellInstancePin = getString(getCell()->getName()) + ":C";
      vector<int> netCells, pinX, pinY;

      for ( RoutingPad* rp : net->getRoutingPads() ) {
        Path path = rp->getOccurrence().getPath();
        Pin* pin  = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() ); 
        if (pin) {
          if (path.isEmpty()) {
            Point pt   = rp->getCenter();
            int xpin = pt.getX() / hpitch;
            int ypin = pt.getY() / vpitch;
          // Dummy last instance
            pinX.push_back(xpin);
            pinY.push_back(ypin);
            netCells.push_back(instanceId);
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
            continue;
          }
        }

        Instance* instance = extractInstance    ( rp );
        Point     offset   = extractRpOffset    ( rp );

        int xpin    = offset.getX() / hpitch;
        int ypin    = offset.getY() / vpitch;

        auto  iid = _instsToIds.find( instance );
        if (iid == _instsToIds.end()) {
          if (not instance) {
            string    insName  = extractInstanceName( rp );
            cerr << Error( "Unable to lookup instance \"%s\".", insName.c_str() ) << endl;
          }
        } else {
          pinX.push_back(xpin);
          pinY.push_back(ypin);
          netCells.push_back((*iid).second);
        }
      }
      _circuit->addNet(netCells, pinX, pinY);
    }
    dots.finish( Dots::Reset );

    cmess1 << "     - Standard cells widths:" << endl;
    cmess2 << stdCellSizes.toString(0) << endl;
    if (_bloatCells.getSelected()->getName() != "disabled")
      cmess2 << stdCellSizes.toString(1) << endl;

    _surface = new coloquinte::Rectangle( (int)(topAb.getXMin() / hpitch)
                            , (int)(topAb.getXMax() / hpitch)
                            , (int)(topAb.getYMin() / vpitch)
                            , (int)(topAb.getYMax() / vpitch)
                            );
    _circuit->setupRows(*_surface, (getSliceHeight() + vpitch - 1) / vpitch);
    _circuit->check();
    _placementLB = new coloquinte::PlacementSolution ();
    _placementUB = new coloquinte::PlacementSolution ( *_placementLB );

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

  void EtesianEngine::_coloquinteCallback(coloquinte::PlacementStep step) {
    auto placement = _circuit->solution();
    if (step == coloquinte::PlacementStep::LowerBound) {
      *_placementLB = placement;
    }
    else {
      *_placementUB = placement;
    }

    // Graphical update
    GraphicUpdate conf = getUpdateConf();
    if (conf == GraphicUpdate::UpdateAll) {
      _updatePlacement(&placement);
    }
    else if (conf == GraphicUpdate::LowerBound &&
             step == coloquinte::PlacementStep::LowerBound) {
      _updatePlacement(&placement);
    }
  }

  void  EtesianEngine::globalPlace ()
  {
    coloquinte::ColoquinteParameters params(getPlaceEffort());
    coloquinte::PlacementCallback callback =std::bind(&EtesianEngine::_coloquinteCallback, this, std::placeholders::_1);
    _circuit->placeGlobal(params, callback);
    *_placementUB = _circuit->solution();
    _updatePlacement(_placementUB);
  }


  void  EtesianEngine::detailedPlace ()
  {
    coloquinte::ColoquinteParameters params(getPlaceEffort());
    coloquinte::PlacementCallback callback =std::bind(&EtesianEngine::_coloquinteCallback, this, std::placeholders::_1);
    _circuit->placeDetailed(params, callback);
    *_placementUB = _circuit->solution();
    *_placementLB = *_placementUB; // In case we run other passes
    _updatePlacement(_placementUB);
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

    cmess1 << "  o  Running Coloquinte." << endl;
    startMeasures();

    cmess1 << _circuit->report() << std::endl;

    cmess1 << "  o  Global placement." << endl;
    globalPlace();

    cmess1 << "  o  Detailed Placement." << endl;
    detailedPlace();

  //toHurricane();
  //addFeeds();

    cmess1 << "  o  Placement finished." << endl;
    stopMeasures();
    printMeasures();
    addMeasure<double>( "placeT", getTimer().getCombTime() );

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

  void  EtesianEngine::_updatePlacement ( const coloquinte::PlacementSolution* placement, uint32_t flags )
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
        auto place = (*placement)[(*iid).second];
        Transformation cellTrans  = toTransformation( place.position
                                                    , place.orientation
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

  void EtesianEngine::_checkNotAFeed( Occurrence occurrence ) const {
    Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
    Cell*     masterCell   = instance->getMasterCell();
    if (CatalogExtension::isFeed(masterCell)) {
      string feedName = getString( instance->getName() );
      if (  (feedName.substr(0,11) != "spare_feed_")
          or (not instance->isFixed())) {
        string    instanceName = occurrence.getCompactString();
        // Remove the enclosing brackets...
        instanceName.erase( 0, 1 );
        instanceName.erase( instanceName.size()-1 );
        throw Error( "EtesianEngine::toColoquinte(): Feed instance \"%s\" found."
                    , instanceName.c_str() );
      }
    }
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

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

// Options for both placers
  unsigned const SteinerModel        = 0x0001;

// Options for the global placer
  unsigned const ForceUniformDensity = 0x0010;
  unsigned const UpdateLB            = 0x0020;
  unsigned const UpdateUB            = 0x0040;

// Options for the detailed placer
  unsigned const UpdateDetailed      = 0x0100;
//unsigned const NonConvexOpt        = 0x0200;


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
    , _surface      ()
    , _circuit      ()
    , _placementLB  ()
    , _placementUB  ()
    , _cellsToIds   ()
    , _idsToInsts   ()
    , _viewer       (NULL)
    , _feedCells    (this)
    , _bloatCells   (this)
    , _yspinSlice0  (0)
    , _sliceHeight  (0)
    , _fixedAbHeight(0)
    , _fixedAbWidth (0)
  {
  }


  void  EtesianEngine::_postCreate ()
  {
    Super::_postCreate();

  // Ugly: Name based detection of ISPD benchmarks.
  // Maybe base it on whether the imported library is in Bookshelf format or not?
    if (getString(getCell()->getName()).substr(0,7) == "bigblue") {
      cmess2 << "  o  ISPD benchmark <" << getCell()->getName()
             << ">, no feed cells will be added." << endl;
    } else {
      _bloatCells.select( getConfiguration()->getBloat() );
      
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
    cdebug_log(129,1) << "EtesianEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << getCell()->getName() << ">" << endl;

    Super::_preDestroy();

    cdebug.log(129,-1);
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


  void  EtesianEngine::setDefaultAb ()
  {
    _bloatCells.resetDxSpace();
    double spaceMargin = getSpaceMargin();
    double aspectRatio = getAspectRatio();
    size_t instanceNb  = 0;
    double cellLength  = 0;

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

      cmess2 << "     - Using as block: " << occurrence.getCompactString() << "." << endl;
      cellLength += DbU::toLambda( _bloatCells.getAb( occurrence ).getWidth() );
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

    double bloatLength = DbU::toLambda( _bloatCells.getDxSpace() );
    double bloatMargin = ( cellLength / (cellLength - bloatLength) ) - 1.0;
    
    double gcellLength = cellLength*(1.0+spaceMargin) / DbU::toLambda( getSliceHeight() );

    if (gcellLength == 0.0) {
      throw Error( "EtesianEngine::setDefaultAb(): Null g-length for \"%s\" (are you using the right gauge?)."
                 , getString(getCell()->getName()).c_str()
                 );
    } 

    double rows    = 0.0;
    double columns = 0.0;
    if (not getFixedAbWidth()) {
      if (getFixedAbHeight()) rows = getFixedAbHeight() / getSliceHeight();
      else                    rows = std::ceil( sqrt( gcellLength/aspectRatio ) );
      columns = std::ceil( gcellLength / rows );
    } else {
      columns = getFixedAbWidth() / getSliceHeight();
      rows    = std::ceil( gcellLength / columns );
    }

    UpdateSession::open();
    for ( auto occurrence : feedOccurrences ) {
      static_cast<Instance*>(occurrence.getEntity())->destroy();
    }

    DbU::Unit abWidth = columns*getSliceHeight();
    DbU::Unit adjust  = abWidth % getSliceStep();
    if (adjust) abWidth += getSliceStep() - adjust;

    getCell()->setAbutmentBox( Box( DbU::fromLambda(0)
                                  , DbU::fromLambda(0)
                                  , abWidth
                                  , rows*getSliceHeight()
                                  ) );
    UpdateSession::close();
    if (_viewer) _viewer->getCellWidget()->fitToContents();

    cmess1 << "  o  Creating abutment box (margin:" << (spaceMargin*100.0)
           << "% aspect ratio:" << (aspectRatio*100.0)
           << "% g-length:" << (cellLength/DbU::toLambda(getSliceHeight()))
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
    UpdateSession::close();

    dots.finish( Dots::Reset );

    if (_viewer) _viewer->getCellWidget()->refresh();
  }


  size_t  EtesianEngine::toColoquinte ()
  {
    AllianceFramework* af     = AllianceFramework::get();
    DbU::Unit          hpitch = getHorizontalPitch();
    DbU::Unit          vpitch = getSliceStep();

    cmess1 << "  o  Converting <" << getCell()->getName() << "> into Coloquinte." << endl;
    cmess1 << ::Dots::asString("     - H-pitch"    , DbU::getValueString(hpitch)) << endl;
    cmess1 << ::Dots::asString("     - V-pitch"    , DbU::getValueString(vpitch)) << endl;

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

    Dots  dots ( cmess2, "       ", 80, 1000 );
    if (not cmess2.enabled()) dots.disable();
    
    Box topAb = getBlockCell()->getAbutmentBox();
    Transformation topTransformation;
    if (getBlockInstance()) topTransformation = getBlockInstance()->getTransformation();
    topTransformation.applyOn( topAb );

    size_t  instancesNb = 0;
    size_t  fixedNb     = 0;
    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) ) {
      ++instancesNb;
      Instance* instance = static_cast<Instance*>(occurrence.getEntity());
      if (instance->getPlacementStatus() == Instance::PlacementStatus::FIXED)
        ++fixedNb;
    }
    if (instancesNb <= fixedNb) {
      cerr << Error( "EtesianEngine::toColoquinte(): \"%s\" has no instance to place, doing nothing."
                   , getString(getCell()->getName()).c_str()
                   ) << endl;
      return 0;
    }

  // Coloquinte circuit description data-structures.
  // One dummy fixed instance at the end
    vector<Transformation>  idsToTransf ( instancesNb+1 );
    vector<temporary_cell>  instances   ( instancesNb+1 );
    vector< point<int_t> >  positions   ( instancesNb+1 );
    vector< point<bool> >   orientations( instancesNb+1, point<bool>(true, true) );

    cmess1 << "     - Converting " << instancesNb << " instances" << endl;
    cout.flush();

    cmess1 << "     - Building RoutingPads (transhierarchical) ..." << endl;
  //getCell()->flattenNets( Cell::Flags::BuildRings|Cell::Flags::NoClockFlatten );
    getCell()->flattenNets( getBlockInstance(), Cell::Flags::NoClockFlatten );

    bool    tooManyInstances = false;
    index_t instanceId       = 0;
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
      int_t xsize = (instanceAb.getWidth () + vpitch -1) / vpitch;
      int_t ysize = (instanceAb.getHeight() + hpitch -1) / hpitch;
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
      }

      _cellsToIds.insert( make_pair(instanceName,instanceId) );
      _idsToInsts.push_back( instance );
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

      nets[netId] = temporary_net( netId, 1 );

    //cerr << "+ " << net << endl;

      for ( Pin* pin : net->getPins() ) {
      // For Gabriel Gouvine : the position of this pin should be added as a fixed
      // attractor in Coloquinte. May be outside the placement area.
        Point pt = pin->getPosition();
        topTransformation.applyOn(pt);
        int_t xpin = pt.getX() / vpitch;
        int_t ypin = pt.getY() / hpitch;
      // Dummy last instance
        pins.push_back( temporary_pin( point<int_t>(xpin,ypin), instanceId, netId ) );
      //cerr << "Outside Pin: " << pin << endl;
      }
      
      string topCellInstancePin = getString(getCell()->getName()) + ":C";

      for ( RoutingPad* rp : net->getRoutingPads() ) {
        if (getBlockInstance() and (rp->getOccurrence().getPath().getHeadInstance() != getBlockInstance())) {
        // For Gabriel Gouvine : if there are multiple blocks (i.e. we have a true
        // floorplan, there may be RoutingPad that are elsewhere. We should check
        // that the RP is placed or is inside a define area (the abutment box of
        // it's own block). No example yet of that case, though.
          cerr << Warning("Net %s has a routing pad that is not rooted at the placed instance.", getString(net).c_str()) << endl;
        //cerr << "Outside RP: " << rp << endl;
          continue;
        }

        string insName = extractInstanceName( rp );
        Point  offset  = extractRpOffset    ( rp );

        int_t xpin    = offset.getX() / vpitch;
        int_t ypin    = offset.getY() / hpitch;

        auto  iid = _cellsToIds.find( insName );
        if (iid == _cellsToIds.end()) {
          if (insName != topCellInstancePin) {
            cerr << Error( "Unable to lookup instance \"%s\".", insName.c_str() ) << endl;
          }
        } else {
          pins.push_back( temporary_pin( point<int_t>(xpin,ypin), (*iid).second, netId ) );
        }

      //cerr << "| " << rp << " pos:(" << xpin << " " << ypin << ")" << endl;
      }

      netId++;
    }
    dots.finish( Dots::Reset );

    _surface = box<int_t>( (int_t)(topAb.getXMin() / vpitch)
                         , (int_t)(topAb.getXMax() / vpitch)
                         , (int_t)(topAb.getYMin() / hpitch)
                         , (int_t)(topAb.getYMax() / hpitch)
                         );
    _circuit = netlist( instances, nets, pins );
    _circuit.selfcheck();
    _placementLB.positions_    = positions;
    _placementLB.orientations_ = orientations;
    _placementUB = _placementLB;

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
    auto first_legalizer = region_distribution::uniform_density_distribution(_surface, _circuit, _placementLB);
    first_legalizer.selfcheck();
    get_rough_legalization( _circuit, _placementUB, first_legalizer);

    _placementLB = _placementUB;

    // Early topology-independent solution with a star model + negligible pulling forces to avoid dumb solutions
    // Spreads well to help subsequent optimization passes
    cmess2 << "  o  Star (*) Optimization." << endl;
    auto solv = get_star_linear_system( _circuit, _placementLB, 1.0, 0, 10) // Limit the number of pins: don't want big awful nets with high weight
              + get_pulling_forces( _circuit, _placementUB, 1000000.0);
    solve_linear_system( _circuit, _placementLB, solv, 200 );
    _progressReport2("     [---]" );
  }


  void  EtesianEngine::roughLegalize ( float minDisruption, unsigned options )
  {
    using namespace coloquinte::gp;
    // Create a legalizer and bipartition it until we have sufficient precision
    auto legalizer = (options & ForceUniformDensity) != 0 ?
        region_distribution::uniform_density_distribution (_surface, _circuit, _placementLB, _densityLimits)
      : region_distribution::full_density_distribution    (_surface, _circuit, _placementLB, _densityLimits);
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
    _placementUB = _placementLB;
    // Update UB
    get_rough_legalization( _circuit, _placementUB, legalizer );
  }


  void  EtesianEngine::globalPlace ( float initPenalty
                                   , float minDisruption
                                   , float targetImprovement
                                   , float minInc
                                   , float maxInc
                                   , unsigned options )
  {
    using namespace coloquinte::gp;

    float_t penaltyIncrease = minInc;
    float_t linearDisruption  = get_mean_linear_disruption(_circuit, _placementLB, _placementUB);
    float_t pullingForce = initPenalty;
    float_t upperWL = static_cast<float_t>(get_HPWL_wirelength(_circuit, _placementUB)),
            lowerWL = static_cast<float_t>(get_HPWL_wirelength(_circuit, _placementLB));
    float_t prevOptRatio = lowerWL / upperWL;

    index_t i=0;
    do{
      roughLegalize(minDisruption, options);
      if(options & UpdateUB)
        _updatePlacement( _placementUB );

      ostringstream label;
      label.str("");
      label  << "     [" << setw(3) << setfill('0') << i << setfill(' ') << "] Bipart.";
      _progressReport1(label.str() );

      upperWL = static_cast<float_t>(get_HPWL_wirelength(_circuit, _placementUB));
    //float_t legRatio = lowerWL / upperWL;

      // Get the system to optimize (tolerance, maximum and minimum pin counts)
      // and the pulling forces (threshold distance)
      auto opt_problem = (options & SteinerModel) ?
        get_RSMT_linear_system  ( _circuit, _placementLB, minDisruption, 2, 100000 ) 
      : get_HPWLF_linear_system ( _circuit, _placementLB, minDisruption, 2, 100000 ); 
      auto solv = opt_problem
                + get_linear_pulling_forces( _circuit, _placementUB, _placementLB, pullingForce, 2.0f * linearDisruption);
      solve_linear_system( _circuit, _placementLB, solv, 200 ); // 200 iterations
      _progressReport2("           Linear." );

      if(options & UpdateLB)
        _updatePlacement( _placementLB );

      // Optimize orientation sometimes
      if (i%5 == 0) {
          optimize_exact_orientations( _circuit, _placementLB );
          _progressReport2("           Orient." );
      }

      lowerWL = static_cast<float_t>(get_HPWL_wirelength(_circuit, _placementLB));
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
      cparanoid << "                  L/U ratio: " << 100*optRatio << "% (previous: " << 100*prevOptRatio << "%)\n"
                << "                  Pulling force: " <<  pullingForce << " Increase: " << penaltyIncrease << endl;

      pullingForce += penaltyIncrease;
      prevOptRatio = optRatio;

      linearDisruption  = get_mean_linear_disruption(_circuit, _placementLB, _placementUB);
      ++i;
      // First way to exit the loop: UB and LB difference is <10%
      // Second way to exit the loop: the legalization is close enough to the previous result
    } while (linearDisruption > minDisruption and prevOptRatio <= 0.9);
    _updatePlacement( _placementUB );
  }


  void  EtesianEngine::detailedPlace ( int iterations, int effort, unsigned options )
  {
    using namespace coloquinte::gp;
    using namespace coloquinte::dp;

    int_t sliceHeight = getSliceHeight() / getHorizontalPitch();
    roughLegalize(sliceHeight, options);
    // TODO: for uniform density distribution, add some margin to the cell sizes so we don't disrupt it during detailed placement

    for ( int i=0; i<iterations; ++i ){
        ostringstream label;
        label.str("");
        label  << "     [" << setw(3) << setfill('0') << i << setfill(' ') << "]";

        optimize_x_orientations( _circuit, _placementUB ); // Don't disrupt VDD/VSS connections in a row
        _progressReport1(label.str() + " Oriented ......." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        auto legalizer = legalize( _circuit, _placementUB, _surface, sliceHeight );
        coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
        _progressReport1("           Legalized ......" );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        row_compatible_orientation( _circuit, legalizer, true );
        swaps_global_HPWL( _circuit, legalizer, 3, 4 );
        coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
        _progressReport1("           Global Swaps ..." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if(options & SteinerModel)
          OSRP_noncvx_RSMT( _circuit, legalizer );
        else
          OSRP_convex_HPWL( _circuit, legalizer );
        coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
        _progressReport1("           Row Optimization" );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if(options & SteinerModel)
          swaps_row_noncvx_RSMT( _circuit, legalizer, effort+2 );
        else
          swaps_row_convex_HPWL( _circuit, legalizer, effort+2 );
        coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
        _progressReport1("           Local Swaps ...." );
        if(options & UpdateDetailed)
          _updatePlacement( _placementUB );

        if (i == iterations-1) {
          //swaps_row_convex_RSMT( _circuit, legalizer, 4 );
          row_compatible_orientation( _circuit, legalizer, true );
          coloquinte::dp::get_result( _circuit, legalizer, _placementUB );
          verify_placement_legality( _circuit, _placementUB, _surface );
          _progressReport1("           Final Legalize ." );
        }
    }
    _placementLB = _placementUB; // In case we run other passes
    _updatePlacement( _placementUB );
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

    findYSpin();
    if (not toColoquinte()) return;

    Effort        placementEffort = getPlaceEffort();
    GraphicUpdate placementUpdate = getUpdateConf();
    Density       densityConf     = getSpreadingConf();
    double        sliceHeight     = getSliceHeight() / getHorizontalPitch();

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

    cmess2 << "  o  Adding feed cells." << endl;
    addFeeds();

    cmess1 << "  o  Placement finished." << endl;
    stopMeasures();
    printMeasures();
    cmess1 << ::Dots::asString
      ( "     - HPWL", DbU::getValueString( (DbU::Unit)coloquinte::gp::get_HPWL_wirelength(_circuit,_placementUB )*getSliceStep() ) ) << endl;
    cmess1 << ::Dots::asString
      ( "     - RMST", DbU::getValueString( (DbU::Unit)coloquinte::gp::get_RSMT_wirelength(_circuit,_placementUB )*getSliceStep() ) ) << endl;

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
           << " HPWL:" << setw(11) << coloquinte::gp::get_HPWL_wirelength( _circuit, _placementUB )
           << " RMST:" << setw(11) << coloquinte::gp::get_RSMT_wirelength( _circuit, _placementUB )
           << endl;
    cparanoid << indent
           <<   " L-Dsrpt:" << setw(8) << coloquinte::gp::get_mean_linear_disruption   ( _circuit, _placementLB, _placementUB )
           <<   " Q-Dsrpt:" << setw(8) << coloquinte::gp::get_mean_quadratic_disruption( _circuit, _placementLB, _placementUB )
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
           << " HPWL:" << setw(11) << coloquinte::gp::get_HPWL_wirelength( _circuit, _placementLB )
           << " RMST:" << setw(11) << coloquinte::gp::get_RSMT_wirelength( _circuit, _placementLB )
           << endl;
  }


  void  EtesianEngine::_updatePlacement ( const coloquinte::placement_t& placement )
  {
    UpdateSession::open();

    Transformation topTransformation;
    if (getBlockInstance()) topTransformation = getBlockInstance()->getTransformation();
    topTransformation.invert();

    for ( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences(getBlockInstance()) )
    {
      DbU::Unit hpitch          = getHorizontalPitch();
      DbU::Unit vpitch          = getSliceStep();
      Point     instancePosition;
      Instance* instance        = static_cast<Instance*>(occurrence.getEntity());
      string    instanceName    = occurrence.getCompactString();
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
                                                  , hpitch
                                                  , vpitch
                                                  );
      //cerr << "Setting <" << instanceName << " @" << instancePosition << endl;

      // This is temporary as it's not trans-hierarchic: we ignore the positions
      // of all the intermediary instances.
        topTransformation.applyOn( trans );
        instance->setTransformation( trans );
        instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
      }
    }

    UpdateSession::close();

    if (_viewer) _viewer->getCellWidget()->refresh();
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
    }
    return record;
  }


} // Etesian namespace.

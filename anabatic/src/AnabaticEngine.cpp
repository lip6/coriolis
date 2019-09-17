// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AnabaticEngine.cpp"                          |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/Measures.h"
#include "crlcore/Histogram.h"
#include "anabatic/GCell.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/NetBuilderM2.h"
#include "anabatic/NetBuilderHV.h"
#include "anabatic/NetBuilderVH.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace Hurricane;
  using namespace Anabatic;


  class SortAcByXY {
    public:
      inline bool  operator() ( AutoContactTerminal* contact1, AutoContactTerminal* contact2 );
  };


  inline bool  SortAcByXY::operator() ( AutoContactTerminal* contact1, AutoContactTerminal* contact2 )
  {
    DbU::Unit x1 = contact1->getX();
    DbU::Unit x2 = contact2->getX();

    if (x1 == x2) {
      DbU::Unit y1 = contact1->getY();
      DbU::Unit y2 = contact2->getY();

      if (y1 == y2) return false;
      return (y1 < y2);
    }
    return (x1 < x2);
  }


  void  shear ( AutoSegment* segment )
  {
    AutoContact* source    = segment->getAutoSource();
    AutoContact* target    = segment->getAutoTarget();
    bool         useSource = true;

    if (segment->isHorizontal()) {
      if (not source->isTurn() and target->isTurn()) useSource = false;
    } else {
      if (not source->isTurn() and target->isTurn()) useSource = false;
    }

    segment->makeDogleg( (useSource ? source->getGCell() : target->getGCell()) );
    Session::getDoglegs()[ Session::getDoglegs().size()-2 ]->unsetFlags( AutoSegment::SegAxisSet );

#if BAD_RESULTS
    AutoContact* source        = segment->getAutoSource();
    AutoContact* target        = segment->getAutoTarget();
    AutoSegment* perpandicular = NULL;
    bool         useSource     = true;

    if (segment->isHorizontal()) {
      if (source->isTurn()) perpandicular = source->getPerpandicular( segment );
      else {
        if (target->isTurn()) { perpandicular = target->getPerpandicular( segment ); useSource = false; }
        else {
          if (source->isHTee()) perpandicular = source->getPerpandicular( segment );
          else {
            if (target->isHTee()) { perpandicular = target->getPerpandicular( segment ); useSource = false; }
            else
              perpandicular = segment;
          }
        }
      }
    } else {
      if (source->isTurn()) perpandicular = source->getPerpandicular( segment );
      else {
        if (target->isTurn()) { perpandicular = target->getPerpandicular( segment ); useSource = false; }
        else {
          if (source->isVTee()) perpandicular = source->getPerpandicular( segment );
          else {
            if (target->isVTee()) { perpandicular = target->getPerpandicular( segment ); useSource = false; }
            else
              perpandicular = segment;
          }
        }
      }
    }

    perpandicular->makeDogleg( (useSource ? source->getGCell() : target->getGCell()) );
#endif
  }


}  // Anonymous namespace.


namespace Anabatic {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::multiset;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::RegularLayer;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Cell;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;
  using CRL::addMeasure;
  using CRL::getMeasure;
  using CRL::Histogram;


// -------------------------------------------------------------------
// Error messages.

  const char* missingAnbt =
    "%s :\n\n"
    "    Cell %s do not have any Anabatic (or not yet created).\n";

  const char* badMethod =
    "%s :\n\n"
    "    No method id %ud (Cell %s).\n";

  const char* lookupFailed =
    "Anabatic::Extension::getDatas(Segment*) :\n\n"
    "    Cannot find AutoSegment associated to %s (internal error).\n";


// -------------------------------------------------------------------
// Class  :  "Anabatic::RawGCellsUnder".

  RawGCellsUnder::RawGCellsUnder ( const AnabaticEngine* engine, Segment* segment )
  {
    cdebug_log(112,1) << "RawGCellsUnder::RawGCellsUnder(Segment*): " << segment << endl;

    commonCtor( engine, segment->getSourcePosition(), segment->getTargetPosition() );

    cdebug_tabw(112,-1);
  }


  RawGCellsUnder::RawGCellsUnder ( const AnabaticEngine* engine, Point source, Point target )
  {
    cdebug_log(112,1) << "RawGCellsUnder::RawGCellsUnder(Point,Point): s:"
                      << source << " t:" << target << endl;

    commonCtor( engine, source, target );

    cdebug_tabw(112,-1);
  }


  void  RawGCellsUnder::commonCtor ( const AnabaticEngine* engine, Point source, Point target )
  {
    cdebug_log(112,1) << "RawGCellsUnder::commontCtor(): s:" << source << " t:" << target << endl;

    Box        gcellsArea = engine->getCell()->getAbutmentBox();
    DbU::Unit  axis       = 0;
    Flags      side       = Flags::NoFlags;

    if (source.getY() == target.getY()) {
      side = Flags::EastSide;
      axis = source.getY();
      if (source.getX() > target.getX()) std::swap( source, target );
    }
    if (source.getX() == target.getX()) {
      side = Flags::NorthSide;
      axis = source.getX();
      if (source.getY() > target.getY()) std::swap( source, target );
    }

    if (side == Flags::NoFlags) {
      cerr << Error( "RawGCellsUnder::commonCtor(): Points are neither horizontally nor vertically aligneds (ignored)."
                   ) << endl;
      cdebug_tabw(112,-1);
      DebugSession::close();
      return;
    }

    if (  (source.getX() >  gcellsArea.getXMax())
       or (source.getY() >  gcellsArea.getYMax())
       or (target.getX() <= gcellsArea.getXMin())
       or (target.getY() <= gcellsArea.getYMin()) ) {
      cerr << Error( "RawGCellsUnder::commonCtor(): Area is completly outside the GCells area (ignored)."
                   ) << endl;
      cdebug_tabw(112,-1);
      DebugSession::close();
      return;
    }

    DbU::Unit xsource = std::max( source.getX(), gcellsArea.getXMin() );
    DbU::Unit ysource = std::max( source.getY(), gcellsArea.getYMin() );
    DbU::Unit xtarget = std::min( target.getX(), gcellsArea.getXMax() );
    DbU::Unit ytarget = std::min( target.getY(), gcellsArea.getYMax() );

    if (xtarget == gcellsArea.getXMax()) --xtarget;
    if (ytarget == gcellsArea.getYMax()) --ytarget;

    GCell* gsource = engine->getGCellUnder( xsource, ysource );
    GCell* gtarget = engine->getGCellUnder( xtarget, ytarget );

    if (not gsource) {
      cerr << Bug( "RawGCellsUnder::RawGCellsUnder(): Source not under a GCell (ignored)."
                 ) << endl;
      cdebug_tabw(112,-1);
      DebugSession::close();
      return;
    }
    if (not gtarget) {
      cerr << Bug( "RawGCellsUnder::RawGCellsUnder(): Target not under a GCell (ignored)."
                 ) << endl;
      cdebug_tabw(112,-1);
      DebugSession::close();
      return;
    }

    if (gsource == gtarget) {
      _elements.push_back( Element(gsource,NULL) );
      cdebug_tabw(112,-1);
      DebugSession::close();
      return;
    }

    cdebug_log(112,0) << "flags:" << side << " axis:" << DbU::getValueString(axis) << endl;

    Edge* edge = gsource->getEdgeAt( side, axis );
    while ( edge ) {
      _elements.push_back( Element(edge->getSource(),edge) );
      cdebug_log(112,0) << "| push:" << edge->getSource() << " from " << edge << endl;

      if (edge->getTarget() == gtarget) break;
      edge = edge->getTarget()->getEdgeAt( side, axis );
    } 
    _elements.push_back( Element(gtarget,NULL) );
    cdebug_log(112,0) << "| push:" << gtarget << " last/target" << endl;

    cdebug_tabw(112,-1);
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::NetData".

  NetData::NetData ( Net* net )
    : _net       (net)
    , _state     (NetRoutingExtension::get(net))
    , _searchArea()
    , _rpCount   (0)
    , _sparsity  (0)
    , _flags     ()
  {
    if (_state and _state->isMixedPreRoute()) return;

    for ( RoutingPad* rp : _net->getRoutingPads() ) {
      _searchArea.merge( rp->getBoundingBox() );
      ++_rpCount;
    }
    _update();
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AnabaticEngine".

  Name  AnabaticEngine::_toolName = "Anabatic";


  AnabaticEngine* AnabaticEngine::get ( const Cell* cell )
  { return static_cast<AnabaticEngine*>(ToolEngine::get(cell,staticGetName())); }


  const Name& AnabaticEngine::staticGetName ()
  { return _toolName; }


  const Name& AnabaticEngine::getName () const
  { return _toolName; }


  AnabaticEngine::AnabaticEngine ( Cell* cell )
    : Super(cell)
    , _configuration    (new Configuration())
    , _chipTools        (cell)
    , _state            (EngineCreation)
    , _matrix           ()
    , _gcells           ()
    , _ovEdges          ()
    , _netOrdering      ()
    , _netDatas         ()
    , _viewer           (NULL)
    , _flags            (Flags::DestroyBaseContact)
    , _stamp            (-1)
    , _densityMode      (MaxDensity)
    , _autoSegmentLut   ()
    , _autoContactLut   ()
    , _edgeCapacitiesLut()
    , _blockageNet      (cell->getNet("blockagenet"))
  {
    _matrix.setCell( cell, _configuration->getSliceHeight() );
    Edge::unity = _configuration->getSliceHeight();

    if (not _blockageNet) _blockageNet = Net::create( cell, "blockagenet" );
  }


  void  AnabaticEngine::_postCreate ()
  {
    Super::_postCreate();

    UpdateSession::open();
    GCell::create( this );
    UpdateSession::close();
  }


  AnabaticEngine* AnabaticEngine::create ( Cell* cell )
  {
    if (not cell) throw Error( "AnabaticEngine::create(): NULL cell argument." );
    if (cell->getAbutmentBox().isEmpty())
      throw Error( "AnabaticEngine::create(): %s has no abutment box." , getString(cell).c_str() );
    
    AnabaticEngine* engine = new AnabaticEngine ( cell );
    engine->_postCreate();
    return engine;
  }


  AnabaticEngine::~AnabaticEngine ()
  {
    delete _configuration;
    for ( pair<unsigned int,NetData*> data : _netDatas ) delete data.second;
  }


  void  AnabaticEngine::_preDestroy ()
  {
    cdebug_log(145,1) << "Anabatic::_preDestroy ()" << endl;

    if (getState() < EngineGutted)
      setState( EnginePreDestroying );

    _gutAnabatic();
    _state = EngineGutted;

    cdebug_log(145,0) << "About to delete base class ToolEngine." << endl;
    Super::_preDestroy();

  //cmess2 << "     - GCells        := " << GCell::getAllocateds() << endl;
    cmess2 << "     - AutoContacts  := " << AutoContact::getAllocateds() << endl;
    cmess2 << "     - AutoSegments  := " << AutoSegment::getAllocateds() << endl;

    cdebug_log(145,0) << "Exiting Anabatic::_preDestroy()." << endl;
    cdebug_tabw(145,-1);
  }


  void  AnabaticEngine::_gutAnabatic ()
  {
    openSession();

    _flags.reset( Flags::DestroyBaseContact|Flags::DestroyBaseSegment );

    if (_state == EngineDriving) {
      cdebug_log(145,1) << "Saving AutoContacts/AutoSegments." << endl;

      size_t fixedSegments    = 0;
      size_t sameLayerDoglegs = 0;
      for ( auto isegment : _autoSegmentLut ) {
        if (isegment.second->isFixed()) ++fixedSegments;
        if (isegment.second->reduceDoglegLayer()) ++sameLayerDoglegs;
      }

      cmess1 << "  o  Driving Hurricane data-base." << endl;
      cmess1 << Dots::asSizet("     - Active AutoSegments",AutoSegment::getAllocateds()-fixedSegments) << endl;
      cmess1 << Dots::asSizet("     - Active AutoContacts",AutoContact::getAllocateds()-fixedSegments*2) << endl;
      cmess1 << Dots::asSizet("     - AutoSegments"       ,AutoSegment::getAllocateds()) << endl;
      cmess1 << Dots::asSizet("     - AutoContacts"       ,AutoContact::getAllocateds()) << endl;
      cmess1 << Dots::asSizet("     - Same Layer doglegs" ,sameLayerDoglegs) << endl;

    //for ( Net* net : _cell->getNets() ) _saveNet( net );

      cdebug_tabw(145,-1);
    }

    if (_state < EngineGutted ) {
      cdebug_log(145,0) << "Gutting Anabatic." << endl;
      _state = EngineGutted;
      _flags |= Flags::DestroyBaseContact;

      _destroyAutoSegments();
      _destroyAutoContacts();

      _flags |= Flags::DestroyGCell;

      for ( GCell* gcell : _gcells ) gcell->_destroyEdges();
      for ( GCell* gcell : _gcells ) gcell->destroy();
      _gcells.clear();
      _ovEdges.clear();
    }

    Session::close();
  }


  Configuration* AnabaticEngine::getConfiguration ()
  { return _configuration; }


  Interval  AnabaticEngine::getUSide ( Flags direction ) const
  {
    Interval side;
    Box      bBox ( getCell()->getBoundingBox() );

    if      (direction & Flags::Horizontal) side = Interval( bBox.getXMin(), bBox.getXMax() );
    else if (direction & Flags::Vertical  ) side = Interval( bBox.getYMin(), bBox.getYMax() );
    else {
      cerr << Error( "AnabaticEngine::getUSide(): Unknown direction flag \"%i\""
                   , getString(direction).c_str() ) << endl;
    }
    return side;
  }


  int  AnabaticEngine::getCapacity ( Interval span, Flags flags ) const
  {
    int           capacity = 0;
    Box           ab       = getCell()->getAbutmentBox();
    RoutingGauge* rg       = _configuration->getRoutingGauge();

    span.inflate( 0, -1 );
    if (span.isEmpty()) return 0;

    const vector<RoutingLayerGauge*>& layerGauges = rg->getLayerGauges();
    for ( size_t depth=0 ; depth <= _configuration->getAllowedDepth() ; ++depth ) {
      if (layerGauges[depth]->getType() != Constant::Default) continue;

      if (flags & Flags::Horizontal) {
        if (layerGauges[depth]->getDirection() != Constant::Horizontal) continue;
        capacity += layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getYMin()
                                                      , span.getVMax() - ab.getYMin() );
      //cdebug_log(110,0) << "Horizontal edge capacity:" << capacity << endl;
      }

      if (flags & Flags::Vertical) {
        if (layerGauges[depth]->getDirection() != Constant::Vertical) continue;
        capacity += layerGauges[depth]->getTrackNumber( span.getVMin() - ab.getXMin()
                                                      , span.getVMax() - ab.getXMin() );
      //cdebug_log(110,0) << "Vertical edge capacity:" << capacity << endl;
      }
    }

    return capacity;
  }


  void  AnabaticEngine::openSession ()
  { Session::_open(this); }


  void  AnabaticEngine::reset ()
  {
    _gutAnabatic();
    _flags.reset( Flags::DestroyMask );
    _state = EngineCreation;

    UpdateSession::open();
    GCell::create( this );
    UpdateSession::close();
  }


  void  AnabaticEngine::setupNetDatas ()
  {
    Histogram  netHistogram ( 0.0, 1.0, 1 );
    netHistogram.setTitle ( "RoutingPads", 0 );
    netHistogram.setColor ( "green"      , 0 );
    netHistogram.setIndent( "       "    , 0 );
    
    size_t  oindex = _netOrdering.size();
    for ( Net* net : _cell->getNets() ) {
      if (_netDatas.find(net->getId()) != _netDatas.end()) continue;
      NetData* data = new NetData( net );
      _netOrdering.push_back( data );

      netHistogram.addSample( (float)data->getRpCount(), 0 );
    }

    for ( ; oindex < _netOrdering.size() ; ++oindex ) {
      _netDatas.insert( make_pair( _netOrdering[oindex]->getNet()->getId()
                                 , _netOrdering[oindex] ) );
    }

    sort( _netOrdering.begin(), _netOrdering.end(), SparsityOrder() );

    cmess2 << "  o  Nets Histogram." << endl;
    cmess2 << netHistogram.toString(0) << endl;
  }


  void  AnabaticEngine::exclude ( const Name& netName )
  {
    Net* net = _cell->getNet( netName );
    if (not net) return;

    exclude( net );
  }


  void  AnabaticEngine::exclude ( Net* net )
  {
    NetDatas::iterator inet =_netDatas.find( net->getId() );
    if (inet == _netDatas.end()) return;

    (*inet).second->setExcluded( true );
  }


  void AnabaticEngine::updateMatrix()
  {
    _matrix.setCell( getCell(), Session::getSliceHeight() );
    for ( GCell* gcell : _gcells ) _updateLookup( gcell );
  }


  size_t  AnabaticEngine::getNetsFromEdge ( const Edge* edge, NetSet& nets )
  {
    size_t  count  = 0;
    GCell*  source = edge->getSource();
    GCell*  target = edge->getTarget();
    const vector<Contact*>& contacts = source->getGContacts();

    for ( Contact* contact : contacts ) {
      for ( Component* component : contact->getSlaveComponents() ) {
        if (edge->isHorizontal()) {
          Horizontal* horizontal = dynamic_cast<Horizontal*>( component );
          if (horizontal
             and (horizontal->getSource() == contact)
             and (target->hasGContact(dynamic_cast<Contact*>(horizontal->getTarget())))) {
            nets.insert( horizontal->getNet() );
            ++count;
          }
        }
        if (edge->isVertical()) {
          Vertical* vertical = dynamic_cast<Vertical*>( component );
          if (vertical
             and (vertical->getSource() == contact)
             and (target->hasGContact(dynamic_cast<Contact*>(vertical->getTarget())))) {
            nets.insert( vertical->getNet() );
            ++count;
          }
        }
      }
    }
    return count;
  }


  NetData* AnabaticEngine::getNetData ( Net* net, Flags flags )
  {
    NetData*            data = NULL;
    NetDatas::iterator idata = _netDatas.find( net->getId() );
    if (idata == _netDatas.end()) {
      data = new NetData( net );
      _netDatas.insert( make_pair(net->getId(),data) );
      _netOrdering.push_back( data );
      // cerr << Bug( "AnabaticEngine::getNetData() - %s is missing in NetDatas table."
      //            , getString(net->getName()).c_str()
      //            ) << endl;
      // return NULL;
    } else
      data = idata->second;

    if ((flags & Flags::Create) and not data->getNetRoutingState()) {
      data->setNetRoutingState( NetRoutingExtension::create(net) );
    }

    return data;
  }


  Contact* AnabaticEngine::breakAt ( Segment* segment, GCell* breakGCell )
  {
    size_t       i      = 0;
    GCellsUnder  gcells ( new RawGCellsUnder(this,segment) );
    for ( ; i<gcells->size() ; ++i ) {
      if (gcells->gcellAt(i) == breakGCell) break;
    }

    Contact* breakContact = breakGCell->getGContact( segment->getNet() );

    if (i == gcells->size()) {
      cerr << Error( "AnabaticEngine::breakAt(): %s is *not* over %s."
                   , getString(segment).c_str()
                   , getString(breakGCell).c_str()
                   ) << endl;
      return breakContact;
    }

    Component* targetContact = segment->getTarget();
    segment->getTargetHook()->detach();
    segment->getTargetHook()->attach( breakContact->getBodyHook() );

    Segment*    splitted   = NULL;
    Horizontal* horizontal = dynamic_cast<Horizontal*>(segment);
    if (horizontal) {
      splitted = Horizontal::create( breakContact
                                   , targetContact
                                   , getConfiguration()->getGHorizontalLayer()
                                   , horizontal->getY()
                                   , DbU::fromLambda(2.0)
                                   );
    } else {
      Vertical* vertical = dynamic_cast<Vertical*>(segment);
      if (vertical) {
        splitted = Vertical::create( breakContact
                                   , targetContact
                                   , getConfiguration()->getGVerticalLayer()
                                   , vertical->getX()
                                   , DbU::fromLambda(2.0)
                                   );
      } else
        return breakContact;
    }

    for ( ; i<gcells->size()-1 ; ++i ) gcells->edgeAt(i)->replace( segment, splitted );

    return breakContact;
  }


  bool  AnabaticEngine::unify ( Contact* contact )
  {
    size_t      hCount     = 0;
    size_t      vCount     = 0;
    Horizontal* horizontals[2];
    Vertical*   verticals  [2];

    cdebug_log(112,1) << "AnabaticEngine::unify(): " << (void*)contact << endl;
    cdebug_log(112,0) << contact << endl;

    for ( Hook* hook : contact->getBodyHook()->getHooks() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( hook->getComponent() );
      if (rp) {
        cdebug_log(112,0) << "Cannot unify, there is a RoutingPad in this GCell." << endl;
        cdebug_tabw(112,-1);
        return false;
      }
    }

    for ( Component* slave : contact->getSlaveComponents() ) {
      Horizontal* h = dynamic_cast<Horizontal*>( slave );
      if (h) {
        if (vCount or (hCount > 1)) return false;
        horizontals[hCount++] = h;
      } else {
        Vertical* v = dynamic_cast<Vertical*>( slave );
        if (v) {
          if (hCount or (vCount > 1)) return false;
          verticals[vCount++] = v;
        } else {
        // Something else depends on this contact.
          cdebug_log(112,0) << "Cannot unify, still have slave components." << endl;
          cdebug_tabw(112,-1);
          return false;
        }
      }
    }

    if (hCount == 2) {
      if (horizontals[0]->getTarget() != contact) std::swap( horizontals[0], horizontals[1] );
      Interval    constraints ( false );
      GCellsUnder gcells0     = getGCellsUnder( horizontals[0] );
      if (not gcells0->empty()) {
        for ( size_t i=0 ; i<gcells0->size() ; ++i )
          constraints.intersection( gcells0->gcellAt(i)->getSide(Flags::Vertical) );
      }

      GCellsUnder gcells1 = getGCellsUnder( horizontals[1] );
      if (not gcells1->empty()) {
        for ( size_t i=0 ; i<gcells1->size() ; ++i ) {
          constraints.intersection( gcells1->gcellAt(i)->getSide(Flags::Vertical) );
          if (constraints.isEmpty()) {
            cdebug_tabw(112,-1);
            return false;
          }
        }
      }

      if (not gcells1->empty()) {
        for ( size_t i=0 ; i<gcells1->size()-1 ; ++i )
          gcells1->edgeAt(i)->replace( horizontals[1], horizontals[0] );
      }

      Component* target = horizontals[1]->getTarget();
      horizontals[1]->destroy();
      horizontals[0]->getTargetHook()->detach();
      horizontals[0]->getTargetHook()->attach( target->getBodyHook() );
      horizontals[0]->setY( constraints.getCenter() );
    } 

    if (vCount == 2) {
      cdebug_log(112,0) << "Vertical unify of: " << contact << endl;
      
      if (verticals[0]->getTarget() != contact) std::swap( verticals[0], verticals[1] );
      Interval    constraints ( false );
      GCellsUnder gcells0     = getGCellsUnder( verticals[0] );
      if (not gcells0->empty()) {
        for ( size_t i=0 ; i<gcells0->size() ; ++i )
          constraints.intersection( gcells0->gcellAt(i)->getSide(Flags::Horizontal) );
      }

      GCellsUnder gcells1 = getGCellsUnder( verticals[1] );
      if (not gcells1->empty()) {
        for ( size_t i=0 ; i<gcells1->size() ; ++i ) {
          constraints.intersection( gcells1->gcellAt(i)->getSide(Flags::Horizontal) );
          if (constraints.isEmpty()) {
            cdebug_log(112,0) << "Cannot unify, shearing constraints." << endl;
            cdebug_tabw(112,-1);
            return false;
          }
        }
      }

      if (not gcells1->empty()) {
        for ( size_t i=0 ; i<gcells1->size()-1 ; ++i )
          gcells1->edgeAt(i)->replace( verticals[1], verticals[0] );
      }

      Component* target = verticals[1]->getTarget();
      verticals[1]->destroy();
      verticals[0]->getTargetHook()->detach();
      verticals[0]->getTargetHook()->attach( target->getBodyHook() );
      verticals[0]->setX( constraints.getCenter() );
    } 

    getGCellUnder( contact->getPosition() )->unrefContact( contact );

    cdebug_tabw(112,-1);
    return true;
  }


  void  AnabaticEngine::ripup ( Segment* seed, Flags flags )
  {
    Net* net = seed->getNet();

    DebugSession::open( net, 112, 120 );
    cdebug_log(112,1) << "AnabaticEngine::ripup(): " << seed << endl;

    Contact* end0 = NULL;
    Contact* end1 = NULL;

    set<Segment*,Entity::CompareById> ripups;
    ripups.insert( seed );

    vector< pair<Segment*,Component*> > stack;
    if (flags & Flags::Propagate) {
      stack.push_back( make_pair(seed,seed->getSource()) );
      stack.push_back( make_pair(seed,seed->getTarget()) );
    }

    while ( not stack.empty() ) {
      Contact* contact = dynamic_cast<Contact*>( stack.back().second );
      Segment* from    = stack.back().first;
      stack.pop_back();
      if (not contact) continue;

      Segment* connected  = NULL;
      size_t   slaveCount = 0;
      for ( Hook* hook : contact->getBodyHook()->getHooks() ) {
        Component* linked = hook->getComponent();
        if ((linked == contact) or (linked == from)) continue;

        if (dynamic_cast<RoutingPad*>(linked)) { ++slaveCount; continue; }

        connected = dynamic_cast<Segment*>( linked );
        if (connected) ++slaveCount; 
      }

      if ((slaveCount == 1) and (connected)) {
        if (not ripups.count(connected)) {
          stack .push_back( make_pair(connected,connected->getOppositeAnchor(contact)) );
          ripups.insert( connected );
        }
      } else {
        if (not end0) {
          end0 = contact;
          cdebug_log(112,0) << "end0:" << contact << endl;
        } else {
          if (contact != end0) {
            end1 = contact;
            cdebug_log(112,0) << "end1:" << contact << endl;
          }
        }
      }
    }

    for ( Segment* segment : ripups ) {
      cdebug_log(112,1) << "| Destroy:" << segment << endl;

      GCellsUnder gcells = getGCellsUnder( segment );
      if (not gcells->empty()) {
        for ( size_t i=0 ; i<gcells->size()-1 ; ++i ) {
          cdebug_log(112,0) << "| " << gcells->gcellAt(i) << endl;
          cdebug_log(112,0) << "| " << gcells->edgeAt(i) << endl;
          gcells->edgeAt(i)->remove( segment );
        }
      } else {
        cdebug_log(112,0) << "No GCells under segment." << endl;
      }

      Contact* source = dynamic_cast<Contact*>( segment->getSource() );
      Contact* target = dynamic_cast<Contact*>( segment->getTarget() );
      segment->getSourceHook()->detach();
      segment->getTargetHook()->detach();
      segment->destroy();
      bool deletedSource = gcells->gcellAt( 0                )->unrefContact( source );
      bool deletedTarget = gcells->gcellAt( gcells->size()-1 )->unrefContact( target );

      if (deletedSource) {
        if (source == end0) end0 = NULL;
        if (source == end1) end1 = NULL;
      }
      if (deletedTarget) {
        if (target == end0) end0 = NULL;
        if (target == end1) end1 = NULL;
      }

      cdebug_tabw(112,-1);
    }

    if (end0) unify( end0 );
    if (end1) unify( end1 );

    getNetData( net )->setGlobalRouted( false );

    cdebug_tabw(112,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::cleanupGlobal ()
  {
    UpdateSession::open();
    for ( GCell* gcell : _gcells ) gcell->cleanupGlobal();
    UpdateSession::close();
  }


  void  AnabaticEngine::loadGlobalRouting ( uint32_t method )
  {
    if (_state < EngineGlobalLoaded)
      throw Error ("AnabaticEngine::loadGlobalRouting() : global routing not present yet.");

    if (_state > EngineGlobalLoaded)
      throw Error ("AnabaticEngine::loadGlobalRouting() : global routing already loaded.");

    if (method == EngineLoadGrByNet ) { _loadGrByNet(); }
    else {
      throw Error( badMethod, "Anabatic::loadGlobalRouting()", method, getString(_cell).c_str() );
    }
    cleanupGlobal();

    if (not getConfiguration()->isTwoMetals()) relaxOverConstraineds();

    _state = EngineActive;
  }


  void  AnabaticEngine::relaxOverConstraineds ()
  {
    openSession();

    DbU::Unit           pitch3 = Session::getPitch( 2 );
    AutoSegment::IdSet  constraineds;
    AutoSegment::IdSet  processeds;
    
    for ( GCell* gcell : _gcells ) {
      multiset<AutoContactTerminal*,SortAcByXY>  acTerminals;
      for ( AutoContact* contact : gcell->getContacts() ) {
        if (contact->isTerminal() and (Session::getViaDepth(contact->getLayer()) == 0) )
          acTerminals.insert( dynamic_cast<AutoContactTerminal*>(contact) );
      }

      AutoContactTerminal* south = NULL;
      for ( AutoContactTerminal* north : acTerminals ) {
        if (south) {
          if (   south->canDrag()
             and north->canDrag()
             and (south->getNet() != north->getNet())
             and (south->getX  () == north->getX  ()) ) {
            Interval constraints ( gcell->getYMin(), north->getCBYMin() /*- pitch3*/ );
            AutoSegment* terminal = south->getSegment();
            AutoContact* opposite = terminal->getOppositeAnchor( south );

            for ( AutoSegment* segment : AutoSegments_OnContact(terminal,opposite->base()) ) {
              segment->mergeUserConstraints( constraints );
              constraineds.insert( segment );
            }

            constraints = Interval( south->getCBYMax() /*+ pitch3*/, gcell->getYMax() );
            terminal    = north->getSegment();
            opposite    = terminal->getOppositeAnchor( north );

            for ( AutoSegment* segment : AutoSegments_OnContact(terminal,opposite->base()) ) {
              segment->mergeUserConstraints( constraints );
              constraineds.insert( segment );
            }
          }
        }
        south = north;
      }
    }

    Session::revalidate();

    for ( AutoSegment* constrained : constraineds ) {
      if (processeds.find(constrained) != processeds.end()) continue;
      processeds.insert( constrained );
      
      Interval              userConstraints ( false );
      vector<AutoSegment*>  aligneds;

      aligneds.push_back( constrained );

      for ( AutoSegment* aligned : constrained->getAligneds() ) {
        aligneds.push_back( aligned );
        processeds.insert( aligned );
      }

      sort( aligneds.begin(), aligneds.end(), AutoSegment::CompareBySourceU() );

    //AutoSegment* previous = NULL;
      for ( AutoSegment* aligned : aligneds ) {
        Interval constraints = userConstraints.getIntersection( aligned->getUserConstraints() );

        if (constraints.getSize() < Session::getPitch(1)) {
        //if (not previous) {
        //  cerr << Warning( "protectAlignedAccesses(): Shearing constraints between S/T on\n"
        //                   "          %s\n"
        //                   "          S:%s\n"
        //                   "          T:%s\n"
        //                   "          Combined user constraints are too tight [%s : %s]."
        //                 , getString(aligned ).c_str()
        //                 , getString(aligned->getAutoSource()->getConstraintBox()).c_str()
        //                 , getString(aligned->getAutoTarget()->getConstraintBox()).c_str()
        //                 , DbU::getValueString(constraints.getVMin()).c_str()
        //                 , DbU::getValueString(constraints.getVMax()).c_str()
        //                 ) << endl;
        //} else {
        //  cerr << Warning( "protectAlignedAccesses(): Shearing constraints between\n"
        //                   "          %s\n"
        //                   "          %s\n"
        //                   "          Combined user constraints are too tight [%s : %s]."
        //                 , getString(previous).c_str()
        //                 , getString(aligned ).c_str()
        //                 , DbU::getValueString(constraints.getVMin()).c_str()
        //                 , DbU::getValueString(constraints.getVMax()).c_str()
        //                 ) << endl;
        //}
        //if (previous) {
        //  if (previous->getAutoTarget() == aligned->getAutoSource()) {
        //    cerr << "Found a shared contact: " << aligned->getAutoSource() << endl;
              shear( aligned );
        //  }
        //}
          
          userConstraints = aligned->getUserConstraints();
        } else {
          userConstraints = constraints;
        }

      //previous = aligned;
      }
    }

    Session::close();
  }


  void  AnabaticEngine::_loadGrByNet ()
  {
    cmess1 << "  o  Building detailed routing from global. " << endl;

    size_t shortNets = 0;

    startMeasures();
    openSession();

    int gaugeKind = 3;
    if (getConfiguration()->isTwoMetals()) gaugeKind = 0;
    if (getConfiguration()->isHV       ()) gaugeKind = 1;
    if (getConfiguration()->isVH       ()) gaugeKind = 2;

    if (gaugeKind < 3) {
      for ( Net* net : getCell()->getNets() ) {
        if (NetRoutingExtension::isShortNet(net)) {
        //AutoSegment::setShortNetMode( true );
          ++shortNets;
        }
        if (NetRoutingExtension::isAutomaticGlobalRoute(net)) {
          DebugSession::open( net, 145, 150 );
          AutoSegment::setAnalogMode( NetRoutingExtension::isAnalog(net) );

          switch ( gaugeKind ) {
            case 0: NetBuilder::load<NetBuilderM2>( this, net ); break;
            case 1: NetBuilder::load<NetBuilderHV>( this, net ); break;
            case 2: NetBuilder::load<NetBuilderVH>( this, net ); break;
          }

          Session::revalidate();
          DebugSession::close();
        }
        AutoSegment::setAnalogMode  ( false );
        AutoSegment::setShortNetMode( false );
      }
    }

#if defined(CHECK_DATABASE)
    _check ( "after Anabatic loading" );
#endif

    Session::close();
    stopMeasures();

    cmess2 << Dots::asSizet("     - Short nets",shortNets) << endl;

    if (gaugeKind > 2) {
      throw Error( "AnabaticEngine::_loadGrByNet(): Unsupported kind of routing gauge \"%s\"."
                 , getString(getConfiguration()->getRoutingGauge()->getName()).c_str() );
    }
    
    printMeasures( "load" );

    addMeasure<size_t>( getCell(), "Globals", AutoSegment::getGlobalsCount() );
    addMeasure<size_t>( getCell(), "Edges"  , AutoSegment::getAllocateds() );
  }


  void  AnabaticEngine::updateNetTopology ( Net* net )
  {
    DebugSession::open( net, 145, 150 );

    cdebug_log(149,0) << "Anabatic::updateNetTopology( " << net << " )" << endl;
    cdebug_tabw(145,1);

    vector<AutoContact*>  contacts;
    for ( Component* component : net->getComponents() ) {
      Contact* contact = dynamic_cast<Contact*>( component );
      if (contact) {
        AutoContact* autoContact = Session::lookup( contact );
        if (autoContact and autoContact->isInvalidatedCache()) 
          contacts.push_back( autoContact );
      }
    }

    for ( size_t i=0 ; i<contacts.size() ; ++i )
      contacts[i]->updateTopology();

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::finalizeLayout ()
  {
    cdebug_log(145,0) << "Anabatic::finalizeLayout()" << endl;
    if (_state > EngineDriving) return;

    _state = EngineDriving;

    startMeasures();
    _gutAnabatic();
    stopMeasures ();
    printMeasures( "fin" );

    _state = EngineGutted;
  }


  void  AnabaticEngine::_alignate ( Net* net )
  {
    DebugSession::open( net, 145, 150 );

    cdebug_log(149,0) << "Anabatic::_alignate( " << net << " )" << endl;
    cdebug_tabw(145,1);

  //cmess2 << "     - " << getString(net) << endl;

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> unexploreds;
    vector<AutoSegment*> aligneds;

    for ( Component* component : net->getComponents() ) {
      Segment* segment = dynamic_cast<Segment*>(component);
      if (segment) {
        AutoSegment* seedSegment = Session::lookup( segment );
        if (seedSegment) unexploreds.push_back( seedSegment );
      }
    }
    sort( unexploreds.begin(), unexploreds.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<unexploreds.size() ; i++ ) {
      AutoSegment* seedSegment = unexploreds[i];

      if (exploredSegments.find(seedSegment->base()) == exploredSegments.end()) {
        cdebug_log(145,0) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back( seedSegment );

        for ( AutoSegment* collapsed : seedSegment->getAligneds() ) {
          cdebug_log(145,0) << "Aligned: " << collapsed << endl;
          aligneds.push_back( collapsed );
          exploredSegments.insert( collapsed->base() );
        }

        cdebug_tabw(145,1);
        sort( aligneds.begin(), aligneds.end(), AutoSegment::CompareId() );

        cdebug_log(145,0) << "Seed: " << (void*)aligneds[0]->base() << " " << aligneds[0] << endl;
        for ( size_t j=1 ; j<aligneds.size() ; j++ ) {
          cdebug_log(145,0) << "Secondary: " << (void*)(aligneds[j]->base()) << " " << aligneds[j] << endl;
        }

        cdebug_log(149,0) << "Align on " << aligneds[0]
                    << " " << DbU::getValueString(aligneds[0]->getAxis()) << endl;
        aligneds[0]->setAxis( aligneds[0]->getAxis(), Flags::Realignate );
        aligneds.clear();

        cdebug_tabw(145,-1);
      }
    }

    cdebug_tabw(145,-1);

    DebugSession::close();
  }


  void  AnabaticEngine::_computeNetTerminals ( Net* net )
  {
    DebugSession::open( net, 145, 150 );

    cdebug_log(149,0) << "Anabatic::_computeNetTerminals( " << net << " )" << endl;
    cdebug_tabw(145,1);

    for ( Segment* segment : net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( segment );
      if (autoSegment == NULL) continue;
      if (autoSegment->isInvalidated()) autoSegment->computeTerminal();
    }

    cdebug_tabw(145,-1);

    DebugSession::close();
  }


  void  AnabaticEngine::_saveNet ( Net* net )
  {
    DebugSession::open( net, 145, 150 );

    cdebug_log(145,0) << "Anabatic::_saveNet() " << net << endl;
    cdebug_tabw(145,1);

#if 0
    cdebug_log(145,0) << "Deleting zero-length segments." << endl;

    vector<Segment*>   nullSegments;
    set<const Layer*>  connectedLayers;

    forEach ( Segment*, segment, net->getSegments() ) {
      if (segment->getLength()) {
        if (net->isExternal()) {
          NetExternalComponents::setExternal( *segment );
        }
        continue;
      }
    
      if (Session::lookup(*segment) == NULL) {
        cdebug_log(145,0) << "* Not associated to an AutoSegment: " << *segment << endl;
        continue;
      }

      if (not isTopAndBottomConnected(*segment,connectedLayers)) {
        nullSegments.push_back( *segment );
        cdebug_log(145,0) << "* Null Length: " << *segment << endl;
      }
    }
    
    setFlags( EngineDestroyBaseSegment );
    for ( size_t i = 0 ; i < nullSegments.size() ; i++ ) {
      Contact* source = dynamic_cast<Contact*>(nullSegments[i]->getSource());
      Contact* target = dynamic_cast<Contact*>(nullSegments[i]->getTarget());

      if ( (source == NULL) or (target == NULL) ) {
        cerr << Error("Unconnected source/target on %s.",getString(nullSegments[i]).c_str()) << endl;
        continue;
      }

      if (source->getAnchor()) {
        if (target->getAnchor()) {
          continue;
        //cerr << Bug("Both source & target are anchored while deleting zero-length segment:\n"
        //            "       %s.",getString(nullSegments[i]).c_str()) << endl;
        } else
          swap( source, target );
      }

      cdebug_log(145,0) << "Deleting: " << nullSegments[i] << endl;
      if (isTopAndBottomConnected(nullSegments[i],connectedLayers)) {
        cdebug_log(145,0) << "Deletion cancelled, no longer top or bottom connected." << endl;
        continue;
      }

      cdebug_log(145,0) << "* Source: " << (void*)source << " " << source << endl;
      cdebug_log(145,0) << "* Target: " << (void*)target << " " << target << endl;

      const Layer* layer = DataBase::getDB()->getTechnology()
        ->getViaBetween( *connectedLayers.begin(), *connectedLayers.rbegin() );

      cdebug_log(145,0) << *connectedLayers.begin() << " + " << *connectedLayers.rbegin() << endl;
      cdebug_log(145,0) << "* Shrink layer: " << layer << endl;
      if ( !layer ) {
        cerr << Error("NULL contact layer while deleting %s."
                     ,getString(nullSegments[i]).c_str()) << endl;
        continue;
      }

      Session::lookup( nullSegments[i] )->destroy ();

      vector<Hook*>  slaveHooks;
      Hook*          masterHook = source->getBodyHook()->getPreviousMasterHook();

      while ( masterHook->getNextHook() != source->getBodyHook() ) {
        slaveHooks.push_back( masterHook->getNextHook() );
        cdebug_log(145,0) << "* detach: "
                   << (void*)masterHook->getNextHook()->getComponent()
                   << " " << masterHook->getNextHook()->getComponent() << endl;
        masterHook->getNextHook()->detach();
      }
      source->destroy();

      masterHook = target->getBodyHook();
      for ( size_t j=0 ; j < slaveHooks.size() ; j++ ) {
        slaveHooks[j]->attach( masterHook );
      }

      cdebug_log(145,0) << (void*)target << " " << target << " setLayer: " << layer << endl;
      target->setLayer( layer );
    }
    unsetFlags( EngineDestroyBaseSegment );
#endif

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::invalidateRoutingPads ()
  {
  // This a flaw in the Hurricane Session update mechanism
  // and needs to be fixed in the future.
    for ( Net* net : getCell()->getNets() ) {
      for ( RoutingPad* rp : net->getRoutingPads() ) {
        rp->invalidate( false );
      }
    }
  }


  void  AnabaticEngine::updateDensity ()
  { for ( GCell* gcell : _gcells ) gcell->updateDensity(); } 


  size_t  AnabaticEngine::checkGCellDensities ()
  {
    size_t saturateds = 0;
    for ( GCell* gcell : _gcells ) saturateds += gcell->checkDensity();
    return saturateds;
  } 


  AutoSegment* AnabaticEngine::_lookup ( Segment* segment ) const
  {
    AutoSegmentLut::const_iterator it = _autoSegmentLut.find( segment );
    if (it == _autoSegmentLut.end()) return NULL;

    return (*it).second;
  }


  void  AnabaticEngine::_link ( AutoSegment* autoSegment )
  {
    if (_state > EngineActive) return;
    _autoSegmentLut[ autoSegment->base() ] = autoSegment;
  }


  void  AnabaticEngine::_unlink ( AutoSegment* autoSegment )
  {
    if (_state > EngineDriving) return;

    AutoSegmentLut::iterator it = _autoSegmentLut.find( autoSegment->base() );
    if (it != _autoSegmentLut.end())
      _autoSegmentLut.erase( it );
  }


  AutoContact* AnabaticEngine::_lookup ( Contact* contact ) const
  {
    AutoContactLut::const_iterator it = _autoContactLut.find( contact );
    if (it == _autoContactLut.end()) {
      return NULL;
    }
    return (*it).second;
  }


  void  AnabaticEngine::_link ( AutoContact* autoContact )
  {
    if (_state > EngineActive) return;
    _autoContactLut [ autoContact->base() ] = autoContact;
  }


  void  AnabaticEngine::_unlink ( AutoContact* autoContact )
  {
    if ( _state > EngineActive ) return;

    AutoContactLut::iterator it = _autoContactLut.find( autoContact->base() );
    if (it != _autoContactLut.end())
      _autoContactLut.erase( it );
  }


  void  AnabaticEngine::_destroyAutoSegments ()
  {
    cdebug_log(145,0) << "Anabatic::_destroyAutoSegments ()" << endl;

    size_t expandeds = 0;
    for ( auto sasp : _autoSegmentLut ) {
      expandeds++;
      sasp.second->destroy();
    }
    if (_state == EngineDriving)
      cmess2 << "     - Expandeds     := " << expandeds << endl;

    _autoSegmentLut.clear();
  }


  void  AnabaticEngine::_destroyAutoContacts ()
  {
    cdebug_log(145,0) << "Anabatic::_destroyAutoContacts ()" << endl;

    for ( auto cacp : _autoContactLut ) cacp.second->destroy();
    _autoContactLut.clear();
  }


  EdgeCapacity* AnabaticEngine::_createCapacity ( Flags flags, Interval span )
  {
    size_t        depth        = getConfiguration()->getAllowedDepth();
    EdgeCapacity* edgeCapacity = NULL;

    flags &= Flags::EdgeCapacityMask;
    EdgeCapacity  key ( this, flags, span, depth );

    auto icap = _edgeCapacitiesLut.find( &key );
    if (icap != _edgeCapacitiesLut.end()) {
      edgeCapacity = *icap;
    } else {
      edgeCapacity = new EdgeCapacity ( this, flags, span, depth );
      _edgeCapacitiesLut.insert( edgeCapacity );
    }

    edgeCapacity->incref();
    return edgeCapacity;
  }

  
  size_t  AnabaticEngine::_unrefCapacity ( EdgeCapacity* capacity )
  {
    if (capacity->getref() < 2) _edgeCapacitiesLut.erase( capacity );
    return capacity->decref();
  }
  

  void  AnabaticEngine::computeEdgeCapacities ( int maxHCap, int maxVCap, int termSatThreshold, int maxTermSat )
  {
          vector<RoutingPad*> rps;
          vector<GCell*>      saturateds;
    const vector<NetData*>&   netDatas = getNetOrdering();

    for ( NetData* netData : netDatas ) {
      for ( Component* component : netData->getNet()->getComponents() ) {
        RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
        if (rp) rps.push_back( rp ); 
      }
    }

    UpdateSession::open();
    
    for ( auto rp : rps ) {
      if (not getConfiguration()->selectRpComponent(rp))
        cerr << Warning( "AnabaticEngine::computeEdgeCapacities(): %s has no components on grid.", getString(rp).c_str() ) << endl;

      Point  center = rp->getBoundingBox().getCenter();
      GCell* gcell  = getGCellUnder( center );
      
      if (not gcell) {
        cerr << Error( "AnabaticEngine::computeEdgeCapacities(): %s\n"
                       "        @%s of %s is not under any GCell.\n"
                       "        It will be ignored so the edge capacity estimate may be wrong."
                     , getString(rp).c_str()
                     , getString(center).c_str()
                     , getString(rp->getNet()).c_str()
                     ) << endl;
        continue;
      }

      gcell->incRpCount( 1 );
      if (gcell->getRpCount() == termSatThreshold) saturateds.push_back( gcell );
    }

    for ( GCell* gcell : getGCells() ) {
      if (not gcell->isMatrix()) continue;

      for ( Edge* edge : gcell->getEdges(Flags::EastSide|Flags::NorthSide) ) {
        GCell* opposite    = edge->getOpposite( gcell );
        int    maxReserved = maxHCap;
        int    reserved    = std::max( gcell->getRpCount(), opposite->getRpCount() );

        if (edge->isVertical()) maxReserved = maxVCap;
        edge->reserveCapacity( std::min( maxReserved, reserved ) );
      }
    }

    for ( GCell* gcell : saturateds ) {
      GCell* neighbor = gcell;
      for ( size_t i=0 ; i<2; ++i ) {
        Edge* edge = neighbor->getWestEdge();
        if (not edge) break;

        if (edge->getReservedCapacity() < maxTermSat)
          edge->reserveCapacity( maxTermSat - edge->getReservedCapacity() );
        neighbor = neighbor->getWest();
      }
      neighbor = gcell;
      for ( size_t i=0 ; i<2; ++i ) {
        Edge* edge = neighbor->getEastEdge();
        if (not edge) break;

        if (edge->getReservedCapacity() < maxTermSat)
          edge->reserveCapacity( maxTermSat - edge->getReservedCapacity() );
        neighbor = neighbor->getEast();
      }
    }

    UpdateSession::close();

  //Breakpoint::stop( 1, "Edge capacities computeds." );
  }
  

  void  AnabaticEngine::_check ( Net* net ) const
  {
    cdebug_log(149,1) << "Checking " << net << endl;
    for ( Segment* segment : net->getComponents().getSubSet<Segment*>() ) {
      AutoSegment* autoSegment = _lookup( segment );
      cdebug_log(149,0) << autoSegment << endl;
      if (autoSegment) {
        AutoContact* autoContact = autoSegment->getAutoSource();
        cdebug_log(149,0) << autoContact << endl;
        if (autoContact) autoContact->checkTopology();

        autoContact = autoSegment->getAutoTarget();
        cdebug_log(149,0) << autoContact << endl;
        if (autoContact) autoContact->checkTopology();
      }
    }
    cdebug_tabw(149,-1);
  }


  bool  AnabaticEngine::_check ( const char* message ) const
  {
    bool coherency = true;
    if (message)
      cerr << "     o  checking Anabatic DB (" << message << ")." << endl;

    for ( auto element : _autoSegmentLut )
      coherency = element.second->_check() and coherency;

    for ( GCell* gcell : _gcells ) {
      for ( AutoContact* contact : gcell->getContacts() )
        contact->checkTopology();
    }

    if (message) cerr << "        - completed." << endl;

    return coherency;
  }


  void  AnabaticEngine::printMeasures ( const string& tag ) const
  {
    Super::printMeasures();

    // if (not tag.empty()) {
    //   addMeasure<double>( getCell(), tag+"T",  getTimer().getCombTime  () );
    //   addMeasure<size_t>( getCell(), tag+"S", (getTimer().getMemorySize() >> 20) );
    // }
  }


  string  AnabaticEngine::_getTypeName () const
  { return getString(_toolName); }


  string  AnabaticEngine::_getString () const
  {
    ostringstream os;
    os << "<" << _toolName << " " << _cell->getName() << ">";
    return os.str();
  }


  Record* AnabaticEngine::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_configuration"    ,  _configuration     ) );
    record->add( getSlot("_gcells"           , &_gcells            ) );
    record->add( getSlot("_matrix"           , &_matrix            ) );
    record->add( getSlot("_flags"            , &_flags             ) );
    record->add( getSlot("_autoSegmentLut"   , &_autoSegmentLut    ) );
    record->add( getSlot("_autoContactLut"   , &_autoContactLut    ) );
    record->add( getSlot("_edgeCapacitiesLut", &_edgeCapacitiesLut ) );
    return record;
  }


}  // Anabatic namespace.

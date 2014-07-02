// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KatabaticEngine.cpp"                    |
// +-----------------------------------------------------------------+


#include <iostream>
#include <fstream>
#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "crlcore/Catalog.h"
#include "crlcore/Measures.h"
#include "crlcore/AllianceFramework.h"
#include "katabatic/Session.h"
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/GCell.h"
#include "katabatic/GCellGrid.h"
#include "katabatic/KatabaticEngine.h"


namespace {

  using namespace Hurricane;


  bool  isTopAndBottomConnected ( Segment* segment, set<const Layer*>& layers )
  {
    ltrace(88) << "* Potential Null Length: " << segment << endl;
    ltracein(88);

    Contact* source = dynamic_cast<Contact*>(segment->getSource());
    Contact* target = dynamic_cast<Contact*>(segment->getTarget());

    layers.clear();

    if (source != NULL) {
      forEach ( Hook*, ihook, source->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ((*ihook)->getComponent() == segment) continue;
        layers.insert( (*ihook)->getComponent()->getLayer() );
      }
    }

    if (target != NULL) {
      forEach ( Hook*, ihook, target->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ((*ihook)->getComponent() == segment) continue;
        layers.insert( (*ihook)->getComponent()->getLayer() );
      }
    }

    size_t supplemental = (layers.find(segment->getLayer()) == layers.end()) ? 1 : 0;
    if ( (source->getAnchor() != NULL) or (target->getAnchor() != NULL) ) supplemental++;


    ltraceout(88);

    return layers.size()+supplemental > 2;
  }


} // End of anonymous namespace.


namespace Katabatic {


  using  namespace std;

  using  Hurricane::tab;
  using  Hurricane::ltracein;
  using  Hurricane::ltraceout;
  using  Hurricane::inltrace;
  using  Hurricane::DebugSession;
  using  Hurricane::ForEachIterator;
  using  Hurricane::Bug;
  using  Hurricane::Error;
  using  Hurricane::Warning;
  using  Hurricane::DataBase;
  using  Hurricane::Technology;
  using  Hurricane::Layer;
  using  Hurricane::BasicLayer;
  using  Hurricane::NetExternalComponents;
  using  CRL::AllianceFramework;
  using  CRL::Catalog;
  using  CRL::CatalogProperty;
  using  CRL::Measures;
  using  CRL::addMeasure;
  using  CRL::getMeasure;


// -------------------------------------------------------------------
// Global Variables.


  const char* missingKTBT =
    "%s :\n\n"
    "    Cell %s do not have any Katabatic (or not yet created).\n";

  const char* badMethod =
    "%s :\n\n"
    "    No method id %ud (Cell %s).\n";

  const char* lookupFailed =
    "Katabatic::Extension::getDatas(Segment*) :\n\n"
    "    Cannot find AutoSegment associated to %s (internal error).\n";



// -------------------------------------------------------------------
// Class  :  "Katabatic::KatabaticEngine".

  Name  KatabaticEngine::_toolName = "Katabatic";


  KatabaticEngine* KatabaticEngine::get ( const Cell* cell )
  { return static_cast<KatabaticEngine*>(ToolEngine::get(cell,staticGetName())); }


  const Name& KatabaticEngine::staticGetName ()
  { return _toolName; }


  const Name& KatabaticEngine::getName () const
  { return _toolName; }


  KatabaticEngine::KatabaticEngine ( Cell* cell )
    : ToolEngine         (cell)
    , _timer             ()
    , _state             (EngineCreation)
    , _flags             (EngineDestroyBaseContact)
    , _configuration     (new ConfigurationConcrete())
    , _gcellGrid         (NULL)
    , _chipTools         (cell)
    , _autoSegmentLut    ()
    , _autoContactLut    ()
    , _netRoutingStates  ()
  {
    addMeasure<size_t>( cell, "Gates"
                      , AllianceFramework::getInstancesCount(cell,AllianceFramework::IgnoreFeeds
                                                                 |AllianceFramework::Recursive) );
  }


  void  KatabaticEngine::_postCreate ()
  { ToolEngine::_postCreate(); }


  void  KatabaticEngine::createDetailedGrid ()
  {
    _gcellGrid = GCellGrid::create( this );
    Session::revalidate();

    addMeasure<size_t>( getCell(), "GCells", _gcellGrid->getGCellVector()->size() );

    if (getChipTools().isChip()) {
      unsigned int columns = _gcellGrid->getColumns();
      unsigned int rows    = _gcellGrid->getRows();

      for ( unsigned int depth=0 ; depth<8 ; ++depth ) {
      // West.
        forEach( GCell*, igcell, _gcellGrid->getGCellsColumn(depth,0,rows-1)) {
          ltrace(89) << "Setting as West Pad:" << (*igcell) << endl;
          igcell->setUnderIoPad();
        }
      // East.
        forEach( GCell*, igcell, _gcellGrid->getGCellsColumn(columns-1-depth,0,rows-1)) {
          ltrace(89) << "Setting as East Pad:" << (*igcell) << endl;
          igcell->setUnderIoPad();
        }
      // South.
        forEach( GCell*, igcell, _gcellGrid->getGCellsRow(depth,0,columns-1)) {
          ltrace(89) << "Setting as South Pad:" << (*igcell) << endl;
          igcell->setUnderIoPad();
        }
      // North.
        forEach( GCell*, igcell, _gcellGrid->getGCellsRow(rows-1-depth,0,columns-1)) {
          ltrace(89) << "Setting as North Pad:" << (*igcell) << endl;
          igcell->setUnderIoPad();
        }
      }
    }
  }


  KatabaticEngine* KatabaticEngine::create ( Cell* cell )
  {
    ltrace(90) << "KatabaticEngine::create() - " << cell << endl;

    KatabaticEngine* katabatic = new KatabaticEngine( cell );

    katabatic->_postCreate();

    return katabatic;
  }


  KatabaticEngine::~KatabaticEngine ()
  { delete _configuration; }


  void  KatabaticEngine::_preDestroy ()
  {
    ltrace(90) << "Katabatic::_preDestroy ()" << endl;
    ltracein(90);

    if (getState() < Katabatic::EngineGutted)
      setState( Katabatic::EnginePreDestroying );

    _gutKatabatic();
    _state = EngineGutted;

    ltrace(89) << "About to delete base class ToolEngine." << endl;
    ToolEngine::_preDestroy();

    ltrace(89) << "Exiting Katabatic::_preDestroy()." << endl;
    ltraceout(90);

    cmess2 << "     - GCells        := " << GCell::getAllocateds() << endl;
    cmess2 << "     - AutoContacts  := " << AutoContact::getAllocateds() << endl;
    cmess2 << "     - AutoSegments  := " << AutoSegment::getAllocateds() << endl;
  }


  void  KatabaticEngine::_gutKatabatic ()
  {
    Session::open( this );

    unsetFlags( EngineDestroyBaseContact|EngineDestroyBaseSegment );

    if (_state == EngineDriving) {
      ltracein(90);
      ltrace(90) << "Saving AutoContacts/AutoSegments." << endl;

      size_t fixedSegments = 0;
      AutoSegmentLut::const_iterator isegment = _autoSegmentLut.begin();
      for ( ; isegment != _autoSegmentLut.end() ; ++isegment ) {
        if ((*isegment).second->isFixed()) fixedSegments++;
      }

      cmess1 << "  o  Driving Hurricane data-base." << endl;
      cmess1 << Dots::asSizet("     - Active AutoSegments",AutoSegment::getAllocateds()-fixedSegments) << endl;
      cmess1 << Dots::asSizet("     - Active AutoContacts",AutoContact::getAllocateds()-fixedSegments*2) << endl;
      cmess1 << Dots::asSizet("     - AutoSegments"       ,AutoSegment::getAllocateds()) << endl;
      cmess1 << Dots::asSizet("     - AutoContacts"       ,AutoContact::getAllocateds()) << endl;

      forEach ( Net*, inet, _cell->getNets() )
        _saveNet( *inet );

    //_autoContactLut.clear ();

      ltraceout(90);
    }

    if (_state < EngineGutted ) {
      ltrace(90) << "Gutting Katabatic." << endl;
      _state = EngineGutted;
      setFlags( EngineDestroyBaseContact );

      _destroyAutoSegments();
      _destroyAutoContacts();

      if (_gcellGrid) {
        _gcellGrid->destroy();
        _gcellGrid = NULL;
      }
    }

    Session::close();
  }


  AutoSegment* KatabaticEngine::_lookup ( Segment* segment ) const
  {
    AutoSegmentLut::const_iterator it = _autoSegmentLut.find( segment );
    if (it == _autoSegmentLut.end()) return NULL;

    return (*it).second;
  }


  void  KatabaticEngine::_link ( AutoSegment* autoSegment )
  {
    if (_state > EngineActive) return;
    _autoSegmentLut[ autoSegment->base() ] = autoSegment;
  }


  void  KatabaticEngine::_unlink ( AutoSegment* autoSegment )
  {
    if (_state > EngineDriving) return;

    AutoSegmentLut::iterator it = _autoSegmentLut.find( autoSegment->base() );
    if (it != _autoSegmentLut.end())
      _autoSegmentLut.erase( it );
  }


  AutoContact* KatabaticEngine::_lookup ( Contact* contact ) const
  {
    AutoContactLut::const_iterator it = _autoContactLut.find( contact );
    if (it == _autoContactLut.end()) {
      return NULL;
    }
    return (*it).second;
  }


  void  KatabaticEngine::_link ( AutoContact* autoContact )
  {
    if (_state > EngineActive) return;
    _autoContactLut [ autoContact->base() ] = autoContact;
  }


  void  KatabaticEngine::_unlink ( AutoContact* autoContact )
  {
    if ( _state > EngineActive ) return;

    AutoContactLut::iterator it = _autoContactLut.find( autoContact->base() );
    if (it != _autoContactLut.end())
      _autoContactLut.erase( it );
  }


  void  KatabaticEngine::xmlWriteGCellGrid ( const string& fileName )
  {
    ofstream  file (fileName.c_str());

    xmlWriteGCellGrid( file );
    file.close ();
  }


  void  KatabaticEngine::xmlWriteGCellGrid ( ostream& o )
  {
    if (_gcellGrid)
      _gcellGrid->_xmlWrite( o );
    else
      cerr << Error("Cannot dump GCellGrid: not allocated yet.") << endl;
  }


  void  KatabaticEngine::startMeasures ()
  {
    _timer.resetIncrease();
    _timer.start();
  }


  void  KatabaticEngine::stopMeasures ()
  { _timer.stop(); }


  void  KatabaticEngine::printMeasures ( const string& tag ) const
  {
    ostringstream result;

    result <<  Timer::getStringTime(_timer.getCombTime()) 
           << ", " << Timer::getStringMemory(_timer.getIncrease());
    cmess1 << Dots::asString( "     - Done in", result.str() ) << endl;

    result.str("");
    result << _timer.getCombTime()
           << "s, +" << (_timer.getIncrease()>>10) <<  "Kb/"
           << (_timer.getMemorySize()>>10) << "Kb";
    cmess2 << Dots::asString( "     - Raw measurements", result.str() ) << endl;

    if (not tag.empty()) {
      addMeasure<double>( getCell(), tag+"T",  _timer.getCombTime  () );
      addMeasure<size_t>( getCell(), tag+"S", (_timer.getMemorySize() >> 20) );
    }
  }


  bool  KatabaticEngine::_check ( const char* message ) const
  {
    bool coherency = true;
    if (message)
      cerr << "     o  checking Katabatic DB (" << message << ")." << endl;

    AutoSegmentLut::const_iterator  it = _autoSegmentLut.begin();
    AutoSegmentLut::const_iterator end = _autoSegmentLut.end  ();
    for ( ; it != end ; it++ )
      coherency = it->second->_check() and coherency;

    vector<GCell*>::const_iterator  itGCell = _gcellGrid->getGCellVector()->begin();
    vector<GCell*>::const_iterator endGCell = _gcellGrid->getGCellVector()->end();
    for ( ; itGCell != endGCell ; itGCell++ ) {
      vector<AutoContact*>::const_iterator  itAutoContact = (*itGCell)->getContacts().begin();
      vector<AutoContact*>::const_iterator endAutoContact = (*itGCell)->getContacts().end();
      for ( ; itAutoContact != endAutoContact ; itAutoContact++ ) {
        (*itAutoContact)->checkTopology();
      }
    }

    if (message) cerr << "        - completed." << endl;

    return coherency;
  }


  void  KatabaticEngine::refresh ( unsigned int flags )
  { if (_gcellGrid) _gcellGrid->updateContacts( flags ); }


  void  KatabaticEngine::_destroyAutoSegments ()
  {
    ltrace(90) << "Katabatic::_destroyAutoSegments ()" << endl;

    size_t  expandeds = 0;

    AutoSegmentLut::iterator  it = _autoSegmentLut.begin();
    AutoSegmentLut::iterator end = _autoSegmentLut.end  ();
    for ( ; it != end ; it++ ) {
      expandeds++;
      it->second->destroy();
    }
    if (_state == EngineDriving)
      cerr << "     - Expandeds     := " << expandeds << endl;

    _autoSegmentLut.clear();
  }


  void  KatabaticEngine::_destroyAutoContacts ()
  {
    ltrace(90) << "Katabatic::_destroyAutoContacts ()" << endl;

    AutoContactLut::iterator  it = _autoContactLut.begin();
    AutoContactLut::iterator end = _autoContactLut.end  ();
    for ( ; it != end ; it++ )
      it->second->destroy();

    _autoContactLut.clear();
  }


  Configuration* KatabaticEngine::getConfiguration ()
  { return _configuration; }


  void  KatabaticEngine::findSpecialNets ()
  {
    AllianceFramework* af = AllianceFramework::get();
    forEach ( Net*, net, _cell->getNets() ) {
      
      const char* excludedType = NULL;
      if (net->getType() == Net::Type::POWER ) excludedType = "POWER";
      if (net->getType() == Net::Type::GROUND) excludedType = "GROUND";
    //if (net->getType() == Net::Type::CLOCK ) excludedType = "CLOCK";
      if (excludedType) {
        cparanoid << Warning( "%s is not a routable net (%s,excluded)."
                            , getString(*net).c_str(), excludedType ) << endl;
      }
      if (af->isBLOCKAGE(net->getName())) excludedType = "BLOCKAGE";
      if (excludedType) {
        NetRoutingState* state = getRoutingState( *net, KbCreate );
        state->setFlags( NetRoutingState::Fixed );
      }
    }  // forEach( Net* )
  }


  NetRoutingState* KatabaticEngine::getRoutingState ( Net* net, unsigned int flags )
  {
    NetRoutingState* state = NetRoutingExtension::get( net );

    if (state) {
      NetRoutingStates::iterator istate = _netRoutingStates.find( net->getName() );
      if (istate != _netRoutingStates.end()) {
        if (istate->second != state) {
          cerr << Error( "KatabaticEngine::updateRoutingStates() - %s incoherency between property and LUT:\n"
                        "        Property:%x vs. LUT:%x, re-init LUT from property."
                       , getString(net->getName()).c_str()
                       , (void*)state
                       , (void*)(istate->second)) << endl;
          _netRoutingStates.insert( make_pair(net->getName(), state) );
        }
        return state;
      }
    } else {
      if (not (flags & KbCreate)) return NULL;

      state = NetRoutingExtension::create( net );
    }

    _netRoutingStates.insert( make_pair(net->getName(), state) );
    return state;
  }


  void  KatabaticEngine::loadGlobalRouting ( unsigned int method )
  {
    if (_state < EngineGlobalLoaded)
      throw Error ("KatabaticEngine::loadGlobalRouting() : global routing not present yet.");

    if (_state > EngineGlobalLoaded)
      throw Error ("KatabaticEngine::loadGlobalRouting() : global routing already loaded.");

    switch ( method ) {
      case EngineLoadGrByNet:   _loadGrByNet(); break;
      case EngineLoadGrByGCell:
      default:
        throw Error( badMethod
                   , "Katabatic::loadGlobalRouting()"
                   , method
                   , getString(_cell).c_str()
                   );
    }

    _state = EngineActive;
  }


  void  KatabaticEngine::finalizeLayout ()
  {
    ltrace(90) << "Katabatic::finalizeLayout()" << endl;
    if (_state > EngineDriving) return;

    _state = EngineDriving;

    startMeasures();
    _gutKatabatic();
    stopMeasures ();
    printMeasures( "fin" );

    _state = EngineGutted;
  }


  void  KatabaticEngine::_alignate ( Net* net )
  {
    DebugSession::open( net, 99 );

    ltrace(100) << "Katabatic::_alignate( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << getString(net) << endl;

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> unexploreds;
    vector<AutoSegment*> aligneds;

    forEach ( Component*, icomponent, net->getComponents() ) {
      Segment* segment = dynamic_cast<Segment*>(*icomponent);
      if (segment) {
        AutoSegment* seedSegment = Session::lookup( segment );
        if (seedSegment) unexploreds.push_back( seedSegment );
      }
    }
    sort( unexploreds.begin(), unexploreds.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<unexploreds.size() ; i++ ) {
      AutoSegment* seedSegment = unexploreds[i];

      if (exploredSegments.find(seedSegment->base()) == exploredSegments.end()) {
        ltrace(99) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back( seedSegment );

        forEach ( AutoSegment*, collapsed, seedSegment->getAligneds() ) {
          ltrace(99) << "Aligned: " << *collapsed << endl;
          aligneds.push_back( *collapsed );
          exploredSegments.insert( collapsed->base() );
        }

        ltracein(99);
        sort( aligneds.begin(), aligneds.end(), AutoSegment::CompareId() );

        ltrace(99) << "Seed: " << (void*)aligneds[0]->base() << " " << aligneds[0] << endl;
        for ( size_t j=1 ; j<aligneds.size() ; j++ ) {
          ltrace(99) << "Secondary: " << (void*)(aligneds[j]->base()) << " " << aligneds[j] << endl;
        }

        ltrace(159) << "Align on " << aligneds[0]
                    << " " << DbU::toLambda(aligneds[0]->getAxis()) << endl;
        aligneds[0]->setAxis( aligneds[0]->getAxis(), KbRealignate );
        aligneds.clear();

        ltraceout(99);
      }
    }

    ltraceout(99);

    DebugSession::close();
  }


  void  KatabaticEngine::updateNetTopology ( Net* net )
  {
    DebugSession::open( net );

    ltrace(100) << "Katabatic::updateNetTopology( " << net << " )" << endl;
    ltracein(99);

    vector<AutoContact*>  contacts;
    forEach ( Component*, icomponent, net->getComponents() ) {
      Contact* contact = dynamic_cast<Contact*>( *icomponent );
      if (contact) {
        AutoContact* autoContact = Session::lookup( contact );
        if (autoContact and autoContact->isInvalidatedCache()) 
          contacts.push_back( autoContact );
      }
    }

    for ( size_t i=0 ; i<contacts.size() ; ++i )
      contacts[i]->updateTopology();

    ltraceout(99);
    DebugSession::close();
  }


  void  KatabaticEngine::_computeNetTerminals ( Net* net )
  {
    DebugSession::open( net, 88 );

    ltrace(100) << "Katabatic::_computeNetTerminals( " << net << " )" << endl;
    ltracein(99);

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *segment );
      if (autoSegment == NULL) continue;
      if (autoSegment->isInvalidated()) autoSegment->computeTerminal();
    }

    ltraceout(99);

    DebugSession::close();
  }


  void  KatabaticEngine::_saveNet ( Net* net )
  {
    DebugSession::open( net, 88 );

    ltrace(90) << "Katabatic::_saveNet() " << net << endl;
    ltracein(90);

#if 0
    ltrace(90) << "Deleting zero-length segments." << endl;

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
        ltrace(90) << "* Not associated to an AutoSegment: " << *segment << endl;
        continue;
      }

      if (not isTopAndBottomConnected(*segment,connectedLayers)) {
        nullSegments.push_back( *segment );
        ltrace(90) << "* Null Length: " << *segment << endl;
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

      ltrace(90) << "Deleting: " << nullSegments[i] << endl;
      if (isTopAndBottomConnected(nullSegments[i],connectedLayers)) {
        ltrace(90) << "Deletion cancelled, no longer top or bottom connected." << endl;
        continue;
      }

      ltrace(90) << "* Source: " << (void*)source << " " << source << endl;
      ltrace(90) << "* Target: " << (void*)target << " " << target << endl;

      const Layer* layer = DataBase::getDB()->getTechnology()
        ->getViaBetween( *connectedLayers.begin(), *connectedLayers.rbegin() );

      ltrace(90) << *connectedLayers.begin() << " + " << *connectedLayers.rbegin() << endl;
      ltrace(90) << "* Shrink layer: " << layer << endl;
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
        ltrace(90) << "* detach: "
                   << (void*)masterHook->getNextHook()->getComponent()
                   << " " << masterHook->getNextHook()->getComponent() << endl;
        masterHook->getNextHook()->detach();
      }
      source->destroy();

      masterHook = target->getBodyHook();
      for ( size_t j=0 ; j < slaveHooks.size() ; j++ ) {
        slaveHooks[j]->attach( masterHook );
      }

      ltrace(90) << (void*)target << " " << target << " setLayer: " << layer << endl;
      target->setLayer( layer );
    }
    unsetFlags( EngineDestroyBaseSegment );
#endif

    ltraceout(90);
    DebugSession::close();
  }


  void  KatabaticEngine::_check ( Net* net ) const
  {
    ltrace(200) << "Checking " << net << endl;
    ltracein(200);
    forEach ( Segment*, isegment, net->getComponents().getSubSet<Segment*>() ) {
      AutoSegment* autoSegment = _lookup ( *isegment );
      ltrace(200) << autoSegment << endl;
      if ( autoSegment ) {
        AutoContact* autoContact = autoSegment->getAutoSource();
        ltrace(200) << autoContact << endl;
        if ( autoContact ) autoContact->checkTopology ();

        autoContact = autoSegment->getAutoTarget();
        ltrace(200) << autoContact << endl;
        if ( autoContact ) autoContact->checkTopology ();
      }
    }
    ltraceout(200);
  }


  void  KatabaticEngine::_print ( Net* net ) const
  {
    cerr << "Order: Components of " << net << endl;

    vector<AutoSegment*> segments;
    forEach ( Segment*, isegment, net->getComponents().getSubSet<Segment*>() ) {
      AutoSegment* autoSegment = _lookup( *isegment );
      if (autoSegment)
        segments.push_back( autoSegment );
    }

    sort ( segments.begin(), segments.end(), AutoSegment::CompareId() );

    Interval constraints;
    for ( size_t i=0 ; i<segments.size() ; i++ ) {
      segments[i]->getConstraints( constraints );
      cerr <<  "Order: " << i
           <<  " " << segments[i]->isCanonical()
           <<  " " << setw(6) << DbU::getValueString(segments[i]->getSourceU())
           <<  " " << setw(6) << DbU::getValueString(segments[i]->getLength())
           <<  " " << segments[i]->isGlobal()
           <<  " " << segments[i]->isStrongTerminal()
           <<  " " << segments[i]->isHorizontal()
           <<  " " << setw(6) << DbU::getValueString(segments[i]->getAxis())
           <<  " " << segments[i]->isFixed()
           << " [" << DbU::getValueString(constraints.getVMin())
           <<  ":" << DbU::getValueString(constraints.getVMax())
           << "] " << segments[i]
           << endl;
    }
  }


  void  KatabaticEngine::_print () const
  {
    vector<Net*> nets;
    forEach ( Net*, inet, getCell()->getNets() ) {
      nets.push_back( *inet );
    }

    sort ( nets.begin(), nets.end(), NetCompareByName() );
  }


  string  KatabaticEngine::_getTypeName () const
  {
    return "Katabatic";
  }


  string  KatabaticEngine::_getString () const
  {
    ostringstream  os;

    os << "<" << "Katabatic " << _cell->getName () << " " << _configuration->getRoutingGauge()->getName() << ">";

    return ( os.str() );
  }


  Record* KatabaticEngine::_getRecord () const
  {
    Record* record = ToolEngine::_getRecord ();
    record->add( getSlot( "_flags"           ,  _flags           ) );
    record->add( getSlot( "_state"           ,  _state           ) );
    record->add( getSlot( "_configuration"   ,  _configuration   ) );
    record->add( getSlot( "_gcellGrid"       ,  _gcellGrid       ) );
    record->add( getSlot( "_chipTools"       ,  _chipTools       ) );
    record->add( getSlot( "_autoContactLut"  , &_autoContactLut  ) );
    record->add( getSlot( "_autoSegmentLut"  , &_autoSegmentLut  ) );
    record->add( getSlot( "_netRoutingStates", &_netRoutingStates) );
                                     
    return record;
  }


} // End of Katabatic namespace.

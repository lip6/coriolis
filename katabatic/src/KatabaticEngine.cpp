
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./KatabaticEngine.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <fstream>

#include  "hurricane/DebugSession.h"
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Layer.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Cell.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/Catalog.h"
#include  "crlcore/Measures.h"
#include  "crlcore/AllianceFramework.h"

#include  "katabatic/Session.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCell.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"


namespace {

  using namespace Hurricane;


  bool  isTopAndBottomConnected ( Segment* segment, set<const Layer*>& layers )
  {
    ltrace(88) << "* Potential Null Length: " << segment << endl;
    ltracein(88);

    Contact* source = dynamic_cast<Contact*>(segment->getSource());
    Contact* target = dynamic_cast<Contact*>(segment->getTarget());

    layers.clear ();

    if ( source != NULL ) {
      forEach ( Hook*, ihook, source->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ( (*ihook)->getComponent() == segment ) continue;
        layers.insert ( (*ihook)->getComponent()->getLayer() );
      }
    }

    if ( target != NULL ) {
      forEach ( Hook*, ihook, target->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ( (*ihook)->getComponent() == segment ) continue;
        layers.insert ( (*ihook)->getComponent()->getLayer() );
      }
    }

    size_t supplemental = (layers.find(segment->getLayer()) == layers.end()) ? 1 : 0;
    if ( (source->getAnchor() != NULL) or (target->getAnchor() != NULL) ) supplemental++;

#if 0
    bool  bottomConnect = false;
    bool  topConnect    = false;

    if ( source ) {
      if ( segment->getLayer() != source->getLayer() ) {
        if ( source->getLayer()->getTop() == segment->getLayer() ) {
          bottomConnect = true;
          ltrace(88) << "Source bottom connected: " << source << endl;
        } else {
          topConnect = true;
          ltrace(88) << "Source top connected: " << source << endl;
        }
      }
      forEach ( Hook*, ihook, source->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ( (*ihook)->getComponent() == segment ) continue;
        if ( (*ihook)->getComponent()->getLayer() == segment->getLayer() ) {
          if ( bottomConnect ) topConnect = true;
          else bottomConnect = true;
          break;
        }
      }
    }

    if ( target ) {
      if ( segment->getLayer() != target->getLayer() ) {
        if ( target->getLayer()->getTop() == segment->getLayer() ) {
          bottomConnect = true;
          ltrace(88) << "Target bottom connected: " << target << endl;
        } else {
          topConnect = true;
          ltrace(88) << "Target top connected: " << target << endl;
        }
      }
      forEach ( Hook*, ihook, target->getBodyHook()->getSlaveHooks() ) {
        ltrace(88) << "* Slave: " << (*ihook)->getComponent() << endl;
        if ( (*ihook)->getComponent() == segment ) continue;
        if ( (*ihook)->getComponent()->getLayer() == segment->getLayer() ) {
          if ( bottomConnect ) topConnect = true;
          else bottomConnect = true;
          break;
        }
      }
    }
#endif

    ltraceout(88);

  //return topConnect && bottomConnect;
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
  {
    return static_cast<KatabaticEngine*>(ToolEngine::get(cell,staticGetName()));
  }


  const Name& KatabaticEngine::staticGetName ()
  {
    return _toolName;
  }


  const Name& KatabaticEngine::getName () const
  {
    return _toolName;
  }


  KatabaticEngine::KatabaticEngine ( Cell* cell )
    : ToolEngine         (cell)
    , _timer             ()
    , _state             (StateCreation)
    , _destroyBaseContact(true)
    , _destroyBaseSegment(false)
    , _demoMode          (false)
    , _warnGCellOverload (false)
    , _configuration     (new ConfigurationConcrete())
    , _gcellGrid         (NULL)
    , _chipTools         (cell)
    , _routingNets       ()
  {
    addMeasure<size_t> ( cell, "Gates"
                       , AllianceFramework::getInstancesCount(cell,AllianceFramework::IgnoreFeeds
                                                                  |AllianceFramework::Recursive) );
  }


  void  KatabaticEngine::_postCreate ()
  {
    ToolEngine::_postCreate ();
  }


  void  KatabaticEngine::createDetailedGrid ()
  {
    _gcellGrid = GCellGrid::create ( this );
    Session::revalidate ();

    addMeasure<size_t> ( getCell(), "GCells", _gcellGrid->getGCellVector()->size() );
  }


  KatabaticEngine* KatabaticEngine::create ( Cell* cell )
  {
    ltrace(90) << "KatabaticEngine::create() - " << cell << endl;

    KatabaticEngine* katabatic = new KatabaticEngine ( cell );

    katabatic->_postCreate ();

    return katabatic;
  }


  KatabaticEngine::~KatabaticEngine ()
  {
    delete _configuration;
  }


  void  KatabaticEngine::_preDestroy ()
  {
    ltrace(90) << "Katabatic::_preDestroy ()" << endl;
    ltracein(90);

    if ( getState() < Katabatic::StateGutted )
      setState ( Katabatic::StatePreDestroying );

    _gutKatabatic ();
    _state = StateGutted;

    ltrace(89) << "About to delete base class ToolEngine." << endl;
    ToolEngine::_preDestroy ();

    ltrace(89) << "Exiting Katabatic::_preDestroy()." << endl;
    ltraceout(90);

    cmess2 << "     - GCells        := " << GCell::getAllocateds() << endl;
    cmess2 << "     - AutoContacts  := " << AutoContact::getAllocateds() << endl;
    cmess2 << "     - AutoSegments  := " << AutoSegment::getAllocateds() << endl;
    cmess2 << "     - SegmentEnds   := " << AutoContact::getSegmentEndAllocateds() << endl;
  }


  void  KatabaticEngine::_gutKatabatic ()
  {
    Session::open ( this );

    _destroyBaseContact = false;
    _destroyBaseSegment = false;

    if ( _state == StateDriving ) {
      ltracein(90);
      ltrace(90) << "Saving AutoContacts/AutoSegments." << endl;

      cmess1 << "  o  Driving Hurricane data-base." << endl;
      cmess1 << Dots::asSizet("     - AutoSegments",AutoSegment::getAllocateds()) << endl;
      cmess1 << Dots::asSizet("     - AutoContacts",AutoContact::getAllocateds())<< endl;

      forEach ( Net*, inet, _cell->getNets() )
        _saveNet ( *inet );

    //_autoContactLut.clear ();

      ltraceout(90);
    }

    if ( _state < StateGutted ) {
      ltrace(90) << "Gutting Katabatic." << endl;
      _state = StateGutted;
      _destroyBaseContact = true;

      _destroyAutoSegments ();
      _destroyAutoContacts ();

      if ( _gcellGrid ) {
        _gcellGrid->destroy ();
        _gcellGrid = NULL;
      }
    }

    Session::close ();
  }


  AutoSegment* KatabaticEngine::_lookup ( Segment* segment ) const
  {
    AutoSegmentLut::const_iterator it = _autoSegmentLut.find ( segment );
    if ( it == _autoSegmentLut.end() ) return NULL;

    return (*it).second;
  }


  void  KatabaticEngine::_link ( AutoSegment* autoSegment )
  {
    if ( _state > StateActive ) return;
    _autoSegmentLut [ autoSegment->getSegment() ] = autoSegment;
  }


  void  KatabaticEngine::_unlink ( AutoSegment* autoSegment )
  {
    if ( _state > StateDriving ) return;

    AutoSegmentLut::iterator it = _autoSegmentLut.find ( autoSegment->getSegment() );
    if ( it != _autoSegmentLut.end() )
      _autoSegmentLut.erase ( it );
  }


  AutoContact* KatabaticEngine::_lookup ( Contact* contact ) const
  {
    AutoContactLut::const_iterator it = _autoContactLut.find ( contact );
    if ( it == _autoContactLut.end() ) {
      return NULL;
    }
    return (*it).second;
  }


  void  KatabaticEngine::_link ( AutoContact* autoContact )
  {
    if ( _state > StateActive ) return;
    _autoContactLut [ autoContact->base() ] = autoContact;
  }


  void  KatabaticEngine::_unlink ( AutoContact* autoContact )
  {
    if ( _state > StateActive ) return;

    AutoContactLut::iterator it = _autoContactLut.find ( autoContact->getContact() );
    if ( it != _autoContactLut.end() )
      _autoContactLut.erase ( it );
  }


  void  KatabaticEngine::xmlWriteGCellGrid ( const string& fileName )
  {
    ofstream  file ( fileName.c_str() );

    xmlWriteGCellGrid ( file );

    file.close ();
  }


  void  KatabaticEngine::xmlWriteGCellGrid ( ostream& o )
  {
    if ( _gcellGrid )
      _gcellGrid->_xmlWrite ( o );
    else
      cerr << Error("Cannot dump GCellGrid: not allocated yet.") << endl;
  }


  void  KatabaticEngine::startMeasures ()
  {
    _timer.resetIncrease ();
    _timer.start ();
  }


  void  KatabaticEngine::stopMeasures ()
  {
    _timer.stop ();
  }


  void  KatabaticEngine::printMeasures ( const string& tag ) const
  {
    cmess1 << "     - Done in " << Timer::getStringTime(_timer.getCombTime()) 
           << " [+" << Timer::getStringMemory(_timer.getIncrease()) << "]." << endl;
    cmess1 << "       (raw measurements : " << _timer.getCombTime()
           << "s [+" << (_timer.getIncrease()>>10) <<  "Ko/"
           << (_timer.getMemorySize()>>10) << "Ko])" << endl;

    if ( not tag.empty() ) {
      addMeasure<double> ( getCell(), tag+"T",  _timer.getCombTime  () );
      addMeasure<size_t> ( getCell(), tag+"S", (_timer.getMemorySize() >> 20) );
    }
  }


  bool  KatabaticEngine::_check ( const char* message ) const
  {
    bool coherency = true;
    if ( message )
      cerr << "     o  checking Katabatic DB (" << message << ")." << endl;

    AutoSegmentLut::const_iterator  it = _autoSegmentLut.begin ();
    AutoSegmentLut::const_iterator end = _autoSegmentLut.end   ();
    for ( ; it != end ; it++ )
      coherency = coherency && it->second->_check();

    vector<GCell*>::const_iterator  itGCell = _gcellGrid->getGCellVector()->begin();
    vector<GCell*>::const_iterator endGCell = _gcellGrid->getGCellVector()->end();
    for ( ; itGCell != endGCell ; itGCell++ ) {
      vector<AutoContact*>::const_iterator  itAutoContact = (*itGCell)->getContacts()->begin();
      vector<AutoContact*>::const_iterator endAutoContact = (*itGCell)->getContacts()->end();
      for ( ; itAutoContact != endAutoContact ; itAutoContact++ ) {
        (*itAutoContact)->checkTopology ();
      }
    }

    if ( message )
      cerr << "        - completed." << endl;

    return coherency;
  }


  void  KatabaticEngine::refresh ( bool openSession )
  {
    if ( _gcellGrid ) _gcellGrid->updateContacts ( openSession );
  }


  void  KatabaticEngine::_destroyAutoSegments ()
  {
    ltrace(90) << "Katabatic::_destroyAutoSegments ()" << endl;

    size_t  expandeds = 0;

    AutoSegmentLut::iterator  it = _autoSegmentLut.begin ();
    AutoSegmentLut::iterator end = _autoSegmentLut.end   ();
    for ( ; it != end ; it++ ) {
      if ( !it->second->isCollapsed() ) expandeds++;
      it->second->destroy ();
    }
    if ( _state == StateDriving )
      cerr << "     - Expandeds     := " << expandeds << endl;

    _autoSegmentLut.clear ();
  }


  void  KatabaticEngine::_destroyAutoContacts ()
  {
    ltrace(90) << "Katabatic::_destroyAutoContacts ()" << endl;

    AutoContactLut::iterator  it = _autoContactLut.begin ();
    AutoContactLut::iterator end = _autoContactLut.end   ();
    for ( ; it != end ; it++ )
      it->second->destroy ();

    _autoContactLut.clear ();
  }


  Configuration* KatabaticEngine::getConfiguration ()
  { return _configuration; }


  void  KatabaticEngine::loadGlobalRouting ( unsigned int method, NetSet& nets )
  {
    if ( _state < StateGlobalLoaded )
      throw Error ("KatabaticEngine::loadGlobalRouting() : global routing not present yet.");

    if ( _state > StateGlobalLoaded )
      throw Error ("KatabaticEngine::loadGlobalRouting() : global routing already loaded.");

    AllianceFramework* af = AllianceFramework::get ();
    if ( nets.empty() ) {
      forEach ( Net*, net, _cell->getNets() ) {
        const char* excludedType = NULL;
        if ( net->getType() == Net::Type::POWER  ) excludedType = "POWER";
        if ( net->getType() == Net::Type::GROUND ) excludedType = "GROUND";
        if ( net->getType() == Net::Type::CLOCK  ) excludedType = "CLOCK";
        if ( excludedType ) {
          cerr << Warning("%s is not a routable net (%s,excluded)."
                         ,getString(*net).c_str(),excludedType) << endl;
          continue;
        }
        if ( af->isBLOCKAGE(net->getName()) ) continue;
        _routingNets.insert ( *net );
      }
    } else {
      NetSet::iterator  it = nets.begin();
      for ( ; it != nets.end() ; it++ ) {
        const char* excludedType = NULL;
        if ( (*it)->getType() == Net::Type::POWER  ) excludedType = "POWER";
        if ( (*it)->getType() == Net::Type::GROUND ) excludedType = "GROUND";
        if ( (*it)->getType() == Net::Type::CLOCK  ) excludedType = "CLOCK";
        if ( af->isBLOCKAGE((*it)->getName()) )      excludedType = "BLOCKAGE";
        if ( excludedType ) {
          cerr << Warning("%s is not a routable net (%s), removed from set."
                         ,getString(*it).c_str(),excludedType) << endl;
        } else
          _routingNets.insert ( *it );
      }
    }

    switch ( method ) {
      case LoadGrByNet:   _loadGrByNet(); break;
      case LoadGrByGCell:
      default:
        throw Error ( badMethod
                    , "Katabatic::loadGlobalRouting()"
                    , method
                    , getString(_cell).c_str()
                    );
    }

    _state = StateActive;
  }


  void  KatabaticEngine::finalizeLayout ()
  {
    ltrace(90) << "Katabatic::finalizeLayout()" << endl;
    if ( _state > StateDriving ) return;

    _state = StateDriving;

    startMeasures ();
    _gutKatabatic ();
    stopMeasures  ();
    printMeasures ( "fin" );

    _state = StateGutted;
  }


  void  KatabaticEngine::_alignate ( Net* net )
  {
    DebugSession::open ( net, 99 );

    ltrace(100) << "Katabatic::_alignate ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << getString(net) << endl;

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> unexploreds;
    vector<AutoSegment*> aligneds;

    forEach ( Component*, icomponent, net->getComponents() ) {
      Segment* segment = dynamic_cast<Segment*>(*icomponent);
      if ( segment ) {
        AutoSegment* seedSegment = Session::lookup ( segment );
        if ( seedSegment ) unexploreds.push_back ( seedSegment );
      }
    }
    sort ( unexploreds.begin(), unexploreds.end(), AutoSegment::CompareCanonical() );

    for ( size_t i=0 ; i<unexploreds.size() ; i++ ) {
      AutoSegment* seedSegment = unexploreds[i];

      if ( exploredSegments.find(seedSegment->getSegment()) == exploredSegments.end() ) {
        ltrace(99) << "New chunk from: " << seedSegment << endl;
        aligneds.push_back ( seedSegment );

        forEach ( AutoSegment*, collapsed, seedSegment->getCollapseds() ) {
          ltrace(99) << "Aligned: " << *collapsed << endl;
          aligneds.push_back ( *collapsed );
          exploredSegments.insert ( collapsed->getSegment() );
        }

        ltracein(99);
        sort ( aligneds.begin(), aligneds.end(), AutoSegment::CompareCanonical() );

        ltrace(99) << "Seed: " << (void*)aligneds[0]->base() << " " << aligneds[0] << endl;
        for ( size_t j=1 ; j<aligneds.size() ; j++ ) {
          ltrace(99) << "Secondary: " << (void*)(aligneds[j]->base()) << " " << aligneds[j] << endl;
        }

        ltrace(159) << "Align on " << aligneds[0]
                    << " " << DbU::getLambda(aligneds[0]->getAxis()) << endl;
        aligneds[0]->setAxis ( aligneds[0]->getAxis(), Realignate|AxisSet );
        aligneds.clear ();

        ltraceout(99);
      }
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_canonize ( Net* net )
  {
    DebugSession::open ( net, 99 );

    ltrace(100) << "Katabatic::_canonize ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << getString(net) << endl;

    set<Segment*>        exploredSegments;
    vector<AutoSegment*> unexploreds;
    vector<AutoSegment*> aligneds;

    forEach ( Component*, icomponent, net->getComponents() ) {
      Segment* segment = dynamic_cast<Segment*>(*icomponent);
      if ( segment ) {
        AutoSegment* seedSegment = Session::lookup ( segment );
        if ( seedSegment )
          unexploreds.push_back ( seedSegment );
      }
    }
    sort ( unexploreds.begin(), unexploreds.end(), AutoSegment::CompareCanonical() );

    for ( size_t i=0 ; i<unexploreds.size() ; i++ ) {
      AutoSegment* seedSegment = unexploreds[i];

      if ( exploredSegments.find(seedSegment->getSegment()) == exploredSegments.end() ) {
        ltrace(99) << "New chunk from: " << (void*)seedSegment->base() << ":" << seedSegment << endl;
        aligneds.push_back ( seedSegment );

        bool isCanonicalLocal = seedSegment->isLocal();
        forEach ( AutoSegment*, collapsed, seedSegment->getCollapseds() ) {
          ltrace(99) << "Aligned: " << (void*)collapsed->base() << ":" << *collapsed << endl;
          aligneds.push_back ( *collapsed );
          exploredSegments.insert ( collapsed->getSegment() );

          if ( collapsed->isGlobal() ) isCanonicalLocal = false;
        }

        ltracein(99);
        sort ( aligneds.begin(), aligneds.end(), AutoSegment::CompareCanonical() );

        aligneds[0]->setCanonical      ( true );
        aligneds[0]->setCanonicalLocal ( isCanonicalLocal );
        ltrace(99) << "Canonical: " << (void*)aligneds[0]->base() << ":" << aligneds[0] << endl;

        for ( size_t j=1 ; j<aligneds.size() ; j++ ) {
          aligneds[j]->setCanonical ( false );
          ltrace(99) << "Secondary: " << (void*)(aligneds[j]->base()) << ":" << aligneds[j] << endl;
        }

        ltrace(159) << "Align on " << aligneds[0]
                    << " " << DbU::getLambda(aligneds[0]->getAxis()) << endl;
        aligneds[0]->setAxis ( aligneds[0]->getAxis(), Realignate|AxisSet );
        aligneds.clear ();
        ltraceout(99);
      }
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_computeNetTerminals ( Net* net )
  {
    DebugSession::open ( net, 88 );

    ltrace(100) << "Katabatic::_computeNetTerminals ( " << net << " )" << endl;
    ltracein(99);

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup ( *segment );
      if ( !autoSegment ) continue;
      if ( autoSegment->isInvalidated() ) autoSegment->_computeTerminal();
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_saveNet ( Net* net )
  {
    DebugSession::open ( net, 88 );

    ltrace(90) << "Katabatic::_saveNet() " << net << endl;
    ltracein(90);

    vector<AutoContact*>  autoContacts;

    forEach ( Contact*, icontact, net->getContacts() ) {
      AutoContact* autoContact = Session::lookup ( *icontact );
      if ( autoContact )
        autoContacts.push_back ( autoContact );
    }

    ltrace(90) << "Breaking/Deleting AutoContacts." << endl;
    vector<AutoContact*>::iterator  it = autoContacts.begin();
    for ( ; it != autoContacts.end() ; it++ ) {
      ltrace(90) << "Examining " << (void*)*it << " " << (*it) << endl;
      ltracein(90);
      ltrace(90) << "In " << (void*)((*it)->getGCell()) << " - "
                 << (*it)->getGCell() << endl;

      (*it)->breakUp ();
    // AutoContacts are destroyed through the tool.
    //(*it)->destroy ();

      ltraceout(90);
    }


    ltrace(90) << "Deleting zero-length segments." << endl;

    vector<Segment*>   nullSegments;
    set<const Layer*>  connectedLayers;

    forEach ( Segment*, segment, net->getSegments() ) {
      if ( segment->getLength() ) {
        if ( net->isExternal() ) {
          NetExternalComponents::setExternal ( *segment );
        }
        continue;
      }
    
      if ( Session::lookup(*segment) == NULL ) {
        ltrace(90) << "* Not associated to an AutoSegment: " << *segment << endl;
        continue;
      }

      if ( not isTopAndBottomConnected(*segment,connectedLayers) ) {
        nullSegments.push_back ( *segment );
        ltrace(90) << "* Null Length: " << *segment << endl;
      }
    }

    setDestroyBaseSegment ( true );
    for ( size_t i = 0 ; i < nullSegments.size() ; i++ ) {
      Contact* source = dynamic_cast<Contact*>(nullSegments[i]->getSource());
      Contact* target = dynamic_cast<Contact*>(nullSegments[i]->getTarget());

      if ( (source == NULL) or (target == NULL) ) {
        cerr << Error("Unconnected source/target on %s.",getString(nullSegments[i]).c_str()) << endl;
        continue;
      }

      if ( source->getAnchor() ) {
        if ( target->getAnchor() ) {
          continue;
        //cerr << Bug("Both source & target are anchored while deleting zero-length segment:\n"
        //            "       %s.",getString(nullSegments[i]).c_str()) << endl;
        } else
          swap ( source, target );
      }

      ltrace(90) << "Deleting: " << nullSegments[i] << endl;
      if ( isTopAndBottomConnected(nullSegments[i],connectedLayers) ) {
        ltrace(90) << "Deletion cancelled, no longer top or bottom connected." << endl;
        continue;
      }

      ltrace(90) << "* Source: " << (void*)source << " " << source << endl;
      ltrace(90) << "* Target: " << (void*)target << " " << target << endl;

      const Layer* layer = DataBase::getDB()->getTechnology()
        ->getViaBetween ( *connectedLayers.begin(), *connectedLayers.rbegin() );

      ltrace(90) << *connectedLayers.begin() << " + " << *connectedLayers.rbegin() << endl;
      ltrace(90) << "* Shrink layer: " << layer << endl;
      if ( !layer ) {
        cerr << Error("NULL contact layer while deleting %s."
                     ,getString(nullSegments[i]).c_str()) << endl;
        continue;
      }

      Session::lookup ( nullSegments[i] )->destroy ();

      vector<Hook*>  slaveHooks;
      Hook*          masterHook = source->getBodyHook()->getPreviousMasterHook();

      while ( masterHook->getNextHook() != source->getBodyHook() ) {
        slaveHooks.push_back ( masterHook->getNextHook() );
        ltrace(90) << "* detach: "
                   << (void*)masterHook->getNextHook()->getComponent()
                   << " " << masterHook->getNextHook()->getComponent() << endl;
        masterHook->getNextHook()->detach ();
      }
      source->destroy ();

      masterHook = target->getBodyHook ();
      for ( size_t j=0 ; j < slaveHooks.size() ; j++ ) {
        slaveHooks[j]->attach ( masterHook );
      }

      ltrace(90) << (void*)target << " " << target << " setLayer: " << layer << endl;
      target->setLayer ( layer );
    }
    setDestroyBaseSegment ( false );

    ltraceout(90);
    DebugSession::close ();
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
      AutoSegment* autoSegment = _lookup ( *isegment );
      if ( autoSegment )
        segments.push_back ( autoSegment );
    }

    sort ( segments.begin(), segments.end(), AutoSegment::CompareCanonical() );

    Interval constraints;
    for ( size_t i=0 ; i<segments.size() ; i++ ) {
      segments[i]->getConstraints ( constraints );
      cerr <<  "Order: " << i
           <<  " " << segments[i]->isCanonical()
           <<  " " << segments[i]->isCollapsed()
           <<  " " << setw(6) << DbU::getValueString(segments[i]->getSourceU())
           <<  " " << setw(6) << DbU::getValueString(segments[i]->getLength())
           <<  " " << segments[i]->isGlobal()
           <<  " " << segments[i]->isTerminal()
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
#if defined(CHECK_DETERMINISM)
    cerr << "Order: Nets components." << endl;
#endif

    vector<Net*> nets;
    forEach ( Net*, inet, getCell()->getNets() ) {
      nets.push_back ( *inet );
    }

    sort ( nets.begin(), nets.end(), NetCompareByName() );

#if defined(CHECK_DETERMINISM)
    for ( size_t i=0 ; i<nets.size() ; i++ )
      _print ( nets[i] );
#endif
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
    record->add ( getSlot ( "_demoMode"        ,  _demoMode        ) );
    record->add ( getSlot ( "_state"           ,  _state           ) );
    record->add ( getSlot ( "_configuration"   ,  _configuration   ) );
    record->add ( getSlot ( "_gcellGrid"       ,  _gcellGrid       ) );
    record->add ( getSlot ( "_chipTools"       ,  _chipTools       ) );
    record->add ( getSlot ( "_routingNets"     , &_routingNets     ) );
    record->add ( getSlot ( "_autoContactLut"  , &_autoContactLut  ) );
    record->add ( getSlot ( "_autoSegmentLut"  , &_autoSegmentLut  ) );
                                     
    return ( record );
  }


} // End of Katabatic namespace.

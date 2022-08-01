// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Elmore.cpp"                                  |
// +-----------------------------------------------------------------+

#include "hurricane/DebugSession.h"
#include "hurricane/Error.h"
#include "hurricane/Segment.h"
#include "hurricane/Plug.h"
#include "hurricane/Net.h"
#include "hurricane/Hook.h"
#include "seabreeze/Elmore.h"
#include "seabreeze/Node.h"
#include "seabreeze/SeabreezeEngine.h"

namespace Seabreeze {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::DBo;
  using Hurricane::DbU;
  using Hurricane::Plug;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Hook;
  using Hurricane::Instance;
  using Hurricane::Property;
  using Hurricane::PrivateProperty;
  using Hurricane::Component;
  using Hurricane::Segment;
  using Hurricane::DebugSession;


//---------------------------------------------------------
// Class : "Elmore"

  Elmore::Elmore ( Net* net )
    : _seabreeze(nullptr)
    , _net      (net)
    , _driver   (nullptr)
    , _tree     (new Tree(this))
    , _delays   ()
  {}


  Elmore::~Elmore ()
  {
    cdebug_log(199,0) << "Elmore::~Elmore() " << endl;
    delete _tree;
    for ( Delay* delay : _delays ) delete delay;
  }


  void  Elmore::setup ()
  {
    cdebug_log(199,1) << "Elmore::findDriver()" << endl;

    for ( RoutingPad* rp : _net->getRoutingPads() ) {
      Plug* plug = static_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
      if (plug->getMasterNet()->getDirection() & Net::Direction::DirOut) {
        if (_driver) {
          cerr << Error( "Elmore::setup(): %s has more than one driver:\n"
                         "        * Using:    %s\n"
                         "        * Ignoring: %s"
                       , getString(_net).c_str()
                       , getString(_driver).c_str()
                       , getString(rp).c_str()
                       ) << endl;
          continue;
        }
        _driver = rp;
      } else {
        _delays.push_back( new Delay(this,rp) );
      }
    }
    cdebug_log(199,0) << "Found " << _delays.size() << " sink points:" << endl;
    for ( Delay* delay : _delays ) {
      cdebug_log(199,0) << "| " << delay << endl;
    }
    if (not _driver) {
      cerr << Error( "Elmore::_postCreate(): No driver found on %s, aborting."
                   , getString(_net).c_str() ) << endl;
      cdebug_tabw(199,-1);
      return;
    }

    cdebug_tabw(199,-1);
  }


  const Configuration* Elmore::getConfiguration () const
  { return _seabreeze->getConfiguration(); }


  Delay* Elmore::getDelay ( RoutingPad* rp ) const
  {
    for ( Delay* delay : _delays ) {
      if (delay->getSink() == rp) return delay;
    }
    return nullptr;
  }


  void Elmore::buildTree ()
  {
    if (not _driver) {
      cerr << Error( "Elmore::buildTree(): Net has no driver, aborting." ) << endl;
      return;
    }

    Contact* rootContact = nullptr;
    for ( Component* component : _driver->getSlaveComponents() ) {
      Contact* contact = dynamic_cast<Contact*>(component);
      if (contact) {
        rootContact = contact;
        break;
      }
    }
    if (not rootContact) {
      cerr << Error( "Elmore::buildTree(): No Contact anchored on %s."
                   , getString(_driver).c_str() ) << endl;
      return;
    }

    cdebug_log(199,1) << "Elmore::buildTree()" << endl;
    cdebug_log(199,0) << "Root contact " << rootContact << endl;

    Node*  rootNode = new Node( nullptr, rootContact );
    double R        = 0;
    double C        = 0;
    setRC( &R, &C, rootContact, nullptr );
    rootNode->setR( R );
    rootNode->setC( (C == 0.0) ? 0.0 : 1/C );

    Segment* segment = nullptr;
    size_t   count   = 0;
    for ( Component* component : rootContact->getSlaveComponents() ) {
      segment = dynamic_cast<Segment*>( component );
      if (not segment) continue;
      ++count;
    }
    if (count != 1) {
      cerr << Error( "Elmore::buildTree(): Terminal contact has more than one segment (%d), aborting.\n"
                     "        (on %s)"
                   , count
                   , getString(rootContact).c_str()
                   ) << endl;
      cdebug_tabw(199,-1);
      return;
    }

    buildFromNode( rootNode, segment );

//---------- More than 1 Contact in main RoutingPad ------- ( 1 )
    for ( Component* component : _driver->getSlaveComponents() ) {
      Contact* sideContact = dynamic_cast<Contact*>(component);
      if (sideContact && sideContact != rootContact) {
        Node* sideNode = new Node( rootNode, sideContact );
        double sR = 0;
        double sC = 0;
        setRC ( &sR, &sC, sideContact, nullptr );
        sideNode->setR( sR );
        sideNode->setC( (sC == 0.0 ) ? 0.0: 1/sC );

        Segment* sideSegment = nullptr;
        size_t   count   = 0;
        for ( Component* component : sideContact->getSlaveComponents() ) {
          sideSegment = dynamic_cast<Segment*>( component );
          if (not sideSegment) continue;
          ++count;
        }
        if (count != 1) {
          cerr << Error( "Elmore::buildTree(): Terminal contact has more than one segment (%d), aborting.\n"
                         "        (on %s)"
                       , count
                       , getString(sideContact).c_str()
                       ) << endl;
          cdebug_tabw(199,-1);
          return;
        }

        buildFromNode( sideNode, sideSegment );
      }
    }   
//---------------------------------------------------------------
/*
//---------- More than 1 Contact in 1 RoutingPad ---------- ( 2 )
    for ( RoutingPad* rp : getNet()->getRoutingPads() ) {
      Node* mainNode = nullptr;
      for ( Component* rpComp : rp->getSlaveComponents() ) {
        Contact* mainContact = dynamic_cast<Contact*>(rpComp);
        if ( not mainContact ) continue;
        mainNode = _tree->getNode( mainContact );
        if ( mainNode ) break;
      }
      if ( mainNode == nullptr ) {
        cerr << "No mainNode found!" << endl;
        break;
      }
      for ( Component* rpComp : rp->getSlaveComponents() ) {
        Contact* sideContact = dynamic_cast<Contact*>(rpComp);
        if ( not sideContact || sideContact == mainNode->contact() ) continue;

        Node* sideNode = new Node( mainNode, sideContact );
        double sR      = 0;
        double sC      = 0;
        setRC( &sR, &sC, sideContact, nullptr );
        sideNode->setR( sR );
        sideNode->setC( (sC == 0.0) ? 0.0 : 1/sC );

        Segment* sideSegment = nullptr;
        size_t   sideCount   = 0;
        for ( Component* component : sideContact->getSlaveComponents() ) {
          sideSegment = dynamic_cast<Segment*>( component );
          if (not sideSegment) continue;
          ++sideCount;
        }
        if (sideCount != 1) {
          cerr << Error( "Elmore::buildTree(): Terminal side contact has to have one segment (%d), aborting.\n"
                         "        (on %s)"
                         , sideCount
                         , getString(sideContact).c_str()
                       ) << endl;
          cdebug_tabw(199,-1);
          return;
        }

        buildFromNode( sideNode, sideSegment );
      }
    }
//---------------------------------------------------------
*/
    cdebug_log(199,0) << "Elmore::buildTree() - Finished" << endl;
    cdebug_tabw(199,-1);

    _tree->print( cerr );
  }


  void Elmore::buildFromNode ( Node* rootNode, Segment* toSegment ) 
  {
    if (not rootNode->contact()) {
      cerr << Error( "Elmore::buildFromNode(): rootNode has no contact, aborting." ) << endl;
      return;
    }
    _tree->addNode( rootNode );

    cdebug_log(199,1) << "Elmore::buildFromNode()" << endl;
    cdebug_log(199,0) << "rootNode->_contact=" << rootNode->contact() << endl;
    cdebug_log(199,0)  << "toSegment=" << toSegment << endl;

    Contact* opposite = dynamic_cast<Contact*>( toSegment->getOppositeAnchor( rootNode->contact()) );
    if (not opposite or (rootNode->parent() and (opposite == rootNode->parent()->contact())) ) {
      cdebug_tabw(199,-1);
      return;
    }
    cdebug_log(199,0) << "opposite=" << opposite << endl;

    double Rb = 0;
    double Cb = 0;
    opposite = buildBranch( &Rb, &Cb, opposite );
    if (not opposite) {
      cerr << Error( "Elmore::buildFromNode(): Branch end up on NULL Contact, pruned." ) << endl;
      cdebug_tabw(199,-1);
      return;
    }
    cdebug_log(199,0) << "Reached fork " << opposite << endl;

    Node* node = new Node( rootNode, opposite);
    node->setR( Rb );
    node->setC( (Cb == 0.0) ? 0.0 : 1/Cb );
    cdebug_log(199,0) << "R=" << Rb << " C=" << Cb << endl;

    int count = 0;
    for ( Component* component : opposite->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(component)) ? 1 : 0;
    }
    cdebug_log(199,0) << "Node's contact has : " << count << " segments" << endl;

    if (count == 1) {
      _tree->addNode( node );
    } else if (count > 2) {
      for ( Component* component : opposite->getSlaveComponents() ) {
        Segment* segment = dynamic_cast<Segment*>( component );
        if (not segment) continue;
        cdebug_log(199,0) << "| " << segment << endl;

        Contact* target = dynamic_cast<Contact*>( segment->getOppositeAnchor(opposite) );
        if (not target) {
          cerr << Error( "Elmore::buildFromNode(): Segment missing opposite anchor. pruned." ) << endl;
          continue;
        }
        cdebug_log(199,0) << "target=" << target << endl;

        if (not _tree->isReached(target)) {
          buildFromNode( node, segment);
        }
      }
    }

//---------- More than 1 Contact in this RoutingPad ------- ( 1 )
    Hook* masterHook = opposite->getAnchorHook()->getMasterHook();
    RoutingPad* currentRP = nullptr;
    if ( masterHook ) {
      currentRP = dynamic_cast<RoutingPad*>( masterHook->getComponent() );
      cdebug_log(199,0) << " Look back = " <<currentRP << endl;
    }
    if ( currentRP ) {
      for ( Component* component : currentRP->getSlaveComponents() ) {
        Contact* sideContact = dynamic_cast<Contact*>(component);
        if (sideContact && sideContact != opposite) {
          Node* sideNode = new Node( node, sideContact );
          double sR = 0;
          double sC = 0;
          setRC ( &sR, &sC, sideContact, nullptr );
          sideNode->setR( sR );
          sideNode->setC( (sC == 0.0 ) ? 0.0: 1/sC );

          Segment* sideSegment = nullptr;
          size_t   count   = 0;
          for ( Component* component : sideContact->getSlaveComponents() ) {
            sideSegment = dynamic_cast<Segment*>( component );
            if (not sideSegment) continue;
            ++count;
          }
          if (count != 1) {
            cerr << Error( "Elmore::buildTree(): Terminal contact has more than one segment (%d), aborting.\n"
                           "        (on %s)"
                         , count
                         , getString(sideContact).c_str()
                         ) << endl;
            cdebug_tabw(199,-1);
            return;
          }

          buildFromNode( sideNode, sideSegment );
        }
      }   
    }   
//---------------------------------------------------------------

    cdebug_tabw(199,-1);
  }


  Contact* Elmore::buildBranch ( double* R, double* C, Contact* current ) 
  {
    cdebug_log(199,1) << "Elmore::buildBranch()" << endl;
    cdebug_log(199,0) << "current=" << current << endl;

    while ( true ) {
      _tree->setReached( current );
      int      count   = 0;
      Segment* segment = nullptr; 
      for ( Component* component : current->getSlaveComponents() ) {
        segment = dynamic_cast<Segment*>( component );
        if (not segment) continue;

        Contact* opposite = dynamic_cast<Contact*>( segment->getOppositeAnchor(current) );
        if (opposite and _tree->isReached(opposite)) {
          setRC( R, C, current, segment );
          cdebug_log(199,0) << "current=" << current << endl;
          cdebug_log(199,0) << "segment=" << segment << endl;
          cdebug_log(199,0) << "R=" << *R << endl;
          cdebug_log(199,0) << "C=" << *C << endl;
        }
        ++count;
      }
      if (not count) {
        cerr << Error( "Elmore::buildBranch(): Contact seems to be a dead end, pruning.\n"
                       "        (on %s)"
                     , getString(current).c_str() ) << endl;
        cdebug_tabw(199,-1);
        return nullptr;
      }
      else if (count != 2) {
        break;
      }

      Contact* next = nullptr;
      for ( Component* component : current->getSlaveComponents() ) {
        segment = dynamic_cast<Segment*>( component );
        if (not segment) continue;

        cdebug_log(199,0) << "| " << segment << endl;

        Contact* opposite = dynamic_cast<Contact*>( segment->getOppositeAnchor(current) );
        cdebug_log(199,0) << "opposite=" << opposite << endl;
        if (opposite and not _tree->isReached(opposite))
          next = opposite;
      }

      if (not next) {
        cerr << Error( "Elmore::buildBranch(): Wire loop detected.\n"
                       "        (on %s)"
                     , getString(current).c_str() ) << endl;
        cdebug_tabw(199,-1);
        return nullptr;
      }

      current = next;
    }

    cdebug_tabw(199,-1);
    return current;
  }


  void Elmore::setRC ( double* R, double* C, Contact* contact, Segment* segment )
  {
    double Rcont = getConfiguration()->getRct();
  //double Hcont = DbU::toPhysical( contact->getHeight(), DbU::UnitPower::Nano );
  //double Wcont = DbU::toPhysical( contact->getWidth (), DbU::UnitPower::Nano );
    double Wcont = DbU::toLambda( contact->getWidth () );
    double Acont = Wcont * Wcont;
    *R += Rcont * Acont;

    if (not segment) {
      *C = 0;
    } else {
      double Rseg = getConfiguration()->getRsm();
      double Cseg = getConfiguration()->getCsm();
    //double Lseg = DbU::toPhysical( segment->getLength(), DbU::UnitPower::Nano );
    //double Wseg = DbU::toPhysical( segment->getWidth (), DbU::UnitPower::Nano );
      double Lseg = DbU::toLambda( segment->getLength() );
      double Wseg = DbU::toLambda( segment->getWidth () );
      double Aseg = Lseg * Wseg;
      cdebug_log(199,0) << "Elmore::setRC() on " << segment << endl;
      cdebug_log(199,0) <<  "  Lseg=" << Lseg << " Wseg=" << Wseg << " Aseg=" << Aseg << endl;
      *R += Rseg * Aseg;
      *C += (Aseg) ? 1/(Cseg*Aseg) : 0.0;
    }
  }


  Delay* Elmore::delayElmore ( RoutingPad* rp )
  { return _tree->computeElmoreDelay( rp ); }
 

  void  Elmore::toTree ( ostream& os ) const
  { _tree->print( os ); }


  string  Elmore::_getTypeName () const
  { return "Seabreeze::Elmore"; }


  string  Elmore::_getString () const
  {
    string  s = "<" + _getTypeName() + ">";
    return s;
  }


  Record* Elmore::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record != nullptr) {
      record->add( getSlot("_seabreeze", _seabreeze) );
      record->add( getSlot("_net"      ,  _net     ) );
      record->add( getSlot("_driver"   ,  _driver  ) );
      record->add( getSlot("_tree"     ,  _tree    ) );
      record->add( getSlot("_delays"   , &_delays  ) );
    }
    return record;
  }


//---------------------------------------------------------
// Class : "ElmoreProperty"

  Name ElmoreProperty::_name = "Seabreeze::Elmore";
  

  ElmoreProperty::ElmoreProperty ( Net* net )
    : PrivateProperty()
    , _elmore        (net)
  {
    if (net) {
      SeabreezeEngine* seabreeze
        = dynamic_cast<SeabreezeEngine*>( ToolEngine::get( net->getCell(), "Seabreeze" ));
      if (not seabreeze) {
        cerr << Error( "ElmoreProperty::ElmoreProperty(): Cannot find SeabreezeEngine on %s."
                     , getString(net->getCell()).c_str()) << endl;
      }
      _elmore.setSeabreeze( seabreeze );
      _elmore.setup();
    }
    cdebug_log(199,0) << "ElmoreProperty::ElmoreProperty() on " << net << endl;
  }
  

  ElmoreProperty* ElmoreProperty::create ( Net* net )
  {
    ElmoreProperty* property = new ElmoreProperty( net );
    property->_postCreate();
    return property;
  }


  Name  ElmoreProperty::staticGetName ()
  { return _name; }


  Name ElmoreProperty::getName () const
  { return _name; }


  string ElmoreProperty::_getTypeName () const
  { return "ElmoreProperty"; }


  string  ElmoreProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_elmore) );
    return s;
  }


  Record* ElmoreProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"  ,  _name   ) );
      record->add( getSlot( "_elmore", &_elmore ) );
    }
    return record;
  }


//---------------------------------------------------------
// Class : "ElmoreExtension"

  Elmore* ElmoreExtension::create ( Net* net )
  {
    cdebug_log(199,1) << "ElmoreExtension::create() " << net << endl;
    Elmore* elmore = get( net );
    if (not elmore) {
      ElmoreProperty* property = new ElmoreProperty( net );
      net->put( property );
      elmore = property->getElmore();
    }
    cdebug_tabw(199,-1);
    return elmore;
  }


  void ElmoreExtension::destroy ( Net* net )
  {
    cdebug_log(199,1) << "ElmoreExtension::destroy() " << net << endl;
    Property* property = net->getProperty( ElmoreProperty::staticGetName() );
    if (property) net->remove( property );
    cdebug_tabw(199,-1);
  }


  Elmore* ElmoreExtension::get ( Net* net )
  {
    Elmore* elmore = nullptr;
    Property* property = net->getProperty( ElmoreProperty::staticGetName() );
    if (property) elmore = static_cast<ElmoreProperty*>( property )->getElmore();
    return elmore;
  }

  
}  // Seabreeze namespace.

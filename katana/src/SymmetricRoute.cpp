// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SymmetricRoute.cpp"                     |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContactTerminal.h"
#include "katana/RoutingPlane.h"
#include "katana/TrackFixedSegment.h"
#include "katana/Track.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::DebugSession;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Net;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Component;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Anabatic::Flags;
  using Anabatic::GCell;
  using Anabatic::AutoContact;
  using Anabatic::AutoContactTerminal;
  using Anabatic::AutoSegment;
  using Katana::TrackElement;
  using Katana::DataSymmetric;
  using Katana::KatanaEngine;
  using Katana::Session;


  class Message {
    public:
      inline                Message   ( size_t, string header="" );
      inline size_t         size      () const;
      inline ostringstream& newline   ();
      inline ostringstream& line      ();
      inline void           setHeader ( string );
             void           print     ( ostream& );
    private:
      size_t         _indent;
      string         _header;
      vector<string> _lines;
      ostringstream  _current;
  };


  inline  Message::Message   ( size_t indent, string header )
    : _indent (indent)
    , _header (header)
    , _lines  ()
    , _current()
  { }

  inline size_t         Message::size      () const { return _lines.size(); }
  inline void           Message::setHeader ( string header ) { _header = header; }
  inline ostringstream& Message::line      () { return _current; }

  inline ostringstream& Message::newline ()
  {
    if (_current.str().size())
      _lines.push_back(_current.str());
    _current.str("");
    return _current;
  }

  void Message::print ( ostream& o )
  {
    if (not _header.empty()) _indent = _header.size()+1;

    string head ( _indent, ' ' );
    for ( size_t i=0 ; i<_lines.size() ; ++i ) {
      if ((i == 0) and not _header.empty()) o << _header << " ";
      else                                  o << head;
      o << _lines[i] << endl;
    }
  }


  class TopologicalPairing {
    public:
                           TopologicalPairing    ( KatanaEngine*, Net* );
      bool                 doPairing             ();
      void                 _doSelfPairing        ();
      void                 _doDualPairing        ();
      AutoContactTerminal* _getSymmetricTerminal ( AutoContactTerminal* masterContact );
      Component*           _findMiddleComponent  ();
      void                 _associate            ();
    private:
      KatanaEngine*         _katana;
      AutoSegment*          _seed;
      DataSymmetric*        _data;
  };


  TopologicalPairing::TopologicalPairing ( KatanaEngine* katana, Net* net )
    : _katana      (katana)
    , _seed        (NULL)
    , _data        (NULL)
  {
    _data = _katana->getDataSymmetric( net );
    if (not _data)
      _data = _katana->addDataSymmetric( net );

    if (_data and (_data->getNet() != net) ) _data = NULL;
  }


  bool  TopologicalPairing::doPairing ()
  {
    if (not _data) return false;
    if (not _data->isValid()) return _data->isValid();

    DebugSession::open( _data->getNet(), 144, 146 );

  // Temporary.
  //_data->setSymAxis( _katana->getCell()->getAbutmentBox().getCenter().getX() );
    _data->setSymAxis( NetRoutingExtension::getSymAxis(_data->getNet()) );

    cmess2 << "     - Net: \"" << _data->getNet()->getName() << "\" ";
    cmess2 << "@" << DbU::getValueString(_data->getSymAxis()) << " ";
    cmess2 << (_data->isSymVertical() ? "Vertical" : "Horizontal") << " ";
    if (_data->getSymNet()) cmess2 << "(paired: \"" << _data->getSymNet()->getName() << "\")";
    else         cmess2 << "(self symmetric)";
    cmess2 << endl;
    
    if (_data->getSymNet()) _doDualPairing();
    else                    _doSelfPairing();

    if (_data->isValid()) _data->checkPairing();
    _associate();

    DebugSession::close();

    return _data->isValid();
  }


  void  TopologicalPairing::_doSelfPairing ()
  {
    Component* middle = _findMiddleComponent();

    AutoSegment* _seed = Session::base()->lookup( dynamic_cast<Segment*>( middle ) );
    if (_seed) {
      for ( AutoSegment* segment : _seed->getConnecteds(Flags::Source) ) 
        _data->addReference( segment );

      for ( AutoSegment* segment : _seed->getConnecteds(Flags::Target) )
        _data->addSymmetrical( segment );
    }
  }


  void  TopologicalPairing::_doDualPairing ()
  {
    for ( Contact* terminal : _data->getNet()->getContacts() ) {
      AutoContactTerminal* autoTerminal = dynamic_cast<AutoContactTerminal*>(Session::lookup( terminal ));
      if (autoTerminal) {
        if (not autoTerminal->isEndPoint()) continue;

        _seed = autoTerminal->getSegment();
        Flags flags = (_seed->getAutoSource() == autoTerminal) ? Flags::Target : Flags::Source;
        for ( AutoSegment* segment : _seed->getConnecteds(flags) )
          _data->addReference( segment );

        AutoContactTerminal* symTerminal = _getSymmetricTerminal( autoTerminal );
        if (not symTerminal) { _data->setValid( false ); break; }

        AutoSegment* symSeed = symTerminal->getSegment();
        if (not symSeed) { _data->setValid( false ); break; }

        flags = (symSeed->getAutoSource() == symTerminal) ? Flags::Target : Flags::Source;
        for ( AutoSegment* segment : symSeed->getConnecteds(flags) ) {
          _data->addSymmetrical( segment );
        }
        
        break;
      }
    }
  }


  AutoContactTerminal* TopologicalPairing::_getSymmetricTerminal ( AutoContactTerminal* masterContact )
  {
    Point  mirror      = _data->getSymmetrical( masterContact->getCenter() );
    GCell* mirrorGCell = _katana->getGCellUnder( mirror );
    if (not mirrorGCell) {
      cerr << Error( "getSymmetricTerminal() No GCell under symmetric position (%s,%s)."
                   , DbU::getValueString(mirror.getX()).c_str()
                   , DbU::getValueString(mirror.getY()).c_str()
                   ) << endl;
      _data->setValid( false );
      return NULL;
    }

    for ( AutoContact* mirrorContact : mirrorGCell->getContacts() ) {
      if (mirrorContact->getNet() == _data->getSymNet()) {
        AutoContactTerminal* symContact = dynamic_cast<AutoContactTerminal*>( mirrorContact );
        if (symContact) return symContact;
      }
    }

    cerr << Error( "getSymmetricTerminal() Missing terminal contact in symmetric GCell.\n"
                   "        master:%s\n"
                   "        mirror:%s"
                 , getString(masterContact).c_str(), getString(mirrorGCell).c_str() 
                 ) << endl;

    _data->setValid( false );
    return NULL;
  }


  Component* TopologicalPairing::_findMiddleComponent ()
  {
    DbU::Unit        axis   = _data->getSymAxis();
    Component*       middle = NULL;

    if (_data->isSymVertical()) {
      for ( Component* component : _data->getNet()->getComponents() ) {
        Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
        if (horizontal) {
          if ( (horizontal->getSourceX() < axis) and (axis < horizontal->getTargetX()) ) {
            if (not middle) middle = horizontal;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Horizontal candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(horizontal).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }

        Vertical* vertical = dynamic_cast<Vertical*>(component);
        if (vertical) {
          if (vertical->getSourceX() == axis) {
            if (not middle) middle = vertical;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Vertical candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(vertical).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }

        RoutingPad* rp = dynamic_cast<RoutingPad*>(component);
        if (rp) {
          if ( (rp->getSourcePosition().getX() < axis) and (axis < rp->getTargetPosition().getX()) ) {
            if (not middle) middle = rp;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Horizontal candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(rp).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }
      }
    } else {
      for ( Component* component : _data->getNet()->getComponents() ) {
        Vertical* vertical = dynamic_cast<Vertical*>(component);
        if (vertical) {
          if ( (vertical->getSourceY() < axis) and (axis > vertical->getTargetY()) ) {
            if (not middle) middle = vertical;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Vertical candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(vertical).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }

        Horizontal* horizontal = dynamic_cast<Horizontal*>(component);
        if (horizontal) {
          if (horizontal->getSourceY() == axis) {
            if (not middle) middle = horizontal;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Horizontal candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(horizontal).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }

        RoutingPad* rp = dynamic_cast<RoutingPad*>(component);
        if (rp) {
          if ( (rp->getSourcePosition().getY() < axis) and (axis > rp->getTargetPosition().getY()) ) {
            if (not middle) middle = rp;
            else {
              cerr << Error( "::findMiddleComponent(): Multiple middle Horizontal candidates on \"%s\"\n"
                             "  %s\n"
                             "  %s"
                           , getString(_data->getNet()->getName()).c_str()
                           , getString(middle).c_str()
                           , getString(rp).c_str()
                           ) << endl;
              _data->setValid( false );
              break;
            }
          }
        }
      }
    }

    return middle;
  }


  void  TopologicalPairing::_associate ()
  {
    cdebug_log(144,1) << "TopologicalPairing::_associate()" << endl;

  //cmess1 << "     - Associating symmetrics." << endl;

    if (not _data->isValid()) return;

    const DataSymmetric::Paireds& paireds = _data->getPaireds();
    for ( auto sympair : paireds ) {
      if (not sympair[0]->isCanonical() or not sympair[1]->isCanonical()) continue;

      TrackElement* trackSegment0 = Session::lookup( sympair[0] );
      TrackElement* trackSegment1 = Session::lookup( sympair[1] );

      if (not trackSegment0 or not trackSegment1) continue;

      trackSegment0->setSymmetric( trackSegment1 );
      trackSegment1->setSymmetric( trackSegment0 );
    }

    cdebug_tabw(144,-1);
  }

} // Anonymous namespace.


namespace Katana {

  using namespace std;


  void  KatanaEngine::pairSymmetrics ()
  {
    for ( Net* net : getCell()->getNets() ) {
      if (not NetRoutingExtension::isSymmetric(net)) continue;
      TopologicalPairing(this,net).doPairing();
    }
  }



} // Katana namespace.

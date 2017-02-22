// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./NetRoutingProperty.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Initializer.h"
#include "hurricane/Net.h"


namespace Hurricane {

  using namespace std;
  using Hurricane::Property;


// -------------------------------------------------------------------
// Class  :  "NetRoutingState"

  string  NetRoutingState::_getString () const
  {
    string  s;

    s += (isExcluded            ()) ? 'e' : '-';
    s += (isFixed               ()) ? 'f' : '-';
    s += (isUnconnected         ()) ? 'u' : '-';
    s += (isManualGlobalRoute   ()) ? 'm' : '-';
    s += (isAutomaticGlobalRoute()) ? 'a' : '-';
    s += (isSymmetric           ()) ? 'S' : '-';
    s += (isSymHorizontal       ()) ? 'h' : '-';
    s += (isSymVertical         ()) ? 'v' : '-';
    s += (isSymMaster           ()) ? 'M' : '-';

    return s;
  }


  Record* NetRoutingState::_getRecord () const
  {
    Record* record = new Record ( "<NetRoutingState " + _getString() + " >" );
    if (record != NULL) {
      record->add( getSlot("_net"  , _net   ) );
      record->add( getSlot("_flags", _flags ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "NetRoutingProperty"

  Name  NetRoutingProperty::_name = "Hurricane NetRouting State";


  NetRoutingProperty* NetRoutingProperty::create ( Net* owner )
  {
    NetRoutingProperty *property = new NetRoutingProperty( owner );

    property->_postCreate ();
    return property;
  }


  void  NetRoutingProperty::onReleasedBy ( DBo* owner )
  {
    _state.setNet( NULL );
    PrivateProperty::onReleasedBy( owner );
  }


  Name  NetRoutingProperty::getPropertyName ()
  { return _name; }


  Name  NetRoutingProperty::getName () const
  { return getPropertyName(); }


  string  NetRoutingProperty::_getTypeName () const
  { return _TName ( "NetRoutingProperty" ); }


  string  NetRoutingProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_state) );

    return s;
  }


  Record* NetRoutingProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot("_name" , _name ) );
      record->add( getSlot("_state",&_state) );
    }
    return record;
  }


  bool  NetRoutingProperty::hasJson () const
  { return true; }


  void  NetRoutingProperty::toJson ( JsonWriter* w, const DBo* ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", _getTypeName()      );
    jsonWrite( w, "_state"   , _state._getString() );
    jsonWrite( w, "_axis"    , _state.getSymAxis() );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonNetRoutingProperty"

  Initializer<JsonNetRoutingProperty>  jsonNetRoutingPropertyInit ( 20 );


  JsonNetRoutingProperty::JsonNetRoutingProperty ( unsigned long flags )
    : JsonObject(flags)
  { 
    add( "_state", typeid(string) );
  }


  string  JsonNetRoutingProperty::getTypeName () const
  { return "NetRoutingProperty"; }


  void  JsonNetRoutingProperty::initialize ()
  { JsonTypes::registerType( new JsonNetRoutingProperty (JsonWriter::RegisterMode) ); }


  JsonNetRoutingProperty* JsonNetRoutingProperty::clone ( unsigned long flags ) const
  { return new JsonNetRoutingProperty ( flags ); }


  void JsonNetRoutingProperty::toData ( JsonStack& stack )
  {
    check( stack, "JsonNetRoutingProperty::toData" );

    string       sflags = get<string>( stack, "_state" );
    unsigned int flags  = 0;

    flags |= (sflags[0] == 'e') ? NetRoutingState::Excluded             : 0;
    flags |= (sflags[1] == 'f') ? NetRoutingState::Fixed                : 0;
    flags |= (sflags[2] == 'u') ? NetRoutingState::Unconnected          : 0;
    flags |= (sflags[3] == 'm') ? NetRoutingState::ManualGlobalRoute    : 0;
    flags |= (sflags[4] == 'a') ? NetRoutingState::AutomaticGlobalRoute : 0;
    flags |= (sflags[5] == 'S') ? NetRoutingState::Symmetric            : 0;
    flags |= (sflags[6] == 'h') ? NetRoutingState::Horizontal           : 0;
    flags |= (sflags[7] == 'v') ? NetRoutingState::Vertical             : 0;
    flags |= (sflags[8] == 'M') ? NetRoutingState::SymmetricMaster      : 0;

    NetRoutingProperty* property = NULL;
    DBo*                dbo      = stack.back_dbo();
    if (dbo) {
      Net* net = dynamic_cast<Net*>( dbo );
      if (net) {
        property = dynamic_cast<NetRoutingProperty*>( net->getProperty( NetRoutingProperty::getPropertyName() ) );
        if (property) {
          cerr << Error( "JsonNetRoutingProperty::toData(): %s has already a NetRoutingProperty (overwrite)."
                       , getString(net).c_str()
                       )  << endl;
          NetRoutingState* state = property->getState();
          state->unsetFlags( (unsigned int)-1 );
          state->setFlags  ( flags );
          state->setSymAxis( DbU::fromDb( get<int64_t>(stack,"_axis") ) );
        } else {
          property = NetRoutingProperty::create( net );
          property->getState()->setFlags  ( flags );
          property->getState()->setSymAxis( DbU::fromDb( get<int64_t>(stack,"_axis") ) );
          net->put( property );
        }
      } else {
        cerr << Error( "JsonNetRoutingProperty::toData(): %s must be a Net."
                     , getString(dbo).c_str()
                     ) << endl;
      }
    } else {
      cerr << Error( "JsonNetRoutingProperty::toData(): No DBo in stack to attach to." ) << endl;
    }

    update( stack, property );
  }


// -------------------------------------------------------------------
// Class  :  "NetRoutingExtension"


  const Net*       NetRoutingExtension::_owner = NULL;
  NetRoutingState* NetRoutingExtension::_cache = NULL;


  NetRoutingState* NetRoutingExtension::get ( const Net* net )
  {
    if (net == _owner) return _cache;
    _owner = net;

    Property* property = _owner->getProperty( NetRoutingProperty::getPropertyName() );
    if (property) _cache = static_cast<NetRoutingProperty*>(property)->getState();
    else          _cache = NULL;

    return _cache;
  }


  NetRoutingState* NetRoutingExtension::create ( Net* net )
  {
    get( net );
    if (_cache) return _cache;

    NetRoutingProperty* property = new NetRoutingProperty( net );
    net->put( property );

    _cache = property->getState();
    return _cache;
  }


}  // Hurricane namespace.

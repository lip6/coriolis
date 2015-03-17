// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2015, All Rights Reserved
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


#include  "hurricane/NetRoutingProperty.h"
#include  "hurricane/Net.h"


namespace Hurricane {

  using namespace std;
  using Hurricane::Property;


  string  NetRoutingState::_getString () const
  {
    string  s;

    if (isFixed               ()) s += 'f';
    if (isManualGlobalRoute   ()) s += 'm';
    if (isAutomaticGlobalRoute()) s += 'a';

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

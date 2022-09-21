// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DagProperty.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "foehn/DagProperty.h"


namespace Foehn {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Property;


// -------------------------------------------------------------------
// Class  :  "DagProperty"

  Name  DagProperty::_name = "Foehn::DagProperty";


  DagProperty* DagProperty::create ( DBo* owner )
  {
    DagProperty *property = new DagProperty();

    property->_postCreate ();
    owner->put( property );
    return property;
  }

  
  void  DagProperty::_preDestroy ()
  {
    Super::_preDestroy();
  }


  bool  DagProperty::isNetOwned () const
  { return (dynamic_cast<Net*>( getOwner() ) != nullptr); }


  bool  DagProperty::isInstanceOwned () const
  { return (dynamic_cast<Instance*>( getOwner() ) != nullptr); }


  void  DagProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  DagProperty::getPropertyName ()
  { return _name; }


  Name  DagProperty::getName () const
  { return getPropertyName(); }


  string  DagProperty::_getTypeName () const
  { return "Foehn::DagProperty"; }


  string  DagProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(getOwner()) );

    return s;
  }


  Record* DagProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"    , _name     ) );
      record->add( getSlot( "_flags"   , _flags    ) );
      record->add( getSlot( "_minDepth", _minDepth ) );
      record->add( getSlot( "_maxDepth", _maxDepth ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "DagExtension"


  DagProperty* DagExtension::get ( const DBo* dbo )
  {
    Property* property = dbo->getProperty( DagProperty::getPropertyName() );
    if (property)
      return dynamic_cast<DagProperty*>( property );
    return NULL;
  }


  DagProperty* DagExtension::create ( DBo* dbo )
  {
    DagProperty* property = get( dbo );
    if (not property)
      property = DagProperty::create( dbo );
    return property;
  }


}  // Foehn namespace.

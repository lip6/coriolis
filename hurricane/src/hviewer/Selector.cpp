

// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Selector.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Quark.h"
#include "hurricane/Error.h"
#include "hurricane/viewer/Selector.h"
#include "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  using namespace std;


// -------------------------------------------------------------------
// Class :  "Hurricane::Selector".


  const Name  Selector::_propertyName = _PName ( "Selector" );


  Selector::Selector () : PrivateProperty()
                        , _cellWidgets()
  { }


  string  Selector::_getTypeName () const
  { return _TName("Selector"); }


  const Name&  Selector::getPropertyName ()
  { return _propertyName; }


  Name Selector::getName () const
  { return Selector::getPropertyName(); }


  Occurrence  Selector::getOccurrence () const
  {
	DBo* owner = getOwner();
	if (!owner) return Occurrence();

    Quark* quark = dynamic_cast<Quark*>(owner);
	assert ( quark );

	return quark->getOccurrence();
  }


  Selector* Selector::create ( Occurrence& occurrence )
  {
	if ( !occurrence.isValid() )
      throw Error ( "Can't create " + _TName("Selector") + " : invalid occurrence" );

	if ( occurrence.getProperty(Selector::getPropertyName()) )
      throw Error ( "Can't create " + _TName("Selector") + " : already exists" );

	Selector* selector = new Selector();

	selector->_postCreate();
	occurrence.put ( selector );

	return selector;
  }


  void Selector::_preDestroy()
  {
    set<CellWidget*>::iterator it = _cellWidgets.begin ();
    for ( ; it != _cellWidgets.end() ; it++ )
      detachFrom ( *it, true );

	PrivateProperty::_preDestroy();
  }


  string  Selector::_getString() const
  {
	return "<" + _TName("Selector") + " " + getString(getOccurrence()) + ">";
  }


  Record* Selector::_getRecord () const
  {
	Record* record = PrivateProperty::_getRecord();
	if ( record )
      record->add(getSlot("_cellWidgets", &_cellWidgets));

	return record;
  }


  bool  Selector::isAttachedTo ( CellWidget* widget ) const
  {
	if ( !widget )
      throw Error ( "Can't attach selector : null CellWidget." );

    if ( _cellWidgets.find(widget) == _cellWidgets.end() )
      return false;

    return true;
  }


  void  Selector::attachTo ( CellWidget* widget )
  {
	if ( !widget )
      throw Error ( "Can't attach selector : null CellWidget." );

	_cellWidgets.insert ( widget );
	widget->getSelectorSet().insert ( this );
  }


  void  Selector::detachFrom ( CellWidget* widget, bool inDeletion )
  {
	if ( !widget )
		throw Error ( "Can't detach selector : null CellWidget" );

	widget->getSelectorSet().erase ( this );
	_cellWidgets.erase ( widget );

	if ( !inDeletion && _cellWidgets.empty() ) destroy();
  }


} // End of Hurricane namespace.

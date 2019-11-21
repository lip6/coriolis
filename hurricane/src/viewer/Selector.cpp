//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Selector.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Quark.h"
#include "hurricane/Instance.h"
#include "hurricane/Component.h"
#include "hurricane/Rubber.h"
#include "hurricane/ExtensionGo.h"
#include "hurricane/viewer/Selector.h"
#include "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  using namespace std;


// -------------------------------------------------------------------
// Comparison Class :  "Hurricane::SelectorLess::Selector".
//
// Perform:  lhs < rhs


  bool SelectorLess::operator () ( const Selector* lhs, const Selector* rhs ) const
  {
    const Entity* lhsEntity = lhs->getOccurrence().getEntity();
    const Entity* rhsEntity = rhs->getOccurrence().getEntity();

    const Component* lhsComponent = dynamic_cast<const Component*>( lhsEntity );
    const Component* rhsComponent = dynamic_cast<const Component*>( rhsEntity );
    if (    lhsComponent and     rhsComponent) return lhs < rhs; // lhs & rhs are Components.
    if (    lhsComponent and not rhsComponent) return true;      // lhs only is an Component.
    if (not lhsComponent and     rhsComponent) return false;     // rhs only is an Component.

    const Instance* lhsInstance = dynamic_cast<const Instance*>( lhsEntity );
    const Instance* rhsInstance = dynamic_cast<const Instance*>( rhsEntity );
  //cerr << "Instance LHS: " << (void*)lhsInstance << " RHS: " << (void*)rhsInstance << endl;

    if (    lhsInstance and     rhsInstance) return lhs < rhs; // lhs & rhs are Instances.
    if (    lhsInstance and not rhsInstance) return true;      // lhs only is an Instance.
    if (not lhsInstance and     rhsInstance) return false;     // rhs only is an Instance.

    const Rubber* lhsRubber = dynamic_cast<const Rubber*>( lhsEntity );
    const Rubber* rhsRubber = dynamic_cast<const Rubber*>( rhsEntity );
    if (    lhsRubber and     rhsRubber) return lhs < rhs; // lhs & rhs are Rubbers.
    if (    lhsRubber and not rhsRubber) return true;      // lhs only is an Rubber.
    if (not lhsRubber and     rhsRubber) return false;     // rhs only is an Rubber.

    const ExtensionGo* lhsExtensionGo = dynamic_cast<const ExtensionGo*>( lhsEntity );
    const ExtensionGo* rhsExtensionGo = dynamic_cast<const ExtensionGo*>( rhsEntity );
    if (lhsExtensionGo and  rhsExtensionGo) {             // lhs & rhs are ExtensionGos.
      if (lhsExtensionGo->getName() == rhsExtensionGo->getName())
        return lhs < rhs;

      return lhsExtensionGo->getName() < rhsExtensionGo->getName();
    }
    if (    lhsExtensionGo and not rhsExtensionGo) return true;  // lhs only is an ExtensionGo.
    if (not lhsExtensionGo and     rhsExtensionGo) return false; // rhs only is an ExtensionGo.

    return lhs < rhs;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::Selector".


  const Name  Selector::_propertyName = Name( "Selector" );


  Selector::Selector () : PrivateProperty()
                        , _cellWidgets()
  { }


  string  Selector::_getTypeName () const
  { return "Selector"; }


  const Name& Selector::getPropertyName ()
  { return _propertyName; }


  Name Selector::getName () const
  { return Selector::getPropertyName(); }


  Selector* Selector::create ( Occurrence& occurrence )
  {
	if (not occurrence.isValid())
      throw Error( "Selector::create(): Can't create Selector, invalid occurrence" );

	if (occurrence.getProperty(Selector::getPropertyName()))
      throw Error( "Selector::create(): Can't create Selector, already exists" );

	Selector* selector = new Selector();

	selector->_postCreate();
	occurrence.put( selector );

	return selector;
  }


  void Selector::_preDestroy()
  {
    for ( auto iwidget : _cellWidgets ) detachFrom( iwidget.first, true );
	PrivateProperty::_preDestroy();
  }


  string  Selector::_getString() const
  {
	return "<Selector " + getString(getOccurrence()) + ">";
  }


  Record* Selector::_getRecord () const
  {
	Record* record = PrivateProperty::_getRecord();
	if (record)
      record->add(getSlot("_cellWidgets", &_cellWidgets));

	return record;
  }


  const Quark* Selector::getQuark () const
  {
    const Quark* owner = dynamic_cast<const Quark*>( getOwner() );
    if (not owner) {
      cerr << Error( "Selector::getQuark(): Selector Property is not owned by a Quark." ) << endl;
      return NULL;
    }
    return owner;
  }


  Occurrence  Selector::getOccurrence () const
  {
    const Quark* quark = getQuark();
    return (quark) ? quark->getOccurrence() : Occurrence();
  }


  Path  Selector::getPath () const
  {
    const Quark* quark = getQuark();
    return (quark) ? quark->getOccurrence().getPath() : Path();
  }


  Entity* Selector::getEntity () const
  {
    const Quark* quark = getQuark();
    return (quark) ? quark->getOccurrence().getEntity() : NULL;
  }



  bool  Selector::isAttachedTo ( CellWidget* widget ) const
  {
	if (not widget)
      throw Error( "Selector::isAttachedTo(): NULL widget argument." );

    return (_cellWidgets.find(widget) != _cellWidgets.end());
  }


  void  Selector::attachTo ( CellWidget* widget )
  {
	if (not widget)
      throw Error( "Selector::attachTo(): Cannot attach, NULL widget argument." );

	_cellWidgets.insert( make_pair(widget,0) );
	widget->getSelectorSet().insert( this );
  }


  void  Selector::detachFrom ( CellWidget* widget, bool inDeletion )
  {
	if (not widget)
      throw Error( "Selector::detachFrom(): Cannot detach, NULL widget argument." );

	widget->detach( this );
	_cellWidgets.erase( widget );

	if (not inDeletion and _cellWidgets.empty()) destroy();
  }


  uint32_t  Selector::getFlags ( CellWidget* widget ) const
  {
    map<CellWidget*,uint32_t>::const_iterator iw = _cellWidgets.find( widget );
    if (iw == _cellWidgets.end()) {
      cerr << Error( "Selector::getFlags(): %s is not attached to %s."
                   , getString(this).c_str()
                   , getString(widget).c_str()
                   ) << endl;
      return 0;
    }
    return (*iw).second;
  }

  
  void  Selector::setFlags ( CellWidget* widget , uint32_t flags )
  {
    map<CellWidget*,uint32_t>::iterator iw = _cellWidgets.find( widget );
    if (iw == _cellWidgets.end()) {
      cerr << Error( "Selector::setFlags(): %s is not attached to %s."
                   , getString(this).c_str()
                   , getString(widget).c_str()
                   ) << endl;
      return;
    }
    (*iw).second |= flags;
  }

  
  void  Selector::resetFlags ( CellWidget* widget , uint32_t flags )
  {
    map<CellWidget*,uint32_t>::iterator iw = _cellWidgets.find( widget );
    if (iw == _cellWidgets.end()) {
      cerr << Error( "Selector::resetFlags(): %s is not attached to %s."
                   , getString(this).c_str()
                   , getString(widget).c_str()
                   ) << endl;
      return;
    }
    (*iw).second &= ~flags;
  }

  
  void  Selector::toggle ( CellWidget* widget )
  {
    map<CellWidget*,uint32_t>::iterator iw = _cellWidgets.find( widget );
    if (iw == _cellWidgets.end()) {
      cerr << Error( "Selector::toggle(): %s is not attached to %s."
                   , getString(this).c_str()
                   , getString(widget).c_str()
                   ) << endl;
      return;
    }
    if ((*iw).second & Selected) (*iw).second &= ~Selected;
    else                         (*iw).second |=  Selected;
    (*iw).second |= ToggledByController;
  }


} // End of Hurricane namespace.

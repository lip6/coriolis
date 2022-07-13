// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SelectorCriterion.cpp"                  |
// +-----------------------------------------------------------------+


#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/SelectorCriterion.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectorCriterion".


  SelectorCriterion::~SelectorCriterion ()
  { }


  Occurrence  SelectorCriterion::getNetOccurrence () const
  { return Occurrence(); }


  const Box& SelectorCriterion::getArea () const
  { static const Box empty; return empty; }


  void  SelectorCriterion::undoSelection ( CellWidget* cw )
  { }


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetSelectorCriterion".


  NetSelectorCriterion::NetSelectorCriterion ( Occurrence netOccurrence )
    : _netOccurrence(netOccurrence)
  { }


  NetSelectorCriterion::~NetSelectorCriterion ()
  { }


  NetSelectorCriterion* NetSelectorCriterion::clone () const
  { return new NetSelectorCriterion(_netOccurrence); }


  Occurrence  NetSelectorCriterion::getNetOccurrence () const
  { return _netOccurrence; }


  bool  NetSelectorCriterion::isValid ( CellWidget* cw ) const
  {
    if (cw->getCell() == NULL) return false;
    return _netOccurrence.isValid();
  }


  void  NetSelectorCriterion::doSelection ( CellWidget* cw )
  { cw->select( _netOccurrence ); }


  void  NetSelectorCriterion::undoSelection ( CellWidget* cw )
  { cw->unselect( _netOccurrence ); }


  string  NetSelectorCriterion::_getTypeName () const
  { return "NetSelectorCriterion"; }


  string  NetSelectorCriterion::_getString () const
  {
    string s = "<" + _getTypeName() + " " + getString(_netOccurrence) + ">";
    return s;
  }


  Record* NetSelectorCriterion::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot("_netOccurrence",&_netOccurrence) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::AreaSelectorCriterion".


  AreaSelectorCriterion::AreaSelectorCriterion ( const Box& area )
    : _area(area)
  { }


  AreaSelectorCriterion::~AreaSelectorCriterion ()
  { }


  AreaSelectorCriterion* AreaSelectorCriterion::clone () const
  { return new AreaSelectorCriterion(_area); }


  const Box& AreaSelectorCriterion::getArea () const
  { return _area; }


  bool  AreaSelectorCriterion::isValid ( CellWidget* ) const
  { return true; }


  void  AreaSelectorCriterion::doSelection ( CellWidget* cw )
  { cw->selectOccurrencesUnder ( _area ); }


  string  AreaSelectorCriterion::_getTypeName () const
  { return "AreaSelectorCriterion"; }


  string  AreaSelectorCriterion::_getString () const
  {
    string s = "<" + _getTypeName() + " " + getString(_area) + ">";
    return s;
  }


  Record* AreaSelectorCriterion::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot("_area",&_area) );
    return record;
  }
  

} // End of Hurricane namespace.

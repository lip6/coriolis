
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SelectorCriterion.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/SelectorCriterion.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectorCriterion".


  SelectorCriterion::~SelectorCriterion ()
  { }


  const Net* SelectorCriterion::getNet () const
  { return NULL; }


  const Box& SelectorCriterion::getArea () const
  { static const Box empty; return empty; }


  void  SelectorCriterion::undoSelection ( CellWidget* cw )
  { }


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetSelectorCriterion".


  NetSelectorCriterion::NetSelectorCriterion ( const Net* net )
    : _net (net)
    , _name(_net->getName())
  { }


  NetSelectorCriterion::~NetSelectorCriterion ()
  { }


  const Net* NetSelectorCriterion::getNet () const
  { return _net; }


  bool  NetSelectorCriterion::isValid ( CellWidget* cw ) const
  {
    if ( cw->getCell() == NULL ) return false;
    if ( not cw->getCell()->getNet(_name) ) return false;
    return true;
  }


  void  NetSelectorCriterion::doSelection ( CellWidget* cw )
  { cw->select ( Occurrence(_net) ); }


  void  NetSelectorCriterion::undoSelection ( CellWidget* cw )
  { cw->unselect ( Occurrence(_net) ); }


  string  NetSelectorCriterion::_getTypeName () const
  { return "NetSelectorCriterion"; }


  string  NetSelectorCriterion::_getString () const
  {
    string s = "<" + _getTypeName() + " " + getString(_net) + ">";
    return s;
  }


  Record* NetSelectorCriterion::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot("_net",_net) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::AreaSelectorCriterion".


  AreaSelectorCriterion::AreaSelectorCriterion ( const Box& area )
    : _area(area)
  { }


  AreaSelectorCriterion::~AreaSelectorCriterion ()
  { }


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

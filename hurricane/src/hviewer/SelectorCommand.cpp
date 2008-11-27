
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Header  :       "./SelectorCommand.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/SelectorCommand.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectorCommand".


  SelectorCommand::~SelectorCommand ()
  { }


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetSelectorCommand".


  NetSelectorCommand::NetSelectorCommand ( const Net* net )
    : _net(net)
  { }


  NetSelectorCommand::~NetSelectorCommand ()
  { }


  const Net* NetSelectorCommand::getNet () const
  { return _net; }


  void  NetSelectorCommand::doSelection ( CellWidget* cw, bool delayRedraw )
  {
    cw->_select ( _net, delayRedraw );
  }


  string  NetSelectorCommand::_getTypeName () const
  { return "NetSelectorCommand"; }


  string  NetSelectorCommand::_getString () const
  {
    string s = "<" + _getTypeName() + " " + getString(_net) + ">";
    return s;
  }


  Record* NetSelectorCommand::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot("_net",_net) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::AreaSelectorCommand".


  AreaSelectorCommand::AreaSelectorCommand ( const Box& area )
    : _area(area)
  { }


  AreaSelectorCommand::~AreaSelectorCommand ()
  { }


  const Box& AreaSelectorCommand::getArea () const
  { return _area; }


  void  AreaSelectorCommand::doSelection ( CellWidget* cw, bool delayRedraw )
  {
    cw->_selectOccurrencesUnder ( _area );
  }


  string  AreaSelectorCommand::_getTypeName () const
  { return "AreaSelectorCommand"; }


  string  AreaSelectorCommand::_getString () const
  {
    string s = "<" + _getTypeName() + " " + getString(_area) + ">";
    return s;
  }


  Record* AreaSelectorCommand::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot("_area",&_area) );
    return record;
  }
  

} // End of Hurricane namespace.

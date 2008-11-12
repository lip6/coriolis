
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DebugSession.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#include  <cstdlib>
#include  <sstream>

#include  "hurricane/Warning.h"
#include  "hurricane/Name.h"
#include  "hurricane/Cell.h"
#include  "hurricane/DebugSession.h"




namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::DebugSession".


  DebugSession* DebugSession::_singleton = DebugSession::create ();


  DebugSession* DebugSession::create ()
  {
    if ( !_singleton )
      _singleton = new DebugSession ();
    return _singleton;
  }


  DebugSession::DebugSession ()
    : _symbols()
    , _levels()
  { }


  DebugSession::~DebugSession ()
  { }


  void  DebugSession::_addToTrace ( const Cell* cell, const Name& name )
  {
    Net* net = cell->getNet ( name );
    if ( net )
      _addToTrace ( net );
    else
      cerr << Warning ( "DebugSession::_addToTrace(): %s do not contain Net %s."
                      , getString(cell).c_str(), getString(name).c_str() ) << endl;
  }


  string  DebugSession::_getTypeName () const
  {
    return _TName("DebugSession");
  }


  string  DebugSession::_getString () const
  {
    string  s = "<" + _getTypeName()
              + " " + getString(_symbols.size())
              + " " + getString(_levels.size())
              + ">";
    return s;
  }


  Record* DebugSession::_getRecord () const
  {
    Record* record = new Record ( this->_getString() );
    record->add ( getSlot ( "_symbols", &_symbols ) );
  //record->add ( getSlot ( "_levels" , &_levels  ) );
                                     
    return record;
  }


} // End of Hurricane namespace.

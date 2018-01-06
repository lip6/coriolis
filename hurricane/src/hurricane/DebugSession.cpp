// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DebugSession.cpp"                            |
// +-----------------------------------------------------------------+


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

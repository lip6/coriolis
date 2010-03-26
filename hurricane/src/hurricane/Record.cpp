
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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
// |  C++ Module  :  "./Record.cpp"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Commons.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Record".


  size_t  Record::_allocateds = 0;


  Record::Record ( const string& name )
    : _name (name)
    , _slots()
  {
    _allocateds++;
  }


  Record::~Record ()
  {
  //cerr << "Record::~Record() - " << _name << ": " << hex << (void*)this << dec << endl;
    for ( size_t i=0 ; i<_slots.size() ; i++ )
      delete _slots[i];

    _allocateds--;
  }


  size_t  Record::getAllocateds ()
  {
    return _allocateds;
  }


  Slot* Record::getSlot ( unsigned no ) const
  {
    if ( no >= _slots.size() ) return NULL;
    return _slots[no];
  }


  void Record::add ( Slot* slot )
  {
    if (!slot) {
      cerr << "[ERROR] Record::add(): Attempt to add NULL Slot." << endl;
      return;
    }
    _slots.push_back(slot);
  }


} // End of Hurricane namespace.


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
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ExtensionSlice.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/ExtensionSlice.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ExtensionSlice".


  unsigned int  ExtensionSlice::_masks = 0;
  vector<Name>  ExtensionSlice::_names;


  ExtensionSlice::ExtensionSlice ( Cell* cell, const Name& name, ExtensionSlice::Mask mask )
    : _cell(cell)
    , _name(name)
    , _mask(mask)
    , _quadTree()
  {
    _cell->_insertSlice ( this );
  }


  ExtensionSlice::~ExtensionSlice ()
  {
    cdebug_log(18,0) << "ExtensionSlice::~ExtensionSlice() - " << (void*)this << endl;
    _cell->_removeSlice ( this );
  }


  ExtensionSlice* ExtensionSlice::_create ( Cell* cell, const Name& name )
  {
    if ( !cell )
      throw Error ( "ExtensionSlice::_create(): Cannot create, NULL cell argument." );

    if ( name.isEmpty() )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create, empty name argument."
                  , getString(cell->getName()).c_str()
                  );

    if ( cell->getExtensionSlice(name) )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create, %s already exists."
                  , getString(cell->getName()).c_str()
                  , getString(name).c_str()
                  );

    if ( _masks >= sizeof(unsigned long long) )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create %s, no more free masks."
                  , getString(cell->getName()).c_str()
                  , getString(name).c_str()
                  );

    size_t ibit = 0;
    for ( ibit=0 ; ibit<_masks ; ibit++ ) {
      if ( _names[ibit] == name ) break;
    }
    if ( ibit == _masks ) {
      _names.push_back ( name );
      _masks++;
    }

    ExtensionSlice* slice = new ExtensionSlice(cell,name,(1<<ibit));

    return slice;
  }


  void  ExtensionSlice::_destroy ()
  {
    delete this;
  }


  string  ExtensionSlice::_getTypeName () const
  {
    return _TName("ExtensionSlice");
  }


  string ExtensionSlice::_getString () const
  {
    string s = "<" + _getTypeName();
    s += " " + getString(_name);
    s += ">";
    return s;
  }


  Record* ExtensionSlice::_getRecord () const
  {
    Record* record = new Record(getString(this));
    if (record) {
      record->add(getSlot("Cell"    , _cell    ));
      record->add(getSlot("Name"    , _name    ));
      record->add(getSlot("Mask"    ,&_mask    ));
      record->add(getSlot("QuadTree",&_quadTree));
    }
    return record;
  }


} // End of Hurricane namespace.

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
// |  C++ Module  :  "./ExtensionGo.cpp"                             |
// +-----------------------------------------------------------------+


#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Cell.h"
#include  "hurricane/ExtensionGo.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ExtensionGo".


  ExtensionGo::ExtensionGo ( Cell* cell )
    : Go()
    , _cell(cell)
  { }


  void  ExtensionGo::_preDestroy ()
  {
    cdebug_log(18,0) << "ExtensionGo::_preDestroy() - " << (void*)this << endl;
    Go::_preDestroy ();
  }


  void  ExtensionGo::materialize ()
  {
    if (not isMaterialized()) {
      if (_cell) {
        ExtensionSlice* slice = _cell->getExtensionSlice( getName() );
        if (not slice) slice = ExtensionSlice::_create( _cell, getName() );
        QuadTree* quadTree = slice->_getQuadTree();
        quadTree->insert( this );
        _cell->_fit( quadTree->getBoundingBox() );
      } else {
        cerr << Warning( "%s not inserted into QuadTree.", getString(this).c_str() ) << endl;
      }
    }
  }


  void ExtensionGo::unmaterialize ()
  {
    cdebug_log(18,1) << "ExtensionGo::unmaterialize() - start" << endl;

    if ( isMaterialized() ) {
      ExtensionSlice* slice = _cell->getExtensionSlice( getName() );
      if (slice) {
        _cell->_unfit( getBoundingBox() );
        slice->_getQuadTree()->remove( this );
        if (slice->isEmpty()) slice->_destroy();
      }
    }

    cdebug_tabw(18,-1);
    cdebug_log(18,0) << "ExtensionGo::unmaterialize() - completed" << endl;
  }


  Cell* ExtensionGo::getCell () const
  {
    return _cell;
  }


  string  ExtensionGo::_getTypeName () const
  {
    return "ExtensionGo";
  }


  string ExtensionGo::_getString () const
  {
    string s = Go::_getString();
  //s.insert ( s.length() - 1, " " + getString(getName()) );
    return s;
  }


  Record* ExtensionGo::_getRecord () const
  {
    return Go::_getRecord();
  }


} // End of Hurricane namespace.

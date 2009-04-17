
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ExtensionGo.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


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
    ltrace(10) << "ExtensionGo::_preDestroy() - " << (void*)this << endl;
    Go::_preDestroy ();
  }


  void  ExtensionGo::materialize ()
  {
    if ( !isMaterialized() ) {
      if ( _cell ) {
        ExtensionSlice* slice = _cell->getExtensionSlice ( getName() );
        if ( !slice ) slice = ExtensionSlice::_create ( _cell, getName() );
        QuadTree* quadTree = slice->_getQuadTree ();
        quadTree->insert ( this );
        _cell->_fit ( quadTree->getBoundingBox() );
      } else {
        cerr << Warning("%s not inserted into QuadTree.",getString(this).c_str()) << endl;
      }
    }
  }


  void ExtensionGo::unmaterialize ()
  {
  //ltrace(9) << "ExtensionGo::unmaterialize() - " << (void*)this << endl;
  //ltracein(9);

    if ( isMaterialized() ) {
      ExtensionSlice* slice = _cell->getExtensionSlice ( getName() );
      if ( slice ) {
        _cell->_unfit ( getBoundingBox() );
        slice->_getQuadTree()->remove ( this );
        if ( slice->isEmpty() ) slice->_destroy ();
      }
    }

  //ltrace(9) << "ExtensionGo::unmaterialize() - exit" << endl;
  //ltraceout(9);
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
    s.insert ( s.length() - 1, " " + getString(getName()) );
    return s;
  }


  Record* ExtensionGo::_getRecord () const
  {
    return Go::_getRecord();
  }


} // End of Hurricane namespace.

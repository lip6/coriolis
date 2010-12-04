
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2010, All Rights Reserved
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
// |  C++ Module  :  "./Torus.cpp"                                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <sstream>
#include  "hurricane/Torus.h"


namespace Hurricane {

  using std::string;
  using std::ostringstream;


  Torus::Torus ()
    : _outerBox()
    , _innerBox()
  { }


  Torus::Torus ( const Box& outer, const Box& inner )
    : _outerBox(outer)
    , _innerBox(inner)
  { }


  bool  Torus::contains ( const Box& box ) const
  {
    if ( isEmpty() ) return false;

    return    _outerBox.contains (box)
      and not _innerBox.contains (box)
      and not _innerBox.intersect(box);
  }


  string  Torus::_getString () const
  {
    ostringstream s;
    s << "<Torus [" << DbU::getValueString(_outerBox.getXMin())
      << " "        << DbU::getValueString(_outerBox.getYMin())
      << " "        << DbU::getValueString(_outerBox.getXMax())
      << " "        << DbU::getValueString(_outerBox.getYMax())
      << "] ["      << DbU::getValueString(_innerBox.getXMin())
      << " "        << DbU::getValueString(_innerBox.getYMin())
      << " "        << DbU::getValueString(_innerBox.getXMax())
      << " "        << DbU::getValueString(_innerBox.getYMax());
    return s.str();
  }


  Record* Torus::_getRecord () const
  {
    if ( isEmpty() ) return NULL;

    Record* record = new Record(getString(this));
    record->add(getSlot("_outerBox", &_outerBox));
    record->add(getSlot("_innerBox", &_innerBox));
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// File: ./Reference.cpp
// Authors: J.-P. Chaput
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#include "hurricane/Reference.h"
#include "hurricane/Cell.h"
#include "hurricane/Slice.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Reference implementation
// ****************************************************************************************************

// Static Variables
// ****************


  DbU::Unit Reference::_extend = 0;


Reference::Reference(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y, Type type)
// ************************************************************************************
:  Inherit(cell),
  _name(name),
  _point(x,y),
  _type(type)
{
  if ( !_extend ) _extend = DbU::grid(0.5);

  if (_name.isEmpty())
    throw Error("Can't create " + _TName("Reference") + " : empty name");
}

Reference* Reference::create(Cell* cell, const Name& name, DbU::Unit x, DbU::Unit y, Type type )
// *********************************************************************************************
{
  Reference* reference = new Reference(cell, name, x, y, type);

  reference->_postCreate();

  return reference;
}

Reference* Reference::create(Cell* cell, const Name& name, const Point& point, Type type)
// **************************************************************************************
{
  return create(cell,name,point.getX(),point.getY(),type);
}

Box  Reference::getBoundingBox() const
// ***********************************
{
  return Box(_point).inflate(_extend);
}

void Reference::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *****************************************************
{
  if ((dx != 0) || (dy != 0)) {
    invalidate(false);
    _point.translate(dx, dy);
  }
}

string Reference::_getString() const
// ********************************
{
  string s = Inherit::_getString();
  s.insert(s.length() - 1, " " + getString(_name));
  s.insert(s.length() - 1, " " + getString(_point));
  return s;
}

Record* Reference::_getRecord() const
// ***************************
{
  Record* record = Inherit::_getRecord();
  if (record) {
    record->add(getSlot("Name", &_name));
    record->add(getSlot("point", &_point));
  }
  return record;
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

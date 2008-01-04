// ****************************************************************************************************
// File: Reference.cpp
// Authors: J.-P. Chaput
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
// ****************************************************************************************************

#include "Reference.h"
#include "Cell.h"
#include "Slice.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Reference implementation
// ****************************************************************************************************

// Static Variables
// ****************


Unit Reference::_extend = 0;


Reference::Reference(Cell* cell, const Name& name, Unit x, Unit y)
// ***************************************************************
:  Inherit(cell),
  _name(name),
  _point(x,y)
{
  if ( !_extend ) _extend = GetUnit(0.5);

  if (_name.IsEmpty())
    throw Error("Can't create " + _TName("Reference") + " : empty name");
}

Reference* Reference::Create(Cell* cell, const Name& name, Unit x, Unit y)
// ***********************************************************************
{
  Reference* reference = new Reference(cell, name, x, y);

  reference->_PostCreate();

  return reference;
}

Reference* Reference::Create(Cell* cell, const Name& name, const Point& point)
// ***************************************************************************
{
  return Create(cell,name,point.getX(),point.getY());
}

Box  Reference::GetBoundingBox() const
// ***********************************
{
  return Box(_point).inflate(_extend);
}

void Reference::Translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
  if ((dx != 0) || (dy != 0)) {
    Invalidate(false);
    _point.translate(dx, dy);
  }
}

string Reference::_GetString() const
// ********************************
{
  string s = Inherit::_GetString();
  s.insert(s.length() - 1, " " + GetString(_name));
  s.insert(s.length() - 1, " " + GetString(_point));
  return s;
}

Record* Reference::_GetRecord() const
// ***************************
{
  Record* record = Inherit::_GetRecord();
  if (record) {
    record->Add(GetSlot("Name", &_name));
    record->Add(GetSlot("point", &_point));
  }
  return record;
}

//bool Reference::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// ****************************************************************************************
//{
//  return GetBoundingBox().Intersect(Box(point).Inflate(aperture));
//}
//
//void Reference::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//  assert(!basicLayer);
//
//  Point center = transformation.GetPoint(_point);
//
//  view->DrawLine( center.GetX() - _extend
//                , center.GetY()
//                , center.GetX() + _extend
//                , center.GetY()
//                );
//  view->DrawLine( center.GetX()
//                , center.GetY() - _extend
//                , center.GetX()
//                , center.GetY() + _extend
//                );
//  view->FillCircle(center, _extend>>1);
//  if ( view->IsTextVisible() ) {
//    view->DrawString( GetString(_name)
//                    , center.GetX() + _extend
//                    , center.GetY() + _extend
//                    );
//  }
//}
//
//void Reference::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
//  _Draw(view,NULL,updateArea,transformation);
//}
//
//

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) LIP6/UPMC 2006-2006, All Rights Reserved
// ****************************************************************************************************

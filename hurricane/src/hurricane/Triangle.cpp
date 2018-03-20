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
// |  C++ Module  :  "./Triangle.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Triangle.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"


namespace Hurricane {


  Triangle::Triangle ( Net* net, const Layer* layer, const Point& a, const Point& b, const Point& c )
    : Super  (net)
    , _layer (layer)
    , _points()
  {
    if (not _layer)
      throw Error("Can't create " + _TName("Triangle") + " : null layer");

    _points[0] = a;
    _points[1] = b;
    _points[2] = c;
  }


  Triangle* Triangle::create ( Net* net, const Layer* layer, const Point& a, const Point& b, const Point& c )
  {
    Triangle* triangle = new Triangle ( net, layer, a, b, c );
    triangle->_postCreate();
    return triangle;
  }


  const Layer* Triangle::getLayer () const
  { return _layer; };


  DbU::Unit Triangle::getX () const
  {
    DbU::Unit center = 0;
    for ( Point p : _points ) center += p.getX();
    return center/3;
  }


  DbU::Unit Triangle::getY () const
  {
    DbU::Unit center = 0;
    for ( Point p : _points ) center += p.getY();
    return center/3;
  }


  Box Triangle::getBoundingBox() const
  {
    DbU::Unit xmin = DbU::Max;
    DbU::Unit ymin = DbU::Max;
    DbU::Unit xmax = DbU::Min;
    DbU::Unit ymax = DbU::Min;

    for ( Point p : _points ) {
      xmin = std::min( xmin, p.getX() );
      ymin = std::min( ymin, p.getY() );
      xmax = std::max( xmax, p.getX() );
      ymax = std::max( ymax, p.getY() );
    }

    return Box( xmin, ymin, xmax, ymax );
  }


  Box Triangle::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void Triangle::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ((dx != 0) or (dy != 0)) {
      invalidate(true);
      for ( Point p : _points ) p.translate( dx, dy );
    }
  }


  void Triangle::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error("Can't set layer : null layer");

    if (layer != _layer) {
      invalidate(false);
      _layer = layer;
    }
  }


  void Triangle::setPoints ( const Point& a, const Point& b, const Point& c )
  {
    invalidate(true);
    _points[0] = a;
    _points[1] = b;
    _points[2] = c;
  }


  void Triangle::_toJson ( JsonWriter* writer ) const
  {
    Super::_toJson( writer );

    jsonWrite( writer, "_layer"    ,  _layer->getName() );
    jsonWrite( writer, "_points[0]", &_points[0]        );
    jsonWrite( writer, "_points[1]", &_points[1]        );
    jsonWrite( writer, "_points[2]", &_points[2]        );
  }


  string  Triangle::_getTypeName () const
  { return _TName( "Triangle" ); };


  string Triangle::_getString() const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()));
    s.insert( s.length() - 1, " " + getString(_points[0]));
    s.insert( s.length() - 1, " " + getString(_points[1]));
    s.insert( s.length() - 1, " " + getString(_points[2]));
    return s;
  }


  Record* Triangle::_getRecord() const
  {
    Record* record = Inherit::_getRecord();
    if (record) {
      record->add( getSlot("_layer" ,  _layer ) );
      record->add( getSlot("_points", &_points) );
    }
    return record;
  }


  Initializer<JsonTriangle>  jsonTriangleInit ( 0 );


  void  JsonTriangle::initialize ()
  { JsonTypes::registerType( new JsonTriangle (JsonWriter::RegisterMode) ); }


  JsonTriangle::JsonTriangle ( unsigned long flags )
    : JsonComponent(flags)
  {
    add( "_layer"   , typeid(string) );
    add( "_point[0]", typeid(Point)  );
    add( "_point[1]", typeid(Point)  );
    add( "_point[2]", typeid(Point)  );
  }


  string JsonTriangle::getTypeName () const
  { return "Triangle"; }


  JsonTriangle* JsonTriangle::clone ( unsigned long flags ) const
  { return new JsonTriangle ( flags ); }


  void JsonTriangle::toData ( JsonStack& stack )
  {
    check( stack, "JsonTriangle::toData" );
    unsigned int jsonId = presetId( stack );

    Triangle* triangle = Triangle::create
      ( get<Net*>(stack,".Net")
      , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
      , get<Point>(stack,".Point")
      , get<Point>(stack,".Point")
      , get<Point>(stack,".Point")
      );
  
    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( triangle->getBodyHook(), jsonId, get<string>(stack,"_bodyHook" ) );
    } else {
      cerr << Error( "JsonTriangle::toData(): Missing (Json)Net in stack context." ) << endl;
    }

  // Hook/Ring rebuild are done as a post-process.
    update( stack, triangle );
}


} // Hurricane namespace.

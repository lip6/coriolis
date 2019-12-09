// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2018, All Rights Reserved
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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Rectilinear.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"


namespace Hurricane {

  
// -------------------------------------------------------------------
// Class  :  "Rectilinear".

  Rectilinear::Rectilinear ( Net* net, const Layer* layer, const vector<Point>& points )
    :  Super (net)
    , _layer (layer)
    , _points(points)
  { }


  Rectilinear* Rectilinear::create ( Net* net, const Layer* layer, const vector<Point>& points )
  {
    if (not layer)
      throw Error( "Rectilinear::create(): Can't create, NULL layer" );

    if (points.size() > 1000)
      throw Error( "Rectilinear::create(): Rectlinear polygons must not exceed 1000 vertexes." );

    for ( size_t i=0 ; i<points.size() ; ++i ) {
      size_t j = (i+1) % points.size();

      if (   (points[i].getX() != points[j].getX())
         and (points[i].getY() != points[j].getY()) )
         throw Error( "Rectilinear::create(): Can't create, non H/V edge (points %d:%s - %d:%s)."
                    , i, getString(points[i]).c_str(), j, getString(points[j]).c_str() );
    }

    Rectilinear* rectilinear = new Rectilinear ( net, layer, points );

    rectilinear->_postCreate();

    return rectilinear;
  }




  bool         Rectilinear::isNonRectangle () const { return true; }
  const Layer* Rectilinear::getLayer       () const { return _layer; }
  DbU::Unit    Rectilinear::getX           () const { return getBoundingBox().getCenter().getX(); }
  DbU::Unit    Rectilinear::getY           () const { return getBoundingBox().getCenter().getY(); }


  Box  Rectilinear::getBoundingBox() const
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


  Box  Rectilinear::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void  Rectilinear::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ( (dx != 0) or (dy != 0) ) {
      invalidate( true );
      for ( Point& p : _points ) p.translate( dx, dy );
    }
  }


  void  Rectilinear::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error( "Rectilinear::setLayer(): Can't set layer, NULL layer" );

    if (layer != _layer) {
      invalidate( false );
      _layer = layer;
    }
  }


  void  Rectilinear::setPoints ( const vector<Point>& points )
  {
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      size_t j = (i+1) % points.size();

      if (   (points[i].getX() != points[j].getX())
         and (points[i].getY() != points[j].getY()) )
         throw Error( "Rectilinear::create(): Can't create, non H/V edge." );
    }

    _points = points;
    invalidate(true);
  }


  size_t  Rectilinear::getPointsSize () const
  { return _points.size(); }


  Point  Rectilinear::getPoint ( size_t i ) const
  {
    return _points[ i % getPointsSize() ];
  }


  void Rectilinear::_toJson ( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_layer" ,  _layer->getName() );
  }


  string  Rectilinear::_getTypeName () const
  { return _TName( "Rectilinear" ); }


  string  Rectilinear::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()) );
    s.insert( s.length() - 1, " points:" + getString(_points.size()) );
    return s;
  }


  Record* Rectilinear::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_layer" ,  _layer ) );
      record->add( getSlot("_points", &_points) );
    }
    return record;
  }

  
// -------------------------------------------------------------------
// Class  :  "JsonRectilinear".


  Initializer<JsonRectilinear>  jsonRectilinearInit ( 0 );


  void  JsonRectilinear::initialize ()
  { JsonTypes::registerType( new JsonRectilinear (JsonWriter::RegisterMode) ); }


  JsonRectilinear::JsonRectilinear ( unsigned long flags )
    : JsonComponent(flags)
  {
    add( "_layer" , typeid(string)   );
  }


  string JsonRectilinear::getTypeName () const
  { return "Rectilinear"; }


  JsonRectilinear* JsonRectilinear::clone ( unsigned long flags ) const
  { return new JsonRectilinear ( flags ); }


  void JsonRectilinear::toData ( JsonStack& stack )
  {
    check( stack, "JsonRectilinear::toData" );
    unsigned int jsonId = presetId( stack );

    vector<Point> points;

  // Have to loop over all the points.
  //points.push_back( get<Point>(stack,".Point") );

    Rectilinear* rectilinear = Rectilinear::create
      ( get<Net*>(stack,".Net")
      , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
      , points
      );
  
    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( rectilinear->getBodyHook  (), jsonId, get<string>(stack,"_bodyHook"  ) );
    } else {
      cerr << Error( "Jsonrectilinear::toData(): Missing (Json)Net in stack context." ) << endl;
    }

  // Hook/Ring rebuild are done as a post-process.
    update( stack, rectilinear );
}


} // Hurricane namespace.

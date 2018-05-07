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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Diagonal.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"


namespace Hurricane {

  
// -------------------------------------------------------------------
// Class  :  "Diagonal".

  Diagonal::Diagonal ( Net* net, const Layer* layer, const Point& source, const Point& target, DbU::Unit width )
    :  Super (net)
    , _layer (layer)
    , _source(source)
    , _target(target)
    , _width (width)
    , _B     (0)
  {
    _updateB();
  }


  Diagonal* Diagonal::create ( Net* net, const Layer* layer, const Point& source, const Point& target, DbU::Unit width )
  {
    if (not layer)
      throw Error( "Diagonal::create(): Can't create, NULL layer" );

    DbU::Unit dx = target.getX() - source.getX();
    DbU::Unit dy = target.getY() - source.getY();

    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    if (dx != dy) 
      throw Error( "Diagonal::create(): Can't create, non-diagonal source/target coordinates" );

    Diagonal* diagonal = new Diagonal ( net, layer, source, target, width );

    diagonal->_postCreate();

    return diagonal;
  }


  DbU::Unit    Diagonal::getX              () const { return (_target.getX() + _source.getX()) / 2; }
  DbU::Unit    Diagonal::getY              () const { return (_target.getX() + _source.getX()) / 2; }
  DbU::Unit    Diagonal::getSourceX        () const { return _source.getX(); }
  DbU::Unit    Diagonal::getSourceY        () const { return _source.getY(); }
  DbU::Unit    Diagonal::getTargetX        () const { return _target.getX(); }
  DbU::Unit    Diagonal::getTargetY        () const { return _target.getY(); }
  Point        Diagonal::getSourcePosition () const { return _source; }
  Point        Diagonal::getTargetPosition () const { return _target; }
  DbU::Unit    Diagonal::getWidth          () const { return _width; }
  const Layer* Diagonal::getLayer          () const { return _layer; }


  Box  Diagonal::getBoundingBox() const
  {
    return Box( (_source.getX() < _target.getX()) ? _source.getX() : _target.getX()
              , (_source.getY() < _target.getY()) ? _source.getY() : _target.getY()
              , (_source.getX() > _target.getX()) ? _source.getX() : _target.getX()
              , (_source.getY() > _target.getY()) ? _source.getY() : _target.getY()
              );
  }


  Box  Diagonal::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void  Diagonal::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ( (dx != 0) or (dy != 0) ) {
      invalidate( true );
      _source.translate( dx, dy );
      _target.translate( dx, dy );
    }
  }


  void  Diagonal::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error( "Diagonal::setLayer(): Can't set layer, NULL layer" );

    if (layer != _layer) {
      invalidate( false );
      _layer = layer;
    }
  }


  void  Diagonal::setSource ( Point source )
  {
    if (source == _source) return;
    
    DbU::Unit dx = _target.getX() - source.getX();
    DbU::Unit dy = _target.getY() - source.getY();
    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    if (dx != dy)
        throw Error( "Diagonal::setSource(): Can't set source, non-diagonal points" );

    invalidate(true);
    _source = source;
  }


  void  Diagonal::setTarget ( Point target )
  {
    if (target == _target) return;
    
    DbU::Unit dx = target.getX() - _source.getX();
    DbU::Unit dy = target.getY() - _source.getY();
    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    if (dx != dy)
        throw Error( "Diagonal::setSource(): Can't set target, non-diagonal points" );

    invalidate( true );
    _target = target;
  }


  void  Diagonal::setWidth ( DbU::Unit width )
  {
    if (width == _width) return;

    invalidate( true );
    _width = width;
    _updateB();
  } 


  void  Diagonal::_updateB ()
  {
    _B = (DbU::Unit)( (float)(_width)/sqrt(2) ) - _width/2;
    if (_B % DbU::getPolygonStep()) {
      _B += DbU::getPolygonStep() - (_B % DbU::getPolygonStep());
    }

    if (_B > _width/2) {
      cerr << Error( "Diagonal::_updateB(): Width is too small invalid polygon on,\n"
                     "        %s"
                   , getString(this).c_str() ) << endl;
      _B = _width/2;
    }
  }


  size_t  Diagonal::getPointsSize () const
  { return 8; }


  Point  Diagonal::getPoint ( size_t i ) const
  {
    i = i % getPointsSize();
    
    DbU::Unit dx = _target.getX() - _source.getX();
    DbU::Unit dy = _target.getY() - _source.getY();
    DbU::Unit A  = _width/2;

    if (not ( (dx > 0) xor (dy > 0) ) ) {
      Point s = (dx > 0) ? _source : _target;
      Point t = (dx > 0) ? _target : _source;

      switch ( i ) {
        case 0: return Point( s.getX()- A, s.getY()-_B );
        case 1: return Point( s.getX()-_B, s.getY()- A );
        case 2: return Point( s.getX()+_B, s.getY()- A );
        case 3: return Point( t.getX()+ A, t.getY()-_B );
        case 4: return Point( t.getX()+ A, t.getY()+_B );
        case 5: return Point( t.getX()+_B, t.getY()+ A );
        case 6: return Point( t.getX()-_B, t.getY()+ A );
        default:
        case 7: return Point( s.getX()- A, s.getY()+_B );
      }
    } else {
      Point s = (dx > 0) ? _source : _target;
      Point t = (dx > 0) ? _target : _source;

      switch ( i ) {
        case 0: return Point( s.getX()- A, s.getY()-_B );
        case 1: return Point( t.getX()-_B, t.getY()- A );
        case 2: return Point( t.getX()+_B, t.getY()- A );
        case 3: return Point( t.getX()+ A, t.getY()-_B );
        case 4: return Point( t.getX()+ A, t.getY()+_B );
        case 5: return Point( s.getX()+_B, s.getY()+ A );
        case 6: return Point( s.getX()-_B, s.getY()+ A );
        default:
        case 7: return Point( s.getX()- A, s.getY()+_B );
      }
    }
  }


  void Diagonal::_toJson ( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_layer" ,  _layer->getName() );
    jsonWrite( writer, "_width" ,  _width            );
    jsonWrite( writer, "_source", &_source           );
    jsonWrite( writer, "_target", &_target           );
  }


  string  Diagonal::_getTypeName () const
  { return _TName( "Diagonal" ); }


  string  Diagonal::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()) );
    s.insert( s.length() - 1
            ,  " [" + DbU::getValueString(_source.getX())
            +   " " + DbU::getValueString(_source.getY())
            + "] [" + DbU::getValueString(_target.getX())
            +   " " + DbU::getValueString(_target.getY())
            + "]"
            );
    return s;
  }


  Record* Diagonal::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add(           getSlot("_layer" ,  _layer ) );
      record->add( DbU::getValueSlot("_width" , &_width ) );
      record->add(           getSlot("_source", &_source) );
      record->add(           getSlot("_target", &_target) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Diagonal::Points_Contour".

  Diagonal::Points_Contour::Locator::Locator ( const Diagonal* diagonal )
    : PointHL ()
    , _diagonal(diagonal)
    , _iPoint (0)
  { }


  PointHL* Diagonal::Points_Contour::Locator::getClone () const
  { return new Locator(*this); }


  Point  Diagonal::Points_Contour::Locator::getElement () const
  { return _diagonal->getPoint(_iPoint); }


  bool  Diagonal::Points_Contour::Locator::isValid () const
  { return (_iPoint < _diagonal->getPointsSize()); }


  void  Diagonal::Points_Contour::Locator::progress ()
  { if (isValid()) ++_iPoint; }


  string  Diagonal::Points_Contour::Locator::_getString () const
  {
    string s = "<" + _TName("Points_Contour::Locator")
                   + getString(getElement())
                   + ">";
    return s;
  }


  PointHC* Diagonal::Points_Contour::getClone () const
  { return new Points_Contour(*this); }


  PointHL* Diagonal::Points_Contour::getLocator () const
  { return new Locator(_diagonal); }


  string  Diagonal::Points_Contour::_getString () const
  {
    string s = "<" + _TName("Points_Contour") + " "
                   + getString(_diagonal)
                   + ">";
    return s;
  }

  
// -------------------------------------------------------------------
// Class  :  "JsonDiagonal".


  Initializer<JsonDiagonal>  jsonDiagonalInit ( 0 );


  void  JsonDiagonal::initialize ()
  { JsonTypes::registerType( new JsonDiagonal (JsonWriter::RegisterMode) ); }


  JsonDiagonal::JsonDiagonal ( unsigned long flags )
    : JsonComponent(flags)
  {
    add( "_layer" , typeid(string)   );
    add( "_width" , typeid(uint64_t) );
    add( "_source", typeid(Point)    );
    add( "_target", typeid(Point)    );
  }


  string JsonDiagonal::getTypeName () const
  { return "Diagonal"; }


  JsonDiagonal* JsonDiagonal::clone ( unsigned long flags ) const
  { return new JsonDiagonal ( flags ); }


  void JsonDiagonal::toData ( JsonStack& stack )
  {
    check( stack, "JsonDiagonal::toData" );
    unsigned int jsonId = presetId( stack );

    Diagonal* diagonal = Diagonal::create
      ( get<Net*>(stack,".Net")
      , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
      , get<Point>(stack,".Point")
      , get<Point>(stack,".Point")
      , DbU::fromDb( get<int64_t>(stack,"_width"   ) )
      );
  
    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( diagonal->getBodyHook  (), jsonId, get<string>(stack,"_bodyHook"  ) );
    } else {
      cerr << Error( "Jsondiagonal::toData(): Missing (Json)Net in stack context." ) << endl;
    }

  // Hook/Ring rebuild are done as a post-process.
    update( stack, diagonal );
}


} // Hurricane namespace.

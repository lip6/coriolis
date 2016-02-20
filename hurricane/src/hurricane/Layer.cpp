// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Layer.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::Layer".

  Layer::Layer ( Technology* technology
               , const Name& name
               , const DbU::Unit& minimalSize
               , const DbU::Unit& minimalSpacing
               , const DbU::Unit& pitch
               ) : DBo()
                 , _technology(technology)
                 , _name(name)
                 , _mask(0)
                 , _extractMask(0)
                 , _minimalSize(minimalSize)
                 , _minimalSpacing(minimalSpacing)
                 , _nextOfTechnologyLayerMap(NULL)
                 , _working(false)
  {
    if ( !_technology )
      throw Error ( "Can't create " + _TName("Layer") + " : null technology" );

    if ( _name.isEmpty() )
      throw Error ( "Can't create " + _TName("Layer") + " : empty name" );

    if ( _technology->getLayer(_name) )
      throw Error ( "Can't create " + _TName("Layer") + " " + getString(_name) + " : already exists" );
  }


  const Layer* Layer::getBlockageLayer () const
  { return NULL; }


  const Layer* Layer::getTop () const
  { return NULL; } 


  const Layer* Layer::getBottom () const
  { return NULL; } 


  const Layer* Layer::getOpposite ( const Layer* ) const
  { return NULL; } 


  Layer* Layer::getMetalAbove ( bool useWorking ) const
  { return _technology->getMetalAbove(this,useWorking); }


  Layer* Layer::getMetalBelow ( bool useWorking ) const
  { return _technology->getMetalBelow(this,useWorking); }


  Layer* Layer::getCutAbove ( bool useWorking ) const
  { return _technology->getCutAbove(this,useWorking); }


  Layer* Layer::getCutBelow ( bool useWorking ) const
  { return _technology->getCutBelow(this,useWorking); }


  DbU::Unit  Layer::getEnclosure () const
  { return 0; }


  DbU::Unit  Layer::getExtentionCap () const
  { return 0; }


  DbU::Unit  Layer::getExtentionWidth () const
  { return 0; }


  DbU::Unit  Layer::getEnclosure ( const BasicLayer* layer ) const
  { return 0; }


  DbU::Unit  Layer::getExtentionCap ( const BasicLayer* layer ) const
  { return 0; }


  DbU::Unit  Layer::getExtentionWidth ( const BasicLayer* layer ) const
  { return 0; }


  bool Layer::contains ( const Layer* layer ) const
  {
    return ( layer && ((_mask & layer->getMask()) == layer->getMask()) );
  }


  bool Layer::intersect ( const Layer* layer ) const
  {
    return ( layer->getMask().contains(_mask) );
  }


  void Layer::setName ( const Name& name )
  {
    if ( name != _name ) {
      if ( name.isEmpty() )
        throw Error ( "Can't change layer name : empty name" );

      if ( _technology->getLayer(name) )
        throw Error ( "Can't change layer name : already exists" );

      _technology->_getLayerMap()._remove(this);
      _name = name;
      _technology->_getLayerMap()._insert(this);
    }
  }


  void Layer::setMinimalSize ( const DbU::Unit& minimalSize )
  {
    _minimalSize = minimalSize;
  }


  void Layer::setMinimalSpacing ( const DbU::Unit& minimalSpacing )
  {
    _minimalSpacing = minimalSpacing;
  }


  void  Layer::setEnclosure ( const BasicLayer* layer, DbU::Unit )
  {
    cerr << "[WARNING] Layer::setEnclosure() musn't be called on "
         << _getTypeName() << ": dummy implementation." << endl;
  }


  void  Layer::setExtentionCap ( const BasicLayer* layer, DbU::Unit )
  {
    cerr << "[WARNING] Layer::setExtentionCap() musn't be called on "
         << _getTypeName() << ": dummy implementation." << endl;
  }


  void  Layer::setExtentionWidth ( const BasicLayer* layer, DbU::Unit )
  {
    cerr << "[WARNING] Layer::setExtentionWidth() musn't be called on "
         << _getTypeName() << ": dummy implementation." << endl;
  }
  

  void Layer::_postCreate ()
  {
    _technology->_getLayerMap()._insert(this);
    _technology->_insertInLayerMaskMap(this);

    DBo::_postCreate();
  }


  void Layer::_preDestroy ()
  {
    _technology->_getLayerMaskMap().erase(_mask);
    _technology->_getLayerMap()._remove(this);

    DBo::_preDestroy ();
  }


  void  Layer::_onDbuChange ( float scale )
  {
    _minimalSize    = (DbU::Unit)( (float)_minimalSize    * scale );
    _minimalSpacing = (DbU::Unit)( (float)_minimalSpacing * scale );
  }


  string Layer::_getString () const
  {
    string s = DBo::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    return s;
  }


  Record* Layer::_getRecord () const
  {
    Record* record = DBo::_getRecord();
    if (record) {
      record->add(getSlot("Technology"    , _technology     ));
      record->add(getSlot("Name"          , &_name          ));
      record->add(getSlot("Mask"          , &_mask          ));
      record->add(getSlot("ExtractMask"   , &_extractMask   ));
      record->add(DbU::getValueSlot("MinimalSize"   , &_minimalSize   ));
      record->add(DbU::getValueSlot("MinimalSpacing", &_minimalSpacing));
    }
    return record;
  }


  const Name& Layer::_sgetName ( const Layer* layer )
  { return layer->getName(); }


  void  Layer::_toJson ( JsonWriter* writer ) const
  {
    Super::_toJson( writer );

    jsonWrite( writer, "_name"          , getName()               );
    jsonWrite( writer, "_mask"          , getString(_mask)        );
    jsonWrite( writer, "_extractMask"   , getString(_extractMask) );
    jsonWrite( writer, "_minimalSize"   , _minimalSize            );
    jsonWrite( writer, "_minimalSpacing", _minimalSpacing         );
    jsonWrite( writer, "_working"       , _working                );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonLayer".

  JsonLayer::JsonLayer ( unsigned long flags )
    : JsonDBo(flags)
  {
    add( "_name"          , typeid(string)   );
    add( "_mask"          , typeid(string)   );
    add( "_extractMask"   , typeid(string)   );
    add( "_minimalSize"   , typeid(uint64_t) );
    add( "_minimalSpacing", typeid(uint64_t) );
    add( "_working"       , typeid(uint64_t) );
  }


  Technology* JsonLayer::lookupTechnology ( JsonStack& stack, const string& fname ) const
  {
    Technology* techno = get<Technology*>( stack, ".Technology" );
    if (not techno) {
      techno = get<Technology*>( stack, "_technology" );
    }
    if (not techno) {
      cerr << Error( "%s(): .Technology/_technology missing in the stack.", fname.c_str() ) << endl;
      techno = DataBase::getDB()->getTechnology();
    }
    return techno;
  }

} // Hurricane namespace.

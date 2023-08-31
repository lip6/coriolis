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
// |  C++ Module  :  "./Text.cpp"                                    |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Slice.h"
#include "hurricane/Text.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Cell.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"


namespace Hurricane {

  
// -------------------------------------------------------------------
// Class  :  "Text".

  Text::Text ( Cell* cell, const Layer* layer, const Box& boundingBox, std::string text )
    :  Super      ()
    , _cell       (cell)
    , _layer      (layer)
    , _boundingBox(boundingBox)
    , _text       (text)
  { }


  Text* Text::create ( Cell* cell, const Layer* layer, const Box& boundingBox, std::string text )
  {
    if (not layer)
      throw Error( "Text::create(): Can't create, NULL layer" );

    Text* goText = new Text ( cell, layer, boundingBox, text );
    goText->_postCreate();

    return goText;
  }




  Cell*        Text::getCell        () const { return _cell; }
  const Layer* Text::getLayer       () const { return _layer; }
  DbU::Unit    Text::getX           () const { return getBoundingBox().getCenter().getX(); }
  DbU::Unit    Text::getY           () const { return getBoundingBox().getCenter().getY(); }


  Box  Text::getBoundingBox() const
  { return _boundingBox; }


  Box  Text::getBoundingBox ( const BasicLayer* basicLayer ) const
  {
    if (not _layer->contains(basicLayer)) return Box();
    return getBoundingBox();
  }


  void  Text::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    if ( (dx != 0) or (dy != 0) ) {
      invalidate( true );
      _boundingBox.translate( dx, dy );
    }
  }


  void  Text::setLayer ( const Layer* layer )
  {
    if (not layer) throw Error( "Text::setLayer(): Can't set layer, NULL layer" );

    if (layer != _layer) {
      invalidate( false );
      _layer = layer;
    }
  }


  void Text::materialize()
  {
    cdebug_log(18,0) << "Text::materialize() - " << this << endl;
  
    if (isMaterialized()) return;
    
    Cell*  cell  = getCell();
    const Layer* layer = getLayer();
    if (not cell or not layer) return;
    
    Slice* slice = cell->getSlice( layer );
    if (not slice) slice = Slice::_create( cell, layer );
    QuadTree* quadTree = slice->_getQuadTree();
    quadTree->insert( this );
    cell->_fit( quadTree->getBoundingBox() );
  }
  

  void Text::unmaterialize()
  {
    cdebug_log(18,0) << "Text::unmaterialize() " << this << endl;
  
    if (not isMaterialized()) return;
    Cell*  cell  = getCell();
    Slice* slice = cell->getSlice(getLayer());
    if (not slice) return;

    cell->_unfit(getBoundingBox());
    slice->_getQuadTree()->remove(this);
    if (slice->isEmpty()) slice->_destroy();
  }

  string  Text::_getTypeName () const
  { return _TName( "Text" ); }


  string  Text::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " " + getString(_layer->getName()) );
    s.insert( s.length() - 1, " \"" + _text + "\"" );
    s.insert( s.length() - 1, " @" +  getString(_boundingBox) );
    return s;
  }


  Record* Text::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_cell"      ,  _cell       ) );
      record->add( getSlot("_layer"     ,  _layer      ) );
      record->add( getSlot("_boundinBox", &_boundingBox) );
      record->add( getSlot("_text"      ,  _text       ) );
    }
    return record;
  }


} // Hurricane namespace.

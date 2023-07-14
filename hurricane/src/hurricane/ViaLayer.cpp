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
// |  C++ Module  :  "./ViaLayer.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "ViaLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";

  const char* badCutLayer =
    "ViaLayer::create() [%s]\n"
    "  %s layer is not a cut layer.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::ViaLayer".

  ViaLayer::ViaLayer ( Technology* technology
                     , const Name& name
                     , BasicLayer* bottomLayer
                     , BasicLayer* cutLayer
                     , BasicLayer* topLayer
                     ) : Layer(technology
                              ,name
                              ,0  // minimalSize.
                              ,0) // minmalSpacing.
                       ,_basicLayers()
                       ,_enclosures()
                       ,_maximalEnclosure(0)
  {
    if ( !bottomLayer ) throw Error ( nullLayer, getString(name).c_str(), "Bottom" );
    if ( !cutLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Cut" );
    if ( !topLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Top" );

    if ( cutLayer->getMaterial() != BasicLayer::Material::cut )
      throw Error ( badCutLayer, getString(name).c_str()
                               , getString(cutLayer->getName()).c_str() );

    _basicLayers.reserve ( 3 );
    _enclosures .reserve ( 3 );

    if ( bottomLayer->getMask() > topLayer->getMask() )
      swap ( bottomLayer, topLayer );

    _basicLayers.push_back ( bottomLayer );
    _basicLayers.push_back ( cutLayer );
    _basicLayers.push_back ( topLayer );

    _setMask        (  bottomLayer->getMask()
                     | cutLayer   ->getMask()
                     | topLayer   ->getMask() );
    _setExtractMask (  bottomLayer->getExtractMask()
                     | cutLayer   ->getExtractMask()
                     | topLayer   ->getExtractMask() );

    _enclosures.push_back ( make_pair(0,0) );
    _enclosures.push_back ( make_pair(0,0) );
    _enclosures.push_back ( make_pair(0,0) );
  }


  ViaLayer* ViaLayer::create ( Technology* technology
                             , const Name& name
                             , BasicLayer* bottomLayer
                             , BasicLayer* cutLayer
                             , BasicLayer* topLayer
                             )
  {
    ViaLayer* viaLayer = new ViaLayer ( technology
                                      , name
                                      , bottomLayer
                                      , cutLayer
                                      , topLayer
                                      );

    viaLayer->_postCreate();

    return viaLayer;
  }
 

  BasicLayers ViaLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  const Layer* ViaLayer::getCut () const
  { return getTechnology()->getLayer(_basicLayers[1]->getMask()); } 


  const Layer* ViaLayer::getTop () const
  { return getTechnology()->getLayer(_basicLayers[2]->getMask()); } 


  const Layer* ViaLayer::getBottom () const
  { return getTechnology()->getLayer(_basicLayers[0]->getMask()); } 


  const Layer* ViaLayer::getOpposite ( const Layer* layer ) const
  {
    if ( layer->getMask() == _basicLayers[0]->getMask() ) return getTop();
    if ( layer->getMask() == _basicLayers[2]->getMask() ) return getBottom();
    return NULL;
  } 


  DbU::Unit  ViaLayer::getEnclosure ( uint32_t flags ) const
  {
    if (flags & (Layer::EnclosureH|Layer::EnclosureV)) return _maximalEnclosure;

    DbU::Unit enclosure = 0;
    if (flags & Layer::EnclosureH) {
      for ( auto element : _enclosures ) enclosure = std::max( enclosure, element.first );
    }
    if (flags & Layer::EnclosureV) {
      for ( auto element : _enclosures ) enclosure = std::max( enclosure, element.second );
    }

    return enclosure;
  }


  DbU::Unit  ViaLayer::getEnclosure ( const BasicLayer* layer, uint32_t flags  ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
        
        if (flags & Layer::EnclosureH) {
          if (flags & Layer::EnclosureV) return std::max( _enclosures[i].first, _enclosures[i].second );
          return _enclosures[i].first;
        }
        if (flags & Layer::EnclosureV) return _enclosures[i].second;
      }
    }

    return 0;
  }


  DbU::Unit  ViaLayer::getTopEnclosure ( uint32_t flags  ) const
  {
    if (flags & Layer::EnclosureH) {
      if (flags & Layer::EnclosureV) return std::max( _enclosures[2].first, _enclosures[2].second );
      return _enclosures[2].first;
    }
    if (flags & Layer::EnclosureV) return _enclosures[2].second;
    return 0;
  }


  DbU::Unit  ViaLayer::getBottomEnclosure ( uint32_t flags  ) const
  {
    if (flags & Layer::EnclosureH) {
      if (flags & Layer::EnclosureV) return std::max( _enclosures[0].first, _enclosures[0].second );
      return _enclosures[0].first;
    }
    if (flags & Layer::EnclosureV) return _enclosures[0].second;
    return 0;
  }


  void  ViaLayer::setEnclosure ( const BasicLayer* layer, DbU::Unit enclosure, uint32_t flags )
  {
    _maximalEnclosure = 0;
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
        if (flags & Layer::EnclosureH) _enclosures[i].first  = enclosure;
        if (flags & Layer::EnclosureV) _enclosures[i].second = enclosure;
      }
      _maximalEnclosure = std::max( _maximalEnclosure, _enclosures[i].first  );
      _maximalEnclosure = std::max( _maximalEnclosure, _enclosures[i].second );
    }
  }


  void  ViaLayer::_onDbuChange ( float scale )
  {
    Layer::_onDbuChange( scale );
    for ( size_t i=0 ; i<_enclosures.size() ; i++ ) {
      _enclosures[i].first  = (DbU::Unit)( (float)_enclosures[i].first  * scale );
      _enclosures[i].second = (DbU::Unit)( (float)_enclosures[i].second * scale );
    }
    _maximalEnclosure = (DbU::Unit)( (float)_maximalEnclosure * scale );
  }


  string  ViaLayer::_getTypeName () const
  { return _TName("ViaLayer"); }


  string ViaLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* ViaLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers", &_basicLayers));
      record->add(getSlot("_enclosures" , &_enclosures ));
    }
    return record;
  }


  void  ViaLayer::_toJson ( JsonWriter* w ) const
  {
    Super::_toJson( w );

    jsonWrite( w, "_bottom", _basicLayers[0]->getName() );
    jsonWrite( w, "_cut"   , _basicLayers[1]->getName() );
    jsonWrite( w, "_top"   , _basicLayers[2]->getName() );

    jsonWrite( w, "_enclosureH.bottom", _enclosures[0].first  );
    jsonWrite( w, "_enclosureV.bottom", _enclosures[0].second );
    jsonWrite( w, "_enclosureH.cut"   , _enclosures[1].first  );
    jsonWrite( w, "_enclosureV.cut"   , _enclosures[1].second );
    jsonWrite( w, "_enclosureH.top"   , _enclosures[2].first  );
    jsonWrite( w, "_enclosureV.top"   , _enclosures[2].second );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonViaLayer".

  Initializer<JsonViaLayer>  jsonViaLayerInit ( 0 );


  void  JsonViaLayer::initialize ()
  { JsonTypes::registerType( new JsonViaLayer (JsonWriter::RegisterMode) ); }


  JsonViaLayer::JsonViaLayer ( unsigned long flags )
    : JsonLayer(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonViaLayer::JsonViaLayer()" << endl;

    add( "_bottom"           , typeid(string)  );
    add( "_cut"              , typeid(string)  );
    add( "_top"              , typeid(string)  );
    add( "_enclosureH.bottom", typeid(int64_t) );
    add( "_enclosureV.bottom", typeid(int64_t) );
    add( "_enclosureH.cut"   , typeid(int64_t) );
    add( "_enclosureV.cut"   , typeid(int64_t) );
    add( "_enclosureH.top"   , typeid(int64_t) );
    add( "_enclosureV.top"   , typeid(int64_t) );
  }


  JsonViaLayer::~JsonViaLayer ()
  { }


  string  JsonViaLayer::getTypeName () const
  { return "ViaLayer"; }


  JsonViaLayer* JsonViaLayer::clone ( unsigned long flags ) const
  { return new JsonViaLayer ( flags ); }


  void JsonViaLayer::toData(JsonStack& stack)
  {
    cdebug_tabw(19,1);

    check( stack, "JsonViaLayer::toData" );

    Technology* techno = lookupTechnology( stack, "JsonViaLayer::toData" );
    ViaLayer*   layer  = NULL;

    if (techno) {
      string       name           = get<string> ( stack, "_name"           );
      string       smask          = get<string> ( stack, "_mask"           );
    //DbU::Unit    minimalSize    = get<int64_t>( stack, "_minimalSize"    );
    //DbU::Unit    minimalSpacing = get<int64_t>( stack, "_minimalSpacing" );
      bool         isSymbolic     = get<bool>   ( stack, "_symbolic"       );

      BasicLayer*  bottom         = techno->getBasicLayer( get<string>(stack,"_bottom" ) );
      BasicLayer*  cut            = techno->getBasicLayer( get<string>(stack,"_cut"    ) );
      BasicLayer*  top            = techno->getBasicLayer( get<string>(stack,"_top"    ) );
      DbU::Unit    bottomEnclH    = get<int64_t>( stack, "_enclosureH.bottom" );
      DbU::Unit    bottomEnclV    = get<int64_t>( stack, "_enclosureV.bottom" );
      DbU::Unit    cutEnclH       = get<int64_t>( stack, "_enclosureH.cut"    );
      DbU::Unit    cutEnclV       = get<int64_t>( stack, "_enclosureV.cut"    );
      DbU::Unit    topEnclH       = get<int64_t>( stack, "_enclosureH.top"    );
      DbU::Unit    topEnclV       = get<int64_t>( stack, "_enclosureV.top"    );

      Layer::Mask mask = Layer::Mask::fromString( smask );

      if (stack.issetFlags(JsonWriter::TechnoMode)) {
      // Actual creation.
        layer = ViaLayer::create( techno
                                , name
                                , bottom
                                , cut
                                , top
                                );
        layer->setSymbolic ( isSymbolic );
        layer->setEnclosure( bottom, bottomEnclH , Layer::EnclosureH );
        layer->setEnclosure( bottom, bottomEnclV , Layer::EnclosureV );
        layer->setEnclosure( cut   , cutEnclH    , Layer::EnclosureH );
        layer->setEnclosure( cut   , cutEnclV    , Layer::EnclosureV );
        layer->setEnclosure( top   , topEnclH    , Layer::EnclosureH );
        layer->setEnclosure( top   , topEnclV    , Layer::EnclosureV );

        if (layer->getMask() != mask) {
          cerr << Error( "JsonViaLayer::toData(): Layer mask re-creation discrepency on \"%s\":\n"
                         "        Blob:     %s\n"
                         "        DataBase: %s"
                       , name.c_str()
                       , getString(mask).c_str()
                       , getString(layer->getMask()).c_str()
                       ) << endl;
        }
      // Add here association with blockage layer...
      } else {
      // Check coherency with existing layer.
        layer = dynamic_cast<ViaLayer*>( techno->getLayer(name) );
        if (layer) {
          if (layer->getMask() != mask) {
            cerr << Error( "JsonViaLayer::toData(): Layer mask discrepency on \"%s\":\n"
                           "        Blob:     %s\n"
                           "        DataBase: %s"
                         , name.c_str()
                         , getString(mask).c_str()
                         , getString(layer->getMask()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "JsonViaLayer::toData(): No ViaLayer \"%s\" in the existing technology."
                       , name.c_str()
                       ) << endl;
        }
      }
    } else {
      cerr << Error( "JsonViaLayer::toData(): Cannot find technology, aborting ViaLayer creation." ) << endl;
    }
    
    update( stack, layer );

    cdebug_tabw(19,-1);
  }


} // End of Hurricane namespace.

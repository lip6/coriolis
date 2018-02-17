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
// |  C++ Module  :  "./ContactLayer.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/ContactLayer.h"
#include "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "ContactLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::ContactLayer".


  ContactLayer::ContactLayer ( Technology* technology
                             , const Name& name
                             , BasicLayer* metalLayer
                             , BasicLayer* cutLayer
                             , BasicLayer* activeLayer
                             , BasicLayer* diffusionLayer
                             , BasicLayer* wellLayer
                             ) : Layer(technology
                                      ,name
                                      ,0  // minimalSize.
                                      ,0) // minmalSpacing.
                               ,_basicLayers()
                               ,_enclosures()
                               ,_maximalEnclosure(0)
  {
    if (not metalLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Metal" );
    if (not cutLayer      ) throw Error ( nullLayer, getString(name).c_str(), "Cut" );
    if (not activeLayer   ) throw Error ( nullLayer, getString(name).c_str(), "Active" );
    if (not diffusionLayer) throw Error ( nullLayer, getString(name).c_str(), "Diffusion" );

    _basicLayers.reserve( 5 );
    _enclosures .reserve( 5 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back( metalLayer );
    _basicLayers.push_back( cutLayer );
    _basicLayers.push_back( activeLayer );
    _basicLayers.push_back( diffusionLayer );

    for ( size_t i=0 ; i<4 ; i++ ) _enclosures.push_back( make_pair(0,0) );

    _setMask       (  metalLayer    ->getMask()
                    | cutLayer      ->getMask()
                    | activeLayer   ->getMask()
                    | diffusionLayer->getMask() );
    _setExtractMask(  metalLayer    ->getExtractMask()
                    | cutLayer      ->getExtractMask()
                    | activeLayer   ->getExtractMask()
                    | diffusionLayer->getExtractMask() );

    if (wellLayer) {
      _basicLayers.push_back( wellLayer );
      _enclosures .push_back( make_pair(0,0) );

      _setMask       ( getMask() | wellLayer->getMask()        );
      _setExtractMask( getMask() | wellLayer->getExtractMask() );
    }
  }


  ContactLayer* ContactLayer::create ( Technology* technology
                                     , const Name& name
                                     , BasicLayer* metalLayer
                                     , BasicLayer* cutLayer
                                     , BasicLayer* activeLayer
                                     , BasicLayer* diffusionLayer
                                     , BasicLayer* wellLayer
                                     )
  {
    ContactLayer* transistorLayer = new ContactLayer ( technology
                                                     , name
                                                     , metalLayer
                                                     , cutLayer
                                                     , activeLayer
                                                     , diffusionLayer
                                                     , wellLayer
                                                     );

    transistorLayer->_postCreate();

    return transistorLayer;
  }
 

  BasicLayers ContactLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  DbU::Unit  ContactLayer::getEnclosure ( uint32_t flags ) const
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


  DbU::Unit  ContactLayer::getEnclosure ( const BasicLayer* layer, uint32_t flags ) const
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


  void  ContactLayer::setEnclosure ( const BasicLayer* layer, DbU::Unit enclosure, uint32_t flags )
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


  void  ContactLayer::_onDbuChange ( float scale )
  {
    Layer::_onDbuChange( scale );
    for ( size_t i=0 ; i<_enclosures.size() ; i++ ) {
      _enclosures[i].first  = (DbU::Unit)( (float)_enclosures[i].first  * scale );
      _enclosures[i].second = (DbU::Unit)( (float)_enclosures[i].second * scale );
    }
    _maximalEnclosure = (DbU::Unit)( (float)_maximalEnclosure * scale );
  }


  string  ContactLayer::_getTypeName () const
  { return _TName("ContactLayer"); }


  string ContactLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* ContactLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers", &_basicLayers));
      record->add(getSlot("_enclosures" , &_enclosures ));
    }
    return record;
  }


  void  ContactLayer::_toJson ( JsonWriter* w ) const
  {
    Super::_toJson( w );

    jsonWrite( w, "_metal"    , _basicLayers[0]->getName() );
    jsonWrite( w, "_cut"      , _basicLayers[1]->getName() );
    jsonWrite( w, "_active"   , _basicLayers[2]->getName() );
    jsonWrite( w, "_diffusion", _basicLayers[3]->getName() );
    if (_basicLayers.size() == 5) jsonWrite( w, "_well", _basicLayers[4]->getName() );
    else                          jsonWrite( w, "_well", "no_well_layer" );

    jsonWrite( w, "_enclosureH.metal"    , _enclosures[0].first  );
    jsonWrite( w, "_enclosureV.metal"    , _enclosures[0].second );
    jsonWrite( w, "_enclosureH.cut"      , _enclosures[1].first  );
    jsonWrite( w, "_enclosureV.cut"      , _enclosures[1].second );
    jsonWrite( w, "_enclosureH.active"   , _enclosures[2].first  );
    jsonWrite( w, "_enclosureV.active"   , _enclosures[2].second );
    jsonWrite( w, "_enclosureH.diffusion", _enclosures[3].first  );
    jsonWrite( w, "_enclosureV.diffusion", _enclosures[3].second );
    jsonWrite( w, "_enclosureH.well"     , (_basicLayers.size() == 5) ? _enclosures[4].first  : 0 );
    jsonWrite( w, "_enclosureV.well"     , (_basicLayers.size() == 5) ? _enclosures[4].second : 0 );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonContactLayer".

  Initializer<JsonContactLayer>  jsonContactLayerInit ( 0 );


  void  JsonContactLayer::initialize ()
  { JsonTypes::registerType( new JsonContactLayer (JsonWriter::RegisterMode) ); }


  JsonContactLayer::JsonContactLayer ( unsigned long flags )
    : JsonLayer(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonContactLayer::JsonContactLayer()" << endl;

    add( "_metal"               , typeid(string)  );
    add( "_cut"                 , typeid(string)  );
    add( "_active"              , typeid(string)  );
    add( "_diffusion"           , typeid(string)  );
    add( "_well"                , typeid(string)  );
    add( "_enclosureH.metal"    , typeid(int64_t) );
    add( "_enclosureV.metal"    , typeid(int64_t) );
    add( "_enclosureH.cut"      , typeid(int64_t) );
    add( "_enclosureV.cut"      , typeid(int64_t) );
    add( "_enclosureH.active"   , typeid(int64_t) );
    add( "_enclosureV.active"   , typeid(int64_t) );
    add( "_enclosureH.diffusion", typeid(int64_t) );
    add( "_enclosureV.diffusion", typeid(int64_t) );
    add( "_enclosureH.well"     , typeid(int64_t) );
    add( "_enclosureV.well"     , typeid(int64_t) );
  }


  JsonContactLayer::~JsonContactLayer ()
  { }


  string  JsonContactLayer::getTypeName () const
  { return "ContactLayer"; }


  JsonContactLayer* JsonContactLayer::clone ( unsigned long flags ) const
  { return new JsonContactLayer ( flags ); }


  void JsonContactLayer::toData(JsonStack& stack)
  {
    cdebug_tabw(19,1);

    check( stack, "JsonContactLayer::toData" );

    Technology*   techno = lookupTechnology( stack, "JsonContactLayer::toData" );
    ContactLayer* layer  = NULL;

    if (techno) {
      string       name           = get<string> ( stack, "_name"           );
      string       smask          = get<string> ( stack, "_mask"           );
    //DbU::Unit    minimalSize    = get<int64_t>( stack, "_minimalSize"    );
    //DbU::Unit    minimalSpacing = get<int64_t>( stack, "_minimalSpacing" );
      bool         isSymbolic     = get<bool>   ( stack, "_symbolic"       );

      BasicLayer*  metal          = techno->getBasicLayer( get<string>(stack,"_metal"    ) );
      BasicLayer*  cut            = techno->getBasicLayer( get<string>(stack,"_cut"      ) );
      BasicLayer*  active         = techno->getBasicLayer( get<string>(stack,"_active"   ) );
      BasicLayer*  diffusion      = techno->getBasicLayer( get<string>(stack,"_diffusion") );
      BasicLayer*  well           = techno->getBasicLayer( get<string>(stack,"_well"     ) );
      DbU::Unit    metalEnclH     = get<int64_t>( stack, "_enclosureH.metal"     );
      DbU::Unit    metalEnclV     = get<int64_t>( stack, "_enclosureV.metal"     );
      DbU::Unit    cutEnclH       = get<int64_t>( stack, "_enclosureH.cut"       );
      DbU::Unit    cutEnclV       = get<int64_t>( stack, "_enclosureV.cut"       );
      DbU::Unit    activeEnclH    = get<int64_t>( stack, "_enclosureH.active"    );
      DbU::Unit    activeEnclV    = get<int64_t>( stack, "_enclosureV.active"    );
      DbU::Unit    diffusionEnclH = get<int64_t>( stack, "_enclosureH.diffusion" );
      DbU::Unit    diffusionEnclV = get<int64_t>( stack, "_enclosureV.diffusion" );
      DbU::Unit    wellEnclH      = get<int64_t>( stack, "_enclosureH.well"      );
      DbU::Unit    wellEnclV      = get<int64_t>( stack, "_enclosureV.well"      );

      Layer::Mask mask = Layer::Mask::fromString( smask );

      if (stack.issetFlags(JsonWriter::TechnoMode)) {
      // Actual creation.
        layer = ContactLayer::create( techno
                                    , name
                                    , metal
                                    , cut
                                    , active
                                    , diffusion
                                    , well
                                    );
        layer->setSymbolic ( isSymbolic );
        layer->setEnclosure( metal    , metalEnclH    , Layer::EnclosureH );
        layer->setEnclosure( metal    , metalEnclV    , Layer::EnclosureV );
        layer->setEnclosure( cut      , cutEnclH      , Layer::EnclosureH );
        layer->setEnclosure( cut      , cutEnclV      , Layer::EnclosureV );
        layer->setEnclosure( active   , activeEnclH   , Layer::EnclosureH );
        layer->setEnclosure( active   , activeEnclV   , Layer::EnclosureV );
        layer->setEnclosure( diffusion, diffusionEnclH, Layer::EnclosureH );
        layer->setEnclosure( diffusion, diffusionEnclV, Layer::EnclosureV );
        if (well) {
          layer->setEnclosure( well, wellEnclH, Layer::EnclosureH );
          layer->setEnclosure( well, wellEnclV, Layer::EnclosureV );
        }

        if (layer->getMask() != mask) {
          cerr << Error( "JsonContactLayer::toData(): Layer mask re-creation discrepency on \"%s\":\n"
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
        layer = dynamic_cast<ContactLayer*>( techno->getLayer(name) );
        if (layer) {
          if (layer->getMask() != mask) {
            cerr << Error( "JsonContactLayer::toData(): Layer mask discrepency on \"%s\":\n"
                           "        Blob:     %s\n"
                           "        DataBase: %s"
                         , name.c_str()
                         , getString(mask).c_str()
                         , getString(layer->getMask()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "JsonContactLayer::toData(): No ContactLayer \"%s\" in the existing technology."
                       , name.c_str()
                       ) << endl;
        }
      }
    } else {
      cerr << Error( "JsonContactLayer::toData(): Cannot find technology, aborting ContactLayer creation." ) << endl;
    }
    
    update( stack, layer );

    cdebug_tabw(19,-1);
  }


} // End of Hurricane namespace.

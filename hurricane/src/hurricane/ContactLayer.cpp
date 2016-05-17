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
    if ( !metalLayer     ) throw Error ( nullLayer, getString(name).c_str(), "Metal" );
    if ( !cutLayer       ) throw Error ( nullLayer, getString(name).c_str(), "Cut" );
    if ( !activeLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Active" );
    if ( !diffusionLayer ) throw Error ( nullLayer, getString(name).c_str(), "Diffusion" );

    _basicLayers.reserve ( 5 );
    _enclosures .reserve ( 5 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back ( metalLayer );
    _basicLayers.push_back ( cutLayer );
    _basicLayers.push_back ( activeLayer );
    _basicLayers.push_back ( diffusionLayer );

    for ( size_t i=0 ; i<4 ; i++ ) {
      _enclosures.push_back ( 0 );
    }

    _setMask        (  metalLayer    ->getMask()
                     | cutLayer      ->getMask()
                     | activeLayer   ->getMask()
                     | diffusionLayer->getMask() );
    _setExtractMask (  metalLayer    ->getExtractMask()
                     | cutLayer      ->getExtractMask()
                     | activeLayer   ->getExtractMask()
                     | diffusionLayer->getExtractMask() );

    if ( wellLayer ) {
      _basicLayers.push_back ( wellLayer );
      _enclosures .push_back ( 0 );

      _setMask        ( getMask() | wellLayer->getMask()        );
      _setExtractMask ( getMask() | wellLayer->getExtractMask() );
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


  DbU::Unit  ContactLayer::getEnclosure () const
  { return _maximalEnclosure; }


  DbU::Unit  ContactLayer::getEnclosure ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer )
        return _enclosures[i];
    }

    return 0;
  }


  void  ContactLayer::setEnclosure ( const BasicLayer* layer, DbU::Unit enclosure )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
        _enclosures[i] = enclosure;
        _maximalEnclosure = max ( _maximalEnclosure, enclosure );
      }
    }
  }


  void  ContactLayer::_onDbuChange ( float scale )
  {
    Layer::_onDbuChange ( scale );
    for ( size_t i=0 ; i<_enclosures.size() ; i++ )
      _enclosures[i] = (DbU::Unit)( (float)_enclosures[i] * scale );
    _maximalEnclosure   = (DbU::Unit)( (float)_maximalEnclosure * scale );
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

    jsonWrite( w, "_enclosure.metal"    , _enclosures[0] );
    jsonWrite( w, "_enclosure.cut"      , _enclosures[1] );
    jsonWrite( w, "_enclosure.active"   , _enclosures[2] );
    jsonWrite( w, "_enclosure.diffusion", _enclosures[3] );
    jsonWrite( w, "_enclosure.well"     , (_basicLayers.size() == 5) ? _enclosures[4] : 0 );
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

    cdebug.log(19) << "JsonContactLayer::JsonContactLayer()" << endl;

    add( "_metal"              , typeid(string)  );
    add( "_cut"                , typeid(string)  );
    add( "_active"             , typeid(string)  );
    add( "_diffusion"          , typeid(string)  );
    add( "_well"               , typeid(string)  );
    add( "_enclosure.metal"    , typeid(int64_t) );
    add( "_enclosure.cut"      , typeid(int64_t) );
    add( "_enclosure.active"   , typeid(int64_t) );
    add( "_enclosure.diffusion", typeid(int64_t) );
    add( "_enclosure.well"     , typeid(int64_t) );
  }


  JsonContactLayer::~JsonContactLayer ()
  { }


  string  JsonContactLayer::getTypeName () const
  { return "ContactLayer"; }


  JsonContactLayer* JsonContactLayer::clone ( unsigned long flags ) const
  { return new JsonContactLayer ( flags ); }


  void JsonContactLayer::toData(JsonStack& stack)
  {
    cdebug.tabw(19,1);

    check( stack, "JsonContactLayer::toData" );

    Technology*   techno = lookupTechnology( stack, "JsonContactLayer::toData" );
    ContactLayer* layer  = NULL;

    if (techno) {
      string       name           = get<string> ( stack, "_name"           );
      string       smask          = get<string> ( stack, "_mask"           );
    //DbU::Unit    minimalSize    = get<int64_t>( stack, "_minimalSize"    );
    //DbU::Unit    minimalSpacing = get<int64_t>( stack, "_minimalSpacing" );
      bool         isWorking      = get<bool>   ( stack, "_working"        );

      BasicLayer*  metal          = techno->getBasicLayer( get<string>(stack,"_metal"    ) );
      BasicLayer*  cut            = techno->getBasicLayer( get<string>(stack,"_cut"      ) );
      BasicLayer*  active         = techno->getBasicLayer( get<string>(stack,"_active"   ) );
      BasicLayer*  diffusion      = techno->getBasicLayer( get<string>(stack,"_diffusion") );
      BasicLayer*  well           = techno->getBasicLayer( get<string>(stack,"_well"     ) );
      DbU::Unit    metalEncl      = get<int64_t>( stack, "_enclosure.metal"     );
      DbU::Unit    cutEncl        = get<int64_t>( stack, "_enclosure.cut"       );
      DbU::Unit    activeEncl     = get<int64_t>( stack, "_enclosure.active"    );
      DbU::Unit    diffusionEncl  = get<int64_t>( stack, "_enclosure.diffusion" );
      DbU::Unit    wellEncl       = get<int64_t>( stack, "_enclosure.well"      );

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
        layer->setWorking  ( isWorking );
        layer->setEnclosure( metal    , metalEncl     );
        layer->setEnclosure( cut      , cutEncl       );
        layer->setEnclosure( active   , activeEncl    );
        layer->setEnclosure( diffusion, diffusionEncl );
        if (well) layer->setEnclosure( well, wellEncl );

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

    cdebug.tabw(19,-1);
  }


} // End of Hurricane namespace.

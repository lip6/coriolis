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
// |  C++ Module  :  "./TransistorLayer.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/TransistorLayer.h"
#include "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "TransistorLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::TransistorLayer".


  TransistorLayer::TransistorLayer ( Technology* technology
                                   , const Name& name
                                   , BasicLayer* gateLayer
                                   , BasicLayer* activeLayer
                                   , BasicLayer* diffusionLayer
                                   , BasicLayer* wellLayer
                                   ) : Layer(technology
                                            ,name
                                            ,0  // minimalSize.
                                            ,0) // minmalSpacing.
                                     ,_basicLayers()
                                     ,_extentionCaps()
                                     ,_extentionWidths()
                                     ,_maximalExtentionCap(0)
                                     ,_maximalExtentionWidth(0)
  {
    if ( !gateLayer      ) throw Error ( nullLayer, getString(name).c_str(), "Gate" );
    if ( !activeLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Active" );
    if ( !diffusionLayer ) throw Error ( nullLayer, getString(name).c_str(), "Diffusion" );

    _basicLayers    .reserve ( 4 );
    _extentionCaps  .reserve ( 4 );
    _extentionWidths.reserve ( 4 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back ( gateLayer );
    _basicLayers.push_back ( activeLayer );
    _basicLayers.push_back ( diffusionLayer );

    for ( size_t i=0 ; i<3 ; i++ ) {
      _extentionCaps  .push_back ( 0 );
      _extentionWidths.push_back ( 0 );
    }

    _setMask        (  gateLayer     ->getMask()
                     | activeLayer   ->getMask()
                     | diffusionLayer->getMask() );
    _setExtractMask (  gateLayer     ->getExtractMask()
                     | activeLayer   ->getExtractMask()
                     | diffusionLayer->getExtractMask() );

    if ( wellLayer ) {
      _basicLayers    .push_back ( wellLayer );
      _extentionCaps  .push_back ( 0 );
      _extentionWidths.push_back ( 0 );

      _setMask        ( getMask() | wellLayer->getMask()        );
      _setExtractMask ( getMask() | wellLayer->getExtractMask() );
    }
  }


  TransistorLayer* TransistorLayer::create ( Technology* technology
                                           , const Name& name
                                           , BasicLayer* gateLayer
                                           , BasicLayer* activeLayer
                                           , BasicLayer* diffusionLayer
                                           , BasicLayer* wellLayer
                                           )
  {
    TransistorLayer* transistorLayer = new TransistorLayer ( technology
                                                           , name
                                                           , gateLayer
                                                           , activeLayer
                                                           , diffusionLayer
                                                           , wellLayer
                                                           );

    transistorLayer->_postCreate();

    return transistorLayer;
  }
 

  BasicLayers TransistorLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  DbU::Unit  TransistorLayer::getExtentionCap () const
  { return _maximalExtentionCap; }


  DbU::Unit  TransistorLayer::getExtentionWidth () const
  { return _maximalExtentionWidth; }


  DbU::Unit  TransistorLayer::getExtentionCap ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer )
        return _extentionCaps[i];
    }

    return 0;
  }


  DbU::Unit  TransistorLayer::getExtentionWidth ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer )
        return _extentionWidths[i];
    }

    return 0;
  }


  void  TransistorLayer::setExtentionCap ( const BasicLayer* layer, DbU::Unit cap )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
        _extentionCaps[i] = cap;
        _maximalExtentionCap = max ( _maximalExtentionCap, cap );
      }
    }
  }


  void  TransistorLayer::setExtentionWidth ( const BasicLayer* layer, DbU::Unit width )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer )
        _extentionWidths[i] = width;
        _maximalExtentionWidth = max ( _maximalExtentionWidth, width );
    }
  }


  void  TransistorLayer::_onDbuChange ( float scale )
  {
    Layer::_onDbuChange ( scale );

    for ( size_t i=0 ; i<_extentionCaps.size() ; i++ )
      _extentionCaps[i] = (DbU::Unit)( (float)_extentionCaps[i] * scale );

    for ( size_t i=0 ; i<_extentionWidths.size() ; i++ )
      _extentionWidths[i] = (DbU::Unit)( (float)_extentionWidths[i] * scale );

    _maximalExtentionCap   = (DbU::Unit)( (float)_maximalExtentionCap   * scale );
    _maximalExtentionWidth = (DbU::Unit)( (float)_maximalExtentionWidth * scale );
  }


  string  TransistorLayer::_getTypeName () const
  { return _TName("TransistorLayer"); }


  string TransistorLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* TransistorLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers"    , &_basicLayers    ));
      record->add(getSlot("_extentionCaps"  , &_extentionCaps  ));
      record->add(getSlot("_extentionWidths", &_extentionWidths));
    }
    return record;
  }


  void  TransistorLayer::_toJson ( JsonWriter* w ) const
  {
    Super::_toJson( w );

    jsonWrite( w, "_gate"     , _basicLayers[0]->getName() );
    jsonWrite( w, "_active"   , _basicLayers[1]->getName() );
    jsonWrite( w, "_diffusion", _basicLayers[2]->getName() );
    if (_basicLayers.size() == 4) jsonWrite( w, "_well"   , _basicLayers[3]->getName() );
    else                          jsonWrite( w, "_well", "no_well_layer" );

    jsonWrite( w, "_extentionCap.gate"     , _extentionCaps[0] );
    jsonWrite( w, "_extentionCap.active"   , _extentionCaps[1] );
    jsonWrite( w, "_extentionCap.diffusion", _extentionCaps[2] );
    jsonWrite( w, "_extentionCap.well"     , (_basicLayers.size() == 4) ? _extentionCaps[3] : 0 );

    jsonWrite( w, "_extentionWidth.gate"     , _extentionWidths[0] );
    jsonWrite( w, "_extentionWidth.active"   , _extentionWidths[1] );
    jsonWrite( w, "_extentionWidth.diffusion", _extentionWidths[2] );
    jsonWrite( w, "_extentionWidth.well"   , (_basicLayers.size() == 4) ? _extentionWidths[3] : 0 );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonTransistorLayer".

  Initializer<JsonTransistorLayer>  jsonTransistorLayerInit ( 0 );


  void  JsonTransistorLayer::initialize ()
  { JsonTypes::registerType( new JsonTransistorLayer (JsonWriter::RegisterMode) ); }


  JsonTransistorLayer::JsonTransistorLayer ( unsigned long flags )
    : JsonLayer(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonTransistorLayer::JsonTransistorLayer()" << endl;

    add( "_gate"                    , typeid(string)  );
    add( "_active"                  , typeid(string)  );
    add( "_diffusion"               , typeid(string)  );
    add( "_well"                    , typeid(string)  );
    add( "_extentionCap.gate"       , typeid(int64_t) );
    add( "_extentionCap.active"     , typeid(int64_t) );
    add( "_extentionCap.diffusion"  , typeid(int64_t) );
    add( "_extentionCap.well"       , typeid(int64_t) );
    add( "_extentionWidth.gate"     , typeid(int64_t) );
    add( "_extentionWidth.active"   , typeid(int64_t) );
    add( "_extentionWidth.diffusion", typeid(int64_t) );
    add( "_extentionWidth.well"     , typeid(int64_t) );
  }


  JsonTransistorLayer::~JsonTransistorLayer ()
  { }


  string  JsonTransistorLayer::getTypeName () const
  { return "TransistorLayer"; }


  JsonTransistorLayer* JsonTransistorLayer::clone ( unsigned long flags ) const
  { return new JsonTransistorLayer ( flags ); }


  void JsonTransistorLayer::toData ( JsonStack& stack )
  {
    cdebug_tabw(19,1);

    check( stack, "JsonTransistorLayer::toData" );

    Technology*      techno = lookupTechnology( stack, "JsonTransistorLayer::toData" );
    TransistorLayer* layer  = NULL;

    if (techno) {
      string       name             = get<string> ( stack, "_name"           );
      string       smask            = get<string> ( stack, "_mask"           );
    //DbU::Unit    minimalSize      = get<int64_t>( stack, "_minimalSize"    );
    //DbU::Unit    minimalSpacing   = get<int64_t>( stack, "_minimalSpacing" );
      bool         isWorking        = get<bool>   ( stack, "_working"        );
                                    
      BasicLayer*  gate             = techno->getBasicLayer( get<string>(stack,"_gate"      ) );
      BasicLayer*  active           = techno->getBasicLayer( get<string>(stack,"_active"    ) );
      BasicLayer*  diffusion        = techno->getBasicLayer( get<string>(stack,"_diffusion" ) );
      BasicLayer*  well             = techno->getBasicLayer( get<string>(stack,"_well"      ) );
      DbU::Unit    eCapGate         = get<int64_t>( stack, "_extentionCap.gate"        );
      DbU::Unit    eCapActive       = get<int64_t>( stack, "_extentionCap.active"      );
      DbU::Unit    eCapDiffusion    = get<int64_t>( stack, "_extentionCap.diffusion"   );
      DbU::Unit    eCapWell         = get<int64_t>( stack, "_extentionCap.well"        );
      DbU::Unit    eWidthGate       = get<int64_t>( stack, "_extentionWidth.gate"      );
      DbU::Unit    eWidthActive     = get<int64_t>( stack, "_extentionWidth.active"    );
      DbU::Unit    eWidthDiffusion  = get<int64_t>( stack, "_extentionWidth.diffusion" );
      DbU::Unit    eWidthWell       = get<int64_t>( stack, "_extentionWidth.well"      );

      Layer::Mask mask = Layer::Mask::fromString( smask );

      if (stack.issetFlags(JsonWriter::TechnoMode)) {
      // Actual creation.
        layer = TransistorLayer::create( techno
                                       , name
                                       , gate
                                       , active
                                       , diffusion
                                       , well
                                       );
        layer->setWorking       ( isWorking );
        layer->setExtentionCap  ( gate     , eCapGate        );
        layer->setExtentionCap  ( active   , eCapActive      );
        layer->setExtentionCap  ( diffusion, eCapDiffusion   );
        layer->setExtentionWidth( gate     , eWidthGate      );
        layer->setExtentionWidth( active   , eWidthActive    );
        layer->setExtentionWidth( diffusion, eWidthDiffusion );
        if (well) {
          layer->setExtentionCap  ( well, eCapWell   );
          layer->setExtentionWidth( well, eWidthWell );
        }

        if (layer->getMask() != mask) {
          cerr << Error( "JsonTransistorLayer::toData(): Layer mask re-creation discrepency on \"%s\":\n"
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
        layer = dynamic_cast<TransistorLayer*>( techno->getLayer(name) );
        if (layer) {
          if (layer->getMask() != mask) {
            cerr << Error( "JsonTransistorLayer::toData(): Layer mask discrepency on \"%s\":\n"
                           "        Blob:     %s\n"
                           "        DataBase: %s"
                         , name.c_str()
                         , getString(mask).c_str()
                         , getString(layer->getMask()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "JsonTransistorLayer::toData(): No TransistorLayer \"%s\" in the existing technology."
                       , name.c_str()
                       ) << endl;
        }
      }
    } else {
      cerr << Error( "JsonTransistorLayer::toData(): Cannot find technology, aborting TransistorLayer creation." ) << endl;
    }
    
    update( stack, layer );

    cdebug_tabw(19,-1);
  }


} // End of Hurricane namespace.

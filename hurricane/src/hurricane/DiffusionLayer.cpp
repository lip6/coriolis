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
// |  C++ Module  :  "./DiffusionLayer.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/DiffusionLayer.h"
#include "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "DiffusionLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::DiffusionLayer".


  DiffusionLayer::DiffusionLayer ( Technology* technology
                                 , const Name& name
                                 , BasicLayer* activeLayer
                                 , BasicLayer* diffusionLayer
                                 , BasicLayer* wellLayer
                                 ) : Layer(technology
                                          ,name
                                          ,0  // minimalSize.
                                          ,0) // minimalSpacing.
                                   ,_basicLayers()
                                   ,_extentionCaps()
                                   ,_extentionWidths()
                                   ,_maximalExtentionCap(0)
                                   ,_maximalExtentionWidth(0)
  {
    if ( !activeLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Active" );
    if ( !diffusionLayer ) throw Error ( nullLayer, getString(name).c_str(), "Diffusion" );

    _basicLayers    .reserve ( 3 );
    _extentionCaps  .reserve ( 3 );
    _extentionWidths.reserve ( 3 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back ( activeLayer );
    _basicLayers.push_back ( diffusionLayer );

    for ( size_t i=0 ; i<2 ; i++ ) {
      _extentionCaps  .push_back ( 0 );
      _extentionWidths.push_back ( 0 );
    }

    _setMask        ( activeLayer->getMask()        | diffusionLayer->getMask()        );
    _setExtractMask ( activeLayer->getExtractMask() | diffusionLayer->getExtractMask() );

    if ( wellLayer ) {
      _basicLayers    .push_back ( wellLayer );
      _extentionCaps  .push_back ( 0 );
      _extentionWidths.push_back ( 0 );

      _setMask        ( getMask() | wellLayer->getMask()        );
      _setExtractMask ( getMask() | wellLayer->getExtractMask() );
    }
  }


  DiffusionLayer* DiffusionLayer::create ( Technology* technology
                                         , const Name& name
                                         , BasicLayer* activeLayer
                                         , BasicLayer* diffusionLayer
                                         , BasicLayer* wellLayer
                                         )
  {
    DiffusionLayer* transistorLayer = new DiffusionLayer ( technology
                                                         , name
                                                         , activeLayer
                                                         , diffusionLayer
                                                         , wellLayer
                                                         );

    transistorLayer->_postCreate();

    return transistorLayer;
  }
 

  BasicLayers DiffusionLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  DbU::Unit  DiffusionLayer::getExtentionCap () const
  { return _maximalExtentionCap; }


  DbU::Unit  DiffusionLayer::getExtentionWidth () const
  { return _maximalExtentionWidth; }


  DbU::Unit  DiffusionLayer::getExtentionCap ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "getExtentionCap() - " << this << " on: " << layer << " " << _extentionCaps[i] << endl;;
        return _extentionCaps[i];
      }
    }

    return 0;
  }


  DbU::Unit  DiffusionLayer::getExtentionWidth ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "getExtentionWith() - " << this << " on: " << layer << " " << _extentionWidths[i] << endl;;
        return _extentionWidths[i];
      }
    }

    return 0;
  }


  void  DiffusionLayer::setExtentionCap ( const BasicLayer* layer, DbU::Unit cap )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "setExtentionCap() - " << getName() << " " << layer << " " << cap << endl;
        _extentionCaps[i] = cap;
        _maximalExtentionCap = max ( _maximalExtentionCap, cap );
      }
    }
  }


  void  DiffusionLayer::setExtentionWidth ( const BasicLayer* layer, DbU::Unit width )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "setExtentionWidth() - " << getName() << " " << layer << " " << width << endl;
        _extentionWidths[i] = width;
        _maximalExtentionWidth = max ( _maximalExtentionWidth, width );
      }
    }
  }


  void  DiffusionLayer::_onDbuChange ( float scale )
  {
    Layer::_onDbuChange ( scale );

    for ( size_t i=0 ; i<_extentionCaps.size() ; i++ )
      _extentionCaps[i] = (DbU::Unit)( (float)_extentionCaps[i] * scale );

    for ( size_t i=0 ; i<_extentionWidths.size() ; i++ )
      _extentionWidths[i] = (DbU::Unit)( (float)_extentionWidths[i] * scale );

    _maximalExtentionCap   = (DbU::Unit)( (float)_maximalExtentionCap   * scale );
    _maximalExtentionWidth = (DbU::Unit)( (float)_maximalExtentionWidth * scale );
  }


  string  DiffusionLayer::_getTypeName () const
  { return _TName("DiffusionLayer"); }


  string DiffusionLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* DiffusionLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers"    , &_basicLayers    ));
      record->add(getSlot("_extentionCaps"  , &_extentionCaps  ));
      record->add(getSlot("_extentionWidths", &_extentionWidths));
    }
    return record;
  }


  void  DiffusionLayer::_toJson ( JsonWriter* w ) const
  {
    Super::_toJson( w );

    jsonWrite( w, "_active"   , _basicLayers[0]->getName() );
    jsonWrite( w, "_diffusion", _basicLayers[1]->getName() );
    if (_basicLayers.size() == 3) jsonWrite( w, "_well", _basicLayers[2]->getName() );
    else                          jsonWrite( w, "_well", "no_well_layer" );

    jsonWrite( w, "_extentionCap.active"   , _extentionCaps[0] );
    jsonWrite( w, "_extentionCap.diffusion", _extentionCaps[1] );
    jsonWrite( w, "_extentionCap.well"     , (_basicLayers.size() == 3) ? _extentionCaps[2] : 0 );

    jsonWrite( w, "_extentionWidth.active"   , _extentionWidths[0] );
    jsonWrite( w, "_extentionWidth.diffusion", _extentionWidths[1] );
    jsonWrite( w, "_extentionWidth.well"     , (_basicLayers.size() == 3) ? _extentionWidths[2] : 0 );
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonDiffusionLayer".

  Initializer<JsonDiffusionLayer>  jsonDiffusionLayerInit ( 0 );


  void  JsonDiffusionLayer::initialize ()
  { JsonTypes::registerType( new JsonDiffusionLayer (JsonWriter::RegisterMode) ); }


  JsonDiffusionLayer::JsonDiffusionLayer ( unsigned long flags )
    : JsonLayer(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonDiffusionLayer::JsonDiffusionLayer()" << endl;

    add( "_active"                  , typeid(string)  );
    add( "_diffusion"               , typeid(string)  );
    add( "_well"                    , typeid(string)  );
    add( "_extentionCap.active"     , typeid(int64_t) );
    add( "_extentionCap.diffusion"  , typeid(int64_t) );
    add( "_extentionCap.well"       , typeid(int64_t) );
    add( "_extentionWidth.active"   , typeid(int64_t) );
    add( "_extentionWidth.diffusion", typeid(int64_t) );
    add( "_extentionWidth.well"     , typeid(int64_t) );
  }


  JsonDiffusionLayer::~JsonDiffusionLayer ()
  { }


  string  JsonDiffusionLayer::getTypeName () const
  { return "DiffusionLayer"; }


  JsonDiffusionLayer* JsonDiffusionLayer::clone ( unsigned long flags ) const
  { return new JsonDiffusionLayer ( flags ); }


  void JsonDiffusionLayer::toData(JsonStack& stack)
  {
    cdebug_tabw(19,1);

    check( stack, "JsonDiffusionLayer::toData" );

    Technology*     techno = lookupTechnology( stack, "JsonDiffusionLayer::toData" );
    DiffusionLayer* layer  = NULL;

    if (techno) {
      string       name             = get<string> ( stack, "_name"           );
      string       smask            = get<string> ( stack, "_mask"           );
    //DbU::Unit    minimalSize      = get<int64_t>( stack, "_minimalSize"    );
    //DbU::Unit    minimalSpacing   = get<int64_t>( stack, "_minimalSpacing" );
      bool         isSymbolic       = get<bool>   ( stack, "_symbolic"       );
                                    
      BasicLayer*  active           = techno->getBasicLayer( get<string>(stack,"_active"    ) );
      BasicLayer*  diffusion        = techno->getBasicLayer( get<string>(stack,"_diffusion" ) );
      BasicLayer*  well             = techno->getBasicLayer( get<string>(stack,"_well"      ) );
      DbU::Unit    eCapActive       = get<int64_t>( stack, "_extentionCap.active"      );
      DbU::Unit    eCapDiffusion    = get<int64_t>( stack, "_extentionCap.diffusion"   );
      DbU::Unit    eCapWell         = get<int64_t>( stack, "_extentionCap.well"        );
      DbU::Unit    eWidthActive     = get<int64_t>( stack, "_extentionWidth.active"    );
      DbU::Unit    eWidthDiffusion  = get<int64_t>( stack, "_extentionWidth.diffusion" );
      DbU::Unit    eWidthWell       = get<int64_t>( stack, "_extentionWidth.well"      );

      Layer::Mask mask = Layer::Mask::fromString( smask );

      if (stack.issetFlags(JsonWriter::TechnoMode)) {
      // Actual creation.
        layer = DiffusionLayer::create( techno
                                      , name
                                      , active
                                      , diffusion
                                      , well
                                      );
        layer->setSymbolic      ( isSymbolic );
        layer->setExtentionCap  ( active   , eCapActive      );
        layer->setExtentionCap  ( diffusion, eCapDiffusion   );
        layer->setExtentionWidth( active   , eWidthActive    );
        layer->setExtentionWidth( diffusion, eWidthDiffusion );
        if (well) {
          layer->setExtentionCap  ( well, eCapWell   );
          layer->setExtentionWidth( well, eWidthWell );
        }

        if (layer->getMask() != mask) {
          cerr << Error( "JsonDiffusionLayer::toData(): Layer mask re-creation discrepency on \"%s\":\n"
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
        layer = dynamic_cast<DiffusionLayer*>( techno->getLayer(name) );
        if (layer) {
          if (layer->getMask() != mask) {
            cerr << Error( "JsonDiffusionLayer::toData(): Layer mask discrepency on \"%s\":\n"
                           "        Blob:     %s\n"
                           "        DataBase: %s"
                         , name.c_str()
                         , getString(mask).c_str()
                         , getString(layer->getMask()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "JsonDiffusionLayer::toData(): No DiffusionLayer \"%s\" in the existing technology."
                       , name.c_str()
                       ) << endl;
        }
      }
    } else {
      cerr << Error( "JsonDiffusionLayer::toData(): Cannot find technology, aborting DiffusionLayer creation." ) << endl;
    }
    
    update( stack, layer );

    cdebug_tabw(19,-1);
  }


} // End of Hurricane namespace.


// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DiffusionLayer.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/BasicLayer.h"
#include  "hurricane/DiffusionLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Error.h"


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
                                          ,0) // minmalSpacing.
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


} // End of Hurricane namespace.



// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Module  :       "./DiffusionLayer.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "hurricane/BasicLayer.h"
# include  "hurricane/DiffusionLayer.h"
# include  "hurricane/Technology.h"
# include  "hurricane/Error.h"


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


  Unit  DiffusionLayer::getExtentionCap () const
  { return _maximalExtentionCap; }


  Unit  DiffusionLayer::getExtentionWidth () const
  { return _maximalExtentionWidth; }


  Unit  DiffusionLayer::getExtentionCap ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "getExtentionCap() - " << this << " on: " << layer << " " << _extentionCaps[i] << endl;;
        return _extentionCaps[i];
      }
    }

    return 0;
  }


  Unit  DiffusionLayer::getExtentionWidth ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "getExtentionWith() - " << this << " on: " << layer << " " << _extentionWidths[i] << endl;;
        return _extentionWidths[i];
      }
    }

    return 0;
  }


  void  DiffusionLayer::setExtentionCap ( const BasicLayer* layer, Unit cap )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "setExtentionCap() - " << getName() << " " << layer << " " << cap << endl;
        _extentionCaps[i] = cap;
        _maximalExtentionCap = max ( _maximalExtentionCap, cap );
      }
    }
  }


  void  DiffusionLayer::setExtentionWidth ( const BasicLayer* layer, Unit width )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
      //cerr << "setExtentionWidth() - " << getName() << " " << layer << " " << width << endl;
        _extentionWidths[i] = width;
        _maximalExtentionWidth = max ( _maximalExtentionWidth, width );
      }
    }
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

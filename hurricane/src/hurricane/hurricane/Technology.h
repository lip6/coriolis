
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Technology.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_TECHNOLOGY__
#define  __HURRICANE_TECHNOLOGY__

#include  <map>
#include  "hurricane/Mask.h"
#include  "hurricane/DBo.h"
#include  "hurricane/Layer.h"
#include  "hurricane/BasicLayers.h"
#include  "hurricane/RegularLayers.h"
#include  "hurricane/ViaLayers.h"
#include  "hurricane/IntrusiveMap.h"


namespace Hurricane {

  using std::multimap;

  class DataBase;
  class BasicLayer;
  class RegularLayer;
  class ViaLayer;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology".


  class Technology : public DBo {

    public:
      typedef DBo Inherit;
      typedef multimap<Layer::Mask,Layer*> LayerMaskMap;

    public:
    // Sub-class : LayerMap.
      class LayerMap : public IntrusiveMap<Name,Layer> {
        public:
          typedef IntrusiveMap<Name,Layer> Inherit;
        public:
                            LayerMap        ();
          virtual Name      _getKey         ( Layer* ) const;
          virtual unsigned  _getHashValue   ( Name ) const;
          virtual Layer*    _getNextElement ( Layer* ) const;
          virtual void      _setNextElement ( Layer* , Layer* nextLayer) const;
      };

    public:
    // Constructor.
      static  Technology*    create                  ( DataBase* , const Name& );
    // Accessors.
      inline  bool           isMetal                 ( const Layer* ) const;
      inline  DataBase*      getDataBase             () const;
      inline  const Name&    getName                 () const;
      inline  Layer*         getLayer                ( const Name& ) const;
              BasicLayer*    getBasicLayer           ( const Name& ) const;
              RegularLayer*  getRegularLayer         ( const Name& ) const;
              ViaLayer*      getViaLayer             ( const Name& ) const;
      inline  Layers         getLayers               () const;
              BasicLayers    getBasicLayers          () const;
              BasicLayers    getBasicLayers          ( const Layer::Mask& ) const;
              RegularLayers  getRegularLayers        () const;
              ViaLayers      getViaLayers            () const;
              Layer*         getLayer                ( const Layer::Mask&, bool useWorking=true ) const;
              Layer*         getMetalAbove           ( const Layer*, bool useWorking=true ) const;
              Layer*         getMetalBelow           ( const Layer*, bool useWorking=true ) const;
              Layer*         getCutAbove             ( const Layer*, bool useWorking=true ) const;
              Layer*         getCutBelow             ( const Layer*, bool useWorking=true ) const;
              Layer*         getViaBetween           ( const Layer*, const Layer* ) const;
              Layer*         getNthMetal             ( int ) const;
    // Updators.
              void           setName                 ( const Name& );
              bool           setWorkingLayer         ( const Name& );
              bool           setWorkingLayer         ( const Layer* );
    // Others.
      inline  LayerMap&      _getLayerMap            ();
      inline  LayerMaskMap&  _getLayerMaskMap        ();
              void           _insertInLayerMaskMap   ( Layer* );
              void           _removeFromLayerMaskMap ( Layer* );
      inline  Layer::Mask&   _getCutMask             ();
      inline  Layer::Mask&   _getMetalMask           ();
              void           _onDbuChange            ( float scale );
    // Hurricane Managment.
      virtual string         _getTypeName            () const;
      virtual string         _getString              () const;
      virtual Record*        _getRecord              () const;
      
    private:
    // Internal: Attributes.
              DataBase*      _dataBase;
              Name           _name;
              LayerMap       _layerMap;
              LayerMaskMap   _layerMaskMap;
              Layer::Mask    _cutMask;
              Layer::Mask    _metalMask;

    protected:
    // Constructors & Destructors.
                             Technology              ( DataBase* , const Name& );
      virtual void           _postCreate             ();
      virtual void           _preDestroy             ();
  };


// Inline Functions.
  inline  bool                      Technology::isMetal          ( const Layer* layer ) const { return _metalMask.contains(layer->getMask()); }
  inline  DataBase*                 Technology::getDataBase      () const { return _dataBase; }
  inline  const Name&               Technology::getName          () const { return _name; }
  inline  Layer*                    Technology::getLayer         ( const Name& name ) const { return _layerMap.getElement(name); }
  inline  Layers                    Technology::getLayers        () const { return getCollection(&_layerMaskMap); }
  inline  Technology::LayerMap&     Technology::_getLayerMap     () { return _layerMap; }
  inline  Technology::LayerMaskMap& Technology::_getLayerMaskMap () { return _layerMaskMap; }
  inline  Layer::Mask&              Technology::_getCutMask      () { return _cutMask; }
  inline  Layer::Mask&              Technology::_getMetalMask    () { return _metalMask; }


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Technology);


#endif  // __HURRICANE_TECHNOLOGY__

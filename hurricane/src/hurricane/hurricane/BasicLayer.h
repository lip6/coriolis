
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/BasicLayer.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Layer.h"
#include "hurricane/BasicLayers.h"
#include "hurricane/Box.h"


namespace Hurricane {


  class BasicLayer : public Layer {
    public:
      typedef  Layer  Super;

    public:
    // Subclass: Material.
      class Material {
        // Enum: Code.
        public:
          enum Code { nWell =0
                    , pWell
                    , nImplant
                    , pImplant
                    , active
                    , poly
                    , cut
                    , metal
                    , blockage
                    , info
                    , other
                    };
        // Constructors.
                             Material             ( const Code& code = other );
                             Material             ( const Material& material );
        // Methods.
                 Material&   operator=            ( const Material& material );
          inline             operator const Code& () const;
          inline const Code& getCode              () const;
          static Material    fromString           ( const string& );
          inline string      _getTypeName         () const;
                 string      _getString           () const;
                 Record*     _getRecord           () const;

        // Internal: Attributes.
        private:
                 Code        _code;
      };

    public:
    // Constructor.
      static  BasicLayer*     create                 ( Technology*      technology
                                                     , const Name&      name
                                                     , const Material&  material
                                                     , uint32_t         gds2Layer      = 0
                                                     , uint32_t         gds2Datatype   = 0
                                                     , const DbU::Unit& minimalSize    = 0
                                                     , const DbU::Unit& minimalSpacing = 0
                                                     );
    // Accessors.                                    
      inline  bool            hasGds                 () const;
      inline  const Material& getMaterial            () const;
      inline  uint32_t        getGds2Layer           () const;
      inline  uint32_t        getGds2Datatype        () const;
      virtual BasicLayers     getBasicLayers         () const;
      virtual BasicLayer*     getBlockageLayer       () const;
      virtual BasicLayer*     getRoutingLayer        () const;
      virtual const Layer*    getTop                 () const;
      virtual const Layer*    getBottom              () const;
      inline  const Name&     getRealName            () const;
    // Updators                                      
      inline  void            setBlockageLayer       ( BasicLayer* layer);
      inline  void            setRoutingLayer        ( BasicLayer* layer);
      inline  void            setGds2Layer           ( uint32_t );
      inline  void            setGds2Datatype        ( uint32_t );
      inline  void            setRealName            ( const char* realName);
    // Hurricane Managment.
      virtual void            _toJson                ( JsonWriter* writer ) const;
      virtual BasicLayer*     _getSymbolicBasicLayer ();
      virtual string          _getTypeName           () const;
      virtual string          _getString             () const;
      virtual Record*         _getRecord             () const;

    private:
    // Internal: Attributes
              Material        _material;
              uint32_t        _gds2Layer;
              uint32_t        _gds2Datatype;
              BasicLayer*     _blockageLayer;
              BasicLayer*     _routingLayer;
              Name            _realName;
              bool            _hasGds;

    protected:
    // Internal: Constructors & Destructors.
                              BasicLayer             ( Technology*      technology
                                                     , const Name&      name
                                                     , const Material&  material
                                                     , uint32_t         gds2Layer
                                                     , uint32_t         gds2Datatype
                                                     , const DbU::Unit& minimalSize    = 0
                                                     , const DbU::Unit& minimalSpacing = 0
                                                     );
     virtual void             _postCreate            ();
     virtual void             _preDestroy            ();
  };


// Inline Functions.
  inline                 BasicLayer::Material::operator const Code& () const { return _code; }
  inline const BasicLayer::Material::Code&
                         BasicLayer::Material::getCode              () const { return _code; }
  inline string          BasicLayer::Material::_getTypeName         () const { return _TName("BasicLayer::Material"); }
  inline bool            BasicLayer::hasGds                         () const { return _hasGds; }
  inline const BasicLayer::Material&
                         BasicLayer::getMaterial                    () const { return _material; }
  inline uint32_t        BasicLayer::getGds2Layer                   () const { return _gds2Layer; }
  inline uint32_t        BasicLayer::getGds2Datatype                () const { return _gds2Datatype; }
  inline const Name&     BasicLayer::getRealName                    () const { return _realName; }
  inline void            BasicLayer::setRoutingLayer                ( BasicLayer* layer) { _routingLayer = layer; }
  inline void            BasicLayer::setGds2Layer                   ( uint32_t number ) { _gds2Layer=number; _hasGds=true; }
  inline void            BasicLayer::setGds2Datatype                ( uint32_t number ) { _gds2Datatype=number; }
  inline void            BasicLayer::setRealName                    ( const char* realName) { _realName = realName; }

  inline void  BasicLayer::setBlockageLayer ( BasicLayer* layer)
  {
    _blockageLayer = layer;
    layer->setBlockage( true );
    layer->setRoutingLayer( this );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonBasicLayer".

  class JsonBasicLayer : public JsonLayer {
    public:
      static  void            initialize     ();
                              JsonBasicLayer ( unsigned long flags );
                             ~JsonBasicLayer ();
      virtual string          getTypeName    () const;
      virtual JsonBasicLayer* clone          ( unsigned long ) const;
      virtual void            toData         ( JsonStack& ); 
  };


} // End of Hurricane namespace.


// -------------------------------------------------------------------
// Inspector Support for  :  BasicLayer::Material::Code.


template<>
inline std::string  getString<const Hurricane::BasicLayer::Material::Code*>
                             ( const Hurricane::BasicLayer::Material::Code* object )
{
  switch ( *object ) {
    case Hurricane::BasicLayer::Material::nWell:    return "nWell";
    case Hurricane::BasicLayer::Material::pWell:    return "pWell";
    case Hurricane::BasicLayer::Material::nImplant: return "nImplant";
    case Hurricane::BasicLayer::Material::pImplant: return "pImplant";
    case Hurricane::BasicLayer::Material::active:   return "active";
    case Hurricane::BasicLayer::Material::poly:     return "poly";
    case Hurricane::BasicLayer::Material::cut:      return "cut";
    case Hurricane::BasicLayer::Material::metal:    return "metal";
    case Hurricane::BasicLayer::Material::blockage: return "blockage";
    case Hurricane::BasicLayer::Material::info:     return "info";
    case Hurricane::BasicLayer::Material::other:    return "other";
  }
  return "undefined";
}


template<>
inline Hurricane::Record* getRecord<const Hurricane::BasicLayer::Material::Code*>
                                   ( const Hurricane::BasicLayer::Material::Code* object )
{
  Hurricane::Record* record = new Hurricane::Record(getString(object));
  record->add(getSlot("Code", (unsigned int*)object));
  return record;
}


INSPECTOR_P_SUPPORT(Hurricane::BasicLayer);
INSPECTOR_P_SUPPORT(Hurricane::BasicLayer::Material);
IOSTREAM_POINTER_SUPPORT(Hurricane::BasicLayer::Material::Code);

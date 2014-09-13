
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2014, All Rights Reserved
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


# ifndef  HURRICANE_BASIC_LAYER_H
# define  HURRICANE_BASIC_LAYER_H

# include  "hurricane/Layer.h"
# include  "hurricane/BasicLayers.h"
# include  "hurricane/Box.h"


namespace Hurricane {


  class BasicLayer : public Layer {

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
                    , other
                    };
        // Constructors.
                             Material             ( const Code& code = other );
                             Material             ( const Material& material );
        // Methods.
                 Material&   operator=            ( const Material& material );
          inline             operator const Code& () const;
          inline const Code& getCode              () const;
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
                                                     , unsigned         extractNumber
                                                     , const DbU::Unit& minimalSize    = 0
                                                     , const DbU::Unit& minimalSpacing = 0
                                                     );
    // Accessors.                                    
      inline  const Material& getMaterial            () const;
      inline  unsigned        getExtractNumber       () const;
      virtual BasicLayers     getBasicLayers         () const;
      virtual BasicLayer*     getBlockageLayer       () const;
      inline  const Name&     getRealName            () const;
    // Updators                                      
      inline  void            setBlockageLayer       ( BasicLayer* layer);
      inline  void            setExtractNumber       ( unsigned int );
      inline  void            setRealName            ( const char* realName);
    // Hurricane Managment.
      virtual BasicLayer*     _getSymbolicBasicLayer ();
      virtual string          _getTypeName           () const;
      virtual string          _getString             () const;
      virtual Record*         _getRecord             () const;

    private:
    // Internal: Attributes
              Material        _material;
              unsigned        _extractNumber;
              BasicLayer*     _blockageLayer;
              Name            _realName;

    protected:
    // Internal: Constructors & Destructors.
                              BasicLayer             ( Technology*      technology
                                                     , const Name&      name
                                                     , const Material&  material
                                                     , unsigned         extractNumber
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
  inline const BasicLayer::Material&
                         BasicLayer::getMaterial                    () const { return _material; }
  inline unsigned        BasicLayer::getExtractNumber               () const { return _extractNumber; }
  inline const Name&     BasicLayer::getRealName                    () const { return _realName; }
  inline void            BasicLayer::setBlockageLayer               ( BasicLayer* layer) { _blockageLayer = layer; }
  inline void            BasicLayer::setExtractNumber               ( unsigned int number ) { _extractNumber=number; }
  inline void            BasicLayer::setRealName                    ( const char* realName) { _realName = realName; }


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
    case Hurricane::BasicLayer::Material::other:    return "other";
  }
  return "abnormal";
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


# endif

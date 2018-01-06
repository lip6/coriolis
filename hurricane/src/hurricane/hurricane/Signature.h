// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2018, All Rights Reserved
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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Signature.h"                       |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_SIGNATURE_H
#define HURRICANE_SIGNATURE_H

#include "hurricane/DbU.h"


namespace Hurricane {

  class Layer;


// -------------------------------------------------------------------
// Class  :  "Signature".

  class Signature {
    public:
      enum Type          { TypeContact        = 1
                         , TypeVertical       = 2
                         , TypeHorizontal     = 3
                         , TypePad            = 4
                         , TypePlug           = 5
                         , TypeNet            = 6
                         , TypeInstance       = 7
                         };
      enum DimContacts   { ContactDx          = 0
                         , ContactDy          = 1
                         , ContactWidth       = 2
                         , ContactHeight      = 3
                         };
      enum DimVertical   { VerticalWidth      = 0
                         , VerticalX          = 1
                         , VerticalDySource   = 2
                         , VerticalDyTarget   = 3
                         };
      enum DimHorizontal { HorizontalWidth    = 0
                         , HorizontalY        = 1
                         , HorizontalDxSource = 2
                         , HorizontalDxTarget = 3
                         };
      enum DimPad        { PadXMin            = 0
                         , PadYMin            = 1
                         , PadXMax            = 2
                         , PadYMax            = 3
                         };
    public:
      inline              Signature    ();
      inline              Signature    ( const Signature& );
      inline Type         getType      () const;
      inline std::string  getName      () const;
      inline std::string  getMasterNet () const;
      inline const Layer* getLayer     () const;
      inline DbU::Unit    getDim       ( size_t index ) const;
      inline void         setType      ( Type );
      inline void         setName      ( const std::string& );
      inline void         setMasterNet ( const std::string& );
             void         setLayer     ( const std::string& );
      inline void         setDim       ( size_t index, DbU::Unit );
    private:
    // Attributes.
      Type         _type;
      std::string  _name;       // For Cell, Instance & Net.
      std::string  _masterNet;  // For Plug.
      const Layer* _layer;      // For Components (save Plug).
      DbU::Unit    _dims[4];    // Dimensions.
  };


  inline Signature::Signature ()
    : _type     ()
    , _name     ()
    , _masterNet()
    , _layer    (NULL)
  {
    for ( size_t i=0 ; i<4 ; ++i ) _dims[i] = 0;
  }


  inline Signature::Signature ( const Signature& other )
    : _type     (other._type     )
    , _name     (other._name     )
    , _masterNet(other._masterNet)
    , _layer    (other._layer    )
  {
    for ( size_t i=0 ; i<4 ; ++i ) _dims[i] = other._dims[i];
  }


  inline Signature::Type  Signature::getType      () const { return _type; }
  inline std::string      Signature::getName      () const { return _name; }
  inline std::string      Signature::getMasterNet () const { return _masterNet; }
  inline const Layer*     Signature::getLayer     () const { return _layer; }
  inline DbU::Unit        Signature::getDim       ( size_t index ) const           { return _dims[index]; }
  inline void             Signature::setType      ( Signature::Type type )         { _type=type; }
  inline void             Signature::setName      ( const std::string& name )      { _name=name; }
  inline void             Signature::setMasterNet ( const std::string& masterNet ) { _masterNet=masterNet; }
  inline void             Signature::setDim       ( size_t index, DbU::Unit u )    { _dims[index]=u; }


// -------------------------------------------------------------------
// Class  :  "Signature".

  class JsonSignature : public JsonObject {
    public:
      static  void           initialize    ();
                             JsonSignature ( unsigned long flags );
              void           setSubType    ( const std::string& );
      virtual std::string    getTypeName   () const;
      virtual JsonSignature* clone         ( unsigned long flags ) const;
      virtual void           toData        ( JsonStack& ); 
    private:
                             JsonSignature ( const JsonSignature& );
              JsonSignature& operator=     ( const JsonSignature& ) const;
    private:
      std::string  _subTypeName;
  };

} // End of Hurricane namespace.

#endif  // HURRICANE_SIGNATURE_H

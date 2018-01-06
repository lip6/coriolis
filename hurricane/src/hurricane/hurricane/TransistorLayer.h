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
// |  C++ Header  :  "./hurricane/TransistorLayer.h"                 |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_TRANSISTOR_LAYER_H
#define  HURRICANE_TRANSISTOR_LAYER_H

#include  <vector>

#include  "hurricane/Layer.h"
#include  "hurricane/TransistorLayers.h"


namespace Hurricane {

// -------------------------------------------------------------------
// Class  :  "Hurricane::TransistorLayer".

  class TransistorLayer : public Layer {
    public:
      typedef  Layer  Super;

    public:
    // Constructor.
      static  TransistorLayer*     create            ( Technology* technology
                                                     , const Name& name
                                                     , BasicLayer* gateLayer
                                                     , BasicLayer* activeLayer
                                                     , BasicLayer* diffusionLayer
                                                     , BasicLayer* wellLayer
                                                     );
    // Accessors.
      virtual BasicLayers          getBasicLayers    () const;
      virtual DbU::Unit            getExtentionCap   () const;
      virtual DbU::Unit            getExtentionWidth () const;
      virtual DbU::Unit            getExtentionCap   ( const BasicLayer* layer ) const;
      virtual DbU::Unit            getExtentionWidth ( const BasicLayer* layer ) const;
    // Updators.
      virtual void                 setExtentionCap   ( const BasicLayer* layer, DbU::Unit cap );
      virtual void                 setExtentionWidth ( const BasicLayer* layer, DbU::Unit width );
    // Hurricane Managment.
      virtual void                 _toJson           ( JsonWriter* ) const;
      virtual void                 _onDbuChange      ( float scale );
      virtual string               _getTypeName      () const;
      virtual string               _getString        () const;
      virtual Record*              _getRecord        () const;

    private:
    // Internal: Attributes
              vector<BasicLayer*>  _basicLayers;
              vector<DbU::Unit>    _extentionCaps;
              vector<DbU::Unit>    _extentionWidths;
              DbU::Unit            _maximalExtentionCap;
              DbU::Unit            _maximalExtentionWidth;

    protected:
    // Internal: Constructors & Destructors.
                                   TransistorLayer   ( Technology* technology
                                                     , const Name& name
                                                     , BasicLayer* gateLayer
                                                     , BasicLayer* activeLayer
                                                     , BasicLayer* diffusionLayer
                                                     , BasicLayer* wellLayer
                                                     );
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonTransistorLayer".

  class JsonTransistorLayer : public JsonLayer {
    public:
      static  void                 initialize          ();
                                   JsonTransistorLayer ( unsigned long flags );
                                  ~JsonTransistorLayer ();
      virtual string               getTypeName         () const;
      virtual JsonTransistorLayer* clone               ( unsigned long ) const;
      virtual void                 toData              ( JsonStack& ); 
  };


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::TransistorLayer);


# endif

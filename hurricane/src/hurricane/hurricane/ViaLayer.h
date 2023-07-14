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
// |  C++ Header  :  "./hurricane/ViaLayer.h"                        |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_VIA_LAYER_H
#define  HURRICANE_VIA_LAYER_H

#include  <vector>

#include  "hurricane/Layer.h"
#include  "hurricane/ViaLayers.h"


namespace Hurricane {

// -------------------------------------------------------------------
// Class  :  "Hurricane::ViaLayer".

  class ViaLayer : public Layer {
    public:
      typedef  Layer  Super;

    public:
    // Constructor.
      static  ViaLayer*            create             ( Technology* technology
                                                      , const Name& name
                                                      , BasicLayer* bottomLayer
                                                      , BasicLayer* cutLayer
                                                      , BasicLayer* topLayer
                                                      );
    // Accessors.                                     
      virtual BasicLayers          getBasicLayers     () const;
      virtual const Layer*         getCut             () const;
      virtual const Layer*         getTop             () const;
      virtual const Layer*         getBottom          () const;
      virtual const Layer*         getOpposite        ( const Layer* ) const;
      virtual DbU::Unit            getEnclosure       ( uint32_t flags ) const;
      virtual DbU::Unit            getEnclosure       ( const BasicLayer* layer, uint32_t flags  ) const;
      virtual DbU::Unit            getTopEnclosure    ( uint32_t flags ) const;
      virtual DbU::Unit            getBottomEnclosure ( uint32_t flags ) const;
    // Updators.
      virtual void                 setEnclosure       ( const BasicLayer* layer, DbU::Unit enclosure, uint32_t flags );
    // Hurricane Managment.                           
      virtual void                 _toJson            ( JsonWriter* ) const;
      virtual void                 _onDbuChange       ( float scale );
      virtual string               _getTypeName       () const;
      virtual string               _getString         () const;
      virtual Record*              _getRecord         () const;

    private:
    // Internal: Attributes
              vector<BasicLayer*>                  _basicLayers;
              vector< pair<DbU::Unit,DbU::Unit> >  _enclosures;
              DbU::Unit                            _maximalEnclosure;

    protected:
    // Internal: Constructors & Destructors.
                                   ViaLayer          ( Technology* technology
                                                     , const Name& name
                                                     , BasicLayer* bottomLayer
                                                     , BasicLayer* cutLayer
                                                     , BasicLayer* topLayer
                                                     );
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonViaLayer".

  class JsonViaLayer : public JsonLayer {
    public:
      static  void          initialize   ();
                            JsonViaLayer ( unsigned long flags );
                           ~JsonViaLayer ();
      virtual string        getTypeName  () const;
      virtual JsonViaLayer* clone        ( unsigned long ) const;
      virtual void          toData       ( JsonStack& ); 
  };


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::ViaLayer);


# endif

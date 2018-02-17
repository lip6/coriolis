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
// |  C++ Header  :  "./hurricane/ContactLayer.h"                    |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_CONTACT_LAYER_H
#define  HURRICANE_CONTACT_LAYER_H

#include  <vector>

#include  "hurricane/Layer.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ContactLayer".

  class ContactLayer : public Layer {
    public:
      typedef  Layer  Super;

    public:
    // Constructor.
      static  ContactLayer* create         ( Technology* technology
                                           , const Name& name
                                           , BasicLayer* metalLayer
                                           , BasicLayer* cutLayer
                                           , BasicLayer* activeLayer
                                           , BasicLayer* diffusionLayer
                                           , BasicLayer* wellLayer
                                           );
    // Accessors.
      virtual BasicLayers   getBasicLayers () const;
      virtual DbU::Unit     getEnclosure   ( uint32_t flags ) const;
      virtual DbU::Unit     getEnclosure   ( const BasicLayer* layer, uint32_t flags ) const;
    // Updators.
      virtual void          setEnclosure   ( const BasicLayer* layer, DbU::Unit enclosure, uint32_t flags );
    // Hurricane Managment.
      virtual void          _toJson        ( JsonWriter* ) const;
      virtual void          _onDbuChange   ( float scale );
      virtual string        _getTypeName   () const;
      virtual string        _getString     () const;
      virtual Record*       _getRecord     () const;

    private:
    // Internal: Attributes
      vector<BasicLayer*>                  _basicLayers;
      vector< pair<DbU::Unit,DbU::Unit> >  _enclosures;
      DbU::Unit                            _maximalEnclosure;

    protected:
      ContactLayer ( Technology* technology
                   , const Name& name
                   , BasicLayer* metalLayer
                   , BasicLayer* cutLayer
                   , BasicLayer* activeLayer
                   , BasicLayer* diffusionLayer
                   , BasicLayer* wellLayer
                   );
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonContactLayer".

  class JsonContactLayer : public JsonLayer {
    public:
      static  void              initialize       ();
                                JsonContactLayer ( unsigned long flags );
                               ~JsonContactLayer ();
      virtual string            getTypeName      () const;
      virtual JsonContactLayer* clone            ( unsigned long ) const;
      virtual void              toData           ( JsonStack& ); 
  };


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::ContactLayer);


# endif

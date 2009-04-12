
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
// |  C++ Header  :       "./ContactLayer.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_CONTACT_LAYER__
#define  __HURRICANE_CONTACT_LAYER__

#include  <vector>

#include  "hurricane/Layer.h"


namespace Hurricane {


  class ContactLayer : public Layer {

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
      virtual DbU::Unit     getEnclosure   () const;
      virtual DbU::Unit     getEnclosure   ( const BasicLayer* layer ) const;
    // Updators.
      virtual void          setEnclosure   ( const BasicLayer* layer, DbU::Unit enclosure );
    // Hurricane Managment.
      virtual void          _onDbuChange   ( float scale );
      virtual string        _getTypeName   () const;
      virtual string        _getString     () const;
      virtual Record*       _getRecord     () const;

    private:
    // Internal: Attributes
      vector<BasicLayer*>  _basicLayers;
      vector<DbU::Unit>    _enclosures;
      DbU::Unit            _maximalEnclosure;

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


} // End of Hurricane namespace.


# endif

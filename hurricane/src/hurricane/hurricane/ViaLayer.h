
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
// |  C++ Header  :       "./ViaLayer.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_VIA_LAYER__
#define  __HURRICANE_VIA_LAYER__

#include  <vector>

#include  "hurricane/Layer.h"
#include  "hurricane/ViaLayers.h"


namespace Hurricane {


  class ViaLayer : public Layer {

    public:
    // Constructor.
      static  ViaLayer*            create            ( Technology* technology
                                                     , const Name& name
                                                     , BasicLayer* bottomLayer
                                                     , BasicLayer* cutLayer
                                                     , BasicLayer* topLayer
                                                     );
    // Accessors.
      virtual BasicLayers          getBasicLayers    () const;
      virtual const Layer*         getTop            () const;
      virtual const Layer*         getBottom         () const;
      virtual const Layer*         getOpposite       ( const Layer* ) const;
      virtual DbU::Unit            getEnclosure      () const;
      virtual DbU::Unit            getEnclosure      ( const BasicLayer* layer ) const;
    // Updators.
      virtual void                 setEnclosure      ( const BasicLayer* layer, DbU::Unit enclosure );
    // Hurricane Managment.
      virtual void                 _onDbuChange      ( float scale );
      virtual string               _getTypeName      () const;
      virtual string               _getString        () const;
      virtual Record*              _getRecord        () const;

    private:
    // Internal: Attributes
              vector<BasicLayer*>  _basicLayers;
              vector<DbU::Unit>    _enclosures;
              DbU::Unit            _maximalEnclosure;

    protected:
    // Internal: Constructors & Destructors.
                                   ViaLayer          ( Technology* technology
                                                     , const Name& name
                                                     , BasicLayer* bottomLayer
                                                     , BasicLayer* cutLayer
                                                     , BasicLayer* topLayer
                                                     );
  };


} // End of Hurricane namespace.


# endif

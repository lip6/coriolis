
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
// |  C++ Header  :       "./DiffusionLayer.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_DIFFUSION_LAYER__
#define  __HURRICANE_DIFFUSION_LAYER__

#include  <vector>

#include  "hurricane/Layer.h"


namespace Hurricane {


  class DiffusionLayer : public Layer {

    public:
    // Constructor.
      static  DiffusionLayer* create            ( Technology* technology
                                                , const Name& name
                                                , BasicLayer* activeLayer
                                                , BasicLayer* diffusionLayer
                                                , BasicLayer* wellLayer
                                                );
    // Accessors.
      virtual BasicLayers     getBasicLayers    () const;
      virtual DbU::Unit       getExtentionCap   () const;
      virtual DbU::Unit       getExtentionWidth () const;
      virtual DbU::Unit       getExtentionCap   ( const BasicLayer* layer ) const;
      virtual DbU::Unit       getExtentionWidth ( const BasicLayer* layer ) const;
    // Updators.
      virtual void            setExtentionCap   ( const BasicLayer* layer, DbU::Unit cap );
      virtual void            setExtentionWidth ( const BasicLayer* layer, DbU::Unit width );
    // Hurricane Managment.
      virtual void            _onDbuChange      ( float scale );
      virtual string          _getTypeName      () const;
      virtual string          _getString        () const;
      virtual Record*         _getRecord        () const;

    private:
    // Internal: Attributes
      vector<BasicLayer*>  _basicLayers;
      vector<DbU::Unit>    _extentionCaps;
      vector<DbU::Unit>    _extentionWidths;
      DbU::Unit            _maximalExtentionCap;
      DbU::Unit            _maximalExtentionWidth;

    protected:
    // Internal: Constructors & Destructors.
      DiffusionLayer ( Technology* technology
                     , const Name& name
                     , BasicLayer* activeLayer
                     , BasicLayer* diffusionLayer
                     , BasicLayer* wellLayer
                     );
  };


} // End of Hurricane namespace.


# endif

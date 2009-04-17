
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RegularLayer.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_REGULAR_LAYER__
#define  __HURRICANE_REGULAR_LAYER__

#include  "hurricane/Layer.h"
#include  "hurricane/RegularLayers.h"


namespace Hurricane {


  class RegularLayer : public Layer {

    public:
    // Constructor.
      static  RegularLayer*   create            ( Technology* technology
                                                , const Name& name
                                                );
    // Accessors.
      virtual BasicLayers     getBasicLayers    () const;
      inline  BasicLayer*     getBasicLayer     () const;
      virtual const Layer*    getTop            () const;
      virtual const Layer*    getBottom         () const;
      virtual const Layer*    getOpposite       ( const Layer* ) const;
      virtual DbU::Unit       getEnclosure      () const;
      virtual DbU::Unit       getExtentionCap   () const;
      virtual DbU::Unit       getExtentionWidth () const;
      virtual DbU::Unit       getEnclosure      ( const BasicLayer* layer ) const;
      virtual DbU::Unit       getExtentionCap   ( const BasicLayer* layer ) const;
      virtual DbU::Unit       getExtentionWidth ( const BasicLayer* layer ) const;
    // Updators
              void            setBasicLayer     ( BasicLayer* layer );
      virtual void            setEnclosure      ( const BasicLayer* layer, DbU::Unit enclosure );
      virtual void            setExtentionCap   ( const BasicLayer* layer, DbU::Unit cap );
      virtual void            setExtentionWidth ( const BasicLayer* layer, DbU::Unit width );
    // Hurricane Managment.
      virtual void            _onDbuChange      ( float scale );
      virtual string          _getTypeName      () const;
      virtual string          _getString        () const;
      virtual Record*         _getRecord        () const;

    private:
    // Internal: Attributes
              BasicLayer*     _basicLayer;
              DbU::Unit       _enclosure;
              DbU::Unit       _extentionCap;
              DbU::Unit       _extentionWidth;

    protected:
    // Internal: Constructors & Destructors.
                              RegularLayer      ( Technology*     technology
                                                , const Name&     name
                                                );
  };


// Inline Functions.
  BasicLayer* RegularLayer::getBasicLayer     () const { return _basicLayer; }


} // End of Hurricane namespace.


# endif

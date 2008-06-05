

// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RegularLayer.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_REGULAR_LAYER__
# define  __HURRICANE_REGULAR_LAYER__

# include  "hurricane/Layer.h"
# include  "hurricane/RegularLayers.h"


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
      virtual Unit            getEnclosure      () const;
      virtual Unit            getExtentionCap   () const;
      virtual Unit            getExtentionWidth () const;
      virtual Unit            getEnclosure      ( const BasicLayer* layer ) const;
      virtual Unit            getExtentionCap   ( const BasicLayer* layer ) const;
      virtual Unit            getExtentionWidth ( const BasicLayer* layer ) const;
    // Updators
              void            setBasicLayer     ( BasicLayer* layer );
      virtual void            setEnclosure      ( const BasicLayer* layer, Unit enclosure );
      virtual void            setExtentionCap   ( const BasicLayer* layer, Unit cap );
      virtual void            setExtentionWidth ( const BasicLayer* layer, Unit width );
    // Hurricane Managment.
      virtual string          _getTypeName      () const;
      virtual string          _getString        () const;
      virtual Record*         _getRecord        () const;

    private:
    // Internal: Attributes
              BasicLayer*     _basicLayer;
              Unit            _enclosure;
              Unit            _extentionCap;
              Unit            _extentionWidth;

    protected:
    // Internal: Constructors & Destructors.
                              RegularLayer      ( Technology*     technology
                                                , const Name&     name
                                                );
  };


// Inline Functions.
  BasicLayer* RegularLayer::getBasicLayer     () const { return _basicLayer; }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::RegularLayer)


# endif

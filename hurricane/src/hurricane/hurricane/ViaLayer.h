

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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ViaLayer.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_VIA_LAYER__
# define  __HURRICANE_VIA_LAYER__

# include  <vector>

# include  "hurricane/Layer.h"
# include  "hurricane/ViaLayers.h"


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
      virtual DbU::Unit                 getEnclosure      () const;
      virtual DbU::Unit                 getEnclosure      ( const BasicLayer* layer ) const;
    // Updators.
      virtual void                 setEnclosure      ( const BasicLayer* layer, DbU::Unit enclosure );
    // Hurricane Managment.
      virtual string               _getTypeName      () const;
      virtual string               _getString        () const;
      virtual Record*              _getRecord        () const;

    private:
    // Internal: Attributes
              vector<BasicLayer*>  _basicLayers;
              vector<DbU::Unit>         _enclosures;
             DbU::Unit                 _maximalEnclosure;

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

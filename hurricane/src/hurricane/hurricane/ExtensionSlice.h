
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
// |  C++ Header  :  "./hurricane/ExtensionSlice.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Mask.h"
#include "hurricane/Name.h"
#include "hurricane/ExtensionSlices.h"
#include "hurricane/QuadTree.h"


namespace Hurricane {


  class Cell;


  class ExtensionSlice {

    public:
      typedef Hurricane::Mask<unsigned long long> Mask;
    public:
    // Constructor & Destructor.
      static  ExtensionSlice*     _create        ( Cell* , const Name& );
              void                _destroy       ();
    // Methods.
      inline  bool                isEmpty        () const;
      inline  Cell*               getCell        () const;
      inline  const Name&         getName        () const;
      inline  Mask                getMask        () const;
      inline  const Box&          getBoundingBox () const;
      inline  Gos                 getGos         () const;
      inline  Gos                 getGosUnder    ( const Box& area, DbU::Unit threshold=0 ) const;
      inline  QuadTree*           _getQuadTree   ();
    // Hurricane Managment.
              string              _getTypeName   () const;
              string              _getString     () const;
              Record*             _getRecord     () const;

    private:
    // Internal: Attributes.
      static  unsigned int        _masks;
      static  vector<Name>        _names;
              Cell*               _cell;
              Name                _name;
              Mask                _mask;
              QuadTree            _quadTree;

    protected:
    // Internal: Constructors & Destructors.
                                  ExtensionSlice ( Cell* , const Name&, Mask );
                                 ~ExtensionSlice ();
    private:
                                  ExtensionSlice ( const ExtensionSlice& );
              ExtensionSlice&     operator=      ( const ExtensionSlice& );
};


// Inline Functions.
  inline bool                  ExtensionSlice::isEmpty        () const { return _quadTree.isEmpty(); }
  inline Cell*                 ExtensionSlice::getCell        () const { return _cell; }
  inline const Name&           ExtensionSlice::getName        () const { return _name; }
  inline ExtensionSlice::Mask  ExtensionSlice::getMask        () const { return _mask; }
  inline const Box&            ExtensionSlice::getBoundingBox () const { return _quadTree.getBoundingBox(); }
  inline Gos                   ExtensionSlice::getGos         () const { return _quadTree.getGos(); }
  inline Gos                   ExtensionSlice::getGosUnder    ( const Box& area, DbU::Unit threshold ) const {return _quadTree.getGosUnder(area,threshold); }
  inline QuadTree*             ExtensionSlice::_getQuadTree   () { return &_quadTree; }


} // End of Hurricane namespace.

INSPECTOR_P_SUPPORT(Hurricane::ExtensionSlice);

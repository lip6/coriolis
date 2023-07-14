// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2020, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/CellsSort.h"                       |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_CELLS_SORT_H
#define HURRICANE_CELLS_SORT_H

#include <tuple>
#include <vector>
#include "hurricane/Property.h"


namespace Hurricane {

  class Library;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "DepthProperty".

  
  typedef StandardPrivateProperty<size_t>  DepthProperty;


// -------------------------------------------------------------------
// Class  :  "CellsSort".

  class CellsSort {
    public:
      class CompareByDepth {
        public:
          inline bool operator() ( const DepthProperty*,  const DepthProperty* );
      };
    public:
                                       CellsSort      ();
                   void                addLibrary     ( Library* );
                   void                sort           ();
      inline const std::vector<Cell*>& getSortedCells () const;
    private:
                   void                _computeDepth  ( Cell* );
    private:
      std::vector<Cell*>           _sortedCells;
      std::vector<DepthProperty*>  _depthProperties;
  };

  
  inline const std::vector<Cell*>& CellsSort::getSortedCells   () const { return _sortedCells; }


  inline bool CellsSort::CompareByDepth::operator() ( const DepthProperty* lhs, const DepthProperty* rhs )
  { return lhs->getValue() > rhs->getValue(); }


}  // Hurricane namespace.

#endif  // HURRICANE_CELLS_SORT_H

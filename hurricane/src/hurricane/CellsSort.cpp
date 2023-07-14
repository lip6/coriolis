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
// |  C++ Module  :  "./CellsSort.cpp"                               |
// +-----------------------------------------------------------------+


#include <algorithm>
#include "hurricane/CellsSort.h"
#include "hurricane/Cells.h"
#include "hurricane/Library.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "DepthProperty".
  
  template<>
  Name  DepthProperty::_name = "Hurricane::DepthProperty";


// -------------------------------------------------------------------
// Class  :  "CellsSort".

  CellsSort::CellsSort ()
    : _sortedCells    ()
    , _depthProperties()
  { }


  void  CellsSort::addLibrary ( Library* library )
  {
    for ( Cell* cell : library->getCells() ) {
      if (cell->getProperty(DepthProperty::staticGetName())) continue;

      DepthProperty* property = DepthProperty::create( 0 );
      cell->put( property );
      _depthProperties.push_back( property );
    }

    for ( Library* childLib : library->getLibraries() ) {
      addLibrary( childLib );
    }
  }


  void  CellsSort::sort ()
  {
    for ( DepthProperty* property : _depthProperties ) {
      Cell* cell = dynamic_cast<Cell*>( property->getOwner() );

    // This is a terminal cell.
      if (not cell->getInstances().getFirst()) continue;

    // The depth of this cell has already been computed.
      if (property->getValue()) continue;

      _computeDepth( cell );
    }

    std::sort( _depthProperties.begin(), _depthProperties.end(), CompareByDepth() );

    for ( DepthProperty* property : _depthProperties ) {
      Cell* cell = dynamic_cast<Cell*>(property->getOwner());
      _sortedCells.push_back( cell );
      cell->remove( property );
    }
    
    std::vector<DepthProperty*>().swap( _depthProperties );
  }


  void  CellsSort::_computeDepth ( Cell* cell )
  {
    size_t         depth           = 0;
    DepthProperty* parentDepthProp = dynamic_cast<DepthProperty*>( cell->getProperty( DepthProperty::staticGetName() ));

    for ( Instance* instance : cell->getInstances() ) {
      Cell*          masterCell     = instance->getMasterCell();
      DepthProperty* childDepthProp = dynamic_cast<DepthProperty*>( masterCell->getProperty(DepthProperty::staticGetName()) );

      if (not childDepthProp) continue;
      if (childDepthProp->getValue() == 0) {
        _computeDepth( masterCell );
      }

      depth = std::max( depth, childDepthProp->getValue() + 1 );
    }
    parentDepthProp->setValue( depth );
  }


} // Hurricane namespace.

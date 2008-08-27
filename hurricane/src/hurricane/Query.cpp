
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
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Query.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <climits>

#include "hurricane/BasicLayer.h"
#include "hurricane/Slice.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Query.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Slave Class  :  "QueryState".


// -------------------------------------------------------------------
// Class  :  "QueryStack".


  QueryStack::QueryStack ()
    : vector<QueryState*>()
  //, _tab("  ")
    , _topCell(NULL)
    , _topArea()
    , _topTransformation()
    , _startLevel(0)
    , _stopLevel(UINT_MAX)
  { }


  QueryStack::~QueryStack ()
  {
    for ( size_t i=0 ; i<size() ; i++ ) delete operator[](i);
  }


// -------------------------------------------------------------------
// Class  :  "Query".


  Query::Query ()
    : _stack()
    , _basicLayer(NULL)
    , _filter(DoAll)
  { }


  Query::~Query ()
  { }


  void  Query::setQuery ( Cell*                 cell
                        , const Box&            area
                        , const Transformation& transformation
                        , const BasicLayer*     basicLayer
                        , unsigned int          filter
                        )
  {
    _basicLayer = basicLayer;
    _filter     = filter;

    _stack.setTopCell           ( cell );
    _stack.setTopArea           ( area );
    _stack.setTopTransformation ( transformation );
  }


  void  Query::doQuery ()
  {
    if ( _stack.getTopArea().isEmpty() || !_stack.getTopCell() ) return;
    
  //cerr << "Query::doQuery() - " << _stack.getTopCell() << " " << _stack.getTopArea() << " " << _basicLayer << endl;

    _stack.init ();

    while ( !_stack.empty() ) {
    // Process the Components of the current instance.
      if ( hasGoCallback() && _basicLayer && (_filter & DoComponents) ) {
      //if ( getInstance() )
      //  cerr << getTab() << getInstance() << " " << getTransformation() << endl;
      //else
      //  cerr << "  TopCell: " << getMasterCell() << " " << getTransformation() << endl;

        forEach ( Slice*, islice, getMasterCell()->getSlices() ) {
          if ( !(*islice)->getLayer()->contains(getBasicLayer()) ) continue;
          if ( !(*islice)->getBoundingBox().intersect(getArea()) ) continue;

          forEach ( Go*, igo, (*islice)->getGosUnder(_stack.getArea()) )
            goCallback ( *igo );
        }
      }

      if ( (_filter & DoMasterCells) && hasMasterCellCallback() )
        masterCellCallback ();

      _stack.progress ();
    } // End of while.
  }


  bool  Query::hasGoCallback () const
  {
    return false;
  }


  bool  Query::hasMasterCellCallback () const
  {
    return false;
  }


} // End of Hurricane namespace.

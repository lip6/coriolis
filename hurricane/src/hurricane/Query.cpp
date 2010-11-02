
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Query.cpp"                                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <limits>

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
  //, _tab               ("  ")
    , _topCell           (NULL)
    , _topArea           ()
    , _topTransformation ()
    , _startLevel        (0)
    , _stopLevel         (std::numeric_limits<unsigned int>::max())
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


  void  Query::setBasicLayer ( const BasicLayer* basicLayer )
  { _basicLayer = basicLayer; }


  void  Query::setQuery ( Cell*                 cell
                        , const Box&            area
                        , const Transformation& transformation
                        , const BasicLayer*     basicLayer
                        , ExtensionSlice::Mask  mask
                        , Mask                  filter
                        )
  {
    _basicLayer    = basicLayer;
    _filter        = filter;
    _extensionMask = mask;

    _stack.setTopCell           ( cell );
    _stack.setTopArea           ( area );
    _stack.setTopTransformation ( transformation );
  }


  void  Query::doQuery ()
  {
    if ( _stack.getTopArea().isEmpty() or not _stack.getTopCell() ) return;
    
  //cerr << "Query::doQuery() - " << _stack.getTopCell() << " " << _stack.getTopArea() << " " << _basicLayer << endl;

    _stack.init ();

    while ( !_stack.empty() ) {
    // Process the Components of the current instance.
      if ( hasGoCallback() and _basicLayer and (_filter.isSet(DoComponents)) ) {
      //if ( getInstance() )
      //  cerr << getTab() << getInstance() << " " << getTransformation() << endl;
      //else
      //  cerr << "  TopCell: " << getMasterCell() << " " << getTransformation() << endl;

        if ( not getMasterCell()->isTerminal() or (_filter.isSet(DoTerminalCells)) ) {
          forEach ( Slice*, islice, getMasterCell()->getSlices() ) {
            if ( not (*islice)->getLayer()->contains(getBasicLayer()) ) continue;
            if ( not (*islice)->getBoundingBox().intersect(getArea()) ) continue;

            forEach ( Go*, igo, (*islice)->getGosUnder(_stack.getArea()) )
              goCallback ( *igo );
          }
        }
      }

      if ( (not getMasterCell()->isTerminal() or (_filter.isSet(DoTerminalCells)))
         and _filter.isSet(DoMarkers) ) {
        forEach ( Marker*, marker, getMasterCell()->getMarkersUnder(_stack.getArea()) )
          markerCallback ( *marker );
      }

      if ( not getMasterCell()->isTerminal() and (_filter.isSet(DoRubbers)) ) {
        forEach ( Rubber*, rubber, getMasterCell()->getRubbersUnder(_stack.getArea()) )
          rubberCallback ( *rubber );
      }

      if ( hasExtensionGoCallback() and (_filter.isSet(DoExtensionGos)) ) {
        if ( not getMasterCell()->isTerminal() or (_filter.isSet(DoTerminalCells)) ) {
          forEach ( ExtensionSlice*, islice, getMasterCell()->getExtensionSlices() ) {
            if ( not ( (*islice)->getMask() & _extensionMask ) ) continue;
            if ( not (*islice)->getBoundingBox().intersect(getArea()) ) continue;

            forEach ( Go*, igo, (*islice)->getGosUnder(_stack.getArea()) )
              extensionGoCallback ( *igo );
          }
        }
      }

      if ( (_filter.isSet(DoMasterCells)) and hasMasterCellCallback() )
        masterCellCallback ();

      _stack.progress ();
    } // End of while.
  }


  bool  Query::hasGoCallback () const
  { return false; }


  bool  Query::hasMarkerCallback () const
  { return false; }


  bool  Query::hasRubberCallback () const
  { return false; }


  bool  Query::hasExtensionGoCallback () const
  { return false; }


  bool  Query::hasMasterCellCallback () const
  { return false; }


  void  Query::markerCallback ( Marker* )
  { }


  void  Query::rubberCallback ( Rubber* )
  { }


} // End of Hurricane namespace.


// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
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
    if ( _stack.getTopArea().isEmpty() || !_stack.getTopCell() ) return;
    
  //cerr << "Query::doQuery() - " << _stack.getTopCell() << " " << _stack.getTopArea() << " " << _basicLayer << endl;

    _stack.init ();

    while ( !_stack.empty() ) {
    // Process the Components of the current instance.
      if ( hasGoCallback() && _basicLayer && (_filter.isSet(DoComponents)) ) {
      //if ( getInstance() )
      //  cerr << getTab() << getInstance() << " " << getTransformation() << endl;
      //else
      //  cerr << "  TopCell: " << getMasterCell() << " " << getTransformation() << endl;

        if ( !getMasterCell()->isTerminal() || (_filter.isSet(DoTerminalCells)) ) {
          forEach ( Slice*, islice, getMasterCell()->getSlices() ) {
            if ( !(*islice)->getLayer()->contains(getBasicLayer()) ) continue;
            if ( !(*islice)->getBoundingBox().intersect(getArea()) ) continue;

            forEach ( Go*, igo, (*islice)->getGosUnder(_stack.getArea()) )
              goCallback ( *igo );
          }
        }
      }

      if ( !getMasterCell()->isTerminal() && (_filter.isSet(DoRubbers)) ) {
        forEach ( Rubber*, rubber, getMasterCell()->getRubbersUnder(_stack.getArea()) )
          rubberCallback ( *rubber );
      }

      if ( hasExtensionGoCallback() && (_filter.isSet(DoExtensionGos)) ) {
        if ( !getMasterCell()->isTerminal() || (_filter.isSet(DoTerminalCells)) ) {
          forEach ( ExtensionSlice*, islice, getMasterCell()->getExtensionSlices() ) {
            if ( !( (*islice)->getMask() & _extensionMask ) ) continue;
            if ( !(*islice)->getBoundingBox().intersect(getArea()) ) continue;

            forEach ( Go*, igo, (*islice)->getGosUnder(_stack.getArea()) )
              extensionGoCallback ( *igo );
          }
        }
      }

      if ( (_filter.isSet(DoMasterCells)) && hasMasterCellCallback() )
        masterCellCallback ();

      _stack.progress ();
    } // End of while.
  }


  bool  Query::hasGoCallback () const
  {
    return false;
  }


  bool  Query::hasRubberCallback () const
  {
    return false;
  }


  bool  Query::hasExtensionGoCallback () const
  {
    return false;
  }


  bool  Query::hasMasterCellCallback () const
  {
    return false;
  }


  void  Query::rubberCallback ( Rubber* )
  { }


} // End of Hurricane namespace.

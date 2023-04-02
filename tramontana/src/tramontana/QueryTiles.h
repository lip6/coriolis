// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/QueryTiles.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Query.h"


namespace Tramontana {

  using Hurricane::Box;
  using Hurricane::DbU;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::Go;
  using Hurricane::Component;
  using Hurricane::Rubber;
  using Hurricane::Query;
  class SweepLine;


// -------------------------------------------------------------------
// Class  :  "Tramontana::QueryTiles".

  class QueryTiles : public Query {
    public:
                        QueryTiles          ( SweepLine* );
              bool      isProcessed         ( Component* ) const;
      virtual void      setBasicLayer       ( const BasicLayer* );
      virtual bool      hasGoCallback       () const;
      virtual void      goCallback          ( Go*     );
      virtual void      rubberCallback      ( Rubber* );
      virtual void      extensionGoCallback ( Go*     );
      virtual void      masterCellCallback  ();
      inline  uint32_t  getGoMatchCount     () const;
    private:
      SweepLine*   _sweepLine;
      uint32_t     _goMatchCount;
      Layer::Mask  _processedLayers;
  };


  inline  uint32_t  QueryTiles::getGoMatchCount () const { return _goMatchCount; }


}  // Tramontana namespace.

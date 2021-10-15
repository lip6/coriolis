// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      B o r a   -  A n a l o g   S l i c i n g   T r e e         |
// |                                                                 |
// |  Authors     :                         Eric LAO                 |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/NodeSets.h"                             |
// +-----------------------------------------------------------------+


#ifndef BORA_NODE_SETS_H
#define BORA_NODE_SETS_H

#include <algorithm>
#include <cstddef>
#include <limits>
#include "BoxSet.h"
#include "ParameterRange.h"

namespace Hurricane {
  class Cell;
}

namespace CRL {
  class RoutingGauge;
}

namespace Bora {

  using std::size_t;
  using Hurricane::Cell;
  using CRL::RoutingGauge;


// -------------------------------------------------------------------
// Class  :  "Bora::NodeSets".
//
// Contains all the possible dimensions (BoxSet) for a SlicingNode


  class NodeSets 
  {
    public:
    //static const  size_t  NotFound = std::numeric_limits<size_t>()::max;
      static const  size_t  NotFound = (size_t)-1L;
    public:
                                                   NodeSets          ( ParameterRange *range );
                                                   NodeSets          ( const NodeSets* other );
                                                  ~NodeSets          ();
      static       NodeSets*                       create            ( Cell*              cell =NULL
                                                                     , ParameterRange*    range=NULL
                                                                     , CRL::RoutingGauge* rg   =NULL );
                   BoxSet*                         operator[]        ( size_t );
                   BoxSet*                         at                ( size_t );
      inline       std::vector<BoxSet*>::iterator  begin             ();
      inline       std::vector<BoxSet*>::iterator  end               ();
      inline const std::vector<BoxSet*>&           getBoxSets        () const;
      inline       std::vector<BoxSet*>&           getBoxSets        ();
      inline       size_t                          size              () const;
      inline       bool                            empty             () const;
                   void                            clear             ();
      inline       void                            sort              ();
                   BoxSet*                         getPairH          ( DbU::Unit height ) const;
                   BoxSet*                         getPairHW         ( DbU::Unit height, DbU::Unit width ) const;
                   BoxSet*                         getPairHW         ( unsigned int index ) const;
                   size_t                          findIndex         ( DbU::Unit height, DbU::Unit width ) const;
                   BoxSet*                         find              ( DbU::Unit height, DbU::Unit width );
                   BoxSet*                         find              ( BoxSet* boxSet );
                   BoxSet*                         find              ( size_t  index );
                   void                            print             () const;
                   bool                            compare           ( NodeSets nodeSets2, unsigned int flags=NoFlags ) const;
                   void                            push_back         ( BoxSet* boxSet );
                   void                            push_back         ( std::vector<BoxSet*> vect, DbU::Unit height, DbU::Unit width, unsigned int type );
                   void                            push_back         ( DbU::Unit height, DbU::Unit width );
                   NodeSets*                       clone             ();
      inline       ParameterRange*                 getRange          () const;
    private:
      std::vector<BoxSet*> _boxSets;
      ParameterRange*      _range;
  };


  inline       std::vector<BoxSet*>::iterator NodeSets::begin             ()       { return _boxSets.begin(); }
  inline       std::vector<BoxSet*>::iterator NodeSets::end               ()       { return _boxSets.end  (); }
  inline const std::vector<BoxSet*>&          NodeSets::getBoxSets        () const { return _boxSets;         }
  inline       std::vector<BoxSet*>&          NodeSets::getBoxSets        ()       { return _boxSets;         }
  inline       size_t                         NodeSets::size              () const { return _boxSets.size() ; }
  inline       bool                           NodeSets::empty             () const { return _boxSets.empty(); }
  inline       void                           NodeSets::sort              ()       { std::sort( _boxSets.begin(),_boxSets.end(), compBoxSet() ); }
  inline       ParameterRange*                NodeSets::getRange          () const { return _range; }


}  // Bora namespace.

#endif  // BORA_NODE_SET_H

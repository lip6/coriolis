// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/RHVSlicingNode.h"                       |
// +-----------------------------------------------------------------+


#ifndef BORA_RHV_SLICING_NODE_H
#define BORA_RHV_SLICING_NODE_H

#include "SlicingNode.h"


namespace Bora {


// -------------------------------------------------------------------
// Class  :  "Bora::RHVSlicingNode".


  class RHVSlicingNode: public SlicingNode
  {
    protected:
                                         RHVSlicingNode        ();
                                        ~RHVSlicingNode        ();
    public:                                                    
                    void                 print                 () const;
                    void                 _setGlobalSize        ( DbU::Unit height, DbU::Unit width = 0 );
             inline double               getDevicesArea        () const;
             inline bool                 isRoutingEstimated    () const;
                    unsigned int         getAlignment          () const;
                    NodeSets*            getNodeSets           () const;
                    BoxSet*              getPairH              ( DbU::Unit height ) const; 
                    BoxSet*              getPairHW             ( DbU::Unit height, DbU::Unit width ) const; 
                    void                 setPairH              ( DbU::Unit height ); 
                    void                 setPairHW             ( DbU::Unit height, DbU::Unit width ); 
      static inline int                  getCount              ();
      static inline void                 printCount            ();
      static inline void                 printCountAll         ();
                    void                 preDestroy            ();
                    void                 destroy               ();
                    void                 preRecursiveDestroy   ();
                    void                 recursiveDestroy      ();
                    void                 resetSize             ();
                    bool                 isSame                ( SlicingNode* , unsigned int flags=NoFlags ) const;
                    bool                 isSymmetric           ( SlicingNode* , unsigned int symmetryType, unsigned int flags=NoFlags ) const;
                    void                 _place                ( DbU::Unit x=0, DbU::Unit y=0, bool replace=false );
                    void                 setRailInstance       ( Hurricane::Instance* i );
             inline Hurricane::Instance* getRailInstance       () const ;
                    void                 addWireOccupation     ( DbU::Unit min,  DbU::Unit max, Net* );
                    void                 resetWireOccupation   ();
                    int                  getMaxWireOccupation  ();
    protected:
      static int                  _count;   
      static int                  _countAll;   
             Hurricane::Instance* _railInstance;
             DbU::Unit            _minHeight;
             DbU::Unit            _minWidth;
             ChannelRouting*      _wireOccupation;
      
  };


  inline double               RHVSlicingNode::getDevicesArea     () const { return 0; }
  inline int                  RHVSlicingNode::getCount           ()       { return _count; }
  inline void                 RHVSlicingNode::printCount         ()       { std::cerr << "RHVSlicingNode::Count    = " << _count    << std::endl; }
  inline void                 RHVSlicingNode::printCountAll      ()       { std::cerr << "RHVSlicingNode::CountAll = " << _countAll << std::endl; }
  inline bool                 RHVSlicingNode::isRoutingEstimated () const { return (_flags & RoutingEstimated); }
  inline Hurricane::Instance* RHVSlicingNode::getRailInstance    () const { return _railInstance; }


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::RHVSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::RHVSlicingNode)

#endif  // BORA_RHV_SLICING_NODE_H

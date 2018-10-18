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
// |  C++ Header  :  "./bora/RVSlicingNode.h"                        |
// +-----------------------------------------------------------------+


#ifndef BORA_RV_SLICING_NODE_H
#define BORA_RV_SLICING_NODE_H

#include "bora/RHVSlicingNode.h"


namespace Bora {


// -------------------------------------------------------------------
// Class  :  "Bora::RVSlicingNode".


  class RVSlicingNode: public RHVSlicingNode
  {
    private:
                             RVSlicingNode         ( DbU::Unit width );
                             RVSlicingNode         ( Hurricane::Instance* );
                            ~RVSlicingNode         ();
    public:
      static  RVSlicingNode* create                ( DbU::Unit width=DbU::Unit(0) );
      static  RVSlicingNode* create                ( Hurricane::Vertical* );
      static  RVSlicingNode* create                ( Hurricane::Net*   net
                                                   , Hurricane::Layer* layer
                                                   , int               npitch
                                                   , std::string       cname
                                                   , std::string       iname
                                                   );
              RVSlicingNode* clone                 ( unsigned int tr = None );
              DbU::Unit      getHeight             () const;
              void           setWidth              ( DbU::Unit width );
              void           estimateChannelsSize  ();
              void           _expandRoutingChannel ();
              void           expandRoutingChannel  ( DbU::Unit height, DbU::Unit width );
              void           setGCell              ( Anabatic::GCell* );
              void           updateMasterSize      ( DbU::Unit size ); 
      virtual std::string    _getTypeName          () const;
               void          setHeight             ( DbU::Unit );
  };


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::RVSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::RVSlicingNode)

#endif  // BORA_RV_SLICING_NODE_H

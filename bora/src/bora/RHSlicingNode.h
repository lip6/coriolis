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
// |  C++ Header  :  "./bora/RHSlicingNode.h"                        |
// +-----------------------------------------------------------------+


#ifndef BORA_RH_SLICING_NODE_H
#define BORA_RH_SLICING_NODE_H

#include "bora/RHVSlicingNode.h"


namespace Bora {


// -------------------------------------------------------------------
// Class  :  "Bora::RHSlicingNode".


  class RHSlicingNode: public RHVSlicingNode
  {
    private:
                             RHSlicingNode         ( DbU::Unit height );
                             RHSlicingNode         ( Hurricane::Instance* );
                            ~RHSlicingNode         ();
    public:
      static  RHSlicingNode* create                ( DbU::Unit         height=DbU::Unit(0) );
      static  RHSlicingNode* create                ( Hurricane::Net*   net
                                                   , Hurricane::Layer* layer
                                                   , int               npitch
                                                   , std::string       cname
                                                   , std::string       iname
                                                   );
              RHSlicingNode* clone                 ( unsigned int tr=None );
              
              DbU::Unit      getWidth              () const;
              void           setHeight             ( DbU::Unit );
              
              void           estimateChannelsSize  ();
              void           _expandRoutingChannel ();
              void           expandRoutingChannel  ( DbU::Unit height, DbU::Unit width );
              void           setGCell              ( Anabatic::GCell* );
              void           updateMasterSize      ( DbU::Unit size ); 
      virtual std::string    _getTypeName          () const;
              void           setWidth              ( DbU::Unit width );
  };


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::RHSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::RHSlicingNode)

#endif  // BORA_RH_SLICING_NODE_H

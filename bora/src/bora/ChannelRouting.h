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
// |  C++ Header  :  "./bora/ChannelRouting.h"                       |
// +-----------------------------------------------------------------+


#ifndef BORA_CHANNEL_ROUTING_H
#define BORA_CHANNEL_ROUTING_H


#include "hurricane/IntervalTree.h"
namespace Hurricane {
  class Net;
}

namespace Bora {

  using Hurricane::DbU;
  using Hurricane::IntervalData;
  using Hurricane::IntervalTree;
  using Hurricane::Net;


// -------------------------------------------------------------------
// Class  :  "Bora::ChannelRouting".

  
  class ChannelRouting 
  {
    private:
      typedef  IntervalData<Net*>  WireInterval;
      typedef  IntervalTree<Net*>  WireIntervals;
    private:
                             ChannelRouting ();
                            ~ChannelRouting ();
    public:
      static ChannelRouting* create         ();
             void            destroy        ();
             int             getMaxCount    () const; 
             void            insertChannel  ( DbU::Unit xy1, DbU::Unit xy2, Net* );
             void            insertChannel  ( DbU::Unit xy1, DbU::Unit xy2, unsigned int w );
             void            reset          ();
             void            print          () const;
    private:
      WireIntervals  _wireIntervals;
  };


}  // Bora namespace.

#endif  // BORA_CHANNEL_ROUTING_H

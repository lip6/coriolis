// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CommonGatePair.h"           |
// +-----------------------------------------------------------------+


#ifndef ANALOG_COMMON_GATE_PAIR_H
#define ANALOG_COMMON_GATE_PAIR_H

#include "hurricane/analog/TransistorPair.h"

namespace Analog {


  class CommonGatePair : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static  CommonGatePair* create            ( Hurricane::Library*
                                                , const Hurricane::Name&
                                                , const Type&
                                                , bool bulkConnected );
      virtual Hurricane::Name getDeviceName     () const;
    protected:
                              CommonGatePair    ( Hurricane::Library*, const Hurricane::Name&, const Type& );
      virtual void            createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name  _cgBulkConnectedName;
      static const Hurricane::Name  _cgBulkUnconnectedName;
  };


}  // Analog namespace.

#endif  // ANALOG_COMMON_GATE_PAIR_H

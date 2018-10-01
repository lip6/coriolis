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
// |  C++ Header  :  "./hurricane/analog/CommonDrain.h"              |
// +-----------------------------------------------------------------+


#ifndef ANALOG_COMMON_DRAIN_H
#define ANALOG_COMMON_DRAIN_H

#include "hurricane/analog/TransistorPair.h"


namespace Analog {


  class CommonDrain : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static  CommonDrain*    create            ( Hurricane::Library*
                                                , const Hurricane::Name&
                                                , const Type&
                                                , bool bulkConnected );
      virtual Hurricane::Name getDeviceName     () const;
    protected:                                  
                              CommonDrain       ( Hurricane::Library*, const Hurricane::Name&, const Type& );
    private:                                    
      virtual void            createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name   _cdBulkConnectedName;
      static const Hurricane::Name   _cdBulkUnconnectedName;
  };


}  // Analog namespace.

#endif  // ANALOG_COMMON_DRAIN_H

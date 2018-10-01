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
// |  C++ Header  :  "./hurricane/analog/CrossCoupledPair.h"         |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CROSS_COUPLED_PAIR_H
#define ANALOG_CROSS_COUPLED_PAIR_H

#include "hurricane/analog/TransistorPair.h"

namespace Analog {


  class CrossCoupledPair : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static CrossCoupledPair* create            ( Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const Type& type
                                                 , bool bulkConnected );
      virtual Hurricane::Name  getDeviceName     () const;
    protected:                                   
                               CrossCoupledPair  ( Hurricane::Library*, const Hurricane::Name&, const Type& );
    private:                                     
              void             createConnections ( bool bulkConnected );
    private:                                     
      static const Hurricane::Name  _ccpBulkConnectedName;
      static const Hurricane::Name  _ccpBulkUnconnectedName;
  };


}  // Analog namespace.

#endif  // ANALOG_CROSS_COUPLED_PAIR_H

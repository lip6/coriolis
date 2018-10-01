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
// |  C++ Header  :  "./hurricane/analog/DifferentialPair.h"         |
// +-----------------------------------------------------------------+


#ifndef ANALOG_DIFFERENTIAL_PAIR_H
#define ANALOG_DIFFERENTIAL_PAIR_H

#include "hurricane/analog/TransistorPair.h"

namespace Analog {


  class DifferentialPair : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static  DifferentialPair* create            ( Hurricane::Library*
                                                  , const Hurricane::Name&
                                                  , const Type&
                                                  , bool bulkConnected );
      virtual Hurricane::Name   getDeviceName     () const;
                unsigned int    getRestriction    ( Hurricane::Net* net ) const;
                bool            isSame            ( DifferentialPair* dp );
    protected:                                    
                                DifferentialPair  ( Hurricane::Library*, const Hurricane::Name&, const Type& );
    private:
              void              createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name   _dpBulkConnectedName;
      static const Hurricane::Name   _dpBulkUnconnectedName;
  };


}  // Analog namespace.

#endif  // ANALOG_DIFFERENTIAL_PAIR_H

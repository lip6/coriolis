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
// |  C++ Header  :  "./hurricane/analog/SimpleCurrentMirror.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_SIMPLE_CURRENT_MIRROR_H
#define ANALOG_SIMPLE_CURRENT_MIRROR_H

#include "hurricane/analog/TransistorPair.h"

namespace Analog {


  class SimpleCurrentMirror : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static  SimpleCurrentMirror* create              ( Hurricane::Library*
                                                       , const Hurricane::Name&
                                                       , const Type&
                                                       , bool bulkConnected );
      virtual Hurricane::Name      getDeviceName       () const;
              unsigned int         getRestriction      ( Hurricane::Net* net ) const;
  
    protected:
                                   SimpleCurrentMirror ( Hurricane::Library*, const Hurricane::Name&, const Type& );
      virtual void                 createConnections   ( bool bulkConnected );
    private:
      static const Hurricane::Name   _scmBulkConnectedName;
      static const Hurricane::Name   _scmBulkUnconnectedName;
  };


}  // Analog namespace.

#endif  // ANALOG_SIMPLE_CURRENT_MIRROR_H

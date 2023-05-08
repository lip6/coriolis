// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2009-2023, All Rights Reserved
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


#pragma  once
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
      virtual std::string     _getTypeName      () const;
      virtual Record*         _getRecord        () const;
    protected:
                              CommonGatePair    ( Hurricane::Library*, const Hurricane::Name&, const Type& );
      virtual void            createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name  _cgBulkConnectedName;
      static const Hurricane::Name  _cgBulkUnconnectedName;
  };


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::CommonGatePair);

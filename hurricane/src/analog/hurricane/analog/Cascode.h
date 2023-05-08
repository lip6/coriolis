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
// |  C++ Header  :  "./hurricane/analog/Cascode.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/analog/TransistorPair.h"

namespace Analog {


  class Cascode : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static  Cascode*         create            ( Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const Type&
                                                 , bool bulkConnected );
      virtual Hurricane::Name  getDeviceName     () const;
      virtual std::string     _getTypeName       () const;
      virtual Record*         _getRecord         () const;
    protected:                                   
                               Cascode           ( Hurricane::Library*, const Hurricane::Name&, const Type& );
    private:
      virtual void             createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name   _cBulkConnectedName;
      static const Hurricane::Name   _cBulkUnconnectedName;
  };


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::Cascode);


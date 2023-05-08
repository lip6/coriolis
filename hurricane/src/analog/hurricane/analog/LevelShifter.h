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
// |  C++ Header  :  "./hurricane/analog/LevelShifter.h"             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/analog/TransistorPair.h"

namespace Analog {

  class LevelShifter : public TransistorPair {
    public:
      typedef TransistorPair Super;
    public:
      static LevelShifter*     create            ( Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const Type&
                                                 , bool bulkConnected );
      virtual Hurricane::Name  getDeviceName     () const;
      virtual std::string      _getTypeName      () const;
      virtual Record*          _getRecord        () const;
    protected:
                               LevelShifter      ( Hurricane::Library*, const Hurricane::Name&, const Type& );
      virtual void             createConnections ( bool bulkConnected );
    private:
      static const Hurricane::Name   _lsBulkConnectedName;
      static const Hurricane::Name   _lsBulkUnconnectedName;
  };


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::LevelShifter);

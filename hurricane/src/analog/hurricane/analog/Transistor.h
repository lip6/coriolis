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
// |  C++ Header  :  "./hurricane/analog/Transistor.h"               |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/analog/TransistorFamily.h"

namespace Analog {


  class Transistor : public TransistorFamily {
    public:
      typedef TransistorFamily Super;
    public:
      static  Transistor*     create            ( Hurricane::Library*
                                                , const Hurricane::Name&
                                                , const Type&
                                                , bool bulkConnected );
      virtual Hurricane::Name getDeviceName     () const;
              unsigned int    getRestriction    ( Hurricane::Net* net ) const;
              bool            isSame            ( Transistor* );
  
    protected:
                              Transistor        ( Hurricane::Library* , const Hurricane::Name& , const Type& );
      virtual void            _postCreate       ( const Hurricane::Name& deviceName );
      virtual void            createConnections ( bool bulkConnected );
      virtual std::string     _getTypeName      () const;
    private:
      static const Hurricane::Name  _transistorBulkConnectedName;
      static const Hurricane::Name  _transistorBulkUnconnectedName;
                   MetaTransistor*  _metaTransistor;
  };


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::Transistor);

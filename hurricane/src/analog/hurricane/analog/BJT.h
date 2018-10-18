// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                   Stephanie Youssef              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/BJT.h"                      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_BJT_H
#define ANALOG_BJT_H

#include "hurricane/DbU.h"
#include "hurricane/analog/BJTFamily.h"


namespace Analog {


  class BJT : public BJTFamily {
    public:
      typedef BJTFamily Super;
    public:
      static  BJT*             create            ( Hurricane::Library* , const Hurricane::Name& , const Type& );
              unsigned int     getN              () const;
              unsigned int     getM              () const;
      virtual Hurricane::Name  getDeviceName     () const;
              void             setExternalDummy  ( const bool );
              bool             getExternalDummy  () const;
    protected:                                   
                               BJT               ( Hurricane::Library* , const Hurricane::Name& , const Type& );
      virtual void             _postCreate       ( const Hurricane::Name& deviceName );
      virtual void             createConnections ();
    private:
      static const Hurricane::Name      _bjt;
                   FormFactorParameter* _n;
                   FormFactorParameter* _m;
                   MCheckBoxParameter*  _externalDummy;
  };


}  // Analog namespace.

#endif  // ANALOG_BJT_H

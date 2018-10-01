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
// |  C++ Header  :  "./hurricane/analog/BJTFamily.h"                |
// +-----------------------------------------------------------------+


#ifndef ANALOG_BJT_FAMILY_H
#define ANALOG_BJT_FAMILY_H


namespace Hurricane {
  class Name;
  class Library;
}

#include "hurricane/analog/Device.h"


namespace Analog {


  class BJTFamily : public Device {
    public:
      typedef Device Super;
      enum Type { NPN=1, PNP };
    public:
      inline  Type const& getType          () const;
      inline  int         getOperatorIndex () const;
      inline  void        setOperatorIndex ( int );
    protected:
                          BJTFamily         ( Hurricane::Library* , const Hurricane::Name& , const Type& );
      virtual void        createConnections () = 0;
    private:
      const Type  _type;
            int   _operatorIndex;
  };

  
  inline  BJTFamily::Type const& BJTFamily::getType          () const  { return _type; }
  inline  int                    BJTFamily::getOperatorIndex () const  { return _operatorIndex; }
  inline  void                   BJTFamily::setOperatorIndex ( int i ) { _operatorIndex = i; }


}  // Analog namespace.

#endif  // ANALOG_BJT_FAMILY_H

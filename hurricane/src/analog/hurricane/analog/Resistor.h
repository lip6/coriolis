// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/Resistor.h"                 |
// +-----------------------------------------------------------------+


#ifndef ANALOG_RESISTOR_H
#define ANALOG_RESISTOR_H

#include "hurricane/analog/ResistorFamily.h"

namespace Analog {


  class Resistor : public ResistorFamily {
    public:
      typedef ResistorFamily Super;
    public:
      static  Resistor*       create            ( Hurricane::Library*
                                                , const Hurricane::Name&
                                                , const Type& );
      virtual Hurricane::Name getDeviceName     () const;
              unsigned int    getRestriction    ( Hurricane::Net* ) const;
              bool            isSame            ( Resistor* );
  
    protected:
                              Resistor          ( Hurricane::Library* , const Hurricane::Name& , const Type& );
      virtual void            _postCreate       ( const Hurricane::Name& deviceName );
      virtual void            createConnections ();
    private:
      static const Hurricane::Name  _resistorName;
                   MetaResistor*    _metaResistor;
  };


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::Resistor);

#endif // ANALOG_RESISTOR_H

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CapacitorFamily.h"          |
// +-----------------------------------------------------------------+

#ifndef ANALOG_CAPACITOR_FAMILY_H
#define ANALOG_CAPACITOR_FAMILY_H

#include "hurricane/analog/Device.h"
#include "hurricane/analog/MetaCapacitor.h"

namespace Hurricane {
    class Name;
    class Library;
}


namespace Analog {

  class MetaCapacitor;
  

  class CapacitorFamily : public Device {
      public:
          typedef Device Super;
          enum Type { PIP = 1, MIM, MOM };
      public:
                       void           setReferenceCapacitor        (const Hurricane::Name& referenceCapacitorName);
          inline       MetaCapacitor* getReferenceCapacitor        ();
          inline const MetaCapacitor* getReferenceCapacitor        () const;
        // Structural parameters.
          inline const Type&          getType                      () const;
          inline       bool           isMIM                        () const;
          inline       bool           isPIP                        () const;
          inline       bool           isMOM                        () const;
        // Geometrical parameters.
        // Electrical parameters.
          inline       double         getCE                        () const;
          inline       void           setCE                        ( double );
      protected:
                                      CapacitorFamily              ( Hurricane::Library* , const Hurricane::Name& , const Type& );
          inline       void           setReferenceCapacitor        ( MetaCapacitor* );
          virtual      void           createConnections            () = 0;
      private:
          inline       MetaCapacitor* _secureGetReferenceCapacitor ();
          inline const MetaCapacitor* _secureGetReferenceCapacitor () const;
  
      private:
          const Type           _type;
                MetaCapacitor* _referenceCapacitor;
  };
  
  inline       MetaCapacitor*         CapacitorFamily::getReferenceCapacitor ()       { return _referenceCapacitor; }
  inline const MetaCapacitor*         CapacitorFamily::getReferenceCapacitor () const { return _referenceCapacitor; }
  inline       void                   CapacitorFamily::setReferenceCapacitor ( MetaCapacitor* metaCapacitor ) { _referenceCapacitor = metaCapacitor; }
  inline const CapacitorFamily::Type& CapacitorFamily::getType               () const { return _type; }
  inline       bool                   CapacitorFamily::isMIM                 () const { return getType() == MIM; }
  inline       bool                   CapacitorFamily::isPIP                 () const { return getType() == PIP; }
  inline       bool                   CapacitorFamily::isMOM                 () const { return getType() == MOM; }
  inline       double                 CapacitorFamily::getCE                 () const { return _secureGetReferenceCapacitor()->getCE(); }
  inline       void                   CapacitorFamily::setCE                 (double ce) { _secureGetReferenceCapacitor()->setCE(ce); }
  
  
  inline MetaCapacitor* CapacitorFamily::_secureGetReferenceCapacitor ()
  {
    if (not _referenceCapacitor) throw Hurricane::Error("No MetaCapacitor");
    return _referenceCapacitor;
  }
  
  inline const MetaCapacitor* CapacitorFamily::_secureGetReferenceCapacitor () const
  {
    if (not _referenceCapacitor) throw Hurricane::Error("No MetaCapacitor");
    return _referenceCapacitor;
  }


} // Analog namespace.

#endif // ANALOG_CAPACITOR_FAMILY_H

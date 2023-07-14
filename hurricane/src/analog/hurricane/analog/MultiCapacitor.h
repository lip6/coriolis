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
// |  C++ Header  :  "./hurricane/analog/MultiCapacitor.h"           |
// +-----------------------------------------------------------------+


#ifndef ANALOG_MULTI_CAPACITOR_H
#define ANALOG_MULTI_CAPACITOR_H

#include "hurricane/DbU.h"
#include "hurricane/analog/CapacitiesParameter.h"
#include "hurricane/analog/MCheckBoxParameter.h"
#include "hurricane/analog/MatrixParameter.h"
#include "hurricane/analog/CapacitorFamily.h"

namespace Analog {

  class MetaCapacitor;


  class MultiCapacitor : public CapacitorFamily {
    public:
      typedef CapacitorFamily Super;
    public:
      static  MultiCapacitor*  create            (       Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const CapacitorFamily::Type&
                                                 ,       size_t  count           );
      inline  bool             hasDummy          () const;
      inline  double           getCapacity       ( size_t ) const;
      inline  void             setDummy          ( bool );
      virtual Hurricane::Name  getDeviceName     () const;
    protected:
                               MultiCapacitor    (       Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const CapacitorFamily::Type&
                                                 ,       size_t );
      virtual void             _postCreate       ( const Hurricane::Name& deviceName );
      virtual void             createConnections ();
      virtual unsigned int     getRestriction    ( Hurricane::Net* ) const;
    public:
      virtual std::string      _getTypeName      () const;
    private:
      static const Hurricane::Name      _capacitorName;
                   MetaCapacitor*       _metaCapacitor;
                   MCheckBoxParameter*  _dummy;
                   size_t               _count;
  };
  

  bool    MultiCapacitor::hasDummy    () const           { return (_dummy->getValue() == 1); }
  double  MultiCapacitor::getCapacity ( size_t i ) const { return static_cast<const CapacitiesParameter*>(getParameter("capacities"))->getValue(i); }
  void    MultiCapacitor::setDummy    ( bool state )     { _dummy->setValue( (state)?1:0 ); }


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::MultiCapacitor);

#endif // ANALOH_MULTI_CAPACITOR_H

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
      inline  double           getCapacity       ( size_t ) const;
      inline  double           getCDraw          () const;
      inline  double           getCBorder        () const;
      inline  double           getCParasite      () const;
      virtual Hurricane::Name  getDeviceName     () const;
    protected:
                               MultiCapacitor    (       Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const CapacitorFamily::Type&
                                                 ,       size_t );
      virtual void             _postCreate       ( const Hurricane::Name& deviceName );
      virtual void             createConnections ();
    private:
      static const Hurricane::Name      _capacitorName;
                   MetaCapacitor*       _metaCapacitor;
                   MatrixParameter*     _matrix;
                   size_t               _count;
                   double               _cDraw;
                   double               _cBorder;
                   double               _cParasite;
                   CapacitiesParameter* _capacities;
  };
  

  double  MultiCapacitor::getCapacity   ( size_t i ) const { return _capacities->getValue(i); }
  double  MultiCapacitor::getCDraw      () const { return _cDraw; }
  double  MultiCapacitor::getCBorder    () const { return _cBorder; }
  double  MultiCapacitor::getCParasite  () const { return _cParasite; }


}  // Analog namespace.

#endif // ANALOH_MULTI_CAPACITOR_H

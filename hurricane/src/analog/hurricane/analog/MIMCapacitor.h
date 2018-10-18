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
// |  C++ Header  :  "./hurricane/analog/MIMCapacitor.h"             |
// +-----------------------------------------------------------------+


#ifndef ANALOG_MIM_CAPACITOR_H
#define ANALOG_MIM_CAPACITOR_H

#include "hurricane/DbU.h"
#include "hurricane/analog/CapacitorFamily.h"

namespace Analog {

  class MetaCapacitor;


  class MIMCapacitor : public CapacitorFamily {
    public:
      typedef CapacitorFamily Super;
    public:
      static  MIMCapacitor*    create            (       Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const CapacitorFamily::Type& type = MIM );
      inline  double           getC              () const;
      inline  double           getCDraw          () const;
      inline  double           getCBorder        () const;
      inline  double           getCParasite      () const;
      virtual Hurricane::Name  getDeviceName     () const;
    protected:
                               MIMCapacitor      (       Hurricane::Library*
                                                 , const Hurricane::Name&
                                                 , const CapacitorFamily::Type& type = MIM );
      virtual void             _postCreate       ( const Hurricane::Name& deviceName );
      virtual void             createConnections ();
    private:
      static const Hurricane::Name     _capacitorName;
                   MetaCapacitor*      _metaCapacitor;
                   double              _cDraw;
                   double              _cBorder;
                   double              _cParasite;
                   CapacitorParameter* _c;
  };
  
  double  MIMCapacitor::getC         () const { return _c->getValue(); }
  double  MIMCapacitor::getCDraw     () const { return _cDraw; }
  double  MIMCapacitor::getCBorder   () const { return _cBorder; }
  double  MIMCapacitor::getCParasite () const { return _cParasite; }


}  // Analog namespace.

#endif // ANALOH_MIM_CAPACITOR_H

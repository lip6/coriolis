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
// |  C++ Header  :  "./hurricane/analog/MetaCapacitor.h"            |
// +-----------------------------------------------------------------+


#ifndef ANALOG_METACAPACITOR_H
#define ANALOG_METACAPACITOR_H

#include "hurricane/Cell.h"


namespace Analog {
  
  using namespace Hurricane;


  class MetaCapacitor : public Cell {
    public:
      typedef Cell Super;
    public:
      static MetaCapacitor* create        ( Library* library, const Name& );
      inline Net*           getTopPlate   ();
      inline Net*           getBotPlate   ();
      inline double         getCE         () const;
      inline void           setCE         ( double );
    protected:
             void           _postCreate   ();
    private:
                            MetaCapacitor ( Library* , const Name& );
    private:
      Net*    _topPlate;
      Net*    _botPlate;
      double  _ce;
  };
  

  inline Net*    MetaCapacitor::getTopPlate ()            { return _topPlate; }
  inline Net*    MetaCapacitor::getBotPlate ()            { return _botPlate; }
  inline double  MetaCapacitor::getCE       () const      { return _ce; }
  inline void    MetaCapacitor::setCE       ( double ce ) { _ce = ce; }


}  // Analog namespace.

#endif // ANALOG_METACAPACITOR_H

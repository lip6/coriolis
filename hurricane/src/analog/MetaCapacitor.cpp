// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MetaCapacitor.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MetaCapacitor.h"


namespace Analog {


  MetaCapacitor::MetaCapacitor ( Library* library, const Name& name )
      : Super(library,name)
      , _plate1(NULL)
      , _plate2(NULL)
      , _ce    (0.0)
  { }
  
  
  MetaCapacitor* MetaCapacitor::create ( Library* library, const Name& name )
  {
      MetaCapacitor* mCapacitor = new MetaCapacitor( library, name );
      mCapacitor->_postCreate();
  
      return mCapacitor;
  }
  
  
  void MetaCapacitor::_postCreate ()
  {
    Super::_postCreate();
  
    _plate1 = Net::create(this, "P1");
    _plate1->setExternal(true);
    _plate2 = Net::create(this, "P2");
    _plate2->setExternal(true);
  
    setTerminal( false );
  }


}  // Analog namespace.

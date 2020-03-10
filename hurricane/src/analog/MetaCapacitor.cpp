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
    , _topPlate(NULL)
    , _botPlate(NULL)
    , _ce      (0.0)
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
  
    _topPlate = Net::create( this, "T" );
    _topPlate->setExternal( true );
    _botPlate = Net::create( this, "B" );
    _botPlate->setExternal( true );
  }


}  // Analog namespace.

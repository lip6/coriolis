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
// |  C++ Module  :  "./MetaResistor.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MetaResistor.h"


namespace Analog {


  MetaResistor::MetaResistor ( Library* library, const Name& name )
    : Super(library, name)
    , _pin1     (NULL)
    , _pin2     (NULL)
    , _anonymous(NULL)
    , _we       (0.0)
    , _le       (0.0)
    , _bends    (0) 
  { }
  

  MetaResistor* MetaResistor::create ( Library* library, const Name& name )
  {
    MetaResistor* mResistor = new MetaResistor( library, name );
    mResistor->_postCreate();
  
    return mResistor;
  }
  

  void MetaResistor::_postCreate ()
  {
    Super::_postCreate();
  
    _pin1 = Net::create(this, "PIN1");
    _pin1->setExternal(true);
    _pin2 = Net::create(this, "PIN2");
    _pin2->setExternal(true);
    _anonymous = Net::create(this, "Anonymous");
    _anonymous->setAutomatic( true );
  
    setTerminal(false);
  }


}  // Analog namespace.

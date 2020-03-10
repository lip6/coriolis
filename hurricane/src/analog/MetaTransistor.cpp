// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MetaTransistor.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MetaTransistor.h"


namespace Analog {


  MetaTransistor::MetaTransistor ( Library* library, const Name& name )
    : Super(library, name)
    , _drain    (NULL)
    , _source   (NULL)
    , _gate     (NULL)
    , _bulk     (NULL)
    , _anonymous(NULL)
    , _m        (0)
    , _we       (0.0)
    , _le       (0.0)
    , _ids      (0.0)
    , _vgs      (0.0)
    , _vds      (0.0)
    , _vbs      (0.0)
    , _vg       (0.0)
    , _vd       (0.0)
    , _vb       (0.0)
    , _vs       (0.0)
    , _veg      (0.0)
    , _vth      (0.0)
    , _wmin     (0.0)
    , _wmax     (0.0)
    , _nfing    (1) 
  { }
  

  MetaTransistor* MetaTransistor::create ( Library* library, const Name& name )
  {
    MetaTransistor* mTransistor = new MetaTransistor( library, name );
    mTransistor->_postCreate();
  
    return mTransistor;
  }
  

  void MetaTransistor::_postCreate ()
  {
    Super::_postCreate();
  
    _drain = Net::create(this, "D");
    _drain->setExternal(true);
    _source = Net::create(this, "S");
    _source->setExternal(true);
    _gate = Net::create(this, "G");
    _gate->setExternal(true);
    _bulk = Net::create(this, "B");
    _bulk->setExternal(true);
    _anonymous = Net::create(this, "Anonymous");
    _anonymous->setAutomatic( true );
  }


}  // Analog namespace.

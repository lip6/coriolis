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


  string  MetaTransistor::_getTypeName () const
  { return "MetaTransistor"; }


  Record*  MetaTransistor::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_drain"    , _drain     ) );
    record->add( getSlot("_source"   , _source    ) );
    record->add( getSlot("_gate"     , _gate      ) );
    record->add( getSlot("_bulk"     , _bulk      ) );
    record->add( getSlot("_anonymous", _anonymous ) );
    record->add( getSlot("_m"        , _m         ) );
    record->add( getSlot("_we"       , _we        ) );
    record->add( getSlot("_le"       , _le        ) );
    record->add( getSlot("_ids"      , _ids       ) );
    record->add( getSlot("_vgs"      , _vgs       ) );
    record->add( getSlot("_vds"      , _vds       ) );
    record->add( getSlot("_vbs"      , _vds       ) );
    record->add( getSlot("_vg"       , _vg        ) );
    record->add( getSlot("_vd"       , _vd        ) );
    record->add( getSlot("_vb"       , _vb        ) );
    record->add( getSlot("_vs"       , _vs        ) );
    record->add( getSlot("_veg"      , _veg       ) );
    record->add( getSlot("_vth"      , _vth       ) );
    record->add( getSlot("_wmin"     , _wmin      ) );
    record->add( getSlot("_wmax"     , _wmax      ) );
    record->add( getSlot("_nfing"    , _nfing     ) );
    return record;
  }


}  // Analog namespace.

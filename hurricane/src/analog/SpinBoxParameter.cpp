// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2023-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SpinBoxParameter.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/analog/SpinBoxParameter.h"


namespace Analog {

  using std::string;


  string  SpinBoxParameter::_getTypeName () const
  { return "SpinBoxParameter"; }

  
  std::string  SpinBoxParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_value) );
    return s;
  }

  
  Record* SpinBoxParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_min"  , _min   ) );
    record->add( getSlot( "_max"  , _max   ) );
    record->add( getSlot( "_value", _value ) );
    return record;
  }


} // Analog namespace.

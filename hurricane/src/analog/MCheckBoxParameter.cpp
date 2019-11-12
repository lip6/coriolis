// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MCheckBoxParameter.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MCheckBoxParameter.h"


namespace Analog {

  using std::string;


  string  MCheckBoxParameter::_getTypeName () const
  { return "MCheckBoxParameter"; }

  
  std::string  MCheckBoxParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_value) );
    return s;
  }

  
  Record* MCheckBoxParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_choices", &_choices ) );
    record->add( getSlot( "_value"  , &_value   ) );
    return record;
  }




}  // Analog namespace.

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
// |  C++ Module  :  "./StepParameter.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/analog/StepParameter.h"


namespace Analog {

  using std::string;


  string  StepParameter::_getTypeName () const
  { return "StepParameter"; }

  
  std::string  StepParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_value)
            +" ["+getString(_min)+".."+getString(_max)+"/"+getString(_step)+"]" );
    return s;
  }

  
  Record* StepParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_min"  , &_min   ) );
    record->add( getSlot( "_max"  , &_max   ) );
    record->add( getSlot( "_step" , &_step  ) );
    record->add( getSlot( "_value", &_value ) );
    return record;
  }




}  // Analog namespace.

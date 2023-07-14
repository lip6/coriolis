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
// |  C++ Module  :  "./FormFactorParameter.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/analog/FormFactorParameter.h"


namespace Analog {

  using std::string;


  string  FormFactorParameter::_getTypeName () const
  { return "FormFactorParameter"; }

  
  std::string  FormFactorParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_value)+" ["+getString(_min)+".."+getString(_max)+"]" );
    return s;
  }

  
  Record* FormFactorParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_min"  , &_min   ) );
    record->add( getSlot( "_max"  , &_max   ) );
    record->add( getSlot( "_value", &_value ) );
    return record;
  }




}  // Analog namespace.

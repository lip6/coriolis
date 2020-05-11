// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./StringParameter.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/analog/StringParameter.h"


namespace Analog {

  using std::string;


  string  StringParameter::_getTypeName () const
  { return "StringParameter"; }

  
  std::string  StringParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " \""+_value+"\"" );
    return s;
  }

  
  Record* StringParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_value", &_value ) );
    return record;
  }




}  // Analog namespace.

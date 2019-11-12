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
// |  C++ Module  :  "./CapacitiesParameter.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/analog/CapacitiesParameter.h"


namespace Analog {

  using std::string;


  string  CapacitiesParameter::_getTypeName () const
  { return "CapacitiesParameter"; }

  
  std::string  CapacitiesParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " [" );
    for ( size_t i=0 ; i<_values.size() ; ++i ) {
      if (i) s.insert( s.size()-1, " " );
      s.insert( s.size()-1, getString(_values[i]) );
    }
    s.insert( s.size()-1, "]" );
    return s;
  }

  
  Record* CapacitiesParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_values", &_values   ) );
    return record;
  }




}  // Analog namespace.

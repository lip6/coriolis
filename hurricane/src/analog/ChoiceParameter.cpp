// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ChoiceParameter.cpp"                         |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/analog/ChoiceParameter.h"


namespace Analog {
  
  using namespace std;


  void ChoiceParameter::setValue ( const string& value )
  {
    unsigned index = _choices.findIndexFor( value );
    _value = index;
  }


  string ChoiceParameter::getValue () const
  {
    if (_value >= _choices._values.size()) {
      cerr << "[WARNING] ChoiceParameter::getValue(): \"value\" is out of choices size." << endl;
      return "";
    }
    return _choices._values[ _value ];
  }

  
  std::string  ChoiceParameter::_getTypeName () const
  { return "ChoiceParameter"; }


  std::string  ChoiceParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(_value) );
    return s;
  }


  Record* ChoiceParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_choices", &_choices ) );
    record->add( getSlot( "_value"  , &_value   ) );
    return record;
  }


}  // Analog namespace.

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./UnitRule.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/UnitRule.h"


namespace Hurricane {

  using namespace std;


  UnitRule::~UnitRule () { }


  string UnitRule::_getTypeName () const { return "UnitRule"; }
  string UnitRule::_getString   () const { return "<" + _getTypeName() + " " + getString(getName()) + ">"; }

  Record* UnitRule::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("Value", &_value) );
    return record;
  }


}  // Hurricane namespace.

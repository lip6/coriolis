// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PhysicalRule.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/PhysicalRule.h"


namespace Hurricane {

  using namespace std;


  PhysicalRule::~PhysicalRule () { }


  string PhysicalRule::_getTypeName () const { return "PhysicalRule"; }
  string PhysicalRule::_getString   () const { return "<" + _getTypeName() + " " + getString(getName()) + ">"; }

  Record* PhysicalRule::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_value", &_value ) );
    return record;
  }


}  // Hurricane namespace.

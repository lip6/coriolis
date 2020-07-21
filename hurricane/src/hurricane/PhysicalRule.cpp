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


  string RuleStep::_getTypeName () const { return "RuleStep"; }
  string RuleStep::_getString   () const { return "<" + _getTypeName() + ">"; }

  Record* RuleStep::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( DbU::getValueSlot("_hValue"   , &_hValue   ) );
    record->add( DbU::getValueSlot("_vValue"   , &_vValue   ) );
    record->add( DbU::getValueSlot("_threshold", &_threshold) );
    return record;
  }


  PhysicalRule::~PhysicalRule () { }


  string PhysicalRule::_getTypeName () const { return "PhysicalRule"; }
  string PhysicalRule::_getString   () const
  { return "<"+_getTypeName()+" \""+getString(getName())+"\""+" \""+getReference()+"\">"; }

  Record* PhysicalRule::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_doubleValue", &_doubleValue ) );
    record->add( getSlot( "_stepsValue" , &_stepsValue  ) );
    record->add( getSlot( "_symmetric"   , _symmetric   ) );
    return record;
  }


}  // Hurricane namespace.

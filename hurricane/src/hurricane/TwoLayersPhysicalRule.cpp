// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./TwoLayersPhysicalRule.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/TwoLayersPhysicalRule.h"


namespace Hurricane {

  using namespace std;


  TwoLayersPhysicalRule::~TwoLayersPhysicalRule () { }

  string TwoLayersPhysicalRule::_getTypeName () const { return "TwoLayersPhysicalRule"; }
  string TwoLayersPhysicalRule::_getString   () const { return "<" + _getTypeName() + " " + getString(getName()) + ">"; }

  Record* TwoLayersPhysicalRule::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("Symetric", _symetric) );
    return record;
  }


}  // Hurricane namespace.

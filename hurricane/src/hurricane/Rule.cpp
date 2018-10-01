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
// |  C++ Module  :  "./Rule.cpp"                                    |
// +-----------------------------------------------------------------+


#include "hurricane/Rule.h"


namespace Hurricane {

  using namespace std;


  Rule::~Rule () { }
  
  
  string  Rule::_getTypeName () const { return "Rule"; }
  string  Rule::_getString   () const { return "<" + _getTypeName() + " " + getString(_name) + ">"; }
  
  
  Record* Rule::_getRecord() const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot("_name"     , &_name     ) );
    record->add( getSlot("_reference", &_reference) );
    return record;
  }


}  // Hurricane namespace.

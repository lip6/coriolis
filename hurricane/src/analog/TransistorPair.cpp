// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./TransistorPair.cpp"                          |
// +-----------------------------------------------------------------+


#include <string>
#include <bitset>
#include "hurricane/analog/MetaTransistor.h"
#include "hurricane/analog/TransistorPair.h"

namespace Analog {
  
  using namespace std;


  TransistorPair::TransistorPair ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
    , _m1  (NULL)
    , _m2  (NULL)
    , _mInt(NULL)
  { }
  
  
  void  TransistorPair::_postCreate (const Name& deviceName )
  {
    Super::_postCreate(deviceName);
  
    _mInt = addSpinBoxParameter( "Mint", 1, 12 );
  }


  Record*  TransistorPair::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_m1"  ,  _m1   ) );
    record->add( getSlot("_m2"  ,  _m2   ) );
    record->add( getSlot("_mInt",  _mInt ) );
    return record;
  }

}  // Analog namespace.

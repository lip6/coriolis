// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ResistorFamily.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/PhysicalRule.h"
#include "hurricane/analog/MetaResistor.h"
#include "hurricane/analog/ResistorFamily.h"

namespace Analog {


  ResistorFamily::ResistorFamily ( Library* library, const Name& name, const Type& type )
    : Super             (library,name)
    , _type             (type)
    , _referenceResistor(NULL)
    , _w                (NULL) 
    , _l                (NULL)
    , _r                (NULL)
    , _bends            (NULL)
  { }
  
  
  void  ResistorFamily::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate(deviceName);
  
    DbU::Unit   resistorMinL = 0;        // techno->getPhysicalRule( "resistorMinL" ).getValue();
    DbU::Unit   resistorMaxL = 1000000;  // techno->getPhysicalRule( "resistorMaxL" ).getValue();
    DbU::Unit   resistorMinW = 0;        // techno->getPhysicalRule( "resistorMinW" ).getValue();
    DbU::Unit   resistorMaxW = 10000000; // techno->getPhysicalRule( "resistorMaxW" ).getValue();
  
    _w     = addStepParameter      ( "W"    , resistorMinW, resistorMaxW, DbU::grid(1) );
    _l     = addStepParameter      ( "L"    , resistorMinL, resistorMaxL, DbU::grid(1) );
    _r     = addFloatParameter     ( "R"    );
    _bends = addFormFactorParameter( "bends", 1, 10 );
  }


}  // Analog namespace.

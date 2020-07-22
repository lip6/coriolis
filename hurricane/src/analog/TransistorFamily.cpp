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
// |  C++ Module  :  "./TransistorFamily.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/PhysicalRule.h"
#include "hurricane/analog/MetaTransistor.h"
#include "hurricane/analog/TransistorFamily.h"

namespace Analog {


  TransistorFamily::TransistorFamily ( Library* library, const Name& name, const Type& type )
    : Super               (library,name)
    , _type               (type)
    , _referenceTransistor(NULL)
    , _operatorIndex      (0)
    , _w                  (NULL) 
    , _l                  (NULL)
    , _m                  (NULL)
    , _externalDummy      (NULL)
    , _sourceFirst        (NULL)
    , _bulkType           (NULL)
  { }
  
  
  void  TransistorFamily::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate(deviceName);
  
    Choices sourceChoices;
  //sourceChoices << "Yes" << "No";
    sourceChoices << "";
    _sourceFirst = addMCheckBoxParameter( "Source First", sourceChoices );
      
    Choices bulkChoices;
    bulkChoices << "North" << "South" << "East" << "West";
    _bulkType = addMCheckBoxParameter( "Bulk Type", bulkChoices );
  
    Technology* techno         = DataBase::getDB()->getTechnology();
    DbU::Unit   transistorMinL = techno->getPhysicalRule( "transistorMinL" )->getValue();
    DbU::Unit   transistorMaxL = techno->getPhysicalRule( "transistorMaxL" )->getValue();
    DbU::Unit   transistorMinW = techno->getPhysicalRule( "transistorMinW" )->getValue();
    DbU::Unit   transistorMaxW = techno->getPhysicalRule( "transistorMaxW" )->getValue();
  
    _w             = addStepParameter      ( "W", transistorMinW, transistorMaxW, DbU::grid(1) );
    _l             = addStepParameter      ( "L", transistorMinL, transistorMaxL, DbU::grid(1) );
    _m             = addFormFactorParameter( "M", 1, 10 );
    _externalDummy = addSpinBoxParameter   ( "ExternalDummy", 0, 6 );
    addStepParameter( "NERC" , 1, 1, 1 );
    addStepParameter( "NIRC" , 1, 1, 1 );
  }
  
  
#ifdef DISABLED
  void  TransistorFamily::setReferenceTransistor ( const Name& referenceTransistorName ) {
    Instance* instance = getInstance(referenceTransistorName);
    if (!instance) throw Error("No TransRef ...");
  
    Cell*           cell           = instance->getMasterCell();
    MetaTransistor* metaTransistor = dynamic_cast<MetaTransistor*>(cell);
    if (!metaTransistor) throw Error("Wrong type for TransRef...");
  
    _referenceTransistor = metaTransistor;
  }
#endif


}  // Analog namespace.

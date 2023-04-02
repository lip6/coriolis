// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./EquipotentialRelation.cpp"                  |
// +-----------------------------------------------------------------+


#include "tramontana/EquipotentialRelation.h"


namespace Tramontana {

  using std::string;
  using Hurricane::Property;
    

// -------------------------------------------------------------------
// Class  :  "Tramontana::EquipotentialRelation".
    
  const Name  EquipotentialRelationName = "EquipotentialRelation";


  EquipotentialRelation::EquipotentialRelation ( Equipotential* owner )
    : Super(owner)
  { }


  EquipotentialRelation* EquipotentialRelation::create ( Equipotential* owner )
  {
    EquipotentialRelation* relation = new EquipotentialRelation ( owner );
    relation->_postCreate();
    return relation;
  }


  void  EquipotentialRelation::_preDestroy ()
  { Super::_preDestroy(); }


  Name  EquipotentialRelation::getName () const
  { return EquipotentialRelationName; }


  string  EquipotentialRelation::_getTypeName () const
  { return "EquipotentialRelation"; }


  Record* EquipotentialRelation::_getRecord () const
  {
    Record* record = Super::_getRecord();
    return record;
  }


  EquipotentialRelation* EquipotentialRelation::get ( const Component* component )
  {
    if (not component) return nullptr;

    Property* property = component->getProperty( EquipotentialRelationName );
    if (not property) return nullptr;

    EquipotentialRelation* relation = dynamic_cast<EquipotentialRelation*>( property );
    if (not relation) return nullptr;
    return relation;
  }



}  // Tramontana namespace.


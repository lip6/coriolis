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
// |  C++ Header   :  "./tramontana/EquipotentialRelation.h"         |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Relation.h"


namespace Tramontana {

  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::Relation;
  using Hurricane::Component;
  class Equipotential;
    

// -------------------------------------------------------------------
// Class  :  "Tramontana::EquipotentialRelation".
  
  class EquipotentialRelation : public Relation {
    public:
      typedef  Relation  Super;
    public:
      static  EquipotentialRelation* get                   ( const Component* );
      static  EquipotentialRelation* create                ( Equipotential* );
      static  Name                   staticGetName         ();
    public:
      virtual Name                   getName               () const;
      virtual std::string            _getTypeName          () const;
      virtual Record*                _getRecord            () const;
    private:
                                     EquipotentialRelation ( Equipotential* );
    protected:
      virtual void                   _preDestroy           ();
  };


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::EquipotentialRelation);

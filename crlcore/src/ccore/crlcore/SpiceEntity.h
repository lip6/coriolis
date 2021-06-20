// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         S P I C E  / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/SpiceEntity.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
namespace Hurricane {
  class Net;
  class Instance;
}
#include "crlcore/ToolBox.h"
#include "crlcore/SpiceBit.h"


namespace Spice {

  using std::ostream;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Plug;
  using Hurricane::PrivateProperty;
  using CRL::NamingScheme;

  class Bit;
  class EntityProperty;


// -------------------------------------------------------------------
// Class  :  "Spice::Entity".


  class Entity {
    public:
      static std::vector<Entity*>&
                              getAllEntities  ();
      static void             orderPlugs      ( Instance*, std::vector<Plug*>& );
    public:           
                              Entity          ( EntityProperty*, Cell* );
                             ~Entity          ();
             const Cell*      getCell         () const;
     inline  const BitVector& getBits         () const;
             void             toNodeList      ( ostream&, bool asInterf=true ) const;
             void             toEntity        ( ostream& ) const;
             std::string      _getString      () const;
             Record*          _getRecord      () const;
    private:
      static std::vector<Entity*>  _entities;
      static std::ptrdiff_t        _offset;
             BitVector             _bits;
             size_t                _powerNode;
             size_t                _groundNode;
  };


  inline  const BitVector& Entity::getBits () const { return _bits; }


// -------------------------------------------------------------------
// Class  :  "Spice::EntityProperty".

  class EntityProperty : public Hurricane::PrivateProperty {
      friend class EntityExtension;
    public:
      static  Name  _name;
    public:
      static  EntityProperty* create          ( Cell* owner );
      static  Name            getPropertyName ();
      virtual Name            getName         () const;
      inline  Entity*         getEntity       ();
      virtual void            onReleasedBy    ( DBo* owner );
      virtual std::string     _getTypeName    () const;
      virtual std::string     _getString      () const;
      virtual Record*         _getRecord      () const;
    protected:
    // Attributes.
      Entity  _entity;
      
    protected:
    // Constructor.
      inline  EntityProperty ( Cell* owner );
  };


  inline  EntityProperty::EntityProperty ( Cell* owner )
    : PrivateProperty(), _entity(this,owner)
  { }


  inline Entity* EntityProperty::getEntity () { return &_entity; }


// -------------------------------------------------------------------
// Class  :  "Spice::EntityExtension".

  class EntityExtension {
    public:
      static        void             destroyAll  ();
      static        Entity*          get         ( const Cell* );
      static        Entity*          create      ( Cell* );
      static        void             destroy     ( Cell* );
    private:
      static const Cell*         _owner;
      static Entity*             _cache;
  };


}  // Spice Namespace.


INSPECTOR_P_SUPPORT(Spice::Entity);

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
      static const uint64_t  TopCell       = (1 << 0);
      static const uint64_t  ReferenceCell = (1 << 1);
    public:
      static std::vector<Entity*>&
                               getAllEntities  ();
      static void              orderPlugs      ( Instance*, std::vector<Plug*>& );
    public:           
                               Entity          ( EntityProperty*, Cell*, uint64_t flags );
                              ~Entity          ();
      inline        bool       isTopCell       () const;
      inline        bool       isReferenceCell () const;
      inline        uint64_t   getFlags        () const;
              const Cell*      getCell         () const;
      inline  const std::vector<Bit*>&
                               getBits         () const;
      inline        void       setFlags        ( uint64_t );
              void             setOrder        ( const std::vector<Net*>& );
              void             toNodeList      ( ostream&, bool asInterf=true ) const;
              void             toEntity        ( ostream& ) const;
              std::string      _getString      () const;
              Record*          _getRecord      () const;
    private:
              void             _recheckNets    ();
    private:
      static std::vector<Entity*>  _entities;
      static std::ptrdiff_t        _offset;
             std::vector<Bit*>     _bits;
             size_t                _powerNode;
             size_t                _groundNode;
             uint64_t              _flags;
  };


  inline        bool               Entity::isTopCell       () const { return _flags & TopCell; }
  inline        bool               Entity::isReferenceCell () const { return _flags & ReferenceCell; }
  inline        uint64_t           Entity::getFlags        () const { return _flags; }
  inline  const std::vector<Bit*>& Entity::getBits         () const { return _bits; }
  inline        void               Entity::setFlags        ( uint64_t flags ) { _flags |= flags; }


// -------------------------------------------------------------------
// Class  :  "Spice::EntityProperty".

  class EntityProperty : public Hurricane::PrivateProperty {
      friend class EntityExtension;
    public:
      static  Name  _name;
    public:
      static  EntityProperty* create          ( Cell* owner, uint64_t flags );
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
      inline  EntityProperty ( Cell* owner, uint64_t flags );
  };


  inline  EntityProperty::EntityProperty ( Cell* owner, uint64_t flags )
    : PrivateProperty(), _entity(this,owner,flags)
  { }


  inline Entity* EntityProperty::getEntity () { return &_entity; }


// -------------------------------------------------------------------
// Class  :  "Spice::EntityExtension".

  class EntityExtension {
    public:
      static        void             destroyAll  ();
      static        Entity*          get         ( const Cell* );
      static        Entity*          create      ( Cell*, uint64_t flags );
      static        void             destroy     ( Cell* );
    private:
      static const Cell*         _owner;
      static Entity*             _cache;
  };


}  // Spice Namespace.


INSPECTOR_P_SUPPORT(Spice::Entity);

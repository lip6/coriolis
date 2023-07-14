// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/VhdlEntity.h"                       |
// +-----------------------------------------------------------------+


#ifndef  VHDL_ENTITY_H
#define  VHDL_ENTITY_H

#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
namespace Hurricane {
  class Net;
  class Instance;
}
#include "crlcore/ToolBox.h"
#include "crlcore/VhdlSignal.h"


namespace Vhdl {

  using Hurricane::Tabulation;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::PrivateProperty;
  using CRL::NamingScheme;

  class Signal;
  class ScalarSignal;
  class VectorSignal;
  class EntityProperty;


  extern Tabulation  tab;


// -------------------------------------------------------------------
// Class  :  "Vhdl::Entity".

  typedef  std::set<Signal*,GreaterSignalByName>  SignalSet;


  class Entity {
    public:
      enum Flag { NoFlags              = 0x0000
                , EntityMode           = 0x0001
                , IeeeMode             = 0x0002
                , ComponentMode        = 0x0004
                , AsPortSignal         = 0x0008
                , AsInnerSignal        = 0x0010
                , VstUseConcat         = 0x0020
                , VstNoLowerCase       = 0x0040
                , VstUniquifyUpperCase = 0x0080
                , VstNoLinkage         = 0x0100
                , OptionMask           = VstUseConcat|VstNoLowerCase|VstUniquifyUpperCase|VstNoLinkage
                };
      const unsigned int  ModeMask = VstUseConcat|VstNoLowerCase|VstUniquifyUpperCase|VstNoLinkage;
    public:
      static std::vector<Entity*>&
                              getAllEntities  ();
    public:           
                              Entity          ( EntityProperty*, Cell*, unsigned int flags );
                             ~Entity          ();
      inline bool             isEntityMode    () const;
      inline bool             isIeeeMode      () const;
      inline bool             isComponentMode () const;
             const Cell*      getCell         () const;
             const Signal*    getSignal       ( std::string name ) const;
             const Signal*    getGlobal       ( std::string name ) const;
             const SignalSet* getSignals      () const;
             void             toEntityMode    ();
             void             toSignals       ( std::ostream& ) const;
             void             toPort          ( std::ostream& ) const;
             void             toComponent     ( std::ostream& ) const;
             void             toInstance      ( std::ostream&, Instance* ) const;
             void             toEntity        ( std::ostream& ) const;
             bool             parseNetName    ( const Net*, std::string& stem, size_t& index );
             std::string      _getString      () const;
             Record*          _getRecord      () const;
    private:
      static std::vector<Entity*>  _entities;
      static std::ptrdiff_t        _offset;
             NamingScheme          _ns;
             SignalSet             _signals;
             SignalSet             _globals;
             unsigned int          _flags;
  };


  inline bool  Entity::isIeeeMode      () const { return _flags & IeeeMode; }
  inline bool  Entity::isEntityMode    () const { return _flags & EntityMode; }
  inline bool  Entity::isComponentMode () const { return _flags & ComponentMode; }


// -------------------------------------------------------------------
// Class  :  "Vhdl::EntityProperty".

  class EntityProperty : public Hurricane::PrivateProperty {
      friend class EntityExtension;
    public:
      static  Name                _name;
    public:
      static  EntityProperty* create          ( Cell* owner, unsigned int flags );
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
      inline  EntityProperty ( Cell* owner, unsigned int flags );
  };


  inline  EntityProperty::EntityProperty ( Cell* owner, unsigned int flags )
    : PrivateProperty(), _entity(this,owner,flags)
  { }


  inline Entity* EntityProperty::getEntity () { return &_entity; }


// -------------------------------------------------------------------
// Class  :  "Vhdl::EntityExtension".

  class EntityExtension {
    public:
      static        void             destroyAll  ();
      static inline const SignalSet* getSignals  ( const Cell* );
      static inline const Signal*    getSignal   ( const Cell*, std::string name );
      static inline void             toSignals   ( const Cell*, std::ostream& );
      static inline void             toPort      ( const Cell*, std::ostream& );
      static inline void             toComponent ( const Cell*, std::ostream& );
      static inline void             toInstance  ( const Cell*, std::ostream&, Instance* );
      static inline void             toEntity    ( const Cell*, std::ostream& );
      static        Entity*          get         ( const Cell* );
      static        Entity*          create      ( Cell*, unsigned int flags );
      static        void             destroy     ( Cell* );
    private:
      static const Cell*         _owner;
      static Entity*             _cache;
  };


  inline const SignalSet* EntityExtension::getSignals ( const Cell* cell )
  {
    Entity* entity = get( cell );
    return (entity == NULL) ? NULL : entity->getSignals();
  }


  inline const Signal* EntityExtension::getSignal ( const Cell* cell, std::string name )
  {
    Entity* entity = get( cell );
    return (entity == NULL) ? NULL : entity->getSignal(name);
  }


  inline void  EntityExtension::toSignals ( const Cell* cell, std::ostream& out )
  {
    Entity* entity = get( cell );
    if (entity) entity->toSignals(out);
  }


  inline void  EntityExtension::toPort ( const Cell* cell, std::ostream& out )
  {
    Entity* entity = get( cell );
    if (entity) entity->toPort(out);
  }


  inline void  EntityExtension::toComponent ( const Cell* cell, std::ostream& out )
  {
    Entity* entity = get( cell );
    if (entity) entity->toComponent(out);
  }


  inline void  EntityExtension::toInstance ( const Cell* cell, std::ostream& out, Instance* instance )
  {
    Entity* entity = get( cell );
    if (entity) entity->toInstance(out,instance);
  }


  inline void  EntityExtension::toEntity ( const Cell* cell, std::ostream& out )
  {
    Entity* entity = get( cell );
    if (entity) entity->toEntity(out);
  }


}  // Vhdl Namespace.


INSPECTOR_P_SUPPORT(Vhdl::Entity);


#endif  // VHDL_ENTITY_H

// ****************************************************************************************************
// File: ./hurricane/Net.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#pragma  once
#include <functional>
#include "hurricane/Entity.h"
#include "hurricane/Nets.h"
#include "hurricane/Component.h"
#include "hurricane/Rubbers.h"
#include "hurricane/Rubber.h"
#include "hurricane/RoutingPads.h"
#include "hurricane/Plugs.h"
#include "hurricane/Pins.h"
#include "hurricane/Contacts.h"
#include "hurricane/Segments.h"
#include "hurricane/Verticals.h"
#include "hurricane/Horizontals.h"
#include "hurricane/Pads.h"
#include "hurricane/IntrusiveSet.h"
#include "hurricane/Path.h"
#include "hurricane/NetAlias.h"

namespace Hurricane {


// ****************************************************************************************************
// Net declaration
// ****************************************************************************************************

class Net : public Entity {
// **********************

// Types
// *****

    public: typedef Entity Inherit;

    public: typedef unsigned Arity;

    public: class Type {
    // ***************

        public: enum Code {UNDEFINED=0, LOGICAL=1, CLOCK=2, POWER=3, GROUND=4, BLOCKAGE=5, FUSED=6};

        private: Code _code;

        public: Type(const Code& code = UNDEFINED);
        public: Type(const Type& type);
        public: Type(string);

        public: Type& operator=(const Type& type);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Net::type"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    public: class Direction {
    // ********************

        public: enum Code { DirIn        = 0x0001
                          , DirOut       = 0x0002
                          , DirUndefined = 0x0000
                          , ConnTristate = 0x0100
                          , ConnWiredOr  = 0x0200
                          , UNDEFINED    = DirUndefined
                          , IN           = DirIn
                          , OUT          =         DirOut
                          , INOUT        = DirIn | DirOut
                          , TRISTATE     =         DirOut | ConnTristate
                          , TRANSCV      = DirIn | DirOut | ConnTristate
                          , WOR_OUT      =         DirOut | ConnWiredOr
                          , WOR_INOUT    = DirIn | DirOut | ConnWiredOr
                          , DirMask      = DirIn | DirOut | DirUndefined
                          };

        private: Code _code;

        public: Direction(const Code& code = UNDEFINED);
        public: Direction(const Direction& direction);
        public: Direction(string);

        public: Direction& operator =(const Direction& direction);
        public: Direction& operator|=(const Direction& direction);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Net::Direction"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    class ComponentSet : public IntrusiveSet<Component> {
    // ************************************************

        public: typedef IntrusiveSet<Component> Inherit;
    
        public: ComponentSet();

        public: virtual unsigned _getHashValue(Component* component) const;
        public: virtual Component* _getNextElement(Component* component) const;
        public: virtual void _setNextElement(Component* component, Component* nextComponent) const;

    };

    class RubberSet : public IntrusiveSet<Rubber> {
    // ******************************************

        public: typedef IntrusiveSet<Rubber> Inherit;
    
        public: RubberSet();

        public: virtual unsigned _getHashValue(Rubber* rubber) const;
        public: virtual Rubber* _getNextElement(Rubber* rubber) const;
        public: virtual void _setNextElement(Rubber* rubber, Rubber* nextRubber) const;

    };

// Attributes
// **********

    private: Cell* _cell;
    private: Name _name;
    private: Arity _arity;
    private: bool _isGlobal;
    private: bool _isExternal;
    private: bool _isAutomatic;
    private: Type _type;
    private: Direction _direction;
    private: Point _position;
    private: ComponentSet _componentSet;
    private: RubberSet _rubberSet;
    private: Net* _nextOfCellNetMap;
    private: NetMainName _mainName;

// Constructors
// ************

    protected: Net(Cell* cell, const Name& name);

    public: static Net* create(Cell* cell, const Name& name);

// Accessors
// *********

    public: virtual Cell* getCell() const {return _cell;};
    public: virtual Box getBoundingBox() const;
    public: const Name& getName() const {return _name;};
    public: const NetMainName* getMainName() const { return &_mainName; }
    public: const Arity& getArity() const {return _arity;};
    public: const Type& getType() const {return _type;};
    public: const Direction& getDirection() const {return _direction;};
    public: const Point& getPosition() const {return _position;};
    public: const DbU::Unit& getX() const {return _position.getX();};
    public: const DbU::Unit& getY() const {return _position.getY();};
    public: Components getComponents() const {return _componentSet.getElements();};
    public: Rubbers getRubbers() const {return _rubberSet.getElements();};
    public: RoutingPads getRoutingPads() const;
    public: Plugs getPlugs() const;
    public: Pins getPins() const;
    public: Contacts getContacts() const;
    public: Segments getSegments() const;
    public: Verticals getVerticals() const;
    public: Horizontals getHorizontals() const;
    public: Pads getPads() const;
    public: Plugs getSlavePlugs() const;
    public: Plugs getConnectedSlavePlugs() const;
    public: Plugs getUnconnectedSlavePlugs() const;
    public: Aliases getAliases() const { return new AliasList(this); };

// Filters
// *******

    public: static NetFilter getIsCellNetFilter();
    public: static NetFilter getIsDeepNetFilter();
    public: static NetFilter getIsGlobalFilter();
    public: static NetFilter getIsExternalFilter();
    public: static NetFilter getIsInternalFilter();
    public: static NetFilter getIsClockFilter();
    public: static NetFilter getIsSupplyFilter();
    public: static NetFilter getIsPowerFilter();
    public: static NetFilter getIsGroundFilter();

// Predicates
// **********

    public: virtual bool          isDeepNet  () const {return false;};
    public:         bool          isGlobal   () const {return _isGlobal;};
    public:         bool          isExternal () const {return _isExternal;};
    public:         bool          isAutomatic() const {return _isAutomatic;};
    public:         bool          isBlockage () const {return (_type == Type::BLOCKAGE);};
    public:         bool          isFused    () const {return (_type == Type::FUSED);};
    public:         bool          isLogical  () const {return (_type == Type::LOGICAL);};
    public:         bool          isClock    () const {return (_type == Type::CLOCK);};
    public:         bool          isPower    () const {return (_type == Type::POWER);};
    public:         bool          isGround   () const {return (_type == Type::GROUND);};
    public:         bool          isSupply   () const {return (isPower() || isGround());};
    public:         bool          hasAlias   (const Name& ) const;
    public:         NetAliasHook* getAlias   (const Name& ) const;

// Updators
// ********

    public: void setName(Name name);
    public: void setArity(const Arity& arity);
    public: void setGlobal(bool isGlobal);
    public: void setExternal(bool isExternal);
    public: void setAutomatic(bool isAutomatic);
    public: void setType(const Type& type);
    public: void setDirection(const Direction& direction);
    public: void setPosition(const Point& position);
    public: void setRoutingState(uint32_t state);
    public: void materialize();
    public: void unmaterialize();
    public: bool addAlias(const Name& name, bool isExternal=false);
    public: bool removeAlias(const Name& name);
    public: void merge(Net* net);
    public: Net* getClone(Cell* cloneCell);

// Others
// ******

    protected: virtual void _postCreate();
    protected: virtual void _preDestroy();

    public: virtual void _toJson(JsonWriter*) const;
    public: virtual void _toJsonSignature(JsonWriter*) const;
    public: virtual void _toJsonCollections(JsonWriter*) const;
    public: virtual string _getTypeName() const {return _TName("Net");};
    public: string _getFlagsAsString() const;
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: NetMainName& _getMainName() { return _mainName; }
    public: ComponentSet& _getComponentSet() {return _componentSet;};
    public: RubberSet& _getRubberSet() {return _rubberSet;};
    public: Net* _getNextOfCellNetMap() const {return _nextOfCellNetMap;};

    public: void _setNextOfCellNetMap(Net* net) {_nextOfCellNetMap = net;};

    public: struct CompareByName {
        inline bool operator() ( const Net* lhs, const Net* rhs ) const { return lhs->getName() < rhs->getName(); }
    };

};


// -------------------------------------------------------------------
// Class  :  "HookKey".

  class HookKey {
    public:
      inline                HookKey ( unsigned int id, const std::string& tname );
      inline  unsigned int  id      () const;
      inline  std::string   tname   () const;
    private:
      unsigned int  _id;
      std::string   _tname;
  };


  inline                HookKey::HookKey ( unsigned int id, const std::string& tname ) : _id(id), _tname(tname) { }
  inline  unsigned int  HookKey::id      () const { return _id; }
  inline  std::string   HookKey::tname   () const { return _tname; }

  inline bool operator< ( const HookKey& lhs, const HookKey& rhs )
  {
    if (lhs.id() != rhs.id()) return lhs.id() < rhs.id();
    return lhs.tname() < rhs.tname();
  }


// -------------------------------------------------------------------
// Class  :  "HookElement".

  class HookElement {
    public:
      enum Flags { OpenRingStart = (1<<0)
                 , ClosedRing    = (1<<1)
                 };
    public:
      inline                HookElement ( Hook*, unsigned long flags=0 );
      inline Hook*          hook        () const;
      inline HookElement*   next        () const;
      inline void           setHook     ( Hook* );
      inline void           setNext     ( HookElement* );
      inline unsigned long  flags       () const;
      inline HookElement&   setFlags    ( unsigned long mask );
      inline HookElement&   resetFlags  ( unsigned long mask );
      inline bool           issetFlags  ( unsigned long mask ) const;
    private:
      Hook*          _hook;
      HookElement*   _next;
      unsigned long  _flags;
  };


  inline                HookElement::HookElement ( Hook* hook, unsigned long flags ) : _hook(hook), _next(NULL), _flags(flags) { }
  inline Hook*          HookElement::hook        () const { return _hook; }
  inline HookElement*   HookElement::next        () const { return _next; }
  inline void           HookElement::setHook     ( Hook* hook ) { _hook = hook; }
  inline void           HookElement::setNext     ( HookElement* element ) { _next = element; }
  inline unsigned long  HookElement::flags       () const { return _flags; }
  inline HookElement&   HookElement::setFlags    ( unsigned long mask ) { _flags |=  mask; return *this; }
  inline HookElement&   HookElement::resetFlags  ( unsigned long mask ) { _flags &= ~mask; return *this; }
  inline bool           HookElement::issetFlags  ( unsigned long mask ) const { return _flags & mask; }


  typedef  map<HookKey,HookElement>  HookLut;


// -------------------------------------------------------------------
// Class  :  "JsonNet".

  class JsonNet : public JsonEntity {
    public:
      static  bool     hookFromString ( std::string s, unsigned int& id, std::string& tname );
      static  void     initialize     ();
                       JsonNet        ( unsigned long flags );
      virtual         ~JsonNet        ();
      virtual string   getTypeName    () const;
      virtual JsonNet* clone          ( unsigned long ) const;
      virtual void     toData         ( JsonStack& ); 
              void     addHookLink    ( Hook*, unsigned int jsonId, const std::string& jsonNext );
              Hook*    getHook        ( unsigned int jsonId, const std::string& tname ) const;
              bool     checkRings     () const;
              void     buildRings     () const;
      inline  void     clearHookLinks ();
  protected:
      bool       _autoMaterialize;
      Net*       _net;
      HookLut    _hooks;
  };


  inline void  JsonNet::clearHookLinks () { _hooks.clear(); }


} // Hurricane namespace.


// -------------------------------------------------------------------
// Inspector Support for  :  Net::Type::Code*".

template<>
inline std::string  getString<const Hurricane::Net::Type::Code*>
                             ( const Hurricane::Net::Type::Code* object )
                             {
                               switch ( *object ) {
                                 case Hurricane::Net::Type::UNDEFINED: return "UNDEFINED";
                                 case Hurricane::Net::Type::LOGICAL:   return "LOGICAL";
                                 case Hurricane::Net::Type::CLOCK:     return "CLOCK";
                                 case Hurricane::Net::Type::POWER:     return "POWER";
                                 case Hurricane::Net::Type::GROUND:    return "GROUND";
                                 case Hurricane::Net::Type::BLOCKAGE:  return "BLOCKAGE";
                                 case Hurricane::Net::Type::FUSED:     return "FUSED";
                               }
                               return "ABNORMAL";
                             }

template<>
inline Hurricane::Record* getRecord<const Hurricane::Net::Type::Code*>
                                   ( const  Hurricane::Net::Type::Code* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Code", (unsigned int*)object));
                                     return record;
                                   }


// -------------------------------------------------------------------
// Inspector Support for  :  "const Net::Direction::Code*".

template<>
inline std::string  getString<const Hurricane::Net::Direction::Code*>
                             ( const Hurricane::Net::Direction::Code* object )
                             {
                               std::ostringstream s;
                               s << (((*object) & Hurricane::Net::Direction::DirIn       ) ? 'i' : '-');
                               s << (((*object) & Hurricane::Net::Direction::DirOut      ) ? 'o' : '-');
                               s << (((*object) & Hurricane::Net::Direction::ConnTristate) ? 't' : '-');
                               s << (((*object) & Hurricane::Net::Direction::ConnWiredOr ) ? 'w' : '-');
                               
                               switch ( (int)*object ) {
                                 case Hurricane::Net::Direction::UNDEFINED: s << " (UNDEFINED)"; break;
                                 case Hurricane::Net::Direction::IN:        s << " (IN)";        break;
                                 case Hurricane::Net::Direction::OUT:       s << " (OUT)";       break;
                                 case Hurricane::Net::Direction::INOUT:     s << " (INOUT)";     break;
                                 case Hurricane::Net::Direction::TRISTATE:  s << " (TRISTATE)";  break;
                                 case Hurricane::Net::Direction::TRANSCV:   s << " (TRANSCV)";   break;
                                 case Hurricane::Net::Direction::WOR_OUT:   s << " (WOR_OUT)";   break;
                                 case Hurricane::Net::Direction::WOR_INOUT: s << " (WOR_INOUT)"; break;
                               }
                               return s.str();
                             }

template<>
inline Hurricane::Record* getRecord<const Hurricane::Net::Direction::Code*>
                                   ( const Hurricane::Net::Direction::Code* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Code", (unsigned int*)object));
                                     return record;
                                   }


INSPECTOR_P_SUPPORT(Hurricane::Net);
INSPECTOR_P_SUPPORT(Hurricane::Net::ComponentSet);
INSPECTOR_P_SUPPORT(Hurricane::Net::RubberSet);
INSPECTOR_PR_SUPPORT(Hurricane::Net::Type);
INSPECTOR_PR_SUPPORT(Hurricane::Net::Direction);
IOSTREAM_POINTER_SUPPORT(Hurricane::Net::Type::Code);
IOSTREAM_VALUE_SUPPORT(Hurricane::Net::Type::Code);
IOSTREAM_POINTER_SUPPORT(Hurricane::Net::Direction::Code);
IOSTREAM_VALUE_SUPPORT(Hurricane::Net::Direction::Code);


namespace Hurricane {

// Force SlotTemplate<> expansion on Net* type.
// Because sometimes it didn't happens (?).
  const SlotTemplate<Net*> dummyNetSlot ( string("dummyNetSlot"), NULL );


  typedef  std::set<Net*,DBo::CompareById>  NetSet;

}


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

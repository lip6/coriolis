// ****************************************************************************************************
// File: Net.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_NET
#define HURRICANE_NET

#include "Entity.h"
#include "Nets.h"
#include "Component.h"
#include "Rubbers.h"
#include "Rubber.h"
#include "RoutingPads.h"
#include "Plugs.h"
#include "Pins.h"
#include "Contacts.h"
#include "Segments.h"
#include "Verticals.h"
#include "Horizontals.h"
#include "Pads.h"
#include "IntrusiveSet.h"
#include "Path.h"

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

        public: enum Code {UNDEFINED=0, LOGICAL=1, CLOCK=2, POWER=3, GROUND=4};

        private: Code _code;

        public: Type(const Code& code = UNDEFINED);
        public: Type(const Type& type);

        public: Type& operator=(const Type& type);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Net::type"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    public: class Direction {
    // ********************

        public: enum Code {UNDEFINED=0, IN=1, OUT=2, INOUT=3, TRISTATE=4};

        private: Code _code;

        public: Direction(const Code& code = UNDEFINED);
        public: Direction(const Direction& direction);

        public: Direction& operator=(const Direction& direction);

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
    private: Type _type;
    private: Direction _direction;
    private: Point _position;
    private: ComponentSet _componentSet;
    private: RubberSet _rubberSet;
    private: Net* _nextOfCellNetMap;

// Constructors
// ************

    protected: Net(Cell* cell, const Name& name);

    public: static Net* create(Cell* cell, const Name& name);

// Accessors
// *********

    public: virtual Cell* getCell() const {return _cell;};
    public: virtual Box getBoundingBox() const;
    public: const Name& getName() const {return _name;};
    public: const Arity& getArity() const {return _arity;};
    public: const Type& getType() const {return _type;};
    public: const Direction& getDirection() const {return _direction;};
    public: const Point& getPosition() const {return _position;};
    public: const Unit& getX() const {return _position.getX();};
    public: const Unit& getY() const {return _position.getY();};
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

    public: virtual bool isDeepNet () const {return false;};
    public:         bool isGlobal  () const {return _isGlobal;};
    public:         bool isExternal() const {return _isExternal;};
    public:         bool isLogical () const {return (_type == Type::LOGICAL);};
    public:         bool isClock   () const {return (_type == Type::CLOCK);};
    public:         bool isPower   () const {return (_type == Type::POWER);};
    public:         bool isGround  () const {return (_type == Type::GROUND);};
    public:         bool isSupply  () const {return (isPower() || isGround());};

// Updators
// ********

    public: void setName(const Name& name);
    public: void setArity(const Arity& arity);
    public: void setGlobal(bool isGlobal);
    public: void setExternal(bool isExternal);
    public: void setType(const Type& type);
    public: void setDirection(const Direction& direction);
    public: void setPosition(const Point& position);
    public: void materialize();
    public: void unmaterialize();
    public: void merge(Net* net);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Net");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: ComponentSet& _getComponentSet() {return _componentSet;};
    public: RubberSet& _getRubberSet() {return _rubberSet;};
    public: Net* _getNextOfCellNetMap() const {return _nextOfCellNetMap;};

    public: void _setNextOfCellNetMap(Net* net) {_nextOfCellNetMap = net;};

};




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Net::Type::Code*>".

template<>
  inline string  ProxyTypeName<Net::Type::Code>
                              ( const Net::Type::Code* object )
                              { return "<PointerSlotAdapter<Net::Type::Code>>"; }

template<>
  inline string  ProxyString  <Net::Type::Code>
                              ( const Net::Type::Code* object )
                              {
                                switch ( *object ) {
                                  case Net::Type::UNDEFINED: return "UNDEFINED";
                                  case Net::Type::LOGICAL:   return "LOGICAL";
                                  case Net::Type::CLOCK:     return "CLOCK";
                                  case Net::Type::POWER:     return "POWER";
                                  case Net::Type::GROUND:    return "GROUND";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Net::Type::Code>
                              ( const Net::Type::Code* object )
                              {
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
                                return record;
                              }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Net::Direction::Code*>".

template<>
  inline string  ProxyTypeName<Net::Direction::Code>
                              ( const Net::Direction::Code* object )
                              { return "<PointerSlotAdapter<Net::Direction::Code>>"; }

template<>
  inline string  ProxyString  <Net::Direction::Code>
                              ( const Net::Direction::Code* object )
                              {
                                switch ( *object ) {
                                  case Net::Direction::UNDEFINED: return "UNDEFINED";
                                  case Net::Direction::IN:        return "IN";
                                  case Net::Direction::OUT:       return "OUT";
                                  case Net::Direction::INOUT:     return "INOUT";
                                  case Net::Direction::TRISTATE:  return "TRISTATE";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Net::Direction::Code>
                              ( const Net::Direction::Code* object )
                              {
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
                                return record;
                              }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Net)


#endif // HURRICANE_NET

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

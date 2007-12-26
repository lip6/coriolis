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

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Net::type"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

    public: class Direction {
	// ********************

		public: enum Code {UNDEFINED=0, IN=1, OUT=2, INOUT=3, TRISTATE=4};

		private: Code _code;

		public: Direction(const Code& code = UNDEFINED);
		public: Direction(const Direction& direction);

		public: Direction& operator=(const Direction& direction);

		public: operator const Code&() const {return _code;};

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Net::Direction"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

	class ComponentSet : public IntrusiveSet<Component> {
	// ************************************************

		public: typedef IntrusiveSet<Component> Inherit;
	
		public: ComponentSet();

		public: virtual unsigned _GetHashValue(Component* component) const;
		public: virtual Component* _GetNextElement(Component* component) const;
		public: virtual void _SetNextElement(Component* component, Component* nextComponent) const;

	};

	class RubberSet : public IntrusiveSet<Rubber> {
	// ******************************************

		public: typedef IntrusiveSet<Rubber> Inherit;
	
		public: RubberSet();

		public: virtual unsigned _GetHashValue(Rubber* rubber) const;
		public: virtual Rubber* _GetNextElement(Rubber* rubber) const;
		public: virtual void _SetNextElement(Rubber* rubber, Rubber* nextRubber) const;

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

	public: static Net* Create(Cell* cell, const Name& name);

// Accessors
// *********

	public: virtual Cell* GetCell() const {return _cell;};
	public: virtual Box GetBoundingBox() const;
	public: const Name& GetName() const {return _name;};
	public: const Arity& GetArity() const {return _arity;};
	public: const Type& GetType() const {return _type;};
	public: const Direction& GetDirection() const {return _direction;};
	public: const Point& GetPosition() const {return _position;};
	public: const Unit& GetX() const {return _position.GetX();};
	public: const Unit& GetY() const {return _position.GetY();};
	public: Components GetComponents() const {return _componentSet.GetElements();};
	public: Rubbers GetRubbers() const {return _rubberSet.GetElements();};
	public: RoutingPads GetRoutingPads() const;
	public: Plugs GetPlugs() const;
	public: Pins GetPins() const;
	public: Contacts GetContacts() const;
	public: Segments GetSegments() const;
	public: Verticals GetVerticals() const;
	public: Horizontals GetHorizontals() const;
	public: Pads GetPads() const;
	public: Plugs GetSlavePlugs() const;
	public: Plugs GetConnectedSlavePlugs() const;
	public: Plugs GetUnconnectedSlavePlugs() const;

// Filters
// *******

	public: static NetFilter GetIsCellNetFilter();
	public: static NetFilter GetIsDeepNetFilter();
	public: static NetFilter GetIsGlobalFilter();
	public: static NetFilter GetIsExternalFilter();
	public: static NetFilter GetIsInternalFilter();
	public: static NetFilter GetIsClockFilter();
	public: static NetFilter GetIsSupplyFilter();
	public: static NetFilter GetIsPowerFilter();
	public: static NetFilter GetIsGroundFilter();

// Predicates
// **********

	public: virtual bool IsDeepNet () const {return false;};
	public:         bool IsGlobal  () const {return _isGlobal;};
	public:         bool IsExternal() const {return _isExternal;};
	public:         bool IsLogical () const {return (_type == Type::LOGICAL);};
	public:         bool IsClock   () const {return (_type == Type::CLOCK);};
	public:         bool IsPower   () const {return (_type == Type::POWER);};
	public:         bool IsGround  () const {return (_type == Type::GROUND);};
	public:         bool IsSupply  () const {return (IsPower() || IsGround());};

// Updators
// ********

	public: void SetName(const Name& name);
	public: void SetArity(const Arity& arity);
	public: void SetGlobal(bool isGlobal);
	public: void SetExternal(bool isExternal);
	public: void SetType(const Type& type);
	public: void SetDirection(const Direction& direction);
	public: void SetPosition(const Point& position);
	public: void Materialize();
	public: void Unmaterialize();
	public: void Merge(Net* net);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Net");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: ComponentSet& _GetComponentSet() {return _componentSet;};
	public: RubberSet& _GetRubberSet() {return _rubberSet;};
	public: Net* _GetNextOfCellNetMap() const {return _nextOfCellNetMap;};

	public: void _SetNextOfCellNetMap(Net* net) {_nextOfCellNetMap = net;};

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
                                Record* record = new Record(GetString(object));
                                record->Add(GetSlot("Code", (unsigned int*)object));
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
                                Record* record = new Record(GetString(object));
                                record->Add(GetSlot("Code", (unsigned int*)object));
                                return record;
                              }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Net)


#endif // HURRICANE_NET

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

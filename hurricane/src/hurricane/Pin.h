// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: Pin.h
// Author: C. Alexandre freely inspired from Remy's Contact.h
// ****************************************************************************************************

#ifndef HURRICANE_PIN
#define HURRICANE_PIN

#include "Contact.h"
#include "Pins.h"

namespace Hurricane {



// ****************************************************************************************************
// Pin declaration
// ****************************************************************************************************

class Pin : public Contact {
// ***********************

// Types
// *****

	public: typedef Contact Inherit;

	public: class AccessDirection {
	// **************************

		public: enum Code {UNDEFINED=0, NORTH=1, SOUTH=2, EAST=3, WEST=4};

		private: Code _code;

		public: AccessDirection(const Code& code = UNDEFINED);
		public: AccessDirection(const AccessDirection& accessDirection);

		public: AccessDirection& operator=(const AccessDirection& accessDirection);

		public: operator const Code&() const {return _code;};

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Pin::AccessDirection"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

	public: class PlacementStatus {
	// **************************

		public: enum Code {UNPLACED=0, PLACED=1, FIXED=2};

		private: Code _code;

		public: PlacementStatus(const Code& code = UNPLACED);
		public: PlacementStatus(const PlacementStatus& placementstatus);

		public: PlacementStatus& operator=(const PlacementStatus& placementstatus);

		public: operator const Code&() const {return _code;};

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Pin::PlacementStatus"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

// Attributes
// **********

	private: Name _name;
	private: AccessDirection _accessDirection;
	private: PlacementStatus _placementStatus;
	private: Pin* _nextOfCellPinMap;

// Constructors
// ************

	protected: Pin(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);

	public: static Pin* Create(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);

// Accessors
// *********

	public: const Name& GetName() const {return _name;};
	public: const AccessDirection& GetAccessDirection() const {return _accessDirection;};
	public: const PlacementStatus& GetPlacementStatus() const {return _placementStatus;};

// Predicates
// **********

    public: bool IsUnplaced() const {return _placementStatus == PlacementStatus::UNPLACED;};
    public: bool IsPlaced()   const {return _placementStatus == PlacementStatus::PLACED;};
	public: bool IsFixed()    const {return _placementStatus == PlacementStatus::FIXED;};

// Updators
// ********

	public: void SetPlacementStatus(const PlacementStatus& placementstatus);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Pin");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: Pin* _GetNextOfCellPinMap() const {return _nextOfCellPinMap;};

	public: void _SetNextOfCellPinMap(Pin* pin) {_nextOfCellPinMap = pin;};

	//public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Pin)

#endif // HURRICANE_PIN

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

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Pin::AccessDirection"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    public: class PlacementStatus {
    // **************************

        public: enum Code {UNPLACED=0, PLACED=1, FIXED=2};

        private: Code _code;

        public: PlacementStatus(const Code& code = UNPLACED);
        public: PlacementStatus(const PlacementStatus& placementstatus);

        public: PlacementStatus& operator=(const PlacementStatus& placementstatus);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Pin::PlacementStatus"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

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

    public: static Pin* create(Net* net, const Name& name, const AccessDirection& accessDirection, const PlacementStatus& placementStatus, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);

// Accessors
// *********

    public: const Name& getName() const {return _name;};
    public: const AccessDirection& getAccessDirection() const {return _accessDirection;};
    public: const PlacementStatus& getPlacementStatus() const {return _placementStatus;};

// Predicates
// **********

    public: bool isUnplaced() const {return _placementStatus == PlacementStatus::UNPLACED;};
    public: bool isPlaced()   const {return _placementStatus == PlacementStatus::PLACED;};
    public: bool isFixed()    const {return _placementStatus == PlacementStatus::FIXED;};

// Updators
// ********

    public: void setPlacementStatus(const PlacementStatus& placementstatus);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Pin");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: Pin* _getNextOfCellPinMap() const {return _nextOfCellPinMap;};

    public: void _setNextOfCellPinMap(Pin* pin) {_nextOfCellPinMap = pin;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Pin)

#endif // HURRICANE_PIN

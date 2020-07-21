// ****************************************************************************************************
// File: ./hurricane/Pin.h
// Authors: C. Alexandre
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

#ifndef HURRICANE_PIN
#define HURRICANE_PIN

#include "hurricane/Contact.h"
#include "hurricane/Pins.h"

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

    protected: Pin( Net* net
                  , const Name& name
                  , const AccessDirection& accessDirection
                  , const PlacementStatus& placementStatus
                  , const Layer* layer
                  , const DbU::Unit& x
                  , const DbU::Unit& y
                  , const DbU::Unit& width  = 0
                  , const DbU::Unit& height = 0
                  );
    public: static Pin* create( Net* net
                              , const Name& name
                              , const AccessDirection& accessDirection
                              , const PlacementStatus& placementStatus
                              , const Layer* layer
                              , const DbU::Unit& x
                              , const DbU::Unit& y
                              , const DbU::Unit& width  = 0
                              , const DbU::Unit& height = 0
                              );

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


INSPECTOR_P_SUPPORT(Hurricane::Pin);
INSPECTOR_PR_SUPPORT(Hurricane::Pin::AccessDirection);
INSPECTOR_PR_SUPPORT(Hurricane::Pin::PlacementStatus);


#endif // HURRICANE_PIN

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

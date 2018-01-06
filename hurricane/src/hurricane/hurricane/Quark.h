// ****************************************************************************************************
// File: ./hurricane/Quark.h
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

#ifndef HURRICANE_QUARK
#define HURRICANE_QUARK

#include "hurricane/DBo.h"
#include "hurricane/Quarks.h"
#include "hurricane/Occurrence.h"

namespace Hurricane {



// ****************************************************************************************************
// Quark declaration
// ****************************************************************************************************

class Quark : public DBo {
// *********************

// Types
// *****

    public: typedef DBo Inherit;

// Attributes
// **********

    private: Occurrence _occurrence;
    private: Quark* _nextOfSharedPathQuarkMap;

// Constructors
// ************

    protected: Quark(const Occurrence& occurrence);

// Accessors
// *********

    public: const Occurrence& getOccurrence() const {return _occurrence;};

// Others
// ******

    public: static Quark* _create(const Occurrence& occurrence);
    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Quark");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Quark* _getNextOfSharedPathQuarkMap() const {return _nextOfSharedPathQuarkMap;};

    public: void _setNextOfSharedPathQuarkMap(Quark* quark) {_nextOfSharedPathQuarkMap = quark;};

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Quark);


#endif // HURRICANE_QUARK


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

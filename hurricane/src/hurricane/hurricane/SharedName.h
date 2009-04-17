// ****************************************************************************************************
// File: ./hurricane/SharedName.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#ifndef HURRICANE_SHARED_NAME
#define HURRICANE_SHARED_NAME

#include "hurricane/Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// SharedName declaration
// ****************************************************************************************************

class SharedName {
// *************

// Friends
// *******

    friend class Name;

// Types
// *****

    private: struct SharedNameMapComparator {
    // ************************************

        bool operator()(string* s1, string* s2) const;

    };

    private: typedef map<string*, SharedName*, SharedNameMapComparator> SharedNameMap;

// Attributes
// **********

    private: static SharedNameMap* _SHARED_NAME_MAP;

    private: int _count;
    private: string _string;

// Constructors
// ************

    private: SharedName(const string& s);

    private: SharedName(const SharedName& sharedName);
                // not implemented to forbid copy construction

// Destructor
// **********

    private: ~SharedName();

// Operators
// *********

    private: SharedName& operator=(const SharedName& sharedName);
                // not implemented to forbid assignment

// Updators
// ********

    private: void capture();
    private: void release();

// Others
// ******

    public: const string &_getSString () const { return _string; };
    public: string _getTypeName() const { return _TName("SharedName"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::SharedName);


#endif // HURRICANE_SHARED_NAME


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************

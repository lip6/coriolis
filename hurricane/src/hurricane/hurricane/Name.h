// ****************************************************************************************************
// File: ./hurricane/Name.h
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

#ifndef HURRICANE_NAME
#define HURRICANE_NAME

#include "hurricane/Commons.h"
#include "hurricane/Names.h"

namespace Hurricane {

class SharedName;



// ****************************************************************************************************
// Name declaration
// ****************************************************************************************************

class Name {
// *******

    private: static const Name _emptyName;
    public: static const Name& emptyName () { return _emptyName; };

// Attributes
// **********

    private: SharedName* _sharedName;

// Constructors
// ************

    public: Name();

    public: Name(const char* c);
    public: Name(const string& s);

    public: Name(const Name& name);

// Destructor
// **********

    public: ~Name();

// Operators
// *********

    public: Name& operator=(const Name& name);

    public: bool operator==(const Name& name) const;
    public: bool operator!=(const Name& name) const;
    public: bool operator<(const Name& name) const;
    public: bool operator<=(const Name& name) const;
    public: bool operator>(const Name& name) const;
    public: bool operator>=(const Name& name) const;

    public: char operator[](unsigned index) const;

// Predicates
// **********

    public: bool isEmpty() const;
    public: size_t size() const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Name"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public: SharedName* _getSharedName() const {return _sharedName;};

};


} // End of Hurricane namespace.


INSPECTOR_PV_SUPPORT(Hurricane::Name);


#endif // HURRICANE_NAME


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************

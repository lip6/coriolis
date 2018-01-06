// ****************************************************************************************************
// File: ./hurricane/Interruption.h
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

#ifndef HURRICANE_INTERRUPTION
#define HURRICANE_INTERRUPTION

#include "hurricane/Exception.h"

namespace Hurricane {



// ****************************************************************************************************
// Interruption declaration
// ****************************************************************************************************

class Interruption : public Exception {
// **********************************

// Types
// *****

    public: typedef Exception Inherit;

// Attributes
// **********

    private: string _reason;
    private: int _code;

// Constructors
// ************

    public: Interruption(const string& reason, int code = 0);

    public: Interruption(const Interruption& interruption);

// Operators
// *********

    public: Interruption& operator=(const Interruption& interruption);

// Accessors
// *********

    public: string getReason() const {return _reason;};
    public: int getCode() const {return _code;};

// Others
// ******

    public: virtual string _getTypeName() const { return _TName("Interruption"); };
    public: virtual string _getString() const;

};


} // End of Hurricane namespace.


#endif // HURRICANE_INTERRUPTION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

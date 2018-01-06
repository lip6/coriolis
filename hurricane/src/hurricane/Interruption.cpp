// ****************************************************************************************************
// File: ./Interruption.cpp
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

#include "hurricane/Interruption.h"

namespace Hurricane {



// ****************************************************************************************************
// Interruption implementation
// ****************************************************************************************************

Interruption::Interruption(const string& reason, int code)
// *******************************************************
:    Inherit(),
    _reason(reason),
    _code(code)
{
}

Interruption::Interruption(const Interruption& interruption)
// *********************************************************
:    Inherit(),
    _reason(interruption._reason),
    _code(interruption._code)
{
}

Interruption& Interruption::operator=(const Interruption& interruption)
// ********************************************************************
{
    _reason = interruption._reason;
    _code = interruption._code;
    return *this;
}

string Interruption::_getString() const
// ************************************
{
    if (!_code) return "[INTERRUPTION] " + _reason;

    return "[INTERRUPTION:" + getString(_code) + "] " + _reason;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

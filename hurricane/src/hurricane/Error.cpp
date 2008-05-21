// ****************************************************************************************************
// File: Error.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Error.h"


namespace Hurricane {



// ****************************************************************************************************
// Error implementation
// ****************************************************************************************************

Error::Error(const string& reason, int code)
// *****************************************
:    Inherit(),
    _reason(reason),
    _code(code)
{
}

Error::Error(const Error& error)
// *****************************
:    Inherit(),
    _reason(error._reason),
    _code(error._code)
{
}

Error& Error::operator=(const Error& error)
// ****************************************
{
    _reason = error._reason;
    _code = error._code;
    return *this;
}

string Error::_getString() const
// *****************************
{
    if (!_code) return "[ERROR] " + _reason;

    return "[ERROR:" + getString(_code) + "] " + _reason;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

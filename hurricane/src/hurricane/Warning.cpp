// ****************************************************************************************************
// File: Warning.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Warning.h"

namespace Hurricane {



// ****************************************************************************************************
// Warning implementation
// ****************************************************************************************************

Warning::Warning(const string& reason, int code)
// *********************************************
:    Inherit(),
    _reason(reason),
    _code(code)
{
}

Warning::Warning(const Warning& warning)
// *************************************
:    Inherit(),
    _reason(warning._reason),
    _code(warning._code)
{
}

Warning& Warning::operator=(const Warning& warning)
// ************************************************
{
    _reason = warning._reason;
    _code = warning._code;
    return *this;
}

string Warning::_getString() const
// *******************************
{
    if (!_code) return "[WARNING] " + _reason;

    return "[WARNING:" + getString(_code) + "] " + _reason;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

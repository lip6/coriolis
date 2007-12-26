// ****************************************************************************************************
// File: Interruption.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Interruption.h"

namespace Hurricane {



// ****************************************************************************************************
// Interruption implementation
// ****************************************************************************************************

Interruption::Interruption(const string& reason, int code)
// *******************************************************
:	Inherit(),
	_reason(reason),
	_code(code)
{
}

Interruption::Interruption(const Interruption& interruption)
// *********************************************************
:	Inherit(),
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

string Interruption::_GetString() const
// ************************************
{
	if (!_code) return "[INTERRUPTION] " + _reason;

	return "[INTERRUPTION:" + GetString(_code) + "] " + _reason;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

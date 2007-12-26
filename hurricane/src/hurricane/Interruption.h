// ****************************************************************************************************
// File: Interruption.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INTERRUPTION
#define HURRICANE_INTERRUPTION

#include "Exception.h"

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

	public: string GetReason() const {return _reason;};
	public: int GetCode() const {return _code;};

// Others
// ******

    public: virtual string _GetTypeName() const { return _TName("Interruption"); };
	public: virtual string _GetString() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Interruption)


#endif // HURRICANE_INTERRUPTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

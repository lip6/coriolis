// ****************************************************************************************************
// File: Error.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_ERROR
#define HURRICANE_ERROR

#include "Exception.h"

namespace Hurricane {



// ****************************************************************************************************
// Error declaration
// ****************************************************************************************************

class Error : public Exception {
// ***************************

// Types
// *****

    public: typedef Exception Inherit;

// Attributes
// **********

    private: string _reason;
    private: int _code;

// Constructors
// ************

    public: Error(const string& reason, int code = 0);

    public: Error(const Error& error);

// Operators
// *********

    public: Error& operator=(const Error& error);

// Accessors
// *********

    public: string getReason() const {return _reason;};
    public: int getCode() const {return _code;};

// Modifiers
// *********

    public: void  setReason ( const string& reason ) { _reason = reason; };

// Others
// ******

    public: virtual string _getTypeName() const { return _TName("Error"); };
    public: virtual string _getString() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Error)

#endif // HURRICANE_ERROR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

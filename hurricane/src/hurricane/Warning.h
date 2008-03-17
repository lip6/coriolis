// ****************************************************************************************************
// File: Warning.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_WARNING
#define HURRICANE_WARNING

#include "Exception.h"

namespace Hurricane {



// ****************************************************************************************************
// Warning declaration
// ****************************************************************************************************

class Warning : public Exception {
// *****************************

// Types
// *****

    public: typedef Exception Inherit;

// Attributes
// **********

    private: string _reason;
    private: int _code;

// Constructors
// ************

    public: Warning(const string& reason, int code = 0);

    public: Warning(const Warning& warning);

// Operators
// *********

    public: Warning& operator=(const Warning& warning);

// Accessors
// *********

    public: string getReason() const {return _reason;};
    public: int getCode() const {return _code;};

// Others
// ******

    public: virtual string _getTypeName() const { return _TName("Warning"); };
    public: virtual string _getString() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Warning)

#endif // HURRICANE_WARNING

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

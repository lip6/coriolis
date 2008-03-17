// ****************************************************************************************************
// File: Exception.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_EXCEPTION
#define HURRICANE_EXCEPTION

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Exception declaration
// ****************************************************************************************************

class Exception : public NestedSlotAdapter {
// ***************************************

// Constructors
// ************

    protected: Exception();

    private: Exception(const Exception& exception); // not implemented to forbid copy construction

// Destructor
// **********

    public: virtual ~Exception();

// Operators
// *********

    private: Exception& operator=(const Exception& exception); // not implemented to forbid assignment

// Accessors
// *********

   public: string  What() const { return _getString(); };

// Others
// ******

    public: virtual string _getString() const = 0;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Exception)


#endif // HURRICANE_EXCEPTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

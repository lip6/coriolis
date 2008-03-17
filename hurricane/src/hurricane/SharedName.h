// ****************************************************************************************************
// File: SharedName.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SHARED_NAME
#define HURRICANE_SHARED_NAME

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// SharedName declaration
// ****************************************************************************************************

class SharedName {
// *************

// Friends
// *******

    friend class Name;

// Types
// *****

    private: struct SharedNameMapComparator {
    // ************************************

        bool operator()(string* s1, string* s2) const;

    };

    private: typedef map<string*, SharedName*, SharedNameMapComparator> SharedNameMap;

// Attributes
// **********

    private: static SharedNameMap* _SHARED_NAME_MAP;

    private: int _count;
    private: string _string;

// Constructors
// ************

    private: SharedName(const string& s);

    private: SharedName(const SharedName& sharedName);
                // not implemented to forbid copy construction

// Destructor
// **********

    private: ~SharedName();

// Operators
// *********

    private: SharedName& operator=(const SharedName& sharedName);
                // not implemented to forbid assignment

// Updators
// ********

    private: void Capture();
    private: void Release();

// Others
// ******

    public: const string &_getSString () const { return _string; };
    public: string _getTypeName() const { return _TName("SharedName"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};


} // End of Hurricane namespace.


#endif // HURRICANE_SHARED_NAME

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

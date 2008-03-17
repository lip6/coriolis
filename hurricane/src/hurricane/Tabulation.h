// ****************************************************************************************************
// File: Tabulation.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_TABULATION
#define HURRICANE_TABULATION

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Tabulation declaration
// ****************************************************************************************************

class Tabulation {
// *************

// Attributes
// **********

    private: string _s;
    private: unsigned _n;

// Constructors
// ************

    public: Tabulation(const string& s = "   ");

    public: Tabulation(const Tabulation& tabulation);

// Destructor
// **********

    public: ~Tabulation();

// Operators
// *********

    public: Tabulation& operator=(const Tabulation& tabulation);

    public: Tabulation& operator++();
    public: Tabulation operator++(int);
    public: Tabulation& operator--();
    public: Tabulation operator--(int);

// Others
// ******

    public: string _getTypeName() const { return _TName("Tabulation"); };
    public: string _getString() const;
    public: Record* _getRecord() const { return NULL; };
    public: Slot* _getSlot() const { return NULL; };

};



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

extern Tabulation tab;



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Tabulation)


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

#endif // HURRICANE_TABULATION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

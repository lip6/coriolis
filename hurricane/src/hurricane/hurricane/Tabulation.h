// ****************************************************************************************************
// File: ./hurricane/Tabulation.h
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

#ifndef HURRICANE_TABULATION_H
#define HURRICANE_TABULATION_H

#ifndef HURRICANE_COMMONS_H
#error "Tabulation.h must be included through Commons.h"
#endif

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

    public: unsigned getWidth() const { return _s.size()*_n; };
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


INSPECTOR_PR_SUPPORT(Hurricane::Tabulation);



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

#endif // HURRICANE_TABULATION_H


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

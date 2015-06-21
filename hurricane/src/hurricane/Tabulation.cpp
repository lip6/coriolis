// ****************************************************************************************************
// File: ./Tabulation.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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

#include "hurricane/Tabulation.h"

namespace Hurricane {



// ****************************************************************************************************
// Tabulation implementation
// ****************************************************************************************************

Tabulation::Tabulation(const string& s)
// ************************************
:    _s(s),
    _n(0)
{
}

Tabulation::Tabulation(const Tabulation& tabulation)
// *************************************************
:    _s(tabulation._s),
    _n(tabulation._n)
{
}

Tabulation::~Tabulation()
// **********************
{
}

Tabulation& Tabulation::operator=(const Tabulation& tabulation)
// ************************************************************
{
    _s = tabulation._s;
    _n = tabulation._n;
    return *this;
}

Tabulation& Tabulation::operator++()
// *********************************
{
    if (_n < 100) _n++;
    return *this;
}

Tabulation Tabulation::operator++(int)
// ***********************************
{
    Tabulation tabulation = *this;
    if (_n < 100) _n++;
    return tabulation;
}

Tabulation& Tabulation::operator--()
// *********************************
{
    if (0 < _n) _n--;
    return *this;
}

Tabulation Tabulation::operator--(int)
// ***********************************
{
    Tabulation tabulation = *this;
    if (0 < _n) _n--;
    return tabulation;
}

string Tabulation::_getString() const
// **********************************
{
    string s = "";
    unsigned n = _n;
    while (n--) s += _s;
    return s;
}



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

Tabulation tab("   ");



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

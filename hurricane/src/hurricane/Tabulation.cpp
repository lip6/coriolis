// ****************************************************************************************************
// File: Tabulation.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Tabulation.h"

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
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

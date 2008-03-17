// ****************************************************************************************************
// File: Unit.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Unit.h"
#include "Error.h"

#include <math.h>

namespace Hurricane {



// ****************************************************************************************************
// Constants & Variables
// ****************************************************************************************************

static const unsigned MAXIMAL_PRECISION_ALLOWED = 3;

static unsigned PRECISION = 0;
static double RESOLUTION = 1;

static Unit GRID_STEP = 1;



// ****************************************************************************************************
// Precision managers
// ****************************************************************************************************

unsigned getPrecision()
// ********************
{
    return PRECISION;
}

unsigned getMaximalPrecisionAllowed()
// **********************************
{
    return MAXIMAL_PRECISION_ALLOWED;
}

void SetPrecision(unsigned precision)
// **********************************
{
    if (MAXIMAL_PRECISION_ALLOWED < precision)
        throw Error("Can't set precision : not allowed");

    PRECISION = precision;

    RESOLUTION = 1;
    while (precision--) RESOLUTION /= 10;

    SetGridStep(getUnit(1));
}



// ****************************************************************************************************
// Resolution managers
// ****************************************************************************************************

double getResolution()
// *******************
{
    return RESOLUTION;
}



// ****************************************************************************************************
// Grid managers
// ****************************************************************************************************

const Unit& getGridStep()
// **********************
{
    return GRID_STEP;
}

void SetGridStep(const Unit& gridStep)
// ***********************************
{
    if (!gridStep) throw Error("Can't set grid step : invalid value");

    GRID_STEP = gridStep;
}

bool IsOnGrid(const Unit& unit, int n)
// ***********************************
{
    if (n <= 0) throw Error("Can't compute : invalid value");

    n *= GRID_STEP;

    return (((abs(unit) / n) * n) == abs(unit));
}

Unit getOnGridUnit(const Unit& unit, int s)
// ****************************************
{
    switch (s) {
        case -1 : {
            if (0 < unit) return (unit / GRID_STEP) * GRID_STEP;
            else if (unit < 0) return ((unit / GRID_STEP) - 1) * GRID_STEP;
            return unit;
        }
        case 0 : {
            int g1 = (unit / GRID_STEP) * GRID_STEP;
           int g2 = ((g1 < unit) ? (g1 + GRID_STEP) : (g1 - GRID_STEP));
            return (abs(g1 - unit) <= abs(g2 - unit)) ? g1 : g2;
        }
        case +1 : {
            if (0 < unit) return ((unit / GRID_STEP) + 1) * GRID_STEP;
            else if (unit < 0) return (unit / GRID_STEP) * GRID_STEP;
            return unit;
        }
    }
    throw Error("Can't get on grid unit : invalid parameter s (should be -1, 0 or +1)");
    return 0;
}



// ****************************************************************************************************
// Others
// ****************************************************************************************************

Unit getUnit(double value)
// ***********************
{
    return (int)rint(value / RESOLUTION);
}

double getValue(const Unit& unit)
// ******************************
{
    return (unit * RESOLUTION);
}

string getValueString(const Unit& unit)
// ************************************
{
    if (unit == 0) return "0";

    char buffer[32];
    unsigned length = sprintf(buffer, "%ld", abs(unit));
    if (length <= PRECISION) {
        unsigned offset = PRECISION - length + 1;
        for (int i = length; 0 <= i; i--) buffer[i + offset] = buffer[i];
        for (unsigned i = 0; i < offset; i++) buffer[i] = '0';
        length += offset;
    }
    for (unsigned i = 0; i <= PRECISION; i++)
        buffer[length - i + 1] = buffer[length - i];
    buffer[length - PRECISION] = '.';
    while (buffer[length] == '0') buffer[length--] = '\0';
    if (buffer[length] == '.') buffer[length--] = '\0';
    if (0 < unit)
        return buffer;
    else
        return "-" + string(buffer);
}



} // End of Hurricane namespace.



bool Scan(const string& s, H::Unit& unit)
// ***********************************
{
    double d;
    if (Scan(s, d)) {
        unit = H::getUnit(d);
        return true;
    }
    return false;
}

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

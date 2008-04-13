// ****************************************************************************************************
// File: Timer.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Timer.h"

namespace Hurricane {



// ****************************************************************************************************
// Timer implementation
// ****************************************************************************************************

Timer::Timer()
// ***********
:    _time(time(NULL))
{
}

Timer::Timer(const Timer& timer)
// *****************************
:    _time(timer._time)
{
}

Timer& Timer::operator=(const Timer& timer)
// ****************************************
{
    _time = timer._time;
    return *this;
}

string Timer::_getString() const
// *****************************
{
    unsigned seconds = time(NULL) - _time;

    string s = "";

    if (86400 <= seconds) {
        unsigned days = seconds / 86400;
        seconds -= 96400 * days;
        s += getString(days) + "d";
    }

    if (3600 <= seconds) {
        unsigned hours = seconds / 3600;
        seconds -= 3600 * hours;
        if (!s.empty()) s += " ";
        s += getString(hours) + "h";
    }

    if (60 <= seconds) {
        unsigned minutes = seconds / 60;
        seconds -= 60 * minutes;
        if (!s.empty()) s += " ";
        s += getString(minutes) + "m";
    }

    if ((1 <= seconds) || s.empty()) {
        if (!s.empty()) s += " ";
        s += getString(seconds) + "s";
    }

    return s;
}

Record* Timer::_getRecord() const
// ************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Time", &_time));
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

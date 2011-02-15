/*
 *  Wire.cpp
 *  openChams
 *
 *  Created by damien dupuis on 08/02/11.
 *  Copyright 2011 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
void Wire::setStartPoint(Name name, Name plug) {
    if (_start)
        throw OpenChamsException("[ERROR] Wire::setStartPoint: cannot set start point since it has already been set.");

    _start = new InstancePoint(name, plug);
}

void Wire::setStartPoint(unsigned idx) {
    if (_start)
        throw OpenChamsException("[ERROR] Wire::setStartPoint: cannot set start point since it has already been set.");

    _start = new PortPoint(idx);
}

void Wire::setEndPoint(Name name, Name plug) {
    if (!_start)
        throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since start point has not been set.");
    if (_end)
        throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since it has already been set.");

    _end = new InstancePoint(name, plug);
}

void Wire::setEndPoint(unsigned idx) {
    if (!_start)
        throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since start point has not been set.");
    if (_end)
        throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since it has already been set.");

    _end = new PortPoint(idx);
}

void Wire::addIntermediatePoint(double x, double y) {
    if (!_start)
        throw OpenChamsException("[ERROR] Wire::addIntermediatePoint: cannot add point since start point has not been set.");
    if (_end)
        throw OpenChamsException("[ERROR] Wire::addIntermediatePoint: cannot add point since end point has already been set.");

    _inters.push_back(new IntermediatePoint(x, y));
}
} // namespace


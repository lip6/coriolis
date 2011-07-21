/*
 *  Device.cpp
 *  openChams
 *
 *  Created by damien dupuis on 22/11/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Device::Device(Name name, Name model, unsigned order, Name mosType, bool sourceBulkConnected, Netlist* netlist)
    : Instance(name, model, order, netlist)
    , _mosType(mosType)
    , _sourceBulkConnected(sourceBulkConnected)
    , _trans() {}

Transistor* Device::addTransistor(Name name) {
    Transistor* tr = new Transistor(name, this);
    if (!tr)
        throw OpenChamsException("[ERROR] Cannot create transistor.");

    _trans.push_back(tr); 
    return tr;
}
}


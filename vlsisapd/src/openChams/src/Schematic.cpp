/*
 *  SchematicInfos.cpp
 *  openChams
 *
 *  Created by damien dupuis on 22/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Schematic::Schematic(Circuit* circuit, double zoom) 
    : _circuit(circuit)
    , _zoom(zoom) {}
    
void Schematic::addInstance(Name instanceName, double x, double y, Name sym) {
    map<Name, Schematic::Infos*>::iterator it = _instances.find(instanceName);
    if (it != _instances.end()) {
        string error("[ERROR] Cannot set same instance twice in 'schematic' (");
        error += instanceName.getString();
        error += ").";
        throw OpenChamsException(error);
    }
    _instances[instanceName] = new Schematic::Infos(x, y, sym);
}

Schematic::Infos::Infos(double x, double y, Name sym)
    : _x(x)
    , _y(y)
    , _sym(sym) {}
} // namespace

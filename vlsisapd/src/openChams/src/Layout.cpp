/*
 *  Layout.cpp
 *  openChams
 *
 *  Created by damien dupuis on 31/08/10.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <string>
using namespace std;

#include "vlsisapd/openChams/Layout.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Layout::Layout(Circuit* circuit): _circuit(circuit) {}
    
void Layout::addInstance(Name name, Name style) {
    map<Name, Name>::iterator it = _instances.find(name);
    if (it != _instances.end()) {
        string error("[ERROR] Cannot set several instances with the same name in 'layout' (");
        error += name.getString();
        error += ").";
        throw OpenChamsException(error);
    }
    _instances[name] = style;
}
    
} // namespace


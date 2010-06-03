/*
 *  Transistor.cpp
 *  openChams
 *
 *  Created by damien dupuis on 01/03/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
    Transistor::Transistor(Name name, Instance* instance)
    : _name(name)
    , _gate("")
    , _source("")
    , _drain("")
    , _bulk("")
    , _instance(instance) {}
    
    void Transistor::setGate(Name gate) {
        if (checkConnector(gate))
            _gate = gate;
    }
        
    void Transistor::setSource(Name source) {
        if (checkConnector(source))
        	_source = source;
    }

    void Transistor::setDrain(Name drain) {
        if (checkConnector(drain))
        	_drain = drain;
    }
            
    void Transistor::setBulk(Name bulk) {
        if (checkConnector(bulk))
            _bulk = bulk;
    }
    bool Transistor::checkConnector(Name name) {
        map<Name, Net*>::const_iterator it = _instance->getConnectors().find(name);
        if (it != _instance->getConnectors().end()) {// si le connecteur existe bien
            return true;
        } else {
            string error("[ERROR] Instance ");
            error += _instance->getName().getString();
            error += " has no connector named ";
            error += name.getString();
            error += ".";
            throw OpenChamsException(error);
        }
    }
}



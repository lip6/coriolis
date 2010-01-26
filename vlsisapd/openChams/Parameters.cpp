/*
 *  Parameters.cpp
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
using namespace std;

#include "Parameters.h"

namespace OpenChams {
double Parameters::getValue(Name name) {
    map<Name, double>::iterator it = _params.find(name);
    if (it == _params.end()) {
        cerr << "[ERROR] No parameters named " << name.getString() << endl;
        return 0.0;
    }
    return (*it).second;
}
    
void Parameters::addParameter(Name name, double value) {
	map<Name, double>::iterator it = _params.find(name);
    if ( it != _params.end() ) {
        cerr << "[ERROR] Cannot addParameter " << name.getString() << " because it already exists !" << endl;
        return;
    }
    _params[name] = value;
}
} // namespace


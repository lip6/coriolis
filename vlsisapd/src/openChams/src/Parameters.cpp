/*
 *  Parameters.cpp
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
double Parameters::getValue(Name name) {
    map<Name, double>::iterator it = _params.find(name);
    if (it == _params.end()) {
        string error("[ERROR] No parameters named ");
        error += name.getString();
        throw OpenChamsException(error);
        //return 0.0;
    }
    return (*it).second;
}
    
void Parameters::addParameter(Name name, double value) {
	map<Name, double>::iterator it = _params.find(name);
    map<Name, string>::iterator it2 = _paramsEq.find(name);
    if ( it != _params.end() || it2 != _paramsEq.end() ) {
        string error("[ERROR] Cannot addParameter ");
        error += name.getString();
        error += " because it already exists !";
        throw OpenChamsException(error);
    }
    _params[name] = value;
}
    
void Parameters::addParameter(Name name, string eqStr) {
    map<Name, double>::iterator it = _params.find(name);
    map<Name, string>::iterator it2 = _paramsEq.find(name);
    if ( it != _params.end() || it2 != _paramsEq.end() ) {
        string error("[ERROR] Cannot addParameter ");
        error += name.getString();
        error += " because it already exists !";
        throw OpenChamsException(error);
    }
    _paramsEq[name] = eqStr;
}
} // namespace


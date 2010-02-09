/*
 *  Sizing.cpp
 *  openChams
 *
 *  Created by damien dupuis on 08/02/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <string>
using namespace std;

#include "Sizing.h"
#include "Circuit.h"
#include "Operator.h"
#include "OpenChamsException.h"

namespace OpenChams {
Sizing::Sizing(Circuit* circuit): _circuit(circuit) {}
    
Operator* Sizing::addOperator(Name instanceName, Name operatorName, Name simulModel, unsigned callOrder) {
    map<Name, Operator*>::iterator it = _operators.find(instanceName);
    if (it != _operators.end()) {
        string error("[ERROR] Cannot set same instance twice in 'sizing' (");
        error += instanceName.getString();
        error += ").";
        throw OpenChamsException(error);
    }
    Operator* op = new Operator(operatorName, simulModel, callOrder);
    _operators[instanceName] = op;
    return op;
}
    
void Sizing::addEquation(Name equationName, string equation) {
    map<Name, string>::iterator it = _equations.find(equationName);
    if (it != _equations.end()) {
        string error("[ERROR] Cannot set several equations with the same name in 'sizing' (");
        error += equationName.getString();
        error += ").";
        throw OpenChamsException(error);
    }
    _equations[equationName] = equation;
}
    
} // namespace
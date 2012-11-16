
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Sizing.cpp"                                  |
// +-----------------------------------------------------------------+



#include "vlsisapd/openChams/Sizing.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/Equation.h"
#include "vlsisapd/openChams/OpenChamsException.h"
using namespace std;


namespace OpenChams {


  Sizing::Sizing ( Circuit* circuit )
    : _circuit(circuit)
  { }
    

  Operator* Sizing::addOperator ( const string& instanceName
                                , const string& operatorName
                                , const string& simulModel )
  {
    map<string, Operator*>::iterator it = _operators.find(instanceName);
    if (it != _operators.end()) {
      string error("[ERROR] Cannot set same instance twice in 'sizing' (");
      error += instanceName;
      error += ").";
      throw OpenChamsException(error);
    }
    Operator* op = new Operator(operatorName, simulModel);
    _operators[instanceName] = op;
    return op;
  }
    

  void Sizing::addEquation ( const string& equationName, Equation* equation)
  {
    map<string, Equation*>::iterator it = _equations.find(equationName);
    if (it != _equations.end()) {
      string error("[ERROR] Cannot set several equations with the same name in 'sizing' (");
      error += equationName;
      error += ").";
      throw OpenChamsException(error);
    }
    _equations[equationName] = equation;
  }
    
  
} // OpenChams namespace



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
// |  C++ Module  :  "./Operator.cpp"                                |
// +-----------------------------------------------------------------+


#include <string>
using namespace std;

#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/OpenChamsException.h"


namespace OpenChams {


  Operator::Operator(const string& operatorName, const string& simulModel) 
    : _name      (operatorName)
    , _simulModel(simulModel)
  { }

  
  void Operator::addConstraint(const string& paramName, const string& ref, const string& refParam)
  { addConstraint(paramName, ref, refParam, 1.0); }


  void Operator::addConstraint(const string& paramName, const string& ref, const string& refParam, double factor)
  {
    map<string, Constraint*>::iterator it = _constraints.find(paramName);
    if (it != _constraints.end()) {
      string error("[ERROR] Cannot set two constraints for the same parameter (");
      error += paramName;
      error += ").";
      throw OpenChamsException(error);
    }
    _constraints[paramName] = new Operator::Constraint(ref, refParam, factor);
  }


  void Operator::addConstraint(const string& paramName, const string& refEquation)
  { addConstraint(paramName, refEquation, 1.0); }


  void Operator::addConstraint(const string& paramName, const string& refEquation, double factor)
  {
    map<string, Constraint*>::iterator it = _constraints.find(paramName);
	if (it != _constraints.end()) {
      string error("[ERROR] Cannot set two constraints for the same parameter (");
      error += paramName;
      error += ").";
      throw OpenChamsException(error);
    }
    _constraints[paramName] = new Operator::Constraint("", refEquation, factor);
  }


  Operator::Constraint::Constraint(const string& ref, const string& refParam, double factor)
    : _ref     (ref)
    , _refParam(refParam)
    , _factor  (factor)
  { }


}  // OpenChams namespace.


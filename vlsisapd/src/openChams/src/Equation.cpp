/*
 *  Equation.cpp
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <string>
using namespace std;

#include "vlsisapd/openChams/Equation.h"

namespace OpenChams {
  Equation::Equation()
    : _equations()
      //  , _paramsInEquation()
  {}

  void Equation::addEquation(std::string eq) {
    _equations[_equations.size()] = eq;
  }

} // namespace

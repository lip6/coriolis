/*
 *  NRCCstr.cpp
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <string>
#include <iostream>

using namespace std;

#include "vlsisapd/openChams/NRCCstr.h"

namespace OpenChams {
  NRCCstr::NRCCstr(string controlVoltage)
    : Equation()
    , _controlVoltage(controlVoltage)
  {}

  // This function prints all equations
  void NRCCstr::printEquations() {
    map<int, string>::iterator it;
    cerr << "Printing equations of a NRCCstr : " << endl;
    cerr << "Control voltage = " << _controlVoltage << endl;
    for(it=_equations.begin(); it!=_equations.end(); ++it) {
      cerr << (*it).first << "\t" << (*it).second << endl;
    }
  }

} // namespace

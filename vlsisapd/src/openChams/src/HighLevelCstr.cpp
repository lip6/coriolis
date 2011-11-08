/*
 *  HighLevelCstr.cpp
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <string>
#include <iostream>

using namespace std;

#include "vlsisapd/openChams/HighLevelCstr.h"

namespace OpenChams {
  HighLevelCstr::HighLevelCstr()
    : Equation()
  {}

  // This function prints all equations
  void HighLevelCstr::printEquations() {
    map<int, string>::iterator it;
    cerr << "Printing equations of a HighLevelCstr : " << endl;
    for(it=_equations.begin(); it!=_equations.end(); ++it) {
      cerr << (*it).first << "\t" << (*it).second << endl;
    }
  }
  
} // namespace

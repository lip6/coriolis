/*
 *  Equation.h
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_EQUATION_H__
#define __OPENCHAMS_EQUATION_H__

#include <vector>
#include <map>
//using namespace std;

namespace OpenChams {
  class Equation {
  public:
    Equation();
  
    void addEquation(std::string eq);
    inline std::map<int, std::string>& getEquationStr();
    virtual void printEquations() = 0;
  
  protected:
    std::map<int, std::string> _equations;       // this map contains the equation(s)
    //  std::vector<std::string> _paramsInEquation;  // 
  };
    
  inline std::map<int, std::string>& Equation::getEquationStr() {return _equations;}

} // namespace
#endif

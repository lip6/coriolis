/*
 *  Sizing.h
 *  openChams
 *
 *  Created by damien dupuis on 08/02/10.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_SIZING_H__
#define __OPENCHAMS_SIZING_H__

#include <map>

namespace OpenChams {
  class Name;
  class Circuit;
  class Operator;
  class Equation;
  //  class HighLevelCstr;
  //  class NRCCstr;
  //  class DDP;

  class Sizing {
  public:
    Sizing(Circuit*);
    
    Operator* addOperator(Name instanceName, Name operatorName, Name simulModel);

    //    void      addEquation(Name equationName, HighLevelCstr*);
    //    void      addEquation(Name equationName, NRCCstr*);
    //    void      addEquation(Name equationName, DDP*);
    void      addEquation(Name equationName, Equation*);
    
    inline bool  hasNoOperators();
    inline bool  hasNoEquations();
    inline const std::map<Name, Operator*>& getOperators();
    inline const std::map<Name, Equation*>& getEquations();
    
  private:
    Circuit* _circuit;
    std::map<Name, Operator*>   _operators; // instanceName <-> operator
    std::map<Name, Equation*> _equations; // equationName <-> equation (string)
  };
    
  inline bool   Sizing::hasNoOperators() { return (_operators.size() == 0) ? true : false; };
  inline bool   Sizing::hasNoEquations() { return (_equations.size() == 0) ? true : false; };
  inline const  std::map<Name, Operator*>&   Sizing::getOperators() { return _operators; };
  inline const  std::map<Name, Equation*>& Sizing::getEquations() { return _equations; };
} // namespace
#endif


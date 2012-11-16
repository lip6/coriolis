
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
// |  C++ Header  :  "./vlsisapd/openChams/Sizing.h"                 |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_SIZING_H__
#define __OPENCHAMS_SIZING_H__

#include <map>
#include <string>

namespace OpenChams {

  class Circuit;
  class Operator;
  class Equation;
//class HighLevelCstr;
//class NRCCstr;
//class DDP;


  class Sizing {
    public:
                       Sizing         ( Circuit* );
             Operator* addOperator    ( const std::string& instanceName
                                      , const std::string& operatorName
                                      , const std::string& simulModel);
    //       void      addEquation    ( const std::string& equationName, HighLevelCstr* );
    //       void      addEquation    ( const std::string& equationName, NRCCstr* );
    //       void      addEquation    ( const std::string& equationName, DDP* );
             void      addEquation    ( const std::string& equationName, Equation* );
      inline bool      hasNoOperators ();
      inline bool      hasNoEquations ();
      inline const std::map<std::string, Operator*>& getOperators();
      inline const std::map<std::string, Equation*>& getEquations();
    private:
      Circuit* _circuit;
      std::map<std::string, Operator*>  _operators; // instanceName <-> operator
      std::map<std::string, Equation*>  _equations; // equationName <-> equation (string)
  };


  inline bool   Sizing::hasNoOperators() { return _operators.empty(); };
  inline bool   Sizing::hasNoEquations() { return _equations.empty(); };
  inline const  std::map<std::string, Operator*>& Sizing::getOperators() { return _operators; };
  inline const  std::map<std::string, Equation*>& Sizing::getEquations() { return _equations; };


} // OpenChams namespace

#endif


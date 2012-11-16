
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
// |  C++ Header  :  "./vlsisapd/openChams/Operator.h"               |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_OPERATOR_H__
#define __OPENCHAMS_OPERATOR_H__

#include <map>


namespace OpenChams {


  class Operator {

    public:
      class Constraint {
        public:
                                    Constraint  ( const std::string& ref
                                                , const std::string& refParam
                                                , double             factor );
          inline const std::string& getRef      ();
          inline const std::string& getRefParam ();
          inline double             getFactor   ();
        private:
          std::string  _ref;
          std::string  _refParam;
          double       _factor;
      };
        
	public:
                                Operator         (const std::string& operatorName, const std::string& simulModel);
             void               addConstraint    (const std::string& paramName, const std::string& ref, const std::string& refParam);
             void               addConstraint    (const std::string& paramName, const std::string& ref, const std::string& refParam, double factor);
             void               addConstraint    (const std::string& paramName, const std::string& refEquation);
             void               addConstraint    (const std::string& paramName, const std::string& refEquation, double factor);
      inline const std::string& getName          ();
      inline const std::string& getSimulModel    ();
      inline bool               hasNoConstraints ();
      inline const std::map<std::string, Constraint*>& getConstraints();
    private:
      std::string                         _name;
      std::string                         _simulModel;
      std::map<std::string, Constraint*>  _constraints;
  };
    

  inline const std::string& Operator::getName                 () { return _name; };
  inline const std::string& Operator::getSimulModel           () { return _simulModel; };
  inline bool               Operator::hasNoConstraints        () { return _constraints.empty(); };
  inline const std::map<std::string, Operator::Constraint*>& Operator::getConstraints() { return _constraints; };        

  inline const std::string& Operator::Constraint::getRef      () { return _ref; };
  inline const std::string& Operator::Constraint::getRefParam () { return _refParam; };
  inline double             Operator::Constraint::getFactor   () { return _factor; };
    

} // namespace

#endif


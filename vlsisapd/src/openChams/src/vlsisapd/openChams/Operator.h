/*
 *  Operator.h
 *  openChams
 *
 *  Created by damien dupuis on 08/02/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_OPERATOR_H__
#define __OPENCHAMS_OPERATOR_H__

#include <map>

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class Operator {
    public:
        class Constraint {
        public:
            Constraint(Name ref, Name refParam, double factor);
        
            inline Name   getRef();
            inline Name   getRefParam();
            inline double getFactor();
            
        private:
            Name   _ref;
            Name   _refParam;
            double _factor;
        };
        
	public:
        Operator(Name operatorName, Name simulModel);
        
        void addConstraint(Name paramName, Name ref, Name refParam               );
        void addConstraint(Name paramName, Name ref, Name refParam, double factor);
        void addConstraint(Name paramName, Name refEquation               );
        void addConstraint(Name paramName, Name refEquation, double factor);
        
        inline Name     getName();
        inline Name     getSimulModel();
        inline bool     hasNoConstraints();
        inline const std::map<Name, Constraint*>& getConstraints();
        
    private:
	    Name _name;
	    Name _simulModel;
        std::map<Name, Constraint*> _constraints;
};
    
    inline Name     Operator::getName() { return _name; };
    inline Name     Operator::getSimulModel() { return _simulModel; };
    inline bool     Operator::hasNoConstraints() { return (_constraints.size() == 0) ? true : false; };
    inline const std::map<Name, Operator::Constraint*>& Operator::getConstraints() { return _constraints; };        

    inline Name   Operator::Constraint::getRef()      { return _ref; };
    inline Name   Operator::Constraint::getRefParam() { return _refParam; };
    inline double Operator::Constraint::getFactor()   { return _factor; };
    
} // namespace
#endif


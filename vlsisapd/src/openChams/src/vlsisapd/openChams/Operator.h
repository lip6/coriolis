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

#include "Name.h"

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
        Operator(Name operatorName, Name simulModel, unsigned callOrder);
        
        void addConstraint(Name paramName, Name ref, Name refParam, double factor = 1.0);
        void addConstraint(Name paramName, Name refEquation, double factor = 1.0);
        
        inline Name     getName();
        inline Name     getSimulModel();
        inline unsigned getCallOrder();
        inline bool     hasNoConstraints();
        inline const std::map<Name, Constraint*>& getConstraints();
        
    private:
	    Name _name;
	    Name _simulModel;
	    unsigned _callOrder;
        std::map<Name, Constraint*> _constraints;
};
    
    inline Name     Operator::getName() { return _name; };
    inline Name     Operator::getSimulModel() { return _simulModel; };
    inline unsigned Operator::getCallOrder() { return _callOrder; };
    inline bool     Operator::hasNoConstraints() { return (_constraints.size() == 0) ? true : false; };
    inline const std::map<Name, Operator::Constraint*>& Operator::getConstraints() { return _constraints; };        

    inline Name   Operator::Constraint::getRef()      { return _ref; };
    inline Name   Operator::Constraint::getRefParam() { return _refParam; };
    inline double Operator::Constraint::getFactor()   { return _factor; };
    
} // namespace
#endif


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

#include "Name.h"

namespace OpenChams {
    class Circuit;
    class Operator;
    class Sizing {
	public:
        Sizing(Circuit*);
        
        Operator* addOperator(Name instanceName, Name operatorName, Name simulModel, unsigned callOrder);
        void      addEquation(Name equationName, std::string equation);
        
        inline bool  hasNoOperators();
        inline bool  hasNoEquations();
        inline const std::map<Name, Operator*>&   getOperators();
        inline const std::map<Name, std::string>& getEquations();
        
    private:
        Circuit* _circuit;
        std::map<Name, Operator*>   _operators; // instanceName <-> operator
        std::map<Name, std::string> _equations; // equationName <-> equation (string)
    };
    
    inline bool   Sizing::hasNoOperators() { return (_operators.size() == 0) ? true : false; };
    inline bool   Sizing::hasNoEquations() { return (_equations.size() == 0) ? true : false; };
    inline const  std::map<Name, Operator*>&   Sizing::getOperators() { return _operators; };
    inline const  std::map<Name, std::string>& Sizing::getEquations() { return _equations; };
} // namespace
#endif


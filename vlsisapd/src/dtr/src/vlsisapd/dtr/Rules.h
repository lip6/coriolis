/*
 *  Rules.h
 *  DTR
 *
 *  Created by damien dupuis on 01/04/10.
 *  Copyright 2010 UPMC / LIP6 All rights reserved.
 *
 */

#ifndef __DTR_RULES_H__
#define __DTR_RULES_H__

#include "vlsisapd/dtr/Name.h"

namespace DTR {
class Rule {
    public:
    Rule(Name name, double value, Name valueStr, Name ref, Name layer1, Name layer2)
        : _name(name), _type(Name("")), _value(value), _valueStr(valueStr), _ref(ref), _layer1(layer1), _layer2(layer2) {};
    
    // accessors
    inline Name   getName();
    inline Name   getType();
    inline double getValue();
    inline std::string getValueAsString();
    inline Name   getRef();
    inline Name   getLayer1();
    virtual inline Name   getLayer2(); // add virtual so the Rule object is polymorphic

    // modifiers
    inline void setType(Name);

    // members
    private:
    Name   _name;
    Name   _type;
    double _value;
    Name   _valueStr;
    Name   _ref;
    Name   _layer1;
    Name   _layer2;
};

class ARule : public Rule {
    public:
    ARule(Name name, double value, Name valueStr, Name ref, Name layer1, Name layer2)
        : Rule(name, value, valueStr, ref, layer1, layer2) {};
};
    
inline Name   Rule::getName()   { return _name;   };
inline Name   Rule::getType()   { return _type;   };
inline double Rule::getValue()  { return _value;  };
inline Name   Rule::getRef()    { return _ref;    };
inline Name   Rule::getLayer1() { return _layer1; };
inline Name   Rule::getLayer2() { return _layer2; };
inline std::string Rule::getValueAsString() { return _valueStr.getString(); };

inline void   Rule::setType(Name type) { _type = type; };
} // namespace DTR
#endif


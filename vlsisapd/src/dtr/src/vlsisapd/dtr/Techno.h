/*
 *  Techno.h
 *  DTR
 *
 *  Created by damien dupuis on 01/04/10.
 *  Copyright 2010 UPMC / LIP6 All rights reserved.
 *
 */

#ifndef __DTR_TECHNO_H__
#define __DTR_TECHNO_H__

#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/dtr/Name.h"

namespace DTR {
class Rule;
class ARule;
class Techno {
    public:
    Techno(Name name, Name unit);
    
    inline Name getName();
    inline Name getUnit();
    inline std::vector<Rule*>&  getRules();

    Rule*  addRule (Name name, double value, Name ref, Name layer1=Name(""), Name layer2=Name(""));
    ARule* addARule(Name name, double value, Name ref, Name layer1         , Name layer2);

    Rule* getRule(Name name, Name layer1=Name(""), Name layer2=Name(""));
    double getValue(Name name, Name layer1=Name(""), Name layer2=Name(""));
    std::string getValueAsString(Name name, Name layer1=Name(""), Name layer2=Name(""));
    
    bool writeToFile(std::string filePath);
    static Techno* readFromFile(const std::string filePath);
    
    Name 	            _name;
    Name 	            _unit;
    std::vector<Rule*>  _rules;
};
    
inline Name Techno::getName() { return _name; };
inline Name Techno::getUnit() { return _unit; };
inline std::vector<Rule*>&  Techno::getRules()  { return _rules;  };
    
} // namespace DTR
#endif


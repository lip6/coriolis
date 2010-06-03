/*
 *  Parameters.h
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_PARAMETERS_H__
#define __OPENCHAMS_PARAMETERS_H__

#include <map>

namespace OpenChams {
class Name;

class Parameters {
public:
    Parameters() {};
    
    double      getValue(Name);
    std::string getEqValue(Name);
    void        addParameter(Name, double);
    void        addParameter(Name, std::string);
    
    // pour parcourir la map :
    inline bool isEmpty();
    inline const std::map<Name, double>&      getValues();
    inline const std::map<Name, std::string>& getEqValues();
    
private:
    std::map<Name, double>      _params;
    std::map<Name, std::string> _paramsEq;
};
    
inline bool Parameters::isEmpty() { return ((_params.size() == 0)&&(_paramsEq.size() == 0))? true : false; }
inline const std::map<Name, double>& Parameters::getValues()   { return _params; };
inline const std::map<Name, std::string>& Parameters::getEqValues() { return _paramsEq; };

} // namespace
#endif


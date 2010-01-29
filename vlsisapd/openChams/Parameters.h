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

#include "Name.h"

namespace OpenChams {
class Parameters {
public:
    Parameters() {};
    
    double getValue(Name);    
    void   addParameter(Name, double);
    
    // pour parcourir la map :
    inline bool isEmpty();
    //inline map<Name, double>::iterator getFirstIt();
    //inline map<Name, double>::iterator getLastIt();
    inline const std::map<Name, double>& getValues();
    
private:
    std::map<Name, double> _params;
};
    
inline bool Parameters::isEmpty() { return (_params.size() == 0)? true : false; }
//inline map<Name, double>::iterator Parameters::getFirstIt() { return _params.begin(); }
//inline map<Name, double>::iterator Parameters::getLastIt()  { return _params.end(); }
inline const std::map<Name, double>& Parameters::getValues() { return _params; };

} // namespace
#endif


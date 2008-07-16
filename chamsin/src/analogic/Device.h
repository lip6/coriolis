#ifndef DEVICE_H
#define DEVICE_H

#include "hurricane/Cell.h"
using namespace Hurricane;

//#include "DeviceParameter.h"

class Device : public Cell {
#if 0
    public:
        struct ParameterCompare:
            public std::binary_function<const Parameter*, const Parameter*, bool> {
                bool operator()(const Parameter<>* dp1, const Parameter<>* dp2) const {   
                    return dp1->_id < dp2->_id;
                }
            };

        typedef set<Parameter*>, ParameterCompare> ParameterSet;
        Parameter* getParameter(const string& parameterId) const;
#endif
    protected:
        Device(Library* library, const Name& name): Cell(library, name) {}
#if 0
        void addParameter(const Parameter* parameter) {
            _parameterSet.insert(parameter);
        }
    private:
        ParameterSet _parameterSet;
#endif
        
};

#endif // DEVICE_H

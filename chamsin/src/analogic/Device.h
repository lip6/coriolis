#ifndef DEVICE_H
#define DEVICE_H

#include "DeviceParameter.h"
#include "hurricane/Cell.h"

class Device : public Cell {
    public:
        struct DeviceParameterCompare:
            public std::binary_function<const DeviceParameter&, const DeviceParameter&, bool> {
                bool operator()(const DeviceParameter& dp1, const DeviceParameter& dp2) const {   
                    return dp1._id < dp2._id;
                }
            };
        typedef set<DeviceParameter, DeviceParameterCompare> DeviceParameterSet;
        DeviceParameter& getParameter(const string& parameterId) const;
    protected:
        Device(Library* library, const Name& name): Cell(library, name) {}
        void addParameter(const DeviceParameter& deviceParameter) {
            _parameterSet.insert(deviceParameter);
        }
    private:
        DeviceParameterSet _parameterSet;
        
};

#endif // DEVICE_H

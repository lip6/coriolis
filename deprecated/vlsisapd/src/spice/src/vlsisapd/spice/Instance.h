#ifndef __SPICE_INSTANCE_H
#define __SPICE_INSTANCE_H


#include <string>
#include <vector>
#include <map>

namespace SPICE {
class Value;

class Instance {
    protected:
    std::string                        _name;
    std::string                        _model;
    std::vector<std::string>           _connectors;
    std::map<std::string, std::string> _parameters;

    public:
    Instance(std::string name, std::string model);
    virtual ~Instance() {};

    inline std::string getName();
    inline std::string getModel();
    inline const std::vector<std::string>& getConnectors();
    inline const std::map<std::string, std::string>& getParameters();

    std::string getParameterValue(std::string name);

    inline void addConnector(std::string connector);

    void addParameter(std::string name, std::string value);

};

inline std::string Instance::getName()  { return _name;  }
inline std::string Instance::getModel() { return _model; }
inline const std::vector<std::string>& Instance::getConnectors() { return _connectors; }
inline const std::map<std::string, std::string>& Instance::getParameters() { return _parameters; }

inline void Instance::addConnector(std::string connector) { _connectors.push_back(connector); } // no verification : same net can be used on several interfaces
}
#endif

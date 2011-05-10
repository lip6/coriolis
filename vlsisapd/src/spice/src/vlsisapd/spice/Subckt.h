#ifndef __SPICE_SUBCKT_H
#define __SPICE_SUBCKT_H


#include <string>
#include <vector>
#include <map>

namespace SPICE {
class Instance;

class Subckt {
    private:
    std::string                        _name;
    std::vector<std::string>           _interfaces;
    std::vector<Instance*>             _instances;
    std::map<std::string, std::string> _parameters;

    public:
    Subckt(std::string name): _name(name), _interfaces(), _instances(), _parameters() {}
    ~Subckt() {}

    inline std::string                         getName();
    inline std::vector<std::string>&           getInterfaces();
    inline std::vector<Instance*>&             getInstances();
    inline std::map<std::string, std::string>& getParameters();

    inline void addInterface(std::string);
    inline void addInstance (Instance*);
    
    void addParameter(std::string, std::string);
};
inline std::string                         Subckt::getName()       { return _name;       }
inline std::vector<std::string>&           Subckt::getInterfaces() { return _interfaces; }
inline std::vector<Instance*>&             Subckt::getInstances()  { return _instances;  }
inline std::map<std::string, std::string>& Subckt::getParameters() { return _parameters; }

inline void Subckt::addInterface(std::string name) { _interfaces.push_back(name); }
inline void Subckt::addInstance(Instance* inst)    { _instances.push_back(inst);  }
}
#endif

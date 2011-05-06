#ifndef __SPICE_SUBCKT_H
#define __SPICE_SUBCKT_H


#include <string>
#include <vector>

namespace SPICE {
class Instance;

class Subckt {
    private:
    std::string              _name;
    std::vector<std::string> _interfaces;
    std::vector<Instance*>   _instances;

    public:
    Subckt(std::string name): _name(name), _interfaces(), _instances() {}
    ~Subckt() {}

    inline std::string               getName();
    inline std::vector<std::string>& getInterfaces();
    inline std::vector<Instance*>&   getInstances();

    inline void addInterface(std::string);
    inline void addInstance (Instance*);
};
inline std::string               Subckt::getName()       { return _name;       }
inline std::vector<std::string>& Subckt::getInterfaces() { return _interfaces; }
inline std::vector<Instance*>&   Subckt::getInstances()  { return _instances;  }

inline void Subckt::addInterface(std::string name) { _interfaces.push_back(name); }
inline void Subckt::addInstance(Instance* inst)    { _instances.push_back(inst);  }
}
#endif

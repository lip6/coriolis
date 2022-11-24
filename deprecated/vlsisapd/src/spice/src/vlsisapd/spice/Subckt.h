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
    std::vector<std::string>           _comments;

    public:
    Subckt(std::string name): _name(name), _interfaces(), _instances(), _parameters(), _comments() {}
    ~Subckt() {}

    inline const std::string                         getName();
    inline const std::vector<std::string>&           getInterfaces();
    inline const std::vector<Instance*>&             getInstances();
    inline const std::map<std::string, std::string>& getParameters();
    inline const std::vector<std::string>&           getComments();

    inline void addInterface(std::string);
    inline void addInstance (Instance*);
    inline void addComment(std::string);
    
    void addParameter(std::string, std::string);
};
inline const std::string                         Subckt::getName()       { return _name;       }
inline const std::vector<std::string>&           Subckt::getInterfaces() { return _interfaces; }
inline const std::vector<Instance*>&             Subckt::getInstances()  { return _instances;  }
inline const std::map<std::string, std::string>& Subckt::getParameters() { return _parameters; }
inline const std::vector<std::string>&           Subckt::getComments()   { return _comments;   }

inline void Subckt::addInterface(std::string name) { _interfaces.push_back(name); }
inline void Subckt::addInstance (Instance* inst)   { _instances.push_back(inst);  }
inline void Subckt::addComment  (std::string com)  { _comments.push_back(com);    }
}
#endif

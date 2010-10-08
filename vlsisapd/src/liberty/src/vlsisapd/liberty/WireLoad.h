#ifndef __LIB_WIRE_LOAD_H__
#define __LIB_WIRE_LOAD_H__

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"

namespace LIB {

class WireLoad {
    public:
    WireLoad(Name name);
    
    inline Name getName();

    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue, const std::string& attrValue2 = "");

    void print();
    bool write(std::ofstream &file);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
};
    
inline Name WireLoad::getName() { return _name; };
    
} // namespace LIB
#endif

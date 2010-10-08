#ifndef __LIB_FLIP_FLOP_H__
#define __LIB_FLIP_FLOP_H__

#include<fstream>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"

namespace LIB {

class FlipFlop {
    public:
    FlipFlop(Name noninverting, Name inverting);
    
    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue);

    void print();
    bool write(std::ofstream &file);

    private:
    Name _noninverting;
    Name _inverting;
    std::map<Name, Attribute*> _attributes;
};
    
} // namespace LIB
#endif

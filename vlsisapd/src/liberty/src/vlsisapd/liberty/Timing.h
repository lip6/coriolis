#ifndef __LIB_TIMING_H__
#define __LIB_TIMING_H__

#include<fstream>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"

namespace LIB {

class Timing {
    public:
    Timing();
    
    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue);

    void print();
    bool write(std::ofstream &file);

    private:
    std::map<Name, Attribute*> _attributes;
};
    
} // namespace LIB
#endif

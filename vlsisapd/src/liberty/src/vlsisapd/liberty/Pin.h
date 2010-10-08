#ifndef __LIB_PIN_H__
#define __LIB_PIN_H__

#include<fstream>
#include<vector>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Timing.h"

namespace LIB {

class Pin {
    public:
    Pin(Name name);
    
    inline Name getName();
    inline std::vector<Timing*> getTiming();

    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue);
    void addTiming();

    void print();
    bool write(std::ofstream &file);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::vector<Timing*> _timings;
};
    
inline Name Pin::getName() { return _name; };
inline std::vector<Timing*> Pin::getTiming() { return _timings; };
    
} // namespace LIB
#endif

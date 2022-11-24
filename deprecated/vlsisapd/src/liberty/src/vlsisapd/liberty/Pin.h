#ifndef __LIB_PIN_H__
#define __LIB_PIN_H__

#include<fstream>
#include<vector>

#include "vlsisapd/liberty/Attribute.h"

namespace LIB {
class Timing;

class Pin {
    public:
    Pin(Name name);
    
    inline Name getName();
    inline std::map<Name, Attribute*>& getAttributes();
    inline std::vector<Timing*>& getTimings();
    Attribute* getAttribute(Name attrName);
    Timing* getTiming(Name pinName);

    void addAttribute(Name attrName, Attribute::Type attrType, const std::string& attrValue);
    void addTiming();

    const std::string getString()const;
    void print();
    bool write(std::ofstream &file);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::vector<Timing*> _timings;
};
    
inline Name                         Pin::getName()       { return _name; };
inline std::map<Name, Attribute*>&  Pin::getAttributes() { return _attributes; };
inline std::vector<Timing*>&        Pin::getTimings()    { return _timings; };
    
} // namespace LIB
#endif

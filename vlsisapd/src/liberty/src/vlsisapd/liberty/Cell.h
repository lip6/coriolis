#ifndef __LIB_CELL_H__
#define __LIB_CELL_H__

#include<fstream>

#include "vlsisapd/liberty/Attribute.h"

namespace LIB {
class Pin;
class FlipFlop;

class Cell {
    public:
    Cell(Name name);
    
    inline Name getName();
    inline std::map<Name, Attribute*>& getAttributes();
    inline std::map<Name, Pin*>& getPins();
    inline FlipFlop* getFF();
    inline Cell* getTestCell();
    Attribute* getAttribute(Name attrName);
    Pin* getPin(Name pinName);

    void addAttribute(Name attrName, Attribute::Type attrType, const std::string& attrValue);
    void addPin(Name pinName);
    void addFF(Name noninverting, Name inverting);
    void setTestCell(Cell *cell);

    const std::string getString()const;
    void print();
    bool write(std::ofstream &file, bool test=false);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::map<Name, Pin*> _pins;
    FlipFlop* _ff;
    Cell* _testCell;
};
    
inline Name Cell::getName() { return _name; };
inline std::map<Name, Pin*>& Cell::getPins() { return _pins; };
inline std::map<Name, Attribute*>& Cell::getAttributes() { return _attributes; };
inline FlipFlop* Cell::getFF() { return _ff; };
inline Cell* Cell::getTestCell() { return _testCell; };
    
} // namespace LIB
#endif

#ifndef __LIB_CELL_H__
#define __LIB_CELL_H__

#include<fstream>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/FlipFlop.h"

namespace LIB {

class Cell {
    public:
    Cell(Name name);
    
    inline Name getName();
    Pin* getPin(Name pinName);
    inline FlipFlop* getFF();

    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue);
    void addPin(Name pinName);
    void addFF(Name noninverting, Name inverting);
    void setTestCell(Cell *cell);

    void print();
    bool write(std::ofstream &file, bool test=false);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::map<Name, Pin*> _pins;
    FlipFlop* _ff;
    Cell* _test_cell;
};
    
inline Name Cell::getName() { return _name; };
inline FlipFlop* Cell::getFF() { return _ff; };
    
} // namespace LIB
#endif

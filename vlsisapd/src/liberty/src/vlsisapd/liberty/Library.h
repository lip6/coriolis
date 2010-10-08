#ifndef __LIB_LIBRARY_H__
#define __LIB_LIBRARY_H__

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/WireLoad.h"
#include "vlsisapd/liberty/WireLoadSelection.h"
#include "vlsisapd/liberty/Cell.h"

namespace LIB {

class Library {
    public:
    Library(Name name);
    
    inline Name getName();
    Cell* getCell(Name cellName);
    WireLoad* getWireLoad(Name wireLoadName);
    inline WireLoadSelection* getWireLoadSelection();

    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue, const std::string& attrUnit="");
    void addWireLoad(Name wireLoadName);
    void addWireLoadSelection(Name wireLoadSelectionName);
    void addCell(Name cellName);

    void print();

    static Library* readFromFile(std::string);
    bool writeToFile ( std::string fileName );

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::map<Name, WireLoad*>  _wires_load;
    WireLoadSelection*         _wire_load_selection;
    std::map<Name, Cell*>      _cells;
};
    
inline Name Library::getName() { return _name; };
inline WireLoadSelection* Library::getWireLoadSelection() { return _wire_load_selection; };
    
} // namespace LIB
#endif

#ifndef __LIB_LIBRARY_H__
#define __LIB_LIBRARY_H__

#include "vlsisapd/liberty/Attribute.h"

namespace LIB {
class WireLoad;
class WireLoadSelection;
class Cell;

class Library {
    public:
    Library(Name name);
    
    inline Name getName();
    inline std::map<Name, Attribute*>& getAttributes();
    inline std::map<Name, WireLoad*>& getWiresLoad();
    inline WireLoadSelection* getWireLoadSelection();
    inline std::map<Name, Cell*>& getCells();
    Attribute* getAttribute(Name attrName);
    WireLoad* getWireLoad(Name wireLoadName);
    Cell* getCell(Name cellName);

    void addAttribute(Name attrName, Attribute::Type attrType, const std::string& attrValue, const std::string& attrUnit="");
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
    
inline Name                         Library::getName()              { return _name; };
inline std::map<Name, Attribute*>&  Library::getAttributes()        { return _attributes; };
inline std::map<Name, WireLoad*>&   Library::getWiresLoad()         { return _wires_load; };
inline WireLoadSelection*           Library::getWireLoadSelection() { return _wire_load_selection; };
inline std::map<Name, Cell*>&       Library::getCells()             { return _cells; };
    
} // namespace LIB
#endif

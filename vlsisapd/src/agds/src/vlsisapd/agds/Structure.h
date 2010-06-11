#ifndef __GDS_STRUCTURE_H
#define __GDS_STRUCTURE_H

#include <fstream>
#include <string>
#include <vector>

namespace AGDS {
class Element;

class Structure {
    public:
        Structure(std::string strName);

        bool addElement ( Element* );
        bool write ( std::ofstream &file );
        
        inline std::string getName();

    private:
        std::string _strName;

        std::vector<Element*> _elements;
};

inline std::string Structure::getName() { return _strName; };
}
#endif

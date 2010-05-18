#ifndef __GDS_STRUCTURE_H
#define __GDS_STRUCTURE_H

#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "GdsElement.h"

namespace vlsisapd {
class GdsStructure {
    public:
        GdsStructure(string strName);

        bool addElement ( GdsElement* );
        bool write ( ofstream &file );
        
        inline string getName();

    private:
        string _strName;

        vector<GdsElement*> _elements;
};

inline string GdsStructure::getName() { return _strName; };
}
#endif

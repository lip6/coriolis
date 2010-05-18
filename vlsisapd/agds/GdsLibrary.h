#ifndef __GDS_LIBRARY_H
#define __GDS_LIBRARY_H

#include <vector>
using namespace std;

#include "GdsStructure.h"

namespace vlsisapd {
class GdsLibrary {
    public:
        GdsLibrary(string libName);

        inline void setUserUnits ( double userUnits );
        inline void setPhysUnits ( double physUnits );

        bool addStructure ( GdsStructure* );
        bool write ( string fileName );

    private:
        string _libName;
        double _userUnits;
        double _physUnits;

        vector<GdsStructure*> _structs;
};

inline void GdsLibrary::setUserUnits(double userUnits) { _userUnits = userUnits; };
inline void GdsLibrary::setPhysUnits(double physUnits) { _physUnits = physUnits; };
} // namespace
#endif

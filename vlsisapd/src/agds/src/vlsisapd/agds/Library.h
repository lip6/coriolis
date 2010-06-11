#ifndef __GDS_LIBRARY_H
#define __GDS_LIBRARY_H

#include <vector>

namespace AGDS {
class Structure;

class Library {
    public:
        Library(std::string libName);

        inline void setUserUnits ( double userUnits );
        inline void setPhysUnits ( double physUnits );

        bool addStructure ( Structure* );
        bool writeToFile  ( std::string fileName );

    private:
        std::string _libName;
        double _userUnits;
        double _physUnits;

        std::vector<Structure*> _structs;
};

inline void Library::setUserUnits(double userUnits) { _userUnits = userUnits; };
inline void Library::setPhysUnits(double physUnits) { _physUnits = physUnits; };
} // namespace
#endif

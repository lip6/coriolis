
// author : Damien Dupuis
// date   : 08.12.2009
// -*- C++ -*-

#ifndef __GDS_DRIVER_H__
#define __GDS_DRIVER_H__

#include <string>

namespace Hurricane {
    class Cell;
}

namespace CRL {
class GdsDriver {
    public:
        GdsDriver(Hurricane::Cell*);

        bool save(const std::string& filePath);
        inline std::string getName();
        inline std::string getLib();
        inline double      getUUnits();
        inline double      getPUnits();

    private:
        Hurricane::Cell* _cell;
        std::string      _name;
        std::string      _lib;
        double           _uUnits;
        double           _pUnits;
};

inline std::string GdsDriver::getName()   { return _name;   };
inline std::string GdsDriver::getLib()    { return _lib;    };
inline double      GdsDriver::getUUnits() { return _uUnits; };
inline double      GdsDriver::getPUnits() { return _pUnits; };
} // End of CRL namespace.


# endif


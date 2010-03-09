
// author : Damien Dupuis
// date   : 24.02.2010
// -*- C++ -*-

#ifndef __CIF_DRIVER_H__
#define __CIF_DRIVER_H__

#include <string>

namespace Hurricane {
    class Cell;
}

namespace CRL {

class CifDriver {
    public:
        CifDriver(Cell*);

        bool save(const std::string&);

        inline std::string getName();
        inline std::string getUnits();
        inline double      getScale();

    private:
        Hurricane::Cell* _cell;
        std::string      _name;
        std::string      _units;
        double           _scale;
};

inline std::string CifDriver::getName()  { return _name;  };
inline std::string CifDriver::getUnits() { return _units; };
inline double      CifDriver::getScale() { return _scale; };
} // End of CRL namespace.


# endif


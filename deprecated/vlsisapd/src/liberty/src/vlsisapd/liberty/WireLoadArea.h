#ifndef __LIB_WIRE_LOAD_AREA_H__
#define __LIB_WIRE_LOAD_AREA_H__

#include<fstream>

#include "vlsisapd/liberty/Name.h"

namespace LIB {

class WireLoadArea {
    public:
    WireLoadArea(double min, double max, Name name);

    inline double getMin();
    inline double getMax();
    inline Name   getName();

    bool write(std::ofstream &file);

    private:
    double _min;
    double _max;
    Name   _name;
};

inline double WireLoadArea::getMin()  { return _min; };
inline double WireLoadArea::getMax()  { return _max; };
inline Name   WireLoadArea::getName() { return _name; };

} // namespace
#endif

#include<fstream>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/WireLoadArea.h"

namespace LIB {
WireLoadArea::WireLoadArea(double min, double max, Name name): _min(min), _max(max), _name(name) {};

bool WireLoadArea::write(std::ofstream &file) {
    file << "wire_load_from_area(" << _min << "," << _max << ",\"" << _name.getString() << "\");" << std::endl;
    return true;
}

} // namespace

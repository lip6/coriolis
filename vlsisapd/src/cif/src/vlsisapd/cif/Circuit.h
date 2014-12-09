#ifndef __CIF_CIRCUIT_H
#define __CIF_CIRCUIT_H

#include <vector>
#include <string>

namespace CIF {
class Polygon;
class Circuit {
    public:
        Circuit(std::string name, std::string unit, double scale);

        bool addPolygon ( Polygon* );
        bool writeToFile ( std::string );

    private:
        std::string _name;
        std::string _unit;
        double _scale;

        std::vector<Polygon*> _polygons;
};
} // namespace
#endif

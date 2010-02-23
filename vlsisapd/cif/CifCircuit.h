#ifndef __CIF_CIRCUIT_H
#define __CIF_CIRCUIT_H

#include <vector>

namespace IO {
class CifPolygon;
class CifCircuit {
    public:
        CifCircuit(string);

        bool addPolygon ( CifPolygon* );
        bool write ( string );

    private:
        string _name;

        std::vector<CifPolygon*> _polygons;
};
} // namespace
#endif

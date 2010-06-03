#ifndef __CIF_POLYGON_H
#define __CIF_POLYGON_H

#include <vector>
#include <fstream>

namespace vlsisapd {
class CifPolygon {
    public:
        CifPolygon(long);

        void addPoint (long, long);
        void write ( ofstream& );

    private:
        long _layer;
        std::vector<std::pair<long,long> > _points;
};
} // namespace
#endif

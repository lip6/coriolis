#ifndef __CIF_POLYGON_H
#define __CIF_POLYGON_H

#include <vector>
#include <fstream>

namespace CIF {
class Polygon {
    public:
        Polygon(long);

        void addPoint (long, long);
        void write ( ofstream& );

    private:
        long _layer;
        std::vector<std::pair<long,long> > _points;
};
} // namespace
#endif

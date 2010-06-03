#ifndef __GDS_RECTANGLE_H
#define __GDS_RECTANGLE_H

#include <fstream>

#include "GdsElement.h"

namespace vlsisapd {
class GdsRectangle : public GdsElement {
    public:
                      GdsRectangle (int layer, double xmin, double ymin, double xmax, double ymax);
        virtual      ~GdsRectangle ();
        virtual bool  write        ( ofstream &file );
    private:
        double _xmin;
        double _ymin;
        double _xmax;
        double _ymax;
};
}
#endif


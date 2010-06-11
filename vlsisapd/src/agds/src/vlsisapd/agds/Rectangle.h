#ifndef __GDS_RECTANGLE_H
#define __GDS_RECTANGLE_H

#include <fstream>

#include "vlsisapd/agds/Element.h"

namespace AGDS {
class Rectangle : public Element {
    public:
                      Rectangle (int layer, double xmin, double ymin, double xmax, double ymax);
        virtual      ~Rectangle ();
        virtual bool  write(std::ofstream &file);
    private:
        double _xmin;
        double _ymin;
        double _xmax;
        double _ymax;
};
}
#endif


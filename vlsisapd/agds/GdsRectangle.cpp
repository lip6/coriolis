#include <iostream>
#include <iomanip>
using namespace std;

#include "GdsRectangle.h"

namespace IO {
GdsElement::~GdsElement () { }

GdsRectangle::GdsRectangle(int layer, double xmin, double ymin, double xmax, double ymax) 
    : GdsElement(layer)
    , _xmin(xmin)
    , _ymin(ymin)
    , _xmax(xmax)
    , _ymax(ymax) {}

GdsRectangle::~GdsRectangle () { }

bool GdsRectangle::write(ofstream &file) {
    file << "BOUNDARY;" << endl
         << "LAYER " << _layer << ";" << endl
         << "DATATYPE 0;" << endl
         << "XY 5;" << endl
         << "  X: " << _xmin << ";\tY: " << _ymin << ";" << endl
         << "  X: " << _xmin << ";\tY: " << _ymax << ";" << endl
         << "  X: " << _xmax << ";\tY: " << _ymax << ";" << endl
         << "  X: " << _xmax << ";\tY: " << _ymin << ";" << endl
         << "  X: " << _xmin << ";\tY: " << _ymin << ";" << endl
         << "ENDEL;" << endl
         << endl;

    return true;
}
}

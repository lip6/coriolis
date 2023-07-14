#include <iostream>
#include <iomanip>
using namespace std;

#include "vlsisapd/agds/Rectangle.h"

namespace AGDS {
Element::~Element () { }

Rectangle::Rectangle(int layer, double xmin, double ymin, double xmax, double ymax) 
    : Element(layer)
    , _xmin(xmin)
    , _ymin(ymin)
    , _xmax(xmax)
    , _ymax(ymax) {}

Rectangle::~Rectangle () { }

bool Rectangle::write(ofstream &file) {
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
} // namespace

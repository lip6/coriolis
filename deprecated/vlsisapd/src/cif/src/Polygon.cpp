#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

#include "vlsisapd/cif/Polygon.h"

namespace CIF {
Polygon::Polygon(long layer) : _layer(layer) {}


void Polygon::addPoint(long x, long y) {
    _points.push_back(pair<long,long>(x,y));
}

void Polygon::write(ofstream& file) {
    file << "L " << _layer << "; P";

    // For each point : write point.
    for ( vector<pair<long,long> >::iterator it = _points.begin() ; it < _points.end() ; it++ ) {
        file << " " << (*it).first << "," << (*it).second;
    }

    file << ";" << endl;
}
} // namespace



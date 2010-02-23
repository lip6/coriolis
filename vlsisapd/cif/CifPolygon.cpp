#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

#include "CifPolygon.h"

namespace IO {

CifPolygon::CifPolygon(long layer) : _layer(layer) {}


void CifPolygon::addPoint(long x, long y) {
    _points.push_back(pair<long,long>(x,y));
}

void CifPolygon::write(ofstream& file) {
    file << "L " << _layer << "; P";

    // For each point : write point.
    for ( vector<pair<long,long> >::iterator it = _points.begin() ; it < _points.end() ; it++ ) {
        file << " " << (*it).first << "," << (*it).second;
    }

    file << ";" << endl;
}
} // namespace



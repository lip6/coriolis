#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

#include "CifCircuit.h"
#include "CifPolygon.h"

namespace IO {

CifCircuit::CifCircuit(string name, string unit, double scale) : _name(name), _unit(unit), _scale(scale) {}


bool CifCircuit::addPolygon(CifPolygon* polygon) {
    if(polygon)
        _polygons.push_back(polygon);
    else {
        cerr << "[CIF DRIVE ERROR]: cannot add invalid polygon." << endl;
        return false;
    }
    return true;
}

bool CifCircuit::write(string filename) {
    time_t curtime = time(0);
    tm now = *localtime(&curtime);
    char date[BUFSIZ]={0};
    const char format[]="%d-%b-%Y  %H:%M:%S";
    if (!strftime(date, sizeof(date)-1, format, &now)>0)
        cerr << "[CIF DRIVE ERROR]: cannot build current date." << endl;

    ofstream file;
    file.open(filename.c_str(), ios::out);
    // Header
    file << "(CIF file written on " << date << " by IO_CIF_DRIVER);" << endl
         << "(Units: " << _unit << "  -  UU/DB Scale: " << _scale << ");" << endl
         << "DS 1 1 1;" << endl
         << "9 " << _name << ";" << endl;

    // For each Polygon : write polygon.
    for ( vector<CifPolygon*>::iterator it = _polygons.begin() ; it < _polygons.end() ; it++ ) {
        (*it)->write(file);
    }

    // Footer
    file << "DF;" << endl
         << "C 1;" << endl
         << "E" << endl;

    file.close();
    return true;
}
} // namespace


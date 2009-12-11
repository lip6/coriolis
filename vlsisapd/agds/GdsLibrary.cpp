#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

#include "GdsLibrary.h"

namespace IO {

GdsLibrary::GdsLibrary(string libName) 
    : _libName(libName)
    , _userUnits(0.00)
    , _physUnits(0.00) {}


bool GdsLibrary::addStructure(GdsStructure* gdsStruct) {
    if(gdsStruct)
        _structs.push_back(gdsStruct);
    else {
        cerr << "[GDS DRIVE ERROR]: cannot hold GdsStructure: "  << gdsStruct->getName() << endl;
        return false;
    }
    return true;
}

bool GdsLibrary::write(string filename) {
    time_t curtime = time(0);
    tm now = *localtime(&curtime);
    char date[BUFSIZ]={0};
    const char format[]="%y-%m-%d  %H:%M:%S";
    if (!strftime(date, sizeof(date)-1, format, &now)>0)
        cerr << "[GDS DRIVE ERROR]: cannot build current date." << endl;

    ofstream file;
    file.open(filename.c_str(), ios::out);
    // Header
    file << "HEADER 5;" << endl
         << "BGNLIB;" << endl
         << "  LASTMOD {" << date << "};" << endl
         << "  LASTACC {" << date << "};" << endl
         << "LIBNAME " << _libName << ".DB;" << endl
         << "UNITS;" << endl
         << "  USERUNITS " << _userUnits << ";" << endl
         << "  PHYSUNITS " << _physUnits << ";" << endl
         << endl;

    // For each Struct : write struct.
    for ( vector<GdsStructure*>::iterator it = _structs.begin() ; it < _structs.end() ; it++ ) {
        (*it)->write(file);
    }

    // Footer
    file << "ENDLIB;" << endl;

    file.close();
    return true;
}
} // namespace

#include <iostream>
#include <ctime>
using namespace std;

#include "vlsisapd/agds/GdsStructure.h"

namespace vlsisapd {
GdsStructure::GdsStructure(string strName) 
    : _strName(strName) {}


bool GdsStructure::addElement(GdsElement* gdsElement) {
    if(gdsElement)
        _elements.push_back(gdsElement);
    else {
        cerr << "[GDS DRIVE ERROR]: cannot hold GdsElement." << endl;
        return false;
    }
    return true;
}

bool GdsStructure::write(ofstream &file) {
    time_t curtime = time(0);
    tm now = *localtime(&curtime);
    char date[BUFSIZ]={0};
    const char format[]="%y-%m-%d  %H:%M:%S";
    if (!strftime(date, sizeof(date)-1, format, &now)>0)
        cerr << "[GDS DRIVE ERROR]: cannot build current date." << endl;

    // Header
    file << "BGNSTR;" << endl
         << "  CREATION {" << date << "};" << endl
         << "  LASTMOD  {" << date << "};" << endl
         << "STRNAME " << _strName << ";" << endl
         << endl;

    // For each Element : write element.
    for ( vector<GdsElement*>::iterator it = _elements.begin() ; it < _elements.end() ; it++ ) {
        (*it)->write(file);
    }

    // Footer
    file << "ENDSTR;" << endl;

    return true;
}
} // namespace

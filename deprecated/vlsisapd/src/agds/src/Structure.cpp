#include <iostream>
#include <ctime>
using namespace std;

#include "vlsisapd/agds/Structure.h"
#include "vlsisapd/agds/Element.h"

namespace AGDS {
Structure::Structure(string strName) 
    : _strName(strName) {}


bool Structure::addElement(Element* gdsElement) {
    if(gdsElement)
        _elements.push_back(gdsElement);
    else {
        cerr << "[GDS DRIVE ERROR]: cannot hold Element." << endl;
        return false;
    }
    return true;
}

bool Structure::write(ofstream &file) {
    time_t curtime = time(0);
    tm now = *localtime(&curtime);
    char date[BUFSIZ]={0};
    const char format[]="%y-%m-%d  %H:%M:%S";
    if (strftime(date, sizeof(date)-1, format, &now) == 0)
        cerr << "[GDS DRIVE ERROR]: cannot build current date." << endl;

    // Header
    file << "BGNSTR;" << endl
         << "  CREATION {" << date << "};" << endl
         << "  LASTMOD  {" << date << "};" << endl
         << "STRNAME " << _strName << ";" << endl
         << endl;

    // For each Element : write element.
    for ( vector<Element*>::iterator it = _elements.begin() ; it < _elements.end() ; it++ ) {
        (*it)->write(file);
    }

    // Footer
    file << "ENDSTR;" << endl;

    return true;
}
} // namespace

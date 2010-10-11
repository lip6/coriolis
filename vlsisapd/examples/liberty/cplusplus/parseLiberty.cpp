#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Library.h"

int main ( int argc, char * argv[] ) {
    LIB::Library* library = LIB::Library::readFromFile("./testParse.lib");

    if ( library ) {
        // print of the library
        library->print();
        // print of one attribute in particular
        cout << "| area of inv_x1= " << library->getCell(LIB::Name("inv_x1"))->getAttribute(LIB::Name("area"))->valueAsString() << endl;
        cout << "| timing intrinsic_rise of nq of inv_x1= " << library->getCell(LIB::Name("inv_x1"))->getPin(LIB::Name("nq"))->getTiming(LIB::Name("i"))->getAttribute(LIB::Name("intrinsic_rise"))->valueAsString() << endl;
    } else {
        cerr << "library is NULL" << endl;
    }

    return 0;
}


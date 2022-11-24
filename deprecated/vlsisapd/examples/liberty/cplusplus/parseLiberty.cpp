#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Library.h"
#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/Timing.h"

int main ( int argc, char * argv[] ) {
    LIB::Library* library = LIB::Library::readFromFile("./testParse.lib");

    if ( library ) {
        // print of the library
        library->print();
        // print of one attribute in particular of a cell
        cout << "Area of inv_x1 : " << library->getCell(LIB::Name("inv_x1"))->getAttribute(LIB::Name("area"))->valueAsString() << endl;
        cout << "Timing intrinsic_rise of nq of inv_x1 : " << library->getCell(LIB::Name("inv_x1"))->getPin(LIB::Name("nq"))->getTiming(LIB::Name("i"))->getAttribute(LIB::Name("intrinsic_rise"))->valueAsString() << endl;
        // print of all the attributes of a cell
        cout << "Attributes of no2_x1 :" << endl;
        for(map<LIB::Name, LIB::Attribute*>::const_iterator it=library->getCell(LIB::Name("no2_x1"))->getAttributes().begin() ; it!=library->getCell(LIB::Name("no2_x1"))->getAttributes().end() ; ++it) {
            cout << "  name= " << (*it).first.getString()
                 << ", type= " << (*it).second->typeToString()
                 << ", value= " << (*it).second->valueAsString() << endl;
        }
        // print of all the timings of a pin
        cout << "Timing's atributes of pin nq of no2_x1 :" << endl;
        for (size_t i = 0 ; i < library->getCell(LIB::Name("no2_x1"))->getPin(LIB::Name("nq"))->getTimings().size() ; i++) {
            library->getCell(LIB::Name("no2_x1"))->getPin(LIB::Name("nq"))->getTimings()[i]->print();
        }
    } else {
        cerr << "library is NULL" << endl;
    }

    return 0;
}


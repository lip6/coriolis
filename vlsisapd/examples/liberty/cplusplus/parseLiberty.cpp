#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Library.h"

int main ( int argc, char * argv[] ) {
    LIB::Library* library = LIB::Library::readFromFile("./sxlib.lib");

    if ( library ) {
        library->print();
    } else {
        cerr << "library is NULL" << endl;
    }

//    library->writeToFile("./mySxlib.lib");

    return 0;
}


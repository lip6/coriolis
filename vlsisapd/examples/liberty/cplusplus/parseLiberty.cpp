#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Library.h"

int main ( int argc, char * argv[] ) {
    LIB::Library* library = LIB::Library::readFromFile("./testParse.lib");

    if ( library ) {
        library->print();
    } else {
        cerr << "library is NULL" << endl;
    }

    return 0;
}


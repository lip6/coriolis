#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/dtr/Techno.h"

int main(int argc, char * argv[]) {
    DTR::Techno* techno = DTR::Techno::readFromFile("./example.dtr.xml");

    cerr << "+-----------------------------+" << endl
         << "| technology:      " << techno->getName()    <<   "    |"   << endl
         << "| units:           " << techno->getUnit()    << "      |" << endl
         << "| version:         " << techno->getVersion() << "      |" << endl
         << "+-----------------------------+" << endl << endl;

    cerr << "transistorMinL                = " << techno->getValue("transistorMinL") << endl
         << "transistorMinW                = " << techno->getValueAsString("transistorMinW") << endl
         << "minWidth of metal1            = " << techno->getValue("minWidth", "metal1") << endl
         << "minSpacing of metal1          = " << techno->getValue("minWidth", "metal1") << endl
         << "minSpacing of active vs poly  = " << techno->getValue("minSpacing", "active", "poly") << endl
         << "minExtension active over poly = " << techno->getValue("minExtension", "poly", "active") << endl
         << "minArea of metal1             = " << techno->getValue("minArea", "metal1") << endl;

    return 0;
}


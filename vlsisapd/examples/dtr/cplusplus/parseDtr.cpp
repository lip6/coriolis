#include <iostream>
#include <string>
using namespace std;

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Name.h"

int main(int argc, char * argv[]) {
    DTR::Techno* techno = DTR::Techno::readFromFile("./example.dtr.xml");

    cerr << "+-----------------------------+" << endl
         << "| technology:      " << techno->getName().getString() << "    |"   << endl
         << "| units:           " << techno->getUnit().getString() << "      |" << endl
         << "+-----------------------------+" << endl << endl;

    cerr << "transistorMinL                = " << techno->getValue(DTR::Name("transistorMinL")) << endl
         << "transistorMinW                = " << techno->getValueAsString(DTR::Name("transistorMinW")) << endl
         << "minWidth of metal1            = " << techno->getValue(DTR::Name("minWidth"), DTR::Name("metal1")) << endl
         << "minSpacing of metal1          = " << techno->getValue(DTR::Name("minWidth"), DTR::Name("metal1")) << endl
         << "minSpacing of active vs poly  = " << techno->getValue(DTR::Name("minSpacing"), DTR::Name("active"), DTR::Name("poly")) << endl
         << "minExtension active over poly = " << techno->getValue(DTR::Name("minExtension"), DTR::Name("poly"), DTR::Name("active")) << endl
         << "minArea of metal1             = " << techno->getValue(DTR::Name("minArea"), DTR::Name("metal1")) << endl;

    return 0;
}


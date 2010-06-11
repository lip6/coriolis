#include <string>
using namespace std;

#include "vlsisapd/agds/Library.h"
#include "vlsisapd/agds/Structure.h"
#include "vlsisapd/agds/Rectangle.h"

int main(int argc, char * argv[]) {
    AGDS::Library* lib = new AGDS::Library(string("myTestLib"));

    lib->setUserUnits(0.001);
    lib->setPhysUnits(1.0E-9);

    AGDS::Rectangle* poly   = new AGDS::Rectangle( 17, 305, 150, 365, 830 );
    AGDS::Rectangle* active = new AGDS::Rectangle(  6, 130, 290, 540, 690 );

    AGDS::Structure* str = new AGDS::Structure("Transistor");

    str->addElement(poly);
    str->addElement(active);

    lib->addStructure(str);

    lib->writeToFile("./transistor.agds");
    
    return 0;
}


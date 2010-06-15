#include <string>
using namespace std;

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"
#include "vlsisapd/dtr/Name.h"

int main(int argc, char * argv[]) {
    DTR::Techno* techno = new DTR::Techno(DTR::Name("MyTech"), DTR::Name("micro"));

    techno->addRule (DTR::Name("transistorMinL"), 0.1 , DTR::Name("0.10"), DTR::Name("ref1"));
    techno->addRule (DTR::Name("transistorMinW"), 0.2 , DTR::Name("0.20"), DTR::Name("ref2"));
    techno->addRule (DTR::Name("minWidth")      , 0.15, DTR::Name("0.15"), DTR::Name("ref3"), DTR::Name("metal1"));
    techno->addRule (DTR::Name("minSpacing")    , 0.2 , DTR::Name("0.20"), DTR::Name("ref4"), DTR::Name("metal1"));
    techno->addRule (DTR::Name("minSpacing")    , 0.1 , DTR::Name("0.10"), DTR::Name("ref5"), DTR::Name("active"), DTR::Name("poly"));
    techno->addARule(DTR::Name("minExtension")  , 0.2 , DTR::Name("0.20"), DTR::Name("ref6"), DTR::Name("poly")  , DTR::Name("active"));
    
    DTR::Rule* rule = techno->addRule(DTR::Name("minArea"), 0.1, DTR::Name("0.10"), DTR::Name("ref7"), DTR::Name("metal1"));
    rule->setType(DTR::Name("area"));

    techno->writeToFile("./out.dtr.xml");
    
    return 0;
}


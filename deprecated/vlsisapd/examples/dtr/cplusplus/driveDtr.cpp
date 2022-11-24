#include <string>
using namespace std;

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"

int main(int argc, char * argv[]) {
    DTR::Techno* techno = new DTR::Techno("MyTech", "micro", "rev.A");

    techno->addRule ("transistorMinL", 0.1 , "ref1");
    techno->addRule ("transistorMinW", 0.2 , "ref2");
    techno->addRule ("minWidth"      , 0.15, "ref3", "metal1");
    techno->addRule ("minSpacing"    , 0.2 , "ref4", "metal1");
    techno->addRule ("minSpacing"    , 0.1 , "ref5", "active", "poly");
    techno->addARule("minExtension"  , 0.2 , "ref6", "poly"  , "active");
    
    DTR::Rule* rule = techno->addRule("minArea", 0.1, "ref7", "metal1");
    rule->setType("area");

    techno->writeToFile("./out.dtr.xml");
    
    return 0;
}


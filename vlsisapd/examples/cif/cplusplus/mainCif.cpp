#include <string>
using namespace std;

#include "vlsisapd/cif/Circuit.h"
#include "vlsisapd/cif/Polygon.h"

int main(int argc, char * argv[]) {
    CIF::Circuit* circuit = new CIF::Circuit(string("Transistor"), string("micro"), 0.001);

    // Layer #6 corresponds to active
    CIF::Polygon* poly = new CIF::Polygon(6);
    poly->addPoint(130, 290);
    poly->addPoint(540, 290);
    poly->addPoint(540, 690);
    poly->addPoint(130, 690);
    circuit->addPolygon(poly);

    // Layer #17 corresponds to polysilicium
    poly = new CIF::Polygon(17);
    poly->addPoint(305, 150);
    poly->addPoint(365, 150);
    poly->addPoint(365, 830);
    poly->addPoint(305, 830);
    circuit->addPolygon(poly);

    circuit->writeToFile("./transistor.cif");
    
    return 0;
}


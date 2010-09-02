#include <string>
using namespace std;

#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/Sizing.h"
#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/Layout.h"

int main(int argc, char * argv[]) {
    OpenChams::Circuit* circuit = new OpenChams::Circuit(OpenChams::Name("design"), OpenChams::Name("myTech"));
    // value parameters
    circuit->addParameter(OpenChams::Name("temp"), 27.0  );
    circuit->addParameter(OpenChams::Name("Vdd") , 1.2   );
    circuit->addParameter(OpenChams::Name("Vss") , 0.0   );
    circuit->addParameter(OpenChams::Name("L")   , 0.1e-6);
    circuit->addParameter(OpenChams::Name("Ids") , 30e-6 );
    circuit->addParameter(OpenChams::Name("Veg") , 0.12  );
    // equation parameters
    circuit->addParameter(OpenChams::Name("complex"), "myEq");

    // netlist
    OpenChams::Netlist* netlist = circuit->createNetlist();
    //  instances
    //   nmos1
    OpenChams::Instance* inst_nmos1 = netlist->addInstance(OpenChams::Name("nmos1"), OpenChams::Name("Transistor"), OpenChams::Name("NMOS"), true);
    inst_nmos1->addConnector(OpenChams::Name("G"));
    inst_nmos1->addConnector(OpenChams::Name("S"));
    inst_nmos1->addConnector(OpenChams::Name("D"));
    OpenChams::Transistor* tr_nmos1 = inst_nmos1->addTransistor(OpenChams::Name("m1"));
    tr_nmos1->setGate  (OpenChams::Name("G")); // the name of the connector of inst_nmos1
    tr_nmos1->setSource(OpenChams::Name("S"));
    tr_nmos1->setDrain (OpenChams::Name("D"));
    tr_nmos1->setBulk  (OpenChams::Name("S"));
    //   pmos1
    OpenChams::Instance* inst_pmos1 = netlist->addInstance(OpenChams::Name("pmos1"), OpenChams::Name("Transistor"), OpenChams::Name("PMOS"), true);
    inst_pmos1->addConnector(OpenChams::Name("G"));
    inst_pmos1->addConnector(OpenChams::Name("S"));
    inst_pmos1->addConnector(OpenChams::Name("D"));
    OpenChams::Transistor* tr_pmos1 = inst_pmos1->addTransistor(OpenChams::Name("m1"));
    tr_pmos1->setGate  (OpenChams::Name("G")); // the name of the connector of inst_pmos1
    tr_pmos1->setSource(OpenChams::Name("S"));
    tr_pmos1->setDrain (OpenChams::Name("D"));
    tr_pmos1->setBulk  (OpenChams::Name("S"));
    //  nets
    OpenChams::Net* _vdd = netlist->addNet(OpenChams::Name("vdd"), OpenChams::Name("power")  , true);
    OpenChams::Net* _vss = netlist->addNet(OpenChams::Name("vss"), OpenChams::Name("ground") , true);
    OpenChams::Net* _in  = netlist->addNet(OpenChams::Name("in" ), OpenChams::Name("logical"), true);
    OpenChams::Net* _out = netlist->addNet(OpenChams::Name("out"), OpenChams::Name("logical"), true);
    _vdd->connectTo(OpenChams::Name("pmos1"), OpenChams::Name("S"));
    _vss->connectTo(OpenChams::Name("nmos1"), OpenChams::Name("S"));
    _in->connectTo (OpenChams::Name("nmos1"), OpenChams::Name("G"));
    _in->connectTo (OpenChams::Name("pmos1"), OpenChams::Name("G"));
    _out->connectTo(OpenChams::Name("nmos1"), OpenChams::Name("D"));
    _out->connectTo(OpenChams::Name("pmos1"), OpenChams::Name("D"));

    // schematic
    OpenChams::Schematic* schematic = circuit->createSchematic(1.0);
    schematic->addInstance(OpenChams::Name("nmos1"), 2490, 2600, OpenChams::Name("ID"));
    schematic->addInstance(OpenChams::Name("pmos1"), 2490, 2300, OpenChams::Name("ID"));
    // sizing
    OpenChams::Sizing* sizing = circuit->createSizing();
    OpenChams::Operator* op_pmos1 = sizing->addOperator(OpenChams::Name("pmos1"), OpenChams::Name("OPVG(Veg)"), OpenChams::Name("BSIM3V3"), 0);
    op_pmos1->addConstraint(OpenChams::Name("Temp"), OpenChams::Name("design"), OpenChams::Name("temp"));
    op_pmos1->addConstraint(OpenChams::Name("Ids") , OpenChams::Name("design"), OpenChams::Name("Ids") );
    op_pmos1->addConstraint(OpenChams::Name("L")   , OpenChams::Name("design"), OpenChams::Name("L")   );
    op_pmos1->addConstraint(OpenChams::Name("Veg") , OpenChams::Name("design"), OpenChams::Name("Veg") );
    op_pmos1->addConstraint(OpenChams::Name("Vd")  , OpenChams::Name("design"), OpenChams::Name("Vdd") , 0.5);
    op_pmos1->addConstraint(OpenChams::Name("Vs")  , OpenChams::Name("design"), OpenChams::Name("Vdd") );
    OpenChams::Operator* op_nmos1 = sizing->addOperator(OpenChams::Name("nmos1"), OpenChams::Name("OPW(Vg,Vs)"), OpenChams::Name("BSIM3V3"), 1);
    op_nmos1->addConstraint(OpenChams::Name("Temp"), OpenChams::Name("design"), OpenChams::Name("temp"));
    op_nmos1->addConstraint(OpenChams::Name("Ids") , OpenChams::Name("design"), OpenChams::Name("Ids" ));
    op_nmos1->addConstraint(OpenChams::Name("L")   , OpenChams::Name("design"), OpenChams::Name("L"   ));
    op_nmos1->addConstraint(OpenChams::Name("Vs")  , OpenChams::Name("design"), OpenChams::Name("Vdd" ));
    op_nmos1->addConstraint(OpenChams::Name("Vg")  , OpenChams::Name("pmos1") , OpenChams::Name("Vg"  ));
    op_nmos1->addConstraint(OpenChams::Name("Vd")  , OpenChams::Name("pmos1") , OpenChams::Name("Vd"  ));
    op_nmos1->addConstraint(OpenChams::Name("another"), OpenChams::Name("myEq"), -2.5 );
    // layout
    OpenChams::Layout* layout = circuit->createLayout();
    layout->addInstance(OpenChams::Name("pmos1"), OpenChams::Name("Common transistor"));
    layout->addInstance(OpenChams::Name("nmos1"), OpenChams::Name("Rotate transistor"));

    circuit->writeToFile("./myInverter.xml");
    return 0;
}


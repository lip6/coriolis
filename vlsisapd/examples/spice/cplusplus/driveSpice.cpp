#include <string>
using namespace std;

#include "vlsisapd/spice/Circuit.h"
#include "vlsisapd/spice/Subckt.h"
#include "vlsisapd/spice/Instances.h"

int main(int argc, char * argv[]) {
    SPICE::Circuit* circuit = new SPICE::Circuit();
    circuit->setTitle("* Single-ended two-stage amplifier");

    // PARAMS
    circuit->addParameter("CC_VALUE", "2.8794pF");
    circuit->addParameter("L_VALUE" , "0.340e-6");

    // SUBCKTS
    // CurrentMirror
    SPICE::Subckt* CM = circuit->addSubckt("currentMirrorPMOS");
    CM->addInterface("d1");
    CM->addInterface("d2");
    CM->addInterface("s1");
    CM->addInterface("s2");
    CM->addParameter("l_val"   , "0.0"   );
    CM->addParameter("w_val"   , "0.0"   );
    CM->addParameter("nf_val"  , "1"     );
    CM->addParameter("aeq_val" , "100e-6");
    CM->addParameter("temp_val", "27"    );

    SPICE::Instance* cmP3 = new SPICE::Mosfet("P3", "d1", "d1", "s1", "s1", "psvt");
    cmP3->addParameter("l"       , "l_val"         );
    cmP3->addParameter("wf"      , "{w_val/nf_val}");
    cmP3->addParameter("nf"      , "nf_val"        );
    cmP3->addParameter("aeq"     , "aeq_val"       );
    cmP3->addParameter("tempsimu", "temp_val"      );
    CM->addInstance(cmP3);

    SPICE::Instance* cmP4 = new SPICE::Mosfet("P4", "d2", "d1", "s2", "s2", "psvt");
    cmP4->addParameter("l"       , "l_val"         );
    cmP4->addParameter("wf"      , "{w_val/nf_val}");
    cmP4->addParameter("nf"      , "nf_val"        );
    cmP4->addParameter("aeq"     , "aeq_val"       );
    cmP4->addParameter("tempsimu", "temp_val"      );
    CM->addInstance(cmP4);

    // DifferentialPair
    SPICE::Subckt* DP = circuit->addSubckt("diffPairNMOS");
    DP->addInterface("d1");
    DP->addInterface("d2");
    DP->addInterface("g1");
    DP->addInterface("g2");
    DP->addInterface("s");
    DP->addInterface("b");
    DP->addParameter("l_val"   , "0.0"   );
    DP->addParameter("w_val"   , "0.0"   );
    DP->addParameter("nf_val"  , "1"     );
    DP->addParameter("aeq_val" , "100e-6");
    DP->addParameter("temp_val", "27"    );

    SPICE::Instance* dpN1 = new SPICE::Mosfet("N1", "d1", "g1", "s", "b", "nsvt");
    dpN1->addParameter("l"       , "l_val"         );
    dpN1->addParameter("wf"      , "{w_val/nf_val}");
    dpN1->addParameter("nf"      , "nf_val"        );
    dpN1->addParameter("aeq"     , "aeq_val"       );
    dpN1->addParameter("tempsimu", "temp_val"      );
    DP->addInstance(dpN1);

    SPICE::Instance* dpN2 = new SPICE::Mosfet("N2", "d2", "g2", "s", "b", "nsvt");
    dpN2->addParameter("l"       , "l_val"         );
    dpN2->addParameter("wf"      , "{w_val/nf_val}");
    dpN2->addParameter("nf"      , "nf_val"        );
    dpN2->addParameter("aeq"     , "aeq_val"       );
    dpN2->addParameter("tempsimu", "temp_val"      );
    DP->addInstance(dpN2);

    //INSTANCES
    SPICE::Instance* iCM = new SPICE::Instance("CM", "currentMirrorPMOS");
    iCM->addConnector("1");
    iCM->addConnector("2");
    iCM->addConnector("vdd");
    iCM->addConnector("vdd");
    iCM->addParameter("l_val" , "L_VALUE"     );
    iCM->addParameter("w_val" , "3.889618e-06");
    iCM->addParameter("nf_val", "2"           );
    circuit->addInstance(iCM);

    SPICE::Instance* iDP = new SPICE::Instance("DP", "diffPairNMOS");
    iDP->addConnector("1");
    iDP->addConnector("2");
    iDP->addConnector("vim");
    iDP->addConnector("vip");
    iDP->addConnector("3");
    iDP->addConnector("vss");
    iDP->addParameter("l_val" , "L_VALUE"     );
    iDP->addParameter("w_val" , "7.683346e-07");
    iDP->addParameter("nf_val", "4"           );
    circuit->addInstance(iDP);

    SPICE::Instance* iP6 = new SPICE::Mosfet("P6", "vout", "2", "vdd", "vdd", "psvt");
    iP6->addParameter("l_val" , "L_VALUE"     );
    iP6->addParameter("w_val" , "3.558995e-05");
    iP6->addParameter("nf_val", "20"          );
    circuit->addInstance(iP6);

    SPICE::Instance* iN5 = new SPICE::Mosfet("N5", "3", "4", "vss", "vss", "nsvt");
    iN5->addParameter("l_val" , "L_VALUE"     );
    iN5->addParameter("w_val" , "2.536703e-06");
    iN5->addParameter("nf_val", "4"           );
    circuit->addInstance(iN5);

    SPICE::Instance* iN7 = new SPICE::Mosfet("N7", "vout", "4", "vss", "vss", "nsvt");
    iN7->addParameter("l_val" , "L_VALUE"     );
    iN7->addParameter("w_val" , "1.069083e-05");
    iN7->addParameter("nf_val", "16"          );
    circuit->addInstance(iN7);

    SPICE::Instance* iN8 = new SPICE::Mosfet("N8", "4", "4", "vss", "vss", "nsvt");
    iN8->addParameter("l_val" , "L_VALUE"     );
    iN8->addParameter("w_val" , "2.536703e-06");
    iN8->addParameter("nf_val", "4"           );
    circuit->addInstance(iN8);

    circuit->addInstance(new SPICE::Capacitor("C1", "vout", "2", "CC_VALUE"));

    circuit->writeToFile("./myOTA.spi");
    return 0;
}


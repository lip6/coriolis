#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "vlsisapd/spice/Circuit.h"
#include "vlsisapd/spice/SpiceException.h"
#include "vlsisapd/spice/Sources.h"
#include "vlsisapd/spice/Subckt.h"
#include "vlsisapd/spice/Instances.h"

int main(int argc, char * argv[]) {
    string file = "";
    if (argc == 1)
        file = "./OTA.cir";
    else if (argc == 2)
        file = argv[1];
    else {
        cerr << "Usage: parseSpice [filename]" << endl;
        exit(1);
    }

    SPICE::Circuit* circuit = NULL;
    try {
        circuit = SPICE::Circuit::readFromFile(file);
    } catch (SPICE::SpiceException& e) {
        cerr << e.what() << endl;
        exit(48);
    }

//    if (!circuit) cerr << "circuit is NULL !!" << endl;
    // TITLE
    cerr << "+ " << circuit->getTitle() << endl;
    // INCLUDES
    vector<string> includes = circuit->getIncludes();
    if (includes.size()) {
        cerr << "| + includes" << endl;
        for (size_t i = 0 ; i < includes.size() ; i++)
            cerr << "| | " << includes[i] << endl;
    }
    // LIBRARIES
    vector<pair<string, string> > libs = circuit->getLibraries();
    if (libs.size()) {
        cerr << "| + libraries" << endl;
        for (size_t i = 0 ; i < libs.size() ; i++)
            cerr << "| | " << libs[i].first << " " << libs[i].second << endl;
    }
    // PARAMETERS
    map<string, string> params = circuit->getParameters();
    if (params.size()) {
        cerr << "| + parameters" << endl;
        for (map<string, string>::const_iterator it = params.begin() ; it != params.end() ; ++it)
            cerr << "| | " << (*it).first << " = " << (*it).second << endl;
    }
    // OPTIONS
    map<string, string> opts = circuit->getOptions();
    if (opts.size()) {
        cerr << "| + options" << endl;
        for (map<string, string>::const_iterator it = opts.begin() ; it != opts.end() ; ++it)
            cerr << "| | " << (*it).first << " = " << (*it).second << endl;
    }
    // SOURCES
    vector<SPICE::Source*> sources = circuit->getSources();
    if (sources.size()) {
        cerr << "| + sources" << endl;
        for (size_t i = 0 ; i < sources.size() ; i++) {
            SPICE::Source* s = sources[i];
            cerr << "| | " <<  s->getName() << " " << s->getPositive() << " " << s->getNegative() << " " << s->getValue() << endl;
        }

    }
    // SUBCKTS
    vector<SPICE::Subckt*> subs = circuit->getSubckts();
    if (subs.size()) {
        cerr << "| + subckts" << endl;
        for (size_t i = 0 ; i < subs.size() ; i++) {
            SPICE::Subckt* sub = subs[i];
            cerr << "| | + " << sub->getName(); 
            for (size_t j = 0 ; j < sub->getInterfaces().size() ; j++)
                cerr << " " << sub->getInterfaces()[j];
            if (sub->getParameters().size()) {
                cerr << " param:";
                for (map<string, string>::const_iterator it = sub->getParameters().begin() ; it != sub->getParameters().end() ; ++it)
                    cerr << " " << (*it).first << "=" << (*it).second;
            }
            cerr << endl;
            for (size_t j = 0 ; j < sub->getInstances().size() ; j++) {
                SPICE::Instance* inst = sub->getInstances()[j];
                cerr << "| | | + " << inst->getName();
                if (dynamic_cast<SPICE::Mosfet*>(inst)) {
                    SPICE::Mosfet* mos = static_cast<SPICE::Mosfet*>(inst);
                    cerr << " " << mos->getDrain() << " " << mos->getGrid() << " " << mos->getSource() << " " << mos->getBulk() << " " << mos->getModel();
                    int k = 0;
                    for (map<string, string>::const_iterator it =mos->getParameters().begin() ; it != mos->getParameters().end(); ++it, k++) {
                        if (k%6 == 0)
                            cerr << endl << "| | | | +";
                        cerr << " " << (*it).first << "=" << (*it).second;
                    }
                } else if (dynamic_cast<SPICE::Resistor*>(inst)) {
                    SPICE::Resistor* res = static_cast<SPICE::Resistor*>(inst);
                    cerr << " " << res->getFirst() << " " << res->getSecond() << " " << res->getValue();
                } else if (dynamic_cast<SPICE::Capacitor*>(inst)) {
                    SPICE::Capacitor* capa = static_cast<SPICE::Capacitor*>(inst);
                    cerr << " " << capa->getPositive() << " " << capa->getNegative() << " " << capa->getValue();
                } else {
                    for (size_t k = 0 ; k < inst->getConnectors().size() ; k++)
                        cerr << " " << inst->getConnectors()[k];
                    cerr << " " << inst->getModel();
                    int l = 0;
                    for (map<string, string>::const_iterator it = inst->getParameters().begin() ; it != inst->getParameters().end() ; ++it, l++) {
                        if (l%6 == 0)
                            cerr << endl << "| | | | +";
                        cerr << " " << (*it).first << "=" << (*it).second;
                    }
                }
                cerr << endl;
            }
        }
    }
    // INSTANCES
    vector<SPICE::Instance*> insts = circuit->getInstances();
    if (insts.size()) {
        cerr << "| + instances" << endl;
        for (size_t i = 0 ; i < insts.size() ; i++) {
            SPICE::Instance* inst = insts[i];
            cerr << "| | + " << inst->getName();
            if (dynamic_cast<SPICE::Mosfet*>(inst)) {
                SPICE::Mosfet* mos = static_cast<SPICE::Mosfet*>(inst);
                cerr << " " << mos->getDrain() << " " << mos->getGrid() << " " << mos->getSource() << " " << mos->getBulk() << " " << mos->getModel();
                int j = 0;
                for (map<string, string>::const_iterator it =mos->getParameters().begin() ; it != mos->getParameters().end(); ++it, j++) {
                    if (j%6 == 0)
                        cerr << endl << "| | | | +";
                    cerr << " " << (*it).first << "=" << (*it).second;
                }
            } else if (dynamic_cast<SPICE::Resistor*>(inst)) {
                SPICE::Resistor* res = static_cast<SPICE::Resistor*>(inst);
                cerr << " " << res->getFirst() << " " << res->getSecond() << " " << res->getValue();
            } else if (dynamic_cast<SPICE::Capacitor*>(inst)) {
                SPICE::Capacitor* capa = static_cast<SPICE::Capacitor*>(inst);
                cerr << " " << capa->getPositive() << " " << capa->getNegative() << " " << capa->getValue();
            } else {
                for (size_t k = 0 ; k < inst->getConnectors().size() ; k++)
                    cerr << " " << inst->getConnectors()[k];
                cerr << " " << inst->getModel();
                int l = 0;
                for (map<string, string>::const_iterator it = inst->getParameters().begin() ; it != inst->getParameters().end() ; ++it, l++) {
                    if (l%6 == 0)
                        cerr << endl << "| | | +";
                    cerr << " " << (*it).first << "=" << (*it).second;
                }
            }
            cerr << endl;
        }
    }
    return 0;
}


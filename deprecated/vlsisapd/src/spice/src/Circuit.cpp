#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

#include "vlsisapd/spice/Circuit.h"
#include "vlsisapd/spice/Value.h"
#include "vlsisapd/spice/Subckt.h"
#include "vlsisapd/spice/Instances.h"
#include "vlsisapd/spice/Sources.h"
#include "vlsisapd/spice/SpiceException.h"

namespace SPICE {
//#define DEBUG 
void Circuit::addOption(string name, string value) {
    map<string, string>::iterator it = _options.find(name);
    if (it != _options.end()) {
        cerr << "[WARNING] SPICE:: Cannot add option " << name
             << " to Circuit since it already exists." << endl;
        return;
    }
    _options[name] = value;
}

void Circuit::addParameter(string name, string value) {
    map<string, string>::iterator pit = _parameters.find(name);
    if (pit != _parameters.end()) {
        cerr << "[WARNING] SPICE:: Cannot add parameter " << name
             << " to Circuit since it already exists." << endl;
        return;
    }
    _parameters[name] = value;
}

Subckt* Circuit::addSubckt(string name) {
    Subckt* sub = new Subckt(name);
    if (!sub)
        throw SpiceException("SPICE::Circuit::addSubckt: Could not create new Subckt !");

    _subckts.push_back(sub);
    return sub;
}

Circuit* Circuit::readFromFile(const string& filename) {
    cerr << "reading file: " << filename << endl;
    std::ifstream spfile(filename.c_str());
    if (!spfile)
        throw SpiceException("SPICE::Circuit::readFromFile: Cannot open file: "+filename+" !");
    Circuit* circuit = new Circuit();
    if (!circuit)
        throw SpiceException("SPICE::Circuit::readFromFile: Cannot create new Circuit !");
    string line;
    // catch first line
    if (getNextLineType(spfile, line) != Circuit::ENDFILE) {
        #ifdef DEBUG
        cerr  << "TITLE    | " << line << endl;
        #endif
        circuit->setTitle(line);
    } else {
        throw SpiceException("SPICE::Circuit::readFromFile: File seems empty !");
    }

    bool inSubckt = false; // to know whether parsed line is in subckt or not
    Subckt*   sub  = NULL;
    Circuit::LineType type = getNextFullLineType(spfile, line);
    while (type != Circuit::ENDFILE) {
        // decompose line into tokens
        vector<string> tokens;
        tokenize(line, tokens);
        // based on line type: do something
        switch(type) {
        case Circuit::INCLUDE:
            {
            #ifdef DEBUG
            cerr  << "INCLUDE  | " << line << endl;
            #endif
            circuit->addInclude(tokens[1]);
            break;
            }
        case Circuit::LIBRARY:
            {
            #ifdef DEBUG
            cerr  << "LIBRARY  | " << line << endl;
            #endif
            if (tokens.size() == 3)
                circuit->addLibrary(tokens[1], tokens[2]); // with LIBTYPE
            else
                circuit->addLibrary(tokens[1]); // without LIBTYPE
            break;
            }
        case Circuit::PARAM:
            {
            #ifdef DEBUG
            cerr  << "PARAM    | " << line << endl;
            #endif
            string name, value;
            parametrize(tokens[1], name, value); // value must be a numeric value 
            circuit->addParameter(name, value);
            break;
            }
        case Circuit::OPTION:
            {
            #ifdef DEBUG
            cerr  << "OPTION   | " << line << endl;
            #endif
            for (size_t i = 1 ; i < tokens.size() ; i++) {
                string name, value;
                parametrize(tokens[i], name, value);
                circuit->addOption(name, value);
            }
            break;
            }
        case Circuit::SUBCKT:
            {
            #ifdef DEBUG
            cerr  << "SUBCKT   | " << line << endl;
            #endif
            if (inSubckt)   throw SpiceException("SPICE::Circuit::readFromFile: Found hierarchic .subckt !");
            inSubckt = true;
            sub = circuit->addSubckt(tokens[1]);
            size_t paramTok = tokens.size();
            for (size_t i = 2 ; i < tokens.size() ; i++)
                if ((tokens[i] == "param:") || (tokens[i] == "PARAM:"))
                    paramTok = i;
            for (size_t i = 2 ; i < paramTok ; i++)    
                sub->addInterface(tokens[i]);
            for (size_t i = paramTok+1 ; i < tokens.size() ; i++) {
                string name, value;
                parametrize(tokens[i], name, value);
                sub->addParameter(name, value);
            }
            break;
            }
        case Circuit::ENDSUB:
            #ifdef DEBUG
            cerr  << "ENDSUB   | " << line << endl;
            #endif
            if (!inSubckt)  throw SpiceException("SPICE::Circuit::readFromFile: Cannot close Subckt since it was not opened !");
            inSubckt = false;
            break;
        case Circuit::END:
            #ifdef DEBUG
            cerr  << "END      | " << line << endl;
            #endif
            break;
        case Circuit::INSTANCE:
            {
            #ifdef DEBUG
            cerr  << "INSTANCE | " << line << endl;
            #endif
            size_t modelIdx = 0;
            for (size_t i = tokens.size()-1 ; i >= 1 ; i--) {
                if(tokens[i].find("=") == string::npos) {
                    modelIdx = i;
                    break;
                }
            }
            Instance* inst = new Instance(tokens[0].erase(0,1), tokens[modelIdx]);
            for (size_t i = 1 ; i < modelIdx ; i++)
                inst->addConnector(tokens[i]);
            for (size_t i = modelIdx+1 ; i < tokens.size() ; i++) {
                string name, value;
                parametrize(tokens[i], name, value);
                inst->addParameter(name, value);
            }
            if (inSubckt) { // must add instance to current subckt
                sub->addInstance(inst);
            } else {        // instance is added to top-level circuit
                circuit->addInstance(inst);
            }
            break;
            }
        case Circuit::VOLTAGE:
            {
            #ifdef DEBUG
            cerr  << "VOLTAGE  | " << line << endl;
            #endif
            circuit->addSource(new Voltage(tokens[0], tokens[1], tokens[2], tokens[3]));
            break;
            }
        case Circuit::CURRENT:
            #ifdef DEBUG
            cerr  << "CURRENT  | " << line << endl;
            #endif
            break;
        case Circuit::MOSFET:
            {
            #ifdef DEBUG
            cerr  << "MOSFET   | " << line << endl;
            #endif
            //                       name                  drain     grid        source     bulk       model
            Mosfet* mos = new Mosfet(tokens[0].erase(0,1), tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
            for (size_t i = 6 ; i < tokens.size() ; i++) {
                string name, value;
                parametrize(tokens[i], name, value);
                mos->addParameter(name, value);
            }
            if (inSubckt) { // must add mosfet to current subckt
                sub->addInstance(mos);
            } else {        // instance is mosfet to top-level circuit
                circuit->addInstance(mos);
            }
            break;
            }
        case Circuit::CAPACITOR:
            {
            #ifdef DEBUG
            cerr  << "CAPACITOR| " << line << endl;
            #endif
            Instance* inst = new Capacitor(tokens[0].erase(0,1), tokens[1], tokens[2], tokens[3]);
            if (inSubckt) { // must add instance to current subckt
                sub->addInstance(inst);
            } else {        // instance is added to top-level circuit
                circuit->addInstance(inst);
            }
            break;
            }
        case Circuit::RESISTOR:
            {
            #ifdef DEBUG
            cerr  << "RESISTOR | " << line << endl;
            #endif
            Instance* inst = new Resistor(tokens[0].erase(0,1), tokens[1], tokens[2], tokens[3]);
            if (inSubckt) { // must add instance to current subckt
                sub->addInstance(inst);
            } else {        // instance is added to top-level circuit
                circuit->addInstance(inst);
            }
            break;
            }
        case Circuit::DIODE:
            #ifdef DEBUG
            cerr  << "DIODE    | " << line << endl;
            #endif
            break;
        case Circuit::UNDEF:
            #ifdef DEBUG
            cerr  << "UNDEF    | " << line << endl;
            #endif
            break;
        default:
            {
            ostringstream oss;
            oss << type;
            throw SpiceException("SPICE::Circuit::readFromFile: Unkown LineType "+oss.str()+" !");
            }
        }
        // prepare to process next line 
        type = getNextFullLineType(spfile, line);
    }
    spfile.close();
    return circuit;
}

void Circuit::writeToFile(const string& filename) {
    cerr << "writing file: " << filename << endl;
    std::ofstream spfile(filename.c_str());
    if (!spfile)
        throw SpiceException("SPICE::Circuit::writeToFile: Cannot open file: "+filename+" !");
    spfile << _title << endl;
    spfile << "* generated with VLSISAPD - SPICE parser/driver" << endl << endl;

    if (_includes.size()) {
        spfile << "* includes" << endl;
        for (size_t i = 0 ; i < _includes.size() ; i++)
            spfile << ".INCLUDE " << _includes[i] << endl;
        spfile << endl;
    }

    if ( _libraries.size()) {
        spfile << "* libraries" << endl;
        for (size_t i = 0 ; i < _libraries.size() ; i++)
            spfile << ".LIB " << _libraries[i].first << " " << _libraries[i].second << endl;
        spfile << endl;
    }

    if (_parameters.size()) {
        spfile << "* parameters" << endl;
        for (map<string, string>::const_iterator it = _parameters.begin() ; it != _parameters.end() ; ++it) 
            spfile << ".PARAM " << (*it).first << "=" << (*it).second << endl; 
        spfile << endl;
    }

    if (_options.size()) {
        spfile << "* options" << endl;
        for (map<string, string>::const_iterator it = _options.begin() ; it != _options.end() ; ++it) {
            spfile << ".OPTION " << (*it).first;
            if ((*it).second != "")
                spfile << "=" << (*it).second;
            spfile << endl;
        }
        spfile << endl;
    }

    if (_sources.size()) {
        spfile << "* sources" << endl;
        for (size_t i = 0 ; i < _sources.size() ; i++) {
            Source* s = _sources[i];
            spfile << s->getName() << " " << s->getPositive() << " " << s->getNegative() << " " << s->getValue() << endl;
        }
        spfile << endl;
    }

    if (_subckts.size()) {
        spfile << "* subckts" << endl;
        for (size_t i = 0 ; i < _subckts.size() ; i++) {
            Subckt* sub = _subckts[i];
            spfile << ".SUBCKT " << sub->getName();
            for (size_t j = 0 ; j < sub->getInterfaces().size() ; j++)
                spfile << " " << sub->getInterfaces()[j];
            if (sub->getParameters().size()) {
                spfile << " PARAM:";
                for(map<string, string>::const_iterator it = sub->getParameters().begin() ; it != sub->getParameters().end() ; ++it)
                    spfile << " " << (*it).first << "=" << (*it).second;
            }
            spfile << endl;
            for (size_t j = 0 ; j < sub->getComments().size() ; j++)
                spfile << "* " << sub->getComments()[j] << endl;
            for (size_t j = 0 ; j < sub->getInstances().size() ; j++) {
                Instance* inst = sub->getInstances()[j];
                writeInstance(spfile, inst);
            }
            spfile << ".ENDS " << sub->getName()<< endl << endl; // to separate consecutive subckts
        }
    }

    if (_instances.size()) {
        spfile << "* instances" << endl;
        for (size_t i = 0 ; i < _instances.size() ; i++) {
            Instance* inst = _instances[i];
            writeInstance(spfile, inst);
        }
    }

    spfile << ".END" << endl;
    spfile.close();
}

// this method ignores comment/empty lines and concatenates "+ lines" 
Circuit::LineType Circuit::getNextFullLineType(std::ifstream& file, string& fullLine) {
    string line;
    Circuit::LineType type = getNextLineType(file, line);
    while ((type == Circuit::COMMENT) || (type == Circuit::EMPTY)) {
        type = getNextLineType(file, line);
    }
    Circuit::LineType baseType = type; // non empty/comment line
    fullLine = string(line);
    int lineStart = file.tellg();
    type = getNextLineType(file, line);
    while ((type == Circuit::CONTINUE) || (type == Circuit::EMPTY) || (type == Circuit::COMMENT)) {
        if (type == Circuit::CONTINUE) {
            fullLine += line.erase(0,1); // remove the '+' sign
        } else {
//        // it is a comment/empty line
        }
        lineStart = file.tellg();
        type = getNextLineType(file, line);
    }
//
    file.seekg(lineStart); // unget last line which is needed (at worst we wil reread a comment)
    return baseType;
}

Circuit::LineType Circuit::getNextLineType(std::ifstream& file, string& line) {
    if (getline(file, line)) {
        if      (lineBeginsWith(line, "*"))        return Circuit::COMMENT;
        else if (lineBeginsWith(line, ".include")) return Circuit::INCLUDE;
        else if (lineBeginsWith(line, ".lib"))     return Circuit::LIBRARY;
        else if (lineBeginsWith(line, ".param"))   return Circuit::PARAM;
        else if (lineBeginsWith(line, ".option"))  return Circuit::OPTION;
        else if (lineBeginsWith(line, ".subckt"))  return Circuit::SUBCKT;
        else if (lineBeginsWith(line, ".ends"))    return Circuit::ENDSUB;
        else if (lineBeginsWith(line, ".end"))     return Circuit::END;
        else if (lineBeginsWith(line, "+"))        return Circuit::CONTINUE;
        else if (lineBeginsWith(line, "x"))        return Circuit::INSTANCE;
        else if (lineBeginsWith(line, "m"))        return Circuit::MOSFET;
        else if (lineBeginsWith(line, "v"))        return Circuit::VOLTAGE;
        else if (lineBeginsWith(line, "i"))        return Circuit::CURRENT;
        else if (lineBeginsWith(line, "c"))        return Circuit::CAPACITOR;
        else if (lineBeginsWith(line, "r"))        return Circuit::RESISTOR;
        else if (lineBeginsWith(line, "d"))        return Circuit::DIODE;
        else if (line == "")                       return Circuit::EMPTY;
        else                                       return Circuit::UNDEF;
    } else {
        return Circuit::ENDFILE;
    }
}

bool Circuit::lineBeginsWith(string line, string substr) {
    int size = substr.size();
    string substrU (substr);
    transform(substrU.begin(), substrU.end(), substrU.begin(), ::toupper);
    if((!line.compare(0, size, substr)) or (!line.compare(0, size, substrU)))
        return true;
    return false;
}

void Circuit::cleanMultipleSpaces(string& line) {
    size_t it;

    // first replace all tabs with spaces
    string search = "\t";
    while ((it = line.find(search)) != string::npos)
        line.replace(it, 1, " ");

    // then remove multiple spaces
    search = "  ";
    while ((it = line.find(search)) != string::npos)
        line.erase(it, 1);

    // finally replace ' = ' with '=' in two steps
    search = " =";
    while ((it = line.find(search)) != string::npos)
        line.erase(it, 1);
    search = "= ";
    while ((it = line.find(search)) != string::npos)
        line.erase(it+1, 1);
}

void Circuit::tokenize(string line, vector<string>& tokens) {
    cleanMultipleSpaces(line);
    istringstream iss(line);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
}

void Circuit::parametrize(string param, string& name, string& value) {
    size_t it = param.find("=");
    if (it == string::npos) {
        name  = param;
        value = "";
    } else {
        name  = param.substr(0, it);
        value = param.substr(it+1, param.size()-it);
    }
    #ifdef DEBUG
    cerr << "parametrize: " << name << "=" << value << endl;
    #endif
}

void Circuit::writeInstance(std::ofstream& file, Instance* inst) {
    if (dynamic_cast<Mosfet*>(inst)) {
        Mosfet* mos = static_cast<Mosfet*>(inst);
        file << "M" << mos->getName() << " " << mos->getDrain() << " " << mos->getGrid() << " " << mos->getSource() << " " << mos->getBulk() << " " << mos->getModel();
        int j = 0;
        for (map<string, string>::const_iterator it =mos->getParameters().begin() ; it != mos->getParameters().end(); ++it, j++) {
            if (j%6 == 0)
                file << endl << "+";
            file << " " << (*it).first << "=" << (*it).second;
        }
    } else if (dynamic_cast<Capacitor*>(inst)) {
        Capacitor* capa = static_cast<Capacitor*>(inst);
        file << "C" << capa->getName() << " " << capa->getPositive() << " " << capa->getNegative() << " " << capa->getValue();
    } else if (dynamic_cast<Resistor*>(inst)) {
        Resistor* res = static_cast<Resistor*>(inst);
        file << "R" << res->getName() << " " << res->getFirst() << " " << res->getSecond() << " " << res->getValue();
    } else {
        file << "X" << inst->getName();
        for (vector<string>::const_iterator it = inst->getConnectors().begin() ; it != inst->getConnectors().end() ; ++it) {
            file << " " << (*it);
        }
        file << " " << inst->getModel();
        int j=0;
        for (map<string, string>::const_iterator it = inst->getParameters().begin() ; it != inst->getParameters().end() ; ++it, j++) {
            if (j%6 == 0)
                file << endl << "+";
            file << " " << (*it).first << "=" << (*it).second;
        }
    }
    file << endl;
}
}

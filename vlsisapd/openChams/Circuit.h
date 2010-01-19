/*
 *  Circuit.h
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6 All rights reserved.
 *
 */

#ifndef __OPENCHAMS_CIRCUIT_H__
#define __OPENCHAMS_CIRCUIT_H__

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Name.h"
#include "Parameters.h"
#include "Netlist.h"
#include "Instance.h"
#include "Net.h"

namespace OpenChams {
class Circuit {
    public:
    Circuit(Name name, Name techno);
    
    inline Name   getName();
    inline Name   getTechno();
    inline double getValue(Name);
    inline void   addParameter(Name, double);
    
    bool writeToFile(string filePath);
    static Circuit* readFromFile(string filePath);
    
	private:
    Name      readParameter(xmlNode*, double&);
    Name      readConnector(xmlNode*);
    void      readCircuitParameters(xmlNode*);
    void      readNetList(xmlNode*);
    void      readInstances(xmlNode*, Netlist*);
    Instance* readInstance (xmlNode*, Netlist*);
    void      readInstanceParameters(xmlNode*, Instance*);
    void      readInstanceConnectors(xmlNode*, Instance*);
    void      readNets(xmlNode*, Netlist*);
    Net*      readNet (xmlNode*, Netlist*);
    void      readNetConnector(xmlNode*, Net*);
    
    
    Name 	   _name;
    Name 	   _techno;
    Parameters _params;
    Netlist*   _netlist;
};
    
inline Name   Circuit::getName()   		   { return _name; }    
inline Name   Circuit::getTechno()         { return _techno; }
inline double Circuit::getValue(Name name) { return _params.getValue(name); }
inline void   Circuit::addParameter(Name name, double value) { _params.addParameter(name, value); }
    
    
} // namespace IO
#endif


/*
 *  NetList.h
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_NETLIST_H__
#define __OPENCHAMS_NETLIST_H__

#include <vector>
using namespace std;

#include "Name.h"
#include "Instance.h"
#include "Net.h"

namespace OpenChams {
class Circuit;
class Netlist {
	public:
    Netlist(Circuit*);
        
    void addInstance(Instance*);
    void addNet(Net*);
    
    Instance* getInstance(Name);
    Net* getNet(Name);
    
    //pour parcourir les vector
    inline bool hasNoInstances();
    inline bool hasNoNets();
    inline vector<Instance*>::iterator getFirstInstanceIt();
    inline vector<Instance*>::iterator getLastInstanceIt();
    inline vector<Net*>::iterator getFirstNetIt();
    inline vector<Net*>::iterator getLastNetIt();
        
    private:
    Circuit*          _circuit;
    vector<Instance*> _instances;
    vector<Net*>      _nets;
};
    
inline bool 					   Netlist::hasNoInstances() 	 { return (_instances.size() == 0)? true : false; }
inline bool 					   Netlist::hasNoNets()      	 { return (_nets.size() == 0)? true : false; }
inline vector<Instance*>::iterator Netlist::getFirstInstanceIt() { return _instances.begin(); }
inline vector<Instance*>::iterator Netlist::getLastInstanceIt()  { return _instances.end(); }
inline vector<Net*>::iterator      Netlist::getFirstNetIt()      { return _nets.begin(); }
inline vector<Net*>::iterator      Netlist::getLastNetIt()       { return _nets.end(); }
    
} // namespace
#endif
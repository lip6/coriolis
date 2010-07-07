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

namespace OpenChams {
class Name;
class Instance;
class Net;
class Circuit;

class Netlist {
	public:
    Netlist(Circuit*);
        
    Instance* addInstance(Name name, Name model, Name mosType, bool);
    Net*      addNet     (Name name, Name type , bool);
    
    Instance* getInstance(Name);
    Net* getNet(Name);
    
    inline bool hasNoInstances();
    inline bool hasNoNets();
    inline const std::vector<Instance*>& getInstances();
    inline const std::vector<Net*>& getNets();
    
    private:
    Circuit*          _circuit;
    std::vector<Instance*> _instances;
    std::vector<Net*>      _nets;
};
    
inline bool Netlist::hasNoInstances() { return (_instances.size() == 0)? true : false; }
inline bool Netlist::hasNoNets()      { return (_nets.size() == 0)? true : false; }
inline const std::vector<Instance*>& Netlist::getInstances()   { return _instances; };
inline const std::vector<Net*>&      Netlist::getNets()        { return _nets; };
    
} // namespace
#endif


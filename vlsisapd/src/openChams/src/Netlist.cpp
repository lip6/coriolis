
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Netlist.cpp"                                 |
// +-----------------------------------------------------------------+


#include <iostream>
#include <algorithm>
using namespace std;

#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/OpenChamsException.h"


namespace OpenChams {


  Netlist::Netlist(Circuit* circuit)
    : _circuit(circuit)
  { }

    
  Instance* Netlist::addInstance(const std::string& name, const std::string& model, unsigned order)
  {
    for (vector<Instance*>::iterator it = _instances.begin() ; it != _instances.end() ; ++it) {
      if ((*it)->getName() == name) {
        string error("[ERROR] Cannot define two instances with the same name in netlist (");
        error += name;
        error += ").";
        throw OpenChamsException(error);
      }
    }
    Instance* inst = new Instance(name, model, order, this);
    if (!inst)
      throw OpenChamsException("[ERROR] Cannot create instance.");
    _instances.push_back(inst);
    
    return inst;
  }


  Device* Netlist::addDevice(const std::string& name, const std::string& model, unsigned order, const std::string& mosType, bool sourceBulkConnected)
  {
    for (vector<Instance*>::iterator it = _instances.begin() ; it != _instances.end() ; ++it) {
      if ((*it)->getName() == name) {
        string error("[ERROR] Cannot define two instances with the same name in netlist (");
        error += name;
        error += ").";
        throw OpenChamsException(error);
      }
    }
    Device* dev = new Device(name, model, order, mosType, sourceBulkConnected, this);
    if (!dev)
      throw OpenChamsException("[ERROR] Cannot create device.");
    _instances.push_back(dev);

    return dev;
  }


  Net* Netlist::addNet(const std::string& name, const std::string& type, bool external)
  {
	for (vector<Net*>::iterator it = _nets.begin() ; it != _nets.end() ; ++it ) {
      if ((*it)->getName() == name) {
        string error("[ERROR] Cannot define two nets with the same name in netlist (");
        error += name;
        error += ").";
        throw OpenChamsException(error);
      }
    }
    Net* net = new Net(name, type, external, this);
    if (!net)
      throw OpenChamsException("[ERROR] Cannot create net.");

    _nets.push_back(net);

    return net;
  }

 
  Instance* Netlist::getInstance(const std::string& instanceName)
  {
    for (size_t i = 0 ; i < _instances.size() ; ++i) {
      if (_instances[i]->getName() == instanceName) {
        return _instances[i];
      }
    }
    return NULL;
  }
    

  Net* Netlist::getNet(const std::string& netName) {
    for (size_t i = 0; i < _nets.size(); ++i) {
      if (_nets[i]->getName() == netName) {
        return _nets[i];
      }
    }
    return NULL;
  }


}  // OpenChams namespace.


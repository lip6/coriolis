
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
// |  C++ Module  :  "./Net.cpp"                                     |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Port.h"
#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

using namespace std;


namespace OpenChams {


  Net::Net(const std::string& netName, const std::string& typeName, bool isExternal, Netlist* netlist) 
    : _name       (netName)
    , _typeName   (typeName)
    , _isExternal (isExternal)
    , _netlist    (netlist)
    , _connections()
    , _ports      ()
    , _wires      ()
  { }
    

  void Net::connectTo(const std::string& instanceName, const std::string& connectorName)
  {
    _connections.push_back(new Net::Connection(instanceName, connectorName));
    Instance* inst = _netlist->getInstance(instanceName);
    if (!inst) {
      string error ("[ERROR] While connecting net ");
      error += _name;
      error += " to instance ";
      error += instanceName;
      error += ": instance does not exist in netlist !";
      throw OpenChamsException(error);
    } else {
      inst->connect(connectorName, _name);
    }
  }


  Port* Net::addPort(const std::string& type, unsigned idx, double x, double y, const std::string& orient)
  {
    while (_ports.size() <= idx)
      _ports.push_back(NULL);
    if (_ports[idx])
      throw OpenChamsException("[ERROR] Net::addPort: cannot add port since another one with the same id already exists.");
    _ports[idx] = new Port(type, idx, x, y, orient);

    return _ports[idx];
  }


  Wire* Net::addWire() {
    Wire* w = new Wire();
    _wires.push_back(w);
    return w;
  }


  Net::Connection::Connection(const std::string& instanceName, const std::string& connectorName)
    : _instanceName (instanceName)
    , _connectorName(connectorName)
  { }


} // OpenChams namespace.

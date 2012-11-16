
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
// |  C++ Header  :  "./vlsisapd/openChams/Net.h"                    |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_NET_H__
#define __OPENCHAMS_NET_H__

#include <vector>
#include <string>


namespace OpenChams {

  class Netlist;
  class Port;
  class Wire;

  class Net {
    public:
      class Connection {
        public:
                                    Connection      (const std::string& instanceName, const std::string& connectorName);
          inline const std::string& getInstanceName () const;
          inline const std::string& getConnectorName() const;
        private:
          const std::string& _instanceName;
          const std::string& _connectorName;
      };

	public:
                                       Net              (const std::string& netName, const std::string& typeName, bool, Netlist*);
             void                      connectTo        (const std::string& instanceName, const std::string& connectorName);
      inline const std::string&        getName          () const;
      inline const std::string&        getType          ();
      inline bool                      isExternal       ();
      inline Netlist*                  getNetlist       ();
      inline bool                      hasNoConnections ();
      inline const std::vector<Net::Connection*>& getConnections();
    // schematic relative methods
             Port*                     addPort          (const std::string& type, unsigned idx, double x, double y, const std::string& orient);
             Wire*                     addWire          ();
      inline bool                      hasNoPorts       ();
      inline const std::vector<Port*>& getPorts         ();
      inline bool                      hasNoWires       ();
      inline const std::vector<Wire*>& getWires         ();

    private:
      const std::string&             _name;
      const std::string&             _typeName;
      bool                           _isExternal;
      Netlist*                       _netlist;
      std::vector<Net::Connection*>  _connections;  // <instanceName, connectorName>
    // schematic relative members    
      std::vector<Port*>             _ports;
      std::vector<Wire*>             _wires;
  };

    
inline const std::string&        Net::getName         () const { return _name; }
inline const std::string&        Net::getType         () { return _typeName; }
inline bool                      Net::isExternal      () { return _isExternal; }
inline Netlist*                  Net::getNetlist      () { return _netlist; }
inline bool                      Net::hasNoConnections() { return (_connections.size() == 0)? true : false; }
inline const std::vector<Net::Connection*>& Net::getConnections() { return _connections; }
inline bool                      Net::hasNoPorts      () { return (_ports.size() == 0)? true : false; }
inline const std::vector<Port*>& Net::getPorts        () { return _ports; }
inline bool                      Net::hasNoWires      () { return (_wires.size() == 0)? true : false; }
inline const std::vector<Wire*>& Net::getWires        () { return _wires; }


inline const std::string& Net::Connection::getInstanceName () const { return _instanceName; };
inline const std::string& Net::Connection::getConnectorName() const { return _connectorName; };

} // namespace
#endif


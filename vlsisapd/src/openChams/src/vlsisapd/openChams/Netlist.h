
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
// |  C++ Header  :  "./vlsisapd/openChams/Netlist.h"                |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_NETLIST_H__
#define __OPENCHAMS_NETLIST_H__

#include <vector>

namespace OpenChams {

  class Instance;
  class Device;
  class Net;
  class Circuit;


  class Netlist {
	public:
                                           Netlist        (Circuit*);
             Instance*                     addInstance    (const std::string& name, const std::string& model, unsigned);
             Device*                       addDevice      (const std::string& name, const std::string& model, unsigned, const std::string& mosType, bool);
             Net*                          addNet         (const std::string& name, const std::string& type , bool);
             Instance*                     getInstance    (const std::string&);
             Net*                          getNet         (const std::string&);
      inline bool                          hasNoInstances ();
      inline bool                          hasNoNets      ();
      inline const std::vector<Instance*>& getInstances   ();
      inline const std::vector<Net*>&      getNets        ();
      inline Circuit*                      getCircuit     ();
    private:
      Circuit*               _circuit;
      std::vector<Instance*> _instances;
      std::vector<Net*>      _nets;
  };


  inline bool                          Netlist::hasNoInstances() { return _instances.empty(); }
  inline bool                          Netlist::hasNoNets     () { return _nets.empty(); }
  inline const std::vector<Instance*>& Netlist::getInstances  () { return _instances; }
  inline const std::vector<Net*>&      Netlist::getNets       () { return _nets; }
  inline Circuit*                      Netlist::getCircuit    () { return _circuit; }


} // OpenChams namespace.

#endif


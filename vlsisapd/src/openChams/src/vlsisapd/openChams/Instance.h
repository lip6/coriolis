
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
// |  C++ Header  :  "./vlsisapd/openChams/Instance.h"               |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_INSTANCE_H__
#define __OPENCHAMS_INSTANCE_H__

#include <map>
#include <vector>
#include <string>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"


namespace OpenChams {

  class Netlist;
  class Net;

  class Instance {
    public:
                                          Instance        (Name name, Name model, unsigned, Netlist*);
      virtual                            ~Instance        ();
                                         
              void                        addConnector    (Name);
              void                        connect         (Name connectorName, Name netName);
                                         
      inline  void                        addParameter    (Name, const char* );
      inline  void                        addParameter    (Name, const std::string& );
      inline  Name                        getName         () const;
      inline  Name                        getModel        ();
      inline  unsigned                    getOrder        ();
      inline  Netlist*                    getNetlist      ();
      inline  Parameters                  getParameters   ();
      inline  bool                        hasNoConnectors ();
      inline  const std::map<Name, Net*>& getConnectors   ();

    private:
      Name                  _name;
      Name                  _model;
      unsigned              _order;
      Netlist*              _netlist;
      Parameters            _params;
      std::map<Name, Net*>  _netMap;
  };


  inline void                        Instance::addParameter    (Name name, const char* value)        { _params.addParameter(name,value); };
  inline void                        Instance::addParameter    (Name name, const std::string& value) { _params.addParameter(name,value); };
  inline Name                        Instance::getName         () const { return _name; };
  inline Name                        Instance::getModel        () { return _model; };
  inline unsigned                    Instance::getOrder        () { return _order; };
  inline Netlist*                    Instance::getNetlist      () { return _netlist; };
  inline Parameters                  Instance::getParameters   () { return _params; };
  inline bool                        Instance::hasNoConnectors () { return _netMap.empty(); };
  inline const std::map<Name, Net*>& Instance::getConnectors   () { return _netMap; };
    

}  // OpenChams namespace.

#endif



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

#include "vlsisapd/openChams/Parameters.h"


namespace OpenChams {

  class Netlist;
  class Net;

  class Instance {
    public:
                                          Instance        (const std::string& name, const std::string& model, unsigned, Netlist*);
      virtual                            ~Instance        ();
                                         
              void                        addConnector    (const std::string&);
              void                        connect         (const std::string& connectorName, const std::string& netName);
                                         
      inline  void                        addParameter    (const std::string&, const char* );
      inline  void                        addParameter    (const std::string&, const std::string& );
      inline  const std::string&          getName         () const;
      inline  const std::string&          getModel        () const;
      inline  unsigned                    getOrder        ();
      inline  Netlist*                    getNetlist      ();
      inline  Parameters                  getParameters   ();
      inline  bool                        hasNoConnectors ();
      inline  const std::map<std::string, Net*>&
                                          getConnectors   ();

    private:
      std::string                  _name;
      std::string                  _model;
      unsigned                     _order;
      Netlist*                     _netlist;
      Parameters                   _params;
      std::map<std::string, Net*>  _netMap;
  };


  inline void                               Instance::addParameter    (const std::string& name, const char* value)        { _params.addParameter(name,value); };
  inline void                               Instance::addParameter    (const std::string& name, const std::string& value) { _params.addParameter(name,value); };
  inline const std::string&                 Instance::getName         () const { return _name; };
  inline const std::string&                 Instance::getModel        () const { return _model; };
  inline unsigned                           Instance::getOrder        () { return _order; };
  inline Netlist*                           Instance::getNetlist      () { return _netlist; };
  inline Parameters                         Instance::getParameters   () { return _params; };
  inline bool                               Instance::hasNoConnectors () { return _netMap.empty(); };
  inline const std::map<std::string, Net*>& Instance::getConnectors   () { return _netMap; };
    

}  // OpenChams namespace.

#endif


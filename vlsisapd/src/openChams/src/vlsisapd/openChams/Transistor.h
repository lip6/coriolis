
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


#ifndef __OPENCHAMS_TRANSISTOR_H__
#define __OPENCHAMS_TRANSISTOR_H__

#include <map>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"


namespace OpenChams {

  class Instance;
  class Net;


  class Transistor {
    public:
                         Transistor     (Name, Instance*);
      inline void        addParameter   (Name, const char*);
      inline void        addParameter   (Name, const std::string&);
      inline Parameters  getParameters  ();
      inline void        setName        (Name);
      inline Name        getName        ();
      inline Name        getGate        ();
      inline Name        getSource      ();
      inline Name        getDrain       ();
      inline Name        getBulk        ();
             void        setGate        (Name);
             void        setSource      (Name);
             void        setDrain       (Name);
             void        setBulk        (Name);
    private:
             bool        checkConnector (Name);
    private:
      Name        _name;
      Name        _gate;     // le nom du connecteur de _instance auquel la gate est reliée
      Name        _source;   // le nom du connecteur de _instance auquel la source est reliée
      Name        _drain;    // le nom du connecteur de _instance auquel le drain est relié
      Name        _bulk;     // le nom du connecteur de _instance auquel le bulk est relié
      Instance*   _instance;
      Parameters  _params;
  };
  

  inline void        Transistor::addParameter  (Name name, const char* value)        { _params.addParameter(name,value); };
  inline void        Transistor::addParameter  (Name name, const std::string& value) { _params.addParameter(name,value); };
  inline Parameters  Transistor::getParameters () { return _params; };
  inline void        Transistor::setName       (Name name) { _name = name; };
  inline Name        Transistor::getName       () { return _name; };
  inline Name        Transistor::getGate       () { return _gate; };
  inline Name        Transistor::getSource     () { return _source; };
  inline Name        Transistor::getDrain      () { return _drain; };
  inline Name        Transistor::getBulk       () { return _bulk; };  


}  // OpenChams namespace.

#endif


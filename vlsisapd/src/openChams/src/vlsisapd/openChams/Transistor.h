
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
// |  C++ Header  :  "./vlsisapd/openChams/Transistor.h"             |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_TRANSISTOR_H__
#define __OPENCHAMS_TRANSISTOR_H__

#include <map>

#include "vlsisapd/openChams/Parameters.h"


namespace OpenChams {

  class Instance;
  class Net;


  class Transistor {
    public:
                                Transistor     (const std::string&, Instance*);
      inline void               addParameter   (const std::string&, const char*);
      inline void               addParameter   (const std::string&, const std::string&);
      inline Parameters         getParameters  ();
      inline void               setName        (const std::string&);
      inline const std::string& getName        ();
      inline const std::string& getGate        ();
      inline const std::string& getSource      ();
      inline const std::string& getDrain       ();
      inline const std::string& getBulk        ();
             void               setGate        (const std::string&);
             void               setSource      (const std::string&);
             void               setDrain       (const std::string&);
             void               setBulk        (const std::string&);
    private:                    
             bool               checkConnector (const std::string&);
    private:
      std::string  _name;
      std::string  _gate;     // le nom du connecteur de _instance auquel la gate est reliée
      std::string  _source;   // le nom du connecteur de _instance auquel la source est reliée
      std::string  _drain;    // le nom du connecteur de _instance auquel le drain est relié
      std::string  _bulk;     // le nom du connecteur de _instance auquel le bulk est relié
      Instance*    _instance;
      Parameters   _params;
  };
  

  inline void               Transistor::addParameter  (const std::string& name, const char* value)        { _params.addParameter(name,value); };
  inline void               Transistor::addParameter  (const std::string& name, const std::string& value) { _params.addParameter(name,value); };
  inline Parameters         Transistor::getParameters () { return _params; };
  inline void               Transistor::setName       (const std::string& name) { _name = name; };
  inline const std::string& Transistor::getName       () { return _name; };
  inline const std::string& Transistor::getGate       () { return _gate; };
  inline const std::string& Transistor::getSource     () { return _source; };
  inline const std::string& Transistor::getDrain      () { return _drain; };
  inline const std::string& Transistor::getBulk       () { return _bulk; };  


}  // OpenChams namespace.

#endif


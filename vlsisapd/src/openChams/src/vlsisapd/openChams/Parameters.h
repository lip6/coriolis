
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Parameters.h"             |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_PARAMETERS_H__
#define __OPENCHAMS_PARAMETERS_H__

#include <map>
#include <string>

namespace OpenChams {
  class Name;

  class Parameters {
    public:
      inline                                   Parameters   ();
      inline bool                              isEmpty      ();
             const std::string&                getValue     (Name);
      inline const std::map<Name,std::string>& getValues    ();
      inline void                              addParameter (Name, const std::string&);
             void                              addParameter (Name, const char*);
      
    private:
      std::map<Name,std::string>  _params;
  };


  inline                                   Parameters::Parameters   () { }
  inline bool                              Parameters::isEmpty      () { return (_params.size() == 0); }
  inline const std::map<Name,std::string>& Parameters::getValues    () { return _params; };
  inline void                              Parameters::addParameter (Name name, const std::string& value) { addParameter(name,value.c_str()); }


}  // OpenChams namespace.

#endif


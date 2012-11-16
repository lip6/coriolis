
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


  class Parameters {
    public:
      inline                                          Parameters   ();
      inline bool                                     isEmpty      ();
             const std::string&                       getValue     (const std::string&);
      inline const std::map<std::string,std::string>& getValues    ();
             void                                     addParameter (const std::string&, const std::string&);
      
    private:
      std::map<std::string,std::string>  _params;
  };


  inline                                          Parameters::Parameters   () { }
  inline bool                                     Parameters::isEmpty      () { return (_params.size() == 0); }
  inline const std::map<std::string,std::string>& Parameters::getValues    () { return _params; };


}  // OpenChams namespace.

#endif


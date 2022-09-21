// -*- mode: C++; explicit-buffer-name: "Configuration.h<foehn>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./foehn/Configuration.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <regex.h>
#include <string>
#include <vector>
namespace Hurricane {
  class Cell;
}


namespace Foehn {

  using  Hurricane::Record;
  using  Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Foehn::Configuration".

  class Configuration {
    public:
    // Constructor & Destructor.
                         Configuration         ();
                         Configuration         ( const Configuration& );
                        ~Configuration         ();
      Configuration*     clone                 () const;
    // Methods.                                
      bool               isDff                 ( std::string ) const;
      bool               isIgnoredNet          ( std::string ) const;
      bool               isIgnoredMasterNet    ( std::string ) const;
      void               setDffRe              ( std::string );        
      void               setIgnoredNetRe       ( std::string );        
      void               setIgnoredMasterNetRe ( std::string );        
      void               _setRegex             ( regex_t*& , std::string pattern, std::string attr );
      void               print                 ( const Cell* ) const;
      Record*            _getRecord            () const;
      std::string        _getString            () const;
      std::string        _getTypeName          () const;
    private:                                   
      Configuration&     operator=             ( const Configuration& ) = delete;
    private:
      std::string  _dffPattern;
      std::string  _ignoredNetPattern;
      std::string  _ignoredMasterNetPattern;
      regex_t*     _dffRe;
      regex_t*     _ignoredNetRe;
      regex_t*     _ignoredMasterNetRe;
  };


} // Foehn namespace.


INSPECTOR_P_SUPPORT(Foehn::Configuration);

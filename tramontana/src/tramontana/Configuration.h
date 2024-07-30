// -*- mode: C++; explicit-buffer-name: "Configuration.h<tramontana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// | =============================================================== |
// |  C++ Header  :  "./tramontana/Configuration.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}


namespace Tramontana {

  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Tramontana::Configuration".

  class Configuration {
    public:
    // Constructor & Destructor.
                              Configuration             ();
                             ~Configuration             ();
             Configuration*   clone                     () const;
    // Methods.                                         
      inline bool             doMergeSupplies           () const;
             void             print                     ( Cell* ) const;
             Record*          _getRecord                () const;
             string           _getString                () const;
             string           _getTypeName              () const;
    protected:
    // Attributes.
      bool  _mergeSupplies;
    private:
                             Configuration ( const Configuration& );
      Configuration& operator=             ( const Configuration& );
  };


  inline bool  Configuration::doMergeSupplies () const { return _mergeSupplies; }


} // Tramontana namespace.

INSPECTOR_P_SUPPORT(Tramontana::Configuration);

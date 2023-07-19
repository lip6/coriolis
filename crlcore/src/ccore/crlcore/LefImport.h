// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        C a d e n c e   L E F   I m p o r t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :i           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/LefImport.h"                    |
// +-----------------------------------------------------------------+


#ifndef  CRL_LEF_IMPORT_H
#define  CRL_LEF_IMPORT_H

#include <string>

namespace Hurricane {
  class Library;
}


namespace CRL {


  class LefImport {
    public:
      static void                reset                  ();
      static Hurricane::Library* load                   ( std::string fileName );
      static void                setMergeLibrary        ( Hurricane::Library* );
      static void                setGdsForeignDirectory ( std::string path );
  };


}  // CRL namespace.


#endif  // CRL_DEF_IMPORT_H

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


#pragma  once
#include <string>

#include <hurricane/DbU.h>
namespace Hurricane {
  class Library;
  class Layer;
}


namespace CRL {

  using Hurricane::DbU;


  class LefImport {
    public:
      static const uint32_t  PinFilter_NOFLAGS = 0;
      static const uint32_t  PinFilter_TALLEST = (1<<0);
      static const uint32_t  PinFilter_WIDEST  = (1<<1);
      static const uint32_t  PinFilter_LARGEST = (1<<2);
    public:
      static void                reset                  ();
      static Hurricane::Library* load                   ( std::string fileName );
      static void                setMergeLibrary        ( Hurricane::Library* );
      static void                setGdsForeignDirectory ( std::string path );
      static void                setPinFilter           ( DbU::Unit xThreshold, DbU::Unit yThreshold, uint32_t flags );
      static Hurricane::Layer*   getLayer               ( std::string name);
      static void                addLayer               ( std::string name, Hurricane::Layer* layer );
      static void                clearLayer             ( std::string name );
  };


}  // CRL namespace.

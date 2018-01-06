// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./LefDefExtension.h"                      |
// +-----------------------------------------------------------------+


#ifndef  CRL_LEFDEF_EXTENSION_H
#define  CRL_LEFDEF_EXTENSION_H

#include <list>
#include "hurricane/Box.h"
#include "hurricane/Property.h"

namespace Hurricane {
  class Cell;
}


namespace CRL {


  using std::list;

  using Hurricane::Box;
  using Hurricane::Name;
  using Hurricane::StandardPrivateProperty;
  using Hurricane::Cell;


  typedef list<Name> PinIocOrder;


// -------------------------------------------------------------------
// Class  :  "CRL::LefDefExtensionDatas".


  class LefDefExtensionDatas {
    public:
                    LefDefExtensionDatas ();
    public:
      unsigned int  _lefDbuPerMicron;
      unsigned int  _defDbuPerMicron;
      Box           _rowsBox;
      PinIocOrder   _northOrder;
      PinIocOrder   _southOrder;
      PinIocOrder   _eastOrder;
      PinIocOrder   _westOrder;
      PinIocOrder   _undefined;
  };


// -------------------------------------------------------------------
// Class  :  "CRL::LefDefExtension".


  class LefDefExtension {
    public:
      typedef StandardPrivateProperty<LefDefExtensionDatas> Extension;
    public:
      static Box           getRowsBox          ( const Cell* );
      static unsigned int  getLefDbuPerMicron  ( const Cell* );
      static unsigned int  getDefDbuPerMicron  ( const Cell* );
      static const Box&    addRowsBox          ( Cell*, Box );
      static void          setLefDbuPerMicron  ( Cell*, unsigned int  );
      static void          setDefDbuPerMicron  ( Cell*, unsigned int  );
      static PinIocOrder&  getNorthPinIocOrder ( Cell*, bool create=false );
      static PinIocOrder&  getSouthPinIocOrder ( Cell*, bool create=false );
      static PinIocOrder&  getEastPinIocOrder  ( Cell*, bool create=false );
      static PinIocOrder&  getWestPinIocOrder  ( Cell*, bool create=false );
      static PinIocOrder&  getUndefinedPinIoc  ( Cell*, bool create=false );
  };


} // CRL namespace.

#endif  // CRL_LEFDEF_EXTENSION_H

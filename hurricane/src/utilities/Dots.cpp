// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2013-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Dots.cpp"                                    |
// +-----------------------------------------------------------------+


#include "hurricane/utilities/Dots.h"


namespace Utilities {


  void  Dots::dot ()
  {
    ++_count;
    if (_count % _divider == 0) {
      unsigned int position = _count / _divider;
      if (position == 1) {
        if (not (_flags & FirstDot))
          CR();
        else
          _flags &= ~FirstDot;
        if (enabled())
          _ostream << _indent;
      }

      _flush( '.' );

      if (position + _indent.size() >= _width) {
        reset( NoFlags );
      }
    }
  }


  void  Dots::finish ( unsigned int flags )
  {
    CR();

    if (flags & Reset) reset( flags );
  }


}  // Utilities namespace.

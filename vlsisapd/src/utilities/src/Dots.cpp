// -*- C++ -*-
//
// This file is part of the VLSI Stand-Alone Software.
// Copyright (c) UPMC 2013-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |    M i s c e l l a n e o u s   U t i l i t i e s                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Dots.cpp"                                    |
// +-----------------------------------------------------------------+


#include  "vlsisapd/utilities/Dots.h"


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

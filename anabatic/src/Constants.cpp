// -*- mode: C++; explicit-buffer-name: "Constants.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Constants.cpp"                               |
// +-----------------------------------------------------------------+


#include <string>
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;


  Flags::~Flags ()
  { }


  string  Flags::_getTypeName () const
  { return "Anabatic::Flags"; }


  string Flags::_getString () const
  {
    string s = "";
    s += (_flags & Horizontal  ) ? 'h' : '-';
    s += (_flags & Vertical    ) ? 'v' : '-';
    s += (_flags & Source      ) ? 'S' : '-';
    s += (_flags & Target      ) ? 'T' : '-';
    s += (_flags & DeviceGCell ) ? 'd' : '-';
    s += (_flags & ChannelGCell) ? 'c' : '-';
    s += (_flags & StrutGCell  ) ? 's' : '-';
    s += (_flags & MatrixGCell ) ? 'm' : '-';
    s += ",";
    s += (_flags & Invalidated ) ? 'i' : '-';
    s += (_flags & DestroyGCell) ? 'D' : '-';
    s += (_flags & AboveLayer  ) ? 'A' : '-';
    s += (_flags & BelowLayer  ) ? 'B' : '-';

    return s;
  }


}  // Anabatic namespace.

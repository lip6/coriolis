// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BaseFlags.cpp"                               |
// +-----------------------------------------------------------------+


#include <climits>
#include <iomanip>
#include "hurricane/Flags.h"


namespace Hurricane {


  BaseFlags::~BaseFlags ()
  { }


  string  BaseFlags::asString ( uint32_t ) const
  {
    std::ostringstream formatted;
    formatted << _flags;
    return formatted.str();
  }


  string  BaseFlags::_getTypeName () const
  { return "Flags"; }


  string  BaseFlags::_getString () const
  {
    static size_t  width = sizeof(uint64_t) * CHAR_BIT;

    std::ostringstream formatted;
    formatted << "0x" << std::hex << std::setw(width) << std::setfill('0') << _flags;
    return formatted.str();
  }



}  // Hurricane namespace.

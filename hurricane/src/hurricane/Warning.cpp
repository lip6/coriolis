
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
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Warning.cpp"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cstdarg>

# include  "hurricane/Warning.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Warning".


  Warning::Warning ( const string& reason )
    : Exception()
    , _reason(reason)
    , _code(0)
  { }


  Warning::Warning ( int code, const string& reason )
    : Exception()
    , _reason(reason)
    , _code(code)
  { }


  Warning::Warning ( const char* format, ... )
    : Exception()
    , _reason()
    , _code(0)
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Warning::Warning ( int code, const char* format, ... )
    : Exception()
    , _reason()
    , _code(code)
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Warning::Warning ( const Warning& warning )
    : Exception()
    , _reason(warning._reason)
    , _code(warning._code)
  { }


  Warning& Warning::operator= ( const Warning& warning )
  {
    _reason = warning._reason;
    _code   = warning._code;
    return *this;
  }


  string  Warning::_getTypeName () const
  { return _TName("Warning"); }


  string Warning::_getString () const
  {
    if (!_code)
      return "[WARNING] " + _reason;

    return "[WARNING:" + getString(_code) + "] " + _reason;
  }


} // End of Hurricane namespace.


// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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
// |  C++ Module  :  "./Error.cpp"                                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <execinfo.h>
#include  <cstdarg>
#include  <iomanip>
#include  <boost/regex.hpp>
#include  "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Error".


  Error::Error ( const string& reason )
    : Exception ()
    , _reason   (reason)
    , _code     (0)
    , _backtrace()
  { }


  Error::Error ( int code, const string& reason )
    : Exception ()
    , _reason   (reason)
    , _code     (code)
    , _backtrace()
  { }


  Error::Error ( const char* format, ... )
    : Exception ()
    , _reason   ()
    , _code     (0)
    , _backtrace()
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Error::Error ( int code, const char* format, ... )
    : Exception ()
    , _reason   ()
    , _code     (code)
    , _backtrace()
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Error::Error ( const Error& error )
    : Exception ()
    , _reason   (error._reason)
    , _code     (error._code)
    , _backtrace(error._backtrace)
  { }


  string  Error::_getTypeName () const
  { return _TName("Error"); }


  string Error::_getString () const
  {
    if ( !_code )
      return "[ERROR] " + _reason;
    
    return "[ERROR:" + getString(_code) + "] " + _reason;
  }

} // End of Hurricane namespace.

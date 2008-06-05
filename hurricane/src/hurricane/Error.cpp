
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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
// |  C++ Module  :       "./Error.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cstdarg>

# include  "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Error".


  Error::Error ( const string& reason )
    : Exception()
    , _reason(reason)
    , _code(0)
  {
  }


  Error::Error ( int code, const string& reason )
    : Exception()
    , _reason(reason)
    , _code(code)
  {
  }


  Error::Error ( const char* format, ... )
    : Exception()
    , _reason("")
    , _code(0)
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Error::Error ( int code, const char* format, ... )
    : Exception()
    , _reason("")
    , _code(code)
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _reason = formatted;
  }


  Error::Error ( const Error& error )
    : Exception()
    , _reason(error._reason)
    , _code(error._code)
  {
  }


  string  Error::_getTypeName () const
  { return _TName("Error"); }


  string Error::_getString() const
  {
    if ( !_code )
      return "[ERROR] " + _reason;
    
    return "[ERROR:" + getString(_code) + "] " + _reason;
  }


} // End of Hurricane namespace.

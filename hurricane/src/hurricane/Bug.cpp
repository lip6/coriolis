
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
// |  C++ Module  :       "./Bug.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cstdarg>

# include  "hurricane/Bug.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Bug".


  Bug::Bug ( const string& reason )
    : Exception()
    , _reason(reason)
    , _code(0)
  { }


  Bug::Bug ( int code, const string& reason )
    : Exception()
    , _reason(reason)
    , _code(code)
  { }


  Bug::Bug ( const char* format, ... )
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


  Bug::Bug ( int code, const char* format, ... )
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


  Bug::Bug ( const Bug& error )
    : Exception()
    , _reason(error._reason)
    , _code(error._code)
  {
  }


  string  Bug::_getTypeName () const
  { return _TName("Bug"); }


  string Bug::_getString () const
  {
    if ( !_code )
      return "[BUG] " + _reason;
    
    return "[BUG:" + getString(_code) + "] " + _reason;
  }


} // End of Hurricane namespace.

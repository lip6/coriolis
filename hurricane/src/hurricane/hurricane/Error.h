
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
// |  C++ Header  :       "./Error.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_ERROR__
# define  __HURRICANE_ERROR__

# include  "hurricane/Exception.h"


namespace Hurricane {


  class Error : public Exception {

    public:
    // Constructors.
                      Error        ( const string& reason );
                      Error        ( const char*   format, ... );
                      Error        ( int code, const string& reason );
                      Error        ( int code, const char*   format, ... );
                      Error        ( const Error& error );
    // Methods.
      inline  string  getReason    () const;
      inline  int     getCode      () const;
    // Hurricane Managment.
      virtual string  _getTypeName () const;
      virtual string  _getString   () const;

    private:
    // Internal: Attributes.
              string  _reason;
              int     _code;
  };


// Inline Functions.
  inline  string  Error::getReason () const { return _reason; }
  inline  int     Error::getCode   () const { return _code; }


} // End of Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Error);
GETSTRING_VALUE_SUPPORT(Hurricane::Error);
IOSTREAM_POINTER_SUPPORT(Hurricane::Error);
IOSTREAM_VALUE_SUPPORT(Hurricane::Error);


# endif // __HURRICANE_ERROR__

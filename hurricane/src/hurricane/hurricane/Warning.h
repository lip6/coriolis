
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
// |  C++ Header  :       "./Warning.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_WARNING__
# define  __HURRICANE_WARNING__

#include "hurricane/Exception.h"


namespace Hurricane {


  class Warning : public Exception {

    public:
    // Constructors.
                       Warning      ( const string& reason );
                       Warning      ( const char*   format, ... );
                       Warning      ( int code, const string& reason );
                       Warning      ( int code, const char*   format, ... );
                       Warning      ( const Warning& warning );
              Warning& operator=    ( const Warning& warning );
    // Methods.
      inline  string   getReason    () const;
      inline  int      getCode      () const;
    // Hurricane Managment.
      virtual string   _getTypeName () const;
      virtual string   _getString   () const;

    protected:
    // Internal: Attributes.
              string   _reason;
              int      _code;
  };


// Inline Functions.
  inline  string  Warning::getReason () const { return _reason; }
  inline  int     Warning::getCode   () const { return _code; }


} // End of Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Warning);
GETSTRING_VALUE_SUPPORT(Hurricane::Warning);
IOSTREAM_POINTER_SUPPORT(Hurricane::Warning);
IOSTREAM_VALUE_SUPPORT(Hurricane::Warning);


# endif // __HURRICANE_WARNING__

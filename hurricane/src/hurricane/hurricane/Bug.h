
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
// |  C++ Header  :       "./Bug.h"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_BUG__
# define  __HURRICANE_BUG__

# include  "hurricane/Exception.h"


namespace Hurricane {


  class Bug : public Exception {

    public:
    // Constructors.
                      Bug          ( const string& reason );
                      Bug          ( const char*   format, ... );
                      Bug          ( int code, const string& reason );
                      Bug          ( int code, const char*   format, ... );
                      Bug          ( const Bug& error );
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
  inline  string  Bug::getReason () const { return _reason; }
  inline  int     Bug::getCode   () const { return _code; }


} // End of Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Bug);
GETSTRING_VALUE_SUPPORT(Hurricane::Bug);
IOSTREAM_POINTER_SUPPORT(Hurricane::Bug);
IOSTREAM_VALUE_SUPPORT(Hurricane::Bug);


# endif // __HURRICANE_BUG__

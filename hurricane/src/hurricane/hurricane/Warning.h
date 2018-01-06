
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Warning.h"                         |
// +-----------------------------------------------------------------+


# ifndef  HURRICANE_WARNING_H
# define  HURRICANE_WARNING_H

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


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Warning);
GETSTRING_VALUE_SUPPORT(Hurricane::Warning);
IOSTREAM_POINTER_SUPPORT(Hurricane::Warning);
IOSTREAM_VALUE_SUPPORT(Hurricane::Warning);


#endif // HURRICANE_WARNING_H

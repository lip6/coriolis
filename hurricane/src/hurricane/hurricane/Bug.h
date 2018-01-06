
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
// |  C++ Header  :  "./hurricane/Bug.h"                             |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_BUG_H
#define  HURRICANE_BUG_H

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


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Bug);
GETSTRING_VALUE_SUPPORT(Hurricane::Bug);
IOSTREAM_POINTER_SUPPORT(Hurricane::Bug);
IOSTREAM_VALUE_SUPPORT(Hurricane::Bug);


#endif // HURRICANE_BUG_H

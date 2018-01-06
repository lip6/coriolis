
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
// |  C++ Header  :  "./hurricane/Error.h"                           |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_ERROR_H
#define  HURRICANE_ERROR_H

#include  "hurricane/Exception.h"
#include  "hurricane/Backtrace.h"


namespace Hurricane {


  class Error : public Exception {
    public:
                                    Error         ( const string& reason );
                                    Error         ( const char*   format, ... );
                                    Error         ( int code, const string& reason );
                                    Error         ( int code, const char*   format, ... );
                                    Error         ( const Error& error );
      inline  string                getReason     () const;
      inline  int                   getCode       () const;
      inline  string                where         () const;
      inline  string                textWhere     () const;
      inline  string                htmlWhere     () const;
      virtual string                _getTypeName  () const;
      virtual string                _getString    () const;
    private:
      string               _reason;
      int                  _code;
      Backtrace            _backtrace;
  };


// Inline Functions.
  inline  string                Error::getReason () const { return _reason; }
  inline  int                   Error::getCode   () const { return _code; }
  inline  string                Error::where     () const { return _backtrace.where(); }
  inline  string                Error::textWhere () const { return _backtrace.textWhere(); }
  inline  string                Error::htmlWhere () const { return _backtrace.htmlWhere(); }


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::Error);
GETSTRING_VALUE_SUPPORT(Hurricane::Error);
IOSTREAM_POINTER_SUPPORT(Hurricane::Error);
IOSTREAM_VALUE_SUPPORT(Hurricane::Error);


#endif // HURRICANE_ERROR_H

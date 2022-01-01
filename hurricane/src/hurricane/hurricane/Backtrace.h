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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Backtrace.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/Commons.h"
#include "hurricane/TextTranslator.h"


namespace Hurricane {


  class Backtrace {
    public:
                          Backtrace    ( bool enabled );
                         ~Backtrace    ();
      inline std::string  where        () const;
      inline std::string  textWhere    () const;
             std::string  htmlWhere    () const;
      inline std::string  _getTypeName () const;
      inline std::string  _getString   () const;
    private:
      static bool                      _inConstructor;
      static TextTranslator            _textTranslator;
      static const size_t              _stackSize;
             std::vector<std::string>  _stack;
  };


// Inline Functions.
  inline std::string  Backtrace::where        () const { return textWhere(); }
  inline std::string  Backtrace::textWhere    () const { return _textTranslator.translate(htmlWhere()); }
  inline std::string  Backtrace::_getTypeName () const { return "Backtrace"; }
  inline std::string  Backtrace::_getString   () const { return "<Backtrace>"; }
  

} // Hurricane namespace.

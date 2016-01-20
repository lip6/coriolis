// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2016, All Rights Reserved
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
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Initializer.cpp"                             |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Initializer.h"


namespace Hurricane {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "Init".

  bool                  Init::_run          = false;
  Init::InitializerMap* Init::_initializers = NULL;


  void  Init::destroy ()
  {
    if (_initializers) delete _initializers;
    _initializers = NULL;
    _run          = false;
  }


  Init::InitializerMap& Init::initializers ()
  {
    if (not _initializers) _initializers = new InitializerMap();
    return *_initializers;
  }


  void Init::addInitializer ( unsigned int order,initializer_t initFunction )
  { initializers().insert( make_pair(order,initFunction) ); }


  bool  Init::runOnce ()
  {
    if (_run) return false;

    for ( auto init : initializers() ) init.second();
    _run = true;

    return true;
  }


}  // Hurricane namespace.

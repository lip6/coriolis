// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2018, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/Initializer.h"                     |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_INITIALIZER_H
#define HURRICANE_INITIALIZER_H

#include <functional>
#include <map>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Init".

  class Init {
    public:
      typedef std::function<void(void)>                  initializer_t; 
      typedef std::multimap<unsigned int,initializer_t>  InitializerMap;
    public:
      static void            destroy        ();
      static InitializerMap& initializers   ();
      static void            addInitializer ( unsigned int order, initializer_t );
      static bool            runOnce        ();
    private:
      static bool            _run;
      static InitializerMap* _initializers;
  };


// -------------------------------------------------------------------
// Class  :  "Initializer".

  template<typename T>
  class Initializer {
    public:
      inline  Initializer ( unsigned int order ) { Init::addInitializer( order, T::initialize ); }
  };


}  // Hurricane namespace.

#endif  // HURRICANE_INITIALIZER_H

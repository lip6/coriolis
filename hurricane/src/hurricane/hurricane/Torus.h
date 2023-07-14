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
// |  C++ Header  :  "./hurricane/Torus.h"                           |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_TORUS_H
#define  HURRICANE_TORUS_H

#include "hurricane/Box.h"


namespace Hurricane {


  class Torus {
    public:
                          Torus          ();
                          Torus          ( const Box& outer, const Box& inner );
      inline bool         isEmpty        () const;
      inline const Box&   getBoundingBox () const;
      inline const Box&   getOuterBox    () const;
      inline const Box&   getInnerBox    () const;
             bool         contains       ( const Box& ) const;
      inline std::string  _getTypeName   () const;
             std::string  _getString     () const;
             Record*      _getRecord     () const;
    private:
      Box  _outerBox;
      Box  _innerBox;
  };


// Inline Functions.
  inline bool         Torus::isEmpty        () const { return _outerBox.isEmpty(); }
  inline const Box&   Torus::getBoundingBox () const { return getOuterBox(); }
  inline const Box&   Torus::getOuterBox    () const { return _outerBox; }
  inline const Box&   Torus::getInnerBox    () const { return _innerBox; }
  inline std::string  Torus::_getTypeName   () const { return "Torus"; }


} // End of Hurricane namespace.


INSPECTOR_PR_SUPPORT(Hurricane::Torus);

#endif  // HURRICANE_TORUS_H

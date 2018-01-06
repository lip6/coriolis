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
// |  C++ Header  :  "./hurricane/ExtensionGo.h"                     |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_EXTENSION_GO_H
#define  HURRICANE_EXTENSION_GO_H

#include  "hurricane/Name.h"
#include  "hurricane/Go.h"
#include  "hurricane/ExtensionGos.h"


namespace Hurricane {


  class ExtensionGo : public Go {

    public:
    // Methods.
      virtual void        materialize   ();
      virtual void        unmaterialize ();
      virtual const Name& getName       () const = 0;
      virtual Cell*       getCell       () const;
    // Hurricane Managment.
      virtual string      _getTypeName  () const;
      virtual string      _getString    () const;
      virtual Record*     _getRecord    () const;

    protected:
    // Internal: Attributes.
              Cell*       _cell;

    protected:
    // Internal: Constructor & destructors.
                          ExtensionGo   ( Cell* );
      virtual void        _preDestroy   ();
  };


} // End of Hurricane namespace.


#endif // HURRICANE_EXTENSION_GO_H



// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2014, All Rights Reserved
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DeepNet.h"                         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_DEEPNET_H
#define  HURRICANE_DEEPNET_H

#include "hurricane/Net.h"
#include "hurricane/HyperNet.h"
#include "hurricane/Occurrence.h"


namespace Hurricane {

  class DeepNet : public Net {

    // Attributes.
    protected:
      Occurrence  _netOccurrence;

    // Constructors.
    protected:
      DeepNet ( Occurrence& netOccurrence );

    // Inspector Management.
    public:
      virtual Record* _getRecord  () const;
      virtual string  _getTypeName() const { return "DeepNet"; };

    // Constructors.
    public:
      static  DeepNet* create      ( HyperNet& hyperNet );

    // Accessors.
    public:

    // Predicates.
    public:
      virtual bool isDeepNet () const { return true; };

    // Internal Modifiers.
    public:
      size_t  _createRoutingPads ( unsigned int flags=0 );

};

Net* getDeepNet(HyperNet& hyperNet);


} // Hurricane namespace.

#endif

// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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


// -------------------------------------------------------------------
// Class  :  "DeepNet".

  class DeepNet : public Net {
    public:
      typedef  Net  Inherit;
    public:
      static  DeepNet*    create               ( HyperNet& hyperNet );
      inline  Occurrence  getRootNetOccurrence () const;
      virtual bool        isDeepNet            () const { return true; };
              size_t      _createRoutingPads   ( unsigned int flags=0 );
      virtual Record*     _getRecord           () const;
      virtual string      _getTypeName         () const { return "DeepNet"; };
      virtual void        _toJson              ( JsonWriter* ) const;
    protected:
                          DeepNet              ( Occurrence& netOccurrence );
    protected:
      Occurrence  _netOccurrence;

  };


  inline Occurrence  DeepNet::getRootNetOccurrence() const { return _netOccurrence; }

  Net* getDeepNet(HyperNet& hyperNet);


// -------------------------------------------------------------------
// Class  :  "JsonDeepNet".

  class JsonDeepNet : public JsonNet {
    public:
                           JsonDeepNet ( unsigned long flags );
      virtual             ~JsonDeepNet ();
      virtual string       getTypeName () const;
      virtual JsonDeepNet* clone       ( unsigned long ) const;
      virtual void         toData      ( JsonStack&); 
  };


} // Hurricane namespace.

#endif

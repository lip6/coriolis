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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DeepNet.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Property.h"
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
      class Uplink : public Hurricane::PrivateProperty {
          static  Name         _name;
        public:
          static  Uplink*      create          ( DeepNet* uplink );
          static  Name         getPropertyName (); 
          virtual Name         getName         () const;
          inline  DeepNet*     getUplink       ();
          virtual void         onReleasedBy    ( DBo* owner );
          virtual std::string  _getTypeName    () const;
          virtual std::string  _getString      () const;
          virtual Record*      _getRecord      () const;
        protected:
          DeepNet* _uplink;
        protected:
          inline  Uplink ( DeepNet* uplink );
      };  
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
      virtual void        _preDestroy          ();
    protected:
      Occurrence  _netOccurrence;

  };


  inline Occurrence  DeepNet::getRootNetOccurrence() const { return _netOccurrence; }

  Net* getDeepNet(HyperNet& hyperNet);


  inline  DeepNet::Uplink::Uplink ( DeepNet* uplink )
    : PrivateProperty(), _uplink(uplink)
  { } 

  inline DeepNet* DeepNet::Uplink::getUplink () { return _uplink; }



// -------------------------------------------------------------------
// Class  :  "JsonDeepNet".

  class JsonDeepNet : public JsonNet {
    public:
      static  void         initialize  ();
                           JsonDeepNet ( unsigned long flags );
      virtual             ~JsonDeepNet ();
      virtual string       getTypeName () const;
      virtual JsonDeepNet* clone       ( unsigned long ) const;
      virtual void         toData      ( JsonStack&); 
  };


} // Hurricane namespace.

INSPECTOR_P_SUPPORT(Hurricane::DeepNet);

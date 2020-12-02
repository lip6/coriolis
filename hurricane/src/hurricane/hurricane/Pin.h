// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2020, All Rights Reserved
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
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Commons.h"                         |
// +-----------------------------------------------------------------+

#pragma  once
#include "hurricane/Contact.h"
#include "hurricane/Pins.h"

namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Pin".

  class Pin : public Contact {
    public:
      typedef Contact Inherit;

    public:
      class AccessDirection {
        public:
          enum Code { UNDEFINED=0, NORTH=1, SOUTH=2, EAST=3, WEST=4 };
        public:
                           AccessDirection ( Code code=UNDEFINED );
                           AccessDirection ( const AccessDirection& accessDirection );
          AccessDirection& operator=       ( const AccessDirection& accessDirection );
                           operator Code   () const { return _code; };
          Code             getCode         () const { return _code; };
          std::string      _getTypeName    () const { return _TName("Pin::AccessDirection"); };
          std::string      _getString      () const;
          Record*          _getRecord      () const;
        private:
          Code _code;
    };

    public:
      class PlacementStatus {
        public:
          enum Code { UNPLACED=0, PLACED=1, FIXED=2 };
        public:
                           PlacementStatus ( Code code=UNPLACED);
                           PlacementStatus ( const PlacementStatus& placementstatus );
          PlacementStatus& operator=       ( const PlacementStatus& placementstatus );
                           operator Code   () const { return _code; };
          Code             getCode         () const { return _code; };
          std::string      _getTypeName    () const { return _TName("Pin::PlacementStatus"); };
          std::string      _getString      () const;
          Record*          _getRecord      () const;
        private:
          Code _code;
      };

    protected:
      Pin (       Net*
          , const Name&
          , const AccessDirection&
          , const PlacementStatus&
          , const Layer*
          ,       DbU::Unit x
          ,       DbU::Unit y
          ,       DbU::Unit width
          ,       DbU::Unit height
          );
    public:
      static Pin* create (       Net*
                         , const Name&
                         , const AccessDirection&
                         , const PlacementStatus&
                         , const Layer*
                         ,       DbU::Unit x
                         ,       DbU::Unit y
                         ,       DbU::Unit width =0
                         ,       DbU::Unit height=0
                         );
    public:
      const   Name&            getName              () const { return _name; };
      const   AccessDirection& getAccessDirection   () const { return _accessDirection; };
      const   PlacementStatus& getPlacementStatus   () const { return _placementStatus; };
              bool             isUnplaced           () const { return _placementStatus == PlacementStatus::UNPLACED; };
              bool             isPlaced             () const { return _placementStatus == PlacementStatus::PLACED; };
              bool             isFixed              () const { return _placementStatus == PlacementStatus::FIXED; };
              void             setPlacementStatus   ( const PlacementStatus& );
    protected:
      virtual void             _postCreate          ();
      virtual void             _preDestroy          ();
    private:                     
              bool             _postCheck           ();
    public:                    
      virtual std::string      _getTypeName         () const {return _TName("Pin");};
      virtual std::string      _getString           () const;
      virtual Record*          _getRecord           () const;
              Pin*             _getNextOfCellPinMap () const { return _nextOfCellPinMap; };
              void             _setNextOfCellPinMap ( Pin* pin ) { _nextOfCellPinMap = pin; };
    private:
      Name             _name;
      AccessDirection  _accessDirection;
      PlacementStatus  _placementStatus;
      Pin*             _nextOfCellPinMap;
  };


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Pin);
INSPECTOR_PR_SUPPORT(Hurricane::Pin::AccessDirection);
INSPECTOR_PR_SUPPORT(Hurricane::Pin::PlacementStatus);

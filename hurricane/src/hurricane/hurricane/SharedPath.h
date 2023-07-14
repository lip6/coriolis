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
// |  Authors     :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/RoutingPad.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Instances.h"
#include "hurricane/SharedPathes.h"
#include "hurricane/Quark.h"
#include "hurricane/Transformation.h"
#include "hurricane/IntrusiveMap.h"

namespace Hurricane {

  class Cell;
  class Entity;


// -------------------------------------------------------------------
// Class  :  "SharedPath".


  class SharedPath {
    public:
      class QuarkMap : public IntrusiveMap<const Entity*, Quark> {
        public:
          typedef IntrusiveMap<const Entity*, Quark> Inherit;
        public:
                                QuarkMap        ();
          virtual const Entity* _getKey         ( Quark* ) const;
          virtual unsigned      _getHashValue   ( const Entity* ) const;
          virtual Quark*        _getNextElement ( Quark* ) const;
          virtual void          _setNextElement ( Quark* , Quark* nextQuark ) const;
    };

    public:
                   SharedPath ( Instance* headInstance, SharedPath* tailSharedPath = NULL );
                  ~SharedPath ();
    private:
                   SharedPath ( const SharedPath& ) = delete;
       SharedPath& operator=  ( const SharedPath& ) = delete;
    public:
      static char getNameSeparator ();
      static void setNameSeparator ( char nameSeparator );
    public:
             unsigned long  getHash           () const;
      inline Instance*      getHeadInstance   () const;
      inline SharedPath*    getTailSharedPath () const;
             SharedPath*    getHeadSharedPath () const;
             Instance*      getTailInstance   () const;
             std::string    getName           () const;
             std::string    getJsonString     ( unsigned long flags ) const;
             Cell*          getOwnerCell      () const;
             Cell*          getMasterCell     () const;
             Instances      getInstances      () const;
             Transformation getTransformation ( const Transformation& transformation=Transformation() ) const;
    public:
             std::string    _getTypeName                    () const;
             std::string    _getString                      () const;
             Record*        _getRecord                      () const;
      inline Quark*         _getQuark                       (const Entity* entity ) const;
      inline Quarks         _getQuarks                      () const;
      inline QuarkMap&      _getQuarkMap                    ();
      inline SharedPath*    _getNextOfInstanceSharedPathMap () const;
      inline void           _setNextOfInstanceSharedPathMap ( SharedPath* sharedPath );
    private:
    // Attributes.
      unsigned long  _hash;
      Instance*      _headInstance;
      SharedPath*    _tailSharedPath;
      QuarkMap       _quarkMap;
      SharedPath*    _nextOfInstanceSharedPathMap;
  };

  
  inline Instance*             SharedPath::getHeadInstance                 () const { return _headInstance; }
  inline SharedPath*           SharedPath::getTailSharedPath               () const { return _tailSharedPath; }
  inline Quark*                SharedPath::_getQuark                       (const Entity* entity ) const { return _quarkMap.getElement(entity); }
  inline Quarks                SharedPath::_getQuarks                      () const { return _quarkMap.getElements(); }
  inline SharedPath::QuarkMap& SharedPath::_getQuarkMap                    () { return _quarkMap; }
  inline SharedPath*           SharedPath::_getNextOfInstanceSharedPathMap () const { return _nextOfInstanceSharedPathMap; }
  inline void                  SharedPath::_setNextOfInstanceSharedPathMap ( SharedPath* sharedPath ) { _nextOfInstanceSharedPathMap = sharedPath; }


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::SharedPath);

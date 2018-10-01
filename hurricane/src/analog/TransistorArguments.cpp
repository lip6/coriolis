// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./TransistorArguments.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/analog/TransistorArguments.h"

namespace Analog {
  

  TransistorArguments::TransistorArguments ( bool isNMOS
                                           , bool isBulkConnected
                                           , bool isSourceFirst
                                           , std::string bulkType
                                           , Hurricane::DbU::Unit w
                                           , Hurricane::DbU::Unit l
                                           , unsigned m
                                           , unsigned externalDummy )
    : Super()
    , _isNMOS         (isNMOS)
    , _isBulkConnected(isBulkConnected) 
    , _isSourceFirst  (isSourceFirst)
    , _bulkType       (bulkType)
    , _w              (w)
    , _l              (l)
    , _m              (m)
    , _externalDummy  (externalDummy)
  { }

  
  TransistorArguments::TransistorArguments ( const TransistorArguments& tArgs )
    : Super()
    , _isNMOS         (tArgs.isNMOS())
    , _isBulkConnected(tArgs.isBulkConnected())
    , _isSourceFirst  (tArgs.isSourceFirst())
    , _bulkType       (tArgs.getBulkType())
    , _w              (tArgs.getW())
    , _l              (tArgs.getL())
    , _m              (tArgs.getM())
    , _externalDummy  (tArgs.getExternalDummy())
  { }

  
  TransistorArguments* TransistorArguments::getClone ()
  { return new TransistorArguments( *this ); }


}  // Analog namespace.

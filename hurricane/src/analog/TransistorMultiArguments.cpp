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
// |  C++ Module  :  "./TransistorMultiArguments.cpp"                |
// +-----------------------------------------------------------------+


#include "hurricane/analog/TransistorMultiArguments.h"

namespace Analog {


  TransistorMultiArguments::TransistorMultiArguments ( bool                 isNMOS
                                                     , bool                 isBulkConnected
                                                     , bool                 isSourceFirst
                                                     , std::string          bulkType
                                                     , Hurricane::DbU::Unit w
                                                     , Hurricane::DbU::Unit l
                                                     , unsigned             m
                                                     , unsigned             mint
                                                     , unsigned             externalDummy )
  : Super(isNMOS, isBulkConnected, isSourceFirst, bulkType, w, l, m, externalDummy)
  , _mint(mint)
  { }
  

  TransistorMultiArguments::TransistorMultiArguments ( const TransistorMultiArguments& dpArgs )
    : Super( dpArgs.isNMOS()
           , dpArgs.isBulkConnected()
           , dpArgs.isSourceFirst()
           , dpArgs.getBulkType()
           , dpArgs.getW()
           , dpArgs.getL()
           , dpArgs.getM()
           , dpArgs.getExternalDummy())
    , _mint(dpArgs.getMInt())
  { }


  TransistorMultiArguments* TransistorMultiArguments::getClone ()
  { return new TransistorMultiArguments( *this ); }


}  // Analog namespace.

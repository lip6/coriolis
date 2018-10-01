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
// |  C++ Header  :  "./hurricane/analog/TransistorMultiArguments.h" |
// +-----------------------------------------------------------------+


#ifndef ANALOG_TRANSISTOR_MULTI_ARGUMENTS_H
#define ANALOG_TRANSISTOR_MULTI_ARGUMENTS_H

#include "hurricane/DbU.h"
#include "hurricane/analog/TransistorArguments.h"

namespace Analog {


  class TransistorMultiArguments : public TransistorArguments {
    public:
      typedef TransistorArguments Super;
    public:
                                        TransistorMultiArguments ( bool                 isNMOS
                                                                 , bool                 isBulkConnected
                                                                 , bool                 isSourceFirst
                                                                 , std::string          bulkType
                                                                 , Hurricane::DbU::Unit w
                                                                 , Hurricane::DbU::Unit l
                                                                 , unsigned int         m
                                                                 , unsigned int         mint
                                                                 , unsigned int         externalDummy );
                                        TransistorMultiArguments ( const TransistorMultiArguments& );
      virtual TransistorMultiArguments* getClone                 ();
      inline  unsigned int              getMInt                  () const;
      inline  void                      setMInt                  ( Hurricane::DbU::Unit mint);
    private:
      unsigned    _mint;
  };


  inline unsigned int  TransistorMultiArguments::getMInt () const { return _mint; };
  inline void          TransistorMultiArguments::setMInt ( Hurricane::DbU::Unit mint) { _mint = mint; };


}  // Analog namespace.

#endif // ANALOG_TRANSISTOR_MULTI_ARGUMENTS_H

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/TransistorPair.h"           |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/analog/TransistorFamily.h"

namespace Analog {


  class TransistorPair : public TransistorFamily {
    public:
      typedef TransistorFamily Super;
    public:
      inline  long       getMint        () const;
      inline  void       setMint        ( long );
      virtual Record*    _getRecord     () const;
    protected:
                         TransistorPair ( Hurricane::Library*, const Hurricane::Name&, const Type& );
      virtual void       _postCreate    ( const Hurricane::Name& deviceName );
    protected:
      MetaTransistor*   _m1;
      MetaTransistor*   _m2;
      SpinBoxParameter* _mInt;
  };
  
  
  inline  long  TransistorPair::getMint () const { return _mInt->getValue(); }
  inline  void  TransistorPair::setMint ( long mint ) { _mInt->setValue(mint); }


}  // Analog namespace.

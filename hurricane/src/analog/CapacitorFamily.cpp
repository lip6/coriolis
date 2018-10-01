// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CapacitorFamily.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MetaCapacitor.h"
#include "hurricane/analog/CapacitorFamily.h"

namespace Analog {
  

  CapacitorFamily::CapacitorFamily ( Library* library, const Name& name, const CapacitorFamily::Type& type ) 
  	: Super(library,name)
  	, _type              (type)
  	, _referenceCapacitor(NULL)
  	, _operatorIndex     (0) 
  	, _row               (0)
  { }
  

  void  CapacitorFamily::setReferenceCapacitor ( const Name& referenceCapacitorName )
  {
    Instance* instance = getInstance( referenceCapacitorName );
    if (not instance)
      throw Error( "CapacitorFamily::setReferenceCapacitor(): No reference instance \"%s\"."
                 , getString(referenceCapacitorName).c_str() );

    Cell*          cell          = instance->getMasterCell();
    MetaCapacitor* metaCapacitor = dynamic_cast<MetaCapacitor*>( cell );
    if (not metaCapacitor)
      throw Error( "CapacitorFamily::setReferenceCapacitor(): Bad type for reference capacitor \"%s\"."
                 , getString(referenceCapacitorName).c_str() );

    _referenceCapacitor = metaCapacitor;
  }


}  // Analog namespace.

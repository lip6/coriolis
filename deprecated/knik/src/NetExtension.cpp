// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              K n i k  -  Global Router                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetExtension.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "knik/NetExtension.h"


template<>
Hurricane::Name  Hurricane::StandardPrivateProperty<Knik::NetExtensionDatas>::_name = "Knik::NetExtension";


namespace Knik {

  using Hurricane::Error;
  using Hurricane::ForEachIterator;


  const char* MissingNetExtension = "NetExtension::%s(): %s missing the Knik::Net extension.";

  
// -------------------------------------------------------------------
// Class  :  "Knik::NetExtensionDatas".


  NetExtensionDatas::NetExtensionDatas ()
    : _wireLength()
  { }

  
// -------------------------------------------------------------------
// Class  :  "Knik::NetExtension".


  unsigned long  NetExtension::getWireLength ( const Net* net )
  {
    Extension* extension = Extension::get ( net );
    if ( !extension )
      return 0;

    return extension->getValue()._wireLength;
  }


  void  NetExtension::setWireLength ( Net* net, unsigned long wireLength )
  {
    Extension* extension = Extension::get ( net, true );

    extension->getValue()._wireLength = wireLength;
  }


  int  NetExtension::compare ( const Net* net1, const Net* net2 )
  {
    Extension* extension1 = Extension::get ( net1 );
    Extension* extension2 = Extension::get ( net2 );

    return ( (extension1) ? extension1->getValue()._wireLength : 0 )
         - ( (extension2) ? extension2->getValue()._wireLength : 0 );
  }


  long  NetExtension::getId ( const Net* net )
  {
    Extension* extension = Extension::get ( net );
    if ( !extension )
      return -1;

    return extension->getValue()._id;
  }


  void  NetExtension::setId ( Net* net, long id )
  {
    Extension* extension = Extension::get ( net, true );

    extension->getValue()._id = id;
  }


} // End of Knik namespace.

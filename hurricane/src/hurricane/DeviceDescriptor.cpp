// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DeviceDescriptor.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/DeviceDescriptor.h"


namespace Hurricane {


  DeviceDescriptor::DeviceDescriptor ( const Name& name )
    : _name         (name)
    , _spiceFilePath()
    , _connectors   ()
    , _layouts      ()
  { }


  void DeviceDescriptor::addConnector ( const string& connector )
  { _connectors.push_back( connector ); }


  void DeviceDescriptor::addLayout ( const Name& layoutName, const string& moduleName )
  {
    Layouts::iterator lit = _layouts.find( layoutName );
    if (lit != _layouts.end())
        throw Error( "DeviceDescriptor::addLayout(): Attemp to add same layout \"%\" twice."
                   , getString(layoutName).c_str() );

    _layouts[ layoutName ] = moduleName;
  }


}  // Hurricane namespace.

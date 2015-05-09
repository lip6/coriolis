// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellDatas.cpp"                               |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/Cell.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/Catalog.h"
#include "crlcore/CellDatas.h"


namespace CRL {

  using namespace std;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "CellLoaders".


  CellLoaders*  CellLoaders::_singleton     = NULL;
  unsigned int  CellLoaders::_loaderBit     = 16;
  unsigned int  CellLoaders::_loaderBitMask = 0xFFFF0000;


  CellLoaders* CellLoaders::get ()
  {
    if (not _singleton) _singleton = new CellLoaders ();
    return _singleton;
  }


  CellLoaders::CellLoaders ()
    : _loaders()
  { }


  CellLoaders::~CellLoaders ()
  { for ( auto iloader : _loaders ) delete iloader; }


  unsigned int  CellLoaders::lmask () { return _loaderBitMask; }


  const CellLoader* CellLoaders::getLoader ( string format ) const
  {
    CellLoader key ( format, "", CellLoader::NoFlags, 0 );
    auto iloader = _loaders.find( &key );
    if (iloader == _loaders.end()) return NULL;
    return *iloader;
  }


  const CellLoader* CellLoaders::getLoader ( unsigned int bit ) const
  {
    for ( auto iloader : _loaders ) {
      if (iloader->getFlags() & bit) return iloader;
    }
    return NULL;
  }


  void  CellLoaders::addLoader ( CellLoader* loader )
  {
    if (getLoader(loader->getFormat())) {
      cerr << Error( "CellLoaders::addLoader(): Attemp to add multiple loaders for format <%s>."
                   , loader->getFormat().c_str()
                   ) << endl;
      return;
    }

    loader->setFlags( (1 << _loaderBit++) );
    _loaders.insert( loader );
  }


// -------------------------------------------------------------------
// Class  :  "CellDatas".


  Utilities::Path  CellDatas::_libraryPath;


  CellDatas::CellDatas ( string name )
    : _name       (name)
    , _flags      (0)
    , _activeFlags(0)
    , _cell       (NULL)
    , _state      (NULL)
  { }


  CellDatas::CellDatas ( Cell* cell )
    : _name       ()
    , _flags      (0)
    , _activeFlags(0)
    , _cell       (cell)
    , _state      (NULL)
  {
    if (cell) {
      _name  = getString(_cell->getName());
      _state = CatalogExtension::get( cell );
    }
    else
      _name = "[ERROR] NULL Cell";
  }


}  // CRL namespace.

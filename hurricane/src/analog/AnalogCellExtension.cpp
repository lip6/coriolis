// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :             Damien Dupuis, Eric Lao              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./AnalogCellExtension.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/Net.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Name.h"
#include "hurricane/Cell.h"
#include "hurricane/analog/AnalogCellExtension.h"


namespace Analog {

  using namespace std;
  using namespace Hurricane;


  AnalogData::~AnalogData ()
  { }


  Record* AnalogData::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    return record;
  }


  template<>
  Name  StandardPrivateProperty<AnalogCellExtensionDatas>::_name = "Analog::CellExtension";


  AnalogCellExtensionDatas::AnalogCellExtensionDatas ()
    : _params()
    , _datas ()
  { }


  AnalogCellExtensionDatas::~AnalogCellExtensionDatas ()
  {
    for ( AnalogData* data : _datas ) delete data;
  }

  
  void  AnalogCellExtension::addParameter ( Cell* cell, const Name& name, boost::any value )
  {
    Extension* extension = Extension::get(cell);
    if (not extension) {
      extension = Extension::create();
      cell->put( extension );
    }
    extension->getValue()._params[ name ] = value;
  } 


  boost::any& AnalogCellExtension::getParameter ( const Cell* cell, const Name& name, bool& found )
  {
    static boost::any empty;

    found = false;

    Extension* extension = Extension::get( cell );
    if (not extension) return empty;

    map<Name,boost::any>::iterator it = extension->getValue()._params.find( name );
    if (it == extension->getValue()._params.end()) return empty;

    found = true;
    return (*it).second;
  }


  map<Name,boost::any>* AnalogCellExtension::getParameters ( const Cell* cell )
  {
    Extension* extension = Extension::get( cell );
    if (not extension) return NULL;

    return &(extension->getValue()._params);
  }


}  // Analog namespace.

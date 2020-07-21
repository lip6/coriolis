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
// |  C++ Header  :  "./hurricane/analog/AnalogCellExtension.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CELL_EXTENSION_H
#define ANALOG_CELL_EXTENSION_H

#include <map>
#include <vector>
#include <boost/any.hpp>
#include <string>
#include <list>
#include "hurricane/Property.h"
#include "hurricane/Cell.h"


namespace Analog {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Cell;
  using Hurricane::StandardPrivateProperty;


  class AnalogData {
    public:
      virtual             ~AnalogData   ();
      virtual std::string  _getTypeName () const = 0;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
  };


  class AnalogCellExtensionDatas {
    public:
                                      AnalogCellExtensionDatas ();
                                     ~AnalogCellExtensionDatas ();
      template< typename Data > Data* get                      ();
      template< typename Data > void  set                      ( Data* );
    private:
                                      AnalogCellExtensionDatas ( const AnalogCellExtensionDatas& ) = delete;
    public:
      std::map<Name,boost::any>  _params;
      std::vector<AnalogData*>   _datas;
  };

  
  template< typename Data >
  Data* AnalogCellExtensionDatas::get ()
  {
    for ( AnalogData* base : _datas ) {
      Data* data = dynamic_cast<Data*>( base );
      if (data) return data;
    }
    return NULL;
  }


  template< typename Data >
  void  AnalogCellExtensionDatas::set ( Data* data )
  {
    size_t i = 0;
    for ( ; i < _datas.size() ; ++i ) {
      Data* data = dynamic_cast<Data*>( _datas[i] );
      if (data) { delete _datas[i]; break; }
    }
    if (i == _datas.size()) _datas.push_back( NULL );
    _datas[i] = data;
  }


  class AnalogCellExtension {
    public:
      typedef  StandardPrivateProperty<AnalogCellExtensionDatas>  Extension;
    public:
      static void                       addParameter   ( Cell*, const Name&, boost::any );
      static boost::any&                getParameter   ( const Cell*, const Name&, bool& found );
      static std::map<Name,boost::any>* getParameters  ( const Cell* );
      template< typename Data >
      static void                       set            ( Cell*, Data* );
      template< typename Data >
      static Data*                      get            ( const Cell* );
  };


  template< typename Data >
  void  AnalogCellExtension::set ( Cell* cell, Data* data )
  {
    Extension* extension = Extension::get( cell );
    if (not extension) {
      extension = Extension::create();
      cell->put( extension );
    }
    extension->getValue().set<Data>( data );
  }


  template< typename Data >
  Data* AnalogCellExtension::get ( const Cell* cell )
  {
    Extension* extension = Extension::get( cell );
    if (not extension) return NULL;

    return extension->getValue().get<Data>();
  }


}  // Analog namespace.


namespace Hurricane {

  template<>
  Name  StandardPrivateProperty<Analog::AnalogCellExtensionDatas>::_name;

}  // Hurricane namespace.


#endif  // ANALOG_CELL_EXTENSION_H

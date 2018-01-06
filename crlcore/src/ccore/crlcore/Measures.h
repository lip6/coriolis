// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Measures.h"                             |
// +-----------------------------------------------------------------+


#ifndef CRL_MEASURES_PROPERTY_H
#define CRL_MEASURES_PROPERTY_H

#include <string>
#include <sstream>
#include <iomanip>
#include <map>

#include "hurricane/Property.h"

namespace Hurricane {
  class DBo;
}


namespace CRL {


  using Hurricane::Name;
  using Hurricane::StandardPrivateProperty;
  using Hurricane::DBo;
  using Hurricane::Record;


// -------------------------------------------------------------------
// Class  :  "CRL::BaseMeasure".


  class BaseMeasure {
    public:
      inline                BaseMeasure   ( const Name&, unsigned int width );
      virtual              ~BaseMeasure   ();
      virtual bool          isSimpleData  () const;
      inline  const Name&   getName       () const;
      inline  unsigned int  getFieldWidth () const;
      virtual std::string   toString      () const = 0;
      virtual void          toGnuplot     ( const std::string& basename ) const;
      virtual std::string   _getString    () const;
      virtual Record*       _getRecord    () const;
    private:
      Name          _name;
      unsigned int  _fieldWidth;
  };


  inline               BaseMeasure::BaseMeasure   ( const Name& name, unsigned int width ) : _name(name), _fieldWidth(width) {}
  inline const Name&   BaseMeasure::getName       () const { return _name; }
  inline unsigned int  BaseMeasure::getFieldWidth () const { return _fieldWidth; }


  template<typename Data>
  class Measure : public BaseMeasure {
    public:
      inline               Measure    ( const Name&, const Data&, unsigned int width );
      inline  const Data&  getData    () const;
      inline  void         setData    ( const Data& );
      virtual std::string  toString   () const;
      virtual std::string  _getString () const;
      virtual Record*      _getRecord () const;
    private:
      Data                 _data;
  };


  template<typename Data>
  inline Measure<Data>::Measure ( const Name& name, const Data& data, unsigned int width )
    : BaseMeasure(name,width), _data(data) { }

  template<typename Data>
  inline const Data& Measure<Data>::getData () const { return _data; }

  template<typename Data>
  inline void  Measure<Data>::setData ( const Data& data ) { _data=data; }

  template<typename Data>
  std::string  Measure<Data>::toString () const
  { std::ostringstream s; s << std::fixed << std::setprecision(2) << _data; return s.str(); }

  template<typename Data>
  std::string  Measure<Data>::_getString () const
  { return "<Measure " + Hurricane::demangle(typeid(_data).name()) + ">"; }

  template<typename Data>
  Record* Measure<Data>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      record->add ( getSlot("_data",&_data) );
    }
    return record;
  }


  class MeasuresSet : public std::map<Name,BaseMeasure*> {
    public:
                  ~MeasuresSet     ();
      std::string  toStringHeaders ( const std::vector<Name>& ) const;
      std::string  toStringDatas   ( const std::vector<Name>& ) const;
      void         toGnuplot       ( Name, const std::string& ) const;
      std::string  _getString      () const;
      Record*      _getRecord      () const;
  };


// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresDatas".


  class MeasuresDatas {
    public:
                          MeasuresDatas ();
                          MeasuresDatas ( const MeasuresDatas& );
      inline std::string  _getTypeName  () const;
      inline std::string  _getString    () const;
      inline Record*      _getRecord    () const;
    public:
      MeasuresSet  _measures;
  };


  inline std::string  MeasuresDatas::_getTypeName  () const { return "MeasuresDatas"; }
  inline std::string  MeasuresDatas::_getString    () const { return "<MeasuresDatas>"; }
  inline Record*      MeasuresDatas::_getRecord    () const { return _measures._getRecord(); }
  

// -------------------------------------------------------------------
// Class  :  "CRL::Measures".


  class Measures {
    public:
      typedef StandardPrivateProperty<MeasuresDatas> Extension;
    public:
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, const Data&, unsigned int width );
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, const Data& );
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, Data* );
      template<typename Data> friend inline const Measure<Data>* getMeasure   ( DBo*, const Name& );
                                     static const MeasuresSet*   get          ( const DBo* );
    private:
                                     static Extension*           _getOrCreate ( DBo* );
  };


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, const Data& data, unsigned int width )
  {
    Measures::Extension*   extension = Measures::_getOrCreate( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);

    if (imeasure == extension->getValue()._measures.end()) {
      extension->getValue()._measures.insert ( std::make_pair(name,new Measure<Data>(name,data,width)) );
    } else {
      static_cast< Measure<Data>* >( (*imeasure).second )->setData( data );
    }
  }


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, const Data& data )
  { return addMeasure(object,name,data,8); }


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, Data* data )
  {
    Measures::Extension*   extension = Measures::_getOrCreate( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);
    
    if (imeasure == extension->getValue()._measures.end()) {
      extension->getValue()._measures.insert( std::make_pair(name,new Measure<Data>(name,data)) );
    } else {
      static_cast< Measure<Data>* >( (*imeasure).second )->setData( data );
    }
  }


  template<typename Data>
  inline const Measure<Data>* getMeasure ( DBo* object, const Name& name )
  {
    Measures::Extension*   extension = Measures::_getOrCreate ( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);

    if ( imeasure != extension->getValue()._measures.end() )
      return static_cast< Measure<Data>* >( (*imeasure).second );

    return NULL;
  }


} // End of CRL namespace.


INSPECTOR_P_SUPPORT(CRL::BaseMeasure);
INSPECTOR_P_SUPPORT(CRL::MeasuresDatas);


#endif  // CRL_MEASURES_PROPERTY_H

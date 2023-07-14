// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
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


#pragma  once
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
      const uint32_t  Set = (1<<0);
    public:
      inline                BaseMeasure   ( const Name&, unsigned int width );
      virtual              ~BaseMeasure   ();
      virtual bool          isSimpleData  () const;
      inline  const Name&   getName       () const;
      inline  unsigned int  getFieldWidth () const;
      virtual std::string   toString      ( size_t index ) const = 0;
      virtual void          toGnuplot     ( size_t index, const std::string& basename ) const;
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
      inline               Measure    ( const Name&, unsigned int width );
      inline  bool         hasDataAt  ( size_t index ) const;
      inline  size_t       getSize    () const;
      inline  const Data&  getData    ( size_t index ) const;
      inline  void         setData    ( size_t index, const Data& );
      virtual std::string  toString   ( size_t index ) const;
      virtual std::string  _getString () const;
      virtual Record*      _getRecord () const;
    private:
      Data                                     _blankData;
      std::vector< std::pair<uint32_t,Data> >  _datas;
  };


  template<typename Data>
  inline Measure<Data>::Measure ( const Name& name, unsigned int width )
    : BaseMeasure(name,width), _blankData(), _datas() { }

  template<typename Data>
  inline bool  Measure<Data>::hasDataAt ( size_t index ) const
  { return (index < _datas.size()) ? _datas[index].first & BaseMeasure::Set : 0; }

  template<typename Data>
  inline size_t  Measure<Data>::getSize () const
  { return _datas.size(); }

  template<typename Data>
  inline const Data& Measure<Data>::getData ( size_t index ) const
  { return (index < _datas.size()) ? _datas[index].second : _blankData; }

  template<typename Data>
  inline void  Measure<Data>::setData ( size_t index, const Data& data )
  {
    while ( _datas.size() <= index ) _datas.push_back( std::make_pair(0,Data()) );
    _datas[index].second  = data;
    _datas[index].first  |= BaseMeasure::Set;
  }

  template<typename Data>
  std::string  Measure<Data>::toString ( size_t index ) const
  { std::ostringstream s; s << std::fixed << std::setprecision(2) << getData(index); return s.str(); }

  template<typename Data>
  std::string  Measure<Data>::_getString () const
  { return "<Measure " + Hurricane::demangle(typeid(_datas[0]).name())  + " [" + getString(_datas.size())+ "]>"; }

  template<typename Data>
  Record* Measure<Data>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      record->add ( getSlot("_datas",&_datas) );
    }
    return record;
  }


  class MeasuresSet : public std::map<Name,BaseMeasure*> {
    public:
                  ~MeasuresSet     ();
      std::string  toStringHeaders ( const std::vector<Name>& ) const;
      std::string  toStringDatas   ( const std::vector<Name>&, size_t index ) const;
      void         toGnuplot       ( Name, size_t index, const std::string& ) const;
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
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, size_t index, const Data&, unsigned int width );
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, size_t index, const Data& );
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, size_t index, Data* );
      template<typename Data> friend inline const Measure<Data>* getMeasure   ( DBo*, const Name& );
                                     static const MeasuresSet*   get          ( const DBo* );
    private:
                                     static Extension*           _getOrCreate ( DBo* );
  };


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, size_t index, const Data& data, unsigned int width )
  {
    Measures::Extension*   extension = Measures::_getOrCreate( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);
    
    Measure<Data>* measure = NULL;
    if (imeasure == extension->getValue()._measures.end()) {
      measure = new Measure<Data>( name, width );
      extension->getValue()._measures.insert ( std::make_pair(name,measure) );
    } else {
      measure = static_cast< Measure<Data>* >( (*imeasure).second );
    }
    measure->setData( index, data );
  }


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, size_t index, const Data& data )
  { return ::CRL::addMeasure(object,name,index,data,8); }


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, size_t index, Data* data )
  {
    Measures::Extension*   extension = Measures::_getOrCreate( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);
    
    Measure<Data>* measure = NULL;
    if (imeasure == extension->getValue()._measures.end()) {
      measure = new Measure<Data>( name );
      extension->getValue()._measures.insert( std::make_pair(name,measure) );
    } else {
      measure = static_cast< Measure<Data>* >( (*imeasure).second );
    }
    measure->setData( index, data );
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

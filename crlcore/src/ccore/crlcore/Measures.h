
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Measures.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __STATISTICS_PROPERTY__
#define  __STATISTICS_PROPERTY__

#include  <string>
#include  <sstream>
#include  <iomanip>
#include  <map>

#include  "hurricane/Property.h"

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
      inline  const Name&   getName       () const;
      inline  unsigned int  getFieldWidth () const;
      virtual std::string   toString      () const = 0;
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
      inline               Measure  ( const Name&, const Data&, unsigned int width );
      inline  const Data&  getData  () const;
      inline  void         setData  ( const Data& );
      virtual std::string  toString () const;
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


  class MeasuresSet : public std::map<Name,BaseMeasure*> {
    public:
                  ~MeasuresSet     ();
      std::string  toStringHeaders ( const std::vector<Name>& ) const;
      std::string  toStringDatas   ( const std::vector<Name>& ) const;
  };


// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresDatas".


  class MeasuresDatas {
    public:
                          MeasuresDatas ();
      inline std::string  _getTypeName  () const;
      inline std::string  _getString    () const;
      inline Record*      _getRecord    () const;
    public:
      MeasuresSet  _measures;
    private:
                          MeasuresDatas ( const MeasuresDatas& );
  };


  inline std::string  MeasuresDatas::_getTypeName  () const { return "MeasuresDatas"; }
  inline std::string  MeasuresDatas::_getString    () const { return "<MeasuresDatas>"; }
  inline Record*      MeasuresDatas::_getRecord    () const { return NULL; }
  

// -------------------------------------------------------------------
// Class  :  "CRL::Measures".


  class Measures {
    public:
      typedef StandardPrivateProperty<MeasuresDatas*> Extension;
    public:
      template<typename Data> friend inline void                 addMeasure   ( DBo*, const Name&, const Data&, unsigned int width=8 );
      template<typename Data> friend inline const Measure<Data>* getMeasure   ( DBo*, const Name& );
                                     static const MeasuresSet*   get          ( const DBo* );
    private:
                                     static Extension*           _getOrCreate ( DBo* );
  };


  template<typename Data>
  inline void  addMeasure ( DBo* object, const Name& name, const Data& data, unsigned int width )
  {
    Measures::Extension* extension = Measures::_getOrCreate ( object );
    extension->getValue()->_measures.insert ( std::make_pair(name,new Measure<Data>(name,data,width)) );
  }


  template<typename Data>
  inline const Measure<Data>* getMeasure ( DBo* object, const Name& name )
  {
    Measures::Extension*   extension = Measures::_getOrCreate ( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()->_measures.find(name);

    if ( imeasure != extension->getValue()->_measures.end() )
      return static_cast< Measure<Data>* >( (*imeasure).second );

    return NULL;
  }


} // End of CRL namespace.


INSPECTOR_P_SUPPORT(CRL::MeasuresDatas);


#endif  // __STATISTICS_PROPERTY__

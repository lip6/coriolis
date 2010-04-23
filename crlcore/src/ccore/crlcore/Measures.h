
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


// -------------------------------------------------------------------
// Class  :  "CRL::BaseMeasure".


  class BaseMeasure {
    public:
      inline               BaseMeasure ( const Name& );
      virtual             ~BaseMeasure ();
      inline  const Name&  getName     () const;
      virtual std::string  toString    () const = 0;
    private:
      Name          _name;
  };


  inline             BaseMeasure::BaseMeasure  ( const Name& name ) : _name(name) {}
  inline const Name& BaseMeasure::getName      () const { return _name; }


  template<typename Data>
  class Measure : public BaseMeasure {
    public:
      inline               Measure  ( const Name&, const Data& );
      inline  const Data&  getData  () const;
      inline  void         setData  ( const Data& );
      virtual std::string  toString () const;
    private:
      Data                 _data;
  };


  template<typename Data>
  inline Measure<Data>::Measure ( const Name& name, const Data& data )
    : BaseMeasure(name), _data(data) { }


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
    public:
      MeasuresSet  _measures;
  };


// -------------------------------------------------------------------
// Class  :  "CRL::Measures".


  class Measures {
    public:
      typedef StandardPrivateProperty<MeasuresDatas> Extension;
    public:
      template<typename Data> friend void                 addMeasure   ( DBo*, const Name&, const Data& );
      template<typename Data> friend const Measure<Data>* getMeasure   ( const DBo*, const Name& );
                              static const MeasuresSet*   get          ( const DBo* );
    private:
                              static Extension*           _getOrCreate ( DBo* );
  };


  template<typename Data>
  static void  addMeasure ( DBo* object, const Name& name, const Data& data )
  {
    Measures::Extension* extension = Measures::_getOrCreate ( object );
    extension->getValue()._measures.insert ( std::make_pair(name,new Measure<Data>(name,data)) );
  }


  template<typename Data>
  static Measure<Data>* getMeasure ( DBo* object, const Name& name )
  {
    Measures::Extension*   extension = Measures::_getOrCreate ( object );
    MeasuresSet::iterator  imeasure  = extension->getValue()._measures.find(name);

    if ( imeasure != extension->getValue()._measures.end() )
      return static_cast< Measure<Data>* >( (*imeasure).second );

    return NULL;
  }


} // End of CRL namespace.


#endif  // __STATISTICS_PROPERTY__


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
// |  C++ Header  :       "./Histogram.h"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_HISTOGRAM__
#define  __CRL_HISTOGRAM__

#include  <string>
#include  <iostream>
#include  <vector>
#include  "hurricane/Commons.h"
#include  "crlcore/Measures.h"


namespace CRL {


  class Histogram {
    public:
                                Histogram        ( double range, double step, size_t nbSets );
                               ~Histogram        ();
      inline void               setMainTitle     ( const std::string& );
      inline void               setTitle         ( const std::string&, size_t iset );
      inline void               setColor         ( const std::string&, size_t iset );
             void               addSample        ( double, size_t iset );
      inline void               setFileExtension ( const std::string& );
             float              getYRange        () const;
             void               toStream         ( std::ostream& );
             void               toFile           ( const std::string& );
             void               toGnuplot        ( const std::string& basename );
             void               normalize        ( size_t iset );
             std::string        _getString       () const;
             Hurricane::Record* _getRecord       () const;
    private:
      double                             _range;
      double                             _step;
      std::string                        _mainTitle;
      std::vector< std::string >         _titles;
      std::vector< std::string >         _colors;
      std::vector< std::vector<float> >  _sets;
      std::vector< int >                 _totalSamples;
      std::string                        _fileExtension;
  };


// Inline Functions.
  inline void  Histogram::setMainTitle     ( const std::string& title ) { _mainTitle=title; }
  inline void  Histogram::setTitle         ( const std::string& title, size_t iset ) { if (iset<_titles.size()) _titles[iset] = title; }
  inline void  Histogram::setColor         ( const std::string& color, size_t iset ) { if (iset<_colors.size()) _colors[iset] = color; }
  inline void  Histogram::setFileExtension ( const std::string& extension ) { _fileExtension=extension; }


  template<>
  class Measure<Histogram> : public BaseMeasure {
    public:
      inline               Measure      ( const Name&, Histogram* );
      virtual             ~Measure      ();
      virtual bool         isSimpleData () const;
      inline  Histogram*   getData      () const;
      inline  void         setData      ( Histogram* );
      virtual std::string  toString     () const;
      virtual void         toGnuplot    ( const std::string& basename ) const;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
    private:
      Histogram* _data;
  };


  inline Measure<Histogram>::Measure ( const Name& name, Histogram* data )
    : BaseMeasure(name,0), _data(data) { }

  Measure<Histogram>::~Measure () { delete _data; }

  bool  Measure<Histogram>::isSimpleData () const { return false; }

  inline Histogram* Measure<Histogram>::getData () const { return _data; }

  inline void  Measure<Histogram>::setData ( Histogram* data ) { _data=data; }

  std::string  Measure<Histogram>::toString () const
  { return "Unsupported"; }

  void  Measure<Histogram>::toGnuplot ( const std::string& basename ) const
  { _data->toGnuplot ( basename ); }

  std::string  Measure<Histogram>::_getString () const
  { return "<Measure Histogram>"; }

  Record* Measure<Histogram>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      record->add ( getSlot("_data",_data) );
    }
    return record;
  }


}  // End of CRL namespace.


INSPECTOR_P_SUPPORT(CRL::Histogram)

#endif  // __CRL_HISTOGRAM__

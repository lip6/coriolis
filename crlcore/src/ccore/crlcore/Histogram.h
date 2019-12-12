// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2008-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/Histogram.h"                    |
// +-----------------------------------------------------------------+


#ifndef  CRL_HISTOGRAM_H
#define  CRL_HISTOGRAM_H

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
      inline void               setMainTitle     ( std::string );
      inline void               setTitle         ( std::string, size_t iset );
      inline void               setColor         ( std::string, size_t iset );
      inline void               setIndent        ( std::string, size_t iset );
             void               addSample        ( double, size_t iset );
      inline void               setFileExtension ( std::string );
             float              getYRange        () const;
             void               toStream         ( std::ostream& );
             std::string        toString         ( size_t iset );
             void               toFile           ( std::string );
             void               toGnuplot        ( std::string basename );
             void               normalize        ( size_t iset );
             std::string        _getString       () const;
             Hurricane::Record* _getRecord       () const;
    private:
      double                             _range;
      double                             _step;
      std::string                        _mainTitle;
      std::vector< std::string >         _titles;
      std::vector< std::string >         _colors;
      std::vector< std::string >         _indents;
      std::vector< std::vector<float> >  _sets;
      std::vector< int >                 _totalSamples;
      std::string                        _fileExtension;
  };


// Inline Functions.
  inline void  Histogram::setMainTitle     ( std::string title )               { _mainTitle=title; }
  inline void  Histogram::setTitle         ( std::string title , size_t iset ) { if (iset<_titles .size()) _titles[iset ] = title;  }
  inline void  Histogram::setColor         ( std::string color , size_t iset ) { if (iset<_colors .size()) _colors[iset ] = color;  }
  inline void  Histogram::setIndent        ( std::string indent, size_t iset ) { if (iset<_indents.size()) _indents[iset] = indent; }
  inline void  Histogram::setFileExtension ( std::string extension )           { _fileExtension=extension; }


  template<>
  class Measure<Histogram> : public BaseMeasure {
    public:
                           Measure      ( const Name& );
      virtual             ~Measure      ();
      virtual bool         isSimpleData () const;
      inline  bool         hasDataAt    ( size_t index ) const;
      inline  size_t       getSize      () const;
              Histogram*   getData      ( size_t index ) const;
              void         setData      ( size_t index, Histogram* );
      virtual std::string  toString     ( size_t index ) const;
      virtual void         toGnuplot    ( size_t index, const std::string& basename ) const;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
    private:
      std::vector< std::pair<uint32_t,Histogram*> >  _datas;
  };


  inline bool  Measure<Histogram>::hasDataAt ( size_t index ) const
  { return (index < _datas.size()) ? _datas[index].first & BaseMeasure::Set : 0; }

  inline size_t  Measure<Histogram>::getSize () const
  { return _datas.size(); }


}  // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::Histogram)

#endif  // CRL_HISTOGRAM_H

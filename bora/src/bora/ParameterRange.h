// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/ParameterRange.h"                       |
// +-----------------------------------------------------------------+


#ifndef  BORA_PARAMETER_RANGE_H
#define  BORA_PARAMETER_RANGE_H


#include "hurricane/Commons.h"
#include "hurricane/analog/Matrix.h"


namespace Bora {

  using Hurricane::Record;
  using Analog::Matrix;


// -------------------------------------------------------------------
// Class  :  "ParameterRange"
  
  class ParameterRange {
    public:
      enum { Nested=1 } Flags;
    public:
                               ParameterRange ();
      virtual                 ~ParameterRange ();
      virtual  size_t          getSize        () const = 0;
      inline   bool            progress       ();
      inline   bool            isNested       () const;
      inline   bool            isValid        () const;
      inline   size_t          getIndex       () const;
      inline   bool            setIndex       ( size_t );
      inline   void            setNested      ();
      inline   void            reset          ();
      virtual  ParameterRange* clone          () const = 0;
      virtual  std::string     _getTypeName   () const = 0;
      virtual  std::string     _getString     () const;
      virtual  Record*         _getRecord     () const;
    private:
      unsigned long  _flags;
      size_t         _index;
  };


  inline bool  ParameterRange::progress ()
  { if (_index<getSize()) { ++_index; return true; } return false; }

  inline bool  ParameterRange::isValid () const
  { return (_index < getSize()); }

  inline bool  ParameterRange::isNested () const
  { return _flags & Nested; }

  inline void  ParameterRange::setNested ()
  { _flags |= Nested; }
  
  inline bool  ParameterRange::setIndex ( size_t i )
  { if (i<getSize()) { _index = i; return true; } return false; }

  inline size_t  ParameterRange::getIndex () const { return _index; }
  inline void    ParameterRange::reset    () { _index=0; }


// -------------------------------------------------------------------
// Class  :  "StepParameterRange"
  
  class StepParameterRange : public ParameterRange {
    public:
      typedef  ParameterRange  Super;
    public:
                                   StepParameterRange ( double start, double step, double count );
      virtual                     ~StepParameterRange ();
               double              getValue           () const;
      virtual  size_t              getSize            () const;
      inline   void                copy               ( const StepParameterRange* );
      virtual  StepParameterRange* clone              () const;
      virtual  std::string         _getTypeName       () const;
      virtual  std::string         _getString         () const;
      virtual  Record*             _getRecord         () const;
    private:
      double  _start;
      double  _step;
      double  _count;
  };


  inline void  StepParameterRange::copy ( const StepParameterRange* from )
  {
    _start = from->_start;
    _step  = from->_step;
    _count = from->_count;
  }


// -------------------------------------------------------------------
// Class  :  "MatrixParameterRange"
  
  class MatrixParameterRange : public ParameterRange {
    public:
      typedef  ParameterRange  Super;
    public:
                                     MatrixParameterRange ();
      virtual                       ~MatrixParameterRange ();
               const Matrix&         getValue             () const;
      virtual  size_t                getSize              () const;
      inline   void                  copy                 ( const MatrixParameterRange* );
      virtual  MatrixParameterRange* clone                () const;
      inline   void                  addValue             ( const Matrix* );
      virtual  std::string           _getTypeName         () const;
      virtual  std::string           _getString           () const;
      virtual  Record*               _getRecord           () const;
    private:
      std::vector<Matrix>  _matrixes;
  };


  inline void  MatrixParameterRange::copy ( const MatrixParameterRange* from )
  { for ( const Matrix& m : from->_matrixes ) addValue( &m ); }


  inline void  MatrixParameterRange::addValue ( const Matrix* m )
  { _matrixes.push_back( *m ); }


}  // Bora namespace.


INSPECTOR_P_SUPPORT(Bora::ParameterRange);
INSPECTOR_P_SUPPORT(Bora::StepParameterRange);
INSPECTOR_P_SUPPORT(Bora::MatrixParameterRange);

#endif

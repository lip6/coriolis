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
// |  C++ Module  :  "./arameterRange.cpp"                           |
// +-----------------------------------------------------------------+


#include "bora/ParameterRange.h"


namespace Bora {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "ParameterRange"


  ParameterRange::ParameterRange ()
    : _flags(0), _index(0)
  { }


  ParameterRange::~ParameterRange ()
  { }


  string  ParameterRange::_getString () const
  { return "<" + _getTypeName() + ">"; }


  Record* ParameterRange::_getRecord () const
  {
    Record* record = new Record( getString(this) );
    record->add( getSlot("_index", _index) );
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "StepParameterRange"

  
  StepParameterRange::StepParameterRange ( double start, double step, double count )
    : Super()
    , _start(start)
    , _step (step )
    , _count(count)
  { }


  StepParameterRange::~StepParameterRange ()
  { }


  StepParameterRange* StepParameterRange::clone () const
  {
    return new StepParameterRange( _start, _step, _count );
  }


  size_t  StepParameterRange::getSize () const
  { return size_t(_count); }


  double  StepParameterRange::getValue () const
  { return _start + getIndex() * _step; }


  string  StepParameterRange::_getTypeName () const
  { return "StepParameterRange"; }


  string  StepParameterRange::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " [" + getString(_start));
    s.insert( s.length() - 1, " +" + getString(_step ));
    s.insert( s.length() - 1, " ," + getString(_count) + "]");
    return s;
  }


  Record* StepParameterRange::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_start", _start) );
      record->add( getSlot("_step" , _step ) );
      record->add( getSlot("_count", _count) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "MatrixParameterRange"

  
  MatrixParameterRange::MatrixParameterRange ()
    : Super()
    , _matrixes()
  { }


  MatrixParameterRange::~MatrixParameterRange ()
  { }


  MatrixParameterRange* MatrixParameterRange::clone () const
  {
    MatrixParameterRange* param = new MatrixParameterRange();
    param->copy( this );
    return param;
  }


  size_t  MatrixParameterRange::getSize () const
  { return _matrixes.size(); }


  const Matrix& MatrixParameterRange::getValue () const
  { return _matrixes[ getIndex() ]; }

    
  string  MatrixParameterRange::_getTypeName () const
  { return "MatrixParameterRange"; }


  string  MatrixParameterRange::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.length() - 1, " [" + getString(getSize()) + "]");
    return s;
  }


  Record* MatrixParameterRange::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_matrixes", &_matrixes) );
    }
    return record;
  }


}  // Bora namespace.

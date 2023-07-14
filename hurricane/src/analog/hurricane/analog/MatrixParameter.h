// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/MatrixParameter.h"          |
// +-----------------------------------------------------------------+


#ifndef ANALOG_MATRIX_PARAMETER_H
#define ANALOG_MATRIX_PARAMETER_H

#include "hurricane/analog/Matrix.h"
#include "hurricane/analog/Parameter.h"

namespace Analog {
  

  class MatrixParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               MatrixParameter ( std::string id, size_t rows=1, size_t columns=1 );
      inline  size_t       getRows         () const;
      inline  size_t       getColumns      () const;
      inline  size_t       getValue        ( size_t row , size_t column  ) const;
      inline  void         setValue        ( size_t row , size_t column  , size_t value );
      inline  void         setMatrix       ( const Matrix* );
      inline  void         resize          ( size_t rows, size_t columns );
      virtual std::string  _getTypeName    () const;
      virtual std::string  _getString      () const;
      virtual Record*      _getRecord      () const;
    private:
      Matrix  _matrix;
  };

  
  inline MatrixParameter::MatrixParameter ( std::string id, size_t rows, size_t columns )
    : Parameter(id), _matrix(rows,columns)
  { } 

  
  inline size_t  MatrixParameter::getRows    () const { return _matrix.rows   (); }
  inline size_t  MatrixParameter::getColumns () const { return _matrix.columns(); }
  inline size_t  MatrixParameter::getValue   ( size_t row , size_t column  ) const { return _matrix.at(row,column); }
  inline void    MatrixParameter::setValue   ( size_t row , size_t column  , size_t value ) { _matrix.at(row,column) = value; }
  inline void    MatrixParameter::resize     ( size_t rows, size_t columns ) { _matrix.resize(rows,columns); }
  inline void    MatrixParameter::setMatrix  ( const Matrix* from ) { _matrix = *from; }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::MatrixParameter);

#endif // ANALOG_MATRIX_PARAMETER_H

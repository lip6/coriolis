// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MatrixParameter.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/analog/MatrixParameter.h"


namespace Analog {

  using std::string;


  string  MatrixParameter::_getTypeName () const
  { return "MatrixParameter"; }

  
  std::string  MatrixParameter::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " ["+getString(_matrix.rows())+" x "+getString(_matrix.columns())+"]" );
    return s;
  }

  
  Record* MatrixParameter::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_matrix", &_matrix ) );
    return record;
  }




}  // Analog namespace.

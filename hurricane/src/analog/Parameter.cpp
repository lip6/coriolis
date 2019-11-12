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
// |  C++ Module  :  "./Parameter.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/analog/Parameter.h"


namespace Analog {


  Parameter::~Parameter ()
  { }

  
  std::string  Parameter::_getString () const
  { return "<"+_getTypeName()+" \""+getName()+"\">"; }

  
  Record* Parameter::_getRecord () const
  {
    Record* record = new Record( _getString() );
    record->add( getSlot( "_name" , &_name  ) );
    record->add( getSlot( "_index", &_index ) );
    return record;
  }




}  // Analog namespace.

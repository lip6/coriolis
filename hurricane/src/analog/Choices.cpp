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
// |  C++ Module  :  "./Choices.cpp"                                 |
// +-----------------------------------------------------------------+


#include <algorithm>
#include "hurricane/Error.h"
#include "hurricane/analog/Choices.h"


namespace Analog {
  
  using namespace Hurricane;


  Choices::Choices ()
    : _values()
  { }


  Choices::Choices ( const Choices& choices )
    : _values(choices._values)
  { }


  Choices& Choices::operator<< ( const string& choice )
  {
    Values::iterator it = find( _values.begin(), _values.end(), choice );
    if (it == _values.end()) _values.push_back(choice);
    else
      throw Error ( "Choices::operator<<(): The choice \"%s\" is already a member of choices."
                  , choice.c_str() );
    return *this;
  }


  unsigned int  Choices::findIndexFor ( const string& choice ) const
  {
    Values::const_iterator it = find( _values.begin(), _values.end(), choice );
    if (it != _values.end()) return it - _values.begin();

    throw Error( "Choices::findIndexFor(): Entry \"%s\" doesn\'t exists in choices."
               , choice.c_str() );
  }


}  // Analog namespace.

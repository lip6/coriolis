// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                   Stephanie Youssef              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./hurricane/analog/BJTArguments.cpp"           |
// +-----------------------------------------------------------------+


#ifndef ANALOG_BJT_ARGUMENTS_H
#define ANALOG_BJT_ARGUMENTS_H

#include "hurricane/DbU.h"
#include "hurricane/analog/Arguments.h"


namespace Analog {

  class BJTArguments : public Arguments {
    public:
      typedef Arguments Super;
    public:
                            BJTArguments    ( bool isNPN, bool isExternalDummy, unsigned m, unsigned n );
                            BJTArguments    ( const BJTArguments & bjtArgs );
      virtual BJTArguments* getClone        ();
      inline  bool          isNPN           () const;
      inline  bool          isExternalDummy () const;
      inline  unsigned int  getM            () const;
      inline  unsigned int  getN            () const;
      inline  void          setM            (unsigned m );
      inline  void          setN            (unsigned n );
    private:
      bool          _isNPN;
      bool          _isExternalDummy;
      unsigned int  _m;
      unsigned int  _n;
  };

  
  inline  bool          BJTArguments::isNPN           () const      { return _isNPN; };
  inline  bool          BJTArguments::isExternalDummy () const      { return _isExternalDummy; };
  inline  unsigned int  BJTArguments::getM            () const      { return _m; };
  inline  unsigned int  BJTArguments::getN            () const      { return _n; };
  inline  void          BJTArguments::setM            (unsigned m ) { _m = m; };
  inline  void          BJTArguments::setN            (unsigned n ) { _n = n; };


}  // Analog namespace.

#endif // ANALOG_BJT_ARGUMENTS_H

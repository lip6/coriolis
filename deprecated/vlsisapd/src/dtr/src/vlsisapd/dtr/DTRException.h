
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |           Design Technological Rules (DTR)                      |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/dtr/Rule.h"                         |
// +-----------------------------------------------------------------+


#ifndef __DTR_EXCEPTION_H__
#define __DTR_EXCEPTION_H__

#include <exception>
#include <string>


namespace DTR {


  class DTRException {
	public:
      inline                     DTRException (const std::string& what);
      inline virtual            ~DTRException ();
      inline virtual const char* what         () const;
    private:
      std::string  _what;
  };


  inline             DTRException::DTRException  ( const std::string& what ) : _what(what) {}
  inline             DTRException::~DTRException () {}
  inline const char* DTRException::what          () const { return _what.c_str(); }


} // DTR namespace.

#endif


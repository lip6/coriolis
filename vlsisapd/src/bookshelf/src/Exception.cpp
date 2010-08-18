
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Exception.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdarg>
#include  "vlsisapd/bookshelf/Exception.h"


namespace Bookshelf {


  Exception::Exception ( const std::string& message ) throw()
    : std::exception()
    , _message(_addHeader(message.c_str()))
  { }


  Exception::Exception ( const char* format, ... ) throw()
    : std::exception()
    , _message()
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    _message = _addHeader(formatted);
  }


  Exception::Exception ( const Exception& e ) throw()
    : std::exception()
    , _message(e._message)
  { }


  Exception::~Exception () throw()
  { }


  const char* Exception::what () const throw()
  { return _message.c_str(); }


  std::string Exception::_addHeader ( const char* rawMessage )
  { return std::string("[ERROR] ") + rawMessage; }



} // End of Bookshelf namespace.

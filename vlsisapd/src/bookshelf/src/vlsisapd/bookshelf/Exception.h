
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
// |  C++ Header  :       "./bookshelf/Exception.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __VLSISAPD_BOOKSHELF_EXCEPTION__
#define  __VLSISAPD_BOOKSHELF_EXCEPTION__

#include  <string>
#include  <exception>


namespace Bookshelf {


  class Exception : public std::exception {
    public:
               Exception ( const std::string& );
               Exception ( const char* format, ... );
               Exception ( const Exception& );
      virtual ~Exception ();
    public:
      virtual const char* what       () const;
    private:
      static std::string  _addHeader ( const char* );
    private:
      std::string  _message;
  };


} // End of Bookshelf namespace.


#endif  // __VLSISAPD_BOOKSHELF_EXCEPTION__


// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Bookshelf.cpp"                          |
// +-----------------------------------------------------------------+


#include  "vlsisapd/bookshelf/Bookshelf.h"
#include  "vlsisapd/bookshelf/BookshelfException.h"


namespace Bookshelf {


  BookshelfParser::BookshelfParser ()
  { }


  void  BookshelfParser::readFromFile ( std::string auxFile )
  {
    Utilities::Path auxPath ( auxFile );
    if ( not auxPath.exists() ) {
      throw BookshelfException ( "%s .aux file not found.", auxPath.string().c_str() );
    }

    std::cout << "  o  Reading Bookshelf .aux: <" << auxPath.string() << ">." << std::endl;
  }


} // End of Bookself namespace.

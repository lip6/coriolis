
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
// |  C++ Module  :       "./Bookshelf.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <boost/filesystem/operations.hpp>
#include  <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

#include  "vlsisapd/bookshelf/Bookshelf.h"
#include  "vlsisapd/bookshelf/BookshelfException.h"


namespace Bookshelf {


  BookshelfParser::BookshelfParser ()
  { }


  void  BookshelfParser::readFromFile ( std::string auxFile )
  {
    bfs::path auxPath ( auxFile );
    if ( not bfs::exists(auxPath) ) {
      throw BookshelfException ( "%s .aux file not found.", auxPath.string().c_str() );
    }

    std::cout << "  o  Reading Bookshelf .aux: <" << auxPath.string() << ">." << std::endl;
  }


} // End of Bookself namespace.

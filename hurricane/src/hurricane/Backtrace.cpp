
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2010, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Error.cpp"                                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <execinfo.h>
#include  <sstream>
#include  <iomanip>
#include  <boost/regex.hpp>
#include  "hurricane/Backtrace.h"


namespace Hurricane {

  using std::string;
  using std::vector;
  using std::setw;
  using std::setfill;
  using std::ostringstream;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Backtrace".


  TextTranslator  Backtrace::_textTranslator = TextTranslator::toTextTranslator();
  const size_t    Backtrace::_stackSize      = 50;


// Examples of stack symbol string:
// * Linux:
//     nwidget(_ZN18SchematicException4initEb+0x47) [0x4515e1]
// * OSX:
//     3 libstdc++.6.dylib 0x9142514b _ZSt9terminatev + 29


  Backtrace::Backtrace ()
    : _stack()
  {
    void*  rawStack [ _stackSize ];
    size_t depth    = backtrace ( rawStack, _stackSize );
    char** symbols  = backtrace_symbols ( rawStack, depth );

#ifdef __linux__
    boost::regex  re ( "([^/]+)\\(([^+]+)\\+" ); 
    boost::cmatch match;

    for ( size_t i=0 ; i<depth ; ++i ) {
      if ( boost::regex_search(symbols[i],match,re) ) {
        string function  ( match[2].first, match[2].second );
        string demangled ( demangle(function.c_str()) );
        if ( demangled.empty() )
          _stack.push_back ( (demangled.empty()) ? function : demangled );
        else {
          string reformated ( match[1].first, match[1].second );
          reformated += "( <b>" + demangled + "</b> )";
          _stack.push_back ( reformated );
        }
      } else {
        _stack.push_back ( symbols[i] );
      }
    }
#else
#  ifdef  __APPLE__
    boost::regex re ( "(\\d+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+\\+\\s+(\\d+)" ); 
    boost::cmatch match;

    for ( size_t i=0 ; i<depth ; ++i ) {
      if ( boost::regex_search(symbols[i],match,re) ) {
        string function  ( match[4].first, match[4].second );
        string demangled ( demangle(function.c_str()) );
        if ( demangled.empty() )
          _stack.push_back ( (demangled.empty()) ? function : demangled );
        else {
          string reformated ( match[1].first, match[1].second );
          reformated += " <b>" + demangled + "</b>";
          _stack.push_back ( reformated );
        }
      } else {
        _stack.push_back ( symbols[i] );
      }
    }
#  else
    _stack.push_back ( "Backtrace only supported under Linux & OSX." );
#  endif
#endif
  }


  string  Backtrace::htmlWhere () const
  {
    ostringstream where;

    for ( size_t depth=0 ; depth<_stack.size() ; ++depth )
      where << "<tt>[" << setw(2) << setfill('0') << depth << "] " << _stack[depth] << "</tt><br>";

    return where.str();
  }


}  // End of Hurricane namespace.

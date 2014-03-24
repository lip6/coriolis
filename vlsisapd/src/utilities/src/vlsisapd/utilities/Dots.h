// -*- C++ -*-
//
// This file is part of the VLSI Stand-Alone Software.
// Copyright (c) UPMC 2013-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |    M i s c e l l a n e o u s   U t i l i t i e s                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./Dots.h"                                      |
// +-----------------------------------------------------------------+


#ifndef VLSISAPD_UTILITIES_DOTS_H
#define VLSISAPD_UTILITIES_DOTS_H

#include <string>
#include <iostream>


namespace Utilities {


  class Dots {
    public:
      enum Flag { NoFlags=0x00, FirstDot=0x01, Reset=0x02 };
    public:
      inline        Dots       ( std::ostream&
                               , const std::string& indent
                               , unsigned int       width  =70
                               , unsigned int       divider= 1 );
      inline  void  setWidth   ( unsigned int );
      inline  void  setDivider ( unsigned int );
      inline  void  reset      ( unsigned int flags );
      inline  void  CR         ();
              void  dot        ();
              void  finish     ( unsigned int flags );
    private:
      inline  void  _flush     ( char );
    private:
      std::ostream& _ostream;
      std::string   _indent;
      unsigned int  _width;
      unsigned int  _divider;
      unsigned int  _count;
      unsigned int  _flags;
  };


  inline Dots::Dots ( std::ostream& o, const std::string& indent, unsigned int width, unsigned int divider )
    : _ostream(o)
    , _indent (indent)
    , _width  (width)
    , _divider(divider)
    , _count  (0)
    , _flags  (FirstDot)
  { }

  inline  void  Dots::setWidth   ( unsigned int w ) { _width=w; }
  inline  void  Dots::setDivider ( unsigned int d ) { _divider=d; }

  inline void  Dots::_flush ( char c )
  {
    _ostream.put( c );
    _ostream.flush();
  }

  inline void  Dots::CR ()
  {
    _flush('\n');
  }

  inline void  Dots::reset ( unsigned int flags )
  {
    _flags = flags;
    _count = 0;
  }


} // Utilities namespace.

#endif // VLSISAPD_UTILITIES_DOTS_H

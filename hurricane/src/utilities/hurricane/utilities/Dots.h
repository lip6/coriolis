// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2013-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/utilities/Dots.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <iostream>


namespace Utilities {


  class Dots {
    public:
      enum Flag { NoFlags=0x00, FirstDot=0x01, Reset=0x02, Disabled=0x04 };
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
      inline  bool  enabled    ();
      inline  void  enable     ();
      inline  void  disable    ();
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
  inline  bool  Dots::enabled    () { return not (_flags & Disabled); }
  inline  void  Dots::enable     () { _flags &= ~Disabled; }
  inline  void  Dots::disable    () { _flags |=  Disabled; }

  inline void  Dots::_flush ( char c )
  {
    if (not enabled()) return;

    _ostream << c;
    _ostream.flush();
  }

  inline void  Dots::CR ()
  {
    _flush('\n');
  }

  inline void  Dots::reset ( unsigned int flags )
  {
    _flags = flags | ((not enabled()) ? Disabled : NoFlags);
    _count = 0;
  }


} // Utilities namespace.

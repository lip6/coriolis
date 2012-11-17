
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2011-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Port.h"                   |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_PORT_H__
#define __OPENCHAMS_PORT_H__


namespace OpenChams {


  class Port {
    public:
      inline                    Port          ( const std::string& type
                                              , unsigned           idx
                                              , double             x
                                              , double             y
                                              , const std::string& orient );
      inline                   ~Port          ();
      inline const std::string& getType       () const;
      inline unsigned           getIndex      () const;
      inline double             getX          () const;
      inline double             getY          () const;
      inline const std::string& getOrientation() const;
    private:
      std::string  _type;
      unsigned     _idx;
      double       _x;
      double       _y;
      std::string  _orient;
};


  inline  Port::Port (const std::string& type
                     , unsigned idx
                     , double x
                     , double y
                     , const std::string& orient)
    : _type(type), _idx(idx), _x(x), _y(y), _orient(orient)
  { }

  inline  Port::~Port ()
  { }

  inline const std::string& Port::getType       () const { return _type; }
  inline unsigned           Port::getIndex      () const { return _idx; }
  inline double             Port::getX          () const { return _x; }
  inline double             Port::getY          () const { return _y; }
  inline const std::string& Port::getOrientation() const { return _orient; }
    

} // OpenChams namespace.

#endif


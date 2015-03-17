// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./bookshelf/Node.h"                       |
// +-----------------------------------------------------------------+


#ifndef  VLSISAPD_BOOKSHELF_NODE_H
#define  VLSISAPD_BOOKSHELF_NODE_H

#include  <string>
#include  <iostream>
#include  <iomanip>
#include  <map>


namespace Bookshelf {

  class Pin;


  class Symmetry {
    public:
      enum Code { Disabled=0x0, X=0x1, Y=0x2, R90=0x4 };
  };


  class Orientation {
    public:
      enum Code { Disabled=0, N, E, S, W, FN, FS, FW, FE };
  };


  class Node {
    public:
      enum Flag { NoFlags=0x0000, Terminal=0x0001, Fixed=0x0002 };
    public:
      inline                        Node           ( const std::string& name
                                                   , double             width   =0.0
                                                   , double             height  =0.0
                                                   , unsigned int       symmetry=Symmetry::Disabled
                                                   , unsigned int       flags   =NoFlags );
      inline bool                   isTerminal     () const;
      inline bool                   isFixed        () const;
      inline const std::string&     getName        () const;
      inline double                 getWidth       () const;
      inline double                 getHeight      () const;
      inline double                 getX           () const;
      inline double                 getY           () const;
      inline unsigned int           getSymmetry    () const;
      inline unsigned int           getOrientation () const;
             Pin*                   getPin         ( size_t id ) const;
      inline std::map<size_t,Pin*>& getPins        ();
             void                   addPin         ( Pin* );
      inline void                   setFlags       ( unsigned int flags );
      inline void                   unsetFlags     ( unsigned int flags );
      inline void                   setX           ( double );
      inline void                   setY           ( double );
      inline void                   setOrientation ( unsigned int );
             void                   writeToStream  ( std::ostream&, unsigned int flags );
      friend std::ostream&          operator<<     ( std::ostream&, const Node* );
    private:
      std::string            _name;
      double                 _width;
      double                 _height;
      unsigned int           _symmetry;
      double                 _x;
      double                 _y;
      unsigned int           _orientation;
      unsigned int           _flags;
      std::map<size_t,Pin*>  _pins;
  };


  inline Node::Node ( const std::string& name
                    , double             width
                    , double             height
                    , unsigned int       symmetry
                    , unsigned int       flags
                    )
    : _name       (name)
    , _width      (width)
    , _height     (height)
    , _symmetry   (symmetry)
    , _x          (0.0)
    , _y          (0.0)
    , _orientation(Orientation::Disabled)
    , _flags      (flags)
  {
    // std::cerr << "Node::Node() " 
    //           << (void*)this
    //           << " - <"
    //           << name
    //           << " " << _width
    //           << " " << _height
    //           << " " << std::boolalpha << _terminal
    //           << " :" << _symmetry
    //           << ">." << std::endl;
  }

  inline bool                   Node::isTerminal     () const { return _flags&Terminal; }
  inline bool                   Node::isFixed        () const { return _flags&Fixed; }
  inline const std::string&     Node::getName        () const { return _name; }
  inline double                 Node::getWidth       () const { return _width; }
  inline double                 Node::getHeight      () const { return _height; }
  inline double                 Node::getX           () const { return _x; }
  inline double                 Node::getY           () const { return _y; }
  inline unsigned int           Node::getSymmetry    () const { return _symmetry; }
  inline unsigned int           Node::getOrientation () const { return _orientation; }
  inline std::map<size_t,Pin*>& Node::getPins        () { return _pins; }
  inline void                   Node::setFlags       ( unsigned int flags ) { _flags |=  flags; }
  inline void                   Node::unsetFlags     ( unsigned int flags ) { _flags &= ~flags; }
  inline void                   Node::setX           ( double x ) { _x=x; }
  inline void                   Node::setY           ( double y ) { _y=y; }
  inline void                   Node::setOrientation ( unsigned int orient ) { _orientation=orient; }


} // End of Bookshelf namespace.


#endif  // __VLSISAPD_BOOKSHELF_NODE__

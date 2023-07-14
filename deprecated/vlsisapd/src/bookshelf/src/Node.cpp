
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
// |  C++ Module  :       "./Node.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Circuit.h"


namespace Bookshelf {


  Pin* Node::getPin ( size_t id ) const
  {
    std::map<size_t,Pin*>::const_iterator ipin = _pins.find(id);
    if ( ipin != _pins.end() ) return (*ipin).second;

    return NULL;
  }


  void  Node::addPin ( Pin* pin )
  {
    if ( getPin(pin->getId()) != NULL ) {
      throw Exception("Bookshelf::Node::addPin(): Node <%s> already connected to net id:%d"
                     ,_name.c_str(),pin->getId());
    }

    _pins.insert ( std::make_pair(pin->getId(),pin) );
  }


  void  Node::writeToStream ( std::ostream& o, unsigned int flags )
  {
  //std::cerr << "Node::writeToStream() " << (void*)this << std::endl;

    if ( flags & Circuit::Nodes ) {
      o << std::setw(20) << std::right << _name;
      if ( (_width != 0.0) or (_height != 0.0) ) {
        o << " " << std::setw(10) << std::right << _width << " " << std::setw(10) << std::right << _height;
      }
      if ( flags & Terminal ) o << " terminal";
      if ( _symmetry != Symmetry::Disabled ) {
        o << " :";
        if ( _symmetry & Symmetry::R90 ) o << " R90";
        if ( _symmetry & Symmetry::X   ) o << " X";
        if ( _symmetry & Symmetry::Y   ) o << " Y";
      }
      o << std::endl;
    } else if ( flags & Circuit::Pl ) {
      o << std::setw(20) << std::right << _name;
      o << " " << std::setw(10) << std::right << _x << " " << std::setw(10) << std::right << _y;
      if ( _orientation != Orientation::Disabled ) {
        o << " :";
        if ( _orientation == Orientation::N  ) o << " N";
        if ( _orientation == Orientation::E  ) o << " E";
        if ( _orientation == Orientation::S  ) o << " S";
        if ( _orientation == Orientation::W  ) o << " W";
        if ( _orientation == Orientation::FN ) o << " FN";
        if ( _orientation == Orientation::FE ) o << " FE";
        if ( _orientation == Orientation::FS ) o << " FS";
        if ( _orientation == Orientation::FW ) o << " FW";
      }
      if ( (flags & Fixed) && (flags & NoImage) ) o << " /FIXED_NI";
      else if ( flags & Fixed ) o << " /FIXED";
      o << std::endl;
    }
  }


  std::ostream& operator<< ( std::ostream& o, const Node* node )
  {
    o << "<Node " << node->_name;
    if ( (node->_x != 0.0) or (node->_y != 0) ) {
      o << " " << node->_x << " " << node->_y;
    }
    if ( node->_symmetry != Symmetry::Disabled ) {
      o << " :";
      if ( node->_symmetry & Symmetry::X   ) o << " X";
      if ( node->_symmetry & Symmetry::Y   ) o << " Y";
      if ( node->_symmetry & Symmetry::R90 ) o << " R90";
    }

    if ( (node->_flags & Node::Terminal) && (node->_flags & Node::NoImage) ) o << " terminal_NI";
    else if ( node->_flags & Node::Terminal ) o << " terminal";

    if ( (node->_flags & Node::Fixed) && (node->_flags & Node::NoImage) ) o << " /FIXED_NI";
    else if ( node->_flags & Node::Fixed    ) o << " /FIXED";
      
    return o;
  }


}  // End of Bookshelf namespace.

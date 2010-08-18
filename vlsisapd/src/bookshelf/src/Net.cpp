
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
// |  C++ Module  :       "./Net.cpp"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Circuit.h"


namespace Bookshelf {


  Net::Net ( Circuit* circuit, size_t degree, const std::string& name )
    : _circuit(circuit)
    , _id     (circuit->getNets().size())
    , _degree (degree)
    , _name   (name)
    , _pins   ()
  {
  //if ( _name.empty() )
  //  _name = circuit->getAutomaticName ();

    circuit->addNet ( this );
  }


  Net::~Net ()
  {
    std::vector<Pin*>::iterator ipin = _pins.begin();
    for ( ; ipin != _pins.end() ; ++ipin )
      delete (*ipin);
  }


  void  Net::addPin ( Pin* pin )
  {
    _pins.push_back(pin);
  }


  void  Net::writeToStream ( std::ostream& o )
  {
    o << "NetDegree : " << _degree;
  //if ( not _circuit->hasAutomaticName(this) )
      o << " " << _name;
    o << std::endl;

    std::vector<Pin*>::iterator ipin = _pins.begin();
    for ( ; ipin != _pins.end() ; ++ipin )
      (*ipin)->writeToStream ( o );
  }


}  // End of Bookshelf namespace.

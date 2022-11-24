
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
// |  C++ Header  :       "./bookshelf/Net.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __VLSISAPD_BOOKSHELF_NET__
#define  __VLSISAPD_BOOKSHELF_NET__

#include  <string>
#include  <vector>
#include  <iostream>
#include  <iomanip>


namespace Bookshelf {

  class Pin;
  class Circuit;


  class Net {
    public:
      static void               resetMapName  ();
    public:
                                Net           ( Circuit*, size_t degree, const std::string& name );
                               ~Net           ();
      inline Circuit*           getCircuit    () const;
      inline size_t             getId         () const;
      inline size_t             getDegree     () const;
      inline const std::string& getName       () const;
      inline std::vector<Pin*>& getPins       ();
             void               addPin        ( Pin* );
      inline void               setName       ( const std::string& );
             void               writeToStream ( std::ostream& );
    private:
             Circuit*           _circuit;
             size_t             _id;
             size_t             _degree;
             std::string        _name;
             std::vector<Pin*>  _pins;
  };


  inline Circuit*           Net::getCircuit () const { return _circuit; }
  inline size_t             Net::getId      () const { return _id; }
  inline size_t             Net::getDegree  () const { return _degree; }
  inline const std::string& Net::getName    () const { return _name; }
  inline std::vector<Pin*>& Net::getPins    () { return _pins; }
  inline void               Net::setName    ( const std::string& name ) { _name=name; }


} // End of Bookshelf namespace.


#endif  // __VLSISAPD_BOOKSHELF_NET__

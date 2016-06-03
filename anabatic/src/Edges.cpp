// -*- mode: C++; explicit-buffer-name: "Edges.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./anabatic/Edges.cpp"                          |
// +-----------------------------------------------------------------+


#include "anabatic/Edges.h"
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::endl;


// -------------------------------------------------------------------
// Class  :  "Anabatic::GCell_Edges".

  GCell_Edges::Locator::Locator ( const GCell* gcell )
    : EdgesHL()
    , _gcell(gcell)
    , _flags(Flags::EastSide)
    , _iedge(0)
  {
    // cdebug.log(110) << "GCell_Edges::Locator::Locator() " << isValid() << endl;
    if (_gcell->getEastEdges().empty()) progress();
  }


  EdgesHL* GCell_Edges::Locator::getClone () const
  {
    // cdebug.log(110) << "GCell_Edges::Locator::getClone()" << endl;
    return new Locator (*this);
  }


  Edge* GCell_Edges::Locator::getElement () const
  { 
    if (_flags.contains(Flags::EastSide )) return _gcell->getEastEdges ()[_iedge];
    if (_flags.contains(Flags::NorthSide)) return _gcell->getNorthEdges()[_iedge];
    if (_flags.contains(Flags::WestSide )) return _gcell->getWestEdges ()[_iedge];
    if (_flags.contains(Flags::SouthSide)) return _gcell->getSouthEdges()[_iedge];
    return NULL;
  }


  bool  GCell_Edges::Locator::isValid () const
  { return _flags; }


  void  GCell_Edges::Locator::progress ()
  {
    // cdebug.log(110) << "GCell_Edges::Locator::progress() [from] " << _flags << " iedge:" << _iedge << endl;
    // cdebug.log(110) << "  East:"  << _gcell->getEastEdges().size()
    //                 << "  North:" << _gcell->getNorthEdges().size()
    //                 << "  West:"  << _gcell->getWestEdges().size()
    //                 << "  South:" << _gcell->getSouthEdges().size() << endl;
    // cdebug.log(110) << this << endl;

    ++_iedge;
    while (_flags) {
      if (_flags.contains(Flags::EastSide)) {
        if (_iedge < _gcell->getEastEdges().size()) break;
        // cdebug.log(110) << "Switching to North side." << endl;
        _flags = Flags::NorthSide;
        _iedge = 0;
        // cdebug.log(110) << this << endl;
        continue;
      }
      if (_flags.contains(Flags::NorthSide))  {
        if (_iedge < _gcell->getNorthEdges().size()) break;
        // cdebug.log(110) << "Switching to West side." << endl;
        _flags = Flags::WestSide;
        _iedge = 0;
        // cdebug.log(110) << this << endl;
        continue;
      }
      if (_flags.contains(Flags::WestSide))  {
        if (_iedge < _gcell->getWestEdges().size()) break;
        // cdebug.log(110) << "Switching to South side." << endl;
        _flags = Flags::SouthSide;
        _iedge = 0;
        continue;
      }
      if (_flags.contains(Flags::SouthSide))  {
        if (_iedge < _gcell->getSouthEdges().size()) break;
        // cdebug.log(110) << "All edges done." << endl;
        _flags = 0;
        _iedge = 0;
        break;;
      }
    }

    // cdebug.log(110) << "GCell_Edges::Locator::progress() [to]   " << _flags << " iedge:" << _iedge << endl;
  }


  string  GCell_Edges::Locator::_getString () const
  {
    string s = "<GCell_Edges::Locator";
    if (_flags.contains(Flags::EastSide )) s += " East["  + getString(_iedge) + "]";
    if (_flags.contains(Flags::NorthSide)) s += " North[" + getString(_iedge) + "]";
    if (_flags.contains(Flags::WestSide )) s += " West["  + getString(_iedge) + "]";
    if (_flags.contains(Flags::SouthSide)) s += " South[" + getString(_iedge) + "]";
    if (_flags == 0)                       s += " invalid";
    s += ">";
    return s;
  }


  EdgesHC* GCell_Edges::getClone () const
  { return new GCell_Edges (*this); }


  EdgesHL* GCell_Edges::getLocator () const
  { return new Locator (_gcell); }


  string  GCell_Edges::_getString () const
  {
    string s = "<GCell_Edges "
             + getString(_gcell)
             + ">";
    return s;
  }


}  // Anabatic namespace.

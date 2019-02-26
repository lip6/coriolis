// -*- mode: C++; explicit-buffer-name: "Edges.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
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

  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Class  :  "Anabatic::GCell_Edges".

  GCell_Edges::Locator::Locator ( const GCell* gcell, Flags filterFlags )
    : EdgesHL()
    , _gcell      (gcell)
    , _stateFlags (Flags::EastSide)
    , _filterFlags(filterFlags)
    , _iedge      (0)
  {
    // cdebug_log(110,0) << "GCell_Edges::Locator::Locator() " << isValid() << endl;
    if (_gcell->getEastEdges().empty() or not _filterFlags.contains(Flags::EastSide)) progress();
  }


  EdgesHL* GCell_Edges::Locator::getClone () const
  {
    // cdebug_log(110,0) << "GCell_Edges::Locator::getClone()" << endl;
    return new Locator (*this);
  }


  Edge* GCell_Edges::Locator::getElement () const
  {
    if (_stateFlags.contains(Flags::EastSide )) return _gcell->getEastEdges ()[_iedge];
    if (_stateFlags.contains(Flags::NorthSide)) return _gcell->getNorthEdges()[_iedge];
    if (_stateFlags.contains(Flags::WestSide )) return _gcell->getWestEdges ()[_iedge];
    if (_stateFlags.contains(Flags::SouthSide)) return _gcell->getSouthEdges()[_iedge];
    return NULL;
  }


  bool  GCell_Edges::Locator::isValid () const
  { return _stateFlags; }


  void  GCell_Edges::Locator::progress ()
  {
    cdebug_log(110,0) << "GCell_Edges::Locator::progress() [from] " << _stateFlags << " iedge:" << _iedge << endl;
    cdebug_log(110,0) << "  _filterFlags:" << _filterFlags << endl;
    cdebug_log(110,0) << "  East:"  << _gcell->getEastEdges ().size()
                      << "  North:" << _gcell->getNorthEdges().size()
                      << "  West:"  << _gcell->getWestEdges ().size()
                      << "  South:" << _gcell->getSouthEdges().size() << endl;
    cdebug_log(110,0) << this << endl;

    ++_iedge;
    while (_stateFlags) {
      if (_stateFlags.contains(Flags::EastSide)) {
        if (   (_iedge < _gcell->getEastEdges().size())
           and _filterFlags.contains(Flags::EastSide)) break;
      //cdebug_log(110,0) << "Switching to North side." << endl;
        _stateFlags = Flags::NorthSide;
        _iedge      = 0;
        // cdebug_log(110,0) << this << endl;
        continue;
      }
      if (_stateFlags.contains(Flags::NorthSide))  {
        if (   (_iedge < _gcell->getNorthEdges().size())
           and _filterFlags.contains(Flags::NorthSide)) break;
      //cdebug_log(110,0) << "Switching to West side." << endl;
        _stateFlags = Flags::WestSide;
        _iedge      = 0;
        // cdebug_log(110,0) << this << endl;
        continue;
      }
      if (_stateFlags.contains(Flags::WestSide))  {
        if (   (_iedge < _gcell->getWestEdges().size())
           and _filterFlags.contains(Flags::WestSide)) break;
      //cdebug_log(110,0) << "Switching to South side." << endl;
        _stateFlags = Flags::SouthSide;
        _iedge      = 0;
        continue;
      }
      if (_stateFlags.contains(Flags::SouthSide))  {
        if (   (_iedge < _gcell->getSouthEdges().size())
           and _filterFlags.contains(Flags::SouthSide)) break;
      //cdebug_log(110,0) << "All edges done." << endl;
        _stateFlags = 0;
        _iedge      = 0;
        break;;
      }
    }

    cdebug_log(110,0) << "GCell_Edges::Locator::progress() [to]   " << _stateFlags << " iedge:" << _iedge << endl;
  }


  string  GCell_Edges::Locator::_getString () const
  {
    string s = "<GCell_Edges::Locator";
    if (_stateFlags.contains(Flags::EastSide )) s += " East["  + getString(_iedge) + "]";
    if (_stateFlags.contains(Flags::NorthSide)) s += " North[" + getString(_iedge) + "]";
    if (_stateFlags.contains(Flags::WestSide )) s += " West["  + getString(_iedge) + "]";
    if (_stateFlags.contains(Flags::SouthSide)) s += " South[" + getString(_iedge) + "]";
    if (_stateFlags == 0)                       s += " invalid";
    s += ">";
    return s;
  }


  EdgesHC* GCell_Edges::getClone () const
  { return new GCell_Edges (*this); }


  EdgesHL* GCell_Edges::getLocator () const
  { return new Locator (_gcell,_filterFlags); }


  string  GCell_Edges::_getString () const
  {
    string s = "<GCell_Edges "
             + getString(_gcell)
             + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::Path_Edges".

  Path_Edges::Locator::Locator ( const GCell* source, const GCell* target, Flags pathFlags )
    : EdgesHL()
    , _source    (source)
    , _target    (target)
    , _stateFlags(Flags::NoFlags)
    , _uprobe    (0)
    , _edge      (NULL)
  {
    if (_source == _target) return;
    
    Interval hoverlap = _source->getHSide().getIntersection( _target->getHSide() );
    Interval voverlap = _source->getVSide().getIntersection( _target->getVSide() );

    if (not voverlap.isEmpty()) {
      if (_source->getXMin() > _target->getXMin()) std::swap( _source, _target );
      _stateFlags |= Flags::EastSide;
      _uprobe      = voverlap.getCenter();
    } else if (not hoverlap.isEmpty()) {
      if (_source->getYMin() > _target->getYMin()) std::swap( _source, _target );
      _stateFlags |= Flags::NorthSide;
      _uprobe      = hoverlap.getCenter();
    } else {
      if (_source->getXMin() > _target->getXMin()) {
        std::swap( _source, _target );
      }

      if (_source->getYMin() < _target->getYMin()) {
        if (pathFlags & Flags::NorthPath) {
          _stateFlags |= Flags::NorthSide;
          _uprobe      = _source->getXCenter();
        } else {
          _stateFlags |= Flags::EastSide;
          _uprobe      = _source->getYCenter();
        }
      } else {
        if (pathFlags & Flags::NorthPath) {
          _stateFlags |= Flags::EastSide;
          _uprobe      = _source->getYCenter();
        } else {
          _stateFlags |= Flags::SouthSide;
          _uprobe      = _source->getXCenter();
        }
      }
    }

    _edge = _source->getEdgeAt( _stateFlags, _uprobe );
  }


  EdgesHL* Path_Edges::Locator::getClone () const
  { return new Locator (*this); }


  Edge* Path_Edges::Locator::getElement () const
  { return _edge; }


  bool  Path_Edges::Locator::isValid () const
  { return (_edge != NULL); }


  void  Path_Edges::Locator::progress ()
  {
    if (not _edge) return;

    GCell* neighbor = NULL;
    if (_stateFlags.contains(Flags::SouthSide) or _stateFlags.contains(Flags::WestSide)) neighbor = _edge->getSource();
    if (_stateFlags.contains(Flags::NorthSide) or _stateFlags.contains(Flags::EastSide)) neighbor = _edge->getTarget();

    if (neighbor == _target) { _edge = NULL; return; }

    if (_stateFlags.contains(Flags::EastSide)) {
      Interval overlap = neighbor->getHSide().getIntersection( _target->getHSide() );
      
      if (not overlap.isEmpty()) {
        overlap = neighbor->getVSide().getIntersection( _target->getVSide() );
        if (not overlap.isEmpty()) { _edge = NULL; return; }

        _stateFlags.reset( Flags::EastSide );
        _stateFlags |= (_target->getYMin() < _source->getYMin()) ? Flags::SouthSide
                                                                 : Flags::NorthSide;
        _uprobe      = overlap.getCenter();
      }
    } else if (_stateFlags.contains(Flags::SouthSide)) {
      Interval overlap = neighbor->getVSide().getIntersection( _target->getVSide() );
      
      if (not overlap.isEmpty()) {
        overlap = neighbor->getHSide().getIntersection( _target->getHSide() );
        if (not overlap.isEmpty()) {
          _edge = NULL; return; }
        
        _stateFlags.reset( Flags::SouthSide );
        _stateFlags |= Flags::EastSide;
        _uprobe      = overlap.getCenter();
      }
    } else if (_stateFlags.contains(Flags::NorthSide)) {
      Interval overlap = neighbor->getVSide().getIntersection( _target->getVSide() );
      
      if (not overlap.isEmpty()) {
        overlap = neighbor->getHSide().getIntersection( _target->getHSide() );
        if (not overlap.isEmpty()) { _edge = NULL; return; }
        
        _stateFlags.reset( Flags::NorthSide );
        _stateFlags |= Flags::EastSide;
        _uprobe      = overlap.getCenter();
      }
    }

    _edge = neighbor->getEdgeAt( _stateFlags, _uprobe );
  }


  string  Path_Edges::Locator::_getString () const
  {
    string s = "<Path_Edges::Locator @" + getString(_edge) + ">";
    return s;
  }


  EdgesHC* Path_Edges::getClone () const
  { return new Path_Edges( *this ); }


  EdgesHL* Path_Edges::getLocator () const
  { return new Locator (_source,_target,_pathFlags); }


  string  Path_Edges::_getString () const
  {
    string s = "<Path_Edges from:"
             + getString(_source) + "to:"
             + getString(_target)
             + ">";
    return s;
  }


}  // Anabatic namespace.

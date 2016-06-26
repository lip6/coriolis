// -*- mode: C++; explicit-buffer-name: "GCell.cpp<anabatic>" -*-
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
// |  C++ Module  :  "./GCell.cpp"                                   |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Contact.h"
#include "hurricane/UpdateSession.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;
  using Hurricane::UpdateSession;


  Name  GCell::_extensionName = "Anabatic::GCell";


  GCell::GCell ( AnabaticEngine* anabatic, DbU::Unit xmin, DbU::Unit ymin )
    : Super(anabatic->getCell())
    , _observable()
    , _anabatic  (anabatic)
    , _flags     (Flags::NoFlags)
    , _westEdges ()
    , _eastEdges ()
    , _southEdges()
    , _northEdges()
    , _xmin      (xmin)
    , _ymin      (ymin)
    , _contacts  ()
  { }


  void  GCell::_postCreate ()
  {
    Super::_postCreate();
    _anabatic->_add( this );
  }


  GCell* GCell::create ( AnabaticEngine* anabatic )
  {
    if (not anabatic)            throw Error( "GCell::create(): NULL anabatic argument." );
    if (not anabatic->getCell()) throw Error( "GCell::create(): AnabaticEngine has no Cell loaded." );

    GCell* gcell = new GCell ( anabatic
                             , anabatic->getCell()->getAbutmentBox().getXMin()
                             , anabatic->getCell()->getAbutmentBox().getYMin() );
    gcell->_postCreate();
    gcell->_revalidate();
    return gcell;
  }


  GCell* GCell::_create ( DbU::Unit xmin, DbU::Unit ymin )
  {
    GCell* gcell = new GCell ( getAnabatic(), xmin, ymin );
    gcell->_postCreate();
    return gcell;
  }


  GCell::~GCell ()
  { }


  void  GCell::_destroyEdges ()
  {
    while (not  _westEdges.empty()) (* _westEdges.rbegin())->destroy();
    while (not  _eastEdges.empty()) (* _eastEdges.rbegin())->destroy();
    while (not _southEdges.empty()) (*_southEdges.rbegin())->destroy();
    while (not _northEdges.empty()) (*_northEdges.rbegin())->destroy();
  }


  void  GCell::_preDestroy ()
  {
    _destroyEdges();
    _anabatic->_remove( this );
    Super::_preDestroy();
  }


  void  GCell::_remove ( Edge* edge, Flags side )
  {
    if (side.contains(Flags::WestSide )) erase_element(  _westEdges, edge );
    if (side.contains(Flags::EastSide )) erase_element(  _eastEdges, edge );
    if (side.contains(Flags::SouthSide)) erase_element( _southEdges, edge );
    if (side.contains(Flags::NorthSide)) erase_element( _northEdges, edge );
  }


  void  GCell::_add ( Edge* edge, Flags side )
  {
    cdebug_log(110,1) << "GCell::_add(side): side:" << side << " " << edge << endl;
    if (side.contains(Flags::WestSide)) {
      cdebug_log(110,0) << "Adding to West side of " << this << endl;
      for ( auto iedge=_westEdges.begin() ; iedge != _westEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _westEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _westEdges.push_back( edge );
    }

    if (side.contains(Flags::EastSide)) {
      cdebug_log(110,0) << "Adding to East side of " << this << endl;
      for ( auto iedge=_eastEdges.begin() ; iedge != _eastEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _eastEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _eastEdges.push_back( edge );
    }

    if (side.contains(Flags::SouthSide)) {
      cdebug_log(110,0) << "Adding to South side of " << this << endl;
      for ( auto iedge=_southEdges.begin() ; iedge != _southEdges.end() ; ++iedge )
        cdebug_log(110,0) << "| @" << DbU::getValueString((*iedge)->getAxisMin()) << " " << *iedge << endl;

      for ( auto iedge=_southEdges.begin() ; iedge != _southEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          cdebug_log(110,0) << "Insert *before* " << *iedge << endl;

          _southEdges.insert( iedge, edge );
          for ( auto iedge2=_southEdges.begin() ; iedge2 != _southEdges.end() ; ++iedge2 )
            cdebug_log(110,0) << "| @" <<  DbU::getValueString((*iedge2)->getAxisMin()) << " " << *iedge2 << endl;
          cdebug_tabw(110,-1);
          return;
        }
      _southEdges.push_back( edge );
    }

    if (side.contains(Flags::NorthSide)) {
      cdebug_log(110,0) << "Adding to North side of " << this << endl;
      for ( auto iedge=_northEdges.begin() ; iedge != _northEdges.end() ; ++iedge )
        if ((*iedge)->getAxisMin() > edge->getAxisMin()) {
          _northEdges.insert( iedge, edge );
          cdebug_tabw(110,-1);
          return;
        }
      _northEdges.push_back( edge );
    }

    cdebug_tabw(110,-1);
  }


  bool  GCell::hasGContact ( const Contact* owned ) const
  {
    for ( Contact* contact : _contacts ) {
      if (contact == owned) return true;
    }
    return false;
  }


  Edge* GCell::getEdgeTo ( GCell* neighbor, Flags sideHint ) const
  {
    for ( Edge* edge : getEdges(sideHint) ) {
      if (edge->getOpposite(this) == neighbor) return edge;
    }
    return NULL;
  }


  GCell* GCell::getWest  ( DbU::Unit y ) const
  {
    for ( Edge* edge : _westEdges ) {
      GCell* side = edge->getOpposite(this);
      if (y < side->getYMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getEast  ( DbU::Unit y ) const
  {
    for ( Edge* edge : _eastEdges ) {
      GCell* side = edge->getOpposite(this);
      cerr << "east @Y: " << DbU::getValueString(y) << " " << side << endl;
      if (y < side->getYMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getSouth ( DbU::Unit x ) const
  {
    for ( Edge* edge : _southEdges ) {
      GCell* side = edge->getOpposite(this);
      if (x < side->getXMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getNorth ( DbU::Unit x ) const
  {
    for ( Edge* edge : _northEdges ) {
      GCell* side = edge->getOpposite(this);
      if (x < side->getXMax()) return side;
    }
    return NULL;
  }


  GCell* GCell::getUnder ( DbU::Unit x, DbU::Unit y ) const
  {
    const GCell* current = this;

    while ( current ) {
      if (not current->isFlat() and current->getBoundingBox().contains(x,y)) break;

      if (x >= current->getXMax()) { current = current->getEast (); continue; }
      if (y >= current->getYMax()) { current = current->getNorth(); continue; }

      cerr << Error( "GCell::getUnder(): No GCell under (%s,%s), this must *never* happen."
                   , DbU::getValueString(x).c_str()
                   , DbU::getValueString(y).c_str()
                   ) << endl;
      current = NULL; break;
    }
    
    return const_cast<GCell*>( current );
  }


  Box  GCell::getBorder ( const GCell* s, const GCell* t )
  {
    Flags flags = Flags::NoFlags;
    flags |= (s->getXMax() == t->getXMin()) ? Flags::EastSide  : 0;
    flags |= (t->getXMax() == s->getXMin()) ? Flags::WestSide  : 0;
    flags |= (s->getYMax() == t->getYMin()) ? Flags::NorthSide : 0;
    flags |= (t->getYMax() == s->getYMin()) ? Flags::SouthSide : 0;

    if (flags & Flags::Vertical) {
      if (flags & Flags::Horizontal) return Box();
      if (flags & Flags::WestSide)
        return Box( s->getXMin(), std::max( s->getYMin(), t->getYMin() )
                  , s->getXMin(), std::min( s->getYMax(), t->getYMax() ) );
      else
        return Box( t->getXMin(), std::max( s->getYMin(), t->getYMin() )
                  , t->getXMin(), std::min( s->getYMax(), t->getYMax() ) );
    }

    if (flags & Flags::Horizontal) {
      if (flags & Flags::Vertical) return Box();
      if (flags & Flags::NorthSide)
        return Box( std::max( s->getXMin(), t->getXMin() ), t->getYMin()
                  , std::min( s->getXMax(), t->getXMax() ), t->getYMin() );
      else
        return Box( std::max( s->getXMin(), t->getXMin() ), s->getYMin()
                  , std::min( s->getXMax(), t->getXMax() ), s->getYMin() );
    }

    return Box();
  }


  GCell*  GCell::vcut ( DbU::Unit x )
  {
    cdebug_log(119,1) << "GCell::vcut() @x:" << DbU::getValueString(x) << " " << this << endl;

    if ( (x < getXMin()) or (x > getXMax()) )
      throw Error( "GCell::vcut(): Vertical cut axis at %s is outside GCell box,\n"
                   "        in %s."
                 , DbU::getValueString(x).c_str()
                 , getString(this).c_str()
                 );

    GCell* chunk = _create( x, getYMin() );
    cdebug_log(119,0) << "New chunk:" << chunk << endl;

    _moveEdges( chunk, 0, Flags::EastSide );
    Edge::create( this, chunk, Flags::Horizontal );

    if (not _southEdges.empty()) {
      cdebug_log(110,0) << "Split/create south edges." << endl;

      size_t iedge = 0;
      for ( ; (iedge < _southEdges.size()) ; ++iedge ) {
        cdebug_log(110,0) << "[" << iedge << "] xmax of:"
                        << _southEdges[iedge]->getOpposite(this)
                        << " " << _southEdges[iedge] << endl;
        if (x <= _southEdges[iedge]->getOpposite(this)->getXMax()) break;
      }

      if (      (x <  _southEdges[iedge]->getOpposite(this)->getXMax())
         or (   (x == _southEdges[iedge]->getOpposite(this)->getXMax())
            and (chunk->getXMax() == getXMax())) )
        Edge::create( _southEdges[iedge]->getOpposite(this), chunk, Flags::Vertical );

      _moveEdges( chunk, iedge+1, Flags::SouthSide );
    }

    if (not _northEdges.empty()) {
      cdebug_log(110,0) << "Split/create north edges." << endl;

      size_t iedge = 0;
      for ( ; (iedge < _northEdges.size()) ; ++iedge )
        if (x <= _northEdges[iedge]->getOpposite(this)->getXMax()) break;

      if (      (x <  _northEdges[iedge]->getOpposite(this)->getXMax())
         or (   (x == _northEdges[iedge]->getOpposite(this)->getXMax())
            and (chunk->getXMax() == getXMax())) )
        Edge::create( chunk, _northEdges[iedge]->getOpposite(this), Flags::Vertical );

      _moveEdges( chunk, iedge+1, Flags::NorthSide );
    }

    _revalidate();
    chunk->_revalidate();

    cdebug_tabw(119,-1);

    return chunk;
  }


  GCell* GCell::hcut ( DbU::Unit y )
  {
    cdebug_log(119,1) << "GCell::hcut() @y:" << DbU::getValueString(y) << " " << this << endl;

    if ( (y < getYMin()) or (y > getYMax()) )
      throw Error( "GCell::hcut(): Horizontal cut axis at %s is outside GCell box,\n"
                   "        in %s."
                 , DbU::getValueString(y).c_str()
                 , getString(this).c_str()
                 );

    GCell* chunk = _create( getXMin(), y );
    cdebug_log(119,0) << "New chunk:" << chunk << endl;

    _moveEdges( chunk, 0, Flags::NorthSide );
    Edge::create( this, chunk, Flags::Vertical );

    if (not _westEdges.empty()) {
      size_t iedge = 0;
      for ( ; (iedge < _westEdges.size()) ; ++iedge )
        if (y <= _westEdges[iedge]->getOpposite(this)->getYMax()) break;

      if (      (y <  _westEdges[iedge]->getOpposite(this)->getYMax())
         or (   (y == _westEdges[iedge]->getOpposite(this)->getYMax())
            and (chunk->getYMax() == getYMax())) )
        Edge::create( _westEdges[iedge]->getOpposite(this), chunk, Flags::Horizontal );

      _moveEdges( chunk, iedge+1, Flags::WestSide );
    }

    if (not _eastEdges.empty()) {
      size_t iedge = 0;
      for ( ; (iedge < _eastEdges.size()) ; ++iedge )
        if (y <= _eastEdges[iedge]->getOpposite(this)->getYMax()) break;

      if (      (y <  _eastEdges[iedge]->getOpposite(this)->getYMax())
         or (   (y == _eastEdges[iedge]->getOpposite(this)->getYMax())
            and (chunk->getYMax() == getYMax())) )
        Edge::create( chunk, _eastEdges[iedge]->getOpposite(this), Flags::Horizontal );

      _moveEdges( chunk, iedge+1, Flags::EastSide );
    }

    _revalidate();
    chunk->_revalidate();

    cdebug_tabw(119,-1);

    return chunk;
  }


  bool  GCell::doGrid ()
  {
    const vector<GCell*>& gcells = getAnabatic()->getGCells();
    size_t    ibegin = gcells.size();
    DbU::Unit side   = getAnabatic()->getConfiguration()->getSliceHeight();

    Interval hspan = getSide( Flags::Horizontal );
    Interval vspan = getSide( Flags::Vertical );

    if (hspan.getSize() < 3*side) {
      cerr << Error( "GCell::doGrid(): GCell is too narrow (dx:%s) to build a grid.\n"
                     "        (%s)"
                   , DbU::getValueString(hspan.getSize()).c_str()
                   , getString(this).c_str()
                   ) << endl;
      return false;
    }

    if (vspan.getSize() < 3*side) {
      cerr << Error( "GCell::doGrid(): GCell is too narrow (dy:%s) to build a grid.\n"
                     "        (%s)"
                   , DbU::getValueString(vspan.getSize()).c_str()
                   , getString(this).c_str()
                   ) << endl;
      return false;
    }

  //UpdateSession::open();

    GCell*    row    = this;
    GCell*    column = NULL;
    DbU::Unit ycut   = vspan.getVMin()+side;
    for ( ; ycut < vspan.getVMax() ; ycut += side ) {
      column = row;
      row    = row->hcut( ycut );

      for ( DbU::Unit xcut = hspan.getVMin()+side ; xcut < hspan.getVMax() ; xcut += side ) {
        column = column->vcut( xcut );
      }
    }

    column = row;
    for ( DbU::Unit xcut = hspan.getVMin()+side ; xcut < hspan.getVMax() ; xcut += side ) {
      column = column->vcut( xcut );
    }

    size_t hLocal = - getAnabatic()->getConfiguration()->getHEdgeLocal();
    size_t vLocal = - getAnabatic()->getConfiguration()->getVEdgeLocal();
    for ( ; ibegin < gcells.size() ; ++ibegin ) {
      for ( Edge* edge : gcells[ibegin]->getEdges(Flags::NorthSide|Flags::EastSide) ) {
        if (edge->isHorizontal()) edge->incCapacity( hLocal );
        else                      edge->incCapacity( vLocal );
      }
    }

  //UpdateSession::close();

    return true;
  }


  void  GCell::_revalidate ()
  {
    cdebug_log(110,1) << "GCell::revalidate() " << this << endl;
    cdebug_log(110,1) << "West side."  << endl; for ( Edge* edge : _westEdges  ) edge->revalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "East side."  << endl; for ( Edge* edge : _eastEdges  ) edge->revalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "South side." << endl; for ( Edge* edge : _southEdges ) edge->revalidate(); cdebug_tabw(110,-1);
    cdebug_log(110,1) << "North side." << endl; for ( Edge* edge : _northEdges ) edge->revalidate(); cdebug_tabw(110,-1);

    if (_xmin > getXMax()+1)
      cerr << Error( "GCell::_revalidate(): %s, X Min is greater than Max.", getString(this).c_str() );
    if (_ymin > getYMax()+1)
      cerr << Error( "GCell::_revalidate(): %s, Y Min is greater than Max.", getString(this).c_str() );

    _anabatic->_updateLookup( this );
  //_anabatic->getMatrix()->show();
    cdebug_tabw(110,-1);
  }


  void  GCell::_moveEdges ( GCell* dest, size_t ibegin, Flags flags )
  {
    cdebug_log(110,1) << "GCell::_moveEdges() " << this << endl;
    cdebug_log(110,0)   << "           toward " << dest << endl;
    cdebug_log(110,0)   << "           ibegin: " << ibegin << " flags:" << flags << endl;

    size_t iclear = ibegin;

    if (flags.contains(Flags::SouthSide) and not _southEdges.empty()) {
      cdebug_log(110,0) << "South side." << endl;

      if (iclear < _southEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _southEdges.size()) ; ++iedge ) {
          _southEdges[iedge]->_setTarget( dest );
          dest->_southEdges.push_back( _southEdges[iedge] );
        }
        _southEdges.resize( iclear );
      } else {
        if (iclear > _southEdges.size())
          cerr << Error("GCell::_moveEdges(): On south side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _southEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::NorthSide) and not _northEdges.empty()) {
      cdebug_log(110,0) << "North side." << endl;

      if (iclear < _northEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _northEdges.size()) ; ++iedge ) {
          _northEdges[iedge]->_setSource( dest );
          dest->_northEdges.push_back( _northEdges[iedge] );
        }
        _northEdges.resize( iclear );
      } else {
        if (iclear > _northEdges.size())
          cerr << Error("GCell::_moveEdges(): On north side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _northEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::WestSide) and not _westEdges.empty()) {
      cdebug_log(110,0) << "West side." << endl;

      if (iclear < _westEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _westEdges.size()) ; ++iedge ) {
          _westEdges[iedge]->_setTarget( dest );
          dest->_westEdges.push_back( _westEdges[iedge] );
        }
        _westEdges.resize( iclear );
      } else {
        if (iclear > _westEdges.size())
          cerr << Error("GCell::_moveEdges(): On west side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                       , iclear
                       , _westEdges.size()
                       , getString(this).c_str()
                       ) << endl;
      }
    }

    if (flags.contains(Flags::EastSide) and not _eastEdges.empty()) {
      cdebug_log(110,0) << "East side." << endl;

      if (iclear < _eastEdges.size()) {
        for ( size_t iedge=ibegin ; (iedge < _eastEdges.size()) ; ++iedge ) {
          _eastEdges[iedge]->_setSource( dest );
          dest->_eastEdges.push_back( _eastEdges[iedge] );
        }
        _eastEdges.resize( iclear );
      } else {
        if (iclear > _eastEdges.size())
           cerr << Error("GCell::_moveEdges(): On east side, iclear=%u is greater than size()-1=%u\n"
                        "        (%s)"
                        , iclear
                        , _eastEdges.size()
                        , getString(this).c_str()
                        ) << endl;
      }
    }

    cdebug_tabw(110,-1);
  }


  Contact* GCell::getGContact ( Net* net )
  {

    for ( Contact* contact : _contacts ) {
      if (contact->getNet() == net) {
        cdebug_log(111,0) << "GCell::getGContact(): " << contact << endl;
        return contact;
      }
    }

    Point    center  = getBoundingBox().getCenter();
    Contact* contact = Contact::create( net
                                      , _anabatic->getConfiguration()->getGContactLayer()
                                      , center.getX()
                                      , center.getY()
                                      , DbU::fromLambda(2.0)
                                      , DbU::fromLambda(2.0)
                                      );
    _contacts.push_back( contact );
    cdebug_log(111,0) << "GCell::getGContact(): " << contact << endl;
    return contact;
  }


  bool  GCell::unrefContact ( Contact* unref )
  {
    if (_contacts.empty()) return false;

    for ( size_t i=0 ; i< _contacts.size() ; ++i ) {
      if (_contacts[i] == unref) {
        if (_contacts[i]->getSlaveComponents().getLocator()->isValid()) return false;
        
        std::swap( _contacts[i], _contacts[_contacts.size()-1] );
        _contacts[ _contacts.size()-1 ]->destroy();
        _contacts.pop_back();

        return true;
      }
    }

    return false;
  }


  const Name& GCell::getName () const
  { return _extensionName; }


  Box  GCell::getBoundingBox () const
  {
    return Box( getXMin(), getYMin(), getXMax(1), getYMax(1) );
  }


  void  GCell::translate ( const DbU::Unit&, const DbU::Unit& )
  {
    cerr << Error( "GCell::translate(): On %s,\n"
                   "        Must never be called on a GCell object (ignored)."
                 , getString(this).c_str()
                 ) << endl;
  }


  string  GCell::_getTypeName () const
  { return getString(_extensionName); }


  string  GCell::_getString () const
  {
    string s = Super::_getString();
    s.insert( s.size()-1, " "+getString(getBoundingBox()) );
    s.insert( s.size()-1, " "+getString(_flags) );
    return s;
  }


  Record* GCell::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_flags"     , &_flags     ) );
    record->add( getSlot("_westEdges" , &_westEdges ) );
    record->add( getSlot("_eastEdges" , &_eastEdges ) );
    record->add( getSlot("_southEdges", &_southEdges) );
    record->add( getSlot("_northEdges", &_northEdges) );
    record->add( DbU::getValueSlot("_xmin", &_xmin) );
    record->add( DbU::getValueSlot("_ymin", &_ymin) );
    return record;
  }



}  // Anabatic namespace.
